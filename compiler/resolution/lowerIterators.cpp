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

#include "optimizations.h"

#include "astutil.h"
#include "CForLoop.h"
#include "driver.h"
#include "errorHandling.h"
#include "expr.h"
#include "ForLoop.h"
#include "iterator.h"
#include "oldCollectors.h"
#include "passes.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "view.h"
#include "wellknown.h"

bool iteratorsLowered = false;

//
// getTheIteratorFn(): get the original (user-written) iterator function
// that corresponds to an _iteratorClass type or symbol
// or a CallExpr therewith.
//

// 'ic' must be an instance of _iteratorClass
FnSymbol* getTheIteratorFn(Symbol* ic) {
  return getTheIteratorFn(ic->type);
}

FnSymbol* getTheIteratorFn(CallExpr* call) {
  return getTheIteratorFn(call->get(1)->typeInfo());
}

// icType is either an _iteratorClass type or a tuple thereof
// When icType is a tuple, this function returns
//  the getIterator function for the first tuple element.
// When icType is an _iteratorClass, this function returns
//  the original iterator function.
FnSymbol* getTheIteratorFn(Type* icType)
{
  // the asserts document the current state
  AggregateType* icTypeAgg = toAggregateType(icType);
  INT_ASSERT(icTypeAgg);

  if (icType->symbol->hasFlag(FLAG_TUPLE)) {
    FnSymbol* getIterFn = icTypeAgg->defaultInitializer;
    // A tuple of iterator classes -> first argument to
    // tuple constructor is the iterator class type.
    Type* firstIcType = getIterFn->getFormal(1)->type;
    INT_ASSERT(firstIcType->symbol->hasFlag(FLAG_ITERATOR_CLASS));
    AggregateType* firstIcTypeAgg = toAggregateType(firstIcType);
    FnSymbol* result = firstIcTypeAgg->iteratorInfo->getIterator;

    return result;
  } else {
    INT_ASSERT(icType->symbol->hasFlag(FLAG_ITERATOR_CLASS));
    INT_ASSERT(icTypeAgg->iteratorInfo);
    FnSymbol* getIterFn = icTypeAgg->iteratorInfo->getIterator;
    // The type of _getIterator's first formal arg is _iteratorRecord.
    Type* irType = getIterFn->getFormal(1)->type;
    INT_ASSERT(irType->symbol->hasFlag(FLAG_ITERATOR_RECORD));

    AggregateType* irTypeAgg = toAggregateType(irType);
    INT_ASSERT(irTypeAgg->iteratorInfo);

    // Return the original iterator function
    FnSymbol* result = irTypeAgg->iteratorInfo->iterator;
    INT_ASSERT(result->hasFlag(FLAG_ITERATOR_FN));

    return result;
  }
}

FnSymbol* debugGetTheIteratorFn(Type* type) {
  FnSymbol* result = NULL;
  if (AggregateType* agg = toAggregateType(type)) {
    if (agg->symbol->hasFlag(FLAG_TUPLE))
      result = getTheIteratorFn(agg);
    else if (agg->symbol->hasFlag(FLAG_ITERATOR_CLASS))
      result = getTheIteratorFn(agg);
    else if (agg->symbol->hasFlag(FLAG_ITERATOR_RECORD))
      if (IteratorInfo* ii = agg->iteratorInfo)
        result = ii->iterator;
  }
  if (result && result->hasFlag(FLAG_AUTO_II))
    if (AggregateType* rettype = toAggregateType(result->retType))
      if (FnSymbol* newresult = debugGetTheIteratorFn(rettype))
        result = newresult;
  return result;
}

FnSymbol* debugGetTheIteratorFn(ForLoop* forLoop) {
  if (forLoop)
    if (SymExpr* iteratorSE = forLoop->iteratorGet())
      return debugGetTheIteratorFn(iteratorSE->symbol()->type);
  // otherwise
  return NULL;
}


// This consistency check should probably be moved earlier in the compilation.
// It needs to be after resolution because it sets FLAG_INLINE_ITERATOR.
// Does it need to be recursive? (Currently, it is not.)
static void nonLeaderParCheckInt(FnSymbol* fn, bool allowYields);

static void nonLeaderParCheck()
{
  //
  // check for parallel constructs in non-leader iterators
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->isIterator() && !fn->hasFlag(FLAG_INLINE_ITERATOR)) {
      nonLeaderParCheckInt(fn, true);
    }
  }
  USR_STOP();
}

static bool isVirtualIterator(Symbol* iterator) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(iterator->type)) {
    Vec<AggregateType*>* children = &(at->dispatchChildren);

    if (children->n == 0) {
      retval = false;

    } else if (children->n == 1 && children->v[0] == dtObject) {
      retval = false;

    } else {
      retval = true;
    }
  }

  return retval;
}

static void parallelIterVirtualCheck() {
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (isAlive(block) && block->isForLoop()) {
      ForLoop* forLoop = toForLoop(block);
      Symbol* iterator = toSymExpr(forLoop->iteratorGet())->symbol();
      FnSymbol* ifn = getTheIteratorFn(iterator);
      if (ifn->hasFlag(FLAG_INLINE_ITERATOR) && isVirtualIterator(iterator)) {
        USR_FATAL_CONT(forLoop, "virtual parallel iterators are not yet supported (see issue #6998)");
      }
    }
  }
  USR_STOP();
}

static void nonLeaderParCheckInt(FnSymbol* fn, bool allowYields)
{
  std::vector<CallExpr*> calls;

  collectCallExprs(fn, calls);

  for_vector(CallExpr, call, calls) {
    if ((call->isPrimitive(PRIM_BLOCK_BEGIN)) ||
        (call->isPrimitive(PRIM_BLOCK_COBEGIN)) ||
        (call->isPrimitive(PRIM_BLOCK_COFORALL))) {
      // begin/cobegin/coforall *blocks* are eliminated earlier.
      // If they are not, need issue the USR_FATAL_CONT like below.
      INT_ASSERT(false);
    }

    FnSymbol* taskFn = resolvedToTaskFun(call);

    bool isParallelConstruct = taskFn &&
                               (taskFn->hasFlag(FLAG_BEGIN) ||
                                taskFn->hasFlag(FLAG_COBEGIN_OR_COFORALL));

    if (isParallelConstruct ||
        call->isNamed("_toLeader") ||
        call->isNamed("_toStandalone")) {
      USR_FATAL_CONT(call,
                     "invalid use of parallel construct in serial iterator");
    }

    if ((call->isPrimitive(PRIM_BLOCK_ON)) ||
        (call->isPrimitive(PRIM_BLOCK_BEGIN_ON)) ||
        (call->isPrimitive(PRIM_BLOCK_COBEGIN_ON)) ||
        (call->isPrimitive(PRIM_BLOCK_COFORALL_ON))) {
      // begin/cobegin/coforall *blocks* are eliminated earlier.
      // If they are not, check for PRIM_YIELD like below.
      INT_ASSERT(false);
    }
    if (!allowYields) {
      if (call->isPrimitive(PRIM_YIELD)) {
        USR_FATAL_CONT(call, "invalid use of 'yield' within 'on' in serial iterator");
      }
    }
    if (taskFn) {
      // This used to be the body of the parallel or 'on' construct
      // so need to descend into it.
      nonLeaderParCheckInt(taskFn, !taskFn->hasFlag(FLAG_ON));
    }
  }
}


// Traverse all callers of this function, to see if it calls the base iterator recursively.
// Return true if so; false otherwise.
// This version uses a depth-first search.
static bool find_recursive_caller(FnSymbol* fn, FnSymbol* iter, Vec<FnSymbol*>& fnSet)
{
  // We've already seen this fn, and if we're still searching it means
  // we didn't return true last time, so return false this time too.
  if (fnSet.set_in(fn))
    return false;

  // OK, first time.  Mark this function as visited.
  fnSet.set_add(fn);

  // Traverse all its call sites.
  forv_Vec(CallExpr, call, *fn->calledBy)
  {
    // Extract the symbol representing the calling function.
    FnSymbol* caller = toFnSymbol(call->parentSymbol);

    // If the caller is the same as the iterator we started with,
    // it calls itself recursively.  Further searching is unnecessary.
    if (caller == iter)
      return true;

    // Otherwise, search recursively up the call chain.
    // If recursion was detected, no further searching is required.
    if (find_recursive_caller(caller, iter, fnSet))
      return true;

    // Otherwise, try the next call site.
  }

  // OK, no recursive callers at all.
  return false;
}


//
// Task functions in this set are like their enclosing recursive iterators -
// they are to be ignored when inlining.
// This property of course does not transfer upon function->copy().
//
static Vec<Symbol*> taskFunInRecursiveIteratorSet;


//
// Mark as recursive those iterators which call themselves (directly or indirectly).
// When dealing with recursive iterators, we only inline the body of the loop,
// not the iterator as a whole.
// Also mark the task functions in those iterators.
//
static void computeRecursiveIteratorSet() {
  compute_call_sites();

  // Walk all functions
  forv_Vec(FnSymbol, iter, gFnSymbols)
  {
    // And select just the iterators.
    if (!iter->isIterator())
      continue;

    // Determine if the iterator calls itself, either directly or indirectly.
    Vec<FnSymbol*> fnSet;   // Used to avoid recursion
    if (find_recursive_caller(iter, iter, fnSet))
      // If so, add the recursive iterator flag.
      iter->addFlag(FLAG_RECURSIVE_ITERATOR);
  }

  // Mark task functions, too, by adding to taskFunInRecursiveIteratorSet
  forv_Vec(FnSymbol, taskFn, gFnSymbols) {
    if (!isTaskFun(taskFn))
      continue;

    // Ascend the callers until we find a non-task function.
    FnSymbol* currFn = taskFn;
    while (true) {
      if (currFn->calledBy->n == 0) {
        // currFn has no callers, so by inference neither does taskFn.
        // That being the case, we can bail -- no harm done.
        break;
      }

      // If there are multiple callers, they should be clones of
      // each other, so any one will do(??).  We grab the first one.
      currFn = toFnSymbol(currFn->calledBy->v[0]->parentSymbol);
      INT_ASSERT(currFn);

      if (isTaskFun(currFn)) {
        // Task functions are not recursive by construction.
        // Otherwise this is an infinite loop.
        INT_ASSERT(currFn != taskFn);
      } else {
        // Found it!
        if (currFn && currFn->hasFlag(FLAG_RECURSIVE_ITERATOR))
          taskFunInRecursiveIteratorSet.set_add(taskFn);
        break;
      }
    }
  }
}


// Remove supporting references in ShadowVarSymbols.
// Otherwise flattenNestedFunction() will try to propagate them.
static void clearUpRefsInShadowVars() {
  forv_Vec(ShadowVarSymbol, svar, gShadowVarSymbols)
    svar->removeSupportingReferences();
}


static bool containsYield(Expr* arg) {
  std::vector<CallExpr*> calls;
  collectCallExprs(arg, calls);
  for_vector(CallExpr, call, calls) {
    if (call->isPrimitive()) {
      if (call->isPrimitive(PRIM_YIELD))
        return true;
    } else if (FnSymbol* tfn = resolvedToTaskFun(call)) { // vass need this?
      if (containsYield(tfn->body))
        return true;
    }
  }
  return false;
}


//
// If a local block has no yields, returns, gotos or labels
// then it can safely be left unfragmented.
// A block whose blockInfo is PRIM_BLOCK_UNLOCAL must also be fragmented.
//
static bool leaveLocalBlockUnfragmented(BlockStmt* block) {
  std::vector<BaseAST*> asts;
  collect_asts(block, asts);

  for_vector(BaseAST, ast, asts) {

    // Check if the AST element is a GOTO.
    if (isGotoStmt(ast))
      return false;     // Yes, FAIL.

    // See if it is a yield or return statement.
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIM_YIELD) || call->isPrimitive(PRIM_RETURN))
        return false;     // Yes, FAIL.

      // Check coforall et al. recursively.
      if (FnSymbol* taskFn = resolvedToTaskFun(call))
        if (!leaveLocalBlockUnfragmented(taskFn->body))
          return false;
    }

    // See if it is an unlocal block.
    if (BlockStmt* block = toBlockStmt(ast)) {
      // NOAKES 2014/11/25. Transitional.  Avoid calling blockInfoGet
      if (block->isLoopStmt() == true) {

      } else if (block->blockInfoGet() &&
                 block->blockInfoGet()->isPrimitive(PRIM_BLOCK_UNLOCAL)) {
        return false;     // Yes, FAIL.
      }
    }

    // See if it is a label.
    if (DefExpr* def = toDefExpr(ast)) {
      if (def && isLabelSymbol(def->sym))
        return false;     // Yes, FAIL.
    }
  }

  // OK.  No undesirable statements found.
  return true;
}


//
// replace a local block by smaller local blocks that do not contain
// goto statements, yields, returns labels or unlocal blocks.
//
static void
fragmentLocalBlocks() {
  Vec<Expr*> preVec; // stmts to be inserted before new local blocks
  Vec<Expr*> inVec;  // stmts to be inserted in new local blocks

  //
  // collect all local blocks which need to be fragmented
  //
  Vec<BlockStmt*> localBlocks; // old local blocks

  forv_Vec(BlockStmt, block, gBlockStmts) {
    // NOAKES 2014/11/25 Transitional.  Avoid calling blockInfoGet() on loops
    if (block->isLoopStmt() == true) {

    } else if (block->parentSymbol &&
               block->blockInfoGet() &&
               block->blockInfoGet()->isPrimitive(PRIM_BLOCK_LOCAL) &&
               !leaveLocalBlockUnfragmented(block)) {
      localBlocks.add(block);
    }
  }

  //
  // collect first statements of local blocks into queue vector
  //
  Vec<Expr*> queue;

  forv_Vec(BlockStmt, block, localBlocks) {
    if (block->body.head)
      queue.add(block->body.head);
  }

  forv_Vec(Expr, expr, queue) {
    SET_LINENO(expr);

    for (Expr* current = expr; current; current = current->next) {
      bool      insertNewLocal = false;
      CallExpr* call           = toCallExpr(current);
      DefExpr*  def            = toDefExpr(current);

      //
      // If this statement is a yield, a return, a label definition, a
      // goto, a conditional, or a block, insert a new local block
      // that contains all the statements seen to this point (by
      // setting insertNewLocal to true) and add the first statements
      // of blocks and conditional blocks to the queue; otherwise, if
      // this statement is a definition, add it to preVec; otherwise,
      // add this statement to inVec.
      // Blocks and conditionals are visited in turn (breadth-first).
      //
      if ((call && (call->isPrimitive(PRIM_YIELD) ||
                    call->isPrimitive(PRIM_RETURN))) ||
          (def && isLabelSymbol(def->sym)) ||
          isGotoStmt(current) ||
          isCondStmt(current) ||
          isBlockStmt(current)) {
        insertNewLocal = true;

        if (BlockStmt* block = toBlockStmt(current)) {
          // NOAKES 2014/11/25 Transitional.
          // Avoid calling blockInfoGet() on loops
          if (block->isLoopStmt()   == false &&
              block->blockInfoGet() != NULL  &&
              block->blockInfoGet()->isPrimitive(PRIM_BLOCK_UNLOCAL)) {
            // UNLOCAL applies to a single LOCAL
            block->blockInfoGet()->remove();

          } else if (block->body.head) {
            queue.add(block->body.head);
          }
        } else if (CondStmt* cond = toCondStmt(current)) {
          if (cond->thenStmt && cond->thenStmt->body.head)
            queue.add(cond->thenStmt->body.head);
          if (cond->elseStmt && cond->elseStmt->body.head)
            queue.add(cond->elseStmt->body.head);
        }
      } else if (call && resolvedToTaskFun(call)) {
        // Do what the above would have done to a coforall/etc. block.
        insertNewLocal = true;

        Expr* taskfnBodyHead = call->resolvedFunction()->body->body.head;

        if (taskfnBodyHead) {
          queue.add(taskfnBodyHead);
        }

      } else if (isDefExpr(current)) {
        preVec.add(current);

      } else {
        inVec.add(current);
      }

      //
      // If ready to insert a new local block or we are on the last
      // statement in a block, insert a new local block containing all
      // the statements in inVec; move all the statements in preVec to
      // a point just before this new local block.
      //
      if (insertNewLocal || !current->next) {
        if (preVec.n || inVec.n) {
          BlockStmt* newLocalBlock = new BlockStmt();
          newLocalBlock->blockInfoSet(new CallExpr(PRIM_BLOCK_LOCAL));
          current->insertBefore(newLocalBlock);
          forv_Vec(Expr, expr, preVec) {
            newLocalBlock->insertBefore(expr->remove());
          }
          preVec.clear();
          forv_Vec(Expr, expr, inVec) {
            newLocalBlock->insertAtTail(expr->remove());
          }
          inVec.clear();
        }
      }
    }
  }

  //
  // remove old local blocks
  //
  forv_Vec(BlockStmt, block, localBlocks) {
    block->blockInfoGet()->remove();
  }
}


// In the body of the iterator, replace references to the iterator formals
// with references to fields in the iterator class.
//
// Multiple temps may be created for each formal.
static void
replaceIteratorFormalsWithIteratorFields(FnSymbol* iterator, Symbol* ic,
                                         SymExpr* se) {
  int count = 1;
  for_formals(formal, iterator) {
    if (se->symbol() == formal) {
      // count is used to get the nth field out of the iterator class;
      // it is replaced by the field once the iterator class is created
      Expr* stmt = se->getStmtExpr();

      // Error variable arguments should have already been handled.
      INT_ASSERT(! (formal->defPoint->parentSymbol != se->parentSymbol &&
                     formal->hasFlag(FLAG_ERROR_VARIABLE)));
      // TODO -- this should use/respect ArgSymbol's Qualifier
      VarSymbol* tmp = newTemp(formal->name, formal->type);

      stmt->insertBefore(new DefExpr(tmp));
      stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, ic, new_IntSymbol(count))));
      se->setSymbol(tmp);
    }
    count++;
  }
}

static void replaceErrorFormalWithEnclosingError(SymExpr* se);

static void
replaceIteratorFormals(FnSymbol* iterator, Symbol* ic,
                       std::vector<SymExpr*> & symExprs) {
  bool throws = iterator->throwsError();

  for_vector(SymExpr, se, symExprs) {
    // For an inlined iterator, we don't want to track the error
    // argument through the iterator class. Instead of setting
    // a formal error argument, set an enclosing error variable.
    if (throws)
      replaceErrorFormalWithEnclosingError(se);
    // if se was not replaced by the above call...
    if (se->parentSymbol != NULL)
      replaceIteratorFormalsWithIteratorFields(iterator, ic, se);
  }
}

static Map<FnSymbol*,FnSymbol*> iteratorFnMap;
static FnSymbol* argBundleCopyFn = NULL;
static FnSymbol* argBundleFreeFn = NULL;
static AggregateType*  argBundleType = NULL;


//
// Build the definition for the abstract recursive iterator argument bundle
// class, if one does not exist yet.
//
static void
ensureArgBundleType(AggregateType* ct) {
  if (argBundleType == NULL) {
    argBundleType = new AggregateType(AGGREGATE_CLASS);
    TypeSymbol* aabSym = new TypeSymbol("chpl_argBundle", argBundleType);
    aabSym->addFlag(FLAG_NO_OBJECT);
    ct->symbol->defPoint->insertBefore(new DefExpr(aabSym));
  }
}


// Creates a function to free the arg bundle associated with a recursive iterator.
//
// This is currently done using just one free function for all arg bundles.
// There is (effectively) a switch statement inside the function that selects
// the appropriate bundle based on a function ID inserted into the last field
//  and frees its memory.
//
// The function looks approximately like:
// void chpl__freeRecursiveIteratorArgumentBundle(int loopBodyFnID, chpl_argBundle loopBodyFnArgs)
// {
//   chpl_free(loopBodyFnArgs);
//   // potentially more
//   return void;
// }
//
static void
createArgBundleFreeFn(AggregateType* ct, FnSymbol* loopBodyFnWrapper) {
  if (argBundleFreeFn == NULL) {
    // Create the shared function that frees recursive argument bundles.
    argBundleFreeFn = new FnSymbol("chpl__freeRecursiveIteratorArgumentBundle");
    argBundleFreeFn->insertFormalAtTail(new ArgSymbol(INTENT_CONST_IN, "loopBodyFnID", dtInt[INT_SIZE_DEFAULT]));
    argBundleFreeFn->insertFormalAtTail(new ArgSymbol(INTENT_CONST_IN, "loopBodyFnArgs", argBundleType));
    argBundleFreeFn->insertAtTail(callChplHereFree(argBundleFreeFn->getFormal(2)));
    argBundleFreeFn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    argBundleFreeFn->retType = dtVoid;
    ct->symbol->defPoint->insertBefore(new DefExpr(argBundleFreeFn));
  }

  bool addedRecursiveCall = false;
  ArgSymbol* loopBodyFnIDArg = argBundleFreeFn->getFormal(1);
  ArgSymbol* loopBodyFnArgsArg = argBundleFreeFn->getFormal(2);

  // Add a block for each concrete arg bundle type.
  BlockStmt* block = new BlockStmt();

  // Get the bundle arg and cast it to the concrete type.
  Symbol* loopBodyFnArgsTmp = newTemp("loopBodyFnArgsTmp", ct);
  block->insertAtTail(new DefExpr(loopBodyFnArgsTmp));
  block->insertAtTail(new CallExpr(PRIM_MOVE, loopBodyFnArgsTmp, new CallExpr(PRIM_CAST, ct->symbol, loopBodyFnArgsArg)));

  // The last field contains the index used to select the recursive iterator argument bundle.
  Symbol* lastField = NULL; // assume function pointer for function argument bundle
  for_fields(field, ct) {
    // If the bundle contains a pointer to another bundle,
    // its clause must call this free function recursively.
    if (field->type->symbol->hasFlag(FLAG_REF) &&
        field->getValType()->symbol->hasFlag(FLAG_LOOP_BODY_ARGUMENT_CLASS)) {
      addedRecursiveCall = true;

      // Load the function ID ref into a temp.
      Symbol* recursiveFnIDRef = newTemp("recursiveFnIDRef", lastField->type);
      block->insertAtTail(new DefExpr(recursiveFnIDRef));
      block->insertAtTail(new CallExpr(PRIM_MOVE, recursiveFnIDRef, new CallExpr(PRIM_GET_MEMBER_VALUE, loopBodyFnArgsTmp, lastField)));

      // Load the function ID for the recursive call.
      VarSymbol* recursiveFnID = newTemp("recursiveFnID", dtInt[INT_SIZE_DEFAULT]);
      block->insertAtTail(new DefExpr(recursiveFnID));
      block->insertAtTail(new CallExpr(PRIM_MOVE, recursiveFnID, new CallExpr(PRIM_DEREF, recursiveFnIDRef)));

      // Load the recursive bundle pointer into a temp.
      Symbol* recursiveArgsRef = newTemp("recursiveArgsRef", field->type);
      block->insertAtTail(new DefExpr(recursiveArgsRef));
      block->insertAtTail(new CallExpr(PRIM_MOVE, recursiveArgsRef, new CallExpr(PRIM_GET_MEMBER_VALUE, loopBodyFnArgsTmp, field)));

      // Load the recursive bundle.
      VarSymbol* recursiveArgsTmp = newTemp("recursiveArgs", argBundleType);
      block->insertAtTail(new DefExpr(recursiveArgsTmp));
      block->insertAtTail(new CallExpr(PRIM_MOVE, recursiveArgsTmp,
                                       new CallExpr(PRIM_CAST, argBundleType->symbol,
                                                    new CallExpr(PRIM_DEREF, recursiveArgsRef))));

      // Generate the recursive call.
      block->insertAtTail(new CallExpr(argBundleFreeFn, recursiveFnID, recursiveArgsTmp));
    }
    lastField = field;
  }

  // If we added a recursive call, we have to add a new clause to the free function:
  //   if (loopBodyFnIDArg == ftable_index(loopBodyFnWrapper))
  //   {
  //     ...
  //     chpl__freeRecursiveIteratorArgumentBundle(recursive_ID, recursive_bundle);
  //   }
  if (addedRecursiveCall) {
    Symbol* cond = newTemp("cond", dtBool);
    argBundleFreeFn->insertAtHead(new CondStmt(new SymExpr(cond), block));
    argBundleFreeFn->insertAtHead(new CallExpr(PRIM_MOVE, cond, new CallExpr(PRIM_EQUAL, loopBodyFnIDArg, new_IntSymbol(ftableMap[loopBodyFnWrapper]))));
    argBundleFreeFn->insertAtHead(new DefExpr(cond));
  }
}


// Creates a function to copy the arg bundle associated with a recursive iterator.
//
// This is currently done using just one copy function for all arg bundles.
// There is (effectively) a switch statement inside the function that selects
// the appropriate bundle based on loopBodyFnID and copies that part.
//
// The created function has the signature:
//  chpl_argBundle chpl__copyRecursiveIteratorArgumentBundle(int loopBodyFnID, chpl_argBundle loopBodyFnArgs)
//
static void
createArgBundleCopyFn(AggregateType* ct, FnSymbol* loopBodyFnWrapper) {
  if (argBundleCopyFn == NULL) {
    // Create the shared function that copies recursive argument bundles.
    // The initial implementation is just a shell.  After creation, it looks like:
    //  chpl_argBundle chpl__copyRecursiveIteratorArgumentBundle(int loopBodyFnID, chpl_argBundle loopBodyFnArgs)
    //  {
    //    chpl_argBundle tmp = nil;
    //    return tmp;
    //  }
    argBundleCopyFn = new FnSymbol("chpl__copyRecursiveIteratorArgumentBundle");
    argBundleCopyFn->insertFormalAtTail(new ArgSymbol(INTENT_CONST_IN, "loopBodyFnID", dtInt[INT_SIZE_DEFAULT]));
    argBundleCopyFn->insertFormalAtTail(new ArgSymbol(INTENT_CONST_IN, "loopBodyFnArgs", argBundleType));
    Symbol* tmp = newTemp("dummyBundle", argBundleType);
    argBundleCopyFn->insertAtTail(new DefExpr(tmp));
    argBundleCopyFn->insertAtTail(new CallExpr(PRIM_MOVE, tmp, gNil));
    argBundleCopyFn->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
    argBundleCopyFn->retType = argBundleType;
    ct->symbol->defPoint->insertBefore(new DefExpr(argBundleCopyFn));
  }

  // Get the arguments to the (shared) copy function.
  ArgSymbol* loopBodyFnIDArg = argBundleCopyFn->getFormal(1);
  ArgSymbol* loopBodyFnArgsArg = argBundleCopyFn->getFormal(2);

  // Create a new body for this call to the copy function.
  //{
  //  args = (ct*)malloc(sizeof(ct));
  //  fnArgsTmp = (ct*)_loopBodyFnArgs;
  //  <field_type> tmp = fnArgsTmp->field;  // for each field
  //  int fnTmp = *lastTmp;                 // for each loop body arg field only
  //  <field_val_tmp> argsTmp = *tmp;       // for each loop body arg field only
  //  tmp = (<field_val_type>)
  //    chpl__copyRecursiveIteratorArgumentBundle(fnTmp, argsTmp);  // ditto
  //  args->field = tmp;                    // for each field
  //  ret = args;
  //}
  BlockStmt* block = new BlockStmt();
  Symbol* argBundle = newTemp("argBundle", ct);
  block->insertAtTail(new DefExpr(argBundle));
  insertChplHereAlloc(block->body.tail, true /*insertAfter*/,
                      argBundle, ct, newMemDesc("bundled args"));
  Symbol* loopBodyFnArgsTmp = newTemp("loopBodyFnArgsTmp", ct);
  block->insertAtTail(new DefExpr(loopBodyFnArgsTmp));
  block->insertAtTail(new CallExpr(PRIM_MOVE, loopBodyFnArgsTmp, new CallExpr(PRIM_CAST, ct->symbol, loopBodyFnArgsArg)));
  Symbol* lastTmp = NULL; // assume function pointer for function argument bundle
  for_fields(field, ct) {
    Symbol* tmp = newTemp(field->name, field->type);
    block->insertAtTail(new DefExpr(tmp));
    block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, loopBodyFnArgsTmp, field)));

    if (field->type->symbol->hasFlag(FLAG_REF) &&
        field->getValType()->symbol->hasFlag(FLAG_LOOP_BODY_ARGUMENT_CLASS)) {
      VarSymbol* copy = newTemp(field->name, field->type);
      VarSymbol* recursiveFnID = newTemp("recursiveFnID", dtInt[INT_SIZE_DEFAULT]);
      VarSymbol* recursiveArgs = newTemp("recursiveArgs", argBundleType);
      block->insertAtTail(new DefExpr(copy));
      block->insertAtTail(new DefExpr(recursiveFnID));
      block->insertAtTail(new DefExpr(recursiveArgs));
      block->insertAtTail(new CallExpr(PRIM_MOVE, recursiveFnID, new CallExpr(PRIM_DEREF, lastTmp)));
      block->insertAtTail(new CallExpr(PRIM_MOVE, recursiveArgs,
                                       new CallExpr(PRIM_CAST, argBundleType->symbol,
                                                    new CallExpr(PRIM_DEREF, tmp))));
      VarSymbol* retTmp = newTemp("retTmp", argBundleCopyFn->retType);
      block->insertAtTail(new DefExpr(retTmp));
      VarSymbol* castTmp = newTemp("castTmp", field->getValType());
      block->insertAtTail(new DefExpr(castTmp));
      block->insertAtTail(new CallExpr(PRIM_MOVE, retTmp, new CallExpr(argBundleCopyFn, recursiveFnID, recursiveArgs)));
      block->insertAtTail(new CallExpr(PRIM_MOVE, castTmp, new CallExpr(PRIM_CAST, castTmp->type->symbol, retTmp)));
      block->insertAtTail(new CallExpr(PRIM_MOVE, copy, new CallExpr(PRIM_ADDR_OF, castTmp)));
      tmp = copy;
    }

    block->insertAtTail(new CallExpr(PRIM_SET_MEMBER, argBundle, field, tmp));
    lastTmp = tmp;
  }
  Symbol* ret = argBundleCopyFn->getReturnSymbol();
  block->insertAtTail(new CallExpr(PRIM_MOVE, ret, new CallExpr(PRIM_CAST, ret->type->symbol, argBundle)));

  // The body is placed in a conditional, so only the body corresponding to the current
  // fnArg is actually executed when the copy function is called:
  //  if (fnArg == ftable[loopBodyFnWrapper])
  //  { <body_as_defined_above> }
  Symbol* cond = newTemp(dtBool);
  argBundleCopyFn->insertBeforeEpilogue(new DefExpr(cond));
  argBundleCopyFn->insertBeforeEpilogue(new CallExpr(PRIM_MOVE, cond, new CallExpr(PRIM_EQUAL, loopBodyFnIDArg, new_IntSymbol(ftableMap[loopBodyFnWrapper]))));
  argBundleCopyFn->insertBeforeEpilogue(new CondStmt(new SymExpr(cond), block));
}


// This creates the class type carrying the arguments to a (possibly recursive)
// iterator function call and fills a temp with the passed-in values.
//
static AggregateType*
bundleLoopBodyFnArgsForIteratorFnCall(CallExpr* iteratorFnCall,
                                      CallExpr* loopBodyFnCall,
                                      FnSymbol* loopBodyFnWrapper) {
  FnSymbol* iteratorFn = iteratorFnCall->resolvedFunction();
  FnSymbol* loopBodyFn = loopBodyFnCall->resolvedFunction();

  // Create the argument bundle type
  AggregateType* ct = new AggregateType(AGGREGATE_CLASS);
  TypeSymbol* ts = new TypeSymbol("_fn_arg_bundle", ct);
  ts->addFlag(FLAG_NO_OBJECT);
  ts->addFlag(FLAG_LOOP_BODY_ARGUMENT_CLASS);
  iteratorFnCall->parentSymbol->defPoint->insertBefore(new DefExpr(ts));

  // And its corresponding reference type.
  AggregateType* rct = new AggregateType(AGGREGATE_CLASS);
  TypeSymbol* rts = new TypeSymbol("_ref_fn_arg_bundle", rct);
  rts->addFlag(FLAG_NO_OBJECT);
  rts->addFlag(FLAG_REF);
  iteratorFnCall->parentSymbol->defPoint->insertBefore(new DefExpr(rts));
  rct->fields.insertAtTail(new DefExpr(new VarSymbol("_val", ct)));
  ct->refType = rct;

  // Create the argument bundle.
  // args = (ct*)malloc(sizeof(ct));
  VarSymbol* argBundle = newTemp("argBundle", ct);
  iteratorFnCall->insertBefore(new DefExpr(argBundle));
  insertChplHereAlloc(iteratorFnCall, false /*insertAfter*/, argBundle,
                      ct, newMemDesc("bundled args"));
  iteratorFnCall->insertAtTail(argBundle);
  iteratorFnCall->insertAfter(callChplHereFree(argBundle));

  // loopBodyWrapper(int index, ct* fn_args) {
  //   loopBodyFn(index);
  // }
  ArgSymbol* wrapperIndexArg = loopBodyFn->getFormal(1)->copy();
  loopBodyFnWrapper->insertFormalAtTail(wrapperIndexArg);
  ArgSymbol* wrapperArgsArg = new ArgSymbol(INTENT_CONST_IN, "fn_args", ct);
  loopBodyFnWrapper->insertFormalAtTail(wrapperArgsArg);
  CallExpr* loopBodyFnWrapperCall = new CallExpr(loopBodyFn, wrapperIndexArg);


  int i = 1;
  Expr* lastActual = NULL;
  while (loopBodyFnCall->numActuals() >= 2) {
    // Skip the index arg.
    Expr* actual = loopBodyFnCall->get(2)->remove();
    // Create a field for this arg.
    VarSymbol* field = new VarSymbol(astr("_arg", istr(i++)), actual->typeInfo());
    ct->fields.insertAtTail(new DefExpr(field));

    if (field->type->symbol->hasFlag(FLAG_REF) &&
        field->getValType()->symbol->hasFlag(FLAG_LOOP_BODY_ARGUMENT_CLASS)) {
      // Does anything need to be done here if the iterator invokes
      // task function(s)?
      if (iteratorFn->hasFlag(FLAG_ITERATOR_WITH_ON)) {

        // For recursive args in forked bodies,
        // build up a local copy of the argument bundle (recursive copy).

        // This code assumes that the depth arg for the recursive loopBodyFnCall
        // precedes the recursive arg bundle in the list of actual arguments.
        //
        // int index = *lastActual;
        // ct* rec_args = *actual;
        // ct* rec_args_copy = (ct*)chpl__copyRecursiveIteratorArgumentBundle(index, rec_args);
        //
        VarSymbol* tmp = newTemp(field->name, field->type);
        VarSymbol* recursiveFnID = newTemp("recursiveFnID", dtInt[INT_SIZE_DEFAULT]);
        VarSymbol* loopBodyFnArgsTmp = newTemp("loopBodyFnArgsTmp", argBundleType);
        iteratorFnCall->insertBefore(new DefExpr(tmp));
        iteratorFnCall->insertBefore(new DefExpr(recursiveFnID));
        iteratorFnCall->insertBefore(new DefExpr(loopBodyFnArgsTmp));
        iteratorFnCall->insertBefore(new CallExpr(PRIM_MOVE, recursiveFnID, new CallExpr(PRIM_DEREF, lastActual->copy())));
        iteratorFnCall->insertBefore(new CallExpr(PRIM_MOVE, loopBodyFnArgsTmp,
                                                  new CallExpr(PRIM_CAST, argBundleType->symbol,
                                                               new CallExpr(PRIM_DEREF, actual))));
        VarSymbol* retTmp = newTemp("retTmp", argBundleCopyFn->retType);
        iteratorFnCall->insertBefore(new DefExpr(retTmp));
        VarSymbol* castTmp = newTemp("castTmp", field->getValType());
        iteratorFnCall->insertBefore(new DefExpr(castTmp));
        iteratorFnCall->insertBefore(new CallExpr(PRIM_MOVE, retTmp, new CallExpr(argBundleCopyFn, recursiveFnID, loopBodyFnArgsTmp)));
        iteratorFnCall->insertBefore(new CallExpr(PRIM_MOVE, castTmp, new CallExpr(PRIM_CAST, castTmp->type->symbol, retTmp)));
        iteratorFnCall->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_ADDR_OF, castTmp)));
        iteratorFnCall->insertAfter(new CallExpr(argBundleFreeFn, recursiveFnID, retTmp));
        actual = new SymExpr(tmp);
      }
    }

    iteratorFnCall->insertBefore(new CallExpr(PRIM_SET_MEMBER, argBundle, field, actual));

    VarSymbol* tmp = newTemp(field->name, field->type);

    // In the wrapper function, moves the current arg bundle field into a temp
    // (unbundles it) and adds it to the args used  to call the loop body function.
    loopBodyFnWrapper->insertAtTail(new DefExpr(tmp));
    loopBodyFnWrapper->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, wrapperArgsArg, field)));
    loopBodyFnWrapperCall->insertAtTail(tmp);
    lastActual = actual; // assume function pointer for function argument bundle
  }
  loopBodyFnCall->remove();
  loopBodyFnWrapper->insertAtTail(loopBodyFnWrapperCall);
  loopBodyFnWrapper->retType = dtVoid;
  loopBodyFnWrapper->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  ensureArgBundleType(ct);
  createArgBundleCopyFn(ct, loopBodyFnWrapper);
  createArgBundleFreeFn(ct, loopBodyFnWrapper);
  return ct;
}


//
// return the number of local blocks that lexically enclose 'expr',
// stopping the search outward when encountering the outer block
// todo: should this count carry across calls to task functions?
//
static int
countEnclosingLocalBlocks(Expr* expr, BlockStmt* outer = NULL) {
  int count = 0;

  for (Expr* tmp = expr; tmp && tmp != outer; tmp = tmp->parentExpr) {
    if (BlockStmt* blk = toBlockStmt(tmp)) {
      // NOAKES 2014/11/25 Transitional.  Avoid calling blockInfoGet() on loops
      if (blk->isLoopStmt() == true) {

      } else if (blk->blockInfoGet() == NULL) {

      } else if (blk->blockInfoGet()->isPrimitive(PRIM_BLOCK_LOCAL)) {
        count++;

      } else if (blk->blockInfoGet()->isPrimitive(PRIM_BLOCK_UNLOCAL)) {
        count--;
      }
    }
  }
  return count;
}


static void localizeReturnSymbols(FnSymbol* iteratorFn, std::vector<SymExpr*> symExprs)
{
  //
  // localize return symbols
  //
  Symbol* ret = iteratorFn->getReturnSymbol();
  Map<BlockStmt*,Symbol*> retReplacementMap;
  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == ret) {
      BlockStmt* block = NULL;
      for (Expr* parent = se->parentExpr; parent; parent = parent->parentExpr) {
        block = toBlockStmt(parent);
        if (block)
          break;
      }
      INT_ASSERT(block);
      if (block != ret->defPoint->parentExpr) {
        if (Symbol* repl = retReplacementMap.get(block)) {
          se->setSymbol(repl);
        } else {
          SET_LINENO(se);
          Symbol* newRet = newTemp("newRet", ret->type);
          newRet->addFlag(FLAG_SHOULD_NOT_PASS_BY_REF);
          block->insertAtHead(new DefExpr(newRet));
          se->setSymbol(newRet);
          retReplacementMap.put(block, newRet);
        }
      }
    }
  }
}


//
// Invokes localizeReturnSymbols() on all iterators.
//
// Q: What about yields in task functions?
// A: Since this is done before flattenFunctions, task functions
// are still nested in their respective iterators. So their yields
// will be included in 'asts' and handled when 'fn' is the enclosing
// iterator.
//
static void localizeIteratorReturnSymbols() {
  forv_Vec(FnSymbol, iterFn, gFnSymbols) {
    if (iterFn->inTree() && iterFn->isIterator()) {
      std::vector<SymExpr*> symExprs;
      collectSymExprs(iterFn, symExprs);
      localizeReturnSymbols(iterFn, symExprs);
    }
  }
}

// processIteratorYields is a separate pass, called before flattenFunctions.
// TODO: Move this and supporting functions into their own source file.
void processIteratorYields() {
  localizeIteratorReturnSymbols();
}


static Map<FnSymbol*,FnSymbol*> loopBodyFnArgsSuppliedMap;

//
// Thread loopBodyFnIDArg, loopBodyFnArgArgs through calls to task functions.
// We assume that threadLoopBodyFnArgs() sees each call at most once.
// The call needs to be augmented to pass these two things as actuals.
//
// The callee taskFn needs to be augmented with two new formals.
// Note that the original taskFn needs to be left around unmodified.
// That's because it is likely invoked from the original recursive iterator
// before (in the AST nesting sense, that is) the point where we
// implement recursion with (loopBodyFnIDArg, loopBodyFnArgArgs).
//
// The call needs to be redirected to this augmented taskFn copy.
// 'taskFnP' is updated to point to this copy as well.
//
static void threadLoopBodyFnArgs(CallExpr* call,
                                 ArgSymbol* loopBodyFnIDArg,
                                 ArgSymbol* loopBodyFnArgArgs,
                                 FnSymbol**  taskFnP,
                                 ArgSymbol** newIdArgP,
                                 ArgSymbol** newArgArgsP)
{
  FnSymbol* origTFn = *taskFnP; // same as call->isResolved()
  FnSymbol* copyTFn = loopBodyFnArgsSuppliedMap.get(origTFn);

  if (copyTFn) {
    // This might happen. It should work by removing the assert,
    // but has not been tested.
    INT_ASSERT(false);

    int nf = copyTFn->numFormals();
    *newIdArgP = copyTFn->getFormal(nf-1);
    *newArgArgsP = copyTFn->getFormal(nf);

  } else {
    // Created an augmented copy.
    copyTFn = origTFn->copy();
    copyTFn->name = astr("rec_iter_task_fn_", copyTFn->name);
    origTFn->defPoint->insertBefore(new DefExpr(copyTFn));
    loopBodyFnArgsSuppliedMap.put(origTFn, copyTFn);

    // Add the formals, created just like in createIteratorFn().
    ArgSymbol* newIdArg = new ArgSymbol(INTENT_CONST_IN, loopBodyFnIDArg->name, loopBodyFnIDArg->type);
    copyTFn->insertFormalAtTail(newIdArg);
    ArgSymbol* newArgArgs = new ArgSymbol(INTENT_CONST_IN, loopBodyFnArgArgs->name, loopBodyFnArgArgs->type);
    copyTFn->insertFormalAtTail(newArgArgs);

    *newIdArgP = newIdArg;
    *newArgArgsP = newArgArgs;
  }

  // This 'call' has not been processed yet. If it has been,
  // it should not be modified again.
  INT_ASSERT(call->numActuals() + 2 == copyTFn->numFormals());

  // Augment the call as well.
  call->insertAtTail(loopBodyFnIDArg);
  call->insertAtTail(loopBodyFnArgArgs);

  // Redirect call to the taskFn copy.
  call->baseExpr->replace(new SymExpr(copyTFn));
  *taskFnP = copyTFn;
}


// Replace each yield with a call to the loop body function,
// and remove return statements.
// If 'asts' are the body of a task function, the return statement
// must stay because it is a required part of a function.
static void convertYieldsAndReturns(std::vector<CallExpr*>& calls, Symbol* index,
                                    ArgSymbol* loopBodyFnIDArg, ArgSymbol* loopBodyFnArgArgs)
{
  for_vector(CallExpr, call, calls) {
    if (call->isPrimitive(PRIM_YIELD)) {
      SET_LINENO(call);
      Symbol* yieldedIndex = newTemp("_yieldedIndex", index->type);
      call->insertBefore(new DefExpr(yieldedIndex));
      call->insertBefore(new CallExpr(PRIM_MOVE, yieldedIndex, call->get(1)->remove()));
      Expr* loopBodyFnCall = new CallExpr(PRIM_FTABLE_CALL, loopBodyFnIDArg, yieldedIndex, loopBodyFnArgArgs);

      // The inserted loop body function call is surrounded by a number of
      // unlocal blocks equal to the number of local blocks surrounding the original call.
      int count = countEnclosingLocalBlocks(call);
      Expr* callOrBlk = loopBodyFnCall;
      while (count-- > 0)
      {
        BlockStmt* blk = new BlockStmt(callOrBlk);
        blk->blockInfoSet(new CallExpr(PRIM_BLOCK_UNLOCAL));
        callOrBlk = blk;
      }
      call->replace(callOrBlk);
    } else if (call->isPrimitive(PRIM_RETURN) && !isTaskFun(toFnSymbol(call->parentSymbol))) {
      // Just remove a return.
      call->remove();
    } else if (FnSymbol* taskFn = resolvedToTaskFun(call)) {
      // Process yields within (a clone of) taskFn.
      // Todo: do this only if taskFn has yields (directly or not).
      ArgSymbol *tIDArg, *tArgArgs;
      threadLoopBodyFnArgs(call, loopBodyFnIDArg, loopBodyFnArgArgs,
                           &taskFn, &tIDArg, &tArgArgs);
      // both 'taskFn' and 'call' are updated by threadLoopBodyFnArgs()
      INT_ASSERT(call->resolvedFunction() == taskFn);
      std::vector<CallExpr*> taskCallExprs;
      collectCallExprs(taskFn, taskCallExprs);
      convertYieldsAndReturns(taskCallExprs, index, tIDArg, tArgArgs);
    }
  }
}


// Returns true if the given function contains an on statement; false otherwise.
// "Contains" includes "had a coforall/etc. block, now replaced with a call".
static bool fnContainsOn(FnSymbol* fn)
{
  std::vector<CallExpr*> calls;

  collectCallExprs(fn, calls);

  for_vector(CallExpr, call, calls) {
    if (call->isPrimitive(PRIM_BLOCK_ON) ||
        call->isPrimitive(PRIM_BLOCK_BEGIN_ON) ||
        call->isPrimitive(PRIM_BLOCK_COBEGIN_ON) ||
        call->isPrimitive(PRIM_BLOCK_COFORALL_ON))
      return true;

    if (FnSymbol* taskFn = resolvedToTaskFun(call))
      if (fnContainsOn(taskFn))
        return true;
  }

  return false;
}


static FnSymbol*
createIteratorFn(FnSymbol* iterator, CallExpr* iteratorFnCall, Symbol* index,
                 CallExpr* loopBodyFnCall, FnSymbol* loopBodyFnWrapper,
                 Symbol* ic)
{
  SET_LINENO(iterator);
  FnSymbol* iteratorFn = new FnSymbol(astr("_rec_iter_fn_", iterator->name));

  // identify and mark iterators with on-statements
  if (fnContainsOn(iterator))
    iteratorFn->addFlag(FLAG_ITERATOR_WITH_ON);

  // Now calls the newly-created iterator function.
  iteratorFnCall->baseExpr->replace(new SymExpr(iteratorFn));
  AggregateType* argsBundleType =
    bundleLoopBodyFnArgsForIteratorFnCall(iteratorFnCall, loopBodyFnCall, loopBodyFnWrapper);

  iteratorFn->body = iterator->body->copy();
  iterator->defPoint->insertBefore(new DefExpr(iteratorFn));
  std::vector<SymExpr*> symExprs;
  std::vector<CallExpr*> calls;
  collectSymExprs(iteratorFn, symExprs);
  collectCallExprs(iteratorFn, calls);
  ArgSymbol* icArg = new ArgSymbol(blankIntentForType(ic->type), "_ic", ic->type);
  iteratorFn->insertFormalAtTail(icArg);

  replaceIteratorFormals(iterator, icArg, symExprs);

  ArgSymbol* loopBodyFnIDArg = new ArgSymbol(INTENT_CONST_IN, "_loopBodyFnID", dtInt[INT_SIZE_DEFAULT]);
  iteratorFn->insertFormalAtTail(loopBodyFnIDArg);
  ArgSymbol* loopBodyFnArgArgs = new ArgSymbol(INTENT_CONST_IN, "_loopBodyFnArgs", argsBundleType);
  iteratorFn->insertFormalAtTail(loopBodyFnArgArgs);

  localizeReturnSymbols(iteratorFn, symExprs);

  convertYieldsAndReturns(calls, index, loopBodyFnIDArg, loopBodyFnArgArgs);

  iteratorFn->retType = dtVoid;
  iteratorFn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  iteratorFn->removeFlag(FLAG_INLINE_ITERATOR);
  iteratorFn->removeFlag(FLAG_ITERATOR_FN);

  return iteratorFn;
}

/// \param call A for loop block primitive.
static void
expandRecursiveIteratorInline(ForLoop* forLoop)
{
  SET_LINENO(forLoop);

  FnSymbol*  parent            = toFnSymbol(forLoop->parentSymbol);

  // create a nested function for the loop body (call->parentExpr), and then transform
  // the iterator into a function that takes this nested function as an argument
  FnSymbol*  loopBodyFn        = new FnSymbol(astr("_rec_iter_loop_", parent->name));

  // The index is passed to the loop body function as its first argument.
  Symbol*    index             = forLoop->indexGet()->symbol();
  ArgSymbol* indexArg          = new ArgSymbol(blankIntentForType(index->type), "_index", index->type);

  // The recursive iterator loop wrapper is ... .
  FnSymbol*  loopBodyFnWrapper = new FnSymbol(astr("_rec_iter_loop_wrapper_", parent->name));

  loopBodyFn->insertFormalAtTail(indexArg);

  forLoop->insertBefore(new DefExpr(loopBodyFn));

  parent->defPoint->insertBefore(new DefExpr(loopBodyFnWrapper));

  ftableVec.push_back(loopBodyFnWrapper);
  ftableMap[loopBodyFnWrapper] = ftableVec.size()-1;

  //
  // insert a call to the iterator function (using iterator as a
  // placeholder); build a call to loopBodyFn which will be
  // removed later and its arguments passed to the iteratorFn (we
  // build this to capture the actual arguments that should be
  // passed to it when this function is flattened)
  //
  Symbol*    ic             = forLoop->iteratorGet()->symbol();
  FnSymbol*  iterator       = getTheIteratorFn(ic);
  CallExpr*  iteratorFnCall = new CallExpr(iterator, ic, new_IntSymbol(ftableMap[loopBodyFnWrapper]));

  // replace function in iteratorFnCall with iterator function once that is created
  CallExpr*  loopBodyFnCall = new CallExpr(loopBodyFn, gVoid);

  // use and remove loopBodyFnCall later
  // We expect this call to cause the loop body function to be converted like a
  // "normal" iterator function body.
  forLoop->insertBefore(loopBodyFnCall);
  forLoop->insertBefore(iteratorFnCall);

  BlockStmt* blockStmt = forLoop->copyBody();

  // Replace the ForLoop with a BlockStmt of the body
  forLoop->replace(blockStmt);

  // The forLoop's body becomes the body of the (new) loop body function.
  loopBodyFn->insertAtTail(blockStmt->remove());

  // Now populate the loop body function.
  // Load the index arg.
  loopBodyFn->insertAtHead(new CallExpr(PRIM_MOVE, index, indexArg));
  loopBodyFn->insertAtHead(index->defPoint->remove());

  // Return nothing (void).
  loopBodyFn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  loopBodyFn->retType = dtVoid;

  // Move the loop body function out to the module level.
  flattenNestedFunction(loopBodyFn);

  FnSymbol* iteratorFn = iteratorFnMap.get(iterator);

  if (!iteratorFn) {
    iteratorFn = createIteratorFn(iterator,
                                  iteratorFnCall,
                                  index,
                                  loopBodyFnCall,
                                  loopBodyFnWrapper,
                                  ic);

    iteratorFnMap.put(iterator, iteratorFn);
  } else {
    iteratorFnCall->baseExpr->replace(new SymExpr(iteratorFn));
    bundleLoopBodyFnArgsForIteratorFnCall(iteratorFnCall, loopBodyFnCall, loopBodyFnWrapper);

    Symbol* argBundleTmp = newTemp("argBundleTmp", iteratorFn->getFormal(3)->type);

    iteratorFnCall->insertBefore(new DefExpr(argBundleTmp));
    iteratorFnCall->insertBefore(new CallExpr(PRIM_MOVE,
                                              argBundleTmp,
                                              new CallExpr(PRIM_CAST,
                                                           argBundleTmp->type->symbol,
                                                           iteratorFnCall->get(3)->remove())));

    iteratorFnCall->insertAtTail(argBundleTmp);
  }
}


typedef Map<FnSymbol*,FnSymbol*> TaskFnCopyMap;

static void
expandBodyForIteratorInline(ForLoop*       forLoop,
                            BlockStmt*     ibody,
                            Symbol*        index);


static void
expandBodyForIteratorInline(ForLoop*       forLoop,
                            BlockStmt*     ibody,
                            Symbol*        index,
                            bool           inTaskFn,
                            TaskFnCopyMap& taskFnCopies,
                            bool&          addErrorArgToCall);

/// \param call A for loop block primitive.
static bool
// Returns true if the given ForLoop was handled (converted and removed from
// the tree); false otherwise.
expandIteratorInline(ForLoop* forLoop) {
  Symbol*   ic       = forLoop->iteratorGet()->symbol();
  FnSymbol* iterator = getTheIteratorFn(ic);

  if (fReportInlinedIterators) {
    ModuleSymbol *mod = iterator->getModule();

    if (developer || mod->modTag == MOD_USER) {
      printf("Inlined iterator (%s) in module %s (%s:%d)\n", iterator->cname,
              mod->name, iterator->fname(), iterator->linenum());
    }
  }


  if (iterator->hasFlag(FLAG_RECURSIVE_ITERATOR)) {
    // NOAKES 2014/11/30  Only 6 tests, some with minor variations, use this path

    // The iterand of the forLoop is marked as a recursive iterator.  This is
    // the case we are looking for.  But first, we have to weed out a few
    // troublesome cases.

    // sjd:
    // loops over recursive iterators in recursive iterators only need
    // to be handled in the recursive iterator function
    //
    if (forLoop->parentSymbol->hasFlag(FLAG_RECURSIVE_ITERATOR)) {
      return false;
    // vass: ditto for task functions called from recursive iterators
    } else if (taskFunInRecursiveIteratorSet.set_in(forLoop->parentSymbol)) {
      return false;
    } else if (containsYield(forLoop)) {
      // Inlining a recursive iterator into a loop with a 'yield' pushes
      // that 'yield' into one of _rec_ functions, where it dangles. Ex.:
      // test/library/standard/FileSystem/filerator/bradc/findfiles-par.chpl
      return false;
    } else {
      expandRecursiveIteratorInline(forLoop);
      INT_ASSERT(!forLoop->inTree());
      return true;
    }

  } else {
    SET_LINENO(forLoop);

    Symbol*       index = forLoop->indexGet()->symbol();
    BlockStmt*    ibody = iterator->body->copy();
    std::vector<SymExpr*> symExprs;

    bool isOrderIndependent = forLoop->isOrderIndependent();
    if (preserveInlinedLineNumbers == false) {
      reset_ast_loc(ibody, forLoop);
    }

    // and the entire for loop block is replaced by the iterator body.
    forLoop->replace(ibody);

    // if the loop being expanded was order independent, all of the yielding
    // loops in the body are also order independent. Note that this must occur
    // after the ibody replaces the forLoop since findEnclosingLoop() requires
    // that its argument be in the AST. It must occur before yields are
    // replaced in the functions below though.
    if (isOrderIndependent) {
      std::vector<CallExpr*> callExprs;

      collectCallExprs(ibody, callExprs);

      for_vector(CallExpr, call, callExprs) {
        if (call->isPrimitive(PRIM_YIELD)) {
          if (LoopStmt* loop = LoopStmt::findEnclosingLoop(call)) {
            if (loop->isCoforallLoop() == false) {
              loop->orderIndependentSet(isOrderIndependent);
            }
          }
        }
      }
    }

    // Replace yield statements in the inlined iterator body with copies
    // of the body of the For Loop that invoked the iterator, substituting
    // the yielded index for the iterator formal.
    expandBodyForIteratorInline(forLoop, ibody, index);

    collectSymExprs(ibody, symExprs);
    replaceIteratorFormals(iterator, ic, symExprs);

    // We can return true if forLoop has been removed from the tree.
    INT_ASSERT(!forLoop->inTree());
    return true;
  }
}

static void
expandBodyForIteratorInline(ForLoop*       forLoop,
                            BlockStmt*     ibody,
                            Symbol*        index) {
  TaskFnCopyMap taskFnCopies;
  bool addErrorArgToCall = false;
  expandBodyForIteratorInline(forLoop, ibody, index, false,
                              taskFnCopies, addErrorArgToCall);
  INT_ASSERT(addErrorArgToCall == false); // case not handled
}

/*
 A Goto ErrorHandling label in the body of a loop will not
 be properly handled when the iterator is inlined into a task
 function because the label will be outside of the task function.

 These functions help to compensate by discovering any GotoStmt
 that needs to be adjusted.
 */

// For task functions, we need an error argument for
// parallel to process them correctly, but the error
// is actually passed through the end-count error list.
static void
addDummyErrorArgumentToCall(CallExpr* call)
{
  VarSymbol* errorTmp = newTemp("dummy_error", dtError);
  errorTmp->addFlag(FLAG_ERROR_VARIABLE);
  call->insertBefore(new DefExpr(errorTmp));
  call->insertAtTail(errorTmp);
}

static ArgSymbol*
findErrorFormalForFn(FnSymbol* fn)
{
  ArgSymbol* errorArg = NULL;
  if (fn->throwsError()) {
    for_formals(formal, fn) {
      // TODO: should this check FLAG_ERROR_VARIABLE?
      if (formal->getValType() == dtError)
        errorArg = formal;
    }
  }
  return errorArg;
}

static ArgSymbol*
addDummyErrorFormalToFn(FnSymbol* fn)
{
  ArgSymbol* errorArg = new ArgSymbol(INTENT_REF, "error_out", dtError);
  errorArg->addFlag(FLAG_ERROR_VARIABLE);
  fn->insertFormalAtTail(errorArg);
  fn->throwsErrorInit();
  return errorArg;
}

// Adjust an error handling exit in two cases:
// 1. the GotoStmt points outside of this function
// 2. the error variable updated is outside of this function.
// In both cases, we need to change it to update this function's
// error_out argument and exit this function.
static void
fixupErrorHandlingExits(BlockStmt* body, bool& adjustCaller) {
  FnSymbol* fn = toFnSymbol(body->parentSymbol);
  INT_ASSERT(fn);

  std::vector<GotoStmt*> gotos;
  collectGotoStmts(body, gotos);

  for_vector(GotoStmt, g, gotos) {

    if (g->gotoTag == GOTO_ERROR_HANDLING ||
        g->gotoTag == GOTO_BREAK_ERROR_HANDLING ||
        g->gotoTag == GOTO_RETURN) {
      // Does the target of this Goto exist within the same function?
      LabelSymbol* target = g->gotoTarget();
      INT_ASSERT(target->defPoint->parentSymbol);

      // Do we need to fix the Goto because it points outside the function?
      bool fixTarget = (target->defPoint->parentSymbol != body->parentSymbol);
      // Do we need to fix the error handling because it uses an Error
      // variable outside of this function?
      bool fixError = false;

      Expr* oldErrorSrc = NULL;
      Expr* oldErrorDst = NULL;

      // Find the PRIM_MOVE/PRIM_ASSIGN setting the error variable
      CallExpr* prevMoveErr = NULL;
      Expr* cur = g->prev;
      while (cur != NULL) {
        if (CallExpr* call = toCallExpr(cur)) {
          if ((call->isPrimitive(PRIM_MOVE) ||
               call->isPrimitive(PRIM_ASSIGN)) &&
              call->get(1)->typeInfo() == dtError) {
            prevMoveErr = call;
            break;
          }
        }
        cur = cur->prev;
      }

      // TODO -- only consider errors with FLAG_ERROR_VAR?
      // Is it possible for user code to look like this?

      if (prevMoveErr != NULL) {
        oldErrorDst = prevMoveErr->get(1);
        oldErrorSrc = prevMoveErr->get(2);
        INT_ASSERT(oldErrorDst);
        INT_ASSERT(oldErrorDst->typeInfo() == dtError);
        SymExpr* dstSe = toSymExpr(oldErrorDst);
        INT_ASSERT(dstSe);
        Symbol* dstSym = dstSe->symbol();
        INT_ASSERT(dstSym->defPoint->parentSymbol);
        if (dstSym->defPoint->parentSymbol != fn)
          fixError = true;
      }

      if (fixTarget || fixError) {
        // Find the function epilogue
        LabelSymbol* epilogue = fn->getOrCreateEpilogueLabel();

        // Find the formal to store the error
        // ... find the (last) error argument
        ArgSymbol* errorArg = findErrorFormalForFn(fn);
        if (errorArg == NULL) {
          errorArg = addDummyErrorFormalToFn(fn);
          adjustCaller = true;
        }
        INT_ASSERT(errorArg != NULL);

        // Replace it with assign / goto return
        if (oldErrorDst && oldErrorSrc) {
          oldErrorDst->remove();
          oldErrorSrc->remove();
          prevMoveErr->replace(new CallExpr(PRIM_ASSIGN, errorArg, oldErrorSrc));
        }
        GotoStmt* newGoto = new GotoStmt(GOTO_RETURN, epilogue);
        g->replace(newGoto);
      }
    }
  }
}

static bool
findFollowingCheckErrorBlock(SymExpr* se, LabelSymbol*& outHandlerLabel,
    Symbol*& outErrorSymbol) {
  Expr* stmt = se->getStmtExpr(); // aka last scope
  Expr* scope = stmt->parentExpr;

  while (scope) {
    if (isBlockStmt(scope)) {
      // Consider statements that appear before stmt
      // We are looking for a DefExpr of an error label
      for(Expr* cur = stmt->next; cur != NULL; cur = cur->next) {
        if (DefExpr* def = toDefExpr(cur))
          if (LabelSymbol* label = toLabelSymbol(def->sym))
            if (label->hasFlag(FLAG_ERROR_LABEL)) {
              outHandlerLabel = label;
              // find the error that this block is working with
              for(Expr* e = def->next; e != NULL; e = e->next) {
                std::vector<CallExpr*> calls;
                collectCallExprs(e, calls);
                for_vector(CallExpr, call, calls) {
                  if (call->isPrimitive(PRIM_CHECK_ERROR)) {
                    SymExpr* se = toSymExpr(call->get(1));
                    INT_ASSERT(se->symbol()->hasFlag(FLAG_ERROR_VARIABLE));
                    outErrorSymbol = se->symbol();
                    return true;
                  }
                }
              }
              INT_FATAL("Could not find error variable for handler");
            }
      }
    }
    stmt = scope;
    scope = scope->parentExpr;
  }

  return false;
}

/* When inlining an iterator, the iterator might throw
   an error. If it does, instead of updating the iterator's
   out error argument, the error should propagate to
   a following error-handling block if one exists.
 */
static void
replaceErrorFormalWithEnclosingError(SymExpr* se) {

  Symbol* oldSymbol = se->symbol();
  if (oldSymbol->defPoint->parentSymbol != se->parentSymbol &&
      oldSymbol->hasFlag(FLAG_ERROR_VARIABLE)) {

    LabelSymbol* newLabel = NULL;
    Symbol* newError = NULL;

    // This code is only written to handle a PRIM_MOVE/ASSIGN
    // that is setting the out error argument of the iterator.
    // This out error argument is no longer available since the
    // iterator is being inlined.
    CallExpr* parentCall = toCallExpr(se->parentExpr);
    INT_ASSERT(parentCall->isPrimitive(PRIM_MOVE) ||
               parentCall->isPrimitive(PRIM_ASSIGN));
    INT_ASSERT(se == parentCall->get(1));

    FnSymbol* inFn = toFnSymbol(se->parentSymbol);

    // find the Goto we need to replace
    GotoStmt* fixGoto = NULL;
    for(Expr* cur = se->getStmtExpr(); cur != NULL; cur = cur->next) {
      fixGoto = toGotoStmt(cur);
      if (fixGoto != NULL)
        break;
    }
    INT_ASSERT(fixGoto);

    if (findFollowingCheckErrorBlock(se, newLabel, newError)) {
      // Adjust the current error handling block.
      // 1. Change the use of the error argument to use the
      // identified error variable.
      // 2. Change the Goto to point to the identified error label.

      se->setSymbol(newError);
      fixGoto->gotoTag = GOTO_ERROR_HANDLING;
      fixGoto->label->replace(new SymExpr(newLabel));
    } else if(isTaskFun(inFn)) {
      // 1. Make sure that the task function has an error argument.
      // 2. Set that out argument instead of the invalid one.
      ArgSymbol* errorArg = findErrorFormalForFn(inFn);
      if (errorArg == NULL) {
        errorArg = addDummyErrorFormalToFn(inFn);
        // And adjust callers of this function
        for_SymbolSymExprs(se, inFn) {
          CallExpr* call = toCallExpr(se->parentExpr);
          addDummyErrorArgumentToCall(call);
        }
      }
      se->setSymbol(errorArg);
      INT_ASSERT(fixGoto->gotoTag == GOTO_RETURN);
    } else {
      // Just call gChplUncaughtError
      VarSymbol* tmp = newTemp("error", dtError);
      parentCall->insertBefore(new DefExpr(tmp));
      se->setSymbol(tmp);
      fixGoto->replace(new CallExpr(gChplUncaughtError, tmp));
    }
  }
}

static void
expandBodyForIteratorInline(ForLoop*       forLoop,
                            BlockStmt*     ibody,
                            Symbol*        index,
                            bool           inTaskFn,
                            TaskFnCopyMap& taskFnCopies,
                            bool&          addErrorArgToCall) {
  std::vector<BaseAST*> asts;
  bool removeReturn = !inTaskFn;

  collect_asts(ibody, asts);

  for_vector(BaseAST, ast, asts) {

    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIM_YIELD)) {
        Symbol*    yieldedIndex  = newTemp("_yieldedIndex", index->type);
        Symbol*    yieldedSymbol = toSymExpr(call->get(1))->symbol();
        BlockStmt* bodyCopy      = NULL;
        bool       inserted      = false;

        if (forLoop->isCoforallLoop()) {
          // parallel.cpp wants to know about these when considering whether
          // or not to insert autoCopies
          yieldedIndex->addFlag(FLAG_COFORALL_INDEX_VAR);
        }

        SymbolMap  map;

        map.put(index, yieldedIndex);

        bodyCopy = forLoop->copyBody(&map);

        if (int count = countEnclosingLocalBlocks(call, ibody)) {
          for (int i = 0; i < count; i++) {
            bodyCopy = new BlockStmt(bodyCopy);

            bodyCopy->blockInfoSet(new CallExpr(PRIM_BLOCK_UNLOCAL));
          }
        }

        // remove move to return-temp that is defined at top of iterator
        if (CallExpr* prev = toCallExpr(call->prev)) {
          if (prev->isPrimitive(PRIM_MOVE)) {
            if (SymExpr* lhs = toSymExpr(prev->get(1))) {
              if (lhs->symbol() == yieldedSymbol) {
                lhs->setSymbol(yieldedIndex);

                prev->insertBefore(new DefExpr(yieldedIndex));

                inserted = true;
              }
            }
          }
        }

        call->replace(bodyCopy);

        if (inserted == false) {
          bodyCopy->insertBefore(new DefExpr(yieldedIndex));
          bodyCopy->insertBefore(new CallExpr(PRIM_MOVE, yieldedIndex, call->get(1)));
        }

        // Fix an error-handling sequence that now refers
        // outside of its function.
        if (inTaskFn)
          fixupErrorHandlingExits(bodyCopy, addErrorArgToCall);

      }


      // Remove returns if the command-line argument indicates to
      if (call->isPrimitive(PRIM_RETURN)) {
        if (removeReturn)
          call->remove();
      }

      // Adjust calls to chpl_propagate_error
      if (FnSymbol* calledFn = call->resolvedFunction()) {
        if (calledFn == gChplPropagateError) {
          SymExpr* errSe = toSymExpr(call->get(1));
          INT_ASSERT(errSe && errSe->typeInfo() == dtError);
          LabelSymbol* label = NULL;
          Symbol* error = NULL;
          if (findFollowingCheckErrorBlock(errSe, label, error)) {
            errSe->remove();
            call->insertBefore(new CallExpr(PRIM_MOVE, error, errSe));
            call->insertBefore(new GotoStmt(GOTO_ERROR_HANDLING, label));
            call->remove();
          }
        }
      }

      // Adjust task functions within the iterator
      if (FnSymbol* cfn = resolvedToTaskFun(call)) {
        // Todo: skip this handling of 'cfn' if it does not have yields
        // in itself or any other taskFns it may call.

        // This holds because we flatten everything right away.
        // We need it so that we can place the def of 'fcopy' anywhere
        // while preserving correct scoping of its SymExprs.
        INT_ASSERT(isGlobal(cfn));

        bool addErrorArgToSubCall = false;
        FnSymbol* fcopy = taskFnCopies.get(cfn);

        if (!fcopy) {
          // Clone the function. Just once per 'body' should suffice.
          fcopy = cfn->copy();
          if (!preserveInlinedLineNumbers)
            reset_ast_loc(fcopy, call);

          // Note that 'fcopy' will likely get a copy of 'body',
          // so we need to preserve correct scoping of its SymExprs.
          call->insertBefore(new DefExpr(fcopy));

          // I don't expect invocation of expandBodyForIteratorInline() below
          // to encounter another call to 'cfn'. But even if does, 'fcopy'
          // will be fetched from 'taskFnCopies', avoiding recursion.
          taskFnCopies.put(cfn, fcopy);

          // Repeat, recursively.
          expandBodyForIteratorInline(forLoop, fcopy->body, index, true,
              taskFnCopies, addErrorArgToSubCall);

        } else {
          // Indeed, 'cfn' is encountered only once per 'body',
          // although I cannot explain why this is *always* the case.
          // ('cfn' may be encountered more than once overall.)
          // If it *is* seen more than once, everything should still work,
          // so this assertion should be OK to remove.
          // (If using flattenOneFunction() below, need to get more calls
          // to its actual argument.)
          INT_ASSERT(false);
        }

        // Call 'fcopy' instead
        call->baseExpr->replace(new SymExpr(fcopy));

        if (addErrorArgToSubCall) {
          addDummyErrorArgumentToCall(call);
        }

        // Note: this is an expensive operation due to compute_call_sites().
        // We do it because it may eliminate further cloning of 'fcopy'
        // e.g. when the enclosing fn or block are copied for any reason.
        // Ideally, replace with flattenOneFunction().
        flattenNestedFunction(fcopy);
      }
    }
  }
}


// Returns the number of yields in the given function.
//
// If any call expressions call a task function, this functions is called
// recursively on those task functions.  Task functions are out-lined in
// createTaskFunctions, but contain statements -- including yields -- that
// actually "belong" to the calling function.  Recursive calls of this function
// effectively inline those out-lined functions so we get an accurate count.
static int
countYieldsInFn(FnSymbol* fn)

{
  int count = 0;

  std::vector<CallExpr*> calls;

  collectCallExprs(fn, calls);

  for_vector(CallExpr, call, calls)
  {
    if (call->isPrimitive(PRIM_YIELD))
      count++;

    if (FnSymbol* taskFn = resolvedToTaskFun(call))
      count += countYieldsInFn(taskFn);
  }
  return count;
}

// Returns true if the iterator can be inlined; false otherwise.
//
// Iterators can be inlined if they contains between 1 and
// inline_iter_yield_limit yield statements
static bool
canInlineIterator(FnSymbol* iterator) {
  int count = countYieldsInFn(iterator);

  // count==0 e.g. in users/biesack/test_recursive_iterator.chpl
  return (count >= 1 && count <= inline_iter_yield_limit);
}

static void
setupSimultaneousIterators(Vec<Symbol*>& iterators,
                           Vec<Symbol*>& indices,
                           Symbol*       iterator,
                           Symbol*       index,
                           ForLoop*      loop) {
  if (iterator->type->symbol->hasFlag(FLAG_TUPLE)) {
    AggregateType* iteratorType = toAggregateType(iterator->type);
    AggregateType* indexType    = toAggregateType(index->type);

    for (int i = 1; i <= iteratorType->fields.length; i++) {
      Symbol* tmpIterator = newTemp("_iterator", iteratorType->getField(i)->type);
      Symbol* tmpIndex    = newTemp("_index",    indexType->getField(i)->type);

      loop->insertBefore(new DefExpr(tmpIterator));
      loop->insertBefore(new CallExpr(PRIM_MOVE,
                                      tmpIterator,
                                      new CallExpr(PRIM_GET_MEMBER_VALUE,
                                                   iterator,
                                                   iteratorType->getField(i))));

      loop->insertAtHead(new CallExpr(PRIM_SET_MEMBER,
                                      index,
                                      indexType->getField(i),
                                      tmpIndex));

      loop->insertAtHead(new DefExpr(tmpIndex));

      setupSimultaneousIterators(iterators, indices, tmpIterator, tmpIndex, loop);
    }
  } else {
    iterators.add(iterator);
    indices.add(index);
  }
}


static bool
isBoundedIterator(FnSymbol* fn) {
  if (fn->_this) {
    Type* type = fn->_this->getValType();
    if (type->symbol->hasFlag(FLAG_RANGE)) {
      if (!strcmp(type->substitutions.v[1].value->name, "bounded"))
        return true;
      else
        return false;
    }
  }
  return true;
}


static void getIteratorChildren(Vec<Type*>& children, Type* type) {
  if (AggregateType* at = toAggregateType(type)) {
    forv_Vec(Type, child, at->dispatchChildren) {
      if (child != dtObject) {
        children.add_exclusive(child);
        getIteratorChildren(children, child);
      }
    }
  }
}

#define ZIP1     1
#define ZIP2     2
#define ZIP3     3
#define ZIP4     4
#define HASMORE  5
#define GETVALUE 6
#define INIT     7
#define INCR     8

static void buildIteratorCallInner(BlockStmt* block,
                                   Symbol*    ret,
                                   int        fnid,
                                   Symbol*    iterator) {
  IteratorInfo* ii = getTheIteratorFn(iterator)->iteratorInfo;
  FnSymbol*     fn = NULL;

  switch (fnid) {
  case ZIP1:     fn = ii->zip1;     break;
  case ZIP2:     fn = ii->zip2;     break;
  case ZIP3:     fn = ii->zip3;     break;
  case ZIP4:     fn = ii->zip4;     break;
  case HASMORE:  fn = ii->hasMore;  break;
  case GETVALUE: fn = ii->getValue; break;
  case INIT:     fn = ii->init;     break;
  case INCR:     fn = ii->incr;     break;
  }

  CallExpr* call = new CallExpr(fn, iterator);

  if (ret) {
    if (fn->retType->getValType() == ret->type->getValType()) {
      INT_ASSERT(fn->retType == ret->type);
      block->insertAtTail(new CallExpr(PRIM_MOVE, ret, call));
    } else {
      VarSymbol* tmp = newTemp("retTmp", fn->retType);
      block->insertAtTail(new DefExpr(tmp));
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, call));
      block->insertAtTail(new CallExpr(PRIM_MOVE, ret, new CallExpr(PRIM_CAST, ret->type->symbol, tmp)));
    }
  } else {
    block->insertAtTail(call);
  }
}


static BlockStmt*
buildIteratorCall(Symbol* ret, int fnid, Symbol* iterator, Vec<Type*>& children) {
  BlockStmt* block = new BlockStmt();
  BlockStmt* outerBlock = block;
  forv_Vec(Type, type, children) {
    VarSymbol* cid = newTemp("cidSelector", dtBool);
    block->insertAtTail(new DefExpr(cid));
    block->insertAtTail(new CallExpr(PRIM_MOVE, cid,
                          new CallExpr(PRIM_TESTCID,
                                       iterator, type->symbol)));
    BlockStmt* thenStmt = new BlockStmt();
    BlockStmt* elseStmt = new BlockStmt();
    VarSymbol* childIterator = newTemp("childIterator", type);
    thenStmt->insertAtTail(new DefExpr(childIterator));
    thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, childIterator, new CallExpr(PRIM_CAST, type->symbol, iterator)));
    buildIteratorCallInner(thenStmt, ret, fnid, childIterator);
    block->insertAtTail(new CondStmt(new SymExpr(cid), thenStmt, elseStmt));
    block = elseStmt;
  }
  buildIteratorCallInner(block, ret, fnid, iterator);
  return outerBlock;
}

// Replace a ForLoop with its inline equivalent, if possible.
// Otherwise, convert it into a C-style for loop.
// The given forLoop is converted unconditionally.
static void
expandForLoop(ForLoop* forLoop) {
  SymExpr*   se2      = forLoop->iteratorGet();
  VarSymbol* iterator = toVarSymbol(se2->symbol());
  bool converted = false;

  if (!fNoInlineIterators)
  {
    FnSymbol* iterFn = getTheIteratorFn(iterator->type);
    if (iterFn->iteratorInfo && canInlineIterator(iterFn) &&
        !isVirtualIterator(iterator)) {
      converted = expandIteratorInline(forLoop);
    }
  }

  if (! converted)
  {
    // This code handles zippered iterators, dynamic iterators, and any other
    // iterator that cannot be inlined.

    // When expanding a single-loop iterator with a single yield, the generated
    // code looks like:
    //   zip1(_iterator1); zip1(_iterator2);
    //   idx2 = getValue(_iterator2); idx1 = getValue(_iterator1);
    //   bool more1 = hasMore(iterator1);
    //   for (init(_iterator1), init(_iterator2); more1; incr(_iterator1), incr(_iterator2)) {
    //     zip2(_iterator);
    //     // Bounds checks inserted here.
    //     zip3(_iterator1); zip3(_iterator2);
    //     more = hasMore(iterator1);
    //   }
    //   zip4(_iterator2); zip4(_iterator1);
    // In zippered iterators, each clause may contain multiple calls to zip1(),
    // getValue(), etc.  These are inserted in the order shown.

    SET_LINENO(forLoop);

    Vec<Symbol*> iterators;
    Vec<Symbol*> indices;

    FnSymbol* iterFn = getTheIteratorFn(iterator->type);
    if (iterFn->throwsError()) {
      // In this event, the error handling pass added a PRIM_CHECK_ERROR
      // after the call to the iterator function.
      // Scroll backwards to find the error handling block.

      // TODO: finish this case
      //       I think we need to use the ForLoop's break label
      USR_FATAL("Throwing non-inlined iterators are not yet supported");
    }

    SymExpr*     se1       = toSymExpr(forLoop->indexGet());
    VarSymbol*   index     = toVarSymbol(se1->symbol());

    BlockStmt*   initBlock = new BlockStmt();
    BlockStmt*   testBlock = NULL;
    BlockStmt*   incrBlock = new BlockStmt();

    setupSimultaneousIterators(iterators, indices, iterator, index, forLoop);

    bool allOrderIndependent = true;
    // For each iterator we add the zip* functions in the appropriate place and
    // if bounds checking was on, we insert the code for that. Note that this
    // code handles iterators that have regular loops, c for loops, and
    // dynamically dispatched iterators. The ordering is VERY important!
    for (int i = 0; i < iterators.n; i++) {
      Vec<Type*> children;
      VarSymbol* cond         = newTemp("_cond", dtBool);
      bool       isNotDynIter = false;

      getIteratorChildren(children, iterators.v[i]->type);

      // Add zip1 before the loop
      forLoop->insertBefore(buildIteratorCall(NULL,         ZIP1,     iterators.v[i], children));

      // Get the current value of the iterator.
      forLoop->insertAtHead(buildIteratorCall(indices.v[i], GETVALUE, iterators.v[i], children));

      isNotDynIter = (children.n == 0);

      if (isNotDynIter) {
        // add the init, and incr functions to the init, and incr blocks of the
        // c for loop. If the underlying iterator does not have a c for loop,
        // these blocks will be empty
        initBlock->insertAtTail(buildIteratorCall(NULL, INIT, iterators.v[i], children));
        incrBlock->insertAtTail(buildIteratorCall(NULL, INCR, iterators.v[i], children));

      } else {
        // for dynamically dispatched iterators, conditional checks and other
        // code are added in buildIteratorCall. These constructs aren't legal
        // in a c for loop, so instead of creating a well formed c for loop we
        // add the init before the loop, and the incr at the bottom of it.
        forLoop->insertBefore(buildIteratorCall(NULL, INIT, iterators.v[i], children));
        forLoop->insertAtTail(buildIteratorCall(NULL, INCR, iterators.v[i], children));
      }

      // Add zip3 and zip4 at tail and after the loop respectively.
      forLoop->insertAtTail(buildIteratorCall(NULL, ZIP3, iterators.v[i], children));
      forLoop->insertAfter (buildIteratorCall(NULL, ZIP4, iterators.v[i], children));

      FnSymbol* iterFn = getTheIteratorFn(iterators.v[i]);

      if (isBoundedIterator(iterFn)) {
        if (testBlock == NULL) {
          if (isNotDynIter) {
            // note that we have found the first test
            testBlock = buildIteratorCall(NULL, HASMORE, iterators.v[i], children);

          } else {
            // note that we have found the first test block and add checks for
            // more before and at the end of the loop. As mentioned above,
            // dynamic iterators generate things that can't be in the header of
            // the c for loop, so we generate a simple bool variable to put at
            // the test of the c for loop, and update that condition var before
            // the loop is run, and at the end of each iteration.
            forLoop->insertBefore(new DefExpr(cond));
            forLoop->insertBefore(buildIteratorCall(cond, HASMORE, iterators.v[i], children));
            forLoop->insertAtTail(buildIteratorCall(cond, HASMORE, iterators.v[i], children));

            testBlock = new BlockStmt(new SymExpr(cond));
          }

        } else if (!fNoBoundsChecks) {
          // for all but the first iterator add checks at the beginning of each loop run
          // and a final one after to make sure the other iterators don't finish before
          // the "leader" and they don't have more afterwards.
          VarSymbol* hasMore    = newTemp("hasMore",    dtBool);
          VarSymbol* isFinished = newTemp("isFinished", dtBool);

          forLoop->insertBefore(new DefExpr(isFinished));
          forLoop->insertBefore(new DefExpr(hasMore));

          forLoop->insertAtHead(new CondStmt(new SymExpr(isFinished),
                                             new CallExpr(PRIM_RT_ERROR,
                                                          new_CStringSymbol("zippered iterations have non-equal lengths"))));

          forLoop->insertAtHead(new CallExpr(PRIM_MOVE, isFinished, new CallExpr(PRIM_UNARY_LNOT, hasMore)));

          forLoop->insertAtHead(buildIteratorCall(hasMore, HASMORE, iterators.v[i], children));

          forLoop->insertAfter(new CondStmt(new SymExpr(hasMore),
                                            new CallExpr(PRIM_RT_ERROR,
                                                         new_CStringSymbol("zippered iterations have non-equal lengths"))));

          forLoop->insertAfter(buildIteratorCall(hasMore, HASMORE, iterators.v[i], children));
        }
      }

      forLoop->insertAtHead(buildIteratorCall(NULL, ZIP2, iterators.v[i], children));

      // Need to check if iterator will be inlined, isSingleLoopIterator()
      // doesn't handle arbitrary blockstmts well, so we collapse them first
      iterFn->collapseBlocks();
      Vec<BaseAST*> asts;
      collect_asts_postorder(iterFn, asts);

      // If the iterator cannot be inlined a re-entrant advance function will
      // be built. This function maintains state and must be called in order.
      // If inlined, the iterator's loop will be order independent if it was
      // independent prior to inlining or the forLoop is order independent
      bool curOrderIndependent = false;
      if (CallExpr* singleLoopYield = isSingleLoopIterator(iterFn, asts)) {
        if (LoopStmt* loop = LoopStmt::findEnclosingLoop(singleLoopYield)) {
          curOrderIndependent = loop->isOrderIndependent() || forLoop->isOrderIndependent();
        }
      }
      allOrderIndependent = allOrderIndependent && curOrderIndependent;
    }

    // The loop will be order independent if all the iters it zips are order
    // independent (all iters will be inlined and were all marked order
    // independent or the forLoop was marked independent prior zippering.)
    forLoop->orderIndependentSet(allOrderIndependent);

    // 2015-02-23 hilde:
    // TODO: I think this wants to be insertBefore, and moved before the call
    // to getValue is inserted.  Check the order in the generated code to see
    // if this is the case.  Avoid moving the global void value when it is
    // the loop index.
    if (index != gVoid)
      forLoop->insertAtHead(index->defPoint->remove());

    // Ensure that the test clause for completely unbounded loops contains
    // something.
    // testBlock is only non-NULL if isBoundedIterator() evaluates to true for
    // at least one of the iterators being zippered together.
    if (testBlock == NULL) {
      testBlock = new BlockStmt();

      testBlock->insertAtTail(new SymExpr(gTrue));
    }

    // NOAKES 2014/11/19: An error occurs if the replacement is moved to
    // earlier in the pass.  I have yet to identify the issue but suspect
    // that doing the copy too soon causes variables to cross from one
    // scope to another if done in mid-transformation.
    CForLoop* cforLoop = CForLoop::buildWithBodyFrom(forLoop);

    // Even for zippered iterators we only have one conditional test for the
    // loop. This takes that conditional and puts it into the test segment of
    // the c for loop.  Code in the initBlock and incrBlock clauses are each a
    // concatenation of the init() and incr() calls for all iterators being
    // zippered together.  The init() and incr() calls appear in the same order
    // as the iterators appear in the zip(,,) tuple.
    cforLoop->loopHeaderSet(initBlock, testBlock, incrBlock);

    forLoop->replace(cforLoop);
  }
}


// Find all iterator constructs
// Select those whose _getIterator() functions have the FLAG_INLINE_ITERATOR.
// Inline the selected iterators at their call sites.
static void
inlineIterators() {
  forv_Vec(BlockStmt, block, gBlockStmts) {
    // Skip blocks that are no longer in the tree.
    if (block->parentSymbol == NULL)
      continue;

    if (ForLoop* forLoop = toForLoop(block)) {
      Symbol*   iterator = toSymExpr(forLoop->iteratorGet())->symbol();
      FnSymbol* ifn = getTheIteratorFn(iterator);
      if (ifn->hasFlag(FLAG_INLINE_ITERATOR)) {
        // The Boolean return value from expandIteratorInline() is being
        // ignored here, which means that forLoop might not have been replaced.
        // However, all ForLoops that remain in the tree after the call to
        // inlineIterators() are passed through expandForLoop() which *does*
        // replace them.
        expandIteratorInline(forLoop);
      }
    }
  }
}


static void fixNumericalGetMemberPrims()
{
  // fix GET_MEMBER primitives that access fields of an iterator class
  // via a number
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol && call->isPrimitive(PRIM_GET_MEMBER)) {
      AggregateType* ct = toAggregateType(call->get(1)->getValType());
      int64_t num;
      if (get_int(call->get(2), &num)) {
        Symbol* field = ct->getField(num+1); // add 1 for super
        SET_LINENO(call);
        call->get(2)->replace(new SymExpr(field));
        CallExpr* parent = toCallExpr(call->parentExpr);
        INT_ASSERT(parent->isPrimitive(PRIM_MOVE));
        Symbol* local = toSymExpr(parent->get(1))->symbol();
        if (local->type == field->type)
          call->primitive = primitives[PRIM_GET_MEMBER_VALUE];
        else if (local->type != field->type->refType)
          INT_FATAL(call, "unexpected case");
      }
    }
  }
}


static void cleanupLeaderFollowerIteratorCalls()
{
  //
  // cleanup leader and follower iterator calls
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol) {
      if (FnSymbol* fn = call->resolvedFunction()) {
        if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) ||
            (isDefExpr(fn->formals.tail) &&
             !strcmp(toDefExpr(fn->formals.tail)->sym->name, "_retArg") &&
             toDefExpr(fn->formals.tail)->sym->getValType() &&
             toDefExpr(fn->formals.tail)->sym->getValType()->symbol->hasFlag(FLAG_ITERATOR_RECORD))) {
          if (!strcmp(call->parentSymbol->name, "_toLeader") ||
              !strcmp(call->parentSymbol->name, "_toFollower") ||
              !strcmp(call->parentSymbol->name, "_toFastFollower") ||
              !strcmp(call->parentSymbol->name, "_toStandalone")) {
            ArgSymbol* iterator = toFnSymbol(call->parentSymbol)->getFormal(1);
            Type* iteratorType = iterator->getValType();
            int i = 2; // first field is super
            for_actuals(actual, call) {
              SymExpr* se = toSymExpr(actual);
              if (isArgSymbol(se->symbol()) && call->parentSymbol != se->symbol()->defPoint->parentSymbol) {
                Symbol* field = toAggregateType(iteratorType)->getField(i);
                VarSymbol* tmp = NULL;
                SET_LINENO(call);
                if (field->type == se->symbol()->type) {
                  tmp = newTemp(field->name, field->type);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, iterator, field)));
                } else if (field->type->refType == se->symbol()->type) {
                  tmp = newTemp(field->name, field->type->refType);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, iterator, field)));
                }
                actual->replace(new SymExpr(tmp));
                i++;
              }
            }
          }
        }
      }
    }
  }
}


static void handlePolymorphicIterators()
{
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    // Find iterator functions that are not in the AST tree.
    if (fn->defPoint->parentSymbol && fn->iteratorInfo) {
      // Assert that the getIterator() function *is* in the tree.
      FnSymbol* getIterator = fn->iteratorInfo->getIterator;
      INT_ASSERT(getIterator->defPoint->parentSymbol);

      // See if the iterator record is polymorphic.
      AggregateType* irecord = fn->iteratorInfo->irecord;
      if (irecord->dispatchChildren.n > 0) {
        // If so, then simulate dynamic dispatch by adding one conditional block
        // for each possible subtype.
        SET_LINENO(getIterator);
        LabelSymbol* label = new LabelSymbol("end");
        getIterator->insertBeforeEpilogue(new DefExpr(label));
        Symbol* ret = getIterator->getReturnSymbol();
        forv_Vec(Type, type, irecord->dispatchChildren) {
          AggregateType* subTypeAgg = toAggregateType(type);
          VarSymbol* tmp = newTemp(irecord->getField(1)->type);
          VarSymbol* cid = newTemp(dtBool);
          BlockStmt* thenStmt = new BlockStmt();
          VarSymbol* recordTmp = newTemp("recordTmp", type);
          VarSymbol* classTmp = newTemp("classTmp", subTypeAgg->iteratorInfo->getIterator->retType);
          thenStmt->insertAtTail(new DefExpr(recordTmp));
          thenStmt->insertAtTail(new DefExpr(classTmp));

          AggregateType* ct = toAggregateType(type);
          for_fields(field, ct) {
            VarSymbol* ftmp = newTemp("ftmp", getIterator->getFormal(1)->type->getField(field->name)->type);
            thenStmt->insertAtTail(new DefExpr(ftmp));
            thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, ftmp, new CallExpr(PRIM_GET_MEMBER_VALUE, getIterator->getFormal(1), getIterator->getFormal(1)->type->getField(field->name))));
            // Store temp in record field.
            if (ftmp->type == field->type) {
              thenStmt->insertAtTail(new CallExpr(PRIM_SET_MEMBER, recordTmp, field, ftmp));
            } else {
              VarSymbol* ftmp2 = newTemp(field->type);
              thenStmt->insertAtTail(new DefExpr(ftmp2));
              thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, ftmp2, new CallExpr(PRIM_CAST, field->type->symbol, ftmp)));
              thenStmt->insertAtTail(new CallExpr(PRIM_SET_MEMBER, recordTmp, field, ftmp2));
            }
          }
          thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, classTmp, new CallExpr(subTypeAgg->iteratorInfo->getIterator, recordTmp)));
          thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, ret, new CallExpr(PRIM_CAST, ret->type->symbol, classTmp)));
          thenStmt->insertAtTail(new GotoStmt(GOTO_GETITER_END, label));
          ret->defPoint->insertAfter(new CondStmt(new SymExpr(cid), thenStmt));
          ret->defPoint->insertAfter(new CallExpr(PRIM_MOVE, cid, new CallExpr(PRIM_TESTCID, tmp, subTypeAgg->iteratorInfo->irecord->getField(1)->type->symbol)));
          ret->defPoint->insertAfter(new DefExpr(cid));
          ret->defPoint->insertAfter(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, getIterator->getFormal(1), irecord->getField(1))));
          ret->defPoint->insertAfter(new DefExpr(tmp));
        }
      }
    }
  }
}


static void reconstructIRAutoCopy(FnSymbol* fn)
{
  Symbol* arg = fn->getFormal(1);
  Symbol* ret = fn->getReturnSymbol();
  BlockStmt* block = new BlockStmt();
  block->insertAtTail(ret->defPoint->remove());
  AggregateType* irt = toAggregateType(arg->type);
  for_fields(field, irt) {
    SET_LINENO(field);
    if (hasAutoCopyForType(field->type)) {
      FnSymbol* autoCopy = getAutoCopyForType(field->type);
      Symbol* tmp1 = newTemp(field->name, field->type);
      Symbol* tmp2 = newTemp(autoCopy->retType);
      Symbol* refTmp = NULL;
      block->insertAtTail(new DefExpr(tmp1));
      block->insertAtTail(new DefExpr(tmp2));
      if (isReferenceType(autoCopy->getFormal(1)->type)) {
        refTmp = newTemp(autoCopy->getFormal(1)->type);
        block->insertAtTail(new DefExpr(refTmp));
      }
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp1, new CallExpr(PRIM_GET_MEMBER_VALUE, arg, field)));
      if (refTmp) {
        block->insertAtTail(new CallExpr(PRIM_MOVE, refTmp, new CallExpr(PRIM_ADDR_OF, tmp1)));
      }
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp2, new CallExpr(autoCopy, refTmp?refTmp:tmp1)));
      block->insertAtTail(new CallExpr(PRIM_SET_MEMBER, ret, field, tmp2));
    } else {
      Symbol* tmp = newTemp(field->name, field->type);
      block->insertAtTail(new DefExpr(tmp));
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, arg, field)));
      block->insertAtTail(new CallExpr(PRIM_SET_MEMBER, ret, field, tmp));
    }
  }
  block->insertAtTail(new CallExpr(PRIM_RETURN, ret));
  fn->body->replace(block);
}


static void reconstructIRAutoDestroy(FnSymbol* fn)
{
  Symbol* arg = fn->getFormal(1);
  BlockStmt* block = new BlockStmt();
  AggregateType* irt = toAggregateType(arg->type);
  for_fields(field, irt) {
    SET_LINENO(field);
    if (FnSymbol* autoDestroy = autoDestroyMap.get(field->type)) {
      Symbol* tmp = newTemp(field->name, field->type);
      block->insertAtTail(new DefExpr(tmp));
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, arg, field)));
      block->insertAtTail(new CallExpr(autoDestroy, tmp));
    }
  }
  block->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  fn->body->replace(block);
}


static void reconstructIRautoCopyAutoDestroy()
{
  //
  // reconstruct autoCopy and autoDestroy for iterator records
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->numFormals() == 1 && fn->getFormal(1)->type->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
      SET_LINENO(fn);
      if (fn->hasFlag(FLAG_AUTO_COPY_FN))
        reconstructIRAutoCopy(fn);
      if (fn->hasFlag(FLAG_AUTO_DESTROY_FN))
        reconstructIRAutoDestroy(fn);
    }
  }
}


// release some memory
static void cleanupTemporaryVectors() {
  taskFunInRecursiveIteratorSet.clear();
  iteratorFnMap.clear();
  loopBodyFnArgsSuppliedMap.clear();
}


// 'depth' is a heuristic to avoid the risk of unbounded recursion.
// Ex. what if 'parentSym' is a recursive function?
static bool maybeCalled(Symbol* parentSym, int depth) {
  if (!parentSym || !parentSym->inTree())
    return false;

  FnSymbol* fn = toFnSymbol(parentSym);
  if (!fn)
    return true; // conservative

  if (ftableMap.count(fn))
    return true;

  if (!fn->calledBy || fn->calledBy->n == 0)
    return false;

  if (depth <= 0)
    return true; // conservative

  depth--;

  forv_Vec(CallExpr, call, *fn->calledBy)
    if (maybeCalled(call->parentSymbol, depth))
      return true;

  return false;
}

// WORKAROUND:
// When the body of a for loop is moved into the loop body function, yield
// primitives remain in it (if the for loop itself appears in an iterator
// function.
// When the containing iterator is expanded in a for loop, yields in the loop
// body will be replaced appropriately (through the recursive call to
// expandBodyForIteratorInline() that occurs when a call to a task function is
// encountered).
// Any yields remaining in the tree at this point are apparently dead code.
// Thus the functions containing them can be removed from the tree and calls to
// them stubbed out.  As a assertion that these calls are never reached, they
// are replaced by internal error primitives.
static void removeUncalledIterators()
{
  compute_call_sites();

  forv_Vec(CallExpr, call, gCallExprs)
  {
    // We only care about calls that are still in the tree.
    if (! call->parentSymbol)
      continue;
    // We only care about yields.
    if (! call->isPrimitive(PRIM_YIELD))
      continue;

    // This is a backup check to ensure PRIM_INT_ERROR is never executed.
    if (fVerify && maybeCalled(call->parentSymbol, 4))
      INT_FATAL(call, "unexpected leftover PRIM_YIELD");

    // If this function contains a yield, it was never expanded, so the static
    // analysis used in lowerIterators says it was never invoked through a for
    // loop.
    FnSymbol* fn = toFnSymbol(call->parentSymbol);
    if (!fn->defPoint->parentSymbol)
      // Only bother with functions that are still in the tree.
      continue;

    // Replace invocations of this function with a runtime error.
    if (fn->calledBy)
      forv_Vec(CallExpr, invcn, *fn->calledBy)
      {
        if (!invcn->parentSymbol)
          continue;

        SET_LINENO(invcn);
        invcn->replace(new CallExpr(PRIM_INT_ERROR));
      }

    // and yank the function itself.
    fn->defPoint->remove();
  }
}


void lowerIterators() {
  nonLeaderParCheck();
  parallelIterVirtualCheck();

  clearUpRefsInShadowVars();

  computeRecursiveIteratorSet();

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->isIterator()) {
      fn->collapseBlocks();

      removeUnnecessaryGotos(fn);

#if DEBUG_CP < 2    // That is, disabled if DEBUG_CP >= 2
      // Run localCopyPropagation to remove any fields we can so they will not
      // be added to the iterator class
      if (!fNoCopyPropagation)
        localCopyPropagation(fn);

#endif
    }
  }

  // TODO: The AST is not valid between inlineIterators and
  // fixNumericalGetMemberPrims because of
  // replaceIteratorFormalsWithIteratorFields.
  // We should look at creating the iterator class before here so we don't have
  // this problem.
  inlineIterators();

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->isIterator()) {
      fn->collapseBlocks();
      removeUnnecessaryGotos(fn);
    }
  }

  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (isAlive(block) == true && block->isForLoop() == true) {
      if (ForLoop* loop = toForLoop(block)) {
        expandForLoop(loop);
      }
    }
  }

  if (fVerify)
  {
    forv_Vec(BlockStmt, block, gBlockStmts)
    {
      if (isAlive(block) && block->isForLoop())
      {
        // All forLoops should have been removed from the tree by now.
        INT_FATAL(block, "Unexpected forLoop in tree.");
      }
    }
  }

  fragmentLocalBlocks();

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->isIterator()) {
      // This collapseBlocks call is required for lowerIterator to inline
      // advance() into zip[1-4]
      fn->collapseBlocks();
      lowerIterator(fn);
    }
  }

  removeUncalledIterators();

  fixNumericalGetMemberPrims();

  cleanupLeaderFollowerIteratorCalls();

  handlePolymorphicIterators();

  reconstructIRautoCopyAutoDestroy();

  cleanupTemporaryVectors();

  iteratorsLowered = true;
}
