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
#line 33 "chapel.ypp"

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;

/* Line 2058 of yacc.c  */
#line 47 "chapel.ypp"

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
#line 67 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_1_
  #define _BISON_CHAPEL_DEFINES_1_

  #include "symbol.h"

  #include "ImportStmt.h"

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

  enum   ProcIterOp {
    ProcIterOp_PROC,
    ProcIterOp_ITER,
    ProcIterOp_OP
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
    ImportStmt*               pimportstmt;
    ProcIterOp                procIterOp;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<PotentialRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif

/* Line 2058 of yacc.c  */
#line 152 "chapel.ypp"

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
#line 175 "chapel.ypp"

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
#line 201 "../include/bison-chapel.h"

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
     TDEPRECATED = 286,
     TDMAPPED = 287,
     TDO = 288,
     TDOMAIN = 289,
     TELSE = 290,
     TENUM = 291,
     TEXCEPT = 292,
     TEXPORT = 293,
     TEXTERN = 294,
     TFALSE = 295,
     TFOR = 296,
     TFORALL = 297,
     TFOREACH = 298,
     TFORWARDING = 299,
     TIF = 300,
     TIMAG = 301,
     TIMPORT = 302,
     TIN = 303,
     TINCLUDE = 304,
     TINDEX = 305,
     TINLINE = 306,
     TINOUT = 307,
     TINT = 308,
     TITER = 309,
     TINITEQUALS = 310,
     TIMPLEMENTS = 311,
     TINTERFACE = 312,
     TLABEL = 313,
     TLAMBDA = 314,
     TLET = 315,
     TLIFETIME = 316,
     TLOCAL = 317,
     TLOCALE = 318,
     TMANAGE = 319,
     TMINUSMINUS = 320,
     TMODULE = 321,
     TNEW = 322,
     TNIL = 323,
     TNOINIT = 324,
     TNONE = 325,
     TNOTHING = 326,
     TON = 327,
     TONLY = 328,
     TOPERATOR = 329,
     TOTHERWISE = 330,
     TOUT = 331,
     TOVERRIDE = 332,
     TOWNED = 333,
     TPARAM = 334,
     TPLUSPLUS = 335,
     TPRAGMA = 336,
     TPRIMITIVE = 337,
     TPRIVATE = 338,
     TPROC = 339,
     TPROTOTYPE = 340,
     TPUBLIC = 341,
     TREAL = 342,
     TRECORD = 343,
     TREDUCE = 344,
     TREF = 345,
     TREQUIRE = 346,
     TRETURN = 347,
     TSCAN = 348,
     TSELECT = 349,
     TSERIAL = 350,
     TSHARED = 351,
     TSINGLE = 352,
     TSPARSE = 353,
     TSTRING = 354,
     TSUBDOMAIN = 355,
     TSYNC = 356,
     TTHEN = 357,
     TTHIS = 358,
     TTHROW = 359,
     TTHROWS = 360,
     TTRUE = 361,
     TTRY = 362,
     TTRYBANG = 363,
     TTYPE = 364,
     TUINT = 365,
     TUNDERSCORE = 366,
     TUNION = 367,
     TUNMANAGED = 368,
     TUSE = 369,
     TVAR = 370,
     TVOID = 371,
     TWHEN = 372,
     TWHERE = 373,
     TWHILE = 374,
     TWITH = 375,
     TYIELD = 376,
     TZIP = 377,
     TALIAS = 378,
     TAND = 379,
     TASSIGN = 380,
     TASSIGNBAND = 381,
     TASSIGNBOR = 382,
     TASSIGNBXOR = 383,
     TASSIGNDIVIDE = 384,
     TASSIGNEXP = 385,
     TASSIGNLAND = 386,
     TASSIGNLOR = 387,
     TASSIGNMINUS = 388,
     TASSIGNMOD = 389,
     TASSIGNMULTIPLY = 390,
     TASSIGNPLUS = 391,
     TASSIGNREDUCE = 392,
     TASSIGNSL = 393,
     TASSIGNSR = 394,
     TBANG = 395,
     TBAND = 396,
     TBNOT = 397,
     TBOR = 398,
     TBXOR = 399,
     TCOLON = 400,
     TCOMMA = 401,
     TDIVIDE = 402,
     TDOT = 403,
     TDOTDOT = 404,
     TDOTDOTDOT = 405,
     TEQUAL = 406,
     TEXP = 407,
     TGREATER = 408,
     TGREATEREQUAL = 409,
     THASH = 410,
     TIO = 411,
     TLESS = 412,
     TLESSEQUAL = 413,
     TMINUS = 414,
     TMOD = 415,
     TNOTEQUAL = 416,
     TOR = 417,
     TPLUS = 418,
     TQUESTION = 419,
     TSEMI = 420,
     TSHIFTLEFT = 421,
     TSHIFTRIGHT = 422,
     TSTAR = 423,
     TSWAP = 424,
     TLCBR = 425,
     TRCBR = 426,
     TLP = 427,
     TRP = 428,
     TLSBR = 429,
     TRSBR = 430,
     TNOELSE = 431,
     TDOTDOTOPENHIGH = 432,
     TUMINUS = 433,
     TUPLUS = 434,
     TLNOT = 435
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
#line 206 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 2058 of yacc.c  */
#line 446 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
