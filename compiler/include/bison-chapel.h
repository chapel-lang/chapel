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
  class NamedExpr;
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
#line 146 "chapel.ypp"

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
#line 169 "chapel.ypp"

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
#line 197 "../include/bison-chapel.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TIDENT = 258,
     TQUERIEDIDENT = 259,
     INTLITERAL = 260,
     REALLITERAL = 261,
     IMAGLITERAL = 262,
     STRINGLITERAL = 263,
     BYTESLITERAL = 264,
     CSTRINGLITERAL = 265,
     EXTERNCODE = 266,
     TALIGN = 267,
     TAS = 268,
     TATOMIC = 269,
     TBEGIN = 270,
     TBREAK = 271,
     TBOOL = 272,
     TBORROWED = 273,
     TBY = 274,
     TBYTES = 275,
     TCATCH = 276,
     TCLASS = 277,
     TCOBEGIN = 278,
     TCOFORALL = 279,
     TCOMPLEX = 280,
     TCONFIG = 281,
     TCONST = 282,
     TCONTINUE = 283,
     TDEFER = 284,
     TDELETE = 285,
     TDMAPPED = 286,
     TDO = 287,
     TDOMAIN = 288,
     TELSE = 289,
     TENUM = 290,
     TEXCEPT = 291,
     TEXPORT = 292,
     TEXTERN = 293,
     TFALSE = 294,
     TFOR = 295,
     TFORALL = 296,
     TFORWARDING = 297,
     TIF = 298,
     TIMAG = 299,
     TIN = 300,
     TINDEX = 301,
     TINLINE = 302,
     TINOUT = 303,
     TINT = 304,
     TITER = 305,
     TINITEQUALS = 306,
     TLABEL = 307,
     TLAMBDA = 308,
     TLET = 309,
     TLIFETIME = 310,
     TLOCAL = 311,
     TLOCALE = 312,
     TMINUSMINUS = 313,
     TMODULE = 314,
     TNEW = 315,
     TNIL = 316,
     TNOINIT = 317,
     TNONE = 318,
     TNOTHING = 319,
     TON = 320,
     TONLY = 321,
     TOTHERWISE = 322,
     TOUT = 323,
     TOVERRIDE = 324,
     TOWNED = 325,
     TPARAM = 326,
     TPLUSPLUS = 327,
     TPRAGMA = 328,
     TPRIMITIVE = 329,
     TPRIVATE = 330,
     TPROC = 331,
     TPROTOTYPE = 332,
     TPUBLIC = 333,
     TREAL = 334,
     TRECORD = 335,
     TREDUCE = 336,
     TREF = 337,
     TREQUIRE = 338,
     TRETURN = 339,
     TSCAN = 340,
     TSELECT = 341,
     TSERIAL = 342,
     TSHARED = 343,
     TSINGLE = 344,
     TSPARSE = 345,
     TSTRING = 346,
     TSUBDOMAIN = 347,
     TSYNC = 348,
     TTHEN = 349,
     TTHIS = 350,
     TTHROW = 351,
     TTHROWS = 352,
     TTRUE = 353,
     TTRY = 354,
     TTRYBANG = 355,
     TTYPE = 356,
     TUINT = 357,
     TUNDERSCORE = 358,
     TUNION = 359,
     TUNMANAGED = 360,
     TUSE = 361,
     TVAR = 362,
     TVOID = 363,
     TWHEN = 364,
     TWHERE = 365,
     TWHILE = 366,
     TWITH = 367,
     TYIELD = 368,
     TZIP = 369,
     TALIAS = 370,
     TAND = 371,
     TASSIGN = 372,
     TASSIGNBAND = 373,
     TASSIGNBOR = 374,
     TASSIGNBXOR = 375,
     TASSIGNDIVIDE = 376,
     TASSIGNEXP = 377,
     TASSIGNLAND = 378,
     TASSIGNLOR = 379,
     TASSIGNMINUS = 380,
     TASSIGNMOD = 381,
     TASSIGNMULTIPLY = 382,
     TASSIGNPLUS = 383,
     TASSIGNREDUCE = 384,
     TASSIGNSL = 385,
     TASSIGNSR = 386,
     TBANG = 387,
     TBAND = 388,
     TBNOT = 389,
     TBOR = 390,
     TBXOR = 391,
     TCOLON = 392,
     TCOMMA = 393,
     TDIVIDE = 394,
     TDOT = 395,
     TDOTDOT = 396,
     TDOTDOTDOT = 397,
     TEQUAL = 398,
     TEXP = 399,
     TGREATER = 400,
     TGREATEREQUAL = 401,
     THASH = 402,
     TIO = 403,
     TLESS = 404,
     TLESSEQUAL = 405,
     TMINUS = 406,
     TMOD = 407,
     TNOTEQUAL = 408,
     TOR = 409,
     TPLUS = 410,
     TQUESTION = 411,
     TSEMI = 412,
     TSHIFTLEFT = 413,
     TSHIFTRIGHT = 414,
     TSTAR = 415,
     TSWAP = 416,
     TLCBR = 417,
     TRCBR = 418,
     TLP = 419,
     TRP = 420,
     TLSBR = 421,
     TRSBR = 422,
     TNOELSE = 423,
     TUMINUS = 424,
     TUPLUS = 425,
     TLNOT = 426
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
#line 200 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 2058 of yacc.c  */
#line 433 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
