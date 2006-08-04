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
      if (block->blockTag == BLOCK_COBEGIN ||
          block->blockTag == BLOCK_BEGIN)
        continue;
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
// In the presence of threads, made even more conservative; compiler
// temps only.
void localCopyPropagation(BasicBlock* bb) {
  ChainHashMap<Symbol*,PointerHashFns,Symbol*> available;
  forv_Vec(Expr, expr, bb->exprs) {
    // Replace rhs that match available
    // It would be easier to replace more if we didn't worry about
    // pass by reference!
    if (CallExpr* move = dynamic_cast<CallExpr*>(expr))
      if (move->isPrimitive(PRIMITIVE_MOVE)) {
        if (SymExpr* rhs = dynamic_cast<SymExpr*>(move->get(2)))
          if (Symbol* copy = available.get(rhs->var))
            rhs->var = copy;
        if (CallExpr* rhs = dynamic_cast<CallExpr*>(move->get(2)))
          if (rhs->primitive)
            for_alist(Expr, actual, rhs->argList) {
              if (SymExpr* sym = dynamic_cast<SymExpr*>(actual))
                if (Symbol* copy = available.get(sym->var))
                  sym->var = copy;
            }
      }

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

            if (!lhs->var->isCompilerTemp) // only compiler temps
              continue;

            // Note: Disabled on configuration variables until those
            // are in the AST for real

            VarSymbol* lhs_var = dynamic_cast<VarSymbol*>(lhs->var);
            if (lhs_var) {
              if (lhs_var->varClass == VAR_CONFIG)
                continue;
            }
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
  bool is_global = false;
  if (dynamic_cast<ModuleSymbol*>(fn->body->blkScope->astParent))
    is_global = true;
  compute_sym_uses(fn);
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (is_global && !def->sym->isCompilerTemp)
        continue;
      if (!dynamic_cast<VarSymbol*>(def->sym)) // labels, types, ...?
        continue;
      if (!def->sym->isCompilerTemp && def->parentSymbol != fn) // nested types not pulled out
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

//
// Removes gotos where the label immediately follows the goto
//
void removeUnnecessaryGotos(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (GotoStmt* gotoStmt = dynamic_cast<GotoStmt*>(ast))
      if (ExprStmt* labelStmt = dynamic_cast<ExprStmt*>(gotoStmt->next))
        if (DefExpr* def = dynamic_cast<DefExpr*>(labelStmt->expr))
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
    if (ExprStmt* labelStmt = dynamic_cast<ExprStmt*>(ast))
      if (DefExpr* def = dynamic_cast<DefExpr*>(labelStmt->expr))
        if (LabelSymbol* label = dynamic_cast<LabelSymbol*>(def->sym))
          if (!labels.set_in(label))
            labelStmt->remove();
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
    buildBasicBlocks(fn);
    forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
      localCopyPropagation(bb);
    }
    deadVariableElimination(fn);
    deadExpressionElimination(fn);
  }
}
