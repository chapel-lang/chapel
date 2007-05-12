#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"
#include "view.h"

//
// Removes local variables that are only targets for moves, but are
// never used anywhere.
//
static bool isDeadVariable(DefExpr* def) {
  if (def->sym->uses.n > 0)
    return false;
  if (def->sym->defs.n <= 1)
    return true;
  forv_Vec(SymExpr, se, def->sym->defs) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(se->parentExpr))
      if (call->isPrimitive(PRIMITIVE_MOVE))
        continue;
    return false;
  }
  return true; // only target of moves
}

void deadVariableElimination(FnSymbol* fn) {
  bool change = true;
  while (change) {
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn);
    compute_sym_uses(fn);
    change = false;
    forv_Vec(BaseAST, ast, asts) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
        if (def->sym->astType != SYMBOL_VAR || def->parentSymbol != fn)
          continue; // only variables, no fields of nested types
        if (isDeadVariable(def)) {
          forv_Vec(SymExpr, se, def->sym->defs) {
            CallExpr* call = dynamic_cast<CallExpr*>(se->parentExpr);
            Expr* rhs = call->get(2);
            if (rhs->astType == EXPR_SYM)
              call->remove();
            else
              call->replace(rhs->remove());
          }
          def->remove();
          change = true;
        }
      }
    }
  }
}

//
// Removes expression statements that have no effect.
//
void deadExpressionElimination(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* expr = dynamic_cast<SymExpr*>(ast)) {
      if (expr->parentExpr && expr == expr->getStmtExpr())
        expr->remove();
    } else if (CallExpr* expr = dynamic_cast<CallExpr*>(ast)) {
      if (expr->isPrimitive(PRIMITIVE_CAST) ||
          expr->isPrimitive(PRIMITIVE_GET_MEMBER))
        if (expr->parentExpr && expr == expr->getStmtExpr())
          expr->remove();
      if (expr->isPrimitive(PRIMITIVE_MOVE))
        if (SymExpr* lhs = dynamic_cast<SymExpr*>(expr->get(1)))
          if (SymExpr* rhs = dynamic_cast<SymExpr*>(expr->get(2)))
            if (lhs->var == rhs->var)
              expr->remove();
    }
  }
}

void deadCodeElimination(FnSymbol* fn) {
  Map<SymExpr*,Vec<SymExpr*>*> DU;
  Map<SymExpr*,Vec<SymExpr*>*> UD;
  buildDefUseChains(fn, DU, UD);
  freeDefUseChains(DU, UD);
}

void deadCodeElimination() {
  forv_Vec(FnSymbol, fn, gFns) {
    deadCodeElimination(fn);
  }
}
