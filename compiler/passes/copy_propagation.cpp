#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

//
// Apply local copy propagation to basic blocks of function
//
void localCopyPropagation(FnSymbol* fn) {
  buildBasicBlocks(fn);
  compute_sym_uses(fn);

  //
  // locals: a vector of local variables in function fn that are
  // candidates for copy propagation
  //
  Vec<Symbol*> locals;
  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    forv_Vec(Expr, expr, bb->exprs) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(expr))
        if (VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym))
          if (var != fn->getReturnSymbol() &&
              !is_complex_type(var->type) &&
              !isRecordType(var->type) &&
              !var->isReference &&
              !var->isConcurrent &&
              !var->on_heap &&
              !var->is_ref)
            locals.add(def->sym);
    }
  }

  //
  // useSet: a set of SymExprs that are uses of local variables
  // defSet: a set of SymExprs that are defs of local variables
  //
  Vec<SymExpr*> useSet;
  Vec<SymExpr*> defSet;
  forv_Vec(Symbol, local, locals) {
    forv_Vec(SymExpr, se, local->defs) {
      defSet.set_add(se);
    } 
    forv_Vec(SymExpr, se, local->uses) {
      useSet.set_add(se);
    }
  }

  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    ChainHashMap<Symbol*,PointerHashFns,Symbol*> available;

    forv_Vec(Expr, expr, bb->exprs) {

      Vec<BaseAST*> asts;
      collect_asts(&asts, expr);

      //
      // replace uses with available copies
      //
      forv_Vec(BaseAST, ast, asts) {
        if (SymExpr* se = dynamic_cast<SymExpr*>(ast)) {
          if (useSet.set_in(se) && !defSet.set_in(se)) {
            if (available.get(se->var)) {
              se->var = available.get(se->var);
            }
          }
        }
      }

      //
      // invalidate available copies based on defs
      //
      forv_Vec(BaseAST, ast, asts) {
        if (SymExpr* se = dynamic_cast<SymExpr*>(ast)) {
          if (defSet.set_in(se)) {
            Vec<Symbol*> keys;
            available.get_keys(keys);
            forv_Vec(Symbol, key, keys) {
              if (key == se->var || available.get(key) == se->var)
                available.del(key);
            }
          }
        }
      }

      //
      // insert pairs into available copies map
      //
      if (CallExpr* call = dynamic_cast<CallExpr*>(expr))
        if (call->isPrimitive(PRIMITIVE_MOVE))
          if (SymExpr* lhs = dynamic_cast<SymExpr*>(call->get(1)))
            if (SymExpr* rhs = dynamic_cast<SymExpr*>(call->get(2)))
              if (lhs->var != rhs->var &&
                  defSet.set_in(lhs) &&
                  (useSet.set_in(rhs) || rhs->var->isConst() || rhs->var->isImmediate()))
                available.put(lhs->var, rhs->var);
    }
  }
}

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

void copyPropagation(void) {
  if (unoptimized)
    return;
  Vec<FnSymbol*> fns;
  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    collapseBlocks(fn);
    removeUnnecessaryGotos(fn);
    localCopyPropagation(fn);
    deadVariableElimination(fn);
    deadExpressionElimination(fn);
  }
}
