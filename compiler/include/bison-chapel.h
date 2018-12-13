/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#line 32 "chapel.ypp" /* yacc.c:1915  */

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;
#line 45 "chapel.ypp" /* yacc.c:1915  */

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif
#line 65 "chapel.ypp" /* yacc.c:1915  */

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
  };

  #endif
#line 132 "chapel.ypp" /* yacc.c:1915  */

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
#line 155 "chapel.ypp" /* yacc.c:1915  */

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

#line 172 "../include/bison-chapel.h" /* yacc.c:1915  */

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
    TLABEL = 303,
    TLAMBDA = 304,
    TLET = 305,
    TLOCAL = 306,
    TLOCALE = 307,
    TMINUSMINUS = 308,
    TMODULE = 309,
    TNEW = 310,
    TNIL = 311,
    TNOINIT = 312,
    TON = 313,
    TONLY = 314,
    TOTHERWISE = 315,
    TOUT = 316,
    TOVERRIDE = 317,
    TOWNED = 318,
    TPARAM = 319,
    TPLUSPLUS = 320,
    TPRAGMA = 321,
    TPRIMITIVE = 322,
    TPRIVATE = 323,
    TPROC = 324,
    TPROTOTYPE = 325,
    TPUBLIC = 326,
    TREAL = 327,
    TRECORD = 328,
    TREDUCE = 329,
    TREF = 330,
    TREQUIRE = 331,
    TRETURN = 332,
    TSCAN = 333,
    TSELECT = 334,
    TSERIAL = 335,
    TSHARED = 336,
    TSINGLE = 337,
    TSPARSE = 338,
    TSTRING = 339,
    TSUBDOMAIN = 340,
    TSYNC = 341,
    TTHEN = 342,
    TTHIS = 343,
    TTHROW = 344,
    TTHROWS = 345,
    TTRUE = 346,
    TTRY = 347,
    TTRYBANG = 348,
    TTYPE = 349,
    TUINT = 350,
    TUNDERSCORE = 351,
    TUNION = 352,
    TUNMANAGED = 353,
    TUSE = 354,
    TVAR = 355,
    TWHEN = 356,
    TWHERE = 357,
    TWHILE = 358,
    TWITH = 359,
    TYIELD = 360,
    TZIP = 361,
    TALIAS = 362,
    TAND = 363,
    TASSIGN = 364,
    TASSIGNBAND = 365,
    TASSIGNBOR = 366,
    TASSIGNBXOR = 367,
    TASSIGNDIVIDE = 368,
    TASSIGNEXP = 369,
    TASSIGNLAND = 370,
    TASSIGNLOR = 371,
    TASSIGNMINUS = 372,
    TASSIGNMOD = 373,
    TASSIGNMULTIPLY = 374,
    TASSIGNPLUS = 375,
    TASSIGNREDUCE = 376,
    TASSIGNSL = 377,
    TASSIGNSR = 378,
    TBAND = 379,
    TBNOT = 380,
    TBOR = 381,
    TBXOR = 382,
    TCOLON = 383,
    TCOMMA = 384,
    TDIVIDE = 385,
    TDOT = 386,
    TDOTDOT = 387,
    TDOTDOTDOT = 388,
    TEQUAL = 389,
    TEXP = 390,
    TGREATER = 391,
    TGREATEREQUAL = 392,
    THASH = 393,
    TIO = 394,
    TLESS = 395,
    TLESSEQUAL = 396,
    TMINUS = 397,
    TMOD = 398,
    TNOT = 399,
    TNOTEQUAL = 400,
    TOR = 401,
    TPLUS = 402,
    TQUESTION = 403,
    TSEMI = 404,
    TSHIFTLEFT = 405,
    TSHIFTRIGHT = 406,
    TSTAR = 407,
    TSWAP = 408,
    TLCBR = 409,
    TRCBR = 410,
    TLP = 411,
    TRP = 412,
    TLSBR = 413,
    TRSBR = 414,
    TNOELSE = 415,
    TUPLUS = 416,
    TUMINUS = 417
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
#line 186 "chapel.ypp" /* yacc.c:1915  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 380 "../include/bison-chapel.h" /* yacc.c:1915  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
