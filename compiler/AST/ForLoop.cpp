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

#include "ForLoop.h"

#include "astutil.h"
#include "AstVisitor.h"
#include "build.h"
#include "codegen.h"

#include <algorithm>

/************************************ | *************************************
*                                                                           *
* Helper functions to optimize anonymous range iteration                    *
*                                                                           *
************************************* | ************************************/

/*
 * Attempts to replace iteration over simple anonymous ranges with calls to
 * direct iterators that take low, high and stride as arguments. This is to
 * avoid the cost of constructing ranges, and if the stride is known at compile
 * time, provide a more optimized iterator that uses "<, <=, >, or >=" as the
 * relational operator.
 *
 * This is only meant to replace anonymous range iteration for "simple" bounded
 * ranges. Simple means it's a range of the form "low..high" or "low..high by
 * stride". Anything more complex is ignored with the thinking that this should
 * optimize the most common range iterators, but it could be expanded to handle
 * more cases.
 *
 * An alternative is to update scalar replacement of aggregates to work on
 * ranges, which should be able to achieve similar results as this optimization
 * while handling all ranges, including non-anonymous ranges.
 *
 * Will optimize things like:
 * - "for i in 1..10"
 * - "for i in 1..10 by 2"
 * - "for (i, j) in zip(1..10 by 2, 1..10 by -2)"
 * - "for (i, j) in zip(A, 1..10 by 2)" // will optimize range iter still
 * - "coforall i in 1..10 by 2"         // works for coforalls as well
 *
 * Will not optimize ranges like:
 * - "for in in (1..)"             // doesn't handle unbounded ranges
 * - "for i in 1..10 by 2 by 2"    // doesn't handle more than one by operator
 * - "for i in 1..10 align 2"      // doesn't handle align operator
 * - "for i in 1..#10"             // doesn't handle # operator
 * - "var r = 1..10"; for i in r"  // not an anonymous range
 * - "forall i in 1..10"           // does not get applied to foralls
 *
 * Note that this function is pretty fragile because it relies on names of
 * functions/iterators as well as the arguments and order of those
 * functions/iterators but there's not really a way around it this early in
 * compilation. If the iterator can't be replaced the original, unchanged
 * iteratorExpr is returned.
 */
static Expr* tryToUseDirectRangeIterator(Expr* iteratorExpr) {
  CallExpr* range = NULL;
  Expr* stride = NULL;
  if (CallExpr* call = toCallExpr(iteratorExpr)) {
    // grab the stride if we have a strided ranges
    if (call->isNamed("by")) {
      range = toCallExpr(call->get(1)->copy());
      stride = toExpr(call->get(2)->copy());
    } else {
      range = call;
    }
    // see if we're looking at a _build_range for an anonymous range
    if (range && range->isNamed("_build_range")) {
      // just a sanity check, this should always be true
      if (range->numActuals() == 2) {
        Expr* low = range->get(1)->copy();
        Expr* high = range->get(2)->copy();
        // only get bounded ranges, unbounded takes a BoundedRangeType as the
        // first argument, which turns into a CallExpr. This is probably the
        // best way to check for bounded ranges this early in compilation
        if ((isUnresolvedSymExpr(low) || isSymExpr(low)) &&
           (isUnresolvedSymExpr(high) || isSymExpr(high))) {
          // replace the range construction with a direct range iterator
          if (stride) {
            iteratorExpr = (new CallExpr("_direct_range_iter", low, high, stride));
          } else {
            SymExpr* noStr = new SymExpr(new_IntSymbol(1));
            iteratorExpr = (new CallExpr("_direct_range_iter", low, high, noStr));
          }
        }
      }
    }
  }
  return iteratorExpr;
}

static Expr* optimizeRangeIteration(Expr* iteratorExpr, bool zippered) {
  iteratorExpr = tryToUseDirectRangeIterator(iteratorExpr);
  // for zippered iterators, try to replace each iterator of the tuple
  if (zippered) {
    if (CallExpr* call = toCallExpr(iteratorExpr)) {
      if (call->isNamed("_build_tuple")) {
        for_actuals(actual, call) {
          actual->replace(tryToUseDirectRangeIterator(actual->copy()));
        }
      }
    }
  }
  return iteratorExpr;
}

/************************************ | *************************************
*                                                                           *
* Factory methods for the Parser                                            *
*                                                                           *
************************************* | ************************************/

BlockStmt* ForLoop::buildForLoop(Expr*      indices,
                                 Expr*      iteratorExpr,
                                 BlockStmt* body,
                                 bool       coforall,
                                 bool       zippered)
{
  VarSymbol*   index         = newTemp("_indexOfInterest");
  VarSymbol*   iterator      = newTemp("_iterator");
  CallExpr*    iterInit      = 0;
  CallExpr*    iterMove      = 0;
  ForLoop*     loop          = new ForLoop(index, iterator, body);
  LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
  LabelSymbol* breakLabel    = new LabelSymbol("_breakLabel");
  BlockStmt*   retval        = new BlockStmt();

  iteratorExpr = optimizeRangeIteration(iteratorExpr, zippered);

  iterator->addFlag(FLAG_EXPR_TEMP);

  // Unzippered loop, treat all objects (including tuples) the same
  if (zippered == false)
    iterInit = new CallExpr(PRIM_MOVE, iterator, new CallExpr("_getIterator",    iteratorExpr));

  // Expand tuple to a tuple containing appropriate iterators for each value.
  else
    iterInit = new CallExpr(PRIM_MOVE, iterator, new CallExpr("_getIteratorZip", iteratorExpr));

  index->addFlag(FLAG_INDEX_OF_INTEREST);

  iterMove = new CallExpr(PRIM_MOVE, index, new CallExpr("iteratorIndex", iterator));

  if (indices == 0)
    indices = new UnresolvedSymExpr("chpl__elidedIdx");

  checkIndices(indices);

  destructureIndices(loop, indices, new SymExpr(index), coforall);

  if (coforall)
    index->addFlag(FLAG_COFORALL_INDEX_VAR);

  loop->mContinueLabel = continueLabel;
  loop->mBreakLabel    = breakLabel;

  loop->insertAtTail(new DefExpr(continueLabel));

  retval->insertAtTail(new DefExpr(index));
  retval->insertAtTail(new DefExpr(iterator));

  retval->insertAtTail(iterInit);
  retval->insertAtTail(new BlockStmt(iterMove, BLOCK_TYPE));

  retval->insertAtTail(loop);

  retval->insertAtTail(new DefExpr(breakLabel));
  retval->insertAtTail(new CallExpr("_freeIterator", iterator));

  return retval;
}

/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

ForLoop::ForLoop() : LoopStmt(0)
{
  mIndex    = 0;
  mIterator = 0;
}

ForLoop::ForLoop(VarSymbol* index,
                 VarSymbol* iterator,
                 BlockStmt* initBody) : LoopStmt(initBody)
{
  mIndex    = new SymExpr(index);
  mIterator = new SymExpr(iterator);
}

ForLoop::~ForLoop()
{

}

ForLoop* ForLoop::copy(SymbolMap* mapRef, bool internal)
{
  SymbolMap  localMap;
  SymbolMap* map       = (mapRef != 0) ? mapRef : &localMap;
  ForLoop*   retval    = new ForLoop();

  retval->astloc         = astloc;
  retval->blockTag       = blockTag;

  retval->mBreakLabel    = mBreakLabel;
  retval->mContinueLabel = mContinueLabel;

  retval->mIndex         = mIndex->copy(map, true),
  retval->mIterator      = mIterator->copy(map, true);

  for_alist(expr, body)
    retval->insertAtTail(expr->copy(map, true));

  if (internal == false)
    update_symbols(retval, map);

  return retval;
}

BlockStmt* ForLoop::copyBody()
{
  SymbolMap map;

  return copyBody(&map);
}

BlockStmt* ForLoop::copyBody(SymbolMap* map)
{
  BlockStmt* retval = new BlockStmt();

  retval->astloc   = astloc;
  retval->blockTag = blockTag;

  for_alist(expr, body)
    retval->insertAtTail(expr->copy(map, true));

  update_symbols(retval, map);

  return retval;
}

bool ForLoop::isForLoop() const
{
  return true;
}

SymExpr* ForLoop::indexGet() const
{
  return mIndex;
}

SymExpr* ForLoop::iteratorGet() const
{
  return mIterator;
}

CallExpr* ForLoop::blockInfoGet() const
{
  printf("Migration: ForLoop   %12d Unexpected call to blockInfoGet()\n", id);

  return 0;
}

CallExpr* ForLoop::blockInfoSet(CallExpr* expr)
{
  printf("Migration: ForLoop   %12d Unexpected call to blockInfoSet()\n", id);

  return 0;
}

bool ForLoop::deadBlockCleanup()
{
  bool retval = false;

  INT_ASSERT(false);

  return retval;
}

void ForLoop::verify()
{
  BlockStmt::verify();

  if (BlockStmt::blockInfoGet() != 0)
    INT_FATAL(this, "ForLoop::verify. blockInfo is not NULL");

  if (mIndex    == 0)
    INT_FATAL(this, "ForLoop::verify. index     is NULL");

  if (mIterator == 0)
    INT_FATAL(this, "ForLoop::verify. iterator  is NULL");

  if (modUses   != 0)
    INT_FATAL(this, "ForLoop::verify. modUses   is not NULL");

  if (byrefVars != 0)
    INT_FATAL(this, "ForLoop::verify. byrefVars is not NULL");
}

GenRet ForLoop::codegen()
{
  GenRet ret;

  INT_FATAL(this, "ForLoop::codegen This should be unreachable");

  return ret;
}

void ForLoop::accept(AstVisitor* visitor)
{
  if (visitor->enterForLoop(this) == true)
  {
    for_alist(next_ast, body)
      next_ast->accept(visitor);

    if (indexGet()    != 0)
      indexGet()->accept(visitor);

    if (iteratorGet() != 0)
      iteratorGet()->accept(visitor);

    if (modUses)
      modUses->accept(visitor);

    if (byrefVars)
      byrefVars->accept(visitor);

    visitor->exitForLoop(this);
  }
}

Expr* ForLoop::getFirstExpr()
{
  Expr* retval = 0;

  if (mIndex         != 0)
    retval = mIndex;

  else if (mIterator != 0)
    retval = mIterator;

  else if (body.head != 0)
    retval = body.head->getFirstExpr();

  else
    retval = this;

  return retval;
}

Expr* ForLoop::getNextExpr(Expr* expr)
{
  Expr* retval = this;

  if (expr == mIndex && mIterator != NULL)
    retval = mIterator;

  else if (expr == mIndex    && body.head != NULL)
    retval = body.head->getFirstExpr();

  else if (expr == mIterator && body.head != NULL)
    retval = body.head->getFirstExpr();

  return retval;
}
