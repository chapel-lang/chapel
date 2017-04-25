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

/* Line 2132 of yacc.c  */
#line 32 "chapel.ypp"

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;


/* Line 2132 of yacc.c  */
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


/* Line 2132 of yacc.c  */
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
    FnSymbol*                 pfnsymbol;
    Flag                      flag;
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    IntentExpr                pIntentExpr;
    ForallIntents*            pForallIntents;
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif


/* Line 2132 of yacc.c  */
#line 136 "chapel.ypp"

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


/* Line 2132 of yacc.c  */
#line 158 "chapel.ypp"

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



/* Line 2132 of yacc.c  */
#line 183 "../include/bison-chapel.h"

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
     TCATCH = 271,
     TCLASS = 272,
     TCOBEGIN = 273,
     TCOFORALL = 274,
     TCONFIG = 275,
     TCONST = 276,
     TCONTINUE = 277,
     TDELETE = 278,
     TDMAPPED = 279,
     TDO = 280,
     TDOMAIN = 281,
     TELSE = 282,
     TENUM = 283,
     TEXCEPT = 284,
     TEXPORT = 285,
     TEXTERN = 286,
     TFOR = 287,
     TFORALL = 288,
     TFORWARDING = 289,
     TIF = 290,
     TIN = 291,
     TINDEX = 292,
     TINLINE = 293,
     TINOUT = 294,
     TITER = 295,
     TLABEL = 296,
     TLAMBDA = 297,
     TLET = 298,
     TLOCAL = 299,
     TMINUSMINUS = 300,
     TMODULE = 301,
     TNEW = 302,
     TNIL = 303,
     TNOINIT = 304,
     TON = 305,
     TONLY = 306,
     TOTHERWISE = 307,
     TOUT = 308,
     TPARAM = 309,
     TPLUSPLUS = 310,
     TPRAGMA = 311,
     TPRIMITIVE = 312,
     TPRIVATE = 313,
     TPROC = 314,
     TPUBLIC = 315,
     TRECORD = 316,
     TREDUCE = 317,
     TREF = 318,
     TREQUIRE = 319,
     TRETURN = 320,
     TSCAN = 321,
     TSELECT = 322,
     TSERIAL = 323,
     TSINGLE = 324,
     TSPARSE = 325,
     TSUBDOMAIN = 326,
     TSYNC = 327,
     TTHEN = 328,
     TTHROW = 329,
     TTHROWS = 330,
     TTRY = 331,
     TTRYBANG = 332,
     TTYPE = 333,
     TUNDERSCORE = 334,
     TUNION = 335,
     TUSE = 336,
     TVAR = 337,
     TWHEN = 338,
     TWHERE = 339,
     TWHILE = 340,
     TWITH = 341,
     TYIELD = 342,
     TZIP = 343,
     TALIAS = 344,
     TAND = 345,
     TASSIGN = 346,
     TASSIGNBAND = 347,
     TASSIGNBOR = 348,
     TASSIGNBXOR = 349,
     TASSIGNDIVIDE = 350,
     TASSIGNEXP = 351,
     TASSIGNLAND = 352,
     TASSIGNLOR = 353,
     TASSIGNMINUS = 354,
     TASSIGNMOD = 355,
     TASSIGNMULTIPLY = 356,
     TASSIGNPLUS = 357,
     TASSIGNSL = 358,
     TASSIGNSR = 359,
     TBAND = 360,
     TBNOT = 361,
     TBOR = 362,
     TBXOR = 363,
     TCOLON = 364,
     TCOMMA = 365,
     TDIVIDE = 366,
     TDOT = 367,
     TDOTDOT = 368,
     TDOTDOTDOT = 369,
     TEQUAL = 370,
     TEXP = 371,
     TGREATER = 372,
     TGREATEREQUAL = 373,
     THASH = 374,
     TLESS = 375,
     TLESSEQUAL = 376,
     TMINUS = 377,
     TMOD = 378,
     TNOT = 379,
     TNOTEQUAL = 380,
     TOR = 381,
     TPLUS = 382,
     TQUESTION = 383,
     TSEMI = 384,
     TSHIFTLEFT = 385,
     TSHIFTRIGHT = 386,
     TSTAR = 387,
     TSWAP = 388,
     TASSIGNREDUCE = 389,
     TIO = 390,
     TLCBR = 391,
     TRCBR = 392,
     TLP = 393,
     TRP = 394,
     TLSBR = 395,
     TRSBR = 396,
     TNOELSE = 397,
     TUMINUS = 398,
     TUPLUS = 399
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

/* Line 2132 of yacc.c  */
#line 189 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 2132 of yacc.c  */
#line 398 "../include/bison-chapel.h"
