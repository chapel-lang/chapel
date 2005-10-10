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
  if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
    asts->add(def->sym);
    if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(def->sym))
      if (ts->definition)
        asts->add(ts->definition);
  }
}

void CollectASTs::preProcessStmt(Stmt* stmt) {
  asts->add(stmt);
}

void collect_asts(Vec<BaseAST*>* asts, BaseAST* ast) {
  TRAVERSE(ast, new CollectASTs(asts), true);
  asts->add(ast);
}

void collect_asts(Vec<BaseAST*>* asts) {
  CollectASTs* traversal = new CollectASTs(asts);
  traversal->run(Symboltable::getModules(MODULES_ALL));
}
