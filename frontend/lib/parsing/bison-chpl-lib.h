/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YYCHPL_BISON_CHPL_LIB_H_INCLUDED
# define YY_YYCHPL_BISON_CHPL_LIB_H_INCLUDED
/* Debug traces.  */
#ifndef YYCHPL_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define YYCHPL_DEBUG 1
#  else
#   define YYCHPL_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define YYCHPL_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined YYCHPL_DEBUG */
#if YYCHPL_DEBUG
extern int yychpl_debug;
#endif
/* "%code requires" blocks.  */
#line 72 "chpl.ypp"

  #include "parser-dependencies.h"
  #include "chpl/util/assertions.h"
#line 78 "chpl.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  namespace chpl {
    int processNewline(yyscan_t scanner);
  }

  #endif
#line 104 "chpl.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_1_
  #define _BISON_CHAPEL_DEFINES_1_

  // TODO: Replace me with bool.
  enum ThrowsTag {
    ThrowsTag_DEFAULT,
    ThrowsTag_THROWS,
  };

  using ParserExprList = std::vector<AstNode*>;
  using UniqueStrList = std::vector<PODUniqueString>;

  // these structures do not use constructors to avoid
  // errors about being in the union below. Additionally
  // they only have pointer or numeric fields.

  struct WhereAndLifetime {
    AstNode* where;
    ParserExprList* lifetime;
  };
  static inline
  WhereAndLifetime makeWhereAndLifetime(AstNode* w, ParserExprList* lt) {
    WhereAndLifetime ret = {w, lt};
    return ret;
  }

  // This is used to propagate any comments that hug the top of a statement
  // upwards through parser rules.
  struct CommentsAndStmt {
    std::vector<ParserComment>* comments;
    AstNode* stmt;
  };
  static inline
  CommentsAndStmt makeCommentsAndStmt(std::vector<ParserComment>* comments,
                                      AstNode* stmt) {
    CommentsAndStmt ret = {comments, stmt};
    return ret;
  }
  static inline
  CommentsAndStmt makeCommentsAndStmt(std::vector<ParserComment>* comments,
                                      owned<AstNode> stmt) {
    return makeCommentsAndStmt(comments, stmt.release());
  }

  // A struct for storing a partially constructed function prototype
  // during parsing for linkage_spec/fn_decl_stmt/etc
  // It is just saving some components of a function to be used
  // when building the FunctionDecl.
  struct FunctionParts {
    bool isBodyNonBlockExpression;
    std::vector<ParserComment>* comments;
    ErroneousExpression* errorExpr; // only used for parser error
    AttributeGroup* attributes;
    Decl::Visibility visibility;
    Decl::Linkage linkage;
    AstNode* linkageNameExpr;
    bool isInline;
    bool isOverride;
    Function::Kind kind;
    Formal::Intent thisIntent;
    Formal* receiver;
    Identifier* name;
    Function::ReturnIntent returnIntent;
    bool throws;
    ParserExprList* formals;
    AstNode* returnType;
    AstNode* where;
    ParserExprList* lifetime;
    ParserExprList* body;
  };

  // A struct to thread along some pieces of a module before it is built.
  struct ModuleParts {
    std::vector<ParserComment>* comments;
    AttributeGroup* attributes;
    Decl::Visibility visibility;
    Module::Kind kind;
    PODUniqueString name;
  };

  // A struct to thread along some pieces of a type before it is built.
  struct TypeDeclParts {
    std::vector<ParserComment>* comments;
    Decl::Visibility visibility;
    Decl::Linkage linkage;
    AstNode* linkageName;
    AttributeGroup* attributes;
    PODUniqueString name;
    asttags::AstTag tag;
  };

  // This is produced by do_stmt. It records whether the do_stmt statements
  // were produced following a 'do' or not. E.g...
  // do { ... } vs { ... }
  struct BlockOrDo {
    CommentsAndStmt cs;
    bool usesDo;
  };

  struct SizedStr {
    const char* allocatedData; // *not* a uniqued string!
    long size;
  };

  struct MaybeNamedActual {
    AstNode* expr;
    PODUniqueString name;
  };
  static inline
  MaybeNamedActual makeMaybeNamedActual(AstNode* expr,
                                        PODUniqueString name) {
    MaybeNamedActual ret = {expr, name};
    return ret;
  }
  static inline
  MaybeNamedActual makeMaybeNamedActual(AstNode* expr) {
    PODUniqueString emptyName = PODUniqueString::get();
    MaybeNamedActual ret = {expr, emptyName};
    return ret;
  }

  using MaybeNamedActualList = std::vector<MaybeNamedActual>;

  // These currently all need to be POD types (i.e. pointers and ints)
  // rather than say std::vector to keep bison working with the current
  // strategy. In the future we could probably switch to a more
  // C++ mode of using bison.
  union YYCHPL_STYPE {
    // The lexer only uses these
    PODUniqueString uniqueStr;
    SizedStr sizedStr;
    AstNode* expr;

    // The remaining types are used only in parser productions

    // integer/enum values

    asttags::AstTag astTag;
    Formal::Intent intentTag;
    Function::Kind functionKind;
    Function::ReturnIntent returnTag;
    Decl::Linkage linkageTag;
    Module::Kind moduleKind;
    New::Management newManagement;
    TaskVar::Intent taskIntent;
    Decl::Visibility visibilityTag;
    ThrowsTag throwsTag;
    Variable::Kind variableKind;

    // simple pointer values
    AttributeGroup* attribute;
    Block* block;
    Call* call;
    Function* function;
    Module* module;
    WithClause* withClause;

    // values that are temporary groupings
    BlockOrDo blockOrDo;
    CommentsAndStmt commentsAndStmt;
    TypeDeclParts typeDeclParts;
    FunctionParts functionParts;
    MaybeNamedActual maybeNamedActual;
    MaybeNamedActualList* maybeNamedActualList;
    ModuleParts moduleParts;
    ParserExprList* exprList;
    UniqueStrList* uniqueStrList;
    WhereAndLifetime lifetimeAndWhere;
  };

  // Put our types in a different namespace to avoid conflicting with the
  // production compiler parser's YYSTYPE/YYLTYPE.
  #define YYSTYPE YYCHPL_STYPE
  #define YYLTYPE YYCHPL_LTYPE

  // Note that the 'YYSTYPE_IS_TRIVIAL' macro tells the generated parser
  // that YYSTYPE only contains simple scalars (it can be bitcopied).
  // This is normally communicated by the '%union' directive, but we
  // stopped using that.
  #define YYCHPL_STYPE_IS_TRIVIAL 1

  #endif
#line 303 "chpl.ypp"

  // forward declare ParserContext
  struct ParserContext;

#line 266 "bison-chpl-lib.h"

/* Token kinds.  */
#ifndef YYCHPL_TOKENTYPE
# define YYCHPL_TOKENTYPE
  enum yychpl_tokentype
  {
    YYCHPL_EMPTY = -2,
    YYCHPL_EOF = 0,                /* "end of file"  */
    YYCHPL_error = 256,            /* error  */
    YYCHPL_UNDEF = 257,            /* "invalid token"  */
    TIDENT = 258,                  /* TIDENT  */
    TQUERIEDIDENT = 259,           /* TQUERIEDIDENT  */
    INTLITERAL = 260,              /* INTLITERAL  */
    REALLITERAL = 261,             /* REALLITERAL  */
    IMAGLITERAL = 262,             /* IMAGLITERAL  */
    STRINGLITERAL = 263,           /* STRINGLITERAL  */
    BYTESLITERAL = 264,            /* BYTESLITERAL  */
    CSTRINGLITERAL = 265,          /* CSTRINGLITERAL  */
    EXTERNCODE = 266,              /* EXTERNCODE  */
    TALIGN = 267,                  /* TALIGN  */
    TAS = 268,                     /* TAS  */
    TATOMIC = 269,                 /* TATOMIC  */
    TBEGIN = 270,                  /* TBEGIN  */
    TBREAK = 271,                  /* TBREAK  */
    TBOOL = 272,                   /* TBOOL  */
    TBORROWED = 273,               /* TBORROWED  */
    TBY = 274,                     /* TBY  */
    TBYTES = 275,                  /* TBYTES  */
    TCATCH = 276,                  /* TCATCH  */
    TCLASS = 277,                  /* TCLASS  */
    TCOBEGIN = 278,                /* TCOBEGIN  */
    TCOFORALL = 279,               /* TCOFORALL  */
    TCOMPLEX = 280,                /* TCOMPLEX  */
    TCONFIG = 281,                 /* TCONFIG  */
    TCONST = 282,                  /* TCONST  */
    TCONTINUE = 283,               /* TCONTINUE  */
    TDEFER = 284,                  /* TDEFER  */
    TDELETE = 285,                 /* TDELETE  */
    TDEPRECATED = 286,             /* TDEPRECATED  */
    TDMAPPED = 287,                /* TDMAPPED  */
    TDO = 288,                     /* TDO  */
    TDOMAIN = 289,                 /* TDOMAIN  */
    TELSE = 290,                   /* TELSE  */
    TENUM = 291,                   /* TENUM  */
    TEXCEPT = 292,                 /* TEXCEPT  */
    TEXPORT = 293,                 /* TEXPORT  */
    TEXTERN = 294,                 /* TEXTERN  */
    TFALSE = 295,                  /* TFALSE  */
    TFOR = 296,                    /* TFOR  */
    TFORALL = 297,                 /* TFORALL  */
    TFOREACH = 298,                /* TFOREACH  */
    TFORWARDING = 299,             /* TFORWARDING  */
    TIF = 300,                     /* TIF  */
    TIMAG = 301,                   /* TIMAG  */
    TIMPORT = 302,                 /* TIMPORT  */
    TIN = 303,                     /* TIN  */
    TINCLUDE = 304,                /* TINCLUDE  */
    TINDEX = 305,                  /* TINDEX  */
    TINLINE = 306,                 /* TINLINE  */
    TINOUT = 307,                  /* TINOUT  */
    TINT = 308,                    /* TINT  */
    TITER = 309,                   /* TITER  */
    TINITEQUALS = 310,             /* TINITEQUALS  */
    TIMPLEMENTS = 311,             /* TIMPLEMENTS  */
    TINTERFACE = 312,              /* TINTERFACE  */
    TLABEL = 313,                  /* TLABEL  */
    TLAMBDA = 314,                 /* TLAMBDA  */
    TLET = 315,                    /* TLET  */
    TLIFETIME = 316,               /* TLIFETIME  */
    TLOCAL = 317,                  /* TLOCAL  */
    TLOCALE = 318,                 /* TLOCALE  */
    TMANAGE = 319,                 /* TMANAGE  */
    TMINUSMINUS = 320,             /* TMINUSMINUS  */
    TMODULE = 321,                 /* TMODULE  */
    TNEW = 322,                    /* TNEW  */
    TNIL = 323,                    /* TNIL  */
    TNOINIT = 324,                 /* TNOINIT  */
    TNONE = 325,                   /* TNONE  */
    TNOTHING = 326,                /* TNOTHING  */
    TON = 327,                     /* TON  */
    TONLY = 328,                   /* TONLY  */
    TOPERATOR = 329,               /* TOPERATOR  */
    TOTHERWISE = 330,              /* TOTHERWISE  */
    TOUT = 331,                    /* TOUT  */
    TOVERRIDE = 332,               /* TOVERRIDE  */
    TOWNED = 333,                  /* TOWNED  */
    TPARAM = 334,                  /* TPARAM  */
    TPLUSPLUS = 335,               /* TPLUSPLUS  */
    TPRAGMA = 336,                 /* TPRAGMA  */
    TPRIMITIVE = 337,              /* TPRIMITIVE  */
    TPRIVATE = 338,                /* TPRIVATE  */
    TPROC = 339,                   /* TPROC  */
    TPROTOTYPE = 340,              /* TPROTOTYPE  */
    TPUBLIC = 341,                 /* TPUBLIC  */
    TPROCLP = 342,                 /* TPROCLP  */
    TREAL = 343,                   /* TREAL  */
    TRECORD = 344,                 /* TRECORD  */
    TREDUCE = 345,                 /* TREDUCE  */
    TREF = 346,                    /* TREF  */
    TREQUIRE = 347,                /* TREQUIRE  */
    TRETURN = 348,                 /* TRETURN  */
    TSCAN = 349,                   /* TSCAN  */
    TSELECT = 350,                 /* TSELECT  */
    TSERIAL = 351,                 /* TSERIAL  */
    TSHARED = 352,                 /* TSHARED  */
    TSINGLE = 353,                 /* TSINGLE  */
    TSPARSE = 354,                 /* TSPARSE  */
    TSTRING = 355,                 /* TSTRING  */
    TSUBDOMAIN = 356,              /* TSUBDOMAIN  */
    TSYNC = 357,                   /* TSYNC  */
    TTHEN = 358,                   /* TTHEN  */
    TTHIS = 359,                   /* TTHIS  */
    TTHROW = 360,                  /* TTHROW  */
    TTHROWS = 361,                 /* TTHROWS  */
    TTRUE = 362,                   /* TTRUE  */
    TTRY = 363,                    /* TTRY  */
    TTRYBANG = 364,                /* TTRYBANG  */
    TTYPE = 365,                   /* TTYPE  */
    TUINT = 366,                   /* TUINT  */
    TUNDERSCORE = 367,             /* TUNDERSCORE  */
    TUNION = 368,                  /* TUNION  */
    TUNMANAGED = 369,              /* TUNMANAGED  */
    TUNSTABLE = 370,               /* TUNSTABLE  */
    TUSE = 371,                    /* TUSE  */
    TVAR = 372,                    /* TVAR  */
    TVOID = 373,                   /* TVOID  */
    TWHEN = 374,                   /* TWHEN  */
    TWHERE = 375,                  /* TWHERE  */
    TWHILE = 376,                  /* TWHILE  */
    TWITH = 377,                   /* TWITH  */
    TYIELD = 378,                  /* TYIELD  */
    TZIP = 379,                    /* TZIP  */
    TALIAS = 380,                  /* TALIAS  */
    TAND = 381,                    /* TAND  */
    TASSIGN = 382,                 /* TASSIGN  */
    TASSIGNBAND = 383,             /* TASSIGNBAND  */
    TASSIGNBOR = 384,              /* TASSIGNBOR  */
    TASSIGNBXOR = 385,             /* TASSIGNBXOR  */
    TASSIGNDIVIDE = 386,           /* TASSIGNDIVIDE  */
    TASSIGNEXP = 387,              /* TASSIGNEXP  */
    TASSIGNLAND = 388,             /* TASSIGNLAND  */
    TASSIGNLOR = 389,              /* TASSIGNLOR  */
    TASSIGNMINUS = 390,            /* TASSIGNMINUS  */
    TASSIGNMOD = 391,              /* TASSIGNMOD  */
    TASSIGNMULTIPLY = 392,         /* TASSIGNMULTIPLY  */
    TASSIGNPLUS = 393,             /* TASSIGNPLUS  */
    TASSIGNREDUCE = 394,           /* TASSIGNREDUCE  */
    TASSIGNSL = 395,               /* TASSIGNSL  */
    TASSIGNSR = 396,               /* TASSIGNSR  */
    TBANG = 397,                   /* TBANG  */
    TBAND = 398,                   /* TBAND  */
    TBNOT = 399,                   /* TBNOT  */
    TBOR = 400,                    /* TBOR  */
    TBXOR = 401,                   /* TBXOR  */
    TCOLON = 402,                  /* TCOLON  */
    TCOMMA = 403,                  /* TCOMMA  */
    TDIVIDE = 404,                 /* TDIVIDE  */
    TDOT = 405,                    /* TDOT  */
    TDOTDOT = 406,                 /* TDOTDOT  */
    TDOTDOTDOT = 407,              /* TDOTDOTDOT  */
    TEQUAL = 408,                  /* TEQUAL  */
    TEXP = 409,                    /* TEXP  */
    TGREATER = 410,                /* TGREATER  */
    TGREATEREQUAL = 411,           /* TGREATEREQUAL  */
    THASH = 412,                   /* THASH  */
    TLESS = 413,                   /* TLESS  */
    TLESSEQUAL = 414,              /* TLESSEQUAL  */
    TMINUS = 415,                  /* TMINUS  */
    TMOD = 416,                    /* TMOD  */
    TNOTEQUAL = 417,               /* TNOTEQUAL  */
    TOR = 418,                     /* TOR  */
    TPLUS = 419,                   /* TPLUS  */
    TQUESTION = 420,               /* TQUESTION  */
    TSEMI = 421,                   /* TSEMI  */
    TSHIFTLEFT = 422,              /* TSHIFTLEFT  */
    TSHIFTRIGHT = 423,             /* TSHIFTRIGHT  */
    TSTAR = 424,                   /* TSTAR  */
    TSWAP = 425,                   /* TSWAP  */
    TLCBR = 426,                   /* TLCBR  */
    TRCBR = 427,                   /* TRCBR  */
    TLP = 428,                     /* TLP  */
    TRP = 429,                     /* TRP  */
    TLSBR = 430,                   /* TLSBR  */
    TRSBR = 431,                   /* TRSBR  */
    TNOELSE = 432,                 /* TNOELSE  */
    TDOTDOTOPENHIGH = 433,         /* TDOTDOTOPENHIGH  */
    TUPLUS = 434,                  /* TUPLUS  */
    TUMINUS = 435,                 /* TUMINUS  */
    TLNOT = 436                    /* TLNOT  */
  };
  typedef enum yychpl_tokentype yychpl_token_kind_t;
#endif

/* Value type.  */

/* Location type.  */
#if ! defined YYCHPL_LTYPE && ! defined YYCHPL_LTYPE_IS_DECLARED
typedef struct YYCHPL_LTYPE YYCHPL_LTYPE;
struct YYCHPL_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYCHPL_LTYPE_IS_DECLARED 1
# define YYCHPL_LTYPE_IS_TRIVIAL 1
#endif




#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct yychpl_pstate yychpl_pstate;


int yychpl_push_parse (yychpl_pstate *ps,
                  int pushed_char, YYCHPL_STYPE const *pushed_val, YYCHPL_LTYPE *pushed_loc, ParserContext* context);

yychpl_pstate *yychpl_pstate_new (void);
void yychpl_pstate_delete (yychpl_pstate *ps);

/* "%code provides" blocks.  */
#line 311 "chpl.ypp"

  extern int yychpl_debug;

  void yychpl_error(YYLTYPE*       loc,
                    ParserContext* context,
                    const char*    errorMessage);
#line 319 "chpl.ypp"

  // include ParserContext.h here because it depends
  // upon YYLTYPE and other types defined by the generated parser
  // headers.
  #include "ParserContext.h"
  // include override of macro used to compute locations
  #include "parser-yylloc-default.h"

#line 510 "bison-chpl-lib.h"

#endif /* !YY_YYCHPL_BISON_CHPL_LIB_H_INCLUDED  */
