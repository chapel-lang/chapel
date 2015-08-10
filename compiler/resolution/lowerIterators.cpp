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
#include "stlUtil.h"
#include "CForLoop.h"
#include "expr.h"
#include "ForLoop.h"
#include "iterator.h"
#include "passes.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"


//
// getTheIteratorFn(): get the original (user-written) iterator function
// that corresponds to an _iteratorClass type or symbol
// or a CallExpr therewith. Its uses were simply:
//
//   ... ->defaultInitializer->getFormal(1)->type->defaultInitializer
//

// 'ic' must be an instance of _iteratorClass
FnSymbol* getTheIteratorFn(Symbol* ic) {
  return getTheIteratorFn(ic->type);
}

FnSymbol* getTheIteratorFn(CallExpr* call) {
  return getTheIteratorFn(call->get(1)->typeInfo());
}

// either an _iteratorClass type or a tuple thereof
FnSymbol* getTheIteratorFn(Type* icType)
{
  // the asserts document the current state
  bool gotTuple = icType->symbol->hasFlag(FLAG_TUPLE);
  INT_ASSERT(gotTuple || icType->symbol->hasFlag(FLAG_ITERATOR_CLASS));

  // The _getIterator function is in _iteratorClass's defaultInitializer.
  FnSymbol* getIterFn = icType->defaultInitializer;

  // The type of _getIterator's first formal arg is _iteratorRecord.
  Type* irType = getIterFn->getFormal(1)->type;
  INT_ASSERT(irType->symbol->hasFlag(FLAG_ITERATOR_RECORD) ||
             (gotTuple && irType->symbol->hasFlag(FLAG_ITERATOR_CLASS)));

  // The original iterator function is in _iteratorRecord's defaultInitializer.
  FnSymbol* result = irType->defaultInitializer;
  INT_ASSERT(gotTuple || result->hasFlag(FLAG_ITERATOR_FN));

  return result;
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
      USR_FATAL_CONT(call, "invalid use of parallel construct in serial iterator");
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
          // NOAKES 2014/11/25 Transitional.  Avoid calling blockInfoGet() on loops
          if (block->isLoopStmt()   == false &&
              block->blockInfoGet() != NULL  &&
              block->blockInfoGet()->isPrimitive(PRIM_BLOCK_UNLOCAL))
            block->blockInfoGet()->remove(); // UNLOCAL applies to a single LOCAL

          else if (block->body.head)
            queue.add(block->body.head);
        } else if (CondStmt* cond = toCondStmt(current)) {
          if (cond->thenStmt && cond->thenStmt->body.head)
            queue.add(cond->thenStmt->body.head);
          if (cond->elseStmt && cond->elseStmt->body.head)
            queue.add(cond->elseStmt->body.head);
        }
      } else if (call && resolvedToTaskFun(call)) {
        // Do what the above would have done to a coforall/etc. block.
        insertNewLocal = true;
        Expr* taskfnBodyHead = call->isResolved()->body->body.head;
        if (taskfnBodyHead)
          queue.add(taskfnBodyHead);
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
                                         std::vector<BaseAST*>& asts) {
  int count = 1;
  for_formals(formal, iterator) {
    for_vector(BaseAST, ast, asts) {
      if (SymExpr* se = toSymExpr(ast)) {
        if (se->var == formal) {
          // count is used to get the nth field out of the iterator class;
          // it is replaced by the field once the iterator class is created
          Expr* stmt = se->getStmtExpr();
          VarSymbol* tmp = newTemp(formal->name, formal->type);
          stmt->insertBefore(new DefExpr(tmp));
          stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, ic, new_IntSymbol(count))));
          se->var = tmp;
        }
      }
    }
    count++;
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
    argBundleFreeFn->insertAtHead(new CallExpr(PRIM_MOVE, cond, new CallExpr(PRIM_EQUAL, loopBodyFnIDArg, new_IntSymbol(ftableMap.get(loopBodyFnWrapper)))));
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
  argBundleCopyFn->insertBeforeReturn(new DefExpr(cond));
  argBundleCopyFn->insertBeforeReturn(new CallExpr(PRIM_MOVE, cond, new CallExpr(PRIM_EQUAL, loopBodyFnIDArg, new_IntSymbol(ftableMap.get(loopBodyFnWrapper)))));
  argBundleCopyFn->insertBeforeReturn(new CondStmt(new SymExpr(cond), block));
}


// This creates the class type carrying the arguments to a (possibly recursive)
// iterator function call and fills a temp with the passed-in values.
//
static AggregateType* 
bundleLoopBodyFnArgsForIteratorFnCall(CallExpr* iteratorFnCall,
                                      CallExpr* loopBodyFnCall,
                                      FnSymbol* loopBodyFnWrapper) {
  FnSymbol* iteratorFn = iteratorFnCall->isResolved();
  FnSymbol* loopBodyFn = loopBodyFnCall->isResolved();

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


static void localizeReturnSymbols(FnSymbol* iteratorFn, std::vector<BaseAST*> asts)
{
  //
  // localize return symbols
  //
  Symbol* ret = iteratorFn->getReturnSymbol();
  Map<BlockStmt*,Symbol*> retReplacementMap;
  for_vector(BaseAST, ast, asts) {
    if (SymExpr* se = toSymExpr(ast)) {
      if (se->var == ret) {
        BlockStmt* block = NULL;
        for (Expr* parent = se->parentExpr; parent; parent = parent->parentExpr) {
          block = toBlockStmt(parent);
          if (block)
            break;
        }
        INT_ASSERT(block);
        if (block != ret->defPoint->parentExpr) {
          if (Symbol* repl = retReplacementMap.get(block)) {
            se->var = repl;
          } else {
            SET_LINENO(se);
            Symbol* newRet = newTemp("newRet", ret->type);
            newRet->addFlag(FLAG_SHOULD_NOT_PASS_BY_REF);
            block->insertAtHead(new DefExpr(newRet));
            se->var = newRet;
            retReplacementMap.put(block, newRet);
          }
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
// will be included in 'asts' and handled when 'fn' is the inclosing
// iterator.
//
static void localizeIteratorReturnSymbols() {
  forv_Vec(FnSymbol, iterFn, gFnSymbols) {
    if (iterFn->inTree() && iterFn->isIterator()) {
      std::vector<BaseAST*> asts;
      collect_asts(iterFn, asts);
      localizeReturnSymbols(iterFn, asts);
    }
  }
}


//
// Convert:
// (248842 CallExpr move
//   (248843 SymExpr 'ret[248825]:[domain(...)] int(64)[803094]')
//   (248839 CallExpr
//     (838015 SymExpr 'fn =[835984]:[domain(...)] int(64)[803094]')
//     (248841 SymExpr 'ret[248825]:[domain(...)] int(64)[803094]')
//     (193499 SymExpr 'p[113796]:[domain(...)] int(64)[803094]'))
// to:
// (248842 CallExpr move
//   (248843 SymExpr 'ret[248825]:[domain(...)] int(64)[803094]')
//   (193499 SymExpr 'p[113796]:[domain(...)] int(64)[803094]'))
//
static void
yieldArraysByRef() {
  forv_Vec(CallExpr, call, gCallExprs) {
    // The ifs are ordered with simpler checks first.
    // Watch out for *initializations* of 'ret' - they look similar.
    if (call->isPrimitive(PRIM_MOVE)) {
      if (FnSymbol* fn = toFnSymbol(call->parentSymbol)) {
        if (fn->isIterator() &&
            fn->hasFlag(FLAG_SPECIFIED_RETURN_TYPE))
        {
          Symbol* ret = fn->getReturnSymbol();
          INT_ASSERT(ret);
          if (ret->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
            if (SymExpr* dest = toSymExpr(call->get(1))) {
              if (dest->var == ret) {
                CallExpr* source = toCallExpr(call->get(2));
                INT_ASSERT(source);
                FnSymbol* sourceFun = source->isResolved();
                INT_ASSERT(sourceFun);
                INT_ASSERT(!strcmp(sourceFun->name, "="));
                SymExpr* sourceArg1 = toSymExpr(source->get(1));
                INT_ASSERT(sourceArg1);
                INT_ASSERT(sourceArg1->var == ret);
                Expr* sourceArg2 = source->get(2);
                INT_ASSERT(sourceArg2);
                // OK, got it. Do the replacement.
                source->replace(sourceArg2->remove());
              }
            }
          }
        }
      }
    }
  }
}


// processIteratorYields is a separate pass, called before flattenFunctions.
// TODO: Move this and supporting functions into their own source file.
void processIteratorYields() {
  yieldArraysByRef();
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
static void convertYieldsAndReturns(std::vector<BaseAST*>& asts, Symbol* index,
                                    ArgSymbol* loopBodyFnIDArg, ArgSymbol* loopBodyFnArgArgs)
{
  for_vector(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
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
        INT_ASSERT(call->isResolved() == taskFn);
        std::vector<BaseAST*> taskAsts;
        collect_asts(taskFn, taskAsts);
        convertYieldsAndReturns(taskAsts, index, tIDArg, tArgArgs);
      }
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
  std::vector<BaseAST*> asts;
  collect_asts(iteratorFn, asts);
  ArgSymbol* icArg = new ArgSymbol(blankIntentForType(ic->type), "_ic", ic->type);
  iteratorFn->insertFormalAtTail(icArg);
  replaceIteratorFormalsWithIteratorFields(iterator, icArg, asts);
  ArgSymbol* loopBodyFnIDArg = new ArgSymbol(INTENT_CONST_IN, "_loopBodyFnID", dtInt[INT_SIZE_DEFAULT]);
  iteratorFn->insertFormalAtTail(loopBodyFnIDArg);
  ArgSymbol* loopBodyFnArgArgs = new ArgSymbol(INTENT_CONST_IN, "_loopBodyFnArgs", argsBundleType);
  iteratorFn->insertFormalAtTail(loopBodyFnArgArgs);

  localizeReturnSymbols(iteratorFn, asts);

  convertYieldsAndReturns(asts, index, loopBodyFnIDArg, loopBodyFnArgArgs);

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
  Symbol*    index             = forLoop->indexGet()->var;
  ArgSymbol* indexArg          = new ArgSymbol(blankIntentForType(index->type), "_index", index->type);

  // The recursive iterator loop wrapper is ... .
  FnSymbol*  loopBodyFnWrapper = new FnSymbol(astr("_rec_iter_loop_wrapper_", parent->name));

  loopBodyFn->insertFormalAtTail(indexArg);

  forLoop->insertBefore(new DefExpr(loopBodyFn));

  parent->defPoint->insertBefore(new DefExpr(loopBodyFnWrapper));

  ftableVec.add(loopBodyFnWrapper);
  ftableMap.put(loopBodyFnWrapper, ftableVec.n-1);

  //
  // insert a call to the iterator function (using iterator as a
  // placeholder); build a call to loopBodyFn which will be
  // removed later and its arguments passed to the iteratorFn (we
  // build this to capture the actual arguments that should be
  // passed to it when this function is flattened)
  //
  Symbol*    ic             = forLoop->iteratorGet()->var;
  FnSymbol*  iterator       = getTheIteratorFn(ic);
  CallExpr*  iteratorFnCall = new CallExpr(iterator, ic, new_IntSymbol(ftableMap.get(loopBodyFnWrapper)));

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
  Vec<FnSymbol*> nestedFunctions;

  nestedFunctions.add(loopBodyFn);

  flattenNestedFunctions(nestedFunctions);

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
                            bool           removeReturn,
                            TaskFnCopyMap& taskFnCopies);

/// \param call A for loop block primitive.
static bool
// Returns true if the given ForLoop was handled (converted and removed from
// the tree); false otherwise.
expandIteratorInline(ForLoop* forLoop) {
  Symbol*   ic       = forLoop->iteratorGet()->var;
  FnSymbol* iterator = getTheIteratorFn(ic);

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
    } else {
      expandRecursiveIteratorInline(forLoop);
      INT_ASSERT(!forLoop->inTree());
      return true;
    }

  } else {
    SET_LINENO(forLoop);

    Symbol*       index = forLoop->indexGet()->var;
    BlockStmt*    ibody = iterator->body->copy();
    std::vector<BaseAST*> asts;

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

    collect_asts(ibody, asts);

    replaceIteratorFormalsWithIteratorFields(iterator, ic, asts);

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

  expandBodyForIteratorInline(forLoop, ibody, index, true, taskFnCopies);
}

static void
expandBodyForIteratorInline(ForLoop*       forLoop,
                            BlockStmt*     ibody,
                            Symbol*        index,
                            bool           removeReturn,
                            TaskFnCopyMap& taskFnCopies) {
  std::vector<BaseAST*> asts;

  collect_asts(ibody, asts);

  for_vector(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIM_YIELD)) {
        Symbol*    yieldedIndex  = newTemp("_yieldedIndex", index->type);
        Symbol*    yieldedSymbol = toSymExpr(call->get(1))->var;
        BlockStmt* bodyCopy      = NULL;
        bool       inserted      = false;

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
              if (lhs->var == yieldedSymbol) {
                lhs->var = yieldedIndex;

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
      }

      if (call->isPrimitive(PRIM_RETURN)) {
        if (removeReturn)
          call->remove();
      }

      if (FnSymbol* cfn = resolvedToTaskFun(call)) {
        // Todo: skip this handling of 'cfn' if it does not have yields
        // in itself or any other taskFns it may call.

        // This holds because we flatten everything right away.
        // We need it so that we can place the def of 'fcopy' anywhere
        // while preserving correct scoping of its SymExprs.
        INT_ASSERT(isGlobal(cfn));

        FnSymbol*      fcopy = taskFnCopies.get(cfn);
        Vec<FnSymbol*> nestedFnVec;

        if (!fcopy) {
          // Clone the function. Just once per 'body' should suffice.
          fcopy = cfn->copy();

          // Note that 'fcopy' will likely get a copy of 'body',
          // so we need to preserve correct scoping of its SymExprs.
          call->insertBefore(new DefExpr(fcopy));

          // I don't expect invocation of expandBodyForIteratorInline() below
          // to encounter another call to 'cfn'. But even if does, 'fcopy'
          // will be fetched from 'taskFnCopies', avoiding recursion.
          taskFnCopies.put(cfn, fcopy);

          // Repeat, recursively.
          expandBodyForIteratorInline(forLoop, fcopy->body, index, false, taskFnCopies);

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

        // Note: this is an expensive operation due to compute_call_sites().
        // We do it because it may eliminate further cloning of 'fcopy'
        // e.g. when the enclosing fn or block are copied for any reason.
        // Ideally, replace with flattenOneFunction().
        nestedFnVec.add(fcopy);

        flattenNestedFunctions(nestedFnVec);
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
static unsigned
countYieldsInFn(FnSymbol* fn)

{
  unsigned count = 0;
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
// It can be inlined if it contains exactly one yield statement.
static bool
canInlineIterator(FnSymbol* iterator) {
  unsigned count = countYieldsInFn(iterator);

  // count==0 e.g. in users/biesack/test_recursive_iterator.chpl
  return (count == 1) ? true : false;
}


static void
getRecursiveIterators(Vec<Symbol*>& iterators, Symbol* gIterator) {
  if (gIterator->type->symbol->hasFlag(FLAG_TUPLE)) {
    AggregateType* iteratorType = toAggregateType(gIterator->type);

    for (int i=1; i <= iteratorType->fields.length; i++) {
      Symbol *iterator = toSymbol(iteratorType->getField(i));

      if (iterator)
        getRecursiveIterators(iterators, iterator);
    }
  }
  else {
    iterators.add(gIterator);
  }
}


static bool
canInlineSingleYieldIterator(Symbol* gIterator) {
  Vec<Symbol*> iterators;

  getRecursiveIterators(iterators, gIterator);

  for (int i = 0; i < iterators.n; i++) {
    FnSymbol*      iterator = getTheIteratorFn(iterators.v[i]);
    BlockStmt*     block    = iterator->body;
    Vec<CallExpr*> calls;
    int            numYields = 0;

    INT_ASSERT(block);

    // TODO: std::vector::iterator does not guarantee correct iteration if the
    // capacity of the vector it refers to increases during iteration.
    // Replace this loop with an equivalent predicate that uses std::vector in
    // a valid fashion.
    collectCallExprs(block, calls);
    forv_Vec(CallExpr, call, calls) {
      if (call && call->isPrimitive(PRIM_YIELD)) {
        numYields++;

        if (iterator->body != call->parentExpr)
          return false;
      }

      if (call) {
        if (FnSymbol* taskFn = resolvedToTaskFun(call)) {
          // Need to descend into 'taskFn' - append to 'asts'.
          // If there are any yields there, they will trigger
          // 'return false' above.
          collectCallExprs(taskFn->body, calls);
        }
      }
    }

    if (numYields != 1)
      return false;
  }

  return true;
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


static void
getIteratorChildren(Vec<Type*>& children, Type* type) {
  forv_Vec(Type, child, type->dispatchChildren) {
    if (child != dtObject) {
      children.add_exclusive(child);
      getIteratorChildren(children, child);
    }
  }
}

#define ZIP1 1
#define ZIP2 2
#define ZIP3 3
#define ZIP4 4
#define HASMORE 5
#define GETVALUE 6
#define INIT 7
#define INCR 8

static void
buildIteratorCallInner(BlockStmt* block, Symbol* ret, int fnid, Symbol* iterator) {
  IteratorInfo* ii = getTheIteratorFn(iterator)->iteratorInfo;
  FnSymbol* fn = NULL;
  switch (fnid) {
  case ZIP1: fn = ii->zip1; break;
  case ZIP2: fn = ii->zip2; break;
  case ZIP3: fn = ii->zip3; break;
  case ZIP4: fn = ii->zip4; break;
  case HASMORE: fn = ii->hasMore; break;
  case GETVALUE: fn = ii->getValue; break;
  case INIT: fn = ii->init; break;
  case INCR: fn = ii->incr; break;
  }
  CallExpr* call = new CallExpr(fn, iterator);
  if (ret) {
    if (fn->retType == ret->type) {
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


static void
inlineSingleYieldIterator(ForLoop* forLoop) {
  SET_LINENO(forLoop);

  SymExpr*     se1      = forLoop->indexGet();
  VarSymbol*   index    = toVarSymbol(se1->var);

  SymExpr*     se2      = forLoop->iteratorGet();
  VarSymbol*   iterator = toVarSymbol(se2->var);

  CallExpr*    noop     = new CallExpr(PRIM_NOOP);

  Vec<Symbol*> iterators;
  Vec<Symbol*> indices;

  setupSimultaneousIterators(iterators, indices, iterator, index, forLoop);

  forLoop->insertAtHead(noop);

  for (int i = 0; i < iterators.n; i++) {
    FnSymbol*     iterator   = getTheIteratorFn(iterators.v[i]);
    BlockStmt*    ibody      = iterator->body->copy();
    bool          afterYield = false;
    int           count      = 1;
    std::vector<BaseAST*> asts;

    collect_asts(ibody, asts);

    for_alist(expr, ibody->body) {
      if (CallExpr* curr_expr = toCallExpr(expr)) {
        if (curr_expr->isPrimitive(PRIM_YIELD)) {
          afterYield = true;
          noop->insertAfter(new CallExpr(PRIM_MOVE, indices.v[i], curr_expr->get(1)->remove()));
        } else if (!curr_expr->isPrimitive(PRIM_RETURN)) {
          if (resolvedToTaskFun(curr_expr))
            // what should we do in this case?
            INT_FATAL(curr_expr, "inlineSingleYieldIterator is not implemented for outlined coforall/cobegin/begin blocks");

          else if (!afterYield)
            noop->insertBefore(curr_expr->remove());

          else
            forLoop->insertAtTail(curr_expr->remove());
        }
      } else {
        if (!afterYield)
          noop->insertBefore(expr->remove());
        else
          forLoop->insertAtTail(expr->remove());
      }
    }

    for_formals(formal, iterator) {
      for_vector(BaseAST, ast, asts) {
        if (SymExpr* se = toSymExpr(ast)) {
          if (se->var == formal) {
            //if ((se->var->type == formal->type) && (!strcmp(se->var->name, formal->name))) {
            // count is used to get the nth field out of the iterator class;
            // it is replaced by the field once the iterator class is created
            Expr*      stmt = se->getStmtExpr();
            VarSymbol* tmp  = newTemp(formal->name, formal->type);

            stmt->insertBefore(new DefExpr(tmp));
            stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, iterators.v[i], new_IntSymbol(count))));

            se->var = tmp;
          }
        }
      }

      count++;
    }
  }

  noop->remove();

  // Create a true BlockStmt for the body of the ForLoop
  BlockStmt* body = forLoop->copyBody();

  forLoop->replace(body);
}


// Replace a ForLoop with its inline equivalent, if possible.
// Otherwise, convert it into a C-style for loop.
// The given forLoop is converted unconditionally.
static void
expandForLoop(ForLoop* forLoop) {
  SymExpr*   se2      = forLoop->iteratorGet();
  VarSymbol* iterator = toVarSymbol(se2->var);
  bool converted = false;

  if (!fNoInlineIterators)
  {
    FnSymbol* iterFn = getTheIteratorFn(iterator->type);
    if (iterFn->iteratorInfo && canInlineIterator(iterFn) &&
        (iterator->type->dispatchChildren.n == 0 ||
         (iterator->type->dispatchChildren.n == 1 &&
          iterator->type->dispatchChildren.v[0] == dtObject))) {
      converted = expandIteratorInline(forLoop);
    } 
    // Conversion fails above if there is a recursive iterator call in a
    // recursive iterator, or if there is a task function call in a recursive
    // iterator.  In either case, it seems like a bad idea to try to perform
    // inlineSingleYieldIterator() on the same forLoop.  That is the reason for
    // the unequivocal "else".
    // To try the other olternative, replace the following line with:
    // if (!converted && canInlineSingleYieldIterator(iterator) &&
    else if (canInlineSingleYieldIterator(iterator) &&
            (iterator->type->dispatchChildren.n == 0 ||
             (iterator->type->dispatchChildren.n == 1 &&
              iterator->type->dispatchChildren.v[0] == dtObject))) {
      inlineSingleYieldIterator(forLoop);
      converted = true;
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
    //     more = hasMore(itertor1);
    //   }
    //   zip4(_iterator2); zip4(_iterator1);
    // In zippered iterators, each clause may contain multiple calls to zip1(),
    // getValue(), etc.  These are inserted in the order shown.

    SET_LINENO(forLoop);

    Vec<Symbol*> iterators;
    Vec<Symbol*> indices;

    SymExpr*     se1       = toSymExpr(forLoop->indexGet());
    VarSymbol*   index     = toVarSymbol(se1->var);

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
          // for all but the first iterator add checks at the begining of each loop run
          // and a final one after to make sure the other iterators don't finish before
          // the "leader" and they don't have more afterwards.
          VarSymbol* hasMore    = newTemp("hasMore",    dtBool);
          VarSymbol* isFinished = newTemp("isFinished", dtBool);

          forLoop->insertBefore(new DefExpr(isFinished));
          forLoop->insertBefore(new DefExpr(hasMore));

          forLoop->insertAtHead(new CondStmt(new SymExpr(isFinished),
                                             new CallExpr(PRIM_RT_ERROR,
                                                          new_StringSymbol("zippered iterations have non-equal lengths"))));

          forLoop->insertAtHead(new CallExpr(PRIM_MOVE, isFinished, new CallExpr(PRIM_UNARY_LNOT, hasMore)));

          forLoop->insertAtHead(buildIteratorCall(hasMore, HASMORE, iterators.v[i], children));

          forLoop->insertAfter(new CondStmt(new SymExpr(hasMore),
                                            new CallExpr(PRIM_RT_ERROR,
                                                         new_StringSymbol("zippered iterations have non-equal lengths"))));

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
    // if this is the case.
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
// Select those whose _getIterator() functions have the FLAG_ITERATOR_INLINE.
// Inline the selected iterators at their call sites.
static void
inlineIterators() {
  forv_Vec(BlockStmt, block, gBlockStmts) {
    // Skip blocks that are no longer in the tree.
    if (block->parentSymbol == NULL)
      continue;

    if (ForLoop* forLoop = toForLoop(block)) {
      Symbol*   iterator = toSymExpr(forLoop->iteratorGet())->var;
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


static bool iteratorDefPrecedesGoto(CallExpr* freeIterCall,
                                    GotoStmt* stmt,
                                    BlockStmt* block)
{
  // The iterator being freed is the argument of the _freeIterator call.
  SymExpr* iterator = toSymExpr(freeIterCall->get(1));

  std::vector<Expr*> exprs;
  collectExprs(block, exprs);
  for_vector(Expr, expr, exprs)
  {
    if (expr == stmt)
      // We found the goto first.
      // The iterator is not live, so there is no need to delete it.
      return false;

    if (CallExpr* call = toCallExpr(expr))
    {
      if (call->isPrimitive(PRIM_MOVE))
      {
        SymExpr* lhs = toSymExpr(call->get(1));
        if (lhs->var == iterator->var)
          // This is a def of the iterator symbol, so it is initialized before
          // the goto statement.
          return true;
      }
    }
  }
  // We never saw a definition for the iterator, so the safe thing to do is to
  // not delete it.
  return false;
}


static void addCrossedFreeIteratorCalls(GotoStmt* stmt)
{
  // Examine the target label of the goto and find the block containing the
  // label's definition.
  SymExpr* lsym = toSymExpr(stmt->label);
  INT_ASSERT(lsym); // These should always have a target label.
  DefExpr* label = lsym->var->defPoint;
  Expr* top_scope = toBlockStmt(label->parentExpr);
  // We expect the top scope to exist and be a block and not be the root scope.
  INT_ASSERT(top_scope);

  // Now traverse scopes outward from the block containing this goto statement.
  Expr* last_scope = stmt;  // Track the scope we just exited.
  Expr* scope = stmt->parentExpr;
  while (scope && scope != top_scope)
  {
    if (BlockStmt* block = toBlockStmt(scope))
    {
      for_alist_backward(expr, block->body)
      {
        // If we encounter the scope we just exited from, we're done.
        // The _freeIterator call always lies in the suffix of the scope
        // containing the loop construct.
        if (expr == last_scope)
          break;

        if (CallExpr* call = toCallExpr(expr))
        {
          FnSymbol* fn = call->isResolved();
          // Naturally, a flag is preferred.
          if (fn && !strcmp(fn->name, "_freeIterator"))
          {
            // Don't bother with iterators that haven't been initialized yet.
            if (iteratorDefPrecedesGoto(call, stmt, block))
            {
              // OK, we found a _freeIterator call that will be crossed,
              // so duplicate it just before the break or return.
              SET_LINENO(call);
              stmt->insertBefore(call->copy());
            }
          }
        }
      }
    }
    last_scope = scope;
    scope = last_scope->parentExpr;
  }

  // We expect the loop to terminate after we have found the top scope.  If it
  // terminates for another reason (like popping out of the top of the function
  // or encountering a non-block parent expression, then we fail with an error.
  INT_ASSERT(scope == top_scope);
}


// This routine is called to fix up GotoStmts (breaks and returns) whose target
// labels lie outside of "body".  This fixup consists of calling the
// _freeIterator function for any iterator class (ic) that goes out of scope as
// a result of the corresponding goto.  The goto "crosses" the _freeIterator
// call at the end of the block.  That is, since the normal exit code is
// skipped, the _freeIterator call is never reached.  This problem is solved by
// copying _freeIterator calls that will be crossed just before each break or
// return statement that causes it to be crossed.
//
// The algorithm is straightforward:
// To determine a stopping point, we find the block containing the DefExpr where
// the target label is defined.  After the body of the loop has been inlined in
// the iterator which has been inlined in the caller, we expect this block to
// contain (perhaps many levels down) the break or return statement causing
// "body" to be exited.
// Then, starting from each break or return statement, we search outward through
// all containing blocks until we match the outermost block.  Along the way, if
// we encounter a block which is a for loop, we insert a call to the
// _freeIterator function directly ahead of the break or return.  Note that
// these will appear in reverse nesting order (as they should).
//
static void addCrossedFreeIteratorCalls()
{
  // Walk all goto statments.
  forv_Vec(GotoStmt, stmt, gGotoStmts)
  {
    // Ignore goto statments that are not in the tree.
    if (!stmt->parentSymbol)
      continue;

    // We don't yet know how to deal with zippered iteration,
    // so just leak memory and move on.
    if (!strcmp(stmt->parentSymbol->name,"advance"))
      continue;

    switch(stmt->gotoTag)
    {
     default:
      break; // We only care about breaks and returns.
     case GOTO_NORMAL:
      INT_FATAL(stmt, "User gotos are not supported.");
      break;
     case GOTO_BREAK:
     case GOTO_RETURN:
      // Add crossed free iterator calls for this statement.
      addCrossedFreeIteratorCalls(stmt);
      break;
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
        Symbol* local = toSymExpr(parent->get(1))->var;
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
      if (FnSymbol* fn = call->isResolved()) {
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
              if (isArgSymbol(se->var) && call->parentSymbol != se->var->defPoint->parentSymbol) {
                Symbol* field = toAggregateType(iteratorType)->getField(i);
                VarSymbol* tmp = NULL;
                SET_LINENO(call);
                if (field->type == se->var->type) {
                  tmp = newTemp(field->name, field->type);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, iterator, field)));
                } else if (field->type->refType == se->var->type) {
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
        getIterator->insertBeforeReturn(new DefExpr(label));
        Symbol* ret = getIterator->getReturnSymbol();
        forv_Vec(Type, type, irecord->dispatchChildren) {
          VarSymbol* tmp = newTemp(irecord->getField(1)->type);
          VarSymbol* cid = newTemp(dtBool);
          BlockStmt* thenStmt = new BlockStmt();
          VarSymbol* recordTmp = newTemp("recordTmp", type);
          VarSymbol* classTmp = newTemp("classTmp", type->defaultInitializer->iteratorInfo->getIterator->retType);
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
          thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, classTmp, new CallExpr(type->defaultInitializer->iteratorInfo->getIterator, recordTmp)));
          thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, ret, new CallExpr(PRIM_CAST, ret->type->symbol, classTmp)));
          thenStmt->insertAtTail(new GotoStmt(GOTO_GETITER_END, label));
          ret->defPoint->insertAfter(new CondStmt(new SymExpr(cid), thenStmt));
          ret->defPoint->insertAfter(new CallExpr(PRIM_MOVE, cid, new CallExpr(PRIM_TESTCID, tmp, type->defaultInitializer->iteratorInfo->irecord->getField(1)->type->symbol)));
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
    if (FnSymbol* autoCopy = autoCopyMap.get(field->type)) {
      Symbol* tmp1 = newTemp(field->name, field->type);
      Symbol* tmp2 = newTemp(autoCopy->retType);
      block->insertAtTail(new DefExpr(tmp1));
      block->insertAtTail(new DefExpr(tmp2));
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp1, new CallExpr(PRIM_GET_MEMBER_VALUE, arg, field)));
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp2, new CallExpr(autoCopy, tmp1)));
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

      if (!fNoDeadCodeElimination)
        deadCodeElimination(fn);
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

  addCrossedFreeIteratorCalls();

  fixNumericalGetMemberPrims();

  cleanupLeaderFollowerIteratorCalls();

  handlePolymorphicIterators();

  reconstructIRautoCopyAutoDestroy();

  cleanupTemporaryVectors();
}

