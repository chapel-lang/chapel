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

  #endif
#line 63 "chapel.ypp" /* yacc.c:1915  */

  #ifndef _BISON_CHAPEL_DEFINES_1_
  #define _BISON_CHAPEL_DEFINES_1_

  #include "symbol.h"

  #include <cstdio>

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
    Expr*  first;
    Expr*  second;
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
  };

  #endif
#line 119 "chapel.ypp" /* yacc.c:1915  */

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
#line 141 "chapel.ypp" /* yacc.c:1915  */

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

#line 158 "../include/bison-chapel.h" /* yacc.c:1915  */

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
    TATOMIC = 266,
    TBEGIN = 267,
    TBREAK = 268,
    TBY = 269,
    TCLASS = 270,
    TCOBEGIN = 271,
    TCOFORALL = 272,
    TCONFIG = 273,
    TCONST = 274,
    TCONTINUE = 275,
    TDELETE = 276,
    TDMAPPED = 277,
    TDO = 278,
    TDOMAIN = 279,
    TELSE = 280,
    TENUM = 281,
    TEXPORT = 282,
    TEXTERN = 283,
    TFOR = 284,
    TFORALL = 285,
    TIF = 286,
    TIN = 287,
    TINDEX = 288,
    TINLINE = 289,
    TINOUT = 290,
    TITER = 291,
    TLABEL = 292,
    TLAMBDA = 293,
    TLET = 294,
    TLOCAL = 295,
    TMINUSMINUS = 296,
    TMODULE = 297,
    TNEW = 298,
    TNIL = 299,
    TNOINIT = 300,
    TON = 301,
    TOTHERWISE = 302,
    TOUT = 303,
    TPARAM = 304,
    TPLUSPLUS = 305,
    TPRAGMA = 306,
    TPRIMITIVE = 307,
    TPRIVATE = 308,
    TPROC = 309,
    TPUBLIC = 310,
    TRECORD = 311,
    TREDUCE = 312,
    TREF = 313,
    TREQUIRE = 314,
    TRETURN = 315,
    TSCAN = 316,
    TSELECT = 317,
    TSERIAL = 318,
    TSINGLE = 319,
    TSPARSE = 320,
    TSUBDOMAIN = 321,
    TSYNC = 322,
    TTHEN = 323,
    TTYPE = 324,
    TUNDERSCORE = 325,
    TUNION = 326,
    TUSE = 327,
    TVAR = 328,
    TWHEN = 329,
    TWHERE = 330,
    TWHILE = 331,
    TWITH = 332,
    TYIELD = 333,
    TZIP = 334,
    TALIAS = 335,
    TAND = 336,
    TASSIGN = 337,
    TASSIGNBAND = 338,
    TASSIGNBOR = 339,
    TASSIGNBXOR = 340,
    TASSIGNDIVIDE = 341,
    TASSIGNEXP = 342,
    TASSIGNLAND = 343,
    TASSIGNLOR = 344,
    TASSIGNMINUS = 345,
    TASSIGNMOD = 346,
    TASSIGNMULTIPLY = 347,
    TASSIGNPLUS = 348,
    TASSIGNSL = 349,
    TASSIGNSR = 350,
    TBAND = 351,
    TBNOT = 352,
    TBOR = 353,
    TBXOR = 354,
    TCOLON = 355,
    TCOMMA = 356,
    TDIVIDE = 357,
    TDOT = 358,
    TDOTDOT = 359,
    TDOTDOTDOT = 360,
    TEQUAL = 361,
    TEXP = 362,
    TGREATER = 363,
    TGREATEREQUAL = 364,
    THASH = 365,
    TLESS = 366,
    TLESSEQUAL = 367,
    TMINUS = 368,
    TMOD = 369,
    TNOT = 370,
    TNOTEQUAL = 371,
    TOR = 372,
    TPLUS = 373,
    TQUESTION = 374,
    TSEMI = 375,
    TSHIFTLEFT = 376,
    TSHIFTRIGHT = 377,
    TSTAR = 378,
    TSWAP = 379,
    TIO = 380,
    TLCBR = 381,
    TRCBR = 382,
    TLP = 383,
    TRP = 384,
    TLSBR = 385,
    TRSBR = 386,
    TNOELSE = 387,
    TUPLUS = 388,
    TUMINUS = 389
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
#line 172 "chapel.ypp" /* yacc.c:1915  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 338 "../include/bison-chapel.h" /* yacc.c:1915  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
