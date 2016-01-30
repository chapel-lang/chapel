/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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

/* "%code requires" blocks.  */

/* Line 2068 of yacc.c  */
#line 33 "chapel.ypp"

  extern int  captureTokens;
  extern char captureString[1024];


/* Line 2068 of yacc.c  */
#line 44 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif


/* Line 2068 of yacc.c  */
#line 64 "chapel.ypp"

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


/* Line 2068 of yacc.c  */
#line 120 "chapel.ypp"

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


/* Line 2068 of yacc.c  */
#line 142 "chapel.ypp"

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



/* Line 2068 of yacc.c  */
#line 167 "../include/bison-chapel.h"

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
     TEXCEPT = 282,
     TEXPORT = 283,
     TEXTERN = 284,
     TFOR = 285,
     TFORALL = 286,
     TIF = 287,
     TIN = 288,
     TINDEX = 289,
     TINLINE = 290,
     TINOUT = 291,
     TITER = 292,
     TLABEL = 293,
     TLAMBDA = 294,
     TLET = 295,
     TLOCAL = 296,
     TMINUSMINUS = 297,
     TMODULE = 298,
     TNEW = 299,
     TNIL = 300,
     TNOINIT = 301,
     TON = 302,
     TONLY = 303,
     TOTHERWISE = 304,
     TOUT = 305,
     TPARAM = 306,
     TPLUSPLUS = 307,
     TPRAGMA = 308,
     TPRIMITIVE = 309,
     TPRIVATE = 310,
     TPROC = 311,
     TPUBLIC = 312,
     TRECORD = 313,
     TREDUCE = 314,
     TREF = 315,
     TREQUIRE = 316,
     TRETURN = 317,
     TSCAN = 318,
     TSELECT = 319,
     TSERIAL = 320,
     TSINGLE = 321,
     TSPARSE = 322,
     TSUBDOMAIN = 323,
     TSYNC = 324,
     TTHEN = 325,
     TTYPE = 326,
     TUNDERSCORE = 327,
     TUNION = 328,
     TUSE = 329,
     TVAR = 330,
     TWHEN = 331,
     TWHERE = 332,
     TWHILE = 333,
     TWITH = 334,
     TYIELD = 335,
     TZIP = 336,
     TALIAS = 337,
     TAND = 338,
     TASSIGN = 339,
     TASSIGNBAND = 340,
     TASSIGNBOR = 341,
     TASSIGNBXOR = 342,
     TASSIGNDIVIDE = 343,
     TASSIGNEXP = 344,
     TASSIGNLAND = 345,
     TASSIGNLOR = 346,
     TASSIGNMINUS = 347,
     TASSIGNMOD = 348,
     TASSIGNMULTIPLY = 349,
     TASSIGNPLUS = 350,
     TASSIGNSL = 351,
     TASSIGNSR = 352,
     TBAND = 353,
     TBNOT = 354,
     TBOR = 355,
     TBXOR = 356,
     TCOLON = 357,
     TCOMMA = 358,
     TDIVIDE = 359,
     TDOT = 360,
     TDOTDOT = 361,
     TDOTDOTDOT = 362,
     TEQUAL = 363,
     TEXP = 364,
     TGREATER = 365,
     TGREATEREQUAL = 366,
     THASH = 367,
     TLESS = 368,
     TLESSEQUAL = 369,
     TMINUS = 370,
     TMOD = 371,
     TNOT = 372,
     TNOTEQUAL = 373,
     TOR = 374,
     TPLUS = 375,
     TQUESTION = 376,
     TSEMI = 377,
     TSHIFTLEFT = 378,
     TSHIFTRIGHT = 379,
     TSTAR = 380,
     TSWAP = 381,
     TIO = 382,
     TLCBR = 383,
     TRCBR = 384,
     TLP = 385,
     TRP = 386,
     TLSBR = 387,
     TRSBR = 388,
     TNOELSE = 389,
     TUMINUS = 390,
     TUPLUS = 391
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



#ifndef YYPUSH_DECLS
#  define YYPUSH_DECLS
struct yypstate;
typedef struct yypstate yypstate;
enum { YYPUSH_MORE = 4 };
#if defined __STDC__ || defined __cplusplus
int yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE const *yypushed_loc, ParserContext* context);
#else
int yypush_parse ();
#endif

#if defined __STDC__ || defined __cplusplus
yypstate * yypstate_new (void);
#else
yypstate * yypstate_new ();
#endif
#if defined __STDC__ || defined __cplusplus
void yypstate_delete (yypstate *yyps);
#else
void yypstate_delete ();
#endif
#endif

/* "%code provides" blocks.  */

/* Line 2068 of yacc.c  */
#line 173 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 2068 of yacc.c  */
#line 374 "../include/bison-chapel.h"
