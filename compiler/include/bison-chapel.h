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
#line 46 "chapel.ypp"

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
#line 66 "chapel.ypp"

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
  class NamedExpr;
  class Type;

  enum   ProcIter {
    ProcIter_PROC,
    ProcIter_ITER
  };

  struct PotentialRename {
    enum{SINGLE, DOUBLE} tag;

    union {
      Expr*  elem;
      std::pair<Expr*, Expr*>* renamed;
    };
  };

  struct WhereAndLifetime {
    Expr* where;
    Expr* lifetime;
  };
  static inline
  WhereAndLifetime makeWhereAndLifetime(Expr* w, Expr* lt) {
    WhereAndLifetime ret;
    ret.where = w;
    ret.lifetime = lt;
    return ret;
  }

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
    std::vector<PotentialRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif

/* Line 2058 of yacc.c  */
#line 147 "chapel.ypp"

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

/* Line 2058 of yacc.c  */
#line 170 "chapel.ypp"

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
#line 197 "../include/bison-chapel.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TIDENT = 258,
     TQUERIEDIDENT = 259,
     INTLITERAL = 260,
     REALLITERAL = 261,
     IMAGLITERAL = 262,
     STRINGLITERAL = 263,
     BYTESLITERAL = 264,
     CSTRINGLITERAL = 265,
     EXTERNCODE = 266,
     TALIGN = 267,
     TAS = 268,
     TATOMIC = 269,
     TBEGIN = 270,
     TBREAK = 271,
     TBOOL = 272,
     TBORROWED = 273,
     TBY = 274,
     TBYTES = 275,
     TCATCH = 276,
     TCLASS = 277,
     TCOBEGIN = 278,
     TCOFORALL = 279,
     TCOMPLEX = 280,
     TCONFIG = 281,
     TCONST = 282,
     TCONTINUE = 283,
     TDEFER = 284,
     TDELETE = 285,
     TDMAPPED = 286,
     TDO = 287,
     TDOMAIN = 288,
     TELSE = 289,
     TENUM = 290,
     TEXCEPT = 291,
     TEXPORT = 292,
     TEXTERN = 293,
     TFALSE = 294,
     TFOR = 295,
     TFORALL = 296,
     TFORWARDING = 297,
     TIF = 298,
     TIMAG = 299,
     TIMPORT = 300,
     TIN = 301,
     TINDEX = 302,
     TINLINE = 303,
     TINOUT = 304,
     TINT = 305,
     TITER = 306,
     TINITEQUALS = 307,
     TLABEL = 308,
     TLAMBDA = 309,
     TLET = 310,
     TLIFETIME = 311,
     TLOCAL = 312,
     TLOCALE = 313,
     TMINUSMINUS = 314,
     TMODULE = 315,
     TNEW = 316,
     TNIL = 317,
     TNOINIT = 318,
     TNONE = 319,
     TNOTHING = 320,
     TON = 321,
     TONLY = 322,
     TOTHERWISE = 323,
     TOUT = 324,
     TOVERRIDE = 325,
     TOWNED = 326,
     TPARAM = 327,
     TPLUSPLUS = 328,
     TPRAGMA = 329,
     TPRIMITIVE = 330,
     TPRIVATE = 331,
     TPROC = 332,
     TPROTOTYPE = 333,
     TPUBLIC = 334,
     TREAL = 335,
     TRECORD = 336,
     TREDUCE = 337,
     TREF = 338,
     TREQUIRE = 339,
     TRETURN = 340,
     TSCAN = 341,
     TSELECT = 342,
     TSERIAL = 343,
     TSHARED = 344,
     TSINGLE = 345,
     TSPARSE = 346,
     TSTRING = 347,
     TSUBDOMAIN = 348,
     TSYNC = 349,
     TTHEN = 350,
     TTHIS = 351,
     TTHROW = 352,
     TTHROWS = 353,
     TTRUE = 354,
     TTRY = 355,
     TTRYBANG = 356,
     TTYPE = 357,
     TUINT = 358,
     TUNDERSCORE = 359,
     TUNION = 360,
     TUNMANAGED = 361,
     TUSE = 362,
     TVAR = 363,
     TVOID = 364,
     TWHEN = 365,
     TWHERE = 366,
     TWHILE = 367,
     TWITH = 368,
     TYIELD = 369,
     TZIP = 370,
     TALIAS = 371,
     TAND = 372,
     TASSIGN = 373,
     TASSIGNBAND = 374,
     TASSIGNBOR = 375,
     TASSIGNBXOR = 376,
     TASSIGNDIVIDE = 377,
     TASSIGNEXP = 378,
     TASSIGNLAND = 379,
     TASSIGNLOR = 380,
     TASSIGNMINUS = 381,
     TASSIGNMOD = 382,
     TASSIGNMULTIPLY = 383,
     TASSIGNPLUS = 384,
     TASSIGNREDUCE = 385,
     TASSIGNSL = 386,
     TASSIGNSR = 387,
     TBANG = 388,
     TBAND = 389,
     TBNOT = 390,
     TBOR = 391,
     TBXOR = 392,
     TCOLON = 393,
     TCOMMA = 394,
     TDIVIDE = 395,
     TDOT = 396,
     TDOTDOT = 397,
     TDOTDOTDOT = 398,
     TEQUAL = 399,
     TEXP = 400,
     TGREATER = 401,
     TGREATEREQUAL = 402,
     THASH = 403,
     TIO = 404,
     TLESS = 405,
     TLESSEQUAL = 406,
     TMINUS = 407,
     TMOD = 408,
     TNOTEQUAL = 409,
     TOR = 410,
     TPLUS = 411,
     TQUESTION = 412,
     TSEMI = 413,
     TSHIFTLEFT = 414,
     TSHIFTRIGHT = 415,
     TSTAR = 416,
     TSWAP = 417,
     TLCBR = 418,
     TRCBR = 419,
     TLP = 420,
     TRP = 421,
     TLSBR = 422,
     TRSBR = 423,
     TNOELSE = 424,
     TDOTDOTOPENHIGH = 425,
     TUMINUS = 426,
     TUPLUS = 427,
     TLNOT = 428
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
#line 201 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 2058 of yacc.c  */
#line 435 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
