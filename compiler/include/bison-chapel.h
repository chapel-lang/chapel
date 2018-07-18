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
     TBORROWED = 270,
     TBY = 271,
     TCATCH = 272,
     TCLASS = 273,
     TCOBEGIN = 274,
     TCOFORALL = 275,
     TCONFIG = 276,
     TCONST = 277,
     TCONTINUE = 278,
     TDEFER = 279,
     TDELETE = 280,
     TDMAPPED = 281,
     TDO = 282,
     TDOMAIN = 283,
     TELSE = 284,
     TENUM = 285,
     TEXCEPT = 286,
     TEXPORT = 287,
     TEXTERN = 288,
     TFOR = 289,
     TFORALL = 290,
     TFORWARDING = 291,
     TIF = 292,
     TIN = 293,
     TINDEX = 294,
     TINLINE = 295,
     TINOUT = 296,
     TITER = 297,
     TLABEL = 298,
     TLAMBDA = 299,
     TLET = 300,
     TLOCAL = 301,
     TMINUSMINUS = 302,
     TMODULE = 303,
     TNEW = 304,
     TNIL = 305,
     TNOINIT = 306,
     TON = 307,
     TONLY = 308,
     TOTHERWISE = 309,
     TOUT = 310,
     TOVERRIDE = 311,
     TOWNED = 312,
     TPARAM = 313,
     TPLUSPLUS = 314,
     TPRAGMA = 315,
     TPRIMITIVE = 316,
     TPRIVATE = 317,
     TPROC = 318,
     TPROTOTYPE = 319,
     TPUBLIC = 320,
     TRECORD = 321,
     TREDUCE = 322,
     TREF = 323,
     TREQUIRE = 324,
     TRETURN = 325,
     TSCAN = 326,
     TSELECT = 327,
     TSERIAL = 328,
     TSHARED = 329,
     TSINGLE = 330,
     TSPARSE = 331,
     TSUBDOMAIN = 332,
     TSYNC = 333,
     TTHEN = 334,
     TTHROW = 335,
     TTHROWS = 336,
     TTRY = 337,
     TTRYBANG = 338,
     TTYPE = 339,
     TUNDERSCORE = 340,
     TUNION = 341,
     TUNMANAGED = 342,
     TUSE = 343,
     TVAR = 344,
     TWHEN = 345,
     TWHERE = 346,
     TWHILE = 347,
     TWITH = 348,
     TYIELD = 349,
     TZIP = 350,
     TALIAS = 351,
     TAND = 352,
     TASSIGN = 353,
     TASSIGNBAND = 354,
     TASSIGNBOR = 355,
     TASSIGNBXOR = 356,
     TASSIGNDIVIDE = 357,
     TASSIGNEXP = 358,
     TASSIGNLAND = 359,
     TASSIGNLOR = 360,
     TASSIGNMINUS = 361,
     TASSIGNMOD = 362,
     TASSIGNMULTIPLY = 363,
     TASSIGNPLUS = 364,
     TASSIGNSL = 365,
     TASSIGNSR = 366,
     TBAND = 367,
     TBNOT = 368,
     TBOR = 369,
     TBXOR = 370,
     TCOLON = 371,
     TCOMMA = 372,
     TDIVIDE = 373,
     TDOT = 374,
     TDOTDOT = 375,
     TDOTDOTDOT = 376,
     TEQUAL = 377,
     TEXP = 378,
     TGREATER = 379,
     TGREATEREQUAL = 380,
     THASH = 381,
     TLESS = 382,
     TLESSEQUAL = 383,
     TMINUS = 384,
     TMOD = 385,
     TNOT = 386,
     TNOTEQUAL = 387,
     TOR = 388,
     TPLUS = 389,
     TQUESTION = 390,
     TSEMI = 391,
     TSHIFTLEFT = 392,
     TSHIFTRIGHT = 393,
     TSTAR = 394,
     TSWAP = 395,
     TASSIGNREDUCE = 396,
     TIO = 397,
     TLCBR = 398,
     TRCBR = 399,
     TLP = 400,
     TRP = 401,
     TLSBR = 402,
     TRSBR = 403,
     TNOELSE = 404,
     TUMINUS = 405,
     TUPLUS = 406
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
#line 397 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
