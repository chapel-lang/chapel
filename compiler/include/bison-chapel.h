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
    Flag                      flag;
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif

/* Line 2058 of yacc.c  */
#line 131 "chapel.ypp"

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

/* Line 2058 of yacc.c  */
#line 153 "chapel.ypp"

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
#line 181 "../include/bison-chapel.h"

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
     TFOR = 291,
     TFORALL = 292,
     TFORWARDING = 293,
     TIF = 294,
     TIMAG = 295,
     TIN = 296,
     TINDEX = 297,
     TINLINE = 298,
     TINOUT = 299,
     TINT = 300,
     TITER = 301,
     TLABEL = 302,
     TLAMBDA = 303,
     TLET = 304,
     TLOCAL = 305,
     TMINUSMINUS = 306,
     TMODULE = 307,
     TNEW = 308,
     TNIL = 309,
     TNOINIT = 310,
     TON = 311,
     TONLY = 312,
     TOTHERWISE = 313,
     TOUT = 314,
     TOVERRIDE = 315,
     TOWNED = 316,
     TPARAM = 317,
     TPLUSPLUS = 318,
     TPRAGMA = 319,
     TPRIMITIVE = 320,
     TPRIVATE = 321,
     TPROC = 322,
     TPROTOTYPE = 323,
     TPUBLIC = 324,
     TREAL = 325,
     TRECORD = 326,
     TREDUCE = 327,
     TREF = 328,
     TREQUIRE = 329,
     TRETURN = 330,
     TSCAN = 331,
     TSELECT = 332,
     TSERIAL = 333,
     TSHARED = 334,
     TSINGLE = 335,
     TSPARSE = 336,
     TSTRING = 337,
     TSUBDOMAIN = 338,
     TSYNC = 339,
     TTHEN = 340,
     TTHROW = 341,
     TTHROWS = 342,
     TTRY = 343,
     TTRYBANG = 344,
     TTYPE = 345,
     TUINT = 346,
     TUNDERSCORE = 347,
     TUNION = 348,
     TUNMANAGED = 349,
     TUSE = 350,
     TVAR = 351,
     TWHEN = 352,
     TWHERE = 353,
     TWHILE = 354,
     TWITH = 355,
     TYIELD = 356,
     TZIP = 357,
     TALIAS = 358,
     TAND = 359,
     TASSIGN = 360,
     TASSIGNBAND = 361,
     TASSIGNBOR = 362,
     TASSIGNBXOR = 363,
     TASSIGNDIVIDE = 364,
     TASSIGNEXP = 365,
     TASSIGNLAND = 366,
     TASSIGNLOR = 367,
     TASSIGNMINUS = 368,
     TASSIGNMOD = 369,
     TASSIGNMULTIPLY = 370,
     TASSIGNPLUS = 371,
     TASSIGNREDUCE = 372,
     TASSIGNSL = 373,
     TASSIGNSR = 374,
     TBAND = 375,
     TBNOT = 376,
     TBOR = 377,
     TBXOR = 378,
     TCOLON = 379,
     TCOMMA = 380,
     TDIVIDE = 381,
     TDOT = 382,
     TDOTDOT = 383,
     TDOTDOTDOT = 384,
     TEQUAL = 385,
     TEXP = 386,
     TGREATER = 387,
     TGREATEREQUAL = 388,
     THASH = 389,
     TIO = 390,
     TLESS = 391,
     TLESSEQUAL = 392,
     TMINUS = 393,
     TMOD = 394,
     TNOT = 395,
     TNOTEQUAL = 396,
     TOR = 397,
     TPLUS = 398,
     TQUESTION = 399,
     TSEMI = 400,
     TSHIFTLEFT = 401,
     TSHIFTRIGHT = 402,
     TSTAR = 403,
     TSWAP = 404,
     TLCBR = 405,
     TRCBR = 406,
     TLP = 407,
     TRP = 408,
     TLSBR = 409,
     TRSBR = 410,
     TNOELSE = 411,
     TUMINUS = 412,
     TUPLUS = 413
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
#line 184 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 2058 of yacc.c  */
#line 404 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
