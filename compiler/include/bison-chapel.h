/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

#ifndef YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED
# define YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
/* Line 2058 of yacc.c  */
#line 32 "chapel.ypp"

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;

/* Line 2058 of yacc.c  */
#line 45 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif

/* Line 2058 of yacc.c  */
#line 65 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_1_
  #define _BISON_CHAPEL_DEFINES_1_

  #include "symbol.h"

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
  class Type;

  enum   ProcIter {
    ProcIter_PROC,
    ProcIter_ITER
  };

  struct OnlyRename {
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
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<OnlyRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif

/* Line 2058 of yacc.c  */
#line 145 "chapel.ypp"

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

/* Line 2058 of yacc.c  */
#line 168 "chapel.ypp"

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


/* Line 2058 of yacc.c  */
#line 196 "../include/bison-chapel.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TIDENT = 258,
     INTLITERAL = 259,
     REALLITERAL = 260,
     IMAGLITERAL = 261,
     STRINGLITERAL = 262,
     BYTESLITERAL = 263,
     CSTRINGLITERAL = 264,
     EXTERNCODE = 265,
     TALIGN = 266,
     TAS = 267,
     TATOMIC = 268,
     TBEGIN = 269,
     TBREAK = 270,
     TBOOL = 271,
     TBORROWED = 272,
     TBY = 273,
     TBYTES = 274,
     TCATCH = 275,
     TCLASS = 276,
     TCOBEGIN = 277,
     TCOFORALL = 278,
     TCOMPLEX = 279,
     TCONFIG = 280,
     TCONST = 281,
     TCONTINUE = 282,
     TDEFER = 283,
     TDELETE = 284,
     TDMAPPED = 285,
     TDO = 286,
     TDOMAIN = 287,
     TELSE = 288,
     TENUM = 289,
     TEXCEPT = 290,
     TEXPORT = 291,
     TEXTERN = 292,
     TFALSE = 293,
     TFOR = 294,
     TFORALL = 295,
     TFORWARDING = 296,
     TIF = 297,
     TIMAG = 298,
     TIN = 299,
     TINDEX = 300,
     TINLINE = 301,
     TINOUT = 302,
     TINT = 303,
     TITER = 304,
     TINITEQUALS = 305,
     TLABEL = 306,
     TLAMBDA = 307,
     TLET = 308,
     TLIFETIME = 309,
     TLOCAL = 310,
     TLOCALE = 311,
     TMINUSMINUS = 312,
     TMODULE = 313,
     TNEW = 314,
     TNIL = 315,
     TNOINIT = 316,
     TNONE = 317,
     TNOTHING = 318,
     TON = 319,
     TONLY = 320,
     TOTHERWISE = 321,
     TOUT = 322,
     TOVERRIDE = 323,
     TOWNED = 324,
     TPARAM = 325,
     TPLUSPLUS = 326,
     TPRAGMA = 327,
     TPRIMITIVE = 328,
     TPRIVATE = 329,
     TPROC = 330,
     TPROTOTYPE = 331,
     TPUBLIC = 332,
     TREAL = 333,
     TRECORD = 334,
     TREDUCE = 335,
     TREF = 336,
     TREQUIRE = 337,
     TRETURN = 338,
     TSCAN = 339,
     TSELECT = 340,
     TSERIAL = 341,
     TSHARED = 342,
     TSINGLE = 343,
     TSPARSE = 344,
     TSTRING = 345,
     TSUBDOMAIN = 346,
     TSYNC = 347,
     TTHEN = 348,
     TTHIS = 349,
     TTHROW = 350,
     TTHROWS = 351,
     TTRUE = 352,
     TTRY = 353,
     TTRYBANG = 354,
     TTYPE = 355,
     TUINT = 356,
     TUNDERSCORE = 357,
     TUNION = 358,
     TUNMANAGED = 359,
     TUSE = 360,
     TVAR = 361,
     TVOID = 362,
     TWHEN = 363,
     TWHERE = 364,
     TWHILE = 365,
     TWITH = 366,
     TYIELD = 367,
     TZIP = 368,
     TALIAS = 369,
     TAND = 370,
     TASSIGN = 371,
     TASSIGNBAND = 372,
     TASSIGNBOR = 373,
     TASSIGNBXOR = 374,
     TASSIGNDIVIDE = 375,
     TASSIGNEXP = 376,
     TASSIGNLAND = 377,
     TASSIGNLOR = 378,
     TASSIGNMINUS = 379,
     TASSIGNMOD = 380,
     TASSIGNMULTIPLY = 381,
     TASSIGNPLUS = 382,
     TASSIGNREDUCE = 383,
     TASSIGNSL = 384,
     TASSIGNSR = 385,
     TBANG = 386,
     TBAND = 387,
     TBNOT = 388,
     TBOR = 389,
     TBXOR = 390,
     TCOLON = 391,
     TCOMMA = 392,
     TDIVIDE = 393,
     TDOT = 394,
     TDOTDOT = 395,
     TDOTDOTDOT = 396,
     TEQUAL = 397,
     TEXP = 398,
     TGREATER = 399,
     TGREATEREQUAL = 400,
     THASH = 401,
     TIO = 402,
     TLESS = 403,
     TLESSEQUAL = 404,
     TMINUS = 405,
     TMOD = 406,
     TNOTEQUAL = 407,
     TOR = 408,
     TPLUS = 409,
     TQUESTION = 410,
     TSEMI = 411,
     TSHIFTLEFT = 412,
     TSHIFTRIGHT = 413,
     TSTAR = 414,
     TSWAP = 415,
     TLCBR = 416,
     TRCBR = 417,
     TLP = 418,
     TRP = 419,
     TLSBR = 420,
     TRSBR = 421,
     TNOELSE = 422,
     TUMINUS = 423,
     TUPLUS = 424
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct yypstate yypstate;

#if defined __STDC__ || defined __cplusplus
int yypush_parse (yypstate *ps, int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, ParserContext* context);
#else
int yypush_parse ();
#endif

#if defined __STDC__ || defined __cplusplus
yypstate * yypstate_new (void);
#else
yypstate * yypstate_new ();
#endif
#if defined __STDC__ || defined __cplusplus
void yypstate_delete (yypstate *ps);
#else
void yypstate_delete ();
#endif
/* "%code provides" blocks.  */
/* Line 2058 of yacc.c  */
#line 199 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 2058 of yacc.c  */
#line 430 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
