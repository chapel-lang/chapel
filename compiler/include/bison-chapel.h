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
    TPROC = 308,
    TRECORD = 309,
    TREDUCE = 310,
    TREF = 311,
    TRETURN = 312,
    TSCAN = 313,
    TSELECT = 314,
    TSERIAL = 315,
    TSINGLE = 316,
    TSPARSE = 317,
    TSUBDOMAIN = 318,
    TSYNC = 319,
    TTHEN = 320,
    TTYPE = 321,
    TUNDERSCORE = 322,
    TUNION = 323,
    TUSE = 324,
    TVAR = 325,
    TWHEN = 326,
    TWHERE = 327,
    TWHILE = 328,
    TWITH = 329,
    TYIELD = 330,
    TZIP = 331,
    TALIAS = 332,
    TAND = 333,
    TASSIGN = 334,
    TASSIGNBAND = 335,
    TASSIGNBOR = 336,
    TASSIGNBXOR = 337,
    TASSIGNDIVIDE = 338,
    TASSIGNEXP = 339,
    TASSIGNLAND = 340,
    TASSIGNLOR = 341,
    TASSIGNMINUS = 342,
    TASSIGNMOD = 343,
    TASSIGNMULTIPLY = 344,
    TASSIGNPLUS = 345,
    TASSIGNSL = 346,
    TASSIGNSR = 347,
    TBAND = 348,
    TBNOT = 349,
    TBOR = 350,
    TBXOR = 351,
    TCOLON = 352,
    TCOMMA = 353,
    TDIVIDE = 354,
    TDOT = 355,
    TDOTDOT = 356,
    TDOTDOTDOT = 357,
    TEQUAL = 358,
    TEXP = 359,
    TGREATER = 360,
    TGREATEREQUAL = 361,
    THASH = 362,
    TLESS = 363,
    TLESSEQUAL = 364,
    TMINUS = 365,
    TMOD = 366,
    TNOT = 367,
    TNOTEQUAL = 368,
    TOR = 369,
    TPLUS = 370,
    TQUESTION = 371,
    TSEMI = 372,
    TSHIFTLEFT = 373,
    TSHIFTRIGHT = 374,
    TSTAR = 375,
    TSWAP = 376,
    TIO = 377,
    TLCBR = 378,
    TRCBR = 379,
    TLP = 380,
    TRP = 381,
    TLSBR = 382,
    TRSBR = 383,
    TNOELSE = 384,
    TUPLUS = 385,
    TUMINUS = 386
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

#line 335 "../include/bison-chapel.h" /* yacc.c:1915  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
