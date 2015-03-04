/*
 * Copyright 2004-2015 Cray Inc.
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


#include "optimizations.h"

#include "astutil.h"
#include "oldCollectors.h"
#include "bb.h"
#include "expr.h"
#include "ForLoop.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "WhileStmt.h"
#include "ForLoop.h"

#include <queue>
#include <set>

typedef std::set<BasicBlock*> BasicBlockSet;

static void deadBlockElimination(FnSymbol* fn);
static void findReachableBlocks(FnSymbol* fn, BasicBlockSet& reachable);
static void deleteUnreachableBlocks(FnSymbol* fn, BasicBlockSet& reachable);
static bool         isInCForLoopHeader(Expr* expr);
static void         cleanupLoopBlocks(FnSymbol* fn);

static unsigned int deadBlockCount;
static unsigned int deadModuleCount;



//
//
// 2014/10/17 TO DO Noakes/Elliot
//
// There are opportunities to do additional cleanup of the AST e.g.
//
// remove blockStmts with empty bodies
// remove condStmts  with empty bodies
// remove jumps to labels that immmediately follow
//
// This may require multiple passses to converge e.g.
//
// A block statement that contains an empty block statement
//
// An empty cond statement between a goto and the target of the goto
//
//

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
    Expr* exprAst = toExpr(ast);

    if (exprAst == 0) {

    } else if (isAlive(exprAst) == false) {

    } else if (SymExpr* expr = toSymExpr(ast)) {
      if (expr->isStmtExpr() == true && isInCForLoopHeader(expr) == false) {
        expr->remove();
      }

    } else if (CallExpr* expr = toCallExpr(ast)) {
      if (expr->isPrimitive(PRIM_CAST) ||
          expr->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
          expr->isPrimitive(PRIM_GET_MEMBER) ||
          expr->isPrimitive(PRIM_DEREF) ||
          expr->isPrimitive(PRIM_ADDR_OF)) {
        if (expr->isStmtExpr())
          expr->remove();
      }

      if (expr->isPrimitive(PRIM_MOVE) || expr->isPrimitive(PRIM_ASSIGN))
        if (SymExpr* lhs = toSymExpr(expr->get(1)))
          if (SymExpr* rhs = toSymExpr(expr->get(2)))
            if (lhs->var == rhs->var)
              expr->remove();

    } else if (CondStmt* cond = toCondStmt(ast)) {
      // Compensate for deadBlockElimination
      if (cond->condExpr == NULL) {
        cond->remove();

      } else if (cond->thenStmt == NULL && cond->elseStmt == NULL) {
        cond->remove();

      } else {

        // Invert the condition and shuffle the alternative
        if (cond->thenStmt == NULL) {
          Expr* condExpr = new CallExpr(PRIM_UNARY_LNOT, cond->condExpr);

          cond->replaceChild(cond->condExpr, condExpr);
          cond->replaceChild(cond->thenStmt, cond->elseStmt);
          cond->replaceChild(cond->elseStmt, NULL);

        // NOAKES 2014/11/14 It's "odd" that folding is being done here
        } else {
          cond->foldConstantCondition();
        }

        // NOAKES 2014/11/14 Testing suggests this is always a NOP
        removeDeadIterResumeGotos();
      }
    }
  }
}

static bool isInCForLoopHeader(Expr* expr) {
  Expr* stmtExpr = expr->parentExpr;
  bool  retval   = false;

  if (BlockStmt* blockStmt = toBlockStmt(stmtExpr)) {
    retval = (blockStmt->blockTag == BLOCK_C_FOR_LOOP) ? true : false;
  }

  return retval;
}

void deadCodeElimination(FnSymbol* fn) {
  std::map<SymExpr*, Vec<SymExpr*>*> DU;
  std::map<SymExpr*, Vec<SymExpr*>*> UD;

  std::map<Expr*,    Expr*>          exprMap;

  Vec<Expr*>                         liveCode;
  Vec<Expr*>                         workSet;

  BasicBlock::buildBasicBlocks(fn);

  buildDefUseChains(fn, DU, UD);

  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    for (size_t i = 0; i < bb->exprs.size(); i++) {
      Expr*         expr        = bb->exprs[i];
      bool          isEssential = bb->marks[i];

      Vec<BaseAST*> asts;

      collect_asts(expr, asts);

      forv_Vec(BaseAST, ast, asts) {
        if (Expr* sub = toExpr(ast)) {
          exprMap[sub] = expr;
        }
      }

      if (isEssential == false) {
        forv_Vec(BaseAST, ast, asts) {
          if (CallExpr* call = toCallExpr(ast)) {
            // mark assignments to global variables as essential
            if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
              if (SymExpr* se = toSymExpr(call->get(1))) {
                if (DU.count(se) == 0) {
                  isEssential = true;
                }
              }
            }
          }
        }
      }

      if (isEssential) {
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
    deadBlockElimination();

    deadModuleCount = 0;

    forv_Vec(FnSymbol, fn, gFnSymbols) {

      // 2014/10/17   Noakes and Elliot
      // Dead Block Elimination may convert valid loops to "malformed" loops.
      // Some of these will break BasicBlock construction. Clean them up.
      cleanupLoopBlocks(fn);

      deadCodeElimination(fn);

      deadVariableElimination(fn);

      // 2014/10/17   Noakes and Elliot
      // Dead Variable Elimination may convert some "uninteresting" loops
      // that were left behind by DeadBlockElimination and turn them in to
      // "malformed" loops.  Cleanup again.
      cleanupLoopBlocks(fn);

      deadExpressionElimination(fn);
    }

    deadModuleElimination();

    if (fReportDeadModules)
      printf("Removed %d dead modules.\n", deadModuleCount);
  }
}

void deadBlockElimination()
{
  deadBlockCount = 0;

  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    if (!isAlive(fn))
      continue;
    deadBlockElimination(fn);
  }

  if (fReportDeadBlocks)
    printf("\tRemoved %d dead blocks.\n", deadBlockCount);

}

// Look for and remove unreachable blocks.
static void deadBlockElimination(FnSymbol* fn)
{
  // We need the basic block information to be correct, so recompute it.
  BasicBlock::buildBasicBlocks(fn);

  // Find the reachable basic blocks within this function.
  BasicBlockSet reachable;

  findReachableBlocks(fn, reachable);
  deleteUnreachableBlocks(fn, reachable);
}

// Muchnick says we can enumerate the unreachable blocks first and then just
// remove them.  We only need to do this once, because removal of an
// unreachable block cannot possibly make any reachable block unreachable.
static void findReachableBlocks(FnSymbol* fn, BasicBlockSet& reachable)
{
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
}

static void deleteUnreachableBlocks(FnSymbol* fn, BasicBlockSet& reachable)
{
  // Visit all the blocks, deleting all those that are not reachable
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

      CondStmt*  condStmt  = toCondStmt(expr->parentExpr);
      WhileStmt* whileStmt = toWhileStmt(expr->parentExpr);
      ForLoop*   forLoop   = toForLoop(expr->parentExpr);

      if (condStmt && condStmt->condExpr == expr)
        // If the expr is the condition expression of an if statement,
        // then remove the entire if. (NOTE 1)
        condStmt->remove();

      else if (whileStmt && whileStmt->condExprGet() == expr)
        // If the expr is the condition expression of a while statement,
        // then remove the entire While. (NOTE 1)
        whileStmt->remove();

      else if (forLoop   && forLoop->indexGet()      == expr)
        // Do nothing. (NOTE 2)
        ;

      else if (forLoop   && forLoop->iteratorGet()   == expr)
        // Do nothing. (NOTE 2)
        ;

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

// 2014/10/15
//
// Dead Block elimination can create at least two forms of mal-formed AST
//
// A valid ForLoop can be transformed in to
//
//              for ( ; ; ) {
//              }
//
// and a valid WhileLoop can be transformed in to
//
//              while ( ) {
//              }
//
// The C standard defines these as infinite loops.  In practice the
// Chapel compiler will only leave these ASTs in unreachable code and
// so these wouldn't lead to runtime failures but each of these forms
// cause problems in the compiler down stream from here.
//
// 2014/10/17
//
// Additionally DBE can create loops that are similar to the above but
// that include some number of DefExprs (there is currently code in DBE
// to prevent it removing DefExprs for reasons that are partially but
// not fully understood).  These loops will be converted to the former
// case during DeadVariableElimination
//

static void cleanupLoopBlocks(FnSymbol* fn) {
  std::vector<Expr*> stmts;

  collect_stmts_STL(fn->body, stmts);

  for_vector (Expr, expr, stmts) {
    if (BlockStmt* stmt = toBlockStmt(expr)) {
      stmt->deadBlockCleanup();
    }
  }
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

//########################################################################
//# NOTES
//#
//# 1. This cleanup is not really part of dead block removal, but is necessary
//#    to avoid leaving the AST in a malformed state.  A more-factored approach
//#    would be to have a cleanup function that can be invoked on parts of the
//#    tree to normalize AST constructs that have been partially eviscerated.
//#
//#    This particular traversal should probably be done in postorder.  Then,
//#    the check at the top of the loop can be eliminated additional checks can
//#    be added to ensure that when a construct is to be removed, all of its
//#    constituent parts have already been removed from the tree.
//#
//# 2. Because the ForLoop construct contains initialization code, we cannot
//#    necessarily remove the entire for loop when we discover that evaluation
//#    of the iterator index is dead.  (It's probably a safer bet when the
//#    iterator expression is dead.)
//#
