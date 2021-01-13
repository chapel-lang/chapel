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
  YYSYMBOL_call_base_expr = 315,           /* call_base_expr  */
  YYSYMBOL_call_expr = 316,                /* call_expr  */
  YYSYMBOL_dot_expr = 317,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 318,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 319,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 320,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 321,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 322,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 323,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 324,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 325,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 326,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 327       /* reduce_scan_op_expr  */
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

#line 889 "bison-chapel.cpp"

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
#define YYLAST   19372

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  175
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  153
/* YYNRULES -- Number of rules.  */
#define YYNRULES  644
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1155

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
    2142,  2143,  2144,  2145,  2154,  2155,  2156,  2157,  2158,  2162,
    2163,  2164,  2168,  2169,  2170,  2171,  2172,  2180,  2181,  2182,
    2183,  2187,  2188,  2192,  2193,  2197,  2198,  2199,  2200,  2201,
    2202,  2203,  2204,  2206,  2208,  2209,  2210,  2214,  2222,  2223,
    2227,  2228,  2229,  2230,  2231,  2232,  2233,  2234,  2235,  2236,
    2237,  2238,  2239,  2240,  2241,  2242,  2243,  2244,  2245,  2246,
    2247,  2248,  2249,  2254,  2255,  2256,  2257,  2258,  2259,  2260,
    2264,  2265,  2266,  2267,  2271,  2272,  2273,  2274,  2279,  2280,
    2281,  2282,  2283,  2284,  2285
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
     425,   426,   427,   428,   429
};
#endif

#define YYPACT_NINF (-1017)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-595)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1017,   105,  3028, -1017,   -44, -1017, -1017, -1017, -1017, -1017,
   -1017, -1017,  4380,    54,   141, -1017, 13698, -1017, 18966,    54,
   10605, -1017,   590,   179,   141,  4380, 10605,  4380,    44, 19016,
   10772,  7750, -1017,  7917,  8927,  6406, 10605, -1017, -1017,   139,
   -1017, -1017, 19066, -1017,  2200,  9094, -1017, 10605,   203, -1017,
   -1017, -1017, 10605, -1017, 13698, -1017, 10605,   265,   198,   188,
   19263, -1017, 19116, -1017,  9263,  7247, 10605,  9094, 13698, 10605,
     283, -1017,   236,  4380, -1017, 10605, -1017, 10939, 10939, 19066,
   -1017, -1017, 13698, -1017, -1017, 10605, 10605, -1017, 10605, -1017,
   10605, -1017, -1017, 13402, 10605, -1017, 10605, -1017, -1017, -1017,
    3366,  6575,  8086, 10605, -1017,  4211, -1017,   230, -1017,   333,
   -1017, -1017,     1, -1017, -1017, -1017, -1017, -1017, -1017, -1017,
   -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017,
   -1017, -1017, 19066, -1017, 19066,   384,   177, -1017, -1017,  2200,
   -1017,   259, -1017,   282, -1017, -1017,   291,   306,   348, 10605,
     347,   376, 18477,  2227,   220,   383,   394, -1017, -1017,   379,
   -1017, -1017, -1017, -1017, -1017,   229, -1017, -1017, 18477,   275,
    4380, -1017, -1017,   401, 10605, -1017, -1017, 10605, 10605, 10605,
   -1017, 10605,  9263,  9263,   395, -1017, -1017, -1017, -1017,   183,
     429, -1017, -1017,   343, 15179, 19066,  2200, -1017,   406, -1017,
     107, 18477,   436,  7416,   484, 19166, 18477,   552,   486, -1017,
   19216, 19066,   552, 19066,   403,    36, 14843,    11, 14767,   370,
   -1017, 14933,  2836,   238,  7416, -1017, -1017, -1017, -1017, -1017,
   -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017,
   -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017,  4380,
   -1017,   412,  1699,   252,    14, -1017,  4380, -1017, -1017, 15255,
      97, -1017,   419,   433, -1017, 15255,   183,    97, -1017,  7416,
    1800, -1017, -1017,  9430, -1017, -1017, -1017, -1017,   168, 18477,
   10605, 10605, -1017, 18477,   447, 15803, -1017, 15255,   183, 18477,
     452,  7416, -1017, 18477, 15879, -1017, -1017, 15959,  2778, -1017,
   -1017, 16039,   505,   469,   183, 15255, 16119,   390,   390,  1473,
      97,    97,   -49, -1017, -1017,  3535,   -46, -1017, 10605, -1017,
      48,    85, -1017,   -21,   -19, 16199,   -33,  1473,   624, -1017,
    3704, -1017,   572, 10605, 10605,   502,   478, -1017, -1017, -1017,
     268,   439, -1017, 10605,   507, 10605, 10605, 10605,  8927,  8927,
   10605,   480, 10605, 10605, 10605, 10605, 10605,   425, 13402, 10605,
   10605, 10605, 10605, 10605, 10605, 10605, 10605, 10605, 10605, 10605,
   10605, 10605, 10605, 10605, 10605,   587, -1017, -1017, -1017, -1017,
   -1017,  8253,  8253, -1017, -1017, -1017, -1017,  8253, -1017, -1017,
    8253,  8253,  7416,  7416,  8927,  8927,  7080, -1017, -1017, 15335,
   15411, 16275,    57,  3873, -1017,  8927,    36,   494,   340, -1017,
   10605, -1017, 10605,   537, -1017,   490,   519, -1017, -1017, -1017,
   19066, -1017,  2200, -1017, 19066,   501, -1017,  2200,   626,  9263,
   -1017,  4549,  8927, -1017,   504, -1017,    36,  4718,  8927, -1017,
      36, 11106, 10605, -1017,  4380, -1017,   641, 10605, -1017, -1017,
     333,   509, -1017,   363, -1017, -1017,  1699, -1017,   538,   510,
   -1017, 11273,   560, 10605,  2200, -1017, -1017, 10605, 10605, -1017,
     513, -1017,  9263, -1017, 18477, 18477, -1017,    30, -1017,  7416,
     516, -1017,   669, -1017,   669, -1017, 11440,   551, -1017, -1017,
   -1017, -1017, -1017, -1017,  8422, -1017, 16355,  6744, -1017,  6913,
   -1017,  4380,   523,  8927,  8591,  3197,   525, 10605,  9597, -1017,
   -1017,   111, -1017,  4042, 19066, -1017,   341, 16431,   375, 15023,
    9263,   531, 18916,   312, -1017, 16511, 18666, 18666,   488, -1017,
     488, -1017,   488, 18761,  2067,   677,  1264,   183,   390,   530,
   -1017, -1017, -1017, -1017,  1473,  2527,   488, 14982, 14982, 18666,
   14982, 14982,  1043,   390,  2527, 18723,  1043,    97,    97,   390,
    1473,   539,   543,   545,   546,   547,   567,   561,   528, -1017,
     488, -1017,   488,    68, -1017, -1017, -1017,   161, -1017, 13205,
   18553,   318, 11607,  8927, 11774,  8927, 10605,  8927, 13962,    54,
   16587, -1017, -1017, -1017, 18477, 16667,  7416, -1017,  7416, 19066,
     502,   378, 19066,   502, -1017,   387, 10605,   249, 10605, 18477,
      60, 15491,  7080, -1017, 10605, 18477,    15, 15099, -1017,   570,
     579,   571, 16747,   579,   574,   676, 16823,  4380, 15567,   670,
   -1017,   170, -1017, -1017, -1017, -1017, -1017, -1017,   674,   267,
   -1017,  2528, -1017,   410,   568,  1699,   252,    44,   139, 10605,
   10605,  6239, -1017, -1017,   666,  7583, -1017, 18477, -1017, 16903,
   16979, -1017, -1017, 18477,   569,    29,   575, -1017,  1967, -1017,
   -1017,   400, 19066, -1017, -1017, -1017, -1017, -1017, -1017, -1017,
    4380,   -14, 15647, -1017, -1017, 18477,  4380, 18477, -1017, 17060,
   -1017, -1017, -1017, 10605, -1017,    61,    84, 10605, -1017,  9764,
   11106, 10605,   597,  1279,   573,   627,   122, -1017,   673, -1017,
   -1017, -1017, -1017, 13240,   583, -1017, -1017, -1017, -1017, -1017,
   -1017, -1017, -1017, -1017, -1017,  7080, -1017,    37,  8927,  8927,
   10605,   721, 17136, 10605,   729, 17216,   593, 17292,    36,    36,
   -1017, -1017, -1017, -1017,   502,   602, -1017,   502,   603, -1017,
   15255, -1017, 14143,  4887, -1017,  5056, -1017,   302, -1017, 14219,
    5225, -1017,    36,  5394, -1017,    36, 10605, -1017, 10605, -1017,
    4380, 10605, -1017,  4380,   735, 19066, -1017, -1017, 19066,   960,
   -1017,  1699,   633,   689, -1017, -1017, -1017,   125, -1017, -1017,
     560,   605,    65, -1017, -1017,   610,   614, -1017,  5563,  9263,
   -1017, -1017, -1017, 19066, -1017,   642,   343, -1017, -1017,  5732,
     615,  5901,   617, -1017, 10605, -1017, -1017, 10605, 17372,    77,
   15723,   629,   630,   618,  1405, -1017, 10605, 19066, -1017, -1017,
     410,   625,   231, -1017,   648, -1017,   650,   656,   667,   665,
     668, -1017,   671,   675,   679,   680,   685,   234,   694,   690,
     693, -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017,
   -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017, -1017,
   -1017, -1017, -1017, 10605, -1017,   702,   706,   699,   625,   625,
   -1017, -1017, -1017,   560,   304,   309, 17448, 11941, 12108, 17528,
   12275, 12442, 12609, 12776, -1017, -1017,   644, -1017,   678, -1017,
   -1017,  4380, 10605, 18477, 10605, 18477,  7080, -1017,  4380, 10605,
   18477, -1017, 10605, 18477, -1017, 17604, 18477, -1017, 18477,   809,
    4380,   686,   633, -1017,   681,  9933,   158, -1017,    45, -1017,
   -1017,  8927, 13830,  7416,  7416,  4380, -1017,    53,   682, 10605,
   -1017, 10605, -1017, 18477,  4380, 10605, -1017, 18477,  4380, 18477,
     146, 10100, 11106, 10605, 11106, 10605, -1017, -1017, -1017, -1017,
    1800, -1017, 14066, -1017, 18477, -1017,   133,   363, -1017, 17684,
   -1017, 14061, -1017, -1017, -1017, 10605, 10605, 10605, 10605, 10605,
   10605, 10605, 10605, -1017,  2294, -1017,  2391, 16823, 14299, 14375,
   -1017, 16823, 14455, 14531, 10605,  4380, -1017, -1017,   158,   633,
    8760, -1017, -1017, -1017,   159,  9263, -1017, -1017,    56, 10605,
     -37, 17760, -1017,   861,   683,   691,   436, -1017,   343, 18477,
   14611, -1017, 14687, -1017, -1017, -1017, 18477,   687,   688,   700,
     701, -1017, -1017, 12943,   753,   319, -1017,   719,   730,   625,
     625, 17840, 17916, 17996, 18072, 18152, 18228,  2786, -1017, 18690,
   -1017,  4380,  4380,  4380,  4380, 18477, -1017, -1017, -1017,   158,
   10269,    66, -1017, 18477, -1017,    64, -1017,   118, -1017,   464,
   18321, -1017, -1017, -1017, 12776,   707,   708, -1017,  4380,  4380,
   -1017, -1017, -1017, -1017,  6070, -1017, -1017,   208, -1017,    45,
   -1017, -1017, -1017, 10605, 10605, 10605, 10605, 10605, 10605, -1017,
   -1017, 16823, 16823, 16823, 16823, -1017, -1017, -1017, -1017, -1017,
     255,  8927,  2707, -1017, 10605,    56,    64,    64,    64,    64,
      64,    64,    56,  1004, -1017, -1017, 16823, 16823,   698, 13110,
      87,   136, 18397, -1017, -1017, 18477, -1017, -1017, -1017, -1017,
   -1017, -1017, -1017,   728, -1017, -1017,   481, 13566, -1017, -1017,
   -1017, 10438, -1017,   491, -1017
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   111,   587,   588,   589,   583,
     584,   590,     0,   509,    97,   132,   478,   139,   480,   509,
       0,   138,     0,   383,    97,     0,     0,     0,     0,   133,
     555,   555,   581,     0,     0,     0,     0,   137,    53,     0,
     260,   134,     0,   256,     0,     0,   141,     0,   528,   500,
     591,   142,     0,   261,   472,   382,     0,     0,     0,   155,
      46,   136,   481,   384,     0,     0,     0,     0,   476,     0,
       0,   140,     0,     0,   112,     0,   582,     0,     0,     0,
     135,   242,   474,   386,   143,     0,     0,   640,     0,   642,
       0,   643,   644,   554,     0,   641,   638,   459,   152,   639,
       0,     0,     0,     0,     4,     0,     5,     0,     9,    48,
      10,    11,     0,    12,    13,    14,    16,   455,   456,    24,
      15,   153,   162,   163,    17,    19,    18,    21,    22,    23,
      20,   161,     0,   159,     0,   544,     0,   157,   160,     0,
     158,   560,   540,   457,   541,   462,   460,     0,     0,     0,
     545,   546,     0,   461,     0,   561,   562,   563,   585,   586,
     539,   464,   463,   542,   543,     0,    40,    26,   470,     0,
       0,   510,    98,     0,     0,   480,   133,     0,     0,     0,
     481,     0,     0,     0,   544,   560,   460,   545,   546,   479,
     461,   561,   562,     0,   509,     0,     0,   385,     0,   216,
       0,   440,     0,   447,   583,   481,   556,   259,   583,   180,
     481,     0,   259,     0,     0,     0,     0,     0,     0,     0,
     167,     0,     0,    50,   447,   105,   113,   125,   119,   118,
     127,   108,   117,   128,   114,   129,   106,   130,   123,   116,
     124,   122,   120,   121,   107,   109,   115,   126,   131,     0,
     110,     0,     0,     0,     0,   389,     0,   149,    34,     0,
     625,   532,   529,   530,   531,     0,   473,   626,     7,   447,
     259,   240,   250,   555,   241,   154,   354,   437,     0,   436,
       0,     0,   150,   559,     0,     0,    37,     0,   477,   465,
       0,   447,    38,   471,     0,   223,   219,     0,   461,   223,
     220,     0,   379,     0,   475,     0,     0,   627,   629,   552,
     624,   623,     0,    55,    58,     0,     0,   442,     0,   444,
       0,     0,   443,     0,     0,   436,     0,   553,     0,     6,
       0,    49,     0,     0,     0,   243,     0,   341,   340,   262,
       0,   458,    25,     0,   533,     0,     0,     0,     0,     0,
       0,   628,     0,     0,     0,     0,     0,     0,   551,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   299,   306,   307,   308,   303,
     305,     0,     0,   301,   304,   302,   300,     0,   310,   309,
       0,     0,   447,   447,     0,     0,     0,    27,    28,     0,
       0,     0,     0,     0,    29,     0,     0,     0,     0,    30,
       0,    31,     0,   455,   453,     0,   448,   449,   454,   174,
       0,   177,     0,   170,     0,     0,   176,     0,     0,     0,
     189,     0,     0,   188,     0,   197,     0,     0,     0,   195,
       0,     0,    70,   164,     0,   299,   209,     0,    52,    51,
      48,     0,    32,   323,   257,   393,     0,   394,   396,     0,
     418,     0,   399,     0,     0,   148,    33,     0,     0,    35,
       0,   156,     0,    90,   557,   558,   151,     0,    36,   447,
       0,   230,   221,   217,   222,   218,     0,   377,   374,   182,
      39,    57,    56,    59,     0,   592,     0,     0,   577,     0,
     579,     0,     0,     0,     0,     0,     0,     0,     0,   596,
       8,     0,    42,     0,     0,    88,     0,    85,     0,    64,
       0,     0,     0,   334,   388,   508,   620,   619,   622,   631,
     630,   635,   634,   616,   613,   614,   615,   548,   603,     0,
     574,   575,   573,   572,   549,   607,   618,   612,   610,   621,
     611,   609,   601,   606,   608,   617,   600,   604,   605,   602,
     550,     0,     0,     0,     0,     0,     0,     0,     0,   633,
     632,   637,   636,   520,   521,   523,   525,     0,   512,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   594,   509,
     509,   185,   375,   387,   441,     0,     0,   467,     0,     0,
     243,     0,     0,   243,   376,     0,     0,     0,     0,   484,
       0,     0,     0,   198,     0,   490,     0,     0,   196,   639,
      73,     0,    60,    71,     0,   208,     0,     0,     0,     0,
     466,   328,   325,   326,   327,   331,   332,   333,   323,     0,
     315,     0,   324,   342,     0,   397,     0,   146,   147,   145,
     144,     0,   417,   416,   540,     0,   391,   538,   390,     0,
       0,   571,   439,   438,     0,     0,     0,   468,     0,   224,
     381,   540,     0,   593,   547,   578,   445,   580,   446,   205,
       0,     0,     0,   595,   203,   494,     0,   598,   597,     0,
      44,    43,    41,     0,    84,     0,     0,     0,    77,     0,
       0,    70,   244,     0,     0,   254,     0,   251,   338,   335,
     336,   339,   263,     0,     0,    96,    94,    95,    93,    92,
      91,   569,   570,   522,   524,     0,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     181,   451,   452,   450,   243,     0,   179,   243,     0,   178,
       0,   207,     0,     0,   187,     0,   186,     0,   515,     0,
       0,   193,     0,     0,   191,     0,     0,   165,     0,   166,
       0,     0,   211,     0,   213,     0,   329,   330,     0,   323,
     313,     0,   431,   343,   346,   345,   347,     0,   395,   398,
     399,     0,     0,   400,   401,     0,     0,   232,     0,     0,
     231,   234,   469,     0,   225,   228,     0,   378,   206,     0,
       0,     0,     0,   204,     0,    89,    86,     0,    67,    66,
      65,     0,     0,     0,   259,   249,     0,   252,   248,   337,
     342,   311,    99,   294,   113,   292,   119,   118,   102,   117,
     114,   297,   129,   100,   130,   116,   120,   101,   103,   115,
     131,   291,   273,   276,   274,   275,   286,   277,   290,   282,
     280,   293,   296,   281,   279,   284,   289,   278,   283,   287,
     288,   285,   295,     0,   272,     0,   104,     0,   311,   311,
     270,   576,   513,   399,   560,   560,     0,     0,     0,     0,
       0,     0,     0,     0,   184,   183,     0,   245,     0,   245,
     190,     0,     0,   483,     0,   482,     0,   514,     0,     0,
     489,   194,     0,   488,   192,    62,    61,   210,   499,   212,
       0,     0,   431,   316,     0,     0,   399,   344,   358,   392,
     422,     0,   594,   447,   447,     0,   236,     0,     0,     0,
     226,     0,   201,   496,     0,     0,   199,   495,     0,   599,
       0,     0,     0,    70,     0,    70,    78,    81,   239,   155,
     259,   238,   259,   246,   255,   253,     0,   323,   269,     0,
     298,     0,   265,   266,   517,     0,     0,     0,     0,     0,
       0,     0,     0,   245,   259,   245,   259,   487,     0,     0,
     516,   493,     0,     0,     0,     0,   215,    54,   399,   431,
       0,   434,   433,   435,   540,   355,   319,   317,     0,     0,
       0,     0,   420,   540,     0,     0,   237,   235,     0,   229,
       0,   202,     0,   200,    87,    69,    68,     0,     0,     0,
       0,   247,   413,     0,   348,     0,   271,    99,   101,   311,
     311,     0,     0,     0,     0,     0,     0,   259,   173,   259,
     169,     0,     0,     0,     0,    63,   214,   320,   318,   399,
     423,     0,   357,   356,   372,     0,   373,   360,   363,     0,
     359,   352,   353,   258,     0,   534,   535,   227,     0,     0,
      80,    83,    79,    82,     0,   412,   411,   540,   349,   358,
     312,   267,   268,     0,     0,     0,     0,     0,     0,   175,
     171,   486,   485,   492,   491,   322,   321,   425,   426,   428,
     540,     0,   594,   371,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   540,   536,   537,   498,   497,     0,   403,
       0,     0,     0,   427,   429,   362,   364,   365,   368,   369,
     370,   366,   367,   361,   408,   406,   540,   594,   350,   264,
     351,   423,   407,   540,   430
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1017, -1017, -1017,     4,  -376,  2079, -1017, -1017, -1017,   420,
   -1017, -1017, -1017,   246,   541,  -430, -1017,  -673,  -664, -1017,
   -1017, -1017,   184, -1017, -1017,   432,   848, -1017,   -10,  -194,
    -697, -1017,  -884,   339,  -959,  -766, -1017,   -57, -1017, -1017,
   -1017, -1017, -1017, -1017, -1017, -1017,   126, -1017, -1017, -1017,
     584, -1017,    79, -1017, -1017, -1017, -1017, -1017, -1017,  -555,
    -653, -1017, -1017, -1017,    58,  1093, -1017, -1017, -1017,  -106,
   -1017, -1017, -1017, -1017,   -87,  -151,  -799, -1017,   -81,   108,
     250, -1017, -1017, -1017,    63, -1017, -1017,  -240,   228,  -927,
    -191,  -227,  -216,  -452, -1017,  -182, -1017,    -5,   867,  -113,
     440, -1017,  -436,  -775,  -983, -1017,  -609,  -479, -1016,  -974,
    -860,   -63, -1017,   -27, -1017,  -219, -1017,   307,   514,  -278,
   -1017, -1017, -1017,  1115, -1017,    -8, -1017, -1017,  -193, -1017,
    -602, -1017, -1017, -1017,  1145,  1490,   -12,   876,   413,   899,
   -1017,  1682,  1874, -1017, -1017, -1017, -1017, -1017, -1017, -1017,
   -1017, -1017,  -362
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   314,   105,   625,   107,   108,   109,   332,
     450,   110,   223,   111,   315,   620,   518,   624,   621,   112,
     113,   114,   515,   516,   115,   116,   173,   874,   253,   117,
     250,   118,   652,   258,   119,   120,   270,   121,   122,   123,
     424,   602,   420,   599,   124,   125,   736,   126,   127,   128,
     482,   669,   806,   129,   130,   665,   801,   131,   132,   521,
     824,   133,   134,   706,   707,   184,   251,   643,   136,   137,
     523,   830,   712,   877,   878,   447,   968,   454,   639,   640,
     641,   642,   713,   339,   787,  1089,  1149,  1073,   277,  1006,
    1010,  1067,  1068,  1069,   138,   303,   487,   139,   140,   254,
     255,   458,   459,   656,  1086,  1034,   462,   653,  1108,  1003,
     926,   316,   200,   320,   321,   415,   416,   417,   185,   142,
     143,   144,   145,   186,   147,   170,   171,   577,   436,   757,
     578,   579,   148,   149,   187,   188,   152,   212,   418,   190,
     154,   191,   192,   157,   158,   159,   160,   326,   161,   162,
     163,   164,   165
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     168,   278,   391,   275,   189,   451,   104,   670,   194,   413,
     758,   193,   623,   407,   201,   929,   876,   196,   206,   206,
     644,   216,   218,   221,   222,   440,   340,   503,   822,   425,
     413,   664,   249,   259,   581,   260,   821,   790,   460,   324,
     265,  1001,   266,   256,   267,   745,   333,   760,   748,    65,
     470,  1072,   279,   283,   285,   287,   288,   289,   963,  1064,
     463,   293,   998,   294,     5,   297,   301,  1064,   256,   302,
     304,  1057,   480,   305,   306,   413,   307,   638,   308,   972,
     973,   309,   310,   408,   311,   256,  1109,     5,   279,   283,
     325,   327,   753,   434,   494,   434,  1133,   413,   798,  -233,
     434,   421,  1008,   587,   539,     3,   426,   508,   974,   334,
     166,   931,  1111,   952,   723,   166,   491,   540,   883,   495,
     402,   504,   335,   882,   336,   434,   460,   403,   347,   434,
     502,   506,  1105,   587,  1032,  1154,   509,   344,  1134,  1059,
     799,  -233,  1065,   541,   172,   953,  1145,   391,   501,  1085,
     505,  1007,   724,  1066,   464,   809,  -414,  1009,    74,   215,
     217,  1066,   168,   543,  1152,   399,   400,   401,   169,   293,
     279,   325,  1072,   567,   568,   403,   461,  -414,   928,   403,
     348,    74,  -414,   654,   349,   302,   406,   542,   497,   896,
    -409,   283,   898,   472,   800,  -233,  1031,   504,   413,   413,
     403,   302,   422,   428,  -414,   504,   504,   427,   671,   789,
     203,  -414,   283,   471,   196,   498,   776,   403,   963,  -409,
     963,   261,    65,  1058,   403,   499,   588,   504,   323,   337,
    1114,   351,  -409,   -76,   932,  1112,   355,  -414,   356,   357,
    1091,  1092,   457,   360,   984,  -409,   986,   410,   817,   -47,
     581,   367,   500,   460,   777,   338,  1147,   283,  1115,   373,
     666,   206,   827,   197,   461,  -410,   411,   -47,   474,   475,
     166,   623,  1033,   268,   876,   262,   690,   655,  -432,   283,
    1028,   963,  1030,   963,  1106,   413,   766,   828,  1027,  -414,
    1029,   257,  -409,   263,  -410,  1148,   -76,  -409,  -414,  -432,
     403,   725,  1005,  -432,   990,   224,   496,  -410,   472,   601,
     264,  1024,   394,   257,   605,   448,   395,   351,   449,   493,
    -410,   517,   519,   295,   299,   357,  -432,   473,   726,   319,
    1047,   525,  1049,   526,   527,   528,   530,   532,   533,   708,
     534,   535,   536,   537,   538,   924,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   581,   269,  -105,   607,  -410,  -107,   283,
     283,  -414,  -410,  -111,  -424,   283,  -112,   290,   283,   283,
     283,   283,   570,   572,   580,   709,   392,   950,   393,   472,
     631,   461,  -414,   590,   330,  -424,   710,    23,   594,  -424,
     595,   729,   291,   638,   413,   395,   286,   779,   464,   632,
     600,  -414,   331,   633,   603,   711,   751,   279,  -502,   609,
     611,   347,  -424,   762,   765,   615,   617,   524,     5,   622,
     622,   414,   626,   634,   780,   628,   635,   783,    57,   404,
     341,   396,   906,    55,  -519,   539,   457,   636,   962,  -518,
    -501,   657,   414,  1012,    63,   659,   660,   702,   540,   779,
     663,   257,   257,   257,   257,   342,   637,   283,   446,   907,
     676,  -519,   678,   348,   529,   531,  -518,   349,   284,    83,
     464,   693,   663,   784,   541,   283,  1090,   283,   810,   812,
     807,   682,   663,   685,   785,   687,   689,   414,   343,   593,
     694,   816,   543,  -314,   692,   257,  -506,   403,   279,   296,
     300,   257,   705,   786,   322,   697,   141,   493,   464,   414,
     569,   571,    74,   623,   351,   623,   141,   464,   542,   355,
    -314,   589,   357,   257,   698,  -507,   360,   746,  -402,   141,
    -380,   141,  -504,  -505,   581,  -568,   749,  -568,  -404,   412,
    -567,   257,  -567,  -503,   430,   433,   435,   439,   610,  -380,
     398,  -567,   879,  -567,   616,   409,   419,  -402,   423,   429,
     609,   732,   615,   735,   626,   737,   685,  -404,   453,    23,
    -402,   738,   739,  1116,   283,   467,   283,   141,   792,   744,
    -404,   638,   747,  -402,   750,  -564,   752,  -564,   466,   468,
     580,    40,   759,  -404,   469,  -566,   476,  -566,   962,  1117,
     962,  1118,  1119,  1113,   141,  1120,  1121,    23,   479,   141,
     414,   414,   351,    53,   486,    55,   478,   355,   488,   681,
     357,   782,   510,   514,   360,   457,    63,   289,   293,   325,
    -402,   520,   522,   283,   489,  -402,  -565,  1004,  -565,   357,
    -404,   561,   257,   592,  1013,  -404,   596,   597,   805,   598,
     604,    83,   302,    55,  1137,  1138,  1139,  1140,  1141,  1142,
     612,   962,   606,   962,    63,   627,   630,   646,   645,   655,
     661,   517,   257,   667,   141,   818,   257,   820,   622,   622,
     668,   672,   680,   195,   686,   703,   714,   722,   715,    83,
     662,   631,   716,   875,   717,   718,   719,   414,   347,   731,
     770,   734,  -415,   580,  1014,  1015,   530,   570,   886,   766,
     632,   889,   662,  -415,   633,   319,   720,   319,   721,   -72,
     767,   775,   662,   769,   797,   788,   937,   472,   825,   413,
     413,   903,   802,   905,   634,   591,   826,   635,   910,   328,
     881,   913,  -415,   887,   915,  1087,   916,   829,   636,   918,
     348,   890,   892,   141,   349,   921,   897,   899,   922,   920,
     141,   457,   925,   927,   930,   613,   933,   637,  -415,   618,
     934,   939,  1110,   958,   944,  -415,   948,   279,   956,   957,
    -132,   967,  -139,   805,   562,   563,  1123,   943,  -138,   947,
     564,  -108,   949,   565,   566,   622,  -415,  -137,   983,  -106,
    -134,   351,   352,  -141,   964,   354,   355,   705,   356,   357,
    1040,  -142,  -136,   360,   741,  -415,   414,  -140,  -109,   141,
    -415,   367,  -135,  -415,  1110,  -143,   970,   371,   372,   373,
    -110,   971,   985,   995,   141,   997,  1080,  1081,   999,  1018,
    1075,  1146,  1088,  -105,   529,   569,   257,   257,  1076,  1082,
    1083,   969,   257,   257,  -107,  1124,  1125,  1144,  1115,  1153,
     629,   513,   198,  1110,   774,   903,   905,   815,   910,   913,
     943,   947,   938,   484,  1039,   965,  1035,   923,   778,   987,
     988,  1150,   989,   966,   580,  1143,   991,   992,  1131,  1136,
     993,   153,   220,   275,   658,   743,   207,  -419,     0,     0,
     322,   153,   322,     0,   804,     0,     0,   141,  -419,  1011,
     685,   283,   283,     0,   153,     0,   153,  1019,     0,  1020,
       0,     0,     0,  1022,     0,     0,     0,  1061,     0,  1026,
     622,   622,   622,   622,     0,   141,     0,  -419,     0,   754,
     756,   141,     0,     0,     0,   761,   764,     0,   141,     0,
       0,   875,     0,   987,  1041,  1042,   991,  1043,  1044,  1045,
    1046,     0,   153,  -419,     0,     0,   298,   298,     0,     0,
    -419,     0,  1055,     0,   257,   257,     0,   631,   325,     0,
       0,     0,     0,  1063,     0,     0,   257,  1070,   446,   153,
       0,  -419,     0,     0,   153,   446,   632,     0,   257,   742,
     633,   257,     0,     0,     0,   141,     0,     0,     0,   141,
    -419,  1130,     0,     0,     0,  -419,     0,   141,  -419,     0,
     634,     0,     0,   635,     0,    57,     0,     0,     0,  1101,
    1102,  1103,  1104,     0,   636,     0,     0,     0,     0,     0,
    -421,     0,   940,     0,     0,     0,     0,     0,     0,     0,
       0,  -421,   947,   637,     0,     0,  1126,  1127,   794,   153,
       0,     0,   325,     0,   347,     0,     0,   894,   895,     0,
       0,  1101,  1102,  1103,  1104,  1126,  1127,     0,     0,   900,
    -421,     0,     0,   727,     0,   135,     0,     0,     0,  1132,
     685,   911,  1135,     0,   914,   135,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -421,   146,   135,     0,
     135,     0,     0,  -421,     0,     0,   348,   146,     0,     0,
     349,     0,     0,     0,     0,   685,     0,     0,     0,   947,
     146,   141,   146,     0,  -421,     0,     0,   150,   153,     0,
       0,     0,     0,  1002,     0,   153,     0,   150,     0,     0,
       0,   414,   414,  -421,     0,     0,   135,     0,  -421,     0,
     150,  -421,   150,     0,     0,     0,     0,   351,   352,     0,
     353,   354,   355,   257,   356,   357,     0,     0,   146,   360,
       0,     0,     0,   135,   141,     0,     0,   367,   135,     0,
     141,     0,     0,   371,   372,   373,     0,     0,     0,     0,
       0,     0,     0,     0,   153,   146,     0,     0,   150,     0,
     146,     0,     0,     0,     0,     0,     0,   880,     0,   153,
       0,     0,     0,  1062,   446,   446,     0,     0,   446,   446,
       0,     0,   884,   885,     0,   150,     0,     0,     0,     0,
     150,     0,     0,     0,     0,     0,  1071,     0,     0,     0,
       0,     0,     0,   135,  1077,     0,   446,   141,   446,   141,
       0,     0,     0,     0,   141,     0,  1017,   141,     0,     0,
     823,     0,     0,     0,   141,   146,     0,   141,  1107,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,  -245,   153,     0,     0,  -245,  -245,     0,     0,     0,
       0,     0,   141,     0,  -245,   150,  -245,  -245,     0,     0,
       0,  -245,     0,   141,     0,   141,     0,     0,  -245,     0,
     153,  -245,     0,     0,     0,     0,   153,     0,     0,     0,
    1107,     0,   135,   153,     0,     0,     0,   348,     0,   135,
    -245,   349,  -245,     0,  -245,     0,  -245,  -245,     0,  -245,
       0,  -245,     0,  -245,   146,     0,     0,     0,     0,     0,
       0,   146,     0,     0,     0,     0,     0,  1071,     0,  1107,
       0,     0,  -245,     0,     0,  -245,     0,     0,  -245,     0,
       0,     0,     0,     0,   150,     0,     0,     0,   351,   352,
     153,   150,     0,   355,   153,   356,   357,     0,   135,     0,
     360,     0,   153,     0,     0,   141,     0,     0,   367,     0,
       0,     0,   141,   135,   371,   372,   373,   271,     0,     0,
     146,    22,    23,     0,   141,     0,     0,     0,  -245,     0,
     272,     0,    30,   273,  -245,   146,     0,    35,     0,   141,
       0,     0,     0,     0,    40,     0,     0,     0,   141,     0,
     150,     0,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   150,    53,     0,    55,     0,
      57,     0,   959,     0,     0,   960,     0,   274,     0,    63,
       0,     0,   151,     0,     0,     0,   135,     0,     0,     0,
       0,     0,   151,     0,   347,     0,     0,     0,    79,   141,
       0,    81,     0,     0,    83,   151,     0,   151,   146,     0,
       0,     0,     0,     0,   135,     0,   153,     0,     0,     0,
     135,     0,     0,     0,     0,     0,     0,   135,     0,     0,
       0,     0,     0,     0,     0,     0,   146,     0,   150,     0,
       0,     0,   146,     0,     0,     0,   348,     0,     0,   146,
     349,     0,     0,   151,    98,   141,   141,   141,   141,     0,
     961,     0,     0,     0,     0,     0,   150,     0,     0,   153,
       0,     0,   150,     0,     0,   153,     0,     0,     0,   150,
     151,     0,   141,   141,   135,   151,     0,     0,   135,     0,
       0,     0,     0,     0,     0,     0,   135,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   146,     0,     0,   360,
     146,     0,     0,     0,     0,     0,   366,   367,   146,     0,
     370,     0,     0,   371,   372,   373,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   150,     0,     0,     0,
     150,     0,   153,     0,   153,     0,     0,     0,   150,   153,
     151,     0,   153,     0,     0,     0,     0,     0,     0,   153,
       0,     0,   153,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,     0,     0,   153,     0,     0,
       0,     0,   225,     0,     0,     0,     0,   155,   153,   155,
     153,     0,     0,     0,     0,     0,   226,   227,     0,   228,
     135,     0,     0,     0,   229,     0,     0,     0,     0,     0,
       0,     0,   230,     0,     0,     0,     0,     0,   231,   151,
       0,     0,   146,   232,     0,     0,   151,   233,     0,     0,
     234,     0,     0,     0,     0,   155,     0,     0,   235,     0,
       0,     0,     0,     0,   236,   237,     0,     0,     0,     0,
       0,   238,   150,   135,     0,     0,     0,     0,     0,   135,
     239,     0,   155,     0,     0,     0,     0,   155,     0,   240,
     241,     0,   242,     0,   243,   146,   244,     0,     0,   245,
     153,   146,     0,   246,   455,   151,   247,   153,     0,   248,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   153,
     151,     0,   271,     0,     0,   150,    22,    23,     0,     0,
       0,   150,     0,     0,   153,   272,     0,    30,   273,     0,
       0,     0,    35,   153,     0,     0,   135,   153,   135,    40,
       0,     0,   155,   135,     0,     0,   135,     0,     0,     0,
       0,     0,     0,   135,     0,   456,   135,     0,   146,     0,
     146,    53,     0,    55,     0,   146,   156,     0,   146,     0,
       0,     0,   274,     0,    63,   146,   156,     0,   146,     0,
       0,   135,     0,   151,   153,     0,     0,     0,   150,   156,
     150,   156,   135,    79,   135,   150,    81,     0,   150,    83,
       0,     0,     0,   146,     0,   150,     0,     0,   150,     0,
       0,   151,     0,     0,   146,     0,   146,   151,     0,     0,
       0,   155,     0,     0,   151,     0,     0,     0,   155,     0,
       0,     0,     0,   150,     0,     0,     0,   156,     0,     0,
     153,   153,   153,   153,   150,     0,   150,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     225,     0,     0,     0,   156,     0,     0,   153,   153,   156,
       0,     0,     0,     0,   226,   227,     0,   228,     0,     0,
       0,   151,   229,     0,   135,   151,     0,   155,     0,     0,
     230,   135,     0,   151,     0,     0,   231,     0,     0,     0,
       0,   232,   155,   135,     0,   233,   146,     0,   234,     0,
       0,     0,     0,   146,     0,     0,   235,     0,   135,     0,
       0,     0,   236,   237,     0,   146,     0,   135,     0,   238,
       0,   135,     0,     0,   156,     0,   150,     0,   239,     0,
     146,     0,     0,   150,     0,     0,     0,   240,   241,   146,
     242,     0,   243,   146,   244,   150,     0,   245,     0,     0,
       0,   246,     0,     0,   247,     0,     0,   248,     0,     0,
     150,   106,     0,     0,     0,   155,     0,     0,   135,   150,
       0,   167,     0,   150,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,   199,     0,   202,     0,     0,     0,
     146,     0,     0,   155,     0,     0,     0,   151,     0,   155,
       0,     0,     0,   156,     0,     0,   155,     0,     0,     0,
     156,   403,     0,   803,     0,     0,     0,     0,     0,     0,
     150,     0,     0,     0,   135,   135,   135,   135,     0,     0,
     348,     0,   292,     0,   349,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   146,   146,   146,   146,
     151,   135,   135,     0,     0,     0,   151,     0,     0,   106,
       0,     0,     0,   155,   329,     0,     0,   155,     0,   156,
       0,     0,     0,   146,   146,   155,   150,   150,   150,   150,
       0,   351,     0,   225,   156,     0,   355,     0,   356,   357,
       0,     0,     0,   360,     0,     0,     0,   226,   227,     0,
     228,   367,     0,   150,   150,   229,     0,   371,   372,   373,
       0,     0,     0,   230,     0,     0,     0,     0,     0,   231,
       0,     0,     0,   151,   232,   151,     0,     0,   233,   397,
     151,   234,     0,   151,     0,     0,     0,     0,     0,   235,
     151,     0,     0,   151,     0,   236,   237,     0,     0,     0,
       0,     0,   238,     0,     0,     0,     0,   156,     0,     0,
       0,   239,     0,     0,     0,     0,     0,     0,   151,     0,
     240,   241,     0,   242,     0,   243,     0,   244,     0,   151,
     245,   151,     0,     0,   246,   156,     0,   247,     0,   155,
     248,   156,     0,     0,     0,     0,   271,     0,   156,     0,
      22,    23,     0,     0,     0,     0,     0,     0,   452,   272,
       0,    30,   273,     0,     0,   465,    35,     0,     0,     0,
       0,     0,     0,    40,     0,     0,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,     0,   155,     0,     0,    53,   252,    55,   155,    57,
       0,   959,     0,     0,   960,   156,   274,  -526,    63,   156,
       0,     0,     0,     0,     0,     0,     0,   156,     0,     0,
     390,   151,     0,  -564,   106,  -564,     0,    79,   151,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,   106,
     151,     0,     0,   271,     0,     0,     0,    22,    23,     0,
       0,     0,     0,     0,     0,   151,   272,     0,    30,   273,
       0,     0,     0,    35,   151,   155,     0,   155,   151,     0,
      40,     0,   155,     0,     0,   155,     0,     0,     0,     0,
       0,     0,   155,    98,     0,   155,     0,     0,     0,  1048,
       0,     0,    53,     0,    55,     0,    57,     0,   959,     0,
       0,   960,     0,   274,     0,    63,     0,     0,     0,     0,
     155,     0,   106,     0,     0,   151,     0,     0,     0,     0,
       0,   155,     0,   155,    79,     0,     0,    81,     0,     0,
      83,   156,     0,     0,     0,     0,     0,     0,     0,     0,
     465,     0,     0,     0,     0,     0,   465,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   225,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   151,   151,   151,   151,   226,   227,     0,   228,     0,
      98,     0,     0,   229,   156,     0,  1050,     0,   347,     0,
     156,   230,     0,     0,     0,     0,     0,   231,   151,   151,
       0,     0,   232,     0,     0,     0,   233,     0,     0,   234,
     679,     0,     0,   155,   684,     0,     0,   235,     0,     0,
     155,     0,   106,   236,   237,     0,     0,     0,     0,     0,
     238,     0,   155,     0,     0,     0,     0,     0,     0,   239,
     348,     0,     0,     0,   349,     0,     0,   155,   240,   241,
       0,   242,     0,   243,     0,   244,   155,   156,   245,   156,
     155,     0,   246,     0,   156,   247,     0,   156,   248,     0,
       0,     0,     0,     0,   156,     0,     0,   156,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,   156,   360,   361,   362,     0,   155,   364,   365,
     366,   367,     0,   156,   370,   156,     0,   371,   372,   373,
       0,     0,     0,     0,   781,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,   772,     0,     0,     0,
       5,   276,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,   155,   155,   155,   155,     0,     0,     0,
      28,     0,   176,     0,     0,     0,    32,   177,   178,     0,
     179,    37,     0,     0,     0,    39,     0,     0,    41,   808,
     155,   155,    43,    44,     0,   813,    46,    47,     0,    48,
      49,     0,    50,    51,     0,   156,     0,     0,     0,    54,
       0,    56,   156,    58,     0,     0,     0,     0,    61,   180,
       0,     0,     0,     0,   156,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,   271,   156,
       0,    80,    22,    23,    82,     0,     0,    84,   156,     0,
       0,   272,   156,    30,   273,     0,  -423,     0,    35,     0,
       0,     0,   465,     0,   465,    40,     0,     0,     0,   465,
       0,     0,   465,    90,     0,     0,     0,  -423,   345,   917,
       0,  -423,   919,     0,     0,   346,     0,    53,     0,    55,
       0,    57,     0,   959,     0,    97,   960,   347,   274,   156,
      63,   182,     0,   101,  -423,  1000,     0,   936,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   942,    79,
     946,     0,    81,     0,     0,    83,     0,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,     0,     0,     0,     0,     0,     0,     0,   348,
       0,     0,     0,   349,     0,   156,   156,   156,   156,     0,
       0,     0,   444,     0,     0,     0,     0,     0,     0,     0,
       0,   390,     0,     0,  -564,    98,  -564,     0,     0,     0,
       0,  1099,   156,   156,   350,   445,   376,   377,   378,   379,
     380,     0,     0,   383,   384,   385,   386,     0,   388,   389,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,     0,   371,   372,   373,   996,
     403,     0,     0,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,  1016,     0,     0,     0,     0,     0,
       0,     0,     0,  1021,     0,     0,     0,  1023,    -2,     4,
       0,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -74,  1056,    38,    39,    40,     0,    41,
    -259,     0,    42,    43,    44,     0,    45,    46,    47,   -45,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,    57,    58,    59,  -259,   -45,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -74,    83,    84,     0,
       0,    85,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,     0,   101,     0,   102,     0,     4,   103,
       5,     0,     6,     7,     8,     9,    10,    11,     0,  -594,
       0,    12,    13,    14,    15,    16,  -594,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,  -594,    27,
      28,  -594,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,   -74,     0,    38,    39,    40,     0,    41,  -259,
       0,    42,    43,    44,     0,    45,    46,    47,   -45,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,     0,    58,    59,  -259,   -45,    60,    61,    62,
    -594,    63,    64,    65,  -594,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -74,    83,    84,     0,     0,
      85,     0,    86,     0,     0,  -594,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -594,  -594,    90,  -594,  -594,  -594,  -594,  -594,  -594,
    -594,     0,  -594,  -594,  -594,  -594,  -594,     0,  -594,  -594,
    -594,  -594,  -594,  -594,  -594,  -594,    98,  -594,  -594,  -594,
       0,   100,  -594,   101,     0,   102,     0,   312,  -594,     5,
     276,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -74,     0,    38,    39,    40,     0,    41,  -259,     0,
      42,    43,    44,     0,    45,    46,    47,   -45,    48,    49,
       0,    50,    51,    52,     0,     0,     0,    53,    54,    55,
      56,    57,    58,    59,  -259,   -45,    60,    61,    62,     0,
      63,    64,    65,     0,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,     0,    81,    82,   -74,    83,    84,     0,     0,    85,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,   313,   101,     0,   102,     0,     4,   103,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -74,     0,    38,    39,    40,     0,    41,  -259,     0,    42,
      43,    44,     0,    45,    46,    47,   -45,    48,    49,     0,
      50,    51,    52,     0,     0,     0,    53,    54,    55,    56,
      57,    58,    59,  -259,   -45,    60,    61,    62,     0,    63,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
       0,    81,    82,   -74,    83,    84,     0,     0,    85,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
     492,   101,     0,   102,     0,   511,   103,     5,     0,     6,
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
      95,    96,    97,    98,     0,     0,    99,     0,   100,   512,
     101,     0,   102,     0,   312,   103,     5,     0,     6,     7,
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
      96,    97,    98,     0,     0,    99,     0,   100,   313,   101,
       0,   102,     0,     4,   103,     5,     0,     6,     7,     8,
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
      97,    98,     0,     0,    99,     0,   100,   691,   101,     0,
     102,     0,     4,   103,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,   -74,     0,    38,    39,
      40,     0,    41,  -259,     0,    42,    43,    44,     0,    45,
      46,    47,   -45,    48,    49,     0,    50,    51,    52,     0,
       0,     0,    53,    54,    55,    56,   328,    58,    59,  -259,
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
      33,    34,    35,    36,    37,   -74,     0,    38,    39,    40,
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
      34,    35,   608,    37,   -74,     0,    38,    39,    40,     0,
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
       0,    99,     0,   100,     0,   101,     0,   102,     0,     4,
     103,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   614,    37,   -74,     0,    38,    39,    40,     0,    41,
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
     902,    37,   -74,     0,    38,    39,    40,     0,    41,  -259,
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
       0,    29,     0,    30,    31,    32,    33,    34,    35,   904,
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
      29,     0,    30,    31,    32,    33,    34,    35,   909,    37,
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
       0,    30,    31,    32,    33,    34,    35,   912,    37,   -74,
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
      23,    24,    25,    26,     0,   935,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -74,     0,
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
      31,    32,    33,    34,    35,   941,    37,   -74,     0,    38,
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
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   945,    37,   -74,     0,    38,    39,
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
       0,  1128,   103,     5,   276,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   179,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   180,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   182,     0,   101,     0,   183,  1129,
     791,   103,     5,   276,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   174,     0,     0,    15,    16,     0,    17,
       0,   175,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   176,     0,     0,     0,    32,   177,
     178,     0,   179,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   180,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   181,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   182,     0,   101,     0,   183,     0,     5,
     103,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     174,     0,     0,    15,    16,     0,    17,     0,   175,     0,
       0,    21,   219,    23,     0,     0,     0,     0,     0,    28,
       0,   176,     0,     0,     0,    32,   177,   178,     0,   179,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    58,     0,     0,     0,     0,    61,   180,     0,
      63,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   181,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,    83,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     182,     0,   101,     0,   183,     0,     0,   103,     5,   276,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   174,
       0,     0,    15,    16,     0,    17,     0,   175,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     176,     0,     0,     0,    32,   177,   178,     0,   179,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   180,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     181,     0,    74,     0,     0,    76,   280,   281,     0,    80,
     317,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,   318,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   182,
       0,   101,     0,   183,     0,     0,   103,     5,   276,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   174,     0,
       0,    15,    16,     0,    17,     0,   175,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   176,
       0,     0,     0,    32,   177,   178,     0,   179,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   180,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   181,
       0,    74,     0,     0,    76,   280,   281,     0,    80,   317,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   182,     0,
     101,   675,   183,     0,     0,   103,     5,   276,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   174,     0,     0,
      15,    16,     0,    17,     0,   175,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   176,     0,
       0,     0,    32,   177,   178,     0,   179,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   180,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   181,     0,
      74,     0,     0,    76,   280,   281,     0,    80,   317,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   182,     0,   101,
     677,   183,     0,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,   573,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   179,    37,     0,   574,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   180,     0,   575,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,   576,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   182,     0,   101,     0,   183,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   176,     0,     0,     0,    32,   177,   178,     0,
     179,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   180,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,   280,   281,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,   282,     0,     0,    99,
       0,   182,     0,   101,     0,   183,     0,     0,   103,     5,
     276,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     174,     0,     0,    15,    16,     0,    17,     0,   175,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   176,     0,     0,     0,    32,   177,   178,     0,   179,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   180,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   181,     0,    74,     0,     0,    76,   280,   281,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     182,     0,   101,     0,   183,     0,     5,   103,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   174,     0,     0,
      15,    16,     0,    17,     0,   175,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   176,     0,
       0,     0,    32,   177,   178,     0,   179,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,   793,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   180,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   181,     0,
      74,     0,     0,    76,   280,   281,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   182,     0,   101,
       0,   183,     0,     5,   103,     6,     7,     8,   208,    10,
      11,   209,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   179,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   210,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,   211,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   182,     0,   101,     0,   183,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   176,     0,     0,     0,    32,   177,   178,     0,
     179,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
     213,    56,     0,    58,     0,     0,     0,     0,    61,   180,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,   214,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   182,     0,   101,     0,   183,     0,     0,   103,     5,
     276,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     174,     0,     0,    15,    16,     0,    17,     0,   175,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   176,     0,     0,     0,    32,   177,   178,     0,   179,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   180,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   181,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,   214,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     182,     0,   101,     0,   183,     0,     5,   103,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   174,     0,     0,
      15,    16,     0,    17,     0,   175,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   176,     0,
       0,     0,    32,   177,   178,     0,   179,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   180,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   181,     0,
      74,     0,     0,    76,   280,   281,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   182,     0,   101,
       0,   183,     0,     0,   103,     5,   276,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   174,     0,     0,    15,
      16,     0,    17,     0,   175,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   176,     0,     0,
       0,    32,   177,   178,     0,   179,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   180,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   181,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   182,   673,   101,     0,
     183,     0,     0,   103,     5,   276,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   174,     0,     0,    15,    16,
       0,    17,     0,   175,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   176,     0,     0,     0,
      32,   177,   178,     0,   179,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   180,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   181,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   182,     0,   101,     0,   183,
     683,     0,   103,     5,   276,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   179,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   180,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   182,     0,   101,     0,   183,  1060,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   176,     0,     0,     0,    32,   177,   178,     0,
     179,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   180,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,   214,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   182,     0,   101,     0,   183,     0,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   174,     0,
       0,    15,    16,     0,    17,     0,   175,     0,     0,    21,
       0,     0,     0,     0,     0,     0,   256,    28,     0,   176,
       0,     0,     0,    32,   177,   178,     0,   179,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   180,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   181,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   100,     0,
     101,     0,   183,     0,     0,   103,     5,   276,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   174,     0,     0,
      15,    16,     0,    17,     0,   175,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   176,     0,
       0,     0,    32,   177,   178,     0,   179,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   180,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   181,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   182,     0,   101,
       0,   183,     0,     5,   103,     6,     7,     8,   208,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   179,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   210,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,   211,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   182,     0,   101,     0,   183,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   176,     0,     0,     0,    32,   177,   178,     0,
     179,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   180,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   182,     0,   101,     0,   183,   688,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   174,     0,
       0,    15,    16,     0,    17,     0,   175,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   176,
       0,     0,     0,    32,   177,   178,     0,   179,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   180,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   181,
       0,    74,     0,     0,    76,     0,     0,     0,    80,   819,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   182,     0,
     101,     0,   183,     0,     0,   103,     5,   276,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   174,     0,     0,
      15,    16,     0,    17,     0,   175,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,   647,     0,   176,     0,
       0,     0,    32,   177,   178,     0,   179,    37,     0,     0,
       0,   648,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   180,     0,     0,     0,     0,
       0,     0,     0,    68,   649,    70,    71,    72,   650,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   182,     0,   101,
       0,  1000,     0,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   179,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   180,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,     0,    80,  1025,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   182,     0,   101,     0,   183,     0,
       0,   103,     5,   276,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   174,     0,     0,    15,    16,     0,    17,
       0,   175,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   176,     0,     0,     0,    32,   177,
     178,     0,   179,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   180,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   181,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   182,     0,   101,     0,  1000,     0,     0,
     103,     5,   276,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   174,     0,     0,    15,    16,     0,    17,     0,
     175,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   176,     0,     0,     0,    32,   177,   178,
       0,   982,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     180,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   181,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   182,     0,   101,     0,   183,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   174,
       0,     0,    15,    16,     0,    17,     0,   175,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     176,     0,     0,     0,    32,   177,   178,     0,   179,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   180,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     181,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   182,
       0,   101,     0,   183,     0,     5,   103,     6,     7,     8,
     204,    10,    11,     0,     0,     0,   174,     0,     0,    15,
      16,     0,    17,     0,   175,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   176,     0,     0,
       0,    32,   177,   178,     0,   179,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   205,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   181,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   182,     0,   101,     0,
     183,     0,     5,   103,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   174,     0,     0,    15,    16,     0,    17,
       0,   175,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   176,     0,     0,     0,    32,   177,
     178,     0,   179,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   180,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   181,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   100,     0,   101,     0,   183,     0,     5,
     103,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     174,     0,     0,    15,    16,     0,    17,     0,   175,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   176,     0,     0,     0,    32,   177,   178,     0,   179,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   180,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   181,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,   619,     0,
     182,     0,   101,     0,   183,     0,     5,   103,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   174,     0,     0,
      15,    16,     0,    17,     0,   175,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,   647,     0,   176,     0,
       0,     0,    32,   177,   178,     0,   179,    37,     0,     0,
       0,   648,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   180,     0,     0,     0,     0,
       0,     0,     0,    68,   649,    70,    71,    72,   650,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   182,     0,   101,
       0,   651,     0,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   179,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   180,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   182,     0,   101,     0,   651,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   176,     0,     0,     0,    32,   177,   178,     0,
     730,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   180,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   182,     0,   101,     0,   183,     0,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   174,     0,
       0,    15,    16,     0,    17,     0,   175,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   176,
       0,     0,     0,    32,   177,   178,     0,   733,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   180,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   181,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   182,     0,
     101,     0,   183,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   174,     0,     0,    15,    16,
       0,    17,     0,   175,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   176,     0,     0,     0,
      32,   177,   178,     0,   976,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   180,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   181,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   182,     0,   101,     0,   183,
       0,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   174,     0,     0,    15,    16,     0,    17,     0,
     175,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   176,     0,     0,     0,    32,   177,   178,
       0,   977,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     180,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   181,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   182,     0,   101,     0,   183,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   174,
       0,     0,    15,    16,     0,    17,     0,   175,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     176,     0,     0,     0,    32,   177,   178,     0,   979,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   180,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     181,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   182,
       0,   101,     0,   183,     0,     5,   103,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   174,     0,     0,    15,
      16,     0,    17,     0,   175,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   176,     0,     0,
       0,    32,   177,   178,     0,   980,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   180,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   181,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   182,     0,   101,     0,
     183,     0,     5,   103,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   174,     0,     0,    15,    16,     0,    17,
       0,   175,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   176,     0,     0,     0,    32,   177,
     178,     0,   981,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   180,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   181,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   182,     0,   101,     0,   183,     0,     5,
     103,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     174,     0,     0,    15,    16,     0,    17,     0,   175,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   176,     0,     0,     0,    32,   177,   178,     0,   982,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   180,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   181,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     182,     0,   101,     0,   183,     0,     5,   103,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   174,     0,     0,
      15,    16,     0,    17,     0,   175,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,   647,     0,   176,     0,
       0,     0,    32,   177,   178,     0,   179,    37,     0,     0,
       0,   648,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   180,     0,     0,     0,     0,
       0,     0,     0,    68,   649,    70,    71,    72,   650,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   182,     0,   101,
       0,  1084,     0,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   179,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   180,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     5,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,    15,     0,     0,    17,     0,     0,    87,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     176,   831,     0,   832,    88,    89,    90,    91,    92,    37,
       0,     0,   833,    93,     0,     0,    41,   834,   227,   835,
     836,     0,     0,    94,    46,   837,    95,    96,    97,     0,
       0,    51,    99,   230,     0,   176,   101,     0,  1084,   838,
       0,   103,     0,     0,   839,     0,    61,     0,   233,     0,
       0,   840,     0,   841,     0,     0,     0,     0,    71,   842,
       0,     0,    74,     0,     0,   843,   844,     0,     0,    80,
       0,     0,   238,     0,     0,    84,     0,     0,     0,     0,
       0,   845,     0,     0,     0,     0,     0,     0,     0,     0,
     240,   241,     0,   846,     0,   243,     0,   847,     0,     0,
     848,     0,     0,     0,   849,     0,     0,   247,     0,     0,
     850,     0,     0,     0,     0,     0,     0,     0,     0,   445,
     376,   377,   378,   379,   380,     0,     0,   383,   384,   385,
     386,     0,   388,   389,   851,   852,   853,   854,   855,     0,
       0,   856,     0,     0,     0,   857,   858,   859,   860,   861,
     862,   863,   864,   865,   866,   867,     0,   868,     0,     0,
     869,   870,   871,   872,     0,     5,   873,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   174,     0,     0,    15,
      16,     0,    17,     0,   175,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   176,     0,     0,
       0,    32,   177,   178,     0,   179,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   180,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   181,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,    96,
      97,     0,     0,     0,    99,     0,   182,     0,   101,     5,
     183,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     174,     0,     0,    15,    16,     0,    17,     0,   175,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   176,     0,     0,     0,    32,   177,   178,     0,   179,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,  -405,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   180,     0,
       0,     0,  -405,     0,     0,     0,    68,    69,    70,    71,
      72,   181,     0,    74,     0,  -405,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,  -405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,    90,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   174,     0,     0,    15,    16,     0,    17,     0,
     175,     0,     0,    21,    97,  -405,     0,     0,     0,     0,
    -405,    28,   101,   176,  1084,     0,     0,    32,   177,   178,
       0,   179,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     180,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   181,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     5,    90,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,    97,     0,     0,     0,
       0,     0,   182,    28,   101,   176,   183,     0,     0,    32,
     177,   178,     0,   179,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   180,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,    90,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   174,     0,     0,    15,
      16,     0,    17,     0,   175,     0,     0,    21,    97,     0,
       0,     0,     0,     0,   182,    28,   101,   176,   651,     0,
       0,    32,   177,   178,     0,   179,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   180,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   181,     0,    74,
       0,     0,    76,     0,  1037,     0,    80,     0,     0,    82,
       0,     0,    84,   833,     0,     0,     0,     0,   226,   227,
     835,   228,     0,     0,     0,     0,   229,     0,   271,     0,
       0,     0,    22,    23,   230,     0,     0,     0,    90,     0,
     838,   272,     0,    30,   273,   232,     0,     0,    35,   233,
       0,     0,   234,     0,   841,    40,     0,     0,     0,     0,
     235,     0,     0,     0,     0,     0,   843,   237,   101,     0,
     183,     0,     0,   238,     0,     0,     0,    53,     0,    55,
       0,   328,   239,   959,     0,     0,   960,     0,   274,     0,
      63,   240,   241,     0,   242,   345,   243,     0,  1038,     0,
       0,   848,   346,     0,     0,   246,     0,     0,   247,    79,
       0,   248,    81,     0,   347,    83,     0,     0,     0,     0,
     445,   376,   377,   378,   379,   380,     0,     0,   383,   384,
     385,   386,     0,   388,   389,   851,   852,   853,   854,   855,
       0,     0,   856,     0,     0,     0,   857,   858,   859,   860,
     861,   862,   863,   864,   865,   866,   867,     0,   868,     0,
       0,   869,   870,   871,   872,    98,   348,     0,     0,     0,
     349,   345,     0,     0,     0,     0,     0,     0,   346,   901,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,   445,   376,   377,   378,   379,   380,     0,     0,
     383,   384,   385,   386,     0,   388,   389,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,     0,   348,   371,   372,   373,   349,   403,     0,     0,
       0,   345,     0,     0,   374,   908,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,   350,   445,   376,
     377,   378,   379,   380,     0,     0,   383,   384,   385,   386,
       0,   388,   389,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,     0,   371,
     372,   373,   348,   403,     0,     0,   349,   345,     0,     0,
     374,     0,     0,     0,   346,  1051,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   350,   445,   376,
     377,   378,   379,   380,     0,     0,   383,   384,   385,   386,
       0,   388,   389,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,   348,   371,
     372,   373,   349,   403,     0,     0,     0,   345,     0,     0,
     374,  1052,     0,     0,   346,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,     0,     0,     0,
       0,     0,     0,   350,   445,   376,   377,   378,   379,   380,
       0,     0,   383,   384,   385,   386,     0,   388,   389,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,     0,
     359,   360,   361,   362,   363,     0,   364,   365,   366,   367,
     368,   369,   370,     0,     0,   371,   372,   373,   348,   403,
       0,     0,   349,   345,     0,     0,   374,     0,     0,     0,
     346,  1053,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,   445,   376,   377,   378,   379,   380,
       0,     0,   383,   384,   385,   386,     0,   388,   389,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,     0,
     359,   360,   361,   362,   363,     0,   364,   365,   366,   367,
     368,   369,   370,     0,   348,   371,   372,   373,   349,   403,
       0,     0,     0,   345,     0,     0,   374,  1054,     0,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,   350,
     445,   376,   377,   378,   379,   380,     0,     0,   383,   384,
     385,   386,     0,   388,   389,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,     0,
       0,   371,   372,   373,   348,   403,     0,     0,   349,   345,
       0,     0,   374,     0,     0,     0,   346,  1078,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   350,
     445,   376,   377,   378,   379,   380,     0,     0,   383,   384,
     385,   386,     0,   388,   389,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,     0,
     348,   371,   372,   373,   349,   403,     0,     0,     0,   345,
       0,     0,   374,  1079,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,   437,
       0,     0,     0,     0,     0,   350,   445,   376,   377,   378,
     379,   380,     0,   438,   383,   384,   385,   386,     0,   388,
     389,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,     0,     0,   371,   372,   373,
     348,   403,     0,     0,   349,   345,     0,     0,   374,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,   431,     0,     0,     0,     0,
       0,   434,     0,     0,     0,   350,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,     0,   348,   371,   372,   373,
     349,   403,     0,     0,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,     0,   347,     0,     0,     0,     0,   441,
       0,     0,     0,     0,     0,     0,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,   442,     0,   371,   372,   373,     0,   403,     0,     0,
       0,     0,     0,   347,   374,     0,   348,     0,     0,     0,
     349,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,   699,     0,     0,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,     0,   347,     0,     0,     0,     0,   700,
       0,     0,     0,     0,     0,   348,     0,   351,   352,   349,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,   701,   443,   371,   372,   373,     0,     0,     0,     0,
       0,     0,     0,     0,   374,     0,   348,     0,     0,     0,
     349,   345,     0,     0,     0,     0,   351,   352,   346,   353,
     354,   355,     0,   356,   357,   358,     0,     0,   360,     0,
     347,   763,     0,     0,     0,   366,   367,     0,     0,   370,
       0,   350,   371,   372,   373,     0,     0,     0,     0,     0,
       0,     0,     0,   374,     0,     0,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,     0,   348,   371,   372,   373,   349,     0,     0,     0,
       0,   345,     0,     0,   374,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,   434,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,   405,     0,     0,     0,     0,
       0,     0,     0,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,     0,   371,
     372,   373,   348,   403,     0,     0,   349,   345,     0,     0,
     374,     0,     0,     0,   346,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,   256,     0,     0,
       0,     0,     0,   169,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,   348,   371,
     372,   373,   349,     0,     0,     0,     0,   345,     0,     0,
     374,     0,     0,     0,   346,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,   582,     0,     0,
       0,     0,     0,   350,     0,     0,     0,     0,     0,     0,
       0,   583,     0,     0,     0,     0,     0,     0,     0,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,     0,
     359,   360,   361,   362,   363,     0,   364,   365,   366,   367,
     368,   369,   370,     0,     0,   371,   372,   373,   348,   403,
       0,     0,   349,   345,     0,     0,   374,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,   584,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,     0,     0,     0,   585,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,     0,
     359,   360,   361,   362,   363,     0,   364,   365,   366,   367,
     368,   369,   370,     0,   348,   371,   372,   373,   349,     0,
       0,     0,     0,   345,     0,     0,   374,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,   755,     0,     0,     0,     0,     0,   350,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,     0,
       0,   371,   372,   373,   348,     0,     0,     0,   349,   345,
       0,     0,   374,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   350,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,     0,
     348,   371,   372,   373,   349,   403,     0,     0,     0,   345,
       0,     0,   374,   773,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,     0,   350,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,     0,     0,   371,   372,   373,
     348,   403,     0,     0,   349,   345,     0,     0,   374,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,     0,     0,     0,     0,   954,
       0,   434,     0,     0,     0,   350,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   955,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,     0,   348,   371,   372,   373,
     349,     0,     0,     0,     0,   345,   811,     0,   374,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,     0,     0,     0,     0,     0,
       0,   350,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,     0,     0,   371,   372,   373,   348,     0,     0,     0,
     349,   345,     0,     0,   374,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,     0,   348,   371,   372,   373,   349,   477,     0,     0,
       0,   345,     0,     0,   374,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,   481,   371,
     372,   373,   348,     0,     0,     0,   349,     0,     0,     0,
     374,   345,     0,     0,     0,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,   483,   371,
     372,   373,   348,     0,     0,     0,   349,     0,     0,     0,
     374,   345,     0,     0,     0,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,   485,   371,
     372,   373,   348,     0,     0,     0,   349,     0,     0,     0,
     374,   345,     0,     0,     0,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,   490,   371,
     372,   373,   348,     0,     0,     0,   349,   345,     0,     0,
     374,     0,     0,     0,   346,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   507,   350,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,   348,   371,
     372,   373,   349,     0,     0,     0,     0,   345,     0,     0,
     374,   586,     0,     0,   346,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,     0,     0,     0,
       0,     0,     0,   350,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,     0,
     359,   360,   361,   362,   363,     0,   364,   365,   366,   367,
     368,   369,   370,     0,     0,   371,   372,   373,   348,     0,
       0,     0,   349,   345,   695,     0,   374,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,     0,
     359,   360,   361,   362,   363,     0,   364,   365,   366,   367,
     368,   369,   370,     0,   348,   371,   372,   373,   349,     0,
       0,     0,   674,   345,     0,     0,   374,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,   350,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   696,   358,     0,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,     0,
       0,   371,   372,   373,   348,     0,     0,     0,   349,   345,
       0,     0,   374,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   350,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
     363,  -527,   364,   365,   366,   367,   368,   369,   370,     0,
     348,   371,   372,   373,   349,     0,     0,     0,     0,   345,
       0,     0,   374,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,   169,     0,     0,     0,   350,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,     0,     0,   371,   372,   373,
     348,     0,     0,     0,   349,     0,     0,     0,   374,   345,
     768,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,     0,   350,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,     0,   740,   371,   372,   373,
     348,     0,     0,     0,   349,   345,     0,     0,   374,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,     0,     0,   771,     0,     0,
       0,     0,     0,     0,     0,   350,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,     0,   348,   371,   372,   373,
     349,     0,     0,     0,     0,   345,     0,     0,   374,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,     0,     0,     0,     0,     0,
       0,   350,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,     0,     0,   371,   372,   373,   348,     0,     0,     0,
     349,   345,     0,     0,   374,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,     0,   348,   371,   372,   373,   349,     0,     0,     0,
     795,     0,   345,     0,   374,     0,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,     0,   371,
     372,   373,     0,   348,     0,     0,   796,   349,   345,     0,
     374,     0,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,   888,     0,
       0,     0,     0,     0,     0,     0,     0,   814,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,   348,
     371,   372,   373,   349,     0,     0,     0,     0,   345,     0,
       0,   374,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,   891,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,     0,   371,   372,   373,   348,
       0,     0,     0,   349,   345,     0,     0,   374,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   348,   371,   372,   373,   349,
       0,     0,     0,     0,   345,   951,     0,   374,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,     0,   371,   372,   373,   348,     0,     0,     0,   349,
     345,   893,     0,   374,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,   348,   371,   372,   373,   349,     0,     0,     0,     0,
     345,     0,     0,   374,   975,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,     0,   371,   372,
     373,   348,     0,     0,     0,   349,   345,   994,     0,   374,
       0,     0,     0,   346,   978,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,   348,   371,   372,
     373,   349,     0,     0,     0,     0,   345,     0,     0,   374,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,     0,   371,   372,   373,   348,     0,     0,
       0,   349,   345,     0,     0,   374,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,   348,   371,   372,   373,   349,     0,     0,
       0,  1036,   345,     0,     0,   374,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,     0,
     371,   372,   373,   348,     0,     0,     0,   349,   345,  1074,
       0,   374,     0,     0,     0,   346,  1093,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,   348,
     371,   372,   373,   349,     0,     0,     0,     0,   345,     0,
       0,   374,  1094,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,     0,   371,   372,   373,   348,
       0,     0,     0,   349,   345,     0,     0,   374,     0,     0,
       0,   346,  1095,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   348,   371,   372,   373,   349,
       0,     0,     0,     0,   345,     0,     0,   374,  1096,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,     0,   371,   372,   373,   348,     0,     0,     0,   349,
     345,     0,     0,   374,     0,     0,     0,   346,  1097,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,   348,   371,   372,   373,   349,     0,     0,     0,     0,
       0,     0,     0,   374,  1098,     0,     0,     0,     0,     0,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
     346,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,  1122,   364,
     365,   366,   367,   368,   369,   370,     0,     0,   371,   372,
     373,     0,     0,     0,     0,     0,     0,     0,     0,   374,
       0,     0,     0,     0,   348,     0,     0,     0,   349,   345,
       0,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   350,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,     0,
     348,   371,   372,   373,   349,     0,     0,     0,     0,   345,
       0,     0,   374,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,     0,   350,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,     0,     0,   371,   372,   373,
     348,     0,     0,     0,   349,   345,  1151,     0,   374,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   350,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,     0,   728,   371,   372,   373,
     349,     0,     0,     0,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,   347,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,     0,   271,   371,   372,   373,    22,    23,     0,     0,
       0,     0,     0,     0,   374,   272,     0,    30,   273,     0,
       0,     0,    35,     0,     0,     0,     0,     0,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   348,
       0,     0,     0,   349,   347,     0,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,   959,     0,     0,
     960,     0,   274,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,   347,    79,     0,     0,    81,     0,     0,    83,
     351,   352,     0,   353,   354,   355,   348,   356,   357,   358,
     349,   359,   360,   361,   362,     0,     0,   364,   365,   366,
     367,   368,   369,   370,     0,     0,   371,   372,   373,     0,
       0,     0,     0,     0,     0,     0,     0,   374,     0,     0,
       0,   350,     0,     0,   348,     0,     0,     0,   349,    98,
       0,     0,     0,     0,     0,  1100,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,     0,     0,   364,   365,   366,   367,   368,     0,
     370,     0,     0,   371,   372,   373,     0,     0,     0,     0,
       0,     0,     0,     0,   374,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
       0,     0,   364,   365,   366,   367,   368,   704,   370,   225,
       0,   371,   372,   373,     0,     0,     0,     0,     0,     0,
       0,     0,   374,   226,   227,     0,   228,     0,     0,     0,
       0,   229,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,     0,     0,     0,   231,     0,     0,     0,     0,
     232,     0,     0,     0,   233,     0,     0,   234,     0,  -240,
       0,     0,     0,     0,     0,   235,     0,     0,     0,     0,
       0,   236,   237,  -240,  -240,     0,  -240,     0,   238,     0,
       0,  -240,     0,     0,     0,     0,     0,   239,     0,  -240,
       0,     0,     0,     0,     0,  -240,   240,   241,     0,   242,
    -240,   243,     0,   244,  -240,     0,   245,  -240,     0,  -250,
     246,     0,     0,   247,     0,  -240,   248,     0,     0,     0,
       0,  -240,  -240,  -250,  -250,     0,  -250,     0,  -240,     0,
       0,  -250,     0,     0,     0,     0,     0,  -240,     0,  -250,
       0,     0,     0,     0,     0,  -250,  -240,  -240,     0,  -240,
    -250,  -240,     0,  -240,  -250,     0,  -240,  -250,     0,   225,
    -240,     0,     0,  -240,     0,  -250,  -240,     0,     0,     0,
       0,  -250,  -250,   226,   227,     0,   228,     0,  -250,     0,
       0,   229,     0,     0,     0,     0,     0,  -250,     0,   230,
       0,     0,     0,     0,     0,   231,  -250,  -250,     0,  -250,
     232,  -250,     0,  -250,   233,     0,  -250,   234,     0,  -241,
    -250,     0,     0,  -250,     0,   235,  -250,     0,     0,     0,
       0,   236,   237,  -241,  -241,     0,  -241,     0,   238,     0,
       0,  -241,     0,     0,     0,     0,     0,   239,     0,  -241,
       0,     0,     0,     0,     0,  -241,   240,   241,     0,   242,
    -241,   243,     0,   244,  -241,     0,   245,  -241,     0,  -172,
     246,     0,     0,   247,     0,  -241,   248,     0,     0,     0,
       0,  -241,  -241,  -172,  -172,     0,  -172,     0,  -241,     0,
       0,  -172,     0,     0,     0,     0,     0,  -241,     0,  -172,
       0,     0,     0,     0,     0,  -172,  -241,  -241,     0,  -241,
    -172,  -241,     0,  -241,  -172,     0,  -241,  -172,     0,  -168,
    -241,     0,     0,  -241,     0,  -172,  -241,     0,     0,     0,
       0,  -172,  -172,  -168,  -168,     0,  -168,     0,  -172,     0,
       0,  -168,     0,     0,     0,     0,     0,  -172,     0,  -168,
       0,     0,     0,     0,     0,  -168,  -172,  -172,     0,  -172,
    -168,  -172,     0,  -172,  -168,     0,  -172,  -168,     0,     0,
    -172,     0,     0,  -172,     0,  -168,  -172,     0,     0,     0,
       0,  -168,  -168,     0,     0,   271,     0,     0,  -168,    22,
      23,     0,     0,     0,     0,     0,     0,  -168,   272,     0,
      30,   273,     0,     0,     0,    35,  -168,  -168,   -75,  -168,
       0,  -168,    40,  -168,     0,  -259,  -168,     0,     0,     0,
    -168,     0,     0,  -168,     0,     0,  -168,     0,     0,     0,
       0,     0,     0,     0,    53,     0,    55,     0,     0,     0,
       0,  -259,     0,     0,     0,   274,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,     0,     0,    81,
       0,   -75,    83
};

static const yytype_int16 yycheck[] =
{
      12,    64,   153,    60,    16,   224,     2,   486,    20,   203,
     612,    19,   442,   195,    26,   790,   713,    22,    30,    31,
     456,    33,    34,    35,    36,   218,   139,    46,   701,   211,
     224,     1,    42,    45,   396,    47,   700,   646,     1,   102,
      52,   925,    54,    32,    56,   600,    45,    32,   603,    86,
     269,  1010,    64,    65,    66,    67,    68,    69,   824,     3,
      46,    73,   922,    75,     3,    77,    78,     3,    32,    79,
      82,   998,   291,    85,    86,   269,    88,   453,    90,   878,
     879,    93,    94,   196,    96,    32,  1060,     3,   100,   101,
     102,   103,    32,   114,   140,   114,  1112,   291,    69,    69,
     114,   207,    57,    46,    20,     0,   212,   140,   883,   108,
     159,    46,    46,    36,    46,   159,   165,    33,   727,   165,
     183,   140,   132,   725,   134,   114,     1,   164,    31,   114,
     323,   324,  1059,    46,     1,  1151,   169,   149,  1112,   999,
     111,   111,    86,    59,     3,    68,  1129,   298,   169,  1033,
     169,   926,    84,    97,   140,   169,   119,   112,    97,    33,
      34,    97,   174,   357,  1147,   177,   178,   179,   114,   181,
     182,   183,  1131,   392,   393,   164,   139,   140,   787,   164,
      83,    97,    57,   461,    87,   195,   194,   103,   140,   744,
      57,   203,   747,   140,   165,   165,   962,   140,   392,   393,
     164,   211,   207,   213,   167,   140,   140,   212,   486,   645,
     166,    86,   224,   270,   219,   167,    46,   164,   984,    86,
     986,    18,    86,   998,   164,   140,   169,   140,   102,    52,
     112,   134,    99,    45,   169,   169,   139,   112,   141,   142,
    1039,  1040,   252,   146,   897,   112,   899,   140,   164,    61,
     612,   154,   167,     1,    84,    78,   169,   269,   140,   162,
     479,   273,   140,    84,   139,    57,   159,    79,   280,   281,
     159,   701,   139,     8,   971,    72,   165,   119,   119,   291,
     953,  1047,   955,  1049,  1059,   479,   140,   165,   952,   164,
     954,    45,   159,    90,    86,   159,   108,   164,    46,   140,
     164,   140,   144,   144,   906,   166,   318,    99,   140,   422,
     107,   165,    83,    67,   427,    77,    87,   134,    80,   315,
     112,   333,   334,    77,    78,   142,   167,   159,   167,   101,
     983,   343,   985,   345,   346,   347,   348,   349,   350,    27,
     352,   353,   354,   355,   356,   781,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   725,   166,   134,   429,   159,   134,   381,
     382,   119,   164,   142,   119,   387,   142,    94,   390,   391,
     392,   393,   394,   395,   396,    73,   166,   817,   168,   140,
      27,   139,   140,   405,   164,   140,    84,    27,   410,   144,
     412,    83,   166,   779,   598,    87,    67,   140,   140,    46,
     420,   159,    79,    50,   424,   103,   167,   429,   159,   431,
     432,    31,   167,   616,   617,   437,   438,   159,     3,   441,
     442,   203,   444,    70,   167,   447,    73,    27,    75,   193,
     158,   166,   140,    73,   140,    20,   456,    84,   824,   140,
     159,   463,   224,   932,    84,   467,   468,   520,    33,   140,
     472,   215,   216,   217,   218,   159,   103,   479,   222,   167,
     497,   167,   499,    83,   348,   349,   167,    87,    65,   109,
     140,   140,   494,    73,    59,   497,   167,   499,   681,   682,
     672,   503,   504,   505,    84,   507,   508,   269,   150,   159,
     159,   695,   696,   140,   514,   259,   159,   164,   520,    77,
      78,   265,   522,   103,   101,   140,     2,   513,   140,   291,
     394,   395,    97,   953,   134,   955,    12,   140,   103,   139,
     167,   405,   142,   287,   159,   159,   146,   159,    57,    25,
     140,    27,   159,   159,   906,   166,   159,   168,    57,   113,
     166,   305,   168,   159,   215,   216,   217,   218,   432,   159,
     159,   166,   713,   168,   438,   159,    82,    86,    82,   166,
     582,   583,   584,   585,   586,   587,   588,    86,   166,    27,
      99,   589,   590,   119,   596,   166,   598,    73,   651,   599,
      99,   967,   602,   112,   606,   166,   608,   168,   259,   166,
     612,    49,   614,   112,   265,   166,   159,   168,   984,   145,
     986,   147,   148,  1065,   100,   151,   152,    27,   166,   105,
     392,   393,   134,    71,   119,    73,   287,   139,   159,   503,
     142,   641,     8,    61,   146,   645,    84,   649,   650,   651,
     159,   139,   164,   655,   305,   164,   166,   925,   168,   142,
     159,    64,   406,   159,   932,   164,   119,   167,   668,   140,
     159,   109,   672,    73,  1116,  1117,  1118,  1119,  1120,  1121,
     166,  1047,    46,  1049,    84,    34,   167,   167,   140,   119,
     167,   693,   436,   167,   170,   697,   440,   699,   700,   701,
      21,   140,   169,   103,   169,   164,   166,   169,   159,   109,
     472,    27,   159,   713,   159,   159,   159,   479,    31,   583,
      34,   585,    46,   725,   933,   934,   728,   729,   730,   140,
      46,   733,   494,    57,    50,   497,   159,   499,   167,   159,
     159,    61,   504,   159,   165,   167,   799,   140,   165,   933,
     934,   753,   167,   755,    70,   406,   119,    73,   760,    75,
     167,   763,    86,    32,   766,  1033,   768,    84,    84,   771,
      83,    32,   169,   249,    87,   775,   164,   164,   778,    34,
     256,   781,   139,    84,   169,   436,   166,   103,   112,   440,
     166,   139,  1060,   165,   169,   119,   169,   799,   159,   159,
     142,   166,   142,   803,   381,   382,  1074,   809,   142,   811,
     387,   134,   814,   390,   391,   817,   140,   142,   164,   134,
     142,   134,   135,   142,   826,   138,   139,   827,   141,   142,
     971,   142,   142,   146,   596,   159,   598,   142,   134,   315,
     164,   154,   142,   167,  1112,   142,   134,   160,   161,   162,
     134,   142,   164,    34,   330,   159,   159,   159,   167,   167,
     167,  1129,    99,   134,   728,   729,   610,   611,   167,   159,
     159,   873,   616,   617,   134,   158,   158,   169,   140,  1147,
     450,   330,    24,  1151,   628,   887,   888,   693,   890,   891,
     892,   893,   803,   299,   971,   827,   967,   779,   638,   901,
     902,  1131,   904,   830,   906,  1122,   908,   909,  1089,  1115,
     912,     2,    35,   960,   464,   598,    30,    46,    -1,    -1,
     497,    12,   499,    -1,   668,    -1,    -1,   403,    57,   931,
     932,   933,   934,    -1,    25,    -1,    27,   939,    -1,   941,
      -1,    -1,    -1,   945,    -1,    -1,    -1,  1000,    -1,   951,
     952,   953,   954,   955,    -1,   431,    -1,    86,    -1,   610,
     611,   437,    -1,    -1,    -1,   616,   617,    -1,   444,    -1,
      -1,   971,    -1,   975,   976,   977,   978,   979,   980,   981,
     982,    -1,    73,   112,    -1,    -1,    77,    78,    -1,    -1,
     119,    -1,   994,    -1,   738,   739,    -1,    27,  1000,    -1,
      -1,    -1,    -1,  1005,    -1,    -1,   750,  1009,   752,   100,
      -1,   140,    -1,    -1,   105,   759,    46,    -1,   762,   596,
      50,   765,    -1,    -1,    -1,   501,    -1,    -1,    -1,   505,
     159,  1084,    -1,    -1,    -1,   164,    -1,   513,   167,    -1,
      70,    -1,    -1,    73,    -1,    75,    -1,    -1,    -1,  1051,
    1052,  1053,  1054,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      46,    -1,   806,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,  1074,   103,    -1,    -1,  1078,  1079,   655,   170,
      -1,    -1,  1084,    -1,    31,    -1,    -1,   738,   739,    -1,
      -1,  1093,  1094,  1095,  1096,  1097,  1098,    -1,    -1,   750,
      86,    -1,    -1,   579,    -1,     2,    -1,    -1,    -1,  1111,
    1112,   762,  1114,    -1,   765,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,     2,    25,    -1,
      27,    -1,    -1,   119,    -1,    -1,    83,    12,    -1,    -1,
      87,    -1,    -1,    -1,    -1,  1147,    -1,    -1,    -1,  1151,
      25,   627,    27,    -1,   140,    -1,    -1,     2,   249,    -1,
      -1,    -1,    -1,   925,    -1,   256,    -1,    12,    -1,    -1,
      -1,   933,   934,   159,    -1,    -1,    73,    -1,   164,    -1,
      25,   167,    27,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,   937,   141,   142,    -1,    -1,    73,   146,
      -1,    -1,    -1,   100,   680,    -1,    -1,   154,   105,    -1,
     686,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   315,   100,    -1,    -1,    73,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,   713,    -1,   330,
      -1,    -1,    -1,  1005,   988,   989,    -1,    -1,   992,   993,
      -1,    -1,   728,   729,    -1,   100,    -1,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,  1010,    -1,    -1,    -1,
      -1,    -1,    -1,   170,  1018,    -1,  1020,   753,  1022,   755,
      -1,    -1,    -1,    -1,   760,    -1,   937,   763,    -1,    -1,
       1,    -1,    -1,    -1,   770,   170,    -1,   773,  1060,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    22,   403,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,   798,    -1,    35,   170,    37,    38,    -1,    -1,
      -1,    42,    -1,   809,    -1,   811,    -1,    -1,    49,    -1,
     431,    52,    -1,    -1,    -1,    -1,   437,    -1,    -1,    -1,
    1112,    -1,   249,   444,    -1,    -1,    -1,    83,    -1,   256,
      71,    87,    73,    -1,    75,    -1,    77,    78,    -1,    80,
      -1,    82,    -1,    84,   249,    -1,    -1,    -1,    -1,    -1,
      -1,   256,    -1,    -1,    -1,    -1,    -1,  1131,    -1,  1151,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,   249,    -1,    -1,    -1,   134,   135,
     501,   256,    -1,   139,   505,   141,   142,    -1,   315,    -1,
     146,    -1,   513,    -1,    -1,   901,    -1,    -1,   154,    -1,
      -1,    -1,   908,   330,   160,   161,   162,    22,    -1,    -1,
     315,    26,    27,    -1,   920,    -1,    -1,    -1,   159,    -1,
      35,    -1,    37,    38,   165,   330,    -1,    42,    -1,   935,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,   944,    -1,
     315,    -1,   948,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   330,    71,    -1,    73,    -1,
      75,    -1,    77,    -1,    -1,    80,    -1,    82,    -1,    84,
      -1,    -1,     2,    -1,    -1,    -1,   403,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    31,    -1,    -1,    -1,   103,   995,
      -1,   106,    -1,    -1,   109,    25,    -1,    27,   403,    -1,
      -1,    -1,    -1,    -1,   431,    -1,   627,    -1,    -1,    -1,
     437,    -1,    -1,    -1,    -1,    -1,    -1,   444,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   431,    -1,   403,    -1,
      -1,    -1,   437,    -1,    -1,    -1,    83,    -1,    -1,   444,
      87,    -1,    -1,    73,   159,  1051,  1052,  1053,  1054,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   431,    -1,    -1,   680,
      -1,    -1,   437,    -1,    -1,   686,    -1,    -1,    -1,   444,
     100,    -1,  1078,  1079,   501,   105,    -1,    -1,   505,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   513,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   501,    -1,    -1,   146,
     505,    -1,    -1,    -1,    -1,    -1,   153,   154,   513,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   501,    -1,    -1,    -1,
     505,    -1,   753,    -1,   755,    -1,    -1,    -1,   513,   760,
     170,    -1,   763,    -1,    -1,    -1,    -1,    -1,    -1,   770,
      -1,    -1,   773,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,   798,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    25,   809,    27,
     811,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
     627,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,   249,
      -1,    -1,   627,    44,    -1,    -1,   256,    48,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    73,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,   627,   680,    -1,    -1,    -1,    -1,    -1,   686,
      81,    -1,   100,    -1,    -1,    -1,    -1,   105,    -1,    90,
      91,    -1,    93,    -1,    95,   680,    97,    -1,    -1,   100,
     901,   686,    -1,   104,   105,   315,   107,   908,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   920,
     330,    -1,    22,    -1,    -1,   680,    26,    27,    -1,    -1,
      -1,   686,    -1,    -1,   935,    35,    -1,    37,    38,    -1,
      -1,    -1,    42,   944,    -1,    -1,   753,   948,   755,    49,
      -1,    -1,   170,   760,    -1,    -1,   763,    -1,    -1,    -1,
      -1,    -1,    -1,   770,    -1,   166,   773,    -1,   753,    -1,
     755,    71,    -1,    73,    -1,   760,     2,    -1,   763,    -1,
      -1,    -1,    82,    -1,    84,   770,    12,    -1,   773,    -1,
      -1,   798,    -1,   403,   995,    -1,    -1,    -1,   753,    25,
     755,    27,   809,   103,   811,   760,   106,    -1,   763,   109,
      -1,    -1,    -1,   798,    -1,   770,    -1,    -1,   773,    -1,
      -1,   431,    -1,    -1,   809,    -1,   811,   437,    -1,    -1,
      -1,   249,    -1,    -1,   444,    -1,    -1,    -1,   256,    -1,
      -1,    -1,    -1,   798,    -1,    -1,    -1,    73,    -1,    -1,
    1051,  1052,  1053,  1054,   809,    -1,   811,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    -1,    -1,   100,    -1,    -1,  1078,  1079,   105,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,   501,    25,    -1,   901,   505,    -1,   315,    -1,    -1,
      33,   908,    -1,   513,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,   330,   920,    -1,    48,   901,    -1,    51,    -1,
      -1,    -1,    -1,   908,    -1,    -1,    59,    -1,   935,    -1,
      -1,    -1,    65,    66,    -1,   920,    -1,   944,    -1,    72,
      -1,   948,    -1,    -1,   170,    -1,   901,    -1,    81,    -1,
     935,    -1,    -1,   908,    -1,    -1,    -1,    90,    91,   944,
      93,    -1,    95,   948,    97,   920,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
     935,     2,    -1,    -1,    -1,   403,    -1,    -1,   995,   944,
      -1,    12,    -1,   948,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
     995,    -1,    -1,   431,    -1,    -1,    -1,   627,    -1,   437,
      -1,    -1,    -1,   249,    -1,    -1,   444,    -1,    -1,    -1,
     256,   164,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
     995,    -1,    -1,    -1,  1051,  1052,  1053,  1054,    -1,    -1,
      83,    -1,    73,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1051,  1052,  1053,  1054,
     680,  1078,  1079,    -1,    -1,    -1,   686,    -1,    -1,   100,
      -1,    -1,    -1,   501,   105,    -1,    -1,   505,    -1,   315,
      -1,    -1,    -1,  1078,  1079,   513,  1051,  1052,  1053,  1054,
      -1,   134,    -1,     3,   330,    -1,   139,    -1,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,    17,    18,    -1,
      20,   154,    -1,  1078,  1079,    25,    -1,   160,   161,   162,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,   753,    44,   755,    -1,    -1,    48,   170,
     760,    51,    -1,   763,    -1,    -1,    -1,    -1,    -1,    59,
     770,    -1,    -1,   773,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,   403,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,   798,    -1,
      90,    91,    -1,    93,    -1,    95,    -1,    97,    -1,   809,
     100,   811,    -1,    -1,   104,   431,    -1,   107,    -1,   627,
     110,   437,    -1,    -1,    -1,    -1,    22,    -1,   444,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   249,    35,
      -1,    37,    38,    -1,    -1,   256,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,    -1,   680,    -1,    -1,    71,   166,    73,   686,    75,
      -1,    77,    -1,    -1,    80,   501,    82,   150,    84,   505,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   513,    -1,    -1,
     163,   901,    -1,   166,   315,   168,    -1,   103,   908,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,   330,
     920,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,   935,    35,    -1,    37,    38,
      -1,    -1,    -1,    42,   944,   753,    -1,   755,   948,    -1,
      49,    -1,   760,    -1,    -1,   763,    -1,    -1,    -1,    -1,
      -1,    -1,   770,   159,    -1,   773,    -1,    -1,    -1,   165,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    77,    -1,
      -1,    80,    -1,    82,    -1,    84,    -1,    -1,    -1,    -1,
     798,    -1,   403,    -1,    -1,   995,    -1,    -1,    -1,    -1,
      -1,   809,    -1,   811,   103,    -1,    -1,   106,    -1,    -1,
     109,   627,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     431,    -1,    -1,    -1,    -1,    -1,   437,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1051,  1052,  1053,  1054,    17,    18,    -1,    20,    -1,
     159,    -1,    -1,    25,   680,    -1,   165,    -1,    31,    -1,
     686,    33,    -1,    -1,    -1,    -1,    -1,    39,  1078,  1079,
      -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
     501,    -1,    -1,   901,   505,    -1,    -1,    59,    -1,    -1,
     908,    -1,   513,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,   920,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      83,    -1,    -1,    -1,    87,    -1,    -1,   935,    90,    91,
      -1,    93,    -1,    95,    -1,    97,   944,   753,   100,   755,
     948,    -1,   104,    -1,   760,   107,    -1,   763,   110,    -1,
      -1,    -1,    -1,    -1,   770,    -1,    -1,   773,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   798,   146,   147,   148,    -1,   995,   151,   152,
     153,   154,    -1,   809,   157,   811,    -1,   160,   161,   162,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   627,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,  1051,  1052,  1053,  1054,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,   680,
    1078,  1079,    55,    56,    -1,   686,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,   901,    -1,    -1,    -1,    72,
      -1,    74,   908,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,   920,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    22,   935,
      -1,   104,    26,    27,   107,    -1,    -1,   110,   944,    -1,
      -1,    35,   948,    37,    38,    -1,   119,    -1,    42,    -1,
      -1,    -1,   753,    -1,   755,    49,    -1,    -1,    -1,   760,
      -1,    -1,   763,   136,    -1,    -1,    -1,   140,    12,   770,
      -1,   144,   773,    -1,    -1,    19,    -1,    71,    -1,    73,
      -1,    75,    -1,    77,    -1,   158,    80,    31,    82,   995,
      84,   164,    -1,   166,   167,   168,    -1,   798,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   809,   103,
     811,    -1,   106,    -1,    -1,   109,    -1,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    87,    -1,  1051,  1052,  1053,  1054,    -1,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,    -1,   166,   159,   168,    -1,    -1,    -1,
      -1,   165,  1078,  1079,   118,   119,   120,   121,   122,   123,
     124,    -1,    -1,   127,   128,   129,   130,    -1,   132,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,   160,   161,   162,   920,
     164,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   935,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   944,    -1,    -1,    -1,   948,     0,     1,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,   995,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    88,    89,    90,    91,
      92,    93,    94,    95,    -1,    97,    98,    -1,   100,   101,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,   159,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,     1,   171,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    66,    67,    -1,    -1,    -1,    71,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    -1,    97,    98,    -1,   100,   101,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,    -1,
     113,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,   165,   166,    -1,   168,    -1,     1,   171,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    -1,    58,    59,    60,    61,    62,    63,
      -1,    65,    66,    67,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    88,    89,    90,    91,    92,    93,
      94,    95,    -1,    97,    98,    -1,   100,   101,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,    -1,   113,
      -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,   159,    -1,    -1,   162,    -1,
     164,   165,   166,    -1,   168,    -1,     1,   171,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    -1,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    88,    89,    90,    91,    92,    93,    94,
      95,    -1,    97,    98,    -1,   100,   101,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,    -1,   113,    -1,
     115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,   159,    -1,    -1,   162,    -1,   164,
     165,   166,    -1,   168,    -1,     1,   171,     3,    -1,     5,
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
      -1,     1,   171,     3,     4,     5,     6,     7,     8,     9,
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
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,   169,
       1,   171,     3,     4,     5,     6,     7,     8,     9,    10,
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
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,    -1,   109,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,
     164,    -1,   166,    -1,   168,    -1,    -1,   171,     3,     4,
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
     105,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,   144,
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
      -1,    97,    -1,    -1,   100,   101,   102,    -1,   104,   105,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,   167,   168,    -1,    -1,   171,     3,     4,     5,     6,
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
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,
     167,   168,    -1,     3,   171,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    46,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,   109,
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
      93,    94,    95,    -1,    97,    -1,    -1,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,   159,    -1,    -1,   162,
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
      94,    95,    -1,    97,    -1,    -1,   100,   101,   102,    -1,
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
      -1,    -1,    59,    60,    -1,    62,    63,    64,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    -1,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,     3,   171,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,   103,   104,    -1,    -1,   107,    -1,    -1,
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
      73,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,    -1,    -1,    -1,    -1,
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
      -1,    -1,   116,    -1,   118,    -1,    -1,    -1,    -1,    -1,
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
      97,    -1,    -1,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
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
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,    -1,    -1,   162,    -1,   164,   165,   166,    -1,
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
     169,    -1,   171,     3,     4,     5,     6,     7,     8,     9,
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
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,   169,
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
      -1,    -1,    -1,   116,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    35,
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
     100,    -1,    -1,   103,   104,    -1,    -1,   107,    -1,    -1,
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
      -1,   164,    -1,   166,    -1,   168,   169,     3,   171,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,   105,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
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
     100,    -1,    -1,    -1,   104,   105,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,
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
      -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,    -1,
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
      90,    91,    92,    93,    94,    95,    -1,    97,     3,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,    17,    -1,    -1,    20,    -1,    -1,   118,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,     1,    -1,     3,   134,   135,   136,   137,   138,    44,
      -1,    -1,    12,   143,    -1,    -1,    51,    17,    18,    19,
      20,    -1,    -1,   153,    59,    25,   156,   157,   158,    -1,
      -1,    66,   162,    33,    -1,    35,   166,    -1,   168,    39,
      -1,   171,    -1,    -1,    44,    -1,    81,    -1,    48,    -1,
      -1,    51,    -1,    53,    -1,    -1,    -1,    -1,    93,    59,
      -1,    -1,    97,    -1,    -1,    65,    66,    -1,    -1,   104,
      -1,    -1,    72,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    -1,    93,    -1,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,   123,   124,    -1,    -1,   127,   128,   129,
     130,    -1,   132,   133,   134,   135,   136,   137,   138,    -1,
      -1,   141,    -1,    -1,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,    -1,   157,    -1,    -1,
     160,   161,   162,   163,    -1,     3,   166,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,   157,
     158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,     3,
     168,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    97,    -1,    99,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,   110,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,   136,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,   158,   159,    -1,    -1,    -1,    -1,
     164,    33,   166,    35,   168,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,     3,   136,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,   158,    -1,    -1,    -1,
      -1,    -1,   164,    33,   166,    35,   168,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,   136,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,   158,    -1,
      -1,    -1,    -1,    -1,   164,    33,   166,    35,   168,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,
      -1,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    -1,    97,
      -1,    -1,   100,    -1,     3,    -1,   104,    -1,    -1,   107,
      -1,    -1,   110,    12,    -1,    -1,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    22,    -1,
      -1,    -1,    26,    27,    33,    -1,    -1,    -1,   136,    -1,
      39,    35,    -1,    37,    38,    44,    -1,    -1,    42,    48,
      -1,    -1,    51,    -1,    53,    49,    -1,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    65,    66,   166,    -1,
     168,    -1,    -1,    72,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    81,    77,    -1,    -1,    80,    -1,    82,    -1,
      84,    90,    91,    -1,    93,    12,    95,    -1,    97,    -1,
      -1,   100,    19,    -1,    -1,   104,    -1,    -1,   107,   103,
      -1,   110,   106,    -1,    31,   109,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,   123,   124,    -1,    -1,   127,   128,
     129,   130,    -1,   132,   133,   134,   135,   136,   137,   138,
      -1,    -1,   141,    -1,    -1,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,    -1,   157,    -1,
      -1,   160,   161,   162,   163,   159,    83,    -1,    -1,    -1,
      87,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    96,
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
      -1,    -1,    87,    12,    -1,    -1,   171,    -1,    -1,    -1,
      19,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,   120,   121,   122,   123,   124,
      -1,    -1,   127,   128,   129,   130,    -1,   132,   133,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    -1,    83,   160,   161,   162,    87,   164,
      -1,    -1,    -1,    12,    -1,    -1,   171,    96,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,   120,   121,   122,   123,   124,    -1,    -1,   127,   128,
     129,   130,    -1,   132,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,   160,   161,   162,    83,   164,    -1,    -1,    87,    12,
      -1,    -1,   171,    -1,    -1,    -1,    19,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,   120,   121,   122,   123,   124,    -1,    -1,   127,   128,
     129,   130,    -1,   132,   133,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      83,   160,   161,   162,    87,   164,    -1,    -1,    -1,    12,
      -1,    -1,   171,    96,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,
     123,   124,    -1,    46,   127,   128,   129,   130,    -1,   132,
     133,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,   160,   161,   162,
      83,   164,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    83,   160,   161,   162,
      87,   164,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    68,    -1,   160,   161,   162,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    31,   171,    -1,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    83,    -1,   134,   135,    87,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    68,   159,   160,   161,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    83,    -1,    -1,    -1,
      87,    12,    -1,    -1,    -1,    -1,   134,   135,    19,   137,
     138,   139,    -1,   141,   142,   143,    -1,    -1,   146,    -1,
      31,    32,    -1,    -1,    -1,   153,   154,    -1,    -1,   157,
      -1,   118,   160,   161,   162,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    83,   160,   161,   162,    87,    -1,    -1,    -1,
      -1,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,   160,
     161,   162,    83,   164,    -1,    -1,    87,    12,    -1,    -1,
     171,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    83,   160,
     161,   162,    87,    -1,    -1,    -1,    -1,    12,    -1,    -1,
     171,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,   160,   161,   162,    83,   164,
      -1,    -1,    87,    12,    -1,    -1,   171,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    -1,    83,   160,   161,   162,    87,    -1,
      -1,    -1,    -1,    12,    -1,    -1,   171,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,   160,   161,   162,    83,    -1,    -1,    -1,    87,    12,
      -1,    -1,   171,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      83,   160,   161,   162,    87,   164,    -1,    -1,    -1,    12,
      -1,    -1,   171,    96,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,   160,   161,   162,
      83,   164,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    68,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    83,   160,   161,   162,
      87,    -1,    -1,    -1,    -1,    12,   169,    -1,   171,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,   160,   161,   162,    83,    -1,    -1,    -1,
      87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    83,   160,   161,   162,    87,   164,    -1,    -1,
      -1,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,   159,   160,
     161,   162,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,
     171,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,   159,   160,
     161,   162,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,
     171,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,   159,   160,
     161,   162,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,
     171,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,   159,   160,
     161,   162,    83,    -1,    -1,    -1,    87,    12,    -1,    -1,
     171,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    83,   160,
     161,   162,    87,    -1,    -1,    -1,    -1,    12,    -1,    -1,
     171,    96,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,   160,   161,   162,    83,    -1,
      -1,    -1,    87,    12,    13,    -1,   171,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    -1,    83,   160,   161,   162,    87,    -1,
      -1,    -1,   167,    12,    -1,    -1,   171,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,   160,   161,   162,    83,    -1,    -1,    -1,    87,    12,
      -1,    -1,   171,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      83,   160,   161,   162,    87,    -1,    -1,    -1,    -1,    12,
      -1,    -1,   171,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,   160,   161,   162,
      83,    -1,    -1,    -1,    87,    -1,    -1,    -1,   171,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,   159,   160,   161,   162,
      83,    -1,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    83,   160,   161,   162,
      87,    -1,    -1,    -1,    -1,    12,    -1,    -1,   171,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,   160,   161,   162,    83,    -1,    -1,    -1,
      87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    83,   160,   161,   162,    87,    -1,    -1,    -1,
     167,    -1,    12,    -1,   171,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,   160,
     161,   162,    -1,    83,    -1,    -1,   167,    87,    12,    -1,
     171,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    83,
     160,   161,   162,    87,    -1,    -1,    -1,    -1,    12,    -1,
      -1,   171,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,   160,   161,   162,    83,
      -1,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    83,   160,   161,   162,    87,
      -1,    -1,    -1,    -1,    12,    13,    -1,   171,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,   160,   161,   162,    83,    -1,    -1,    -1,    87,
      12,   169,    -1,   171,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    83,   160,   161,   162,    87,    -1,    -1,    -1,    -1,
      12,    -1,    -1,   171,    96,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,   160,   161,
     162,    83,    -1,    -1,    -1,    87,    12,    13,    -1,   171,
      -1,    -1,    -1,    19,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    83,   160,   161,
     162,    87,    -1,    -1,    -1,    -1,    12,    -1,    -1,   171,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,   160,   161,   162,    83,    -1,    -1,
      -1,    87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    83,   160,   161,   162,    87,    -1,    -1,
      -1,   167,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
     160,   161,   162,    83,    -1,    -1,    -1,    87,    12,   169,
      -1,   171,    -1,    -1,    -1,    19,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    83,
     160,   161,   162,    87,    -1,    -1,    -1,    -1,    12,    -1,
      -1,   171,    96,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,   160,   161,   162,    83,
      -1,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,    -1,
      -1,    19,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    83,   160,   161,   162,    87,
      -1,    -1,    -1,    -1,    12,    -1,    -1,   171,    96,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,   160,   161,   162,    83,    -1,    -1,    -1,    87,
      12,    -1,    -1,   171,    -1,    -1,    -1,    19,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    83,   160,   161,   162,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   171,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    57,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    87,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      83,   160,   161,   162,    87,    -1,    -1,    -1,    -1,    12,
      -1,    -1,   171,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,   160,   161,   162,
      83,    -1,    -1,    -1,    87,    12,   169,    -1,   171,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    83,   160,   161,   162,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    31,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    22,   160,   161,   162,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,   171,    35,    -1,    37,    38,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    87,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    77,    -1,    -1,
      80,    -1,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,   103,    -1,    -1,   106,    -1,    -1,   109,
     134,   135,    -1,   137,   138,   139,    83,   141,   142,   143,
      87,   145,   146,   147,   148,    -1,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,   118,    -1,    -1,    83,    -1,    -1,    -1,    87,   159,
      -1,    -1,    -1,    -1,    -1,   165,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   171,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
      -1,    -1,   151,   152,   153,   154,   155,     1,   157,     3,
      -1,   160,   161,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    65,    66,    17,    18,    -1,    20,    -1,    72,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    81,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    90,    91,    -1,    93,
      44,    95,    -1,    97,    48,    -1,   100,    51,    -1,     3,
     104,    -1,    -1,   107,    -1,    59,   110,    -1,    -1,    -1,
      -1,    65,    66,    17,    18,    -1,    20,    -1,    72,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    81,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    90,    91,    -1,    93,
      44,    95,    -1,    97,    48,    -1,   100,    51,    -1,     3,
     104,    -1,    -1,   107,    -1,    59,   110,    -1,    -1,    -1,
      -1,    65,    66,    17,    18,    -1,    20,    -1,    72,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    81,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    90,    91,    -1,    93,
      44,    95,    -1,    97,    48,    -1,   100,    51,    -1,     3,
     104,    -1,    -1,   107,    -1,    59,   110,    -1,    -1,    -1,
      -1,    65,    66,    17,    18,    -1,    20,    -1,    72,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    81,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    90,    91,    -1,    93,
      44,    95,    -1,    97,    48,    -1,   100,    51,    -1,     3,
     104,    -1,    -1,   107,    -1,    59,   110,    -1,    -1,    -1,
      -1,    65,    66,    17,    18,    -1,    20,    -1,    72,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    81,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    90,    91,    -1,    93,
      44,    95,    -1,    97,    48,    -1,   100,    51,    -1,     3,
     104,    -1,    -1,   107,    -1,    59,   110,    -1,    -1,    -1,
      -1,    65,    66,    17,    18,    -1,    20,    -1,    72,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    81,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    90,    91,    -1,    93,
      44,    95,    -1,    97,    48,    -1,   100,    51,    -1,    -1,
     104,    -1,    -1,   107,    -1,    59,   110,    -1,    -1,    -1,
      -1,    65,    66,    -1,    -1,    22,    -1,    -1,    72,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    81,    35,    -1,
      37,    38,    -1,    -1,    -1,    42,    90,    91,    45,    93,
      -1,    95,    49,    97,    -1,    52,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,   106,
      -1,   108,   109
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
     321,   323,   324,   325,   326,   327,   159,   180,   311,   114,
     300,   301,     3,   201,    14,    22,    35,    40,    41,    43,
      82,    95,   164,   168,   240,   293,   298,   309,   310,   311,
     314,   316,   317,   300,   311,   103,   272,    84,   201,   180,
     287,   311,   180,   166,     8,    82,   311,   312,     8,    11,
      82,   103,   312,    73,   116,   221,   311,   221,   311,    26,
     273,   311,   311,   187,   166,     3,    17,    18,    20,    25,
      33,    39,    44,    48,    51,    59,    65,    66,    72,    81,
      90,    91,    93,    95,    97,   100,   104,   107,   110,   203,
     205,   241,   166,   203,   274,   275,    32,   188,   208,   311,
     311,    18,    72,    90,   107,   311,   311,   311,     8,   166,
     211,    22,    35,    38,    82,   212,     4,   263,   286,   311,
     101,   102,   159,   311,   313,   311,   208,   311,   311,   311,
      94,   166,   180,   311,   311,   188,   200,   311,   314,   188,
     200,   311,   203,   270,   311,   311,   311,   311,   311,   311,
     311,   311,     1,   165,   178,   189,   286,   105,   144,   263,
     288,   289,   313,   221,   286,   311,   322,   311,    75,   180,
     164,    79,   184,    45,   108,   203,   203,    52,    78,   258,
     274,   158,   159,   150,   311,    12,    19,    31,    83,    87,
     118,   134,   135,   137,   138,   139,   141,   142,   143,   145,
     146,   147,   148,   149,   151,   152,   153,   154,   155,   156,
     157,   160,   161,   162,   171,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     163,   250,   166,   168,    83,    87,   166,   180,   159,   311,
     311,   311,   286,   164,   188,    46,   300,   270,   274,   159,
     140,   159,   113,   204,   263,   290,   291,   292,   313,    82,
     217,   244,   272,    82,   215,   270,   244,   272,   203,   166,
     208,    32,    46,   208,   114,   208,   303,    32,    46,   208,
     303,    36,    68,   159,    96,   119,   188,   250,    77,    80,
     185,   290,   180,   166,   252,   105,   166,   203,   276,   277,
       1,   139,   281,    46,   140,   180,   208,   166,   166,   208,
     290,   212,   140,   159,   311,   311,   159,   164,   208,   166,
     290,   159,   225,   159,   225,   159,   119,   271,   159,   208,
     159,   165,   165,   178,   140,   165,   311,   140,   167,   140,
     167,   169,   303,    46,   140,   169,   303,   117,   140,   169,
       8,     1,   165,   189,    61,   197,   198,   311,   191,   311,
     139,   234,   164,   245,   159,   311,   311,   311,   311,   221,
     311,   221,   311,   311,   311,   311,   311,   311,   311,    20,
      33,    59,   103,   204,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,    64,   313,   313,   313,   313,   313,   290,   290,   221,
     311,   221,   311,    27,    46,    84,   109,   302,   305,   306,
     311,   327,    32,    46,    32,    46,    96,    46,   169,   221,
     311,   208,   159,   159,   311,   311,   119,   167,   140,   218,
     203,   274,   216,   203,   159,   274,    46,   286,    43,   311,
     221,   311,   166,   208,    43,   311,   221,   311,   208,   162,
     190,   193,   311,   190,   192,   180,   311,    34,   311,   184,
     167,    27,    46,    50,    70,    73,    84,   103,   179,   253,
     254,   255,   256,   242,   277,   140,   167,    33,    48,    91,
      95,   168,   207,   282,   294,   119,   278,   311,   275,   311,
     311,   167,   263,   311,     1,   230,   290,   167,    21,   226,
     282,   294,   140,   165,   167,   167,   288,   167,   288,   180,
     169,   221,   311,   169,   180,   311,   169,   311,   169,   311,
     165,   165,   203,   140,   159,    13,   142,   140,   159,    13,
      36,    68,   286,   164,     1,   203,   238,   239,    27,    73,
      84,   103,   247,   257,   166,   159,   159,   159,   159,   159,
     159,   167,   169,    46,    84,   140,   167,   293,    83,    83,
      43,   221,   311,    43,   221,   311,   221,   311,   300,   300,
     159,   263,   313,   292,   203,   234,   159,   203,   234,   159,
     311,   167,   311,    32,   208,    32,   208,   304,   305,   311,
      32,   208,   303,    32,   208,   303,   140,   159,    13,   159,
      34,    34,   180,    96,   188,    61,    46,    84,   255,   140,
     167,   166,   203,    27,    73,    84,   103,   259,   167,   277,
     281,     1,   286,    64,   313,   167,   167,   165,    69,   111,
     165,   231,   167,   166,   188,   203,   227,   270,   180,   169,
     303,   169,   303,   180,   117,   197,   204,   164,   311,   105,
     311,   193,   192,     1,   235,   165,   119,   140,   165,    84,
     246,     1,     3,    12,    17,    19,    20,    25,    39,    44,
      51,    53,    59,    65,    66,    81,    93,    97,   100,   104,
     110,   134,   135,   136,   137,   138,   141,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   157,   160,
     161,   162,   163,   166,   202,   203,   205,   248,   249,   250,
     293,   167,   305,   281,   293,   293,   311,    32,    32,   311,
      32,    32,   169,   169,   208,   208,   234,   164,   234,   164,
     208,    96,    43,   311,    43,   311,   140,   167,    96,    43,
     311,   208,    43,   311,   208,   311,   311,   180,   311,   180,
      34,   203,   203,   254,   277,   139,   285,    84,   281,   278,
     169,    46,   169,   166,   166,    32,   180,   286,   227,   139,
     188,    43,   180,   311,   169,    43,   180,   311,   169,   311,
     190,    13,    36,    68,    36,    68,   159,   159,   165,    77,
      80,   165,   179,   210,   311,   239,   259,   166,   251,   311,
     134,   142,   251,   251,   278,    96,    43,    43,    96,    43,
      43,    43,    43,   164,   235,   164,   235,   311,   311,   311,
     305,   311,   311,   311,    13,    34,   180,   159,   285,   167,
     168,   207,   263,   284,   294,   144,   264,   278,    57,   112,
     265,   311,   282,   294,   290,   290,   180,   208,   167,   311,
     311,   180,   311,   180,   165,   105,   311,   193,   192,   193,
     192,   210,     1,   139,   280,   253,   167,     3,    97,   249,
     250,   311,   311,   311,   311,   311,   311,   235,   165,   235,
     165,    96,    96,    96,    96,   311,   180,   264,   278,   285,
     169,   286,   263,   311,     3,    86,    97,   266,   267,   268,
     311,   188,   209,   262,   169,   167,   167,   188,    96,    96,
     159,   159,   159,   159,   168,   207,   279,   294,    99,   260,
     167,   251,   251,    96,    96,    96,    96,    96,    96,   165,
     165,   311,   311,   311,   311,   264,   278,   263,   283,   284,
     294,    46,   169,   268,   112,   140,   119,   145,   147,   148,
     151,   152,    57,   294,   158,   158,   311,   311,     1,   169,
     286,   265,   311,   283,   284,   311,   267,   268,   268,   268,
     268,   268,   268,   266,   169,   279,   294,   169,   159,   261,
     262,   169,   279,   294,   283
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
     314,   314,   314,   314,   315,   315,   315,   315,   315,   316,
     316,   316,   317,   317,   317,   317,   317,   318,   318,   318,
     318,   319,   319,   320,   320,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   322,   322,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   324,   324,   324,   324,   324,   324,   324,
     325,   325,   325,   325,   326,   326,   326,   326,   327,   327,
     327,   327,   327,   327,   327
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
       1,     1,     1,     1,     1,     2,     2,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     5,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     3,     4,     3,     4,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1
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
#line 6538 "bison-chapel.cpp"
    break;

  case 3:
#line 511 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6544 "bison-chapel.cpp"
    break;

  case 4:
#line 512 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6550 "bison-chapel.cpp"
    break;

  case 6:
#line 519 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6556 "bison-chapel.cpp"
    break;

  case 7:
#line 524 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6562 "bison-chapel.cpp"
    break;

  case 8:
#line 525 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6568 "bison-chapel.cpp"
    break;

  case 25:
#line 546 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6574 "bison-chapel.cpp"
    break;

  case 26:
#line 547 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6580 "bison-chapel.cpp"
    break;

  case 27:
#line 548 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6586 "bison-chapel.cpp"
    break;

  case 28:
#line 549 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6592 "bison-chapel.cpp"
    break;

  case 29:
#line 550 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6598 "bison-chapel.cpp"
    break;

  case 30:
#line 551 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6604 "bison-chapel.cpp"
    break;

  case 31:
#line 552 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6610 "bison-chapel.cpp"
    break;

  case 32:
#line 553 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6616 "bison-chapel.cpp"
    break;

  case 33:
#line 554 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6622 "bison-chapel.cpp"
    break;

  case 34:
#line 555 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6628 "bison-chapel.cpp"
    break;

  case 35:
#line 556 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6634 "bison-chapel.cpp"
    break;

  case 36:
#line 557 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6640 "bison-chapel.cpp"
    break;

  case 37:
#line 558 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6646 "bison-chapel.cpp"
    break;

  case 38:
#line 559 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6652 "bison-chapel.cpp"
    break;

  case 39:
#line 560 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6658 "bison-chapel.cpp"
    break;

  case 40:
#line 561 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6664 "bison-chapel.cpp"
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
#line 6678 "bison-chapel.cpp"
    break;

  case 42:
#line 579 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6687 "bison-chapel.cpp"
    break;

  case 43:
#line 584 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6696 "bison-chapel.cpp"
    break;

  case 44:
#line 589 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6705 "bison-chapel.cpp"
    break;

  case 45:
#line 596 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6711 "bison-chapel.cpp"
    break;

  case 46:
#line 597 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6717 "bison-chapel.cpp"
    break;

  case 47:
#line 598 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6723 "bison-chapel.cpp"
    break;

  case 48:
#line 602 "chapel.ypp"
             { (yyval.b) = false; }
#line 6729 "bison-chapel.cpp"
    break;

  case 49:
#line 603 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6735 "bison-chapel.cpp"
    break;

  case 50:
#line 607 "chapel.ypp"
           { (yyval.b) = false; }
#line 6741 "bison-chapel.cpp"
    break;

  case 51:
#line 608 "chapel.ypp"
           { (yyval.b) = false; }
#line 6747 "bison-chapel.cpp"
    break;

  case 52:
#line 609 "chapel.ypp"
           { (yyval.b) = true; }
#line 6753 "bison-chapel.cpp"
    break;

  case 53:
#line 614 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6762 "bison-chapel.cpp"
    break;

  case 54:
#line 619 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6770 "bison-chapel.cpp"
    break;

  case 55:
#line 634 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6776 "bison-chapel.cpp"
    break;

  case 56:
#line 635 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6782 "bison-chapel.cpp"
    break;

  case 57:
#line 636 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6788 "bison-chapel.cpp"
    break;

  case 58:
#line 641 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6794 "bison-chapel.cpp"
    break;

  case 59:
#line 642 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6800 "bison-chapel.cpp"
    break;

  case 60:
#line 647 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6810 "bison-chapel.cpp"
    break;

  case 61:
#line 652 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6820 "bison-chapel.cpp"
    break;

  case 62:
#line 657 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6829 "bison-chapel.cpp"
    break;

  case 63:
#line 661 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6838 "bison-chapel.cpp"
    break;

  case 64:
#line 670 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6848 "bison-chapel.cpp"
    break;

  case 65:
#line 675 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6858 "bison-chapel.cpp"
    break;

  case 66:
#line 680 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6868 "bison-chapel.cpp"
    break;

  case 67:
#line 685 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6877 "bison-chapel.cpp"
    break;

  case 68:
#line 689 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6886 "bison-chapel.cpp"
    break;

  case 69:
#line 693 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6895 "bison-chapel.cpp"
    break;

  case 70:
#line 701 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6905 "bison-chapel.cpp"
    break;

  case 72:
#line 710 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6915 "bison-chapel.cpp"
    break;

  case 74:
#line 719 "chapel.ypp"
           { (yyval.b) = true; }
#line 6921 "bison-chapel.cpp"
    break;

  case 75:
#line 720 "chapel.ypp"
           { (yyval.b) = false; }
#line 6927 "bison-chapel.cpp"
    break;

  case 76:
#line 721 "chapel.ypp"
           { (yyval.b) = true; }
#line 6933 "bison-chapel.cpp"
    break;

  case 77:
#line 725 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 6939 "bison-chapel.cpp"
    break;

  case 78:
#line 726 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6945 "bison-chapel.cpp"
    break;

  case 79:
#line 727 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6951 "bison-chapel.cpp"
    break;

  case 80:
#line 728 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6957 "bison-chapel.cpp"
    break;

  case 81:
#line 729 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6963 "bison-chapel.cpp"
    break;

  case 82:
#line 730 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6969 "bison-chapel.cpp"
    break;

  case 83:
#line 731 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6975 "bison-chapel.cpp"
    break;

  case 84:
#line 735 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 6982 "bison-chapel.cpp"
    break;

  case 85:
#line 740 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 6988 "bison-chapel.cpp"
    break;

  case 86:
#line 741 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6994 "bison-chapel.cpp"
    break;

  case 87:
#line 742 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 7000 "bison-chapel.cpp"
    break;

  case 88:
#line 746 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 7006 "bison-chapel.cpp"
    break;

  case 89:
#line 747 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 7012 "bison-chapel.cpp"
    break;

  case 90:
#line 751 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 7018 "bison-chapel.cpp"
    break;

  case 91:
#line 756 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 7024 "bison-chapel.cpp"
    break;

  case 92:
#line 758 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 7030 "bison-chapel.cpp"
    break;

  case 93:
#line 760 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 7036 "bison-chapel.cpp"
    break;

  case 94:
#line 762 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 7042 "bison-chapel.cpp"
    break;

  case 95:
#line 764 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7048 "bison-chapel.cpp"
    break;

  case 96:
#line 766 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7054 "bison-chapel.cpp"
    break;

  case 97:
#line 772 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 7060 "bison-chapel.cpp"
    break;

  case 98:
#line 773 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 7066 "bison-chapel.cpp"
    break;

  case 99:
#line 777 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7072 "bison-chapel.cpp"
    break;

  case 100:
#line 778 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7078 "bison-chapel.cpp"
    break;

  case 101:
#line 779 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7084 "bison-chapel.cpp"
    break;

  case 102:
#line 780 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7090 "bison-chapel.cpp"
    break;

  case 103:
#line 781 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7096 "bison-chapel.cpp"
    break;

  case 104:
#line 782 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7102 "bison-chapel.cpp"
    break;

  case 105:
#line 785 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7108 "bison-chapel.cpp"
    break;

  case 106:
#line 786 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7114 "bison-chapel.cpp"
    break;

  case 107:
#line 787 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7120 "bison-chapel.cpp"
    break;

  case 108:
#line 788 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7126 "bison-chapel.cpp"
    break;

  case 109:
#line 789 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7132 "bison-chapel.cpp"
    break;

  case 110:
#line 790 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7138 "bison-chapel.cpp"
    break;

  case 111:
#line 802 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7144 "bison-chapel.cpp"
    break;

  case 112:
#line 803 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7150 "bison-chapel.cpp"
    break;

  case 113:
#line 814 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7156 "bison-chapel.cpp"
    break;

  case 114:
#line 815 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7162 "bison-chapel.cpp"
    break;

  case 115:
#line 816 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7168 "bison-chapel.cpp"
    break;

  case 116:
#line 817 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7174 "bison-chapel.cpp"
    break;

  case 117:
#line 818 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7180 "bison-chapel.cpp"
    break;

  case 118:
#line 819 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7186 "bison-chapel.cpp"
    break;

  case 119:
#line 820 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7192 "bison-chapel.cpp"
    break;

  case 120:
#line 821 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7198 "bison-chapel.cpp"
    break;

  case 121:
#line 822 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 7204 "bison-chapel.cpp"
    break;

  case 122:
#line 823 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 7210 "bison-chapel.cpp"
    break;

  case 123:
#line 824 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 7216 "bison-chapel.cpp"
    break;

  case 124:
#line 825 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 7222 "bison-chapel.cpp"
    break;

  case 125:
#line 826 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 7228 "bison-chapel.cpp"
    break;

  case 126:
#line 827 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 7234 "bison-chapel.cpp"
    break;

  case 127:
#line 828 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7240 "bison-chapel.cpp"
    break;

  case 128:
#line 829 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7246 "bison-chapel.cpp"
    break;

  case 129:
#line 830 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7252 "bison-chapel.cpp"
    break;

  case 130:
#line 831 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7258 "bison-chapel.cpp"
    break;

  case 131:
#line 832 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7264 "bison-chapel.cpp"
    break;

  case 132:
#line 836 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7270 "bison-chapel.cpp"
    break;

  case 133:
#line 837 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7276 "bison-chapel.cpp"
    break;

  case 134:
#line 838 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7282 "bison-chapel.cpp"
    break;

  case 135:
#line 839 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7288 "bison-chapel.cpp"
    break;

  case 136:
#line 840 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7294 "bison-chapel.cpp"
    break;

  case 137:
#line 841 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7300 "bison-chapel.cpp"
    break;

  case 138:
#line 842 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7306 "bison-chapel.cpp"
    break;

  case 139:
#line 843 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7312 "bison-chapel.cpp"
    break;

  case 140:
#line 844 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7318 "bison-chapel.cpp"
    break;

  case 141:
#line 845 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7324 "bison-chapel.cpp"
    break;

  case 142:
#line 846 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7330 "bison-chapel.cpp"
    break;

  case 143:
#line 847 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7336 "bison-chapel.cpp"
    break;

  case 144:
#line 854 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7342 "bison-chapel.cpp"
    break;

  case 145:
#line 855 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7348 "bison-chapel.cpp"
    break;

  case 146:
#line 856 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7354 "bison-chapel.cpp"
    break;

  case 147:
#line 857 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7360 "bison-chapel.cpp"
    break;

  case 148:
#line 861 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7366 "bison-chapel.cpp"
    break;

  case 149:
#line 862 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7372 "bison-chapel.cpp"
    break;

  case 150:
#line 866 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7378 "bison-chapel.cpp"
    break;

  case 151:
#line 867 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7384 "bison-chapel.cpp"
    break;

  case 152:
#line 871 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7390 "bison-chapel.cpp"
    break;

  case 154:
#line 873 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7396 "bison-chapel.cpp"
    break;

  case 155:
#line 883 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7402 "bison-chapel.cpp"
    break;

  case 156:
#line 884 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7408 "bison-chapel.cpp"
    break;

  case 164:
#line 898 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7414 "bison-chapel.cpp"
    break;

  case 165:
#line 899 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7420 "bison-chapel.cpp"
    break;

  case 166:
#line 900 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7426 "bison-chapel.cpp"
    break;

  case 167:
#line 901 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7432 "bison-chapel.cpp"
    break;

  case 168:
#line 906 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7441 "bison-chapel.cpp"
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
#line 7455 "bison-chapel.cpp"
    break;

  case 170:
#line 921 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7464 "bison-chapel.cpp"
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
#line 7479 "bison-chapel.cpp"
    break;

  case 172:
#line 937 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7488 "bison-chapel.cpp"
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
#line 7502 "bison-chapel.cpp"
    break;

  case 174:
#line 952 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7511 "bison-chapel.cpp"
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
#line 7525 "bison-chapel.cpp"
    break;

  case 176:
#line 968 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7533 "bison-chapel.cpp"
    break;

  case 177:
#line 972 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7541 "bison-chapel.cpp"
    break;

  case 178:
#line 977 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7553 "bison-chapel.cpp"
    break;

  case 179:
#line 985 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7565 "bison-chapel.cpp"
    break;

  case 180:
#line 996 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7573 "bison-chapel.cpp"
    break;

  case 181:
#line 1002 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7579 "bison-chapel.cpp"
    break;

  case 182:
#line 1003 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7585 "bison-chapel.cpp"
    break;

  case 183:
#line 1004 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7591 "bison-chapel.cpp"
    break;

  case 184:
#line 1005 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7597 "bison-chapel.cpp"
    break;

  case 185:
#line 1006 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7603 "bison-chapel.cpp"
    break;

  case 186:
#line 1007 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7609 "bison-chapel.cpp"
    break;

  case 187:
#line 1008 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7615 "bison-chapel.cpp"
    break;

  case 188:
#line 1009 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7621 "bison-chapel.cpp"
    break;

  case 189:
#line 1010 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7627 "bison-chapel.cpp"
    break;

  case 190:
#line 1011 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7633 "bison-chapel.cpp"
    break;

  case 191:
#line 1012 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7639 "bison-chapel.cpp"
    break;

  case 192:
#line 1013 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7645 "bison-chapel.cpp"
    break;

  case 193:
#line 1014 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7651 "bison-chapel.cpp"
    break;

  case 194:
#line 1015 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7657 "bison-chapel.cpp"
    break;

  case 195:
#line 1016 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7663 "bison-chapel.cpp"
    break;

  case 196:
#line 1017 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7669 "bison-chapel.cpp"
    break;

  case 197:
#line 1018 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7675 "bison-chapel.cpp"
    break;

  case 198:
#line 1019 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7681 "bison-chapel.cpp"
    break;

  case 199:
#line 1021 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7691 "bison-chapel.cpp"
    break;

  case 200:
#line 1027 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7701 "bison-chapel.cpp"
    break;

  case 201:
#line 1033 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7711 "bison-chapel.cpp"
    break;

  case 202:
#line 1039 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7721 "bison-chapel.cpp"
    break;

  case 203:
#line 1045 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7732 "bison-chapel.cpp"
    break;

  case 204:
#line 1052 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7743 "bison-chapel.cpp"
    break;

  case 205:
#line 1059 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7751 "bison-chapel.cpp"
    break;

  case 206:
#line 1063 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7759 "bison-chapel.cpp"
    break;

  case 207:
#line 1069 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7765 "bison-chapel.cpp"
    break;

  case 208:
#line 1073 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7771 "bison-chapel.cpp"
    break;

  case 209:
#line 1074 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7777 "bison-chapel.cpp"
    break;

  case 210:
#line 1075 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7783 "bison-chapel.cpp"
    break;

  case 211:
#line 1076 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7789 "bison-chapel.cpp"
    break;

  case 212:
#line 1077 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7796 "bison-chapel.cpp"
    break;

  case 213:
#line 1079 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7803 "bison-chapel.cpp"
    break;

  case 214:
#line 1081 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7810 "bison-chapel.cpp"
    break;

  case 215:
#line 1083 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7817 "bison-chapel.cpp"
    break;

  case 216:
#line 1088 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 7823 "bison-chapel.cpp"
    break;

  case 217:
#line 1091 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 7829 "bison-chapel.cpp"
    break;

  case 218:
#line 1092 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 7835 "bison-chapel.cpp"
    break;

  case 219:
#line 1093 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 7841 "bison-chapel.cpp"
    break;

  case 220:
#line 1094 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 7847 "bison-chapel.cpp"
    break;

  case 221:
#line 1095 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7853 "bison-chapel.cpp"
    break;

  case 222:
#line 1096 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7859 "bison-chapel.cpp"
    break;

  case 223:
#line 1100 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7865 "bison-chapel.cpp"
    break;

  case 224:
#line 1101 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7871 "bison-chapel.cpp"
    break;

  case 225:
#line 1105 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 7877 "bison-chapel.cpp"
    break;

  case 226:
#line 1106 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7883 "bison-chapel.cpp"
    break;

  case 227:
#line 1107 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7889 "bison-chapel.cpp"
    break;

  case 228:
#line 1111 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 7895 "bison-chapel.cpp"
    break;

  case 229:
#line 1112 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 7901 "bison-chapel.cpp"
    break;

  case 230:
#line 1116 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 7907 "bison-chapel.cpp"
    break;

  case 231:
#line 1120 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 7913 "bison-chapel.cpp"
    break;

  case 232:
#line 1122 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 7919 "bison-chapel.cpp"
    break;

  case 233:
#line 1126 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7925 "bison-chapel.cpp"
    break;

  case 234:
#line 1127 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7931 "bison-chapel.cpp"
    break;

  case 235:
#line 1132 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 7937 "bison-chapel.cpp"
    break;

  case 236:
#line 1134 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7943 "bison-chapel.cpp"
    break;

  case 237:
#line 1136 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7949 "bison-chapel.cpp"
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
#line 7963 "bison-chapel.cpp"
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
#line 7977 "bison-chapel.cpp"
    break;

  case 240:
#line 1166 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7987 "bison-chapel.cpp"
    break;

  case 241:
#line 1171 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7997 "bison-chapel.cpp"
    break;

  case 242:
#line 1176 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8007 "bison-chapel.cpp"
    break;

  case 243:
#line 1184 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 8013 "bison-chapel.cpp"
    break;

  case 244:
#line 1185 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8019 "bison-chapel.cpp"
    break;

  case 245:
#line 1190 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8025 "bison-chapel.cpp"
    break;

  case 246:
#line 1192 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8031 "bison-chapel.cpp"
    break;

  case 247:
#line 1194 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8037 "bison-chapel.cpp"
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
#line 8057 "bison-chapel.cpp"
    break;

  case 249:
#line 1215 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8065 "bison-chapel.cpp"
    break;

  case 250:
#line 1222 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8075 "bison-chapel.cpp"
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
#line 8087 "bison-chapel.cpp"
    break;

  case 252:
#line 1239 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8095 "bison-chapel.cpp"
    break;

  case 253:
#line 1243 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8103 "bison-chapel.cpp"
    break;

  case 254:
#line 1249 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8109 "bison-chapel.cpp"
    break;

  case 255:
#line 1250 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8115 "bison-chapel.cpp"
    break;

  case 256:
#line 1255 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8124 "bison-chapel.cpp"
    break;

  case 257:
#line 1260 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8133 "bison-chapel.cpp"
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
#line 8155 "bison-chapel.cpp"
    break;

  case 259:
#line 1287 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8163 "bison-chapel.cpp"
    break;

  case 260:
#line 1290 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8172 "bison-chapel.cpp"
    break;

  case 261:
#line 1294 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8181 "bison-chapel.cpp"
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
#line 8195 "bison-chapel.cpp"
    break;

  case 263:
#line 1312 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8206 "bison-chapel.cpp"
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
#line 8233 "bison-chapel.cpp"
    break;

  case 265:
#line 1345 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8241 "bison-chapel.cpp"
    break;

  case 266:
#line 1349 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8250 "bison-chapel.cpp"
    break;

  case 267:
#line 1354 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8258 "bison-chapel.cpp"
    break;

  case 268:
#line 1358 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8267 "bison-chapel.cpp"
    break;

  case 269:
#line 1363 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8275 "bison-chapel.cpp"
    break;

  case 271:
#line 1370 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8281 "bison-chapel.cpp"
    break;

  case 272:
#line 1374 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8287 "bison-chapel.cpp"
    break;

  case 273:
#line 1375 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8293 "bison-chapel.cpp"
    break;

  case 274:
#line 1376 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8299 "bison-chapel.cpp"
    break;

  case 275:
#line 1377 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8305 "bison-chapel.cpp"
    break;

  case 276:
#line 1378 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8311 "bison-chapel.cpp"
    break;

  case 277:
#line 1379 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8317 "bison-chapel.cpp"
    break;

  case 278:
#line 1380 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8323 "bison-chapel.cpp"
    break;

  case 279:
#line 1381 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8329 "bison-chapel.cpp"
    break;

  case 280:
#line 1382 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8335 "bison-chapel.cpp"
    break;

  case 281:
#line 1383 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8341 "bison-chapel.cpp"
    break;

  case 282:
#line 1384 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8347 "bison-chapel.cpp"
    break;

  case 283:
#line 1385 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8353 "bison-chapel.cpp"
    break;

  case 284:
#line 1386 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8359 "bison-chapel.cpp"
    break;

  case 285:
#line 1387 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8365 "bison-chapel.cpp"
    break;

  case 286:
#line 1388 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8371 "bison-chapel.cpp"
    break;

  case 287:
#line 1389 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8377 "bison-chapel.cpp"
    break;

  case 288:
#line 1390 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8383 "bison-chapel.cpp"
    break;

  case 289:
#line 1391 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8389 "bison-chapel.cpp"
    break;

  case 290:
#line 1392 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8395 "bison-chapel.cpp"
    break;

  case 291:
#line 1393 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8401 "bison-chapel.cpp"
    break;

  case 292:
#line 1394 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8407 "bison-chapel.cpp"
    break;

  case 293:
#line 1395 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8413 "bison-chapel.cpp"
    break;

  case 294:
#line 1396 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8419 "bison-chapel.cpp"
    break;

  case 295:
#line 1397 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 8425 "bison-chapel.cpp"
    break;

  case 296:
#line 1398 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 8431 "bison-chapel.cpp"
    break;

  case 297:
#line 1399 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 8437 "bison-chapel.cpp"
    break;

  case 298:
#line 1400 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8443 "bison-chapel.cpp"
    break;

  case 299:
#line 1404 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 8449 "bison-chapel.cpp"
    break;

  case 300:
#line 1405 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 8455 "bison-chapel.cpp"
    break;

  case 301:
#line 1406 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 8461 "bison-chapel.cpp"
    break;

  case 302:
#line 1407 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 8467 "bison-chapel.cpp"
    break;

  case 303:
#line 1408 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 8473 "bison-chapel.cpp"
    break;

  case 304:
#line 1409 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 8479 "bison-chapel.cpp"
    break;

  case 305:
#line 1410 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 8485 "bison-chapel.cpp"
    break;

  case 306:
#line 1411 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 8491 "bison-chapel.cpp"
    break;

  case 307:
#line 1412 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 8497 "bison-chapel.cpp"
    break;

  case 308:
#line 1413 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 8503 "bison-chapel.cpp"
    break;

  case 309:
#line 1414 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 8509 "bison-chapel.cpp"
    break;

  case 310:
#line 1415 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 8515 "bison-chapel.cpp"
    break;

  case 311:
#line 1419 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 8521 "bison-chapel.cpp"
    break;

  case 312:
#line 1420 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8527 "bison-chapel.cpp"
    break;

  case 313:
#line 1424 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8533 "bison-chapel.cpp"
    break;

  case 314:
#line 1428 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 8539 "bison-chapel.cpp"
    break;

  case 315:
#line 1429 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 8545 "bison-chapel.cpp"
    break;

  case 316:
#line 1430 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 8551 "bison-chapel.cpp"
    break;

  case 317:
#line 1435 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 8557 "bison-chapel.cpp"
    break;

  case 318:
#line 1437 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 8563 "bison-chapel.cpp"
    break;

  case 319:
#line 1439 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 8569 "bison-chapel.cpp"
    break;

  case 320:
#line 1441 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 8575 "bison-chapel.cpp"
    break;

  case 321:
#line 1443 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8581 "bison-chapel.cpp"
    break;

  case 322:
#line 1445 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 8587 "bison-chapel.cpp"
    break;

  case 323:
#line 1449 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 8593 "bison-chapel.cpp"
    break;

  case 324:
#line 1450 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 8599 "bison-chapel.cpp"
    break;

  case 325:
#line 1454 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 8605 "bison-chapel.cpp"
    break;

  case 326:
#line 1455 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 8611 "bison-chapel.cpp"
    break;

  case 327:
#line 1456 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 8617 "bison-chapel.cpp"
    break;

  case 328:
#line 1457 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 8623 "bison-chapel.cpp"
    break;

  case 329:
#line 1458 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 8629 "bison-chapel.cpp"
    break;

  case 330:
#line 1459 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 8635 "bison-chapel.cpp"
    break;

  case 331:
#line 1460 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 8641 "bison-chapel.cpp"
    break;

  case 332:
#line 1461 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 8647 "bison-chapel.cpp"
    break;

  case 333:
#line 1462 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 8653 "bison-chapel.cpp"
    break;

  case 334:
#line 1466 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 8659 "bison-chapel.cpp"
    break;

  case 335:
#line 1467 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 8665 "bison-chapel.cpp"
    break;

  case 336:
#line 1468 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 8671 "bison-chapel.cpp"
    break;

  case 337:
#line 1469 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 8677 "bison-chapel.cpp"
    break;

  case 338:
#line 1470 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 8683 "bison-chapel.cpp"
    break;

  case 339:
#line 1471 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 8689 "bison-chapel.cpp"
    break;

  case 340:
#line 1475 "chapel.ypp"
         { (yyval.procIter) = ProcIter_PROC; }
#line 8695 "bison-chapel.cpp"
    break;

  case 341:
#line 1476 "chapel.ypp"
         { (yyval.procIter) = ProcIter_ITER; }
#line 8701 "bison-chapel.cpp"
    break;

  case 342:
#line 1480 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 8707 "bison-chapel.cpp"
    break;

  case 343:
#line 1481 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 8713 "bison-chapel.cpp"
    break;

  case 344:
#line 1482 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 8719 "bison-chapel.cpp"
    break;

  case 345:
#line 1483 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 8725 "bison-chapel.cpp"
    break;

  case 346:
#line 1484 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 8731 "bison-chapel.cpp"
    break;

  case 347:
#line 1485 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 8737 "bison-chapel.cpp"
    break;

  case 348:
#line 1489 "chapel.ypp"
          { (yyval.b) = false; }
#line 8743 "bison-chapel.cpp"
    break;

  case 349:
#line 1490 "chapel.ypp"
          { (yyval.b) = true;  }
#line 8749 "bison-chapel.cpp"
    break;

  case 350:
#line 1493 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 8755 "bison-chapel.cpp"
    break;

  case 353:
#line 1499 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 8761 "bison-chapel.cpp"
    break;

  case 354:
#line 1503 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 8767 "bison-chapel.cpp"
    break;

  case 355:
#line 1507 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8773 "bison-chapel.cpp"
    break;

  case 356:
#line 1508 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8779 "bison-chapel.cpp"
    break;

  case 357:
#line 1509 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 8789 "bison-chapel.cpp"
    break;

  case 358:
#line 1517 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 8795 "bison-chapel.cpp"
    break;

  case 359:
#line 1519 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 8801 "bison-chapel.cpp"
    break;

  case 360:
#line 1521 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 8807 "bison-chapel.cpp"
    break;

  case 361:
#line 1523 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8813 "bison-chapel.cpp"
    break;

  case 362:
#line 1525 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8819 "bison-chapel.cpp"
    break;

  case 363:
#line 1530 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8825 "bison-chapel.cpp"
    break;

  case 364:
#line 1532 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8831 "bison-chapel.cpp"
    break;

  case 365:
#line 1536 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8837 "bison-chapel.cpp"
    break;

  case 366:
#line 1537 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8843 "bison-chapel.cpp"
    break;

  case 367:
#line 1538 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8849 "bison-chapel.cpp"
    break;

  case 368:
#line 1539 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8855 "bison-chapel.cpp"
    break;

  case 369:
#line 1540 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8861 "bison-chapel.cpp"
    break;

  case 370:
#line 1541 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8867 "bison-chapel.cpp"
    break;

  case 371:
#line 1542 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 8873 "bison-chapel.cpp"
    break;

  case 372:
#line 1547 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 8879 "bison-chapel.cpp"
    break;

  case 373:
#line 1549 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 8885 "bison-chapel.cpp"
    break;

  case 374:
#line 1554 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 8891 "bison-chapel.cpp"
    break;

  case 375:
#line 1556 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 8897 "bison-chapel.cpp"
    break;

  case 376:
#line 1558 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 8903 "bison-chapel.cpp"
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
#line 8920 "bison-chapel.cpp"
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
#line 8938 "bison-chapel.cpp"
    break;

  case 379:
#line 1592 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 8944 "bison-chapel.cpp"
    break;

  case 380:
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8950 "bison-chapel.cpp"
    break;

  case 381:
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 8956 "bison-chapel.cpp"
    break;

  case 382:
#line 1600 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 8962 "bison-chapel.cpp"
    break;

  case 383:
#line 1601 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 8968 "bison-chapel.cpp"
    break;

  case 384:
#line 1602 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 8974 "bison-chapel.cpp"
    break;

  case 385:
#line 1603 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 8980 "bison-chapel.cpp"
    break;

  case 386:
#line 1604 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 8986 "bison-chapel.cpp"
    break;

  case 387:
#line 1609 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8996 "bison-chapel.cpp"
    break;

  case 388:
#line 1615 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9005 "bison-chapel.cpp"
    break;

  case 390:
#line 1624 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9014 "bison-chapel.cpp"
    break;

  case 391:
#line 1632 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9020 "bison-chapel.cpp"
    break;

  case 392:
#line 1634 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9026 "bison-chapel.cpp"
    break;

  case 393:
#line 1639 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9032 "bison-chapel.cpp"
    break;

  case 394:
#line 1641 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9038 "bison-chapel.cpp"
    break;

  case 395:
#line 1643 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9044 "bison-chapel.cpp"
    break;

  case 396:
#line 1648 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9050 "bison-chapel.cpp"
    break;

  case 397:
#line 1650 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9056 "bison-chapel.cpp"
    break;

  case 398:
#line 1652 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9062 "bison-chapel.cpp"
    break;

  case 399:
#line 1658 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9068 "bison-chapel.cpp"
    break;

  case 400:
#line 1659 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9074 "bison-chapel.cpp"
    break;

  case 401:
#line 1660 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9080 "bison-chapel.cpp"
    break;

  case 402:
#line 1666 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9086 "bison-chapel.cpp"
    break;

  case 403:
#line 1668 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9092 "bison-chapel.cpp"
    break;

  case 404:
#line 1670 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9100 "bison-chapel.cpp"
    break;

  case 405:
#line 1674 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9108 "bison-chapel.cpp"
    break;

  case 406:
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9114 "bison-chapel.cpp"
    break;

  case 407:
#line 1680 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9122 "bison-chapel.cpp"
    break;

  case 408:
#line 1684 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9130 "bison-chapel.cpp"
    break;

  case 409:
#line 1691 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9136 "bison-chapel.cpp"
    break;

  case 410:
#line 1692 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9142 "bison-chapel.cpp"
    break;

  case 411:
#line 1693 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9148 "bison-chapel.cpp"
    break;

  case 412:
#line 1694 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9154 "bison-chapel.cpp"
    break;

  case 413:
#line 1695 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9160 "bison-chapel.cpp"
    break;

  case 414:
#line 1700 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9166 "bison-chapel.cpp"
    break;

  case 415:
#line 1701 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9172 "bison-chapel.cpp"
    break;

  case 416:
#line 1702 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9178 "bison-chapel.cpp"
    break;

  case 417:
#line 1703 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9184 "bison-chapel.cpp"
    break;

  case 418:
#line 1704 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9190 "bison-chapel.cpp"
    break;

  case 419:
#line 1725 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9198 "bison-chapel.cpp"
    break;

  case 420:
#line 1729 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9206 "bison-chapel.cpp"
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
#line 9218 "bison-chapel.cpp"
    break;

  case 422:
#line 1741 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9226 "bison-chapel.cpp"
    break;

  case 423:
#line 1747 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9232 "bison-chapel.cpp"
    break;

  case 424:
#line 1748 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9238 "bison-chapel.cpp"
    break;

  case 425:
#line 1749 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9244 "bison-chapel.cpp"
    break;

  case 426:
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9250 "bison-chapel.cpp"
    break;

  case 427:
#line 1756 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9256 "bison-chapel.cpp"
    break;

  case 428:
#line 1762 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9262 "bison-chapel.cpp"
    break;

  case 429:
#line 1764 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9268 "bison-chapel.cpp"
    break;

  case 430:
#line 1766 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9274 "bison-chapel.cpp"
    break;

  case 431:
#line 1770 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9280 "bison-chapel.cpp"
    break;

  case 432:
#line 1771 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9286 "bison-chapel.cpp"
    break;

  case 433:
#line 1772 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9292 "bison-chapel.cpp"
    break;

  case 434:
#line 1773 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9298 "bison-chapel.cpp"
    break;

  case 435:
#line 1774 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9304 "bison-chapel.cpp"
    break;

  case 436:
#line 1780 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9310 "bison-chapel.cpp"
    break;

  case 437:
#line 1781 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9316 "bison-chapel.cpp"
    break;

  case 438:
#line 1782 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9322 "bison-chapel.cpp"
    break;

  case 439:
#line 1783 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9328 "bison-chapel.cpp"
    break;

  case 440:
#line 1787 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9334 "bison-chapel.cpp"
    break;

  case 441:
#line 1788 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9340 "bison-chapel.cpp"
    break;

  case 442:
#line 1792 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9346 "bison-chapel.cpp"
    break;

  case 443:
#line 1793 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9352 "bison-chapel.cpp"
    break;

  case 444:
#line 1794 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9358 "bison-chapel.cpp"
    break;

  case 445:
#line 1798 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9364 "bison-chapel.cpp"
    break;

  case 446:
#line 1799 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9370 "bison-chapel.cpp"
    break;

  case 447:
#line 1803 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9376 "bison-chapel.cpp"
    break;

  case 449:
#line 1808 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9382 "bison-chapel.cpp"
    break;

  case 450:
#line 1809 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9388 "bison-chapel.cpp"
    break;

  case 451:
#line 1813 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9394 "bison-chapel.cpp"
    break;

  case 452:
#line 1814 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9400 "bison-chapel.cpp"
    break;

  case 453:
#line 1815 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9406 "bison-chapel.cpp"
    break;

  case 454:
#line 1816 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9412 "bison-chapel.cpp"
    break;

  case 455:
#line 1820 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9418 "bison-chapel.cpp"
    break;

  case 456:
#line 1821 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9424 "bison-chapel.cpp"
    break;

  case 457:
#line 1833 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9430 "bison-chapel.cpp"
    break;

  case 458:
#line 1835 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9436 "bison-chapel.cpp"
    break;

  case 459:
#line 1837 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9442 "bison-chapel.cpp"
    break;

  case 465:
#line 1848 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9448 "bison-chapel.cpp"
    break;

  case 466:
#line 1850 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9454 "bison-chapel.cpp"
    break;

  case 467:
#line 1852 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9460 "bison-chapel.cpp"
    break;

  case 468:
#line 1854 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9466 "bison-chapel.cpp"
    break;

  case 469:
#line 1856 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 9472 "bison-chapel.cpp"
    break;

  case 470:
#line 1858 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9478 "bison-chapel.cpp"
    break;

  case 471:
#line 1860 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9484 "bison-chapel.cpp"
    break;

  case 472:
#line 1863 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 9490 "bison-chapel.cpp"
    break;

  case 473:
#line 1865 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 9496 "bison-chapel.cpp"
    break;

  case 474:
#line 1867 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 9502 "bison-chapel.cpp"
    break;

  case 475:
#line 1869 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9508 "bison-chapel.cpp"
    break;

  case 476:
#line 1871 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 9514 "bison-chapel.cpp"
    break;

  case 477:
#line 1873 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 9520 "bison-chapel.cpp"
    break;

  case 478:
#line 1875 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 9526 "bison-chapel.cpp"
    break;

  case 479:
#line 1877 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9532 "bison-chapel.cpp"
    break;

  case 480:
#line 1880 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 9538 "bison-chapel.cpp"
    break;

  case 481:
#line 1882 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 9544 "bison-chapel.cpp"
    break;

  case 482:
#line 1887 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9550 "bison-chapel.cpp"
    break;

  case 483:
#line 1889 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9556 "bison-chapel.cpp"
    break;

  case 484:
#line 1891 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9562 "bison-chapel.cpp"
    break;

  case 485:
#line 1893 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9568 "bison-chapel.cpp"
    break;

  case 486:
#line 1895 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9574 "bison-chapel.cpp"
    break;

  case 487:
#line 1897 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9580 "bison-chapel.cpp"
    break;

  case 488:
#line 1899 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9586 "bison-chapel.cpp"
    break;

  case 489:
#line 1901 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9592 "bison-chapel.cpp"
    break;

  case 490:
#line 1903 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9598 "bison-chapel.cpp"
    break;

  case 491:
#line 1905 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9604 "bison-chapel.cpp"
    break;

  case 492:
#line 1907 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9610 "bison-chapel.cpp"
    break;

  case 493:
#line 1909 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9616 "bison-chapel.cpp"
    break;

  case 494:
#line 1911 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 9627 "bison-chapel.cpp"
    break;

  case 495:
#line 1918 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 9637 "bison-chapel.cpp"
    break;

  case 496:
#line 1924 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 9647 "bison-chapel.cpp"
    break;

  case 497:
#line 1930 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9657 "bison-chapel.cpp"
    break;

  case 498:
#line 1936 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 9667 "bison-chapel.cpp"
    break;

  case 499:
#line 1945 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9673 "bison-chapel.cpp"
    break;

  case 500:
#line 1954 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 9679 "bison-chapel.cpp"
    break;

  case 508:
#line 1970 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9685 "bison-chapel.cpp"
    break;

  case 509:
#line 1974 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 9691 "bison-chapel.cpp"
    break;

  case 511:
#line 1979 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9697 "bison-chapel.cpp"
    break;

  case 512:
#line 1983 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9703 "bison-chapel.cpp"
    break;

  case 513:
#line 1984 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9709 "bison-chapel.cpp"
    break;

  case 514:
#line 1988 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9715 "bison-chapel.cpp"
    break;

  case 515:
#line 1992 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9721 "bison-chapel.cpp"
    break;

  case 516:
#line 1993 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9727 "bison-chapel.cpp"
    break;

  case 517:
#line 1998 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 9735 "bison-chapel.cpp"
    break;

  case 518:
#line 2002 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9743 "bison-chapel.cpp"
    break;

  case 519:
#line 2006 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9751 "bison-chapel.cpp"
    break;

  case 520:
#line 2012 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 9757 "bison-chapel.cpp"
    break;

  case 521:
#line 2013 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 9763 "bison-chapel.cpp"
    break;

  case 522:
#line 2014 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 9769 "bison-chapel.cpp"
    break;

  case 523:
#line 2015 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 9775 "bison-chapel.cpp"
    break;

  case 524:
#line 2016 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 9781 "bison-chapel.cpp"
    break;

  case 525:
#line 2017 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 9787 "bison-chapel.cpp"
    break;

  case 527:
#line 2023 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9793 "bison-chapel.cpp"
    break;

  case 528:
#line 2028 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 9799 "bison-chapel.cpp"
    break;

  case 529:
#line 2030 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 9807 "bison-chapel.cpp"
    break;

  case 530:
#line 2034 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 9815 "bison-chapel.cpp"
    break;

  case 531:
#line 2038 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 9823 "bison-chapel.cpp"
    break;

  case 532:
#line 2042 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 9831 "bison-chapel.cpp"
    break;

  case 533:
#line 2050 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 9838 "bison-chapel.cpp"
    break;

  case 534:
#line 2055 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9848 "bison-chapel.cpp"
    break;

  case 535:
#line 2061 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9858 "bison-chapel.cpp"
    break;

  case 536:
#line 2067 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9869 "bison-chapel.cpp"
    break;

  case 537:
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9880 "bison-chapel.cpp"
    break;

  case 538:
#line 2084 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 9886 "bison-chapel.cpp"
    break;

  case 547:
#line 2100 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 9892 "bison-chapel.cpp"
    break;

  case 548:
#line 2102 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9898 "bison-chapel.cpp"
    break;

  case 549:
#line 2104 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9904 "bison-chapel.cpp"
    break;

  case 550:
#line 2106 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 9910 "bison-chapel.cpp"
    break;

  case 551:
#line 2121 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 9916 "bison-chapel.cpp"
    break;

  case 552:
#line 2123 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 9922 "bison-chapel.cpp"
    break;

  case 553:
#line 2125 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 9928 "bison-chapel.cpp"
    break;

  case 554:
#line 2127 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 9934 "bison-chapel.cpp"
    break;

  case 555:
#line 2131 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 9940 "bison-chapel.cpp"
    break;

  case 556:
#line 2132 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9946 "bison-chapel.cpp"
    break;

  case 557:
#line 2136 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 9952 "bison-chapel.cpp"
    break;

  case 558:
#line 2137 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 9958 "bison-chapel.cpp"
    break;

  case 559:
#line 2138 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9964 "bison-chapel.cpp"
    break;

  case 564:
#line 2154 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9970 "bison-chapel.cpp"
    break;

  case 565:
#line 2155 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9976 "bison-chapel.cpp"
    break;

  case 566:
#line 2156 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 9982 "bison-chapel.cpp"
    break;

  case 567:
#line 2157 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9988 "bison-chapel.cpp"
    break;

  case 569:
#line 2162 "chapel.ypp"
                                              { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9994 "bison-chapel.cpp"
    break;

  case 570:
#line 2163 "chapel.ypp"
                                              { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10000 "bison-chapel.cpp"
    break;

  case 571:
#line 2164 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10006 "bison-chapel.cpp"
    break;

  case 572:
#line 2168 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10012 "bison-chapel.cpp"
    break;

  case 573:
#line 2169 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10018 "bison-chapel.cpp"
    break;

  case 574:
#line 2170 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10024 "bison-chapel.cpp"
    break;

  case 575:
#line 2171 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10030 "bison-chapel.cpp"
    break;

  case 576:
#line 2172 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10036 "bison-chapel.cpp"
    break;

  case 577:
#line 2180 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10042 "bison-chapel.cpp"
    break;

  case 578:
#line 2181 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10048 "bison-chapel.cpp"
    break;

  case 579:
#line 2182 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10054 "bison-chapel.cpp"
    break;

  case 580:
#line 2183 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10060 "bison-chapel.cpp"
    break;

  case 581:
#line 2187 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10066 "bison-chapel.cpp"
    break;

  case 582:
#line 2188 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10072 "bison-chapel.cpp"
    break;

  case 583:
#line 2192 "chapel.ypp"
                  { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10078 "bison-chapel.cpp"
    break;

  case 584:
#line 2193 "chapel.ypp"
                  { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10084 "bison-chapel.cpp"
    break;

  case 587:
#line 2199 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10090 "bison-chapel.cpp"
    break;

  case 588:
#line 2200 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10096 "bison-chapel.cpp"
    break;

  case 589:
#line 2201 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10102 "bison-chapel.cpp"
    break;

  case 590:
#line 2202 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10108 "bison-chapel.cpp"
    break;

  case 591:
#line 2203 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10114 "bison-chapel.cpp"
    break;

  case 592:
#line 2204 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10121 "bison-chapel.cpp"
    break;

  case 593:
#line 2206 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10128 "bison-chapel.cpp"
    break;

  case 594:
#line 2208 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10134 "bison-chapel.cpp"
    break;

  case 595:
#line 2209 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10140 "bison-chapel.cpp"
    break;

  case 596:
#line 2211 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10148 "bison-chapel.cpp"
    break;

  case 597:
#line 2215 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10156 "bison-chapel.cpp"
    break;

  case 598:
#line 2222 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10162 "bison-chapel.cpp"
    break;

  case 599:
#line 2223 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10168 "bison-chapel.cpp"
    break;

  case 600:
#line 2227 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10174 "bison-chapel.cpp"
    break;

  case 601:
#line 2228 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10180 "bison-chapel.cpp"
    break;

  case 602:
#line 2229 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10186 "bison-chapel.cpp"
    break;

  case 603:
#line 2230 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10192 "bison-chapel.cpp"
    break;

  case 604:
#line 2231 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10198 "bison-chapel.cpp"
    break;

  case 605:
#line 2232 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10204 "bison-chapel.cpp"
    break;

  case 606:
#line 2233 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10210 "bison-chapel.cpp"
    break;

  case 607:
#line 2234 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10216 "bison-chapel.cpp"
    break;

  case 608:
#line 2235 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10222 "bison-chapel.cpp"
    break;

  case 609:
#line 2236 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10228 "bison-chapel.cpp"
    break;

  case 610:
#line 2237 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10234 "bison-chapel.cpp"
    break;

  case 611:
#line 2238 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10240 "bison-chapel.cpp"
    break;

  case 612:
#line 2239 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10246 "bison-chapel.cpp"
    break;

  case 613:
#line 2240 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10252 "bison-chapel.cpp"
    break;

  case 614:
#line 2241 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10258 "bison-chapel.cpp"
    break;

  case 615:
#line 2242 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10264 "bison-chapel.cpp"
    break;

  case 616:
#line 2243 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10270 "bison-chapel.cpp"
    break;

  case 617:
#line 2244 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10276 "bison-chapel.cpp"
    break;

  case 618:
#line 2245 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10282 "bison-chapel.cpp"
    break;

  case 619:
#line 2246 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10288 "bison-chapel.cpp"
    break;

  case 620:
#line 2247 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10294 "bison-chapel.cpp"
    break;

  case 621:
#line 2248 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10300 "bison-chapel.cpp"
    break;

  case 622:
#line 2249 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10307 "bison-chapel.cpp"
    break;

  case 623:
#line 2254 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10313 "bison-chapel.cpp"
    break;

  case 624:
#line 2255 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10319 "bison-chapel.cpp"
    break;

  case 625:
#line 2256 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10325 "bison-chapel.cpp"
    break;

  case 626:
#line 2257 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10331 "bison-chapel.cpp"
    break;

  case 627:
#line 2258 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10337 "bison-chapel.cpp"
    break;

  case 628:
#line 2259 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10343 "bison-chapel.cpp"
    break;

  case 629:
#line 2260 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10349 "bison-chapel.cpp"
    break;

  case 630:
#line 2264 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10355 "bison-chapel.cpp"
    break;

  case 631:
#line 2265 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10361 "bison-chapel.cpp"
    break;

  case 632:
#line 2266 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10367 "bison-chapel.cpp"
    break;

  case 633:
#line 2267 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10373 "bison-chapel.cpp"
    break;

  case 634:
#line 2271 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10379 "bison-chapel.cpp"
    break;

  case 635:
#line 2272 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10385 "bison-chapel.cpp"
    break;

  case 636:
#line 2273 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10391 "bison-chapel.cpp"
    break;

  case 637:
#line 2274 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10397 "bison-chapel.cpp"
    break;

  case 638:
#line 2279 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10403 "bison-chapel.cpp"
    break;

  case 639:
#line 2280 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10409 "bison-chapel.cpp"
    break;

  case 640:
#line 2281 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10415 "bison-chapel.cpp"
    break;

  case 641:
#line 2282 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10421 "bison-chapel.cpp"
    break;

  case 642:
#line 2283 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10427 "bison-chapel.cpp"
    break;

  case 643:
#line 2284 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10433 "bison-chapel.cpp"
    break;

  case 644:
#line 2285 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10439 "bison-chapel.cpp"
    break;


#line 10443 "bison-chapel.cpp"

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
