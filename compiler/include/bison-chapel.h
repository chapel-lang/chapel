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
     CSTRINGLITERAL = 263,
     EXTERNCODE = 264,
     TALIGN = 265,
     TAS = 266,
     TATOMIC = 267,
     TBEGIN = 268,
     TBREAK = 269,
     TBOOL = 270,
     TBORROWED = 271,
     TBY = 272,
     TBYTES = 273,
     TCATCH = 274,
     TCLASS = 275,
     TCOBEGIN = 276,
     TCOFORALL = 277,
     TCOMPLEX = 278,
     TCONFIG = 279,
     TCONST = 280,
     TCONTINUE = 281,
     TDEFER = 282,
     TDELETE = 283,
     TDMAPPED = 284,
     TDO = 285,
     TDOMAIN = 286,
     TELSE = 287,
     TENUM = 288,
     TEXCEPT = 289,
     TEXPORT = 290,
     TEXTERN = 291,
     TFALSE = 292,
     TFOR = 293,
     TFORALL = 294,
     TFORWARDING = 295,
     TIF = 296,
     TIMAG = 297,
     TIN = 298,
     TINDEX = 299,
     TINLINE = 300,
     TINOUT = 301,
     TINT = 302,
     TITER = 303,
     TINITEQUALS = 304,
     TLABEL = 305,
     TLAMBDA = 306,
     TLET = 307,
     TLIFETIME = 308,
     TLOCAL = 309,
     TLOCALE = 310,
     TMINUSMINUS = 311,
     TMODULE = 312,
     TNEW = 313,
     TNIL = 314,
     TNOINIT = 315,
     TNONE = 316,
     TNOTHING = 317,
     TON = 318,
     TONLY = 319,
     TOTHERWISE = 320,
     TOUT = 321,
     TOVERRIDE = 322,
     TOWNED = 323,
     TPARAM = 324,
     TPLUSPLUS = 325,
     TPRAGMA = 326,
     TPRIMITIVE = 327,
     TPRIVATE = 328,
     TPROC = 329,
     TPROTOTYPE = 330,
     TPUBLIC = 331,
     TREAL = 332,
     TRECORD = 333,
     TREDUCE = 334,
     TREF = 335,
     TREQUIRE = 336,
     TRETURN = 337,
     TSCAN = 338,
     TSELECT = 339,
     TSERIAL = 340,
     TSHARED = 341,
     TSINGLE = 342,
     TSPARSE = 343,
     TSTRING = 344,
     TSUBDOMAIN = 345,
     TSYNC = 346,
     TTHEN = 347,
     TTHIS = 348,
     TTHROW = 349,
     TTHROWS = 350,
     TTRUE = 351,
     TTRY = 352,
     TTRYBANG = 353,
     TTYPE = 354,
     TUINT = 355,
     TUNDERSCORE = 356,
     TUNION = 357,
     TUNMANAGED = 358,
     TUSE = 359,
     TVAR = 360,
     TVOID = 361,
     TWHEN = 362,
     TWHERE = 363,
     TWHILE = 364,
     TWITH = 365,
     TYIELD = 366,
     TZIP = 367,
     TALIAS = 368,
     TAND = 369,
     TASSIGN = 370,
     TASSIGNBAND = 371,
     TASSIGNBOR = 372,
     TASSIGNBXOR = 373,
     TASSIGNDIVIDE = 374,
     TASSIGNEXP = 375,
     TASSIGNLAND = 376,
     TASSIGNLOR = 377,
     TASSIGNMINUS = 378,
     TASSIGNMOD = 379,
     TASSIGNMULTIPLY = 380,
     TASSIGNPLUS = 381,
     TASSIGNREDUCE = 382,
     TASSIGNSL = 383,
     TASSIGNSR = 384,
     TBANG = 385,
     TBAND = 386,
     TBNOT = 387,
     TBOR = 388,
     TBXOR = 389,
     TCOLON = 390,
     TCOMMA = 391,
     TDIVIDE = 392,
     TDOT = 393,
     TDOTDOT = 394,
     TDOTDOTDOT = 395,
     TEQUAL = 396,
     TEXP = 397,
     TGREATER = 398,
     TGREATEREQUAL = 399,
     THASH = 400,
     TIO = 401,
     TLESS = 402,
     TLESSEQUAL = 403,
     TMINUS = 404,
     TMOD = 405,
     TNOTEQUAL = 406,
     TOR = 407,
     TPLUS = 408,
     TQUESTION = 409,
     TSEMI = 410,
     TSHIFTLEFT = 411,
     TSHIFTRIGHT = 412,
     TSTAR = 413,
     TSWAP = 414,
     TLCBR = 415,
     TRCBR = 416,
     TLP = 417,
     TRP = 418,
     TLSBR = 419,
     TRSBR = 420,
     TNOELSE = 421,
     TUMINUS = 422,
     TUPLUS = 423
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
#line 429 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
