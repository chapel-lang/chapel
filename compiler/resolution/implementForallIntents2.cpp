/*
 * Copyright 2004-2019 Cray Inc.
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

#include "DeferStmt.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "resolution.h"
#include "resolveFunction.h"

#include <map>
#include <vector>

// Todo: merge this file into foralls.cpp ?

//
// If the loop is zippered, ensure there are no parallel iterators.
// Because it is unclear how to execute a for-loop over parallel iterator(s).
// When/if we address that, we need to convert this ForLoop to a ForallStmt.
// Otherwise we may get data races on the shadow variable(s) 'sum' here:
//   forall .... with (+ reduce sum) {
//     for idx in zip(myIter(tag=standalone), anotherIter()) do
//       sum reduce= f(idx);
//   }
//
// If a zippered loop is over a single iterator, we treat is as non-zippered.
//
static void checkZipperedParallelIterators(ForLoop* loop, Type* iterType) {
  for_fields(field, toAggregateType(iterType)) {
    if (is_arithmetic_type(field->type)) continue; // skip 'size'
    FnSymbol* iter = getTheIteratorFn(field->type);
    if (isParallelIterator(iter))
      USR_FATAL_CONT(loop, "A zippered for-loop over parallel iterator(s)"
                           " is currently not implemented");
  }
}

//
// Is 'forLoop' a loop over a parallel iterator?
// If so, fill in 'eInfo' with details.
//
// The current pattern-matching implementation is far from ideal.
// We could improve it by using for_SymbolSymExprs()
// or by having ForLoop include the iterator call directly.
//
static bool invokesParallelIterator(ForLoop* forLoop)
{
  Symbol* iterSym = forLoop->iteratorGet()->symbol();
  if (iterSym->type->symbol->hasFlag(FLAG_TUPLE)) {
    checkZipperedParallelIterators(forLoop, iterSym->type);
    return false;
  }

  FnSymbol* invokedIterator = getTheIteratorFn(iterSym->type);
  return isParallelIterator(invokedIterator);
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

// The ForLoop iterates over an _iteratorClass temp.
// The ForallStmt needs to iterate over the _iteratorRecord temp
// from which the _iteratorClass is obtained.
// Then, we can remove the _iteratorClass temp.
static void transferTheIterable(ForallStmt* dest, ForLoop* src) {
  Symbol* IC = src->iteratorGet()->symbol();

  SymExpr* icDef = IC->getSingleDef();
  CallExpr* icMove = toCallExpr(icDef->parentExpr);
  INT_ASSERT(icMove->isPrimitive(PRIM_MOVE));

  CallExpr* icSrc = toCallExpr(icMove->get(2));
  INT_ASSERT(icSrc->isNamed("_getIterator")    ||
             icSrc->isNamed("_getIteratorZip") );
  INT_ASSERT(icSrc->numActuals() == 1); // a single iterable

  // This is our _iteratorRecord temp.
  Symbol* IR = toSymExpr(icSrc->get(1))->symbol();
  dest->iteratedExpressions().insertAtHead(new SymExpr(IR));

  // Remove the _iteratorClass temp.
  src->iteratorGet()->remove(); // get it out of the way
  icMove->remove();
  for_SymbolSymExprs(icSE, IC)
    removeWithEnclosing(icSE->getStmtExpr());
  IC->defPoint->remove();

  // no more uses
  INT_ASSERT(IC->firstSymExpr() == NULL);
}

static ForallStmt* doReplaceWithForall(ForLoop* src)
{
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
          // todo: does this ever happen?
          if (isMoveToOIOI(idxSE)) gotMoveToOIOI = true;
        }
      }
    }
  }

  INT_ASSERT(onlySingleUse || gotMoveToOIOI);

  transferTheIterable(dest, src);

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

//
// Replace a parallel ForLoop over a parallel iterator with a ForallStmt.
// Otherwise we may get data races, ex. on shadow variable(s) 'sum' here:
//   forall .... with (+ reduce sum) {
//     for idx in myIter(tag=standalone) do
//       sum reduce= f(idx);
//   }
//
Expr* replaceForWithForallIfNeeded(ForLoop* forLoop) {
  if (!invokesParallelIterator(forLoop))
    // Not a parallel for-loop. Leave it unchanged.
    return forLoop;

  // Yes, it is a parallel for-loop. Replace it.
  ForallStmt* fs = doReplaceWithForall(forLoop);

  // If >1 iterated exprs, how to call resolveForallHeader?
  INT_ASSERT(fs->numIteratedExprs() == 1);
  SymExpr* itExpr = toSymExpr(fs->iteratedExpressions().head);

  resolveForallHeader(fs, itExpr);

  return fs;
}
