#include "collectASTS.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"

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
  TRAVERSE(function, traversal, true);
  Vec<BaseAST *> syms;
  collect_symbols((Vec<Symbol*> *)&syms, function);
  forv_BaseAST(s, syms)
    if (s->astType == SYMBOL_TYPE)
      asts->add(((TypeSymbol*)s)->definition);
  asts->append(syms);
}

void collect_asts(Vec<BaseAST*>* asts) {
  CollectASTs* traversal = new CollectASTs(asts);
  traversal->run(Symboltable::getModules(MODULES_ALL));
  Vec<BaseAST *> syms;
  collect_symbols((Vec<Symbol*> *)&syms);
  forv_BaseAST(s, syms)
    if (s->astType == SYMBOL_TYPE)
      asts->add(((TypeSymbol*)s)->definition);
  asts->append(syms);
}
