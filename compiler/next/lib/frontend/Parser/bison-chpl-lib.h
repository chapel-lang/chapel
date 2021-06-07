/* A Bison parser, made by GNU Bison 3.7.5.  */

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
#line 52 "chpl.ypp"

  #include "parser-dependencies.h"
#line 57 "chpl.ypp"

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
#line 83 "chpl.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_1_
  #define _BISON_CHAPEL_DEFINES_1_

  enum AggregateTag {
    AggregateTag_CLASS,
    AggregateTag_RECORD,
    AggregateTag_UNION,
  };

  enum ThrowsTag {
    ThrowsTag_DEFAULT,
    ThrowsTag_THROWS,
  };

  enum LinkageTag { // TODO: what about combinations of these?
    LinkageTag_DEFAULT,
    LinkageTag_INLINE,
    LinkageTag_OVERRIDE,
    LinkageTag_EXTERN,
    LinkageTag_EXPORT,
  };

  using ParserExprList = std::vector<Expression*>;
  using UniqueStrList = std::vector<PODUniqueString>;

  struct PotentialRename {
    enum {SINGLE, DOUBLE} tag;

    Expression* elem;
    std::pair<Expression*, Expression*> renamed;

    PotentialRename(Expression* elem) {
      this->tag = SINGLE;
      this->elem = elem;
      this->renamed = std::make_pair((Expression*)nullptr, (Expression*)nullptr);
    }
    PotentialRename(owned<Expression> elem) : PotentialRename(elem.release()) { }
    PotentialRename(Expression* from, Expression* to)
    {
      this->tag = DOUBLE;
      this->elem = nullptr;
      this->renamed = std::make_pair(from, to);
    }
    PotentialRename(owned<Expression> from, owned<Expression> to)
    : PotentialRename(from.release(), to.release()) { }
    PotentialRename(Expression* from, owned<Expression> to)
    : PotentialRename(from, to.release()) { }

  };

  using RenameList = std::vector<PotentialRename>;

  // these structures do not use constructors to avoid
  // errors about being in the union below. Additionally
  // they only have pointer or numeric fields.

  struct WhereAndLifetime {
    Expression* where;
    ParserExprList* lifetime;
  };
  static inline
  WhereAndLifetime makeWhereAndLifetime(Expression* w, ParserExprList* lt) {
    WhereAndLifetime ret = {w, lt};
    return ret;
  }

  // This is used to propagate any comments that hug the top of a statement
  // upwards through parser rules.
  struct CommentsAndStmt {
    std::vector<ParserComment>* comments;
    Expression* stmt;
  };
  static inline
  CommentsAndStmt makeCommentsAndStmt(std::vector<ParserComment>* comments,
                                      Expression* stmt) {
    CommentsAndStmt ret = {comments, stmt};
    return ret;
  }
  static inline
  CommentsAndStmt makeCommentsAndStmt(std::vector<ParserComment>* comments,
                                      owned<Expression> stmt) {
    return makeCommentsAndStmt(comments, stmt.release());
  }

  // for extern_export_decl_stmt_start
  struct CommentsAndLinkage {
    std::vector<ParserComment>* comments;
    LinkageTag linkage;
  };

  // A struct for storing a partially constructed function prototype
  // during parsing for linkage_spec/fn_decl_stmt/etc
  // It is just saving some components of a function to be used
  // when building the FunctionDecl.
  struct FunctionParts {
    std::vector<ParserComment>* comments;
    ErroneousExpression* errorExpr; // only used for parser error
    Decl::Visibility visibility;
    Function::Linkage linkage;
    Expression* linkageNameExpr;
    bool isInline;
    bool isOverride;
    Function::Kind kind;
    Formal* receiver;
    PODUniqueString name;
    Function::ReturnIntent returnIntent;
    bool throws;
    ParserExprList* formals;
    Expression* returnType;
    Expression* where;
    ParserExprList* lifetime;
    ParserExprList* body;
  };

  // a struct to store the values in module_decl_start
  struct ModuleParts {
    std::vector<ParserComment>* comments;
    Decl::Visibility visibility;
    Module::Kind kind;
    PODUniqueString name;
  };

  // a struct to store the values in enum_header_lcbr
  struct EnumParts {
    std::vector<ParserComment>* comments;
    Decl::Visibility visibility;
    PODUniqueString name;
  };

  // This is produced by do_stmt. It records whether the do_stmt statements
  // were produced following a 'do' or not. E.g...
  // do { ... } vs { ... }
  struct BlockOrDo {
    ParserExprList* exprList;
    bool usesDo;
  };

  struct SizedStr {
    const char* allocatedData; // *not* a uniqued string!
    long size;
  };

  struct MaybeNamedActual {
    Expression* expr;
    PODUniqueString name;
  };
  static inline
  MaybeNamedActual makeMaybeNamedActual(Expression* expr,
                                        PODUniqueString name) {
    MaybeNamedActual ret = {expr, name};
    return ret;
  }
  static inline
  MaybeNamedActual makeMaybeNamedActual(Expression* expr) {
    PODUniqueString emptyName = PODUniqueString::build();
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
    Expression* expr;

    // The remaining types are used only in parser productions

    // integer/enum values

    AggregateTag aggregateTag;
    Formal::Intent intentTag;
    Function::Kind functionKind;
    Function::ReturnIntent returnTag;
    LinkageTag linkageTag;
    Module::Kind moduleKind;
    New::Management newManagement;
    TaskVar::Intent taskIntent;
    Decl::Visibility visibilityTag;
    ThrowsTag throwsTag;
    Variable::Kind variableKind;

    // simple pointer values
    Block* block;
    Call* call;
    Function* function;
    Module* module;
    WithClause* withClause;

    // values that are temporary groupings
    BlockOrDo blockOrDo;
    CommentsAndLinkage commentsAndLinkage;
    CommentsAndStmt commentsAndStmt;
    EnumParts enumParts;
    FunctionParts functionParts;
    MaybeNamedActual maybeNamedActual;
    MaybeNamedActualList* maybeNamedActualList;
    ModuleParts moduleParts;
    ParserExprList* exprList;
    RenameList* renameList;
    UniqueStrList* uniqueStrList;
    WhereAndLifetime lifetimeAndWhere;

    // TODO
    //Type*                     ptype;
    //EnumType*                 penumtype;
    //std::vector<DefExpr*>*    pvecOfDefs;
    //ImportStmt*               pimportstmt;
    //FlagSet*                  flagSet;
    //ShadowVarSymbol*          pShadowVar;
    //ShadowVarPrefix           pShadowVarPref;
    //std::vector<PotentialRename*>* ponlylist;
    //std::set<Flag>*           pflagset;
  };

  #define YYSTYPE YYCHPL_STYPE
  #define YYLTYPE YYCHPL_LTYPE

  #endif
#line 324 "chpl.ypp"

  // forward declare ParserContext
  struct ParserContext;

#line 307 "bison-chpl-lib.h"

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
    TREAL = 341,                   /* TREAL  */
    TRECORD = 342,                 /* TRECORD  */
    TREDUCE = 343,                 /* TREDUCE  */
    TREF = 344,                    /* TREF  */
    TREQUIRE = 345,                /* TREQUIRE  */
    TRETURN = 346,                 /* TRETURN  */
    TSCAN = 347,                   /* TSCAN  */
    TSELECT = 348,                 /* TSELECT  */
    TSERIAL = 349,                 /* TSERIAL  */
    TSHARED = 350,                 /* TSHARED  */
    TSINGLE = 351,                 /* TSINGLE  */
    TSPARSE = 352,                 /* TSPARSE  */
    TSTRING = 353,                 /* TSTRING  */
    TSUBDOMAIN = 354,              /* TSUBDOMAIN  */
    TSYNC = 355,                   /* TSYNC  */
    TTHEN = 356,                   /* TTHEN  */
    TTHIS = 357,                   /* TTHIS  */
    TTHROW = 358,                  /* TTHROW  */
    TTHROWS = 359,                 /* TTHROWS  */
    TTRUE = 360,                   /* TTRUE  */
    TTRY = 361,                    /* TTRY  */
    TTRYBANG = 362,                /* TTRYBANG  */
    TTYPE = 363,                   /* TTYPE  */
    TUINT = 364,                   /* TUINT  */
    TUNDERSCORE = 365,             /* TUNDERSCORE  */
    TUNION = 366,                  /* TUNION  */
    TUNMANAGED = 367,              /* TUNMANAGED  */
    TUSE = 368,                    /* TUSE  */
    TVAR = 369,                    /* TVAR  */
    TVOID = 370,                   /* TVOID  */
    TWHEN = 371,                   /* TWHEN  */
    TWHERE = 372,                  /* TWHERE  */
    TWHILE = 373,                  /* TWHILE  */
    TWITH = 374,                   /* TWITH  */
    TYIELD = 375,                  /* TYIELD  */
    TZIP = 376,                    /* TZIP  */
    TALIAS = 377,                  /* TALIAS  */
    TAND = 378,                    /* TAND  */
    TASSIGN = 379,                 /* TASSIGN  */
    TASSIGNBAND = 380,             /* TASSIGNBAND  */
    TASSIGNBOR = 381,              /* TASSIGNBOR  */
    TASSIGNBXOR = 382,             /* TASSIGNBXOR  */
    TASSIGNDIVIDE = 383,           /* TASSIGNDIVIDE  */
    TASSIGNEXP = 384,              /* TASSIGNEXP  */
    TASSIGNLAND = 385,             /* TASSIGNLAND  */
    TASSIGNLOR = 386,              /* TASSIGNLOR  */
    TASSIGNMINUS = 387,            /* TASSIGNMINUS  */
    TASSIGNMOD = 388,              /* TASSIGNMOD  */
    TASSIGNMULTIPLY = 389,         /* TASSIGNMULTIPLY  */
    TASSIGNPLUS = 390,             /* TASSIGNPLUS  */
    TASSIGNREDUCE = 391,           /* TASSIGNREDUCE  */
    TASSIGNSL = 392,               /* TASSIGNSL  */
    TASSIGNSR = 393,               /* TASSIGNSR  */
    TBANG = 394,                   /* TBANG  */
    TBAND = 395,                   /* TBAND  */
    TBNOT = 396,                   /* TBNOT  */
    TBOR = 397,                    /* TBOR  */
    TBXOR = 398,                   /* TBXOR  */
    TCOLON = 399,                  /* TCOLON  */
    TCOMMA = 400,                  /* TCOMMA  */
    TDIVIDE = 401,                 /* TDIVIDE  */
    TDOT = 402,                    /* TDOT  */
    TDOTDOT = 403,                 /* TDOTDOT  */
    TDOTDOTDOT = 404,              /* TDOTDOTDOT  */
    TEQUAL = 405,                  /* TEQUAL  */
    TEXP = 406,                    /* TEXP  */
    TGREATER = 407,                /* TGREATER  */
    TGREATEREQUAL = 408,           /* TGREATEREQUAL  */
    THASH = 409,                   /* THASH  */
    TIO = 410,                     /* TIO  */
    TLESS = 411,                   /* TLESS  */
    TLESSEQUAL = 412,              /* TLESSEQUAL  */
    TMINUS = 413,                  /* TMINUS  */
    TMOD = 414,                    /* TMOD  */
    TNOTEQUAL = 415,               /* TNOTEQUAL  */
    TOR = 416,                     /* TOR  */
    TPLUS = 417,                   /* TPLUS  */
    TQUESTION = 418,               /* TQUESTION  */
    TSEMI = 419,                   /* TSEMI  */
    TSHIFTLEFT = 420,              /* TSHIFTLEFT  */
    TSHIFTRIGHT = 421,             /* TSHIFTRIGHT  */
    TSTAR = 422,                   /* TSTAR  */
    TSWAP = 423,                   /* TSWAP  */
    TLCBR = 424,                   /* TLCBR  */
    TRCBR = 425,                   /* TRCBR  */
    TLP = 426,                     /* TLP  */
    TRP = 427,                     /* TRP  */
    TLSBR = 428,                   /* TLSBR  */
    TRSBR = 429,                   /* TRSBR  */
    TNOELSE = 430,                 /* TNOELSE  */
    TDOTDOTOPENHIGH = 431,         /* TDOTDOTOPENHIGH  */
    TUPLUS = 432,                  /* TUPLUS  */
    TUMINUS = 433,                 /* TUMINUS  */
    TLNOT = 434                    /* TLNOT  */
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
#line 332 "chpl.ypp"

  extern int yychpl_debug;

  void yychpl_error(YYLTYPE*       loc,
                    ParserContext* context,
                    const char*    errorMessage);
#line 340 "chpl.ypp"

  // include ParserContext.h here because it depends
  // upon YYLTYPE and other types defined by the generated parser
  // headers.
  #include "ParserContext.h"

#line 546 "bison-chpl-lib.h"

#endif /* !YY_YYCHPL_BISON_CHPL_LIB_H_INCLUDED  */
