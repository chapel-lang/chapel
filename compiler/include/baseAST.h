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
  EXPR_BOOLLITERAL,
  EXPR_INTLITERAL,
  EXPR_FLOATLITERAL,
  EXPR_COMPLEXLITERAL,
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
  EXPR_SIZEOF,
  EXPR_PARENOP,
  EXPR_CAST,
  EXPR_FNCALL,
  EXPR_IOCALL,
  EXPR_ARRAYREF,
  EXPR_MEMBERACCESS,
  EXPR_REDUCE,
  EXPR_TUPLE,

  SYMBOL,
  SYMBOL_UNRESOLVED,
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
  TYPE_INDEX,
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

  static long getNumIDs(void);

  BaseAST(void);
  BaseAST(astType_t type);
};
#define forv_BaseAST(_p, _v) forv_Vec(BaseAST, _p, _v)

#endif

