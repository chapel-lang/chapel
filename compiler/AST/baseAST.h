#ifndef _BASEAST_H_
#define _BASEAST_H_

#include "link.h"

enum astType_t {
  STMT,
  STMT_NOOP,
  STMT_VARDEF,
  STMT_TYPEDEF,
  STMT_FNDEF,
  STMT_EXPR,
  STMT_RETURN,
  STMT_BLOCK,
  STMT_WHILELOOP,
  STMT_FORLOOP,
  STMT_COND,

  EXPR,
  EXPR_LITERAL,
  EXPR_INTLITERAL,
  EXPR_FLOATLITERAL,
  EXPR_STRINGLITERAL,
  EXPR_VARIABLE,
  EXPR_UNOP,
  EXPR_BINOP,
  EXPR_SPECIALBINOP,
  EXPR_ASSIGNOP,
  EXPR_SIMPLESEQ,
  EXPR_FLOOD,
  EXPR_COMPLETEDIM,
  EXPR_DOMAIN,
  EXPR_PARENOP,
  EXPR_CAST,
  EXPR_FNCALL,
  EXPR_WRITECALL,
  EXPR_ARRAYREF,
  EXPR_REDUCE,
  EXPR_TUPLE,

  SYMBOL,
  SYMBOL_USEBEFOREDEF,
  SYMBOL_VAR,
  SYMBOL_PARAM,
  SYMBOL_TYPE,
  SYMBOL_CLASS,
  SYMBOL_REDUCE,
  SYMBOL_FN,
  SYMBOL_ENUM,

  TYPE,
  TYPE_BUILTIN,
  TYPE_ENUM,
  TYPE_DOMAIN,
  TYPE_SUBDOMAIN,
  TYPE_INDEX,
  TYPE_SUBINDEX,
  TYPE_ARRAY,
  TYPE_USER,
  TYPE_CLASS,
  TYPE_TUPLE,
  
  AST_TYPE_END 
};

#define isSomeStmt(_x) (((_x) > astType_t::STMT) && (_x) < astType_t::EXPR)
#define isSomeExpr(_x) (((_x) > astType_t::EXPR) && (_x) < astType_t::SYMBOL)
#define isSomeSymbol(_x) (((_x) > astType_t::SYMBOL) && (_x) < astType_t::TYPE)
#define isSomeType(_x) (((_x) > astType_t::TYPE) && (_x) < astType_t::AST_TYPE_END)

class BaseAST : public ILink {
 public:
  astType_t astType;
  long id;

  int getBaseASTs(Vec<BaseAST *> &asts);
  int getSymbolTypes(Vec<BaseAST *> &asts);
  int getStmtExprs(Vec<BaseAST *> &asts);
  virtual int getSymbols(Vec<BaseAST *> &asts);
  virtual int getTypes(Vec<BaseAST *> &asts);
  virtual int getStmts(Vec<BaseAST *> &asts);
  virtual int getExprs(Vec<BaseAST *> &asts);

  BaseAST(void);
  BaseAST(astType_t type);
};
#define forv_BaseAST(_p, _v) forv_Vec(BaseAST, _p, _v)

#endif

