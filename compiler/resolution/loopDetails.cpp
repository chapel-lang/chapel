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

#include "loopDetails.h"

#include "ForallStmt.h"
#include "ForLoop.h"
#include "resolution.h"

/* This file implements functions allowing
   late resolution passes (late const checking/cull over references
   and lifetime analysis) to reason about iterators and iteration
   variables for loops.
 */

static const IteratorDetails emptyDetails;

bool isChplIterOrLoopIterator(Symbol* sym, ForLoop*& loop)
{
  if (sym->hasFlag(FLAG_CHPL__ITER))
    return true;

  Symbol* checkSym = sym;
  Symbol* nextSym = NULL;

  while (checkSym) {
    nextSym = NULL;

    // Check if checkSym is used in a SymExpr in ForLoop
    for_SymbolSymExprs(se, checkSym) {
      if (ForLoop* forLoop = toForLoop(se->parentExpr))
        if (forLoop->iteratorGet()->symbol() == checkSym) {
          loop = forLoop;
          return true;
        }

      if ((checkSym->hasFlag(FLAG_EXPR_TEMP) &&
           checkSym->type->symbol->hasFlag(FLAG_TUPLE))
          || checkSym->type->symbol->hasFlag(FLAG_ITERATOR_CLASS)) {
        // Check for normalized form of this code
        //   sym = build_tuple(...)
        //   _iterator = _getIteratorZip( sym )
        // or
        //   sym = _getIterator(...)
        //   _iterator = build_tuple( ... sym ... )
        //
        // in that case, we want to continue until we find the
        // iterator variable.
        if (CallExpr* parentCall = toCallExpr(se->parentExpr))
          if (CallExpr* move = toCallExpr(parentCall->parentExpr))
            if (move->isPrimitive(PRIM_MOVE))
              if (SymExpr* lhs = toSymExpr(move->get(1)))
                nextSym = lhs->symbol();
      }
    }
    checkSym = nextSym;
  }

  return false;
}

// Get the non-fast-follower Follower
static
ForLoop* findFollowerForLoop(BlockStmt* block) {
  ForLoop* ret = NULL;
  Expr* e = block->body.first();
  while (e) {
    if (ForLoop* forLoop = toForLoop(e)) {
      return forLoop;
    }
    if (BlockStmt* blk = toBlockStmt(e)) {
      ret = findFollowerForLoop(blk);
      if (ret) return ret;
    }
    if (CondStmt* cond = toCondStmt(e)) {
      // Ignore error handling blocks
      CallExpr* call = toCallExpr(cond->condExpr);
      if (call == NULL || !call->isPrimitive(PRIM_CHECK_ERROR)) {
        // Look in the else block to find the non-fast-follower
        // in case it is decided at run-time whether fast
        // followers can be used.
        ret = findFollowerForLoop(cond->elseStmt);
        if (ret) return ret;
      }
    }
    e = e->next;
  }
  return NULL;
}

/* Given an expression, find the preceding variable
   declared in the same block that is marked with FLAG_CHPL__ITER,
   or NULL if no such variable exists.
 */
static Symbol* findPrecedingChplIter(Expr* ref)
{
  Symbol* chpl_iter = NULL;
  Expr* e = ref->prev;
  while (e) {
    if (DefExpr* d = toDefExpr(e)) {
      Symbol* var = d->sym;
      if (var->hasFlag(FLAG_CHPL__ITER)) {
        chpl_iter = var;
        break;
      }
    } else if (isForallStmt(e)) {
      // This ForallStmt has its own set of variables.
      // Don't look at those.
      break;
    }
    e = e->prev;
  }

  return chpl_iter;
}

static Symbol* findNewIterLF(Expr* ref);

/* Given chpl_iter for a "new-style" forall loop ie generated from ForallStmt,
   find the corresponding chpl__iterLF variable, if it exists.
   That's because it has the corresponding _build_tuple call that
   we can extract information from.
 */
static Symbol* findNewIterLF(Symbol* chpl_iter) {
  INT_ASSERT(!strcmp(chpl_iter->name, "chpl__iterPAR"));
  return findNewIterLF(chpl_iter->defPoint);
}

static Symbol* findNewIterLF(Expr* ref) {
  Symbol* chpl_iter = findPrecedingChplIter(ref);

  if (chpl_iter) {
    INT_ASSERT(!strcmp(chpl_iter->name, "chpl__iterLF"));
  }

  return chpl_iter;
}

/* Collapse compiler-introduced copies of references
   to variables marked "index var"
   This handles chpl__saIdxCopy
 */
static
Symbol* collapseIndexVarReferences(Symbol* index)
{
  bool changed;
  do {
    changed = false;
    for_SymbolSymExprs(se, index) {
      if (CallExpr* move = toCallExpr(se->parentExpr)) {
        if (move->isPrimitive(PRIM_MOVE)) {
          SymExpr* lhs = toSymExpr(move->get(1));
          SymExpr* rhs = toSymExpr(move->get(2));
          INT_ASSERT(lhs && rhs); // or not normalized
          if (lhs->symbol()->hasFlag(FLAG_INDEX_VAR) &&
              rhs->symbol() == index) {
            changed = true;
            index = lhs->symbol();
          }
        }
      }
    }
  } while (changed == true);

  return index;
}

/* Return whatever is producing the Symbol iterator.
   If it was PRIM_MOVE'd from a FLAG_TEMP variable,
     return whatever produced that variable
   If it was the result of a call, return that call
   If it was the result of a simple PRIM_MOVE from a user variable,
     return a SymExpr for that user variable

   This function handles the post-ReturnByRef variants of
   call returns.
 */
static Expr* findExprProducing(Symbol* iterator) {

  if (isArgSymbol(iterator)) return NULL;

  Expr* initExpr = iterator->getInitialization();

  if (CallExpr* initCall = toCallExpr(initExpr)) {
    // Handle the PRIM_MOVE case (pre-ReturnByRef / not for a record)
    if (initCall->isPrimitive(PRIM_MOVE) ||
        initCall->isPrimitive(PRIM_ASSIGN)) {
      if (CallExpr* rhsCall = toCallExpr(initCall->get(2))) {
        return rhsCall;
      } else if (SymExpr* rhsSe = toSymExpr(initCall->get(2))) {

        SymExpr* lhsSe = toSymExpr(initCall->get(1));
        // skip a 'ret_tmp'
        if (rhsSe->symbol()->hasFlag(FLAG_TEMP) &&
            lhsSe->symbol()->getSingleDef() != NULL) {
          return findExprProducing(rhsSe->symbol());
        } else {
          return rhsSe;
        }
      }
    } else if (initCall->resolvedOrVirtualFunction()) {
      // Handle the initializer / ReturnByRef RETARG case
      return initCall;
    }
  }

  return NULL;
}


/* Sets detailsVector[i].index if possible
   Handles syntactically unpacked tuples such as

   for (a, b) in zip(A, B) { ... }
 */
static
void findZipperedIndexVariables(Symbol* index, std::vector<IteratorDetails>&
    detailsVector)
{
  // Now, in a zippered-for, the index is actually
  // a tuple (of references typically). We need to find the
  // un-packed elements.

  for_SymbolSymExprs(indexSe, index) {
    if (CallExpr* indexSeParentCall =
        toCallExpr(indexSe->parentExpr)) {
      if (indexSeParentCall->isPrimitive(PRIM_GET_MEMBER) ||
          indexSeParentCall->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
        AggregateType* tupleType = toAggregateType(index->type);

        if (CallExpr* gpCall =
            toCallExpr(indexSeParentCall->parentExpr)) {
          if (gpCall->isPrimitive(PRIM_MOVE)) {
            SymExpr* lhsSe = toSymExpr(gpCall->get(1));
            if (lhsSe && lhsSe->symbol()->hasFlag(FLAG_INDEX_VAR)) {
              SymExpr* gottenFieldSe = toSymExpr(indexSeParentCall->get(2));
              int i = 0;
              for_fields(field, tupleType) {
                if (gottenFieldSe->symbol() == field) {
                  // setting .index twice probably means we are working
                  // with some new & different AST
                  INT_ASSERT(!detailsVector[i].index);
                  detailsVector[i].index = lhsSe->symbol();
                }
                i++;
              }
            }
          }
        }
      }
    }
  }

  // Set any detailsVector[i].index we didn't figure out to
  // the index variable's tuple element.
  if (index->type->symbol->hasFlag(FLAG_TUPLE)) {
    for(size_t i = 0; i < detailsVector.size(); i++) {
      if (detailsVector[i].index == NULL) {
        detailsVector[i].index = index;
        detailsVector[i].indexTupleElement = i+1;
      }
    }
  }
}

/* Gather important information about a loop.

leaderDetails is only relevant for leader/follower iterators
              and in that case refers to details of the leader loop.
followerForLoop is the follower ForLoop in leader/follower iteration.

When considering zippered iteration, detailsVector contains details of
the individually zippered iterations.

When considering non-zippered iteration, detailsVector contains exactly
one element. It stores information
about the loop. In leader/follower loops, it contains information about
the follower loop.

Always uses the non-fast-follower version of the follower loop.
 */
void gatherLoopDetails(ForLoop*  forLoop,
                       bool&     isForall,
                       IteratorDetails& leaderDetails,
                       ForLoop*& followerForLoop,
                       std::vector<IteratorDetails>& detailsVector)
{
  Symbol* index = forLoop->indexGet()->symbol();

  // TODO -- can we use flags for these?
  bool isFollower = (0 == strcmp(index->name, "chpl__followIdx") ||
                     0 == strcmp(index->name, "chpl__fastFollowIdx"));
  bool isLeader = (0 == strcmp(index->name, "chpl__leadIdx"));
  bool isPar = (0 == strcmp(index->name, "chpl__parIdx"));

  if (isFollower) {
    // Find the leader loop and run the analysis on that.

    Expr* inExpr = forLoop->parentExpr;
    while (inExpr) {
      if (ForLoop* forLoop = toForLoop(inExpr)) {
        gatherLoopDetails(forLoop,
                      isForall, leaderDetails, followerForLoop, detailsVector);
        return;
      }
      if (ForallStmt* forall = toForallStmt(inExpr)) {
        gatherLoopDetails(forall,
                      isForall, leaderDetails, followerForLoop, detailsVector);
        return;
      }
      inExpr = inExpr->parentExpr;
    }
    INT_ASSERT(false); // couldn't find leader ForLoop or ForallStmt
  }


  Symbol* iterator = forLoop->iteratorGet()->symbol();

  // find the variable marked with "chpl__iter" variable
  // in the loop header.
  Symbol* chpl_iter = findPrecedingChplIter(forLoop);

  bool forall = (chpl_iter != NULL);
  // MPF: should be the same as isLoweredForallLoop but it isn't yet
  //INT_ASSERT(forall == forLoop->isLoweredForallLoop());
  bool zippered = forLoop->zipperedGet() &&
                  (iterator->type->symbol->hasFlag(FLAG_TUPLE) ||
                   (chpl_iter != NULL &&
                    chpl_iter->type->symbol->hasFlag(FLAG_TUPLE)));

  // Adjust for new-style forall loops - the counterpart of chpl_iter.
  Symbol* newIterLF = (forall && isPar) ? findNewIterLF(chpl_iter) : NULL;
  if (newIterLF) {
    isLeader = true;
    if (SymExpr* useSE = newIterLF->getSingleUse())
      if (CallExpr* useCall = toCallExpr(useSE->parentExpr))
        if (useCall->isNamed("_toFollowerZip"))
          zippered = true;
  }

  isForall = forall;
  detailsVector.clear();

  if (!forall) {
    // Handle for loops first
    if (! zippered) {
      // simple case of serial, non-zippered iteration
      // i.e. a non-zippered for loop
      // Find the PRIM_MOVE setting iterator
      SymExpr* def = iterator->getSingleDef();
      Expr* iterable = NULL;
      if (def) {
        CallExpr* move = toCallExpr(def->parentExpr);
        INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
        CallExpr* getIteratorCall = toCallExpr(move->get(2));
        INT_ASSERT(getIteratorCall);
        if (getIteratorCall->numActuals() >= 1)
          iterable = getIteratorCall->get(1);
      }

      // Collapse compiler-introduced copies of references
      // to variables marked "index var"
      // This handles chpl__saIdxCopy
      index = collapseIndexVarReferences(index);

      // The thing being iterated over is the argument to getIterator
      IteratorDetails details;
      details.iterable = iterable;
      details.index = index;
      details.iteratorClass = iterator->type;
      details.iterator = getTheIteratorFn(details.iteratorClass);

      IteratorDetails emptyDetails;
      leaderDetails = emptyDetails;
      followerForLoop = NULL;
      detailsVector.push_back(details);
      return;
    } else {
      // serial, zippered iteration
      // i.e. a zippered for loop
      // Find the call to _build_tuple

      // zippered serial iteration has this pattern:
      //   call_tmp1 = _getIterator(a)
      //   call_tmp2 = _getIterator(b)
      //   _iterator = _build_tuple(call_tmp1, call_tmp2)
      //
      // but promoted iteration sometimes has this pattern:
      //   call_tmp = build_tuple(a,b)
      //   _iterator = _getIteratorZip(call_tmp)
      //
      // or
      //
      //   call_tmp = build_tuple(a, b)
      //   p_followerIterator = _toFollowerZip(call_tmp)
      //   _iterator = _getIteratorZip(p_followerIterator)

      SymExpr* tupleIterator = NULL;
      CallExpr* call = toCallExpr(findExprProducing(iterator));
      FnSymbol* calledFn = call?call->resolvedOrVirtualFunction():NULL;
      if (calledFn && !calledFn->hasFlag(FLAG_BUILD_TUPLE)) {
        // expecting call is e.g. _getIteratorZip
        SymExpr* otherSe = toSymExpr(call->get(1));
        INT_ASSERT(otherSe);
        CallExpr* otherCall = toCallExpr(findExprProducing(otherSe->symbol()));
        if (otherCall) {
          call = otherCall;
          calledFn = call->resolvedOrVirtualFunction();
          if (calledFn && !calledFn->hasFlag(FLAG_BUILD_TUPLE)) {
            // expecting call is e.g. _toFollowerZip
            SymExpr* anotherSe = toSymExpr(call->get(1));
            INT_ASSERT(anotherSe);
            CallExpr* anotherCall =
              toCallExpr(findExprProducing(anotherSe->symbol()));
            if (anotherCall) {
              call = anotherCall;
            } else {
              call = NULL;
              tupleIterator = otherSe;
            }
          }
        } else {
          call = NULL;
          tupleIterator = otherSe;
        }
      }

      CallExpr* buildTupleCall = call;
      FnSymbol* buildTupleFn   = NULL;

      if (buildTupleCall) {
        buildTupleFn = buildTupleCall->resolvedOrVirtualFunction();
      }

      if (buildTupleFn && buildTupleFn->hasFlag(FLAG_BUILD_TUPLE)) {

        // build up the detailsVector
        for_formals_actuals(formal, actual, buildTupleCall) {
          // Ignore the RETARG
          if (formal->hasFlag(FLAG_RETARG))
            continue;

          SymExpr* actualSe = toSymExpr(actual);
          INT_ASSERT(actualSe); // otherwise not normalized
          // Find the single definition of actualSe->var to find
          // the call to _getIterator.
          Expr* iterable = actualSe;
          if (actualSe->symbol()->hasFlag(FLAG_EXPR_TEMP)) {
            Symbol* tmpStoringGetIterator = actualSe->symbol();
            Expr* p = findExprProducing(tmpStoringGetIterator);

            // Look for a call to _getIterator that we can ignore
            // If we don't find it, leave iterable set to actualSe from above.
            iterable = actualSe;
            if (p)
              if (CallExpr* call = toCallExpr(p))
                if (call->numActuals() >= 1)
                  if (FnSymbol* fn = call->resolvedOrVirtualFunction())
                    if (fn->hasFlag(FLAG_FN_RETURNS_ITERATOR) ||
                        fn->retType->symbol->hasFlag(FLAG_ITERATOR_CLASS))
                      iterable = call->get(1);
          }
          IteratorDetails details;
          details.iterable = iterable;

          detailsVector.push_back(details);
        }
      } else {
        INT_ASSERT(tupleIterator);
        // Can't find build_tuple call, so fall back on
        // storing tuple elements in iterator details.
        AggregateType* tupleItType = toAggregateType(tupleIterator->typeInfo());
        if (tupleItType->symbol->hasFlag(FLAG_TUPLE)) {
          int i = 0;
          for_fields(field, tupleItType) {
            IteratorDetails details;
            details.iterable = tupleIterator;
            details.iterableTupleElement = i+1;
            detailsVector.push_back(details);

            i++;
          }
        }
      }

      // Figure out iterator class of zippered followers from
      // the tuple type.
      AggregateType* tupleType = toAggregateType(iterator->type);
      int i = 0;
      for_fields(field, tupleType) {
        detailsVector[i].iteratorClass = field->type;
        detailsVector[i].iterator =
          getTheIteratorFn(detailsVector[i].iteratorClass);

        i++;
      }

      // Now, in a zippered-for, the index is actually
      // a tuple (of references typically). We need to find the
      // un-packed elements.

      findZipperedIndexVariables(index, detailsVector);

      IteratorDetails emptyDetails;
      leaderDetails = emptyDetails;
      followerForLoop = NULL;
      return;
    }
  } else {
    // Handle forall loops

    // It could be:
    // standalone iterator that is not zippered
    // leader-follower loop that is not zippered
    // leader-follower loop that is zippered

    if (!isLeader) {
      // parallel, non-zippered standalone
      // ie forall using standalone iterator
      // Find the call setting iterator
      Expr* iterable = findExprProducing(chpl_iter);
      INT_ASSERT(iterable);

      // Collapse compiler-introduced copies of references
      // to variables marked "index var"
      // This handles chpl__saIdxCopy
      index = collapseIndexVarReferences(index);

      IteratorDetails details;
      details.iterable = iterable;
      details.index = index;
      details.iteratorClass = iterator->type;
      details.iterator = getTheIteratorFn(details.iteratorClass);

      IteratorDetails emptyDetails;
      leaderDetails = emptyDetails;
      followerForLoop = NULL;
      detailsVector.push_back(details);
      return;
    } else {

      // Leader-follower iteration

      // Find the iterables

      Expr* iterable = findExprProducing(newIterLF ? newIterLF : chpl_iter);
      INT_ASSERT(iterable);

      if (!zippered) {
        // this is for non-zippered leader-follower iteration
        IteratorDetails details;
        details.iterable = iterable;
        // Other details set below.
        detailsVector.push_back(details);
      } else {
        CallExpr* buildTupleCall = toCallExpr(iterable);
        INT_ASSERT(buildTupleCall && buildTupleCall->resolvedOrVirtualFunction());
        // build up the detailsVector
        for_formals_actuals(formal, actual, buildTupleCall) {
          // Ignore the RETARG
          if (formal->hasFlag(FLAG_RETARG))
            continue;

          SymExpr* actualSe = toSymExpr(actual);
          INT_ASSERT(actualSe); // otherwise not normalized
          // actualSe is the iterable in this case
          IteratorDetails details;
          details.iterable = actualSe;
          // Other details set below.
          detailsVector.push_back(details);
        }
      }

      leaderDetails.iterable = detailsVector[0].iterable;
      leaderDetails.index = index;
      leaderDetails.iteratorClass = iterator->typeInfo();
      leaderDetails.iterator = getTheIteratorFn(leaderDetails.iteratorClass);

      ForLoop* followerFor = findFollowerForLoop(forLoop);
      INT_ASSERT(followerFor);
      followerForLoop = followerFor;

      // Set the detailsVector based upon the follower loop
      Symbol* followerIndex = followerFor->indexGet()->symbol();
      Symbol* followerIterator = followerFor->iteratorGet()->symbol();

      if (!zippered) {
        followerIndex = collapseIndexVarReferences(followerIndex);
        detailsVector[0].index = followerIndex;
        detailsVector[0].iteratorClass = followerIterator->typeInfo();
        detailsVector[0].iterator = getTheIteratorFn(detailsVector[0].iteratorClass);
      } else {
        // Set detailsVector[i].index
        findZipperedIndexVariables(followerIndex, detailsVector);

        // Figure out iterator class of zippered followers from
        // the tuple type.
        AggregateType* tupleType = toAggregateType(followerIterator->type);
        int i = 0;
        for_fields(field, tupleType) {
          detailsVector[i].iteratorClass = field->type;
          detailsVector[i].iterator =
            getTheIteratorFn(detailsVector[i].iteratorClass);

          i++;
        }
      }
      return;
    }
  }
}


//
// Same as above except for a ForallStmt.
// Like for forall case above, this could be:
//  * standalone iterator, which is not zippered
//  * leader-follower loop that is not zippered
//  * leader-follower loop that is zippered

void gatherLoopDetails(ForallStmt* fs,
                       bool&     isForall,
                       IteratorDetails& leaderDetails,
                       ForLoop*& followerForLoop,
                       std::vector<IteratorDetails>& detailsVector)
{
  bool isLeader = false;
  bool zippered = false;

  Symbol* newIterLF = findNewIterLF(fs);
  // copied from the other gatherLoopDetails() TODO -- can we remove?
  if (newIterLF) {
    isLeader = true;
    if (SymExpr* useSE = newIterLF->getSingleUse())
      if (CallExpr* useCall = toCallExpr(useSE->parentExpr))
        if (useCall->isNamed("_toFollowerZip"))
          zippered = true;
  }
  if (fs->zippered())
    zippered = true;

  INT_ASSERT(isLeader ==
             !strcmp(parIdxVar(fs)->name, "chpl_followThis"));

  isForall = true;
  detailsVector.clear();

  {
    // Todo: factor our shared code with the other gatherLoopDetails()?
    // Handle forall loops

    // It could be:
    // standalone iterator that is not zippered
    // leader-follower loop that is not zippered
    // leader-follower loop that is zippered

    if (!isLeader)
    {
      IteratorDetails detailsSA;
      detailsSA.iterable = fs->iteratedExpressions().head;
      detailsSA.index = parIdxVar(fs);
      detailsSA.iteratorClass = NULL;
      detailsSA.iterator = toCallExpr(detailsSA.iterable)->resolvedFunction();

      leaderDetails = emptyDetails;
      followerForLoop = NULL;
      detailsVector.push_back(detailsSA);
      return;
    }
    else
    {
      // Leader-follower iteration

      // Find the iterables
      if (!zippered) {
        SymExpr* def = newIterLF->getSingleDef();
        CallExpr* move = toCallExpr(def->parentExpr);
        INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
        Expr* iterable = move->get(2);
        INT_ASSERT(iterable);
        // Comes up in non-zippered leader-follower iteration
        IteratorDetails details;
        details.iterable = iterable;
        // Other details set below.
        detailsVector.push_back(details);
      } else {
        FnSymbol* buildTupleFn = NULL;
        CallExpr* buildTupleCall = toCallExpr(findExprProducing(newIterLF));
        if (buildTupleCall)
          buildTupleFn = buildTupleCall->resolvedOrVirtualFunction();

        if (buildTupleFn && buildTupleFn->hasFlag(FLAG_BUILD_TUPLE)) {
          // build up the detailsVector
          for_formals_actuals(formal, actual, buildTupleCall) {
            // Ignore the RETARG
            if (formal->hasFlag(FLAG_RETARG))
              continue;

            SymExpr* actualSe = toSymExpr(actual);
            INT_ASSERT(actualSe); // otherwise not normalized
            // actualSe is the iterable in this case
            IteratorDetails details;
            details.iterable = actualSe;
            // Other details set below.
            detailsVector.push_back(details);
          }
        }
      }

      leaderDetails.iterable = detailsVector[0].iterable;
      leaderDetails.index = parIdxVar(fs);
      leaderDetails.iteratorClass = NULL;
      leaderDetails.iterator = toCallExpr(
            fs->iteratedExpressions().head)->resolvedFunction();

      ForLoop* followerFor = findFollowerForLoop(fs->loopBody());
      INT_ASSERT(followerFor);
      followerForLoop = followerFor;

      // Set the detailsVector based upon the follower loop
      Symbol* followerIndex = followerFor->indexGet()->symbol();
      Symbol* followerIterator = followerFor->iteratorGet()->symbol();

      if (!zippered) {
        followerIndex = collapseIndexVarReferences(followerIndex);
        detailsVector[0].index = followerIndex;
        detailsVector[0].iteratorClass = followerIterator->typeInfo();
        detailsVector[0].iterator = getTheIteratorFn(detailsVector[0].iteratorClass);
      } else {
        // Set detailsVector[i].index
        findZipperedIndexVariables(followerIndex, detailsVector);

        // Figure out iterator class of zippered followers from
        // the tuple type.
        AggregateType* tupleType = toAggregateType(followerIterator->type);
        int i = 0;
        for_fields(field, tupleType) {
          detailsVector[i].iteratorClass = field->type;
          detailsVector[i].iterator =
            getTheIteratorFn(detailsVector[i].iteratorClass);

          i++;
        }
      }
      return;
    }
  }
}
