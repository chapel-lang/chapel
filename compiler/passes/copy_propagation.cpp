#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

//
// Removes unnecessary nested and/or empty block statements.
//
void compressUnnecessaryScopes(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (BlockStmt* block = dynamic_cast<BlockStmt*>(ast)) {
      if (block->body->length() == 0) {
        block->remove();
        continue;
      }
      if (block->loopInfo ||
          block->blockTag == BLOCK_COBEGIN ||
          block->blockTag == BLOCK_BEGIN)
        continue;
      if (block->list) {
        for_alist(Expr, stmt, block->body)
          block->insertBefore(stmt->remove());
        block->remove();
        continue;
      }
    }
  }
}

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
                  (useSet.set_in(rhs) || rhs->var->isConst()))
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

//
// Removes gotos where the label immediately follows the goto
//
void removeUnnecessaryGotos(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (GotoStmt* gotoStmt = dynamic_cast<GotoStmt*>(ast))
      if (DefExpr* def = dynamic_cast<DefExpr*>(gotoStmt->next))
        if (def->sym == gotoStmt->label)
          gotoStmt->remove();
  }
}

//
// Remove unused labels
//
void removeUnusedLabels(FnSymbol* fn) {
  Vec<BaseAST*> labels;
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (GotoStmt* gotoStmt = dynamic_cast<GotoStmt*>(ast))
      labels.set_add(gotoStmt->label);
  }
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast))
      if (LabelSymbol* label = dynamic_cast<LabelSymbol*>(def->sym))
        if (!labels.set_in(label))
          def->remove();
  }
}

void copyPropagation(void) {
  if (unoptimized)
    return;
  Vec<FnSymbol*> fns;
  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    compressUnnecessaryScopes(fn);
    removeUnnecessaryGotos(fn);
    removeUnusedLabels(fn);
    localCopyPropagation(fn);
    deadVariableElimination(fn);
    deadExpressionElimination(fn);
  }
}
