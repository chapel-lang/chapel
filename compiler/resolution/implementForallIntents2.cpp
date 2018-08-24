/*
 * Copyright 2004-2018 Cray Inc.
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

#include "implementForallIntents.h"

#include "foralls.h"
#include "resolveFunction.h"

#include <map>
#include <vector>

/////////// EFLOPI: Enclosing For Loop Over Parallel Iterator ///////////

// 
// Info we gather while looking for an eflopi or an enclosing forall loop
//
// The contents are valid only after a call to findEnclosingParallelLoop()
// as follows:
//
//  * 'eflopiForall' and 'eflopiLoop' are always set up
//   -  to the corresponding ForallStmt or ForLoop node if one is found,
//   -  to NULL if neither is found.
//
//  * the other fields are set up only if eflopiLoop != NULL.
//
class EflopiInfo {
public:
  ForallStmt* eflopiForall;
  ForLoop*    eflopiLoop;
  // more details if eflopiLoop
  CallExpr*   eflopiCall;
  CallExpr*   asgnToIter;
  Symbol*     iterCallTemp;
};

//
// Is 'forLoop' a loop over a parallel iterator?
// If so, fill in 'eInfo' with details.
//
// The current pattern-matching implementation is far from ideal.
// We could improve it by using for_SymbolSymExprs()
// or by having ForLoop include the iterator call directly.
//
static bool findCallToParallelIterator(ForLoop* forLoop, EflopiInfo& eInfo)
{
  Symbol* iterSym = forLoop->iteratorGet()->symbol();

  // Find an assignment to 'iterSym'.
  // It is usually located within a short walk before forLoop.
  CallExpr* asgnToIter = NULL;
  for (Expr* curr = forLoop->prev; curr; curr = curr->prev)
    if (CallExpr* call = toCallExpr(curr))
      if (call->isPrimitive(PRIM_MOVE))
        if (SymExpr* lhs1 = toSymExpr(call->get(1)))
          if (lhs1->symbol() == iterSym) {
            asgnToIter = call;
            break;
          }
  INT_ASSERT(asgnToIter);

  // We have:
  //   move( call_tmp call( ITERATOR args... ) )
  //   move( _iterator call( _getIterator call_tmp ) )
  // We need to see if args... contain a leader or standalone tag.
  // 'asgnToIter' is the second of the above moves. Find the first one.
  CallExpr* rhs1 = toCallExpr(asgnToIter->get(2));
  if (rhs1->isNamed("_build_tuple") ||
      rhs1->isNamed("_getIteratorZip"))
    // This is a zippered for-loop. Currently implies not parallel.
    return false;
  INT_ASSERT(rhs1->isNamed("_getIterator"));

  Symbol* calltemp = toSymExpr(rhs1->get(1))->symbol();
  CallExpr* asgnToCallTemp = NULL;
  for (Expr* curr = asgnToIter->prev; curr; curr = curr->prev)
    if (CallExpr* call = toCallExpr(curr))
      if (call->isPrimitive(PRIM_MOVE))
        if (SymExpr* lhs2 = toSymExpr(call->get(1)))
          if (lhs2->symbol() == calltemp) {
            asgnToCallTemp = call;
            break;
          }

  // Sometimes, e.g. forall over a range - ri-5-c.chpl, _getIterator is invoked
  // on a non-temp. We notice this by the absence of asgnToCallTemp.
  // In such a case, the iterator will be nontemp.these(),
  // which is not a parallel iterator.
  if (!asgnToCallTemp)
    return false;

  CallExpr* iterCall = toCallExpr(asgnToCallTemp->get(2));
  INT_ASSERT(iterCall);

  if (callingParallelIterator(iterCall)) {
    // Found it. Fill in the information before returning.
    eInfo.eflopiForall   = NULL;
    eInfo.eflopiLoop     = forLoop;
    eInfo.eflopiCall     = iterCall;
    eInfo.asgnToIter     = asgnToIter;
    eInfo.iterCallTemp   = calltemp;
    return true;
  }

  // no signs of a parallel iterator
  return false;
}

// We need expr->remove().
// If it is the only thing in its enclosing block, yank the block.
// Ditto the defer statement.
static void removeWithEnclosing(Expr* expr) {
  if (BlockStmt* block = toBlockStmt(expr->parentExpr))
    if (expr == block->body.head && expr == block->body.tail)
      {
        // Remove the entire block. Check for an enclosing defer first.
        if (DeferStmt* defer = toDeferStmt(block->parentExpr)) {
          defer->remove();
          return;
        }
        INT_ASSERT(block->parentExpr != NULL);
        block->remove();
        return;
      }
  expr->remove();
  return;
}

// Is this being moved to 'origIndexOfInterest' ?
static bool isMoveToOIOI(SymExpr* se) {
  if (CallExpr* parent1 = toCallExpr(se->parentExpr))
    if (parent1->isPrimitive(PRIM_GET_MEMBER_VALUE))
      if (CallExpr* parent2 = toCallExpr(parent1->parentExpr))
        if (parent2->isPrimitive(PRIM_MOVE))
          if (SymExpr* dest = toSymExpr(parent2->get(1)))
            if (!strcmp(dest->symbol()->name, "origIndexOfInterest"))
              return true;
  return false;
}

static ForallStmt* replaceEflopiWithForall(EflopiInfo& eInfo)
{
  // the loop to be replaced
  ForLoop* src  = eInfo.eflopiLoop;

  // Otherwise currentAstLoc points to the forall loop
  // that invokes the iterator src->parentSymbol.
  SET_LINENO(src);

  // the replacement loop
  ForallStmt* dest = ForallStmt::fromForLoop(src);

  // convert src index variable to dest index variable
  // todo instead use the corresponding user variable
  Symbol* srcIndex = src->indexGet()->symbol();
  src->indexGet()->remove(); // get it out of the way
  dest->inductionVariables().insertAtHead(srcIndex->defPoint->remove());

  // For assertions only.
  // Should not be needed once we eliminate _toLeader, _toStandalone.
  bool onlySingleUse = true;
  bool gotMoveToOIOI = false;

  // remove its uses outside the loop
  for_SymbolSymExprs(idxSE, srcIndex) {
    if (Expr* idxStmt = idxSE->getStmtExpr()) {
      // heuristic, instead of checking the full chain of parentExpr
      // todo soon: instead of heuristic, leverage getSingleDef()
      if (idxStmt->parentExpr != src) {
        INT_ASSERT(idxSE == srcIndex->getSingleDef());
        // todo soon: given that the above always holds, use it
        // and simplify the body of this for_SymbolSymExprs
        INT_ASSERT(isBlockStmt(idxStmt->parentExpr));
        INT_ASSERT(toBlockStmt(idxStmt->parentExpr)->body.length == 1);
        removeWithEnclosing(idxStmt);
      } else {
        if (idxSE == srcIndex->getSingleUse()) {
          // great
        } else {
          // happens for eflopies in extendLeader()-ed iterators
          if (isMoveToOIOI(idxSE)) gotMoveToOIOI = true;
        }
      }
    }
  }

  INT_ASSERT(onlySingleUse || gotMoveToOIOI);

  // set the iterator call
  dest->iteratedExpressions().insertAtHead(new SymExpr(eInfo.iterCallTemp));

  // remove _iterator that keeps the iterator class
  Symbol* IC = src->iteratorGet()->symbol();
  src->iteratorGet()->remove(); // get it out of the way
  for_SymbolSymExprs(icSE, IC)
    removeWithEnclosing(icSE->getStmtExpr());
  IC->defPoint->remove();

  // no more uses
  INT_ASSERT(IC->firstSymExpr() == NULL);

  // no with-clause adjustments - the for loop does not have one

  // transfer the loop body
  AList& sourceBody = src->body;
  AList& resultBody = dest->loopBody()->body;
  while (Expr* forStmt = sourceBody.head)
    resultBody.insertAtTail(forStmt->remove());

  // replace the loop statement itself
  src->replace(dest);

  return dest;
}

// Replace a parallel ForLoop with a ForallStmt.
Expr* replaceForWithForallIfNeeded(ForLoop* forLoop) {
  EflopiInfo eflopiInfo;

  if (!findCallToParallelIterator(forLoop, eflopiInfo))
    // Not a parallel for-loop. Leave it unchanged.
    return forLoop;

  // Yes, it is a parallel for-loop. Replace it.
  // findCallToParallelIterator() filled out eflopiInfo.
  ForallStmt* fs = replaceEflopiWithForall(eflopiInfo);

  // If >1 iterated exprs, how to call resolveForallHeader?
  INT_ASSERT(fs->numIteratedExprs() == 1);
  SymExpr* itExpr = toSymExpr(fs->iteratedExpressions().head);

  resolveForallHeader(fs, itExpr);

  return fs;
}
