#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

//
// Removes nested block statements.  We don't want basic blocks to
// span across multiple block statements or we run into trouble where
// a variable is used outside of its scope during copy propagation.
//
void compressUnnecessaryScopes(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (BlockStmt* block = dynamic_cast<BlockStmt*>(ast)) {
      if (block->loopInfo ||
          block->blockTag == BLOCK_COBEGIN ||
          block->blockTag == BLOCK_BEGIN)
        continue;
      if (block->list) {
        for_alist(Expr, stmt, block->body)
          block->insertBefore(stmt->remove());
        block->remove();
      }
    }
  }
}

//
// Applies local copy propagation ultra conservatively in the presence
// of pass by reference assuming that any argument to a call may be
// pass by reference.
//
// In the presence of threads, made even more conservative; compiler
// temps only.
//
typedef ChainHashMap<Symbol*,PointerHashFns,Symbol*> AvailableMap;

static void
copyAvailable(AvailableMap& available, CallExpr* call) {
  if (call->primitive) {
    if (!strcmp(call->primitive->name, "fscanf"))
      return;
    for_actuals(actual, call) {
      if (SymExpr* sym = dynamic_cast<SymExpr*>(actual))
        if (Symbol* copy = available.get(sym->var))
          sym->var = copy;
      if (CallExpr* call = dynamic_cast<CallExpr*>(actual))
        if (call->isPrimitive(PRIMITIVE_CAST))
          if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2)))
            if (Symbol* copy = available.get(sym->var))
              sym->var = copy;
    }
  } else {
    for_formals_actuals(formal, actual, call) {
      if (formal->intent != INTENT_BLANK)
        continue;
      if (SymExpr* sym = dynamic_cast<SymExpr*>(actual))
        if (Symbol* copy = available.get(sym->var))
          sym->var = copy;
      if (CallExpr* call = dynamic_cast<CallExpr*>(actual))
        if (call->isPrimitive(PRIMITIVE_CAST))
          if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2)))
            if (Symbol* copy = available.get(sym->var))
              sym->var = copy;
    }
  }
}

static void
removeAvailable(AvailableMap& available, CallExpr* call) {
  Vec<Symbol*> keys;
  available.get_keys(keys);
  Vec<BaseAST*> asts;
  collect_asts(&asts, call);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
      forv_Vec(Symbol, key, keys) {
        if (key == sym->var || available.get(key) == sym->var)
          available.del(key);
      }
    }
  }
}

void localCopyPropagation(BasicBlock* bb) {
  AvailableMap available;
  forv_Vec(Expr, expr, bb->exprs) {
    // Replace rhs that match available
    if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (SymExpr* rhs = dynamic_cast<SymExpr*>(call->get(2)))
          if (Symbol* copy = available.get(rhs->var))
            rhs->var = copy;
        if (CallExpr* rhs = dynamic_cast<CallExpr*>(call->get(2)))
          copyAvailable(available, rhs);
      } else {
        copyAvailable(available, call);
      }
    }

    // Remove pairs of invalidated copies
    // This would be alot easier if we didn't have to worry about pass
    // by reference!
    if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (SymExpr* lhs = dynamic_cast<SymExpr*>(call->get(1))) {
          Vec<Symbol*> keys;
          available.get_keys(keys);
          forv_Vec(Symbol, key, keys) {
            if (key == lhs->var || available.get(key) == lhs->var)
              available.del(key);
          }
        }
        if (CallExpr* rhs = dynamic_cast<CallExpr*>(call->get(2)))
          removeAvailable(available, rhs);
      } else {
        removeAvailable(available, call);
      }
    }

    // Insert pairs of available copies
    if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (SymExpr* lhs = dynamic_cast<SymExpr*>(call->get(1))) {
          if (SymExpr* rhs = dynamic_cast<SymExpr*>(call->get(2))) {
            if (lhs->var == rhs->var)
              continue;
            if (!lhs->var->isCompilerTemp) // only compiler temps
              continue;
            if (lhs->var->isReference || rhs->var->isReference)
              continue;
            available.put(lhs->var, rhs->var);
          }
        }
      }
    }
  }
}

void localCopyPropagation(FnSymbol* fn) {
  buildBasicBlocks(fn);
  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    localCopyPropagation(bb);
  }
}

//
// Removes local variables that are only targets for moves, but are
// never used anywhere.
//
void deadVariableElimination(FnSymbol* fn) {
  compute_sym_uses(fn);
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  bool change = true;
  while (change) {
    change = false;
    forv_Vec(BaseAST, ast, asts) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
        if (!def->parentSymbol)
          continue;
        if (!dynamic_cast<VarSymbol*>(def->sym)) // labels, types, ...?
          continue;
        if (!def->sym->isCompilerTemp && def->parentSymbol != fn) // nested types not pulled out
          continue;
        bool used = false;
        forv_Vec(SymExpr, use, def->sym->uses) {
          if (use->parentSymbol) {
            CallExpr* move = dynamic_cast<CallExpr*>(use->parentExpr);
            if (!move || !move->isPrimitive(PRIMITIVE_MOVE) || move->get(1) != use)
              used = true;
          }
        }
        if (!used) {
          forv_Vec(SymExpr, use, def->sym->uses) {
            if (use->parentSymbol) {
              CallExpr* move = dynamic_cast<CallExpr*>(use->parentExpr);
              Expr* rhs = move->get(2);
              if (rhs->astType == EXPR_SYM)
                move->remove();
              else
                move->replace(rhs->remove());
            }
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
