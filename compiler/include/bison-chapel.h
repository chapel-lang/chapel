/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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
#line 33 "chapel.ypp" /* yacc.c:1916  */

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;
#line 47 "chapel.ypp" /* yacc.c:1916  */

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif
#line 67 "chapel.ypp" /* yacc.c:1916  */

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
#line 152 "chapel.ypp" /* yacc.c:1916  */

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
#line 175 "chapel.ypp" /* yacc.c:1916  */

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

#line 190 "../include/bison-chapel.h" /* yacc.c:1916  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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
    TFOREACH = 297,
    TFORWARDING = 298,
    TIF = 299,
    TIMAG = 300,
    TIMPORT = 301,
    TIN = 302,
    TINCLUDE = 303,
    TINDEX = 304,
    TINLINE = 305,
    TINOUT = 306,
    TINT = 307,
    TITER = 308,
    TINITEQUALS = 309,
    TIMPLEMENTS = 310,
    TINTERFACE = 311,
    TLABEL = 312,
    TLAMBDA = 313,
    TLET = 314,
    TLIFETIME = 315,
    TLOCAL = 316,
    TLOCALE = 317,
    TMINUSMINUS = 318,
    TMODULE = 319,
    TNEW = 320,
    TNIL = 321,
    TNOINIT = 322,
    TNONE = 323,
    TNOTHING = 324,
    TON = 325,
    TONLY = 326,
    TOPERATOR = 327,
    TOTHERWISE = 328,
    TOUT = 329,
    TOVERRIDE = 330,
    TOWNED = 331,
    TPARAM = 332,
    TPLUSPLUS = 333,
    TPRAGMA = 334,
    TPRIMITIVE = 335,
    TPRIVATE = 336,
    TPROC = 337,
    TPROTOTYPE = 338,
    TPUBLIC = 339,
    TREAL = 340,
    TRECORD = 341,
    TREDUCE = 342,
    TREF = 343,
    TREQUIRE = 344,
    TRETURN = 345,
    TSCAN = 346,
    TSELECT = 347,
    TSERIAL = 348,
    TSHARED = 349,
    TSINGLE = 350,
    TSPARSE = 351,
    TSTRING = 352,
    TSUBDOMAIN = 353,
    TSYNC = 354,
    TTHEN = 355,
    TTHIS = 356,
    TTHROW = 357,
    TTHROWS = 358,
    TTRUE = 359,
    TTRY = 360,
    TTRYBANG = 361,
    TTYPE = 362,
    TUINT = 363,
    TUNDERSCORE = 364,
    TUNION = 365,
    TUNMANAGED = 366,
    TUSE = 367,
    TVAR = 368,
    TVOID = 369,
    TWHEN = 370,
    TWHERE = 371,
    TWHILE = 372,
    TWITH = 373,
    TYIELD = 374,
    TZIP = 375,
    TALIAS = 376,
    TAND = 377,
    TASSIGN = 378,
    TASSIGNBAND = 379,
    TASSIGNBOR = 380,
    TASSIGNBXOR = 381,
    TASSIGNDIVIDE = 382,
    TASSIGNEXP = 383,
    TASSIGNLAND = 384,
    TASSIGNLOR = 385,
    TASSIGNMINUS = 386,
    TASSIGNMOD = 387,
    TASSIGNMULTIPLY = 388,
    TASSIGNPLUS = 389,
    TASSIGNREDUCE = 390,
    TASSIGNSL = 391,
    TASSIGNSR = 392,
    TBANG = 393,
    TBAND = 394,
    TBNOT = 395,
    TBOR = 396,
    TBXOR = 397,
    TCOLON = 398,
    TCOMMA = 399,
    TDIVIDE = 400,
    TDOT = 401,
    TDOTDOT = 402,
    TDOTDOTDOT = 403,
    TEQUAL = 404,
    TEXP = 405,
    TGREATER = 406,
    TGREATEREQUAL = 407,
    THASH = 408,
    TIO = 409,
    TLESS = 410,
    TLESSEQUAL = 411,
    TMINUS = 412,
    TMOD = 413,
    TNOTEQUAL = 414,
    TOR = 415,
    TPLUS = 416,
    TQUESTION = 417,
    TSEMI = 418,
    TSHIFTLEFT = 419,
    TSHIFTRIGHT = 420,
    TSTAR = 421,
    TSWAP = 422,
    TLCBR = 423,
    TRCBR = 424,
    TLP = 425,
    TRP = 426,
    TLSBR = 427,
    TRSBR = 428,
    TNOELSE = 429,
    TDOTDOTOPENHIGH = 430,
    TUPLUS = 431,
    TUMINUS = 432,
    TLNOT = 433
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
#line 206 "chapel.ypp" /* yacc.c:1916  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 414 "../include/bison-chapel.h" /* yacc.c:1916  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
