#include "collectASTS.h"
#include "expr.h"
#include "stmt.h"

CollectASTs::CollectASTs(Vec<BaseAST*>* init_asts) {
  asts = init_asts;
  asts->clear();
}

void CollectASTs::preProcessExpr(Expr* expr) {
  asts->add(expr);
}

void CollectASTs::preProcessStmt(Stmt* stmt) {
  asts->add(stmt);
}

void collect_asts(Vec<BaseAST*>* asts, FnSymbol* function) {
  CollectASTs* traversal = new CollectASTs(asts);
  TRAVERSE(function->body, traversal, true);
}
