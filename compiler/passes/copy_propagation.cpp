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
      if (block->prev && block->next) {
        for_alist(Stmt, stmt, block->body)
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
void localCopyPropagation(BasicBlock* bb) {
  ChainHashMap<Symbol*,PointerHashFns,Symbol*> available;
  forv_Vec(Expr, expr, bb->exprs) {
    // Replace rhs that match available
    // It would be easier to replace more if we didn't worry about
    // pass by reference!
    if (CallExpr* move = dynamic_cast<CallExpr*>(expr))
      if (move->isPrimitive(PRIMITIVE_MOVE))
        if (SymExpr* rhs = dynamic_cast<SymExpr*>(move->get(2)))
          if (Symbol* copy = available.get(rhs->var))
            rhs->var = copy;

    // Remove pairs of invalidated copies
    // This would be alot easier if we didn't have to worry about pass
    // by reference!
    // Remove all pairs of global variables in presence of a
    //  non-primitive function call
    Vec<Symbol*> keys;
    available.get_keys(keys);
    Vec<BaseAST*> asts;
    collect_asts(&asts, expr);
    forv_Vec(BaseAST, ast, asts) {
      if (SymExpr* sym = dynamic_cast<SymExpr*>(ast))
        forv_Vec(Symbol, key, keys)
          if (key == sym->var || available.get(key) == sym->var)
            available.del(key);
      if (CallExpr* call = dynamic_cast<CallExpr*>(ast))
        if (!call->primitive)
          forv_Vec(Symbol, key, keys)
            if (key->parentScope != ast->parentScope)
              available.del(key);
    }

    // Insert pairs of available copies
    if (CallExpr* move = dynamic_cast<CallExpr*>(expr)) {
      if (move->isPrimitive(PRIMITIVE_MOVE)) {
        if (SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1))) {
          if (SymExpr* rhs = dynamic_cast<SymExpr*>(move->get(2))) {
            VarSymbol* config;
            config = dynamic_cast<VarSymbol*>(lhs->var);
            if (config && config->varClass == VAR_CONFIG)
              continue;
            config = dynamic_cast<VarSymbol*>(rhs->var);
            if (config && config->varClass == VAR_CONFIG)
              continue;
            available.put(lhs->var, rhs->var);
          }
        }
      }
    }
  }
}

//
// Removes local variables that are only targets for moves, but are
// never used anywhere.
//
void deadVariableElimination(FnSymbol* fn) {
  if (dynamic_cast<ModuleSymbol*>(fn->body->blkScope->astParent))
    return;  // don't delete module-level globals
  compute_sym_uses(fn);
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (!dynamic_cast<VarSymbol*>(def->sym)) // labels, types, ...?
        continue;
      if (def->parentSymbol != fn) // nested types not pulled out
        continue;
      bool used = false;
      forv_Vec(SymExpr, use, *def->sym->uses) {
        if (!use->parentSymbol)
          continue;
        CallExpr* move = dynamic_cast<CallExpr*>(use->parentExpr);
        if (!move || !move->isPrimitive(PRIMITIVE_MOVE) || move->get(1) != use)
          used = true;
      }
      if (!used) {
        forv_Vec(SymExpr, use, *def->sym->uses) {
          CallExpr* move = dynamic_cast<CallExpr*>(use->parentExpr);
          move->replace(move->get(2)->remove());
        }
        def->parentStmt->remove();
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
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(ast))
      if (dynamic_cast<SymExpr*>(exprStmt->expr))
        exprStmt->remove();
  }
}

void copyPropagation(void) {
  if (parallelPass) // flattening of blocks a problem for cobegin
    return;
  Vec<FnSymbol*> fns;
  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    compressUnnecessaryScopes(fn);
    buildBasicBlocks(fn);
    forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
      localCopyPropagation(bb);
    }
    deadVariableElimination(fn);
    deadExpressionElimination(fn);
  }
}
