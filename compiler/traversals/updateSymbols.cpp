#include "updateSymbols.h"
#include "expr.h"
#include "symbol.h"
#include "stringutil.h"

UpdateSymbols::UpdateSymbols(Map<BaseAST*,BaseAST*>* init_copy_map) {
  copy_map = init_copy_map;
  /** Prune for symbols? **/ /** Performance reasons --SJD */
  /** If so, prune a local copy! **/ /** Also make it a hash */
}


void UpdateSymbols::preProcessExpr(Expr* expr) {
  Variable* sym_expr = dynamic_cast<Variable*>(expr);

  if (!sym_expr) {
    return;
  }

  for (int i = 0; i < copy_map->n; i++) {
    if (copy_map->v[i].key == sym_expr->var) {
      if (Symbol* new_sym = dynamic_cast<Symbol*>(copy_map->v[i].value)) {
        sym_expr->var = new_sym;
      }
      else {
        INT_FATAL("Major error in UpdateSymbols");
      }
    }
  }
}
