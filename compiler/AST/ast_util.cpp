#include "ast_util.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"

void TRAVERSE(ILink* link, Traversal* traversal, bool atTop) {
  if (Stmt* stmt = dynamic_cast<Stmt*>(link)) {
    stmt->traverse(stmt, traversal, atTop);
  }
  else if (Symbol* symbol = dynamic_cast<Symbol*>(link)) {
    symbol->traverse(symbol, traversal, atTop);
  }
  else if (Expr* expr = dynamic_cast<Expr*>(link)) {
    expr->traverse(expr, traversal, atTop);
  }
  else if (Type* type = dynamic_cast<Type*>(link)) {
    type->traverse(type, traversal, atTop);
  }
}

void TRAVERSE_LS(ILink* link, Traversal* traversal, bool atTop) {
  if (Stmt* stmt = dynamic_cast<Stmt*>(link)) {
    stmt->traverseList(stmt, traversal, atTop);
  }
  else if (Symbol* symbol = dynamic_cast<Symbol*>(link)) {
    symbol->traverseList(symbol, traversal, atTop);
  }
  else if (Expr* expr = dynamic_cast<Expr*>(link)) {
    expr->traverseList(expr, traversal, atTop);
  }
  else if (Type* type = dynamic_cast<Type*>(link)) {
    type->traverseList(type, traversal, atTop);
  }
}
