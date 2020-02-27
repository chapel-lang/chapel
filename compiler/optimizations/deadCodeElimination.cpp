/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
#include "bb.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "ModuleSymbol.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "WhileStmt.h"
#include "DoWhileStmt.h"

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
// This may require multiple passes to converge e.g.
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
static bool isDeadVariable(Symbol* var) {
  if (var->isRef()) {
    // is it defined more than once?
    int ndefs = var->countDefs(/*max*/ 2);
    return (!var->isUsed()) && (ndefs <= 1);
  } else {
    return !var->isUsed();
  }
}

void deadVariableElimination(FnSymbol* fn) {
  std::set<Symbol*> symSet;
  collectSymbolSet(fn, symSet);

  // Use 'symSet' and 'todo' together for a unique queue of symbols to process
  std::queue<Symbol*> todo;
  for_set(Symbol, sym, symSet) {
    todo.push(sym);
  }

  while(todo.empty() == false) {
    Symbol* sym = todo.front();
    todo.pop();
    symSet.erase(sym);

    // We're interested only in VarSymbols.
    if (!isVarSymbol(sym))
      continue;

    // A method must have a _this symbol, even if it is not used.
    if (sym == fn->_this)
      continue;

    if (isDeadVariable(sym)) {
      std::set<Symbol*> potentiallyChanged;
      for_SymbolDefs(se, sym) {
        CallExpr* call = toCallExpr(se->parentExpr);
        collectSymbolSet(call->getStmtExpr(), potentiallyChanged);
        INT_ASSERT(call);

        Expr* dest = NULL;
        Expr* rhs = NULL;
        bool ok = getSettingPrimitiveDstSrc(call, &dest, &rhs);
        INT_ASSERT(ok);

        rhs->remove();
        CallExpr* rhsCall = toCallExpr(rhs);
        if (rhsCall && (rhsCall->isResolved() || rhsCall->isPrimitive(PRIM_VIRTUAL_METHOD_CALL))) {
          // RHS might have side-effects, leave it alone
          call->replace(rhs);
        } else {
          call->remove();
        }
      }
      sym->defPoint->remove();

      // If we just removed a symbol, let's (re)visit the other symbols in
      // this statement in case they are now dead.
      for_set(Symbol, otherSym, potentiallyChanged) {
        if (otherSym != sym && symSet.find(otherSym) == symSet.end()) {
          symSet.insert(otherSym);
          todo.push(otherSym);
        }
      }
    }
  }
}

//
// Removes expression statements that have no effect.
//
void deadExpressionElimination(FnSymbol* fn) {
  std::vector<BaseAST*> asts;

  collect_asts(fn, asts);

  for_vector(BaseAST, ast, asts) {
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
          expr->isPrimitive(PRIM_ARRAY_GET) ||
          expr->isPrimitive(PRIM_ADDR_OF) ||
          expr->isPrimitive(PRIM_SET_REFERENCE)) {
        if (expr->isStmtExpr())
          expr->remove();
      }

      if (expr->isPrimitive(PRIM_MOVE) ||
          expr->isPrimitive(PRIM_ASSIGN))
        if (SymExpr* lhs = toSymExpr(expr->get(1)))
          if (SymExpr* rhs = toSymExpr(expr->get(2)))
            if (lhs->symbol() == rhs->symbol())
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
          cond->foldConstantCondition(false);
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

/************************************* | **************************************
*                                                                             *
*                      Eliminate dead string literals.                        *
*                                                                             *
* String literals are created by new_StringSymbol().  This includes strings   *
* used internally by the compiler (e.g. "boundedNone" for BoundedRangeType)   *
* and param string used for things like compiler error messages as well as    *
* strings that may be used at runtime.                                        *
*                                                                             *
* new_StringSymbol() adds all strings to stringLiteralModule, but strings     *
* that are only used for compilation or whose runtime path was param folded   *
* away are never removed because the code paths that are removed don't        *
* include the string literal initialization.                                  *
*                                                                             *
* There are three components to removing a dead string literal                *
*                                                                             *
*   1) Identifying that a literal is dead.                                    *
*                                                                             *
*      This relies on details of def-use analysis and of the code that        *
*      initializes string literals.  Both of these may change.                *
*                                                                             *
*   2) Remove the module level DefExpr.                                       *
*                                                                             *
*      This is easy.                                                          *
*                                                                             *
*   3) Remove the code that initializes the literal.                          *
*                                                                             *
*      This is fragile as it currently depends on implementation details in   *
*      other portions of the compiler.  It is believed that this will be      *
*      easier to manage when record initializers are in production.           *
*                                                                             *
* The hardest part is determining when this code needs to be revisited.       *
* 2017/03/04: Perform an "ad hoc" sanity check for now.                       *
* 2017/03/09: Disable this pass if flags might alter the pattern.             *
*                                                                             *
************************************** | *************************************/

static bool isDeadStringOrBytesLiteral(VarSymbol* string);
static void removeDeadStringLiteral(DefExpr* defExpr);

static void deadStringLiteralElimination() {
  // Noakes 2017/03/09
  //   These two flags are known to alter the pattern we are looking for.
  //   Rather than handle the variations we simply leak if these flags are
  //   on.  We anticipate that this will be easier to handle when record
  //   initializers are in production and have been applied to strings.
  if (fNoCopyPropagation == false &&
      fNoInline          == false) {
    int numStmt        = 0;
    int numDeadLiteral = 0;

    for_alist(stmt, stringLiteralModule->block->body) {
      numStmt = numStmt + 1;

      if (DefExpr* defExpr = toDefExpr(stmt)) {
        if (VarSymbol* symbol = toVarSymbol(defExpr->sym)) {
          if (isDeadStringOrBytesLiteral(symbol) == true) {
            removeDeadStringLiteral(defExpr);

            numDeadLiteral = numDeadLiteral + 1;
          }
        }
      }
    }

    //
    // Noakes 2017/03/04
    //
    // There is not a principled way to determine if other passes
    // have changed in a way that would confuse this pass.
    //
    // A quick review of a portion of test/release/examples shows that
    // this pass removes 85 - 95% of the string literals.  Signal an
    // error if this pass doesn't reclaim at least 10% of all string
    // literals unless this is minimal modules
    //
    if (fMinimalModules == false) {
      INT_ASSERT((1.0 * numDeadLiteral) / numStmt > 0.10);
    }
  }
}

// Noakes 2017/03/04: All literals have 1 def. Dead literals have 0 uses.
static bool isDeadStringOrBytesLiteral(VarSymbol* string) {
  bool retval = false;

  if (string->hasFlag(FLAG_CHAPEL_STRING_LITERAL) == true ||
      string->hasFlag(FLAG_CHAPEL_BYTES_LITERAL) == true) {
    int numDefs = string->countDefs();
    int numUses = string->countUses();

    retval = numDefs == 1 && numUses == 0;

    INT_ASSERT(numDefs == 1);
  }

  return retval;
}

// The current pattern to initialize a string literal,
// a VarSymbol _str_literal_NNN, is approximately
//
//   def  new_temp  : string; // defTemp
//
//   call createStringWithBorrowedBuffer(new_temp,c"literal",...); //factoryCall
//
//   move _str_literal_NNN, new_temp;  // this is 'defn' - the single def
//
static void removeDeadStringLiteral(DefExpr* defExpr) {
  SymExpr*   defn  = toVarSymbol(defExpr->sym)->getSingleDef();

  // Step backwards from 'defn'
  Expr* lastMove = defn->getStmtExpr();
  Expr* factoryCall = lastMove->prev;
  Expr* defTemp = factoryCall->prev;

  // Simple sanity checks
  INT_ASSERT(isDefExpr(defTemp));
  INT_ASSERT(isCallExpr(factoryCall));
  INT_ASSERT(isCallExpr(lastMove));

  lastMove->remove();
  factoryCall->remove();
  defTemp->remove();

  defExpr->remove();
}

/************************************* | **************************************
*                                                                             *
* A module is dead if                                                         *
*   the module's init function can only be called from module code      and   *
*   the init function is empty                                          and   *
*   the init function is the  only thing in the module                  and   *
*   the module is not a nested module.                                        *
*                                                                             *
************************************** | *************************************/

static bool isDeadModule(ModuleSymbol* mod) {
  AList body   = mod->block->body;
  bool  retval = false;

  // The main module should never be considered dead; the init function
  // can be explicitly called from the runtime or other c code
  if (mod == ModuleSymbol::mainModule()) {
    retval = false;

  // Ditto for an exported module
  } else if (mod->hasFlag(FLAG_EXPORT_INIT) == true) {
    retval = false;

  // Because of the way modules are initialized, we never consider a nested
  // module to be dead.
  } else if (mod->defPoint->getModule() != rootModule &&
             mod->defPoint->getModule() != theProgram) {
    retval = false;

  // Any module with more than 1 module-level statement is assumed to be live
  } else if (body.length >= 2) {
    retval = false;

  // A module might be considered to be dead if it has exactly 1 defExpr
  } else if (body.length == 1) {
    Expr* item = body.only();

    if (DefExpr* defExpr = toDefExpr(item)) {
      // A module is not dead if the sole definition is a type declaration
      if (isTypeSymbol(defExpr->sym) == true) {
        retval = false;

      // A module is dead if the sole definition is an "empty" init function
      } else if (FnSymbol* fn = toFnSymbol(defExpr->sym)) {
        if (mod->initFn == NULL) {
          INT_FATAL("Expected initFn for module '%s', but was null",
                    mod->name);

        } else if (mod->initFn == fn) {
          retval = mod->initFn->body->body.length == 1;

        } else {
          INT_ASSERT(false);
        }

      // The single definition is a nested module.  This module is not dead.
      } else if (isModuleSymbol(defExpr->sym) == true) {
        retval = false;

      } else {
        INT_ASSERT(false);
      }

    } else {
      INT_ASSERT(false);
    }

  } else {
    retval = false;
  }

  return retval;
}


// Eliminates all dead modules
static void deadModuleElimination() {
  deadModuleCount = 0;

  forv_Vec(ModuleSymbol, mod, allModules) {
    if (isDeadModule(mod) == true) {
      deadModuleCount++;

      // remove the dead module and its initFn
      mod->defPoint->remove();

      mod->initFn->defPoint->remove();

      // Inform every module about the dead module
      forv_Vec(ModuleSymbol, modThatMightUse, allModules) {
        if (modThatMightUse != mod) {
          modThatMightUse->deadCodeModuleUseRemove(mod);
        }
      }
    }
  }
}

void deadCodeElimination() {
  if (!fNoDeadCodeElimination) {
    deadBlockElimination();

    deadStringLiteralElimination();


    forv_Vec(FnSymbol, fn, gFnSymbols) {

      // 2014/10/17   Noakes and Elliot
      // Dead Block Elimination may convert valid loops to "malformed" loops.
      // Some of these will break BasicBlock construction. Clean them up.
      cleanupLoopBlocks(fn);

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

  INT_ASSERT(fn->basicBlocks);
  INT_ASSERT(fn->basicBlocks->size() > 0);
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

      CondStmt*    condStmt    = toCondStmt(expr->parentExpr);
      DoWhileStmt* doWhileStmt = toDoWhileStmt(expr->parentExpr);
      WhileStmt*   whileStmt   = toWhileStmt(expr->parentExpr);
      ForLoop*     forLoop     = toForLoop(expr->parentExpr);

      if (condStmt && condStmt->condExpr == expr)
        // If the expr is the condition expression of an if statement,
        // then remove the entire if. (NOTE 1)
        condStmt->remove();

      else if (doWhileStmt && doWhileStmt->condExprGet() == expr)
        if (doWhileStmt->length() == 0)
          doWhileStmt->remove();
        else
          // Do nothing. (NOTE 3)
          ;

      else if (whileStmt   && whileStmt->condExprGet()   == expr)
        // If the expr is the condition expression of a while statement,
        // then remove the entire While. (NOTE 1)
        whileStmt->remove();

      else if (forLoop     && forLoop->indexGet()         == expr)
        // Do nothing. (NOTE 2)
        ;

      else if (forLoop     && forLoop->iteratorGet()      == expr)
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
void verifyRemovedIterResumeGotos() {
  forv_Vec(LabelSymbol, labsym, removedIterResumeLabels) {
    if (!isAlive(labsym) && isAlive(labsym->iterResumeGoto))
      INT_FATAL("unexpected live goto for a dead removedIterResumeLabels label - missing a call to removeDeadIterResumeGotos?");
  }
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

  collect_stmts(fn->body, stmts);

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
//# 3. Even if the condition of a DoWhileLoop is dead, the loop cannot be
//#    removed because a DoWhileLoop must execute at least once. We _could_
//#    remove the condition variable, but the compiler expects a non-null expr
//#    to be there. However, if the loop body is empty, we can remove it.
