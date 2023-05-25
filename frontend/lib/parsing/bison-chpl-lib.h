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
    AttributeGroup* attributeGroup;
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
    AttributeGroup* attributeGroup;
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
    AttributeGroup* attributeGroup;
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
    AttributeGroup* attributeGroup;
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
    TDMAPPED = 286,                /* TDMAPPED  */
    TDO = 287,                     /* TDO  */
    TDOMAIN = 288,                 /* TDOMAIN  */
    TELSE = 289,                   /* TELSE  */
    TENUM = 290,                   /* TENUM  */
    TEXCEPT = 291,                 /* TEXCEPT  */
    TEXPORT = 292,                 /* TEXPORT  */
    TEXTERN = 293,                 /* TEXTERN  */
    TFALSE = 294,                  /* TFALSE  */
    TFOR = 295,                    /* TFOR  */
    TFORALL = 296,                 /* TFORALL  */
    TFOREACH = 297,                /* TFOREACH  */
    TFORWARDING = 298,             /* TFORWARDING  */
    TIF = 299,                     /* TIF  */
    TIMAG = 300,                   /* TIMAG  */
    TIMPORT = 301,                 /* TIMPORT  */
    TIN = 302,                     /* TIN  */
    TINCLUDE = 303,                /* TINCLUDE  */
    TINDEX = 304,                  /* TINDEX  */
    TINLINE = 305,                 /* TINLINE  */
    TINOUT = 306,                  /* TINOUT  */
    TINT = 307,                    /* TINT  */
    TITER = 308,                   /* TITER  */
    TINITEQUALS = 309,             /* TINITEQUALS  */
    TIMPLEMENTS = 310,             /* TIMPLEMENTS  */
    TINTERFACE = 311,              /* TINTERFACE  */
    TLABEL = 312,                  /* TLABEL  */
    TLAMBDA = 313,                 /* TLAMBDA  */
    TLET = 314,                    /* TLET  */
    TLIFETIME = 315,               /* TLIFETIME  */
    TLOCAL = 316,                  /* TLOCAL  */
    TLOCALE = 317,                 /* TLOCALE  */
    TMANAGE = 318,                 /* TMANAGE  */
    TMINUSMINUS = 319,             /* TMINUSMINUS  */
    TMODULE = 320,                 /* TMODULE  */
    TNEW = 321,                    /* TNEW  */
    TNIL = 322,                    /* TNIL  */
    TNOINIT = 323,                 /* TNOINIT  */
    TNONE = 324,                   /* TNONE  */
    TNOTHING = 325,                /* TNOTHING  */
    TON = 326,                     /* TON  */
    TONLY = 327,                   /* TONLY  */
    TOPERATOR = 328,               /* TOPERATOR  */
    TOTHERWISE = 329,              /* TOTHERWISE  */
    TOUT = 330,                    /* TOUT  */
    TOVERRIDE = 331,               /* TOVERRIDE  */
    TOWNED = 332,                  /* TOWNED  */
    TPARAM = 333,                  /* TPARAM  */
    TPLUSPLUS = 334,               /* TPLUSPLUS  */
    TPRAGMA = 335,                 /* TPRAGMA  */
    TPRIMITIVE = 336,              /* TPRIMITIVE  */
    TPRIVATE = 337,                /* TPRIVATE  */
    TPROC = 338,                   /* TPROC  */
    TPROTOTYPE = 339,              /* TPROTOTYPE  */
    TPUBLIC = 340,                 /* TPUBLIC  */
    TPROCLP = 341,                 /* TPROCLP  */
    TREAL = 342,                   /* TREAL  */
    TRECORD = 343,                 /* TRECORD  */
    TREDUCE = 344,                 /* TREDUCE  */
    TREF = 345,                    /* TREF  */
    TREQUIRE = 346,                /* TREQUIRE  */
    TRETURN = 347,                 /* TRETURN  */
    TSCAN = 348,                   /* TSCAN  */
    TSELECT = 349,                 /* TSELECT  */
    TSERIAL = 350,                 /* TSERIAL  */
    TSHARED = 351,                 /* TSHARED  */
    TSINGLE = 352,                 /* TSINGLE  */
    TSPARSE = 353,                 /* TSPARSE  */
    TSTRING = 354,                 /* TSTRING  */
    TSUBDOMAIN = 355,              /* TSUBDOMAIN  */
    TSYNC = 356,                   /* TSYNC  */
    TTHEN = 357,                   /* TTHEN  */
    TTHIS = 358,                   /* TTHIS  */
    TTHROW = 359,                  /* TTHROW  */
    TTHROWS = 360,                 /* TTHROWS  */
    TTRUE = 361,                   /* TTRUE  */
    TTRY = 362,                    /* TTRY  */
    TTRYBANG = 363,                /* TTRYBANG  */
    TTYPE = 364,                   /* TTYPE  */
    TUINT = 365,                   /* TUINT  */
    TUNDERSCORE = 366,             /* TUNDERSCORE  */
    TUNION = 367,                  /* TUNION  */
    TUNMANAGED = 368,              /* TUNMANAGED  */
    TUSE = 369,                    /* TUSE  */
    TVAR = 370,                    /* TVAR  */
    TVOID = 371,                   /* TVOID  */
    TWHEN = 372,                   /* TWHEN  */
    TWHERE = 373,                  /* TWHERE  */
    TWHILE = 374,                  /* TWHILE  */
    TWITH = 375,                   /* TWITH  */
    TYIELD = 376,                  /* TYIELD  */
    TZIP = 377,                    /* TZIP  */
    TALIAS = 378,                  /* TALIAS  */
    TAND = 379,                    /* TAND  */
    TASSIGN = 380,                 /* TASSIGN  */
    TASSIGNBAND = 381,             /* TASSIGNBAND  */
    TASSIGNBOR = 382,              /* TASSIGNBOR  */
    TASSIGNBXOR = 383,             /* TASSIGNBXOR  */
    TASSIGNDIVIDE = 384,           /* TASSIGNDIVIDE  */
    TASSIGNEXP = 385,              /* TASSIGNEXP  */
    TASSIGNLAND = 386,             /* TASSIGNLAND  */
    TASSIGNLOR = 387,              /* TASSIGNLOR  */
    TASSIGNMINUS = 388,            /* TASSIGNMINUS  */
    TASSIGNMOD = 389,              /* TASSIGNMOD  */
    TASSIGNMULTIPLY = 390,         /* TASSIGNMULTIPLY  */
    TASSIGNPLUS = 391,             /* TASSIGNPLUS  */
    TASSIGNREDUCE = 392,           /* TASSIGNREDUCE  */
    TASSIGNSL = 393,               /* TASSIGNSL  */
    TASSIGNSR = 394,               /* TASSIGNSR  */
    TATMARK = 395,                 /* TATMARK  */
    TBANG = 396,                   /* TBANG  */
    TBAND = 397,                   /* TBAND  */
    TBNOT = 398,                   /* TBNOT  */
    TBOR = 399,                    /* TBOR  */
    TBXOR = 400,                   /* TBXOR  */
    TCOLON = 401,                  /* TCOLON  */
    TCOMMA = 402,                  /* TCOMMA  */
    TDIVIDE = 403,                 /* TDIVIDE  */
    TDOT = 404,                    /* TDOT  */
    TDOTDOT = 405,                 /* TDOTDOT  */
    TDOTDOTDOT = 406,              /* TDOTDOTDOT  */
    TEQUAL = 407,                  /* TEQUAL  */
    TEXP = 408,                    /* TEXP  */
    TGREATER = 409,                /* TGREATER  */
    TGREATEREQUAL = 410,           /* TGREATEREQUAL  */
    THASH = 411,                   /* THASH  */
    TLESS = 412,                   /* TLESS  */
    TLESSEQUAL = 413,              /* TLESSEQUAL  */
    TMINUS = 414,                  /* TMINUS  */
    TMOD = 415,                    /* TMOD  */
    TNOTEQUAL = 416,               /* TNOTEQUAL  */
    TOR = 417,                     /* TOR  */
    TPLUS = 418,                   /* TPLUS  */
    TQUESTION = 419,               /* TQUESTION  */
    TSEMI = 420,                   /* TSEMI  */
    TSHIFTLEFT = 421,              /* TSHIFTLEFT  */
    TSHIFTRIGHT = 422,             /* TSHIFTRIGHT  */
    TSTAR = 423,                   /* TSTAR  */
    TSWAP = 424,                   /* TSWAP  */
    TLCBR = 425,                   /* TLCBR  */
    TRCBR = 426,                   /* TRCBR  */
    TLP = 427,                     /* TLP  */
    TRP = 428,                     /* TRP  */
    TLSBR = 429,                   /* TLSBR  */
    TRSBR = 430,                   /* TRSBR  */
    TNOELSE = 431,                 /* TNOELSE  */
    TDOTDOTOPENHIGH = 432,         /* TDOTDOTOPENHIGH  */
    TUPLUS = 433,                  /* TUPLUS  */
    TUMINUS = 434,                 /* TUMINUS  */
    TLNOT = 435                    /* TLNOT  */
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

#line 509 "bison-chpl-lib.h"

#endif /* !YY_YYCHPL_BISON_CHPL_LIB_H_INCLUDED  */
