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
     TINCLUDE = 302,
     TINDEX = 303,
     TINLINE = 304,
     TINOUT = 305,
     TINT = 306,
     TITER = 307,
     TINITEQUALS = 308,
     TIMPLEMENTS = 309,
     TINTERFACE = 310,
     TLABEL = 311,
     TLAMBDA = 312,
     TLET = 313,
     TLIFETIME = 314,
     TLOCAL = 315,
     TLOCALE = 316,
     TMINUSMINUS = 317,
     TMODULE = 318,
     TNEW = 319,
     TNIL = 320,
     TNOINIT = 321,
     TNONE = 322,
     TNOTHING = 323,
     TON = 324,
     TONLY = 325,
     TOPERATOR = 326,
     TOTHERWISE = 327,
     TOUT = 328,
     TOVERRIDE = 329,
     TOWNED = 330,
     TPARAM = 331,
     TPLUSPLUS = 332,
     TPRAGMA = 333,
     TPRIMITIVE = 334,
     TPRIVATE = 335,
     TPROC = 336,
     TPROTOTYPE = 337,
     TPUBLIC = 338,
     TREAL = 339,
     TRECORD = 340,
     TREDUCE = 341,
     TREF = 342,
     TREQUIRE = 343,
     TRETURN = 344,
     TSCAN = 345,
     TSELECT = 346,
     TSERIAL = 347,
     TSHARED = 348,
     TSINGLE = 349,
     TSPARSE = 350,
     TSTRING = 351,
     TSUBDOMAIN = 352,
     TSYNC = 353,
     TTHEN = 354,
     TTHIS = 355,
     TTHROW = 356,
     TTHROWS = 357,
     TTRUE = 358,
     TTRY = 359,
     TTRYBANG = 360,
     TTYPE = 361,
     TUINT = 362,
     TUNDERSCORE = 363,
     TUNION = 364,
     TUNMANAGED = 365,
     TUSE = 366,
     TVAR = 367,
     TVOID = 368,
     TWHEN = 369,
     TWHERE = 370,
     TWHILE = 371,
     TWITH = 372,
     TYIELD = 373,
     TZIP = 374,
     TALIAS = 375,
     TAND = 376,
     TASSIGN = 377,
     TASSIGNBAND = 378,
     TASSIGNBOR = 379,
     TASSIGNBXOR = 380,
     TASSIGNDIVIDE = 381,
     TASSIGNEXP = 382,
     TASSIGNLAND = 383,
     TASSIGNLOR = 384,
     TASSIGNMINUS = 385,
     TASSIGNMOD = 386,
     TASSIGNMULTIPLY = 387,
     TASSIGNPLUS = 388,
     TASSIGNREDUCE = 389,
     TASSIGNSL = 390,
     TASSIGNSR = 391,
     TBANG = 392,
     TBAND = 393,
     TBNOT = 394,
     TBOR = 395,
     TBXOR = 396,
     TCOLON = 397,
     TCOMMA = 398,
     TDIVIDE = 399,
     TDOT = 400,
     TDOTDOT = 401,
     TDOTDOTDOT = 402,
     TEQUAL = 403,
     TEXP = 404,
     TGREATER = 405,
     TGREATEREQUAL = 406,
     THASH = 407,
     TIO = 408,
     TLESS = 409,
     TLESSEQUAL = 410,
     TMINUS = 411,
     TMOD = 412,
     TNOTEQUAL = 413,
     TOR = 414,
     TPLUS = 415,
     TQUESTION = 416,
     TSEMI = 417,
     TSHIFTLEFT = 418,
     TSHIFTRIGHT = 419,
     TSTAR = 420,
     TSWAP = 421,
     TLCBR = 422,
     TRCBR = 423,
     TLP = 424,
     TRP = 425,
     TLSBR = 426,
     TRSBR = 427,
     TNOELSE = 428,
     TDOTDOTOPENHIGH = 429,
     TUMINUS = 430,
     TUPLUS = 431,
     TLNOT = 432
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
#line 443 "../include/bison-chapel.h"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
