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

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;


/* Line 2068 of yacc.c  */
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


/* Line 2068 of yacc.c  */
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
    ImportStmt*               pimportstmt;
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<PotentialRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif


/* Line 2068 of yacc.c  */
#line 151 "chapel.ypp"

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


/* Line 2068 of yacc.c  */
#line 174 "chapel.ypp"

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
     TOTHERWISE = 326,
     TOUT = 327,
     TOVERRIDE = 328,
     TOWNED = 329,
     TPARAM = 330,
     TPLUSPLUS = 331,
     TPRAGMA = 332,
     TPRIMITIVE = 333,
     TPRIVATE = 334,
     TPROC = 335,
     TPROTOTYPE = 336,
     TPUBLIC = 337,
     TREAL = 338,
     TRECORD = 339,
     TREDUCE = 340,
     TREF = 341,
     TREQUIRE = 342,
     TRETURN = 343,
     TSCAN = 344,
     TSELECT = 345,
     TSERIAL = 346,
     TSHARED = 347,
     TSINGLE = 348,
     TSPARSE = 349,
     TSTRING = 350,
     TSUBDOMAIN = 351,
     TSYNC = 352,
     TTHEN = 353,
     TTHIS = 354,
     TTHROW = 355,
     TTHROWS = 356,
     TTRUE = 357,
     TTRY = 358,
     TTRYBANG = 359,
     TTYPE = 360,
     TUINT = 361,
     TUNDERSCORE = 362,
     TUNION = 363,
     TUNMANAGED = 364,
     TUSE = 365,
     TVAR = 366,
     TVOID = 367,
     TWHEN = 368,
     TWHERE = 369,
     TWHILE = 370,
     TWITH = 371,
     TYIELD = 372,
     TZIP = 373,
     TALIAS = 374,
     TAND = 375,
     TASSIGN = 376,
     TASSIGNBAND = 377,
     TASSIGNBOR = 378,
     TASSIGNBXOR = 379,
     TASSIGNDIVIDE = 380,
     TASSIGNEXP = 381,
     TASSIGNLAND = 382,
     TASSIGNLOR = 383,
     TASSIGNMINUS = 384,
     TASSIGNMOD = 385,
     TASSIGNMULTIPLY = 386,
     TASSIGNPLUS = 387,
     TASSIGNREDUCE = 388,
     TASSIGNSL = 389,
     TASSIGNSR = 390,
     TBANG = 391,
     TBAND = 392,
     TBNOT = 393,
     TBOR = 394,
     TBXOR = 395,
     TCOLON = 396,
     TCOMMA = 397,
     TDIVIDE = 398,
     TDOT = 399,
     TDOTDOT = 400,
     TDOTDOTDOT = 401,
     TEQUAL = 402,
     TEXP = 403,
     TGREATER = 404,
     TGREATEREQUAL = 405,
     THASH = 406,
     TIO = 407,
     TLESS = 408,
     TLESSEQUAL = 409,
     TMINUS = 410,
     TMOD = 411,
     TNOTEQUAL = 412,
     TOR = 413,
     TPLUS = 414,
     TQUESTION = 415,
     TSEMI = 416,
     TSHIFTLEFT = 417,
     TSHIFTRIGHT = 418,
     TSTAR = 419,
     TSWAP = 420,
     TLCBR = 421,
     TRCBR = 422,
     TLP = 423,
     TRP = 424,
     TLSBR = 425,
     TRSBR = 426,
     TNOELSE = 427,
     TDOTDOTOPENHIGH = 428,
     TUMINUS = 429,
     TUPLUS = 430,
     TLNOT = 431
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
#line 205 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 2068 of yacc.c  */
#line 444 "../include/bison-chapel.h"
