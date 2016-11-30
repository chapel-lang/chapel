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
#line 33 "chapel.ypp" /* yacc.c:1915  */

  extern int  captureTokens;
  extern char captureString[1024];
#line 44 "chapel.ypp" /* yacc.c:1915  */

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif
#line 64 "chapel.ypp" /* yacc.c:1915  */

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

  struct IntentExpr {
    Expr*     iVar;
    IntentTag tfIntent; // undefined for a reduce intent
    Expr*     riExp;    // non-NULL for a reduce intent
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
    const char*       pch;

    Vec<const char*>* vpch;
    RetTag            retTag;
    bool              b;
    IntentTag         pt;
    Expr*             pexpr;
    DefExpr*          pdefexpr;
    CallExpr*         pcallexpr;
    BlockStmt*        pblockstmt;
    Type*             ptype;
    EnumType*         penumtype;
    FnSymbol*         pfnsymbol;
    Flag              flag;
    ProcIter          procIter;
    FlagSet*          flagSet;
    IntentExpr        pIntentExpr;
    ForallIntents*    pForallIntents;
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif
#line 134 "chapel.ypp" /* yacc.c:1915  */

  #ifndef _BISON_CHAPEL_DEFINES_2_
  #define _BISON_CHAPEL_DEFINES_2_

  struct YYLTYPE {
    int         first_line;
    int         first_column;
    int         last_line;
    int         last_column;
    const char* comment;
  };

  #define YYLTYPE_IS_DECLARED 1
  #define YYLTYPE_IS_TRIVIAL  1

  #endif
#line 156 "chapel.ypp" /* yacc.c:1915  */

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

#line 173 "../include/bison-chapel.h" /* yacc.c:1915  */

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
    TBY = 270,
    TCLASS = 271,
    TCOBEGIN = 272,
    TCOFORALL = 273,
    TCONFIG = 274,
    TCONST = 275,
    TCONTINUE = 276,
    TDELETE = 277,
    TDMAPPED = 278,
    TDO = 279,
    TDOMAIN = 280,
    TELSE = 281,
    TENUM = 282,
    TEXCEPT = 283,
    TEXPORT = 284,
    TEXTERN = 285,
    TFOR = 286,
    TFORALL = 287,
    TIF = 288,
    TIN = 289,
    TINDEX = 290,
    TINLINE = 291,
    TINOUT = 292,
    TITER = 293,
    TLABEL = 294,
    TLAMBDA = 295,
    TLET = 296,
    TLOCAL = 297,
    TMINUSMINUS = 298,
    TMODULE = 299,
    TNEW = 300,
    TNIL = 301,
    TNOINIT = 302,
    TON = 303,
    TONLY = 304,
    TOTHERWISE = 305,
    TOUT = 306,
    TPARAM = 307,
    TPLUSPLUS = 308,
    TPRAGMA = 309,
    TPRIMITIVE = 310,
    TPRIVATE = 311,
    TPROC = 312,
    TPUBLIC = 313,
    TRECORD = 314,
    TREDUCE = 315,
    TREF = 316,
    TREQUIRE = 317,
    TRETURN = 318,
    TSCAN = 319,
    TSELECT = 320,
    TSERIAL = 321,
    TSINGLE = 322,
    TSPARSE = 323,
    TSUBDOMAIN = 324,
    TSYNC = 325,
    TTHEN = 326,
    TTRY = 327,
    TTYPE = 328,
    TUNDERSCORE = 329,
    TUNION = 330,
    TUSE = 331,
    TVAR = 332,
    TWHEN = 333,
    TWHERE = 334,
    TWHILE = 335,
    TWITH = 336,
    TYIELD = 337,
    TZIP = 338,
    TALIAS = 339,
    TAND = 340,
    TASSIGN = 341,
    TASSIGNBAND = 342,
    TASSIGNBOR = 343,
    TASSIGNBXOR = 344,
    TASSIGNDIVIDE = 345,
    TASSIGNEXP = 346,
    TASSIGNLAND = 347,
    TASSIGNLOR = 348,
    TASSIGNMINUS = 349,
    TASSIGNMOD = 350,
    TASSIGNMULTIPLY = 351,
    TASSIGNPLUS = 352,
    TASSIGNSL = 353,
    TASSIGNSR = 354,
    TBAND = 355,
    TBNOT = 356,
    TBOR = 357,
    TBXOR = 358,
    TCOLON = 359,
    TCOMMA = 360,
    TDIVIDE = 361,
    TDOT = 362,
    TDOTDOT = 363,
    TDOTDOTDOT = 364,
    TEQUAL = 365,
    TEXP = 366,
    TGREATER = 367,
    TGREATEREQUAL = 368,
    THASH = 369,
    TLESS = 370,
    TLESSEQUAL = 371,
    TMINUS = 372,
    TMOD = 373,
    TNOT = 374,
    TNOTEQUAL = 375,
    TOR = 376,
    TPLUS = 377,
    TQUESTION = 378,
    TSEMI = 379,
    TSHIFTLEFT = 380,
    TSHIFTRIGHT = 381,
    TSTAR = 382,
    TSWAP = 383,
    TASSIGNREDUCE = 384,
    TIO = 385,
    TLCBR = 386,
    TRCBR = 387,
    TLP = 388,
    TRP = 389,
    TLSBR = 390,
    TRSBR = 391,
    TNOELSE = 392,
    TUPLUS = 393,
    TUMINUS = 394
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
#line 187 "chapel.ypp" /* yacc.c:1915  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 358 "../include/bison-chapel.h" /* yacc.c:1915  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
