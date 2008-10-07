#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "view.h"

//
// Removes local variables that are only targets for moves, but are
// never used anywhere.
//
static bool isDeadVariable(Symbol* var,
                           Map<Symbol*,Vec<SymExpr*>*>& defMap,
                           Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  if (isReferenceType(var->type)) {
    Vec<SymExpr*>* uses = useMap.get(var);
    Vec<SymExpr*>* defs = defMap.get(var);
    return (!uses || uses->n == 0) && (!defs || defs->n <= 1);
  } else {
    Vec<SymExpr*>* uses = useMap.get(var);
    return !uses || uses->n == 0;
  }
}

static void deadVariableEliminationHelp(FnSymbol* fn,
                                        Symbol* var,
                                        Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                        Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  if (var->astTag == SYMBOL_VAR &&
      var->defPoint &&
      var->defPoint->parentSymbol == fn &&
      isDeadVariable(var, defMap, useMap)) {
    for_defs(se, defMap, var) {
      CallExpr* call = toCallExpr(se->parentExpr);
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (SymExpr* rhs = toSymExpr(call->get(2))) {
          call->remove();
          if (Vec<SymExpr*>* uses = useMap.get(var))
            uses->n--;
          deadVariableEliminationHelp(fn, rhs->var, defMap, useMap);
        } else {
          call->replace(call->get(2)->remove());
        }
      } else
        INT_FATAL("unexpected case");
    }
    var->defPoint->remove();
  }
}

void deadVariableElimination(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  collect_asts(fn, asts);
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(fn, defMap, useMap);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      deadVariableEliminationHelp(fn, def->sym, defMap, useMap);
    }
  }
  freeDefUseMaps(defMap, useMap);
}

//
// Removes expression statements that have no effect.
//
void deadExpressionElimination(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  collect_asts(fn, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* expr = toSymExpr(ast)) {
      if (expr->parentExpr && expr == expr->getStmtExpr())
        expr->remove();
    } else if (CallExpr* expr = toCallExpr(ast)) {
      if (expr->isPrimitive(PRIMITIVE_CAST) ||
          expr->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE) ||
          expr->isPrimitive(PRIMITIVE_GET_MEMBER) ||
          expr->isPrimitive(PRIMITIVE_GET_REF) ||
          expr->isPrimitive(PRIMITIVE_SET_REF))
        if (expr->parentExpr && expr == expr->getStmtExpr())
          expr->remove();
      if (expr->isPrimitive(PRIMITIVE_MOVE))
        if (SymExpr* lhs = toSymExpr(expr->get(1)))
          if (SymExpr* rhs = toSymExpr(expr->get(2)))
            if (lhs->var == rhs->var)
              expr->remove();
    } else if (CondStmt* cond = toCondStmt(ast)) {
      cond->fold_cond_stmt();
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
      collect_asts(expr, asts);
      forv_Vec(BaseAST, ast, asts) {
        if (CallExpr* call = toCallExpr(ast)) {
          // mark function calls and essential primitives as essential
          if (call->isResolved() ||
              (call->primitive && call->primitive->isEssential))
            essential = true;
          // mark assignments to global variables as essential
          if (call->isPrimitive(PRIMITIVE_MOVE))
            if (SymExpr* se = toSymExpr(call->get(1)))
              if (!DU.get(se) || // DU chain only contains locals
                  !se->var->type->refType) // reference issue
                essential = true;
        }
        if (Expr* sub = toExpr(ast)) {
          exprMap.put(sub, expr);
          if (BlockStmt* block = toBlockStmt(sub->parentExpr))
            if (block->blockInfo == sub)
              essential = true;
          if (CondStmt* cond = toCondStmt(sub->parentExpr))
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
    collect_asts(expr, asts);
    forv_Vec(BaseAST, ast, asts) {
      if (SymExpr* se = toSymExpr(ast)) {
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
      if (expr->astTag == EXPR_SYM || expr->astTag == EXPR_CALL)
        if (!liveCode.set_in(expr))
          expr->remove();
    }
  }

  freeDefUseChains(DU, UD);
}

void deadCodeElimination() {
  if (!fNoDeadCodeElimination) {
    forv_Vec(FnSymbol, fn, gFns) {
      deadCodeElimination(fn);
      deadVariableElimination(fn);
      deadExpressionElimination(fn);
    }
  }
}
