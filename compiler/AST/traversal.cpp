#include "traversal.h"

Traversal::Traversal(void) :
  exploreStmts(true),
  exploreExprs(true),
  exploreSymbols(true),
  exploreTypes(true)
{}


void Traversal::preProcessStmt(Stmt* stmt) {
}


void Traversal::postProcessStmt(Stmt* stmt) {
}


void Traversal::preProcessExpr(Expr* expr) {
}


void Traversal::postProcessExpr(Expr* expr) {
}


void Traversal::preProcessSymbol(Symbol* sym) {
}


void Traversal::postProcessSymbol(Symbol* sym) {
}


void Traversal::preProcessType(Type* type) {
}


void Traversal::postProcessType(Type* type) {
}
