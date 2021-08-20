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
    TMINUSMINUS = 319,
    TMODULE = 320,
    TNEW = 321,
    TNIL = 322,
    TNOINIT = 323,
    TNONE = 324,
    TNOTHING = 325,
    TON = 326,
    TONLY = 327,
    TOPERATOR = 328,
    TOTHERWISE = 329,
    TOUT = 330,
    TOVERRIDE = 331,
    TOWNED = 332,
    TPARAM = 333,
    TPLUSPLUS = 334,
    TPRAGMA = 335,
    TPRIMITIVE = 336,
    TPRIVATE = 337,
    TPROC = 338,
    TPROTOTYPE = 339,
    TPUBLIC = 340,
    TREAL = 341,
    TRECORD = 342,
    TREDUCE = 343,
    TREF = 344,
    TREQUIRE = 345,
    TRETURN = 346,
    TSCAN = 347,
    TSELECT = 348,
    TSERIAL = 349,
    TSHARED = 350,
    TSINGLE = 351,
    TSPARSE = 352,
    TSTRING = 353,
    TSUBDOMAIN = 354,
    TSYNC = 355,
    TTHEN = 356,
    TTHIS = 357,
    TTHROW = 358,
    TTHROWS = 359,
    TTRUE = 360,
    TTRY = 361,
    TTRYBANG = 362,
    TTYPE = 363,
    TUINT = 364,
    TUNDERSCORE = 365,
    TUNION = 366,
    TUNMANAGED = 367,
    TUSE = 368,
    TVAR = 369,
    TVOID = 370,
    TWHEN = 371,
    TWHERE = 372,
    TWHILE = 373,
    TWITH = 374,
    TYIELD = 375,
    TZIP = 376,
    TALIAS = 377,
    TAND = 378,
    TASSIGN = 379,
    TASSIGNBAND = 380,
    TASSIGNBOR = 381,
    TASSIGNBXOR = 382,
    TASSIGNDIVIDE = 383,
    TASSIGNEXP = 384,
    TASSIGNLAND = 385,
    TASSIGNLOR = 386,
    TASSIGNMINUS = 387,
    TASSIGNMOD = 388,
    TASSIGNMULTIPLY = 389,
    TASSIGNPLUS = 390,
    TASSIGNREDUCE = 391,
    TASSIGNSL = 392,
    TASSIGNSR = 393,
    TBANG = 394,
    TBAND = 395,
    TBNOT = 396,
    TBOR = 397,
    TBXOR = 398,
    TCOLON = 399,
    TCOMMA = 400,
    TDIVIDE = 401,
    TDOT = 402,
    TDOTDOT = 403,
    TDOTDOTDOT = 404,
    TEQUAL = 405,
    TEXP = 406,
    TGREATER = 407,
    TGREATEREQUAL = 408,
    THASH = 409,
    TIO = 410,
    TLESS = 411,
    TLESSEQUAL = 412,
    TMINUS = 413,
    TMOD = 414,
    TNOTEQUAL = 415,
    TOR = 416,
    TPLUS = 417,
    TQUESTION = 418,
    TSEMI = 419,
    TSHIFTLEFT = 420,
    TSHIFTRIGHT = 421,
    TSTAR = 422,
    TSWAP = 423,
    TLCBR = 424,
    TRCBR = 425,
    TLP = 426,
    TRP = 427,
    TLSBR = 428,
    TRSBR = 429,
    TNOELSE = 430,
    TDOTDOTOPENHIGH = 431,
    TUPLUS = 432,
    TUMINUS = 433,
    TLNOT = 434
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

#line 415 "../include/bison-chapel.h" /* yacc.c:1916  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */
