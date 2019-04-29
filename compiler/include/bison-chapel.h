/* A Bison parser, made by GNU Bison 3.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 32 "chapel.ypp" /* yacc.c:1919  */

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;
#line 45 "chapel.ypp" /* yacc.c:1919  */

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif
#line 65 "chapel.ypp" /* yacc.c:1919  */

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
#line 145 "chapel.ypp" /* yacc.c:1919  */

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
#line 168 "chapel.ypp" /* yacc.c:1919  */

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

#line 185 "../include/bison-chapel.h" /* yacc.c:1919  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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
    TINITEQUALS = 303,
    TLABEL = 304,
    TLAMBDA = 305,
    TLET = 306,
    TLIFETIME = 307,
    TLOCAL = 308,
    TLOCALE = 309,
    TMINUSMINUS = 310,
    TMODULE = 311,
    TNEW = 312,
    TNIL = 313,
    TNOINIT = 314,
    TON = 315,
    TONLY = 316,
    TOTHERWISE = 317,
    TOUT = 318,
    TOVERRIDE = 319,
    TOWNED = 320,
    TPARAM = 321,
    TPLUSPLUS = 322,
    TPRAGMA = 323,
    TPRIMITIVE = 324,
    TPRIVATE = 325,
    TPROC = 326,
    TPROTOTYPE = 327,
    TPUBLIC = 328,
    TREAL = 329,
    TRECORD = 330,
    TREDUCE = 331,
    TREF = 332,
    TREQUIRE = 333,
    TRETURN = 334,
    TSCAN = 335,
    TSELECT = 336,
    TSERIAL = 337,
    TSHARED = 338,
    TSINGLE = 339,
    TSPARSE = 340,
    TSTRING = 341,
    TSUBDOMAIN = 342,
    TSYNC = 343,
    TTHEN = 344,
    TTHIS = 345,
    TTHROW = 346,
    TTHROWS = 347,
    TTRUE = 348,
    TTRY = 349,
    TTRYBANG = 350,
    TTYPE = 351,
    TUINT = 352,
    TUNDERSCORE = 353,
    TUNION = 354,
    TUNMANAGED = 355,
    TUSE = 356,
    TVAR = 357,
    TWHEN = 358,
    TWHERE = 359,
    TWHILE = 360,
    TWITH = 361,
    TYIELD = 362,
    TZIP = 363,
    TALIAS = 364,
    TAND = 365,
    TASSIGN = 366,
    TASSIGNBAND = 367,
    TASSIGNBOR = 368,
    TASSIGNBXOR = 369,
    TASSIGNDIVIDE = 370,
    TASSIGNEXP = 371,
    TASSIGNLAND = 372,
    TASSIGNLOR = 373,
    TASSIGNMINUS = 374,
    TASSIGNMOD = 375,
    TASSIGNMULTIPLY = 376,
    TASSIGNPLUS = 377,
    TASSIGNREDUCE = 378,
    TASSIGNSL = 379,
    TASSIGNSR = 380,
    TBAND = 381,
    TBNOT = 382,
    TBOR = 383,
    TBXOR = 384,
    TCOLON = 385,
    TCOMMA = 386,
    TDIVIDE = 387,
    TDOT = 388,
    TDOTDOT = 389,
    TDOTDOTDOT = 390,
    TEQUAL = 391,
    TEXP = 392,
    TGREATER = 393,
    TGREATEREQUAL = 394,
    THASH = 395,
    TIO = 396,
    TLESS = 397,
    TLESSEQUAL = 398,
    TMINUS = 399,
    TMOD = 400,
    TNOT = 401,
    TNOTEQUAL = 402,
    TOR = 403,
    TPLUS = 404,
    TQUESTION = 405,
    TSEMI = 406,
    TSHIFTLEFT = 407,
    TSHIFTRIGHT = 408,
    TSTAR = 409,
    TSWAP = 410,
    TLCBR = 411,
    TRCBR = 412,
    TLP = 413,
    TRP = 414,
    TLSBR = 415,
    TRSBR = 416,
    TNOELSE = 417,
    TUPLUS = 418,
    TUMINUS = 419
  };
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

int yypush_parse (yypstate *ps, int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, ParserContext* context);

yypstate * yypstate_new (void);
void yypstate_delete (yypstate *ps);
/* "%code provides" blocks.  */
#line 199 "chapel.ypp" /* yacc.c:1919  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 395 "../include/bison-chapel.h" /* yacc.c:1919  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
