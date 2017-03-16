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
#line 32 "chapel.ypp"

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;


/* Line 2068 of yacc.c  */
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


/* Line 2068 of yacc.c  */
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


/* Line 2068 of yacc.c  */
#line 135 "chapel.ypp"

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
#line 157 "chapel.ypp"

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
#line 182 "../include/bison-chapel.h"

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
     TFORWARDING = 288,
     TIF = 289,
     TIN = 290,
     TINDEX = 291,
     TINLINE = 292,
     TINOUT = 293,
     TITER = 294,
     TLABEL = 295,
     TLAMBDA = 296,
     TLET = 297,
     TLOCAL = 298,
     TMINUSMINUS = 299,
     TMODULE = 300,
     TNEW = 301,
     TNIL = 302,
     TNOINIT = 303,
     TON = 304,
     TONLY = 305,
     TOTHERWISE = 306,
     TOUT = 307,
     TPARAM = 308,
     TPLUSPLUS = 309,
     TPRAGMA = 310,
     TPRIMITIVE = 311,
     TPRIVATE = 312,
     TPROC = 313,
     TPUBLIC = 314,
     TRECORD = 315,
     TREDUCE = 316,
     TREF = 317,
     TREQUIRE = 318,
     TRETURN = 319,
     TSCAN = 320,
     TSELECT = 321,
     TSERIAL = 322,
     TSINGLE = 323,
     TSPARSE = 324,
     TSUBDOMAIN = 325,
     TSYNC = 326,
     TTHEN = 327,
     TTHROW = 328,
     TTHROWS = 329,
     TTRY = 330,
     TTRYBANG = 331,
     TTYPE = 332,
     TUNDERSCORE = 333,
     TUNION = 334,
     TUSE = 335,
     TVAR = 336,
     TWHEN = 337,
     TWHERE = 338,
     TWHILE = 339,
     TWITH = 340,
     TYIELD = 341,
     TZIP = 342,
     TALIAS = 343,
     TAND = 344,
     TASSIGN = 345,
     TASSIGNBAND = 346,
     TASSIGNBOR = 347,
     TASSIGNBXOR = 348,
     TASSIGNDIVIDE = 349,
     TASSIGNEXP = 350,
     TASSIGNLAND = 351,
     TASSIGNLOR = 352,
     TASSIGNMINUS = 353,
     TASSIGNMOD = 354,
     TASSIGNMULTIPLY = 355,
     TASSIGNPLUS = 356,
     TASSIGNSL = 357,
     TASSIGNSR = 358,
     TBAND = 359,
     TBNOT = 360,
     TBOR = 361,
     TBXOR = 362,
     TCOLON = 363,
     TCOMMA = 364,
     TDIVIDE = 365,
     TDOT = 366,
     TDOTDOT = 367,
     TDOTDOTDOT = 368,
     TEQUAL = 369,
     TEXP = 370,
     TGREATER = 371,
     TGREATEREQUAL = 372,
     THASH = 373,
     TLESS = 374,
     TLESSEQUAL = 375,
     TMINUS = 376,
     TMOD = 377,
     TNOT = 378,
     TNOTEQUAL = 379,
     TOR = 380,
     TPLUS = 381,
     TQUESTION = 382,
     TSEMI = 383,
     TSHIFTLEFT = 384,
     TSHIFTRIGHT = 385,
     TSTAR = 386,
     TSWAP = 387,
     TASSIGNREDUCE = 388,
     TIO = 389,
     TLCBR = 390,
     TRCBR = 391,
     TLP = 392,
     TRP = 393,
     TLSBR = 394,
     TRSBR = 395,
     TNOELSE = 396,
     TUMINUS = 397,
     TUPLUS = 398
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
#line 188 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 2068 of yacc.c  */
#line 396 "../include/bison-chapel.h"
