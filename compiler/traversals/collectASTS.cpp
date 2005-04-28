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
  Vec<BaseAST *> syms;
  collect_symbols((Vec<Symbol*> *)&syms, function);
  forv_BaseAST(s, syms)
    if (s->astType == SYMBOL_TYPE)
      asts->add(((TypeSymbol*)s)->type);
  asts->append(syms);
}
