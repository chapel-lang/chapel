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
    EXTERNCODE = 263,
    TALIGN = 264,
    TATOMIC = 265,
    TBEGIN = 266,
    TBREAK = 267,
    TBY = 268,
    TCLASS = 269,
    TCOBEGIN = 270,
    TCOFORALL = 271,
    TCONFIG = 272,
    TCONST = 273,
    TCONTINUE = 274,
    TDELETE = 275,
    TDMAPPED = 276,
    TDO = 277,
    TDOMAIN = 278,
    TELSE = 279,
    TENUM = 280,
    TEXPORT = 281,
    TEXTERN = 282,
    TFOR = 283,
    TFORALL = 284,
    TIF = 285,
    TIN = 286,
    TINDEX = 287,
    TINLINE = 288,
    TINOUT = 289,
    TITER = 290,
    TLABEL = 291,
    TLAMBDA = 292,
    TLET = 293,
    TLOCAL = 294,
    TMINUSMINUS = 295,
    TMODULE = 296,
    TNEW = 297,
    TNIL = 298,
    TNOINIT = 299,
    TON = 300,
    TOTHERWISE = 301,
    TOUT = 302,
    TPARAM = 303,
    TPLUSPLUS = 304,
    TPRAGMA = 305,
    TPRIMITIVE = 306,
    TPROC = 307,
    TRECORD = 308,
    TREDUCE = 309,
    TREF = 310,
    TRETURN = 311,
    TSCAN = 312,
    TSELECT = 313,
    TSERIAL = 314,
    TSINGLE = 315,
    TSPARSE = 316,
    TSUBDOMAIN = 317,
    TSYNC = 318,
    TTHEN = 319,
    TTYPE = 320,
    TUNDERSCORE = 321,
    TUNION = 322,
    TUSE = 323,
    TVAR = 324,
    TWHEN = 325,
    TWHERE = 326,
    TWHILE = 327,
    TWITH = 328,
    TYIELD = 329,
    TZIP = 330,
    TALIAS = 331,
    TAND = 332,
    TASSIGN = 333,
    TASSIGNBAND = 334,
    TASSIGNBOR = 335,
    TASSIGNBXOR = 336,
    TASSIGNDIVIDE = 337,
    TASSIGNEXP = 338,
    TASSIGNLAND = 339,
    TASSIGNLOR = 340,
    TASSIGNMINUS = 341,
    TASSIGNMOD = 342,
    TASSIGNMULTIPLY = 343,
    TASSIGNPLUS = 344,
    TASSIGNSL = 345,
    TASSIGNSR = 346,
    TBAND = 347,
    TBNOT = 348,
    TBOR = 349,
    TBXOR = 350,
    TCOLON = 351,
    TCOMMA = 352,
    TDIVIDE = 353,
    TDOT = 354,
    TDOTDOT = 355,
    TDOTDOTDOT = 356,
    TEQUAL = 357,
    TEXP = 358,
    TGREATER = 359,
    TGREATEREQUAL = 360,
    THASH = 361,
    TLESS = 362,
    TLESSEQUAL = 363,
    TMINUS = 364,
    TMOD = 365,
    TNOT = 366,
    TNOTEQUAL = 367,
    TOR = 368,
    TPLUS = 369,
    TQUESTION = 370,
    TSEMI = 371,
    TSHIFTLEFT = 372,
    TSHIFTRIGHT = 373,
    TSTAR = 374,
    TSWAP = 375,
    TIO = 376,
    TLCBR = 377,
    TRCBR = 378,
    TLP = 379,
    TRP = 380,
    TLSBR = 381,
    TRSBR = 382,
    TNOELSE = 383,
    TUPLUS = 384,
    TUMINUS = 385
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

#line 334 "../include/bison-chapel.h" /* yacc.c:1915  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
