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
     TCATCH = 273,
     TCLASS = 274,
     TCOBEGIN = 275,
     TCOFORALL = 276,
     TCOMPLEX = 277,
     TCONFIG = 278,
     TCONST = 279,
     TCONTINUE = 280,
     TDEFER = 281,
     TDELETE = 282,
     TDMAPPED = 283,
     TDO = 284,
     TDOMAIN = 285,
     TELSE = 286,
     TENUM = 287,
     TEXCEPT = 288,
     TEXPORT = 289,
     TEXTERN = 290,
     TFALSE = 291,
     TFOR = 292,
     TFORALL = 293,
     TFORWARDING = 294,
     TIF = 295,
     TIMAG = 296,
     TIN = 297,
     TINDEX = 298,
     TINLINE = 299,
     TINOUT = 300,
     TINT = 301,
     TITER = 302,
     TLABEL = 303,
     TLAMBDA = 304,
     TLET = 305,
     TLIFETIME = 306,
     TLOCAL = 307,
     TLOCALE = 308,
     TMINUSMINUS = 309,
     TMODULE = 310,
     TNEW = 311,
     TNIL = 312,
     TNOINIT = 313,
     TON = 314,
     TONLY = 315,
     TOTHERWISE = 316,
     TOUT = 317,
     TOVERRIDE = 318,
     TOWNED = 319,
     TPARAM = 320,
     TPLUSPLUS = 321,
     TPRAGMA = 322,
     TPRIMITIVE = 323,
     TPRIVATE = 324,
     TPROC = 325,
     TPROTOTYPE = 326,
     TPUBLIC = 327,
     TREAL = 328,
     TRECORD = 329,
     TREDUCE = 330,
     TREF = 331,
     TREQUIRE = 332,
     TRETURN = 333,
     TSCAN = 334,
     TSELECT = 335,
     TSERIAL = 336,
     TSHARED = 337,
     TSINGLE = 338,
     TSPARSE = 339,
     TSTRING = 340,
     TSUBDOMAIN = 341,
     TSYNC = 342,
     TTHEN = 343,
     TTHIS = 344,
     TTHROW = 345,
     TTHROWS = 346,
     TTRUE = 347,
     TTRY = 348,
     TTRYBANG = 349,
     TTYPE = 350,
     TUINT = 351,
     TUNDERSCORE = 352,
     TUNION = 353,
     TUNMANAGED = 354,
     TUSE = 355,
     TVAR = 356,
     TWHEN = 357,
     TWHERE = 358,
     TWHILE = 359,
     TWITH = 360,
     TYIELD = 361,
     TZIP = 362,
     TALIAS = 363,
     TAND = 364,
     TASSIGN = 365,
     TASSIGNBAND = 366,
     TASSIGNBOR = 367,
     TASSIGNBXOR = 368,
     TASSIGNDIVIDE = 369,
     TASSIGNEXP = 370,
     TASSIGNLAND = 371,
     TASSIGNLOR = 372,
     TASSIGNMINUS = 373,
     TASSIGNMOD = 374,
     TASSIGNMULTIPLY = 375,
     TASSIGNPLUS = 376,
     TASSIGNREDUCE = 377,
     TASSIGNSL = 378,
     TASSIGNSR = 379,
     TBAND = 380,
     TBNOT = 381,
     TBOR = 382,
     TBXOR = 383,
     TCOLON = 384,
     TCOMMA = 385,
     TDIVIDE = 386,
     TDOT = 387,
     TDOTDOT = 388,
     TDOTDOTDOT = 389,
     TEQUAL = 390,
     TEXP = 391,
     TGREATER = 392,
     TGREATEREQUAL = 393,
     THASH = 394,
     TIO = 395,
     TLESS = 396,
     TLESSEQUAL = 397,
     TMINUS = 398,
     TMOD = 399,
     TNOT = 400,
     TNOTEQUAL = 401,
     TOR = 402,
     TPLUS = 403,
     TQUESTION = 404,
     TSEMI = 405,
     TSHIFTLEFT = 406,
     TSHIFTRIGHT = 407,
     TSTAR = 408,
     TSWAP = 409,
     TLCBR = 410,
     TRCBR = 411,
     TLP = 412,
     TRP = 413,
     TLSBR = 414,
     TRSBR = 415,
     TNOELSE = 416,
     TUMINUS = 417,
     TUPLUS = 418
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
#line 424 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
