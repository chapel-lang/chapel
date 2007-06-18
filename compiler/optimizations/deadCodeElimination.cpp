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
  if (isReference(def->sym->type)) {
    return def->sym->uses.n == 0 && def->sym->defs.n <= 1;
  } else {
    return def->sym->uses.n == 0;
  }
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

  Map<Expr*,Expr*> exprMap;
  Vec<Expr*> liveCode;
  Vec<Expr*> workSet;
  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    forv_Vec(Expr, expr, bb->exprs) {
      bool essential = false;
      Vec<BaseAST*> asts;
      collect_asts(&asts, expr);
      forv_Vec(BaseAST, ast, asts) {
        if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
          // mark function calls and essential primitives as essential
          if (call->isResolved() ||
              (call->primitive && call->primitive->isEssential))
            essential = true;
          // mark assignments to global variables as essential
          if (call->isPrimitive(PRIMITIVE_MOVE))
            if (SymExpr* se = dynamic_cast<SymExpr*>(call->get(1)))
              if (!DU.get(se) || // DU chain only contains locals
                  !se->var->type->refType) // reference issue
                essential = true;
        }
        if (Expr* sub = dynamic_cast<Expr*>(ast)) {
          exprMap.put(sub, expr);
          if (BlockStmt* block = dynamic_cast<BlockStmt*>(sub->parentExpr))
            if (block->loopInfo == sub)
              essential = true;
          if (CondStmt* cond = dynamic_cast<CondStmt*>(sub->parentExpr))
            if (cond->condExpr == sub)
              essential = true;
        }
      }
      if (essential) {
        liveCode.set_add(expr);
        workSet.add(expr);
      }
    }
  }

  forv_Vec(Expr, expr, workSet) {
    Vec<BaseAST*> asts;
    collect_asts(&asts, expr);
    forv_Vec(BaseAST, ast, asts) {
      if (SymExpr* se = dynamic_cast<SymExpr*>(ast)) {
        if (Vec<SymExpr*>* defs = UD.get(se)) {
          forv_Vec(SymExpr, def, *defs) {
            Expr* expr = exprMap.get(def);
            if (!liveCode.set_in(expr)) {
              liveCode.set_add(expr);
              workSet.add(expr);
            }
          }
        }
      }
    }
  }

  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    forv_Vec(Expr, expr, bb->exprs) {
      if (expr->astType == EXPR_SYM || expr->astType == EXPR_CALL)
        if (!liveCode.set_in(expr))
          expr->remove();
    }
  }

  freeDefUseChains(DU, UD);
}

void deadCodeElimination() {
  if (fBaseline)
    return;
  forv_Vec(FnSymbol, fn, gFns) {
    if (!fNoFlowAnalysis)
      deadCodeElimination(fn);
    deadVariableElimination(fn);
  }
}
