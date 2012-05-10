
#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "view.h"


//
// Static function declarations.
//
static bool deadBlockElimination(FnSymbol* fn);
// static void deadGotoElimination(FnSymbol* fn);

// Static variables.
static unsigned deadBlockCount;


//
// Removes local variables that are only targets for moves, but are
// never used anywhere.
//
static bool isDeadVariable(Symbol* var,
                           Map<Symbol*,Vec<SymExpr*>*>& defMap,
                           Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  if (var->type->symbol->hasFlag(FLAG_REF)) {
    Vec<SymExpr*>* uses = useMap.get(var);
    Vec<SymExpr*>* defs = defMap.get(var);
    return (!uses || uses->n == 0) && (!defs || defs->n <= 1);
  } else {
    Vec<SymExpr*>* uses = useMap.get(var);
    return !uses || uses->n == 0;
  }
}

void deadVariableElimination(FnSymbol* fn) {
  Vec<Symbol*> symSet;
  Vec<SymExpr*> symExprs;
  collectSymbolSetSymExprVec(fn, symSet, symExprs);

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(symSet, symExprs, defMap, useMap);

  forv_Vec(Symbol, sym, symSet) if (isVarSymbol(sym)) {
    if (isDeadVariable(sym, defMap, useMap)) {
      for_defs(se, defMap, sym) {
        CallExpr* call = toCallExpr(se->parentExpr);
        INT_ASSERT(call && call->isPrimitive(PRIM_MOVE));
        Expr* rhs = call->get(2)->remove();
        if (!isSymExpr(rhs))
          call->replace(rhs);
        else
          call->remove();
      }
      sym->defPoint->remove();
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
    Expr *expr = toExpr(ast);
    if (expr && expr->parentExpr == NULL) // expression already removed
      continue;
    if (SymExpr* expr = toSymExpr(ast)) {
      if (expr == expr->getStmtExpr())
        expr->remove();
    } else if (CallExpr* expr = toCallExpr(ast)) {
      if (expr->isPrimitive(PRIM_CAST) ||
          expr->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
          expr->isPrimitive(PRIM_GET_MEMBER) ||
          expr->isPrimitive(PRIM_DEREF) ||
          expr->isPrimitive(PRIM_ADDR_OF))
        if (expr == expr->getStmtExpr())
          expr->remove();
      if (expr->isPrimitive(PRIM_MOVE))
        if (SymExpr* lhs = toSymExpr(expr->get(1)))
          if (SymExpr* rhs = toSymExpr(expr->get(2)))
            if (lhs->var == rhs->var)
              expr->remove();
    } else if (CondStmt* cond = toCondStmt(ast)) {
      cond->fold_cond_stmt();
    }
  }
}

void deadCodeElimination(FnSymbol* fn)
{
  buildBasicBlocks(fn);

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
          if (call->isPrimitive(PRIM_MOVE))
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
    Vec<SymExpr*> symExprs;
    collectSymExprs(expr, symExprs);
    forv_Vec(SymExpr, se, symExprs) {
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

  // This removes dead expressions from each block.
  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    forv_Vec(Expr, expr, bb->exprs) {
      if (isSymExpr(expr) || isCallExpr(expr))
        if (!liveCode.set_in(expr))
          expr->remove();
    }
  }

  freeDefUseChains(DU, UD);
}

void deadCodeElimination() {
  if (!fNoDeadCodeElimination) {
    deadBlockCount = 0;
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      bool change = false;
      do {
        change = deadBlockElimination(fn);
      } while (change);
//      deadGotoElimination(fn);
      deadCodeElimination(fn);
      deadVariableElimination(fn);
      deadExpressionElimination(fn);
    }

    if (fReportDeadBlocks)
      printf("\tRemoved %d dead blocks.\n", deadBlockCount);
  }
}

// Look for and remove unreachable blocks.
static bool deadBlockElimination(FnSymbol* fn)
{
  // We need the basic block information to be right here.
  buildBasicBlocks(fn);

  bool change = false;
  forv_Vec(BasicBlock, bb, *fn->basicBlocks)
  {
    // Ignore the first block.
    if (bb == fn->basicBlocks->v[0])
      continue;

    // If this block has no predecessors, then it is dead.
    if (bb->ins.count() == 0)
    {
      if (bb->exprs.n > 0)
      {
        change = true;
        ++deadBlockCount;

        // Remove all of its expressions.
        forv_Vec(Expr, expr, bb->exprs)
        {
          if (! expr->parentExpr)
            continue;   // This node is no longer in the tree.

          CondStmt* cond = toCondStmt(expr->parentExpr);
          if (cond && cond->condExpr == expr)
            // If expr is the condition expression in an if statement,
            // then remove the entire if.
            cond->remove();
          else
            expr->remove();
        }
      }

      // Get more out of one pass by removing this BB from the predecessor
      // lists of its successors.
      forv_Vec(BasicBlock, succ, bb->outs)
        succ->ins.set_remove(bb); 

      // We leave the "dead" bb structure in place.
      // The next time we construct basic blocks for this fn, it should be gone.
    }
  }

  return change;
}

//
// See if any iterator resume labels have been remove (and not re-inserted).
// If so, remove the matching gotos, if they haven't been yet.
//
void removeDeadIterResumeGotos() {
  forv_Vec(LabelSymbol, labsym, removedIterResumeLabels) {
    if (!isAlive(labsym) && isAlive(labsym->iterResumeGoto))
      labsym->iterResumeGoto->remove();
  }
  removedIterResumeLabels.clear();
}

//
// Make sure there are no iterResumeGotos to remove.
// Reset removedIterResumeLabels.
//
void verifyNcleanRemovedIterResumeGotos() {
  forv_Vec(LabelSymbol, labsym, removedIterResumeLabels) {
    if (!isAlive(labsym) && isAlive(labsym->iterResumeGoto))
      INT_FATAL("unexpected live goto for a dead removedIterResumeLabels label - missing a call to removeDeadIterResumeGotos?");
  }
  removedIterResumeLabels.clear();
}

// Look for pointless gotos and remove them.
// Probably the best way to do this is to scan the AST and remove gotos
// whose target labels follow immediately.
#if 0
static void deadGotoElimination(FnSymbol* fn)
{
  // We recompute basic blocks because deadBlockElimination may cause them
  // to be out of sequence.
  buildBasicBlocks(fn);
  forv_Vec(BasicBlock, bb, *fn->basicBlocks)
  {
    // Get the last expression in the block as a goto.
    int last = bb->exprs.length() - 1;
    if (last < 0)
      continue;

    Expr* e = bb->exprs.v[last];
    GotoStmt* s = toGotoStmt(e);
    if (!s)
      continue;

    // If there is only one successor to this block and it is the next block,
    // then the goto must point to it and is therefore pointless [sts].
    // This test should be more foolproof using the structure of the AST.
    if (bb->outs.n == 1 && bb->outs.v[0]->id == bb->id + 1)
      e->remove();
  }
}
#endif
