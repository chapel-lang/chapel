#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

static long uid = 1;


void BaseAST::traverse(BaseAST* &_this, Traversal* traversal, bool atTop) {
  if (isNull()) return;

  if (Stmt* stmt = dynamic_cast<Stmt*>(_this)) {
    stmt->traverse(stmt, traversal, atTop);
  }
  else if (Symbol* symbol = dynamic_cast<Symbol*>(_this)) {
    symbol->traverse(symbol, traversal, atTop);
  }
  else if (Expr* expr = dynamic_cast<Expr*>(_this)) {
    expr->traverse(expr, traversal, atTop);
  }
  else if (Type* type = dynamic_cast<Type*>(_this)) {
    type->traverse(type, traversal, atTop);
  }
}

long BaseAST::getNumIDs(void) {
  return uid;
}

BaseAST::BaseAST(void) {
  INT_FATAL("Base AST must be given a type");
}


BaseAST::BaseAST(astType_t type) :
  astType(type),
  id(uid++)
{}
