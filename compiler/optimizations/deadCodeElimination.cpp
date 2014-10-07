/*
 * Copyright 2004-2014 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "view.h"

#include <set>
#include <queue>


//
// Static function declarations.
//
static void deadBlockElimination(FnSymbol* fn);
// static void deadGotoElimination(FnSymbol* fn);

// Static variables.
static unsigned deadBlockCount;
static unsigned deadModuleCount;


// Determines if an expr is used inside of the header for a c for loop. c for
// loop header is of the form '"c for loop" {inits}, {test}, {incrs}'
//
// Only returns true for exprs in the init, test, incr blocks, not for the
// blocks themselves.
//
// TODO should this be updated to only look for exprs in the test segment that
// are conditional primitives?
static bool isInCForLoopHeader(Expr* expr) {
  if (expr->parentExpr && expr->parentExpr->parentExpr) {
    if (CallExpr* call = toCallExpr(expr->parentExpr->parentExpr)) {
      if (call->isPrimitive(PRIM_BLOCK_C_FOR_LOOP)) {
        return true;
      }
    }
  }
  return false;
}

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

  forv_Vec(Symbol, sym, symSet)
  {
    // We're interested only in VarSymbols.
    if (!isVarSymbol(sym))
      continue;

    // A method must have a _this symbol, even if it is not used.
    if (sym == fn->_this)
      continue;

    if (isDeadVariable(sym, defMap, useMap)) {
      for_defs(se, defMap, sym) {
        CallExpr* call = toCallExpr(se->parentExpr);
        INT_ASSERT(call && 
                   (call->isPrimitive(PRIM_MOVE) ||
                    call->isPrimitive(PRIM_ASSIGN)));
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
      if (isInCForLoopHeader(expr)) {
        continue;
      }
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
      if (expr->isPrimitive(PRIM_MOVE) || expr->isPrimitive(PRIM_ASSIGN))
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
// TODO: Factor this long function?
  buildBasicBlocks(fn);

  std::map<SymExpr*,Vec<SymExpr*>*> DU;
  std::map<SymExpr*,Vec<SymExpr*>*> UD;
  buildDefUseChains(fn, DU, UD);

  std::map<Expr*,Expr*> exprMap;
  Vec<Expr*> liveCode;
  Vec<Expr*> workSet;
  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    for_vector(Expr, expr, bb->exprs) {
      bool essential = false;
      Vec<BaseAST*> asts;
      collect_asts(expr, asts);
      forv_Vec(BaseAST, ast, asts) {
        if (isInCForLoopHeader(expr)) {
          essential = true;
        }
        if (CallExpr* call = toCallExpr(ast)) {
          // mark function calls and essential primitives as essential
          if (call->isResolved() ||
              (call->primitive && call->primitive->isEssential))
            essential = true;
          // mark assignments to global variables as essential
          if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN))
            if (SymExpr* se = toSymExpr(call->get(1)))
              if (DU.count(se) == 0 || // DU chain only contains locals
                  !se->var->type->refType) // reference issue
                essential = true;
        }
        if (Expr* sub = toExpr(ast)) {
          exprMap[sub] = expr;
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
      if (UD.count(se) != 0) {
        Vec<SymExpr*>* defs = UD[se];
        forv_Vec(SymExpr, def, *defs) {
          INT_ASSERT(exprMap.count(def) != 0);
          Expr* expr = exprMap[def];
          if (!liveCode.set_in(expr)) {
            liveCode.set_add(expr);
            workSet.add(expr);
          }
        }
      }
    }
  }

  // This removes dead expressions from each block.
  for_vector(BasicBlock, bb1, *fn->basicBlocks) {
    for_vector(Expr, expr, bb1->exprs) {
      if (isSymExpr(expr) || isCallExpr(expr))
        if (!liveCode.set_in(expr))
          expr->remove();
    }
  }

  freeDefUseChains(DU, UD);
}


// Determines if a module is dead. A module is dead if the module's init
// function can only be called from module code, and the init function
// is empty, and the init function is the only thing in the module, and the
// module is not a nested module. 
static bool isDeadModule(ModuleSymbol* mod) {
  // The main module and any module whose init function is exported
  // should never be considered dead, as the init function can be
  // explicitly called from the runtime, or other c code
  if (mod == mainModule || mod->hasFlag(FLAG_EXPORT_INIT)) return false;

  // because of the way modules are initialized, we don't want to consider a
  // nested function as dead as its outer module and all of its uses should
  // have their initializer called by the inner module. 
  if (mod->defPoint->getModule() != theProgram && 
      mod->defPoint->getModule() != rootModule) 
    return false;

  // if there is only one thing in the module
  if (mod->block->body.length == 1) {
    // and that thing is the init function 
    if (mod->block->body.only() == mod->initFn->defPoint) {
      // and the init function is empty (only has a return)
      if (mod->initFn->body->body.length == 1) {
        // then the module is dead 
        return true;
      }
    }
  }
  return false;
}


// Eliminates all dead modules
static void deadModuleElimination() {
  forv_Vec(ModuleSymbol, mod, allModules) {
    if (isDeadModule(mod) == true) {
      deadModuleCount++;

      // remove the dead module and its initFn
      mod->defPoint->remove();
      mod->initFn->defPoint->remove();

      // Inform every module about the dead module
      forv_Vec(ModuleSymbol, modThatMightUse, allModules) {
        if (modThatMightUse != mod) {
          modThatMightUse->moduleUseRemove(mod);
        }
      }
    }
  }
}


void deadCodeElimination() {
  if (!fNoDeadCodeElimination) {
    deadBlockCount = 0;
    deadModuleCount = 0;
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      deadBlockElimination(fn);
//      deadGotoElimination(fn);
      deadCodeElimination(fn);
      deadVariableElimination(fn);
      deadExpressionElimination(fn);
    }
    deadModuleElimination();
    
    if (fReportDeadBlocks)
      printf("\tRemoved %d dead blocks.\n", deadBlockCount);
    if (fReportDeadModules)
      printf("Removed %d dead modules.\n", deadModuleCount);

  }
}

// Look for and remove unreachable blocks.
// Muchnick says we can enumerate the unreachable blocks first and then just
// remove them.  We only need to do this once, because removal of an
// unreachable block cannot possibly make any reachable block unreachable.
static void deadBlockElimination(FnSymbol* fn)
{
  // We need the basic block information to be correct, so recompute it.
  buildBasicBlocks(fn);

  // Find the reachable basic blocks within this function.
  std::set<BasicBlock*> reachable;

  // We set up a work queue to perform a BFS on reachable blocks, and seed it
  // with the first block in the function.
  std::queue<BasicBlock*> work_queue;
  work_queue.push((*fn->basicBlocks)[0]);

  // Then we iterate until there are no more blocks to visit.
  while (!work_queue.empty())
  {
    // Fetch and remove the next block.
    BasicBlock* bb = work_queue.front(); 
    work_queue.pop();

    // Ignore it if we've already seen it.
    if (reachable.count(bb))
      continue;

    // Otherwise, mark it as reachable, and append all of its successors to the
    // work queue.
    reachable.insert(bb);
    for_vector(BasicBlock, out, bb->outs)
      work_queue.push(out);
  }

  // Now we simply visit all the blocks, deleting all those that are not
  // rechable.
  for_vector(BasicBlock, bb, *fn->basicBlocks)
  {
    if (reachable.count(bb))
      continue;

    ++deadBlockCount;

    // Remove all of its expressions.
    for_vector(Expr, expr, bb->exprs)
    {
      if (! expr->parentExpr)
        continue;   // This node is no longer in the tree.

      // Do not remove def expressions (for now)
      // In some cases (associated with iterator code), defs appear in dead
      // blocks but are used in later blocks, so removing the defs results
      // in a verify error.
      // TODO: Perhaps this reformulation of unreachable block removal does a better
      // job and those blocks are now removed as well.  If so, this IF can be removed.
      if (toDefExpr(expr))
        continue;

      CondStmt* cond = toCondStmt(expr->parentExpr);
      if (cond && cond->condExpr == expr)
        // If expr is the condition expression in an if statement,
        // then remove the entire if.
        cond->remove();
      else
        expr->remove();
    }
  }
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
