#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "resolution.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "iterator.h"


// This consistency check should probably be moved earlier in the compilation.
// Does it need to be recursive? (Currently, it is not.)
static void nonLeaderParCheck()
{
  //
  // check for parallel constructs in non-leader iterators
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ITERATOR_FN) && !fn->hasFlag(FLAG_INLINE_ITERATOR)) {
      Vec<CallExpr*> calls;
      collectCallExprs(fn, calls);
      forv_Vec(CallExpr, call, calls) {
        if ((call->isPrimitive(PRIM_BLOCK_BEGIN)) ||
            (call->isPrimitive(PRIM_BLOCK_COBEGIN)) ||
            (call->isPrimitive(PRIM_BLOCK_COFORALL)) ||
            (call->isNamed("_toLeader"))) {
          USR_FATAL_CONT(call, "invalid use of parallel construct in serial iterator");
        }
        if ((call->isPrimitive(PRIM_BLOCK_ON)) ||
            (call->isPrimitive(PRIM_BLOCK_ON_NB))) {
          BlockStmt* onBlock = toBlockStmt(call->parentExpr);
          if (!onBlock || onBlock->blockInfo != call) {
            INT_FATAL(call, "mis-assumption about structure of on blocks in "
                      "lowerIterators pass");
          }
          Vec<CallExpr*> callsWithinOn;
          collectCallExprs(onBlock, callsWithinOn);
          forv_Vec(CallExpr, call, callsWithinOn) {
            if (call->isPrimitive(PRIM_YIELD)) {
              USR_FATAL_CONT(call, "invalid use of 'yield' within 'on' in serial iterator");
            }
          }
        }
      }
    }
  }
  USR_STOP();
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
// Mark as recursive those iterators which call themselves (directly or indirectly).
// When dealing with recursive iterators, we only inline the body of the loop,
// not the iterator as a whole.
//
static void computeRecursiveIteratorSet() {
  compute_call_sites();

  // Walk all functions
  forv_Vec(FnSymbol, iter, gFnSymbols)
  {
    // And select just the iterators.
    if (!iter->hasFlag(FLAG_ITERATOR_FN))
      continue;

    // Determine if the iterator calls itself, either directly or indirectly.
    Vec<FnSymbol*> fnSet;   // Used to avoid recursion
    if (find_recursive_caller(iter, iter, fnSet))
      // If so, add the recursive iterator flag.
      iter->addFlag(FLAG_RECURSIVE_ITERATOR);
  }
}


//
// If a local block has no yields, returns, gotos or labels
// then it can safely be left unfragmented.
// A block whose blockInfo is PRIM_BLOCK_UNLOCAL must also be fragmented.
//
static bool leaveLocalBlockUnfragmented(BlockStmt* block) {
  Vec<BaseAST*> asts;
  collect_asts(block, asts);

  forv_Vec(BaseAST, ast, asts) {

    // Check if the AST element is a GOTO.
    if (isGotoStmt(ast))
      return false;     // Yes, FAIL.

    // See if it is a yield or return statement.
    CallExpr* call = toCallExpr(ast);
    if (call &&
        (call->isPrimitive(PRIM_YIELD) || call->isPrimitive(PRIM_RETURN)))
      return false;     // Yes, FAIL.

    // See if it is an unlocal block.
    BlockStmt* block = toBlockStmt(ast);
    if (block && block->blockInfo &&
        block->blockInfo->isPrimitive(PRIM_BLOCK_UNLOCAL))
      return false;     // Yes, FAIL.
    
    // See if it is a label.
    DefExpr* def = toDefExpr(ast);
    if (def && isLabelSymbol(def->sym))
      return false;     // Yes, FAIL.
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
    if (block->parentSymbol &&
        block->blockInfo &&
        block->blockInfo->isPrimitive(PRIM_BLOCK_LOCAL) &&
        !leaveLocalBlockUnfragmented(block))
      localBlocks.add(block);
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
      bool insertNewLocal = false;
      CallExpr* call = toCallExpr(current);
      DefExpr* def = toDefExpr(current);

      //
      // if this statement is a yield, a return, a label definition, a
      // goto, a conditional, or a block, insert a new local block
      // that contains all the statements seen to this point (by
      // setting insertNewLocal to true) and add the first statements
      // of blocks and conditional blocks to the queue; otherwise, if
      // this statement is a definition, add it to preVec; otherwise,
      // add this statement to inVec.
      //
      if ((call && (call->isPrimitive(PRIM_YIELD) ||
                    call->isPrimitive(PRIM_RETURN))) ||
          (def && isLabelSymbol(def->sym)) ||
          isGotoStmt(current) ||
          isCondStmt(current) ||
          isBlockStmt(current)) {
        insertNewLocal = true;
        if (BlockStmt* block = toBlockStmt(current)) {
          if (block->blockInfo && block->blockInfo->isPrimitive(PRIM_BLOCK_UNLOCAL))
            block->blockInfo->remove(); // UNLOCAL applies to a single LOCAL
          else if (block->body.head)
            queue.add(block->body.head);
        } else if (CondStmt* cond = toCondStmt(current)) {
          if (cond->thenStmt && cond->thenStmt->body.head)
            queue.add(cond->thenStmt->body.head);
          if (cond->elseStmt && cond->elseStmt->body.head)
            queue.add(cond->elseStmt->body.head);
        }
      } else if (isDefExpr(current)) {
        preVec.add(current);
      } else {
        inVec.add(current);
      }
      // Note that yield and return statements are removed, as are
      // gotos and labels.  Blocks and conditionals are unwrapped.

      //
      // if ready to insert a new local block or we are on the last
      // statement in a block, insert a new local block containing all
      // the statements in inVec; move all the statements in preVec to
      // a point just before this new local block.
      //
      if (insertNewLocal || !current->next) {
        if (preVec.n || inVec.n) {
          BlockStmt* newLocalBlock = new BlockStmt();
          newLocalBlock->blockInfo = new CallExpr(PRIM_BLOCK_LOCAL);
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
    block->blockInfo->remove();
  }
}


// In the body of the iterator, replace references to the iterator formals
// with references to fields in the iterator class.
//
// Multiple temps may be created for each formal.
static void
replaceIteratorFormalsWithIteratorFields(FnSymbol* iterator, Symbol* ic,
                                         Vec<BaseAST*>& asts) {
  int count = 1;
  for_formals(formal, iterator) {
    forv_Vec(BaseAST, ast, asts) {
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
static Vec<FnSymbol*> iteratorWithOnStatementSet;
static FnSymbol* argBundleCopyFn = NULL;
static FnSymbol* argBundleFreeFn = NULL;


// Creates a function to free the arg bundle associated with a recursive iterator.
//
// This is currently done using just one free function for all arg bundles.
// There is (effectively) a switch statement inside the function that selects
// the appropriate bundle based on a function ID inserted into the last field
//  and frees its memory.
//
// The function looks approximately like:
// void chpl__freeRecursiveIteratorArgumentBundle(int functionID, 
//          struct _fn_arg_bundle* bdl) {
//   chpl_free(bdl);
//   return void;
// }
//
static void
createArgBundleFreeFn(ClassType* ct, FnSymbol* loopBodyFnWrapper) {
  if (argBundleFreeFn == NULL) {
    // Create the shared function that frees recursive argument bundles.
    argBundleFreeFn = new FnSymbol("chpl__freeRecursiveIteratorArgumentBundle");
    argBundleFreeFn->retType = dtVoid;
    argBundleFreeFn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_loopBodyFnID", dtInt[INT_SIZE_DEFAULT]));
    argBundleFreeFn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_loopBodyFnArgs", ct));
    argBundleFreeFn->insertAtTail(new CallExpr(PRIM_CHPL_FREE, argBundleFreeFn->getFormal(2)));
    argBundleFreeFn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
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
      VarSymbol* recursiveArgsTmp = newTemp("recursiveArgs", field->getValType());
      block->insertAtTail(new DefExpr(recursiveArgsTmp));
      block->insertAtTail(new CallExpr(PRIM_MOVE, recursiveArgsTmp, new CallExpr(PRIM_DEREF, recursiveArgsRef)));

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
// the appropriate bundle based on ... and copies that part.
//
// The created function has the signature:
//  chpl__copyRecursiveIteratorArgumentBundle(_loopBodyFnID:int, _loopBodyFnArgs:ct);
// where ct is the passed in ClassType.
//
static void
createArgBundleCopyFn(ClassType* ct, FnSymbol* loopBodyFnWrapper) {
  if (argBundleCopyFn == NULL) {
    // Create the shared function that copies recursive argument bundles.
    // The initial implementation is just a shell.  After creation, it looks like:
    //  chpl__copyRecursiveIteratorArgumentBundle(int _loopBodyFnID,
    //                                            ct* _loopBodyFnArgs)
    //  {
    //    ct* tmp = nil;
    //    return tmp;
    //  }
    argBundleCopyFn = new FnSymbol("chpl__copyRecursiveIteratorArgumentBundle");
    argBundleCopyFn->retType = ct;
    argBundleCopyFn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_loopBodyFnID", dtInt[INT_SIZE_DEFAULT]));
    argBundleCopyFn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_loopBodyFnArgs", ct));
    Symbol* tmp = newTemp("dummyBundle", ct);
    argBundleCopyFn->insertAtTail(new DefExpr(tmp));
    argBundleCopyFn->insertAtTail(new CallExpr(PRIM_MOVE, tmp, gNil));
    argBundleCopyFn->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
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
  block->insertAtTail(new CallExpr(PRIM_MOVE, argBundle,
                        new CallExpr(PRIM_CHPL_ALLOC_PERMIT_ZERO,
                                     ct->symbol,
                                     newMemDesc("compiler-inserted argument bundle"))));
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
      VarSymbol* recursiveArgs = newTemp("recursiveArgs", field->getValType());
      block->insertAtTail(new DefExpr(copy));
      block->insertAtTail(new DefExpr(recursiveFnID));
      block->insertAtTail(new DefExpr(recursiveArgs));
      block->insertAtTail(new CallExpr(PRIM_MOVE, recursiveFnID, new CallExpr(PRIM_DEREF, lastTmp)));
      block->insertAtTail(new CallExpr(PRIM_MOVE, recursiveArgs, new CallExpr(PRIM_DEREF, tmp)));
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
static ClassType* 
bundleLoopBodyFnArgsForIteratorFnCall(CallExpr* iteratorFnCall,
                                      CallExpr* loopBodyFnCall,
                                      FnSymbol* loopBodyFnWrapper) {
  FnSymbol* iteratorFn = iteratorFnCall->isResolved();
  FnSymbol* loopBodyFn = loopBodyFnCall->isResolved();

  // Create the argument bundle type
  ClassType* ct = new ClassType(CLASS_CLASS);
  TypeSymbol* ts = new TypeSymbol("_fn_arg_bundle", ct);
  ts->addFlag(FLAG_NO_OBJECT);
  ts->addFlag(FLAG_LOOP_BODY_ARGUMENT_CLASS);
  iteratorFnCall->parentSymbol->defPoint->insertBefore(new DefExpr(ts));

  // And its corresponding reference type.
  ClassType* rct = new ClassType(CLASS_CLASS);
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
  iteratorFnCall->insertBefore(new CallExpr(PRIM_MOVE, argBundle,
                                 new CallExpr(PRIM_CHPL_ALLOC_PERMIT_ZERO,
                                              ts,
                                              newMemDesc("compiler-inserted argument bundle"))));
  iteratorFnCall->insertAtTail(argBundle);
  iteratorFnCall->insertAfter(new CallExpr(PRIM_CHPL_FREE, argBundle));

  // loopBodyWrapper(int index, ct* fn_args) {
  //   loopBodyFn(index);
  // }
  ArgSymbol* wrapperIndexArg = loopBodyFn->getFormal(1)->copy();
  loopBodyFnWrapper->insertFormalAtTail(wrapperIndexArg);
  ArgSymbol* wrapperArgsArg = new ArgSymbol(INTENT_BLANK, "fn_args", ct);
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
        VarSymbol* loopBodyFnArgsTmp = newTemp("loopBodyFnArgsTmp", field->getValType());
        iteratorFnCall->insertBefore(new DefExpr(tmp));
        iteratorFnCall->insertBefore(new DefExpr(recursiveFnID));
        iteratorFnCall->insertBefore(new DefExpr(loopBodyFnArgsTmp));
        iteratorFnCall->insertBefore(new CallExpr(PRIM_MOVE, recursiveFnID, new CallExpr(PRIM_DEREF, lastActual->copy())));
        iteratorFnCall->insertBefore(new CallExpr(PRIM_MOVE, loopBodyFnArgsTmp, new CallExpr(PRIM_DEREF, actual)));
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
  createArgBundleCopyFn(ct, loopBodyFnWrapper);
  createArgBundleFreeFn(ct, loopBodyFnWrapper);
  return ct;
}


//
// return the number of local blocks that lexically enclose 'expr',
// stopping the search outward when encountering the outer block
//
static int
countEnclosingLocalBlocks(Expr* expr, BlockStmt* outer = NULL) {
  int count = 0;
  for (Expr* tmp = expr; tmp && tmp != outer; tmp = tmp->parentExpr) {
    if (BlockStmt* blk = toBlockStmt(tmp)) {
      if (blk->blockInfo && blk->blockInfo->isPrimitive(PRIM_BLOCK_LOCAL))
        count++;
      if (blk->blockInfo && blk->blockInfo->isPrimitive(PRIM_BLOCK_UNLOCAL))
        count--;
    }
  }
  return count;
}


static void localizeReturnSymbols(FnSymbol* iteratorFn, Vec<BaseAST*> asts)
{
  //
  // localize return symbols
  //
  Symbol* ret = iteratorFn->getReturnSymbol();
  Map<BlockStmt*,Symbol*> retReplacementMap;
  forv_Vec(BaseAST, ast, asts) {
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
            Symbol* newRet = newTemp("newRet", ret->type);
            block->insertAtHead(new CallExpr(PRIM_MOVE, newRet, ret));
            block->insertAtHead(new DefExpr(newRet));
            se->var = newRet;
            retReplacementMap.put(block, newRet);
          }
        }
      }
    }
  }
}


// Replace each yield with a call to the loop body function,
// and remove return statements.
static void convertYieldsAndReturns(Vec<BaseAST*>& asts, Symbol* index,
                                    ArgSymbol* loopBodyFnIDArg, ArgSymbol* loopBodyFnArgArgs)
{
  forv_Vec(BaseAST, ast, asts) {
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
          blk->blockInfo = new CallExpr(PRIM_BLOCK_UNLOCAL);
          callOrBlk = blk;
        }
        call->replace(callOrBlk);
      } else if (call->isPrimitive(PRIM_RETURN)) {
        // Just remove a return.
        call->remove();
      }
    }
  }
}


// Returns true if the given function contains an on statement; false otherwise.
static bool fnContainsOn(FnSymbol* fn)
{
  Vec<CallExpr*> calls;
  collectCallExprs(fn, calls);
  forv_Vec(CallExpr, call, calls) {
    if (call->isPrimitive(PRIM_BLOCK_ON) ||
        call->isPrimitive(PRIM_BLOCK_ON_NB))
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
  ClassType* argsBundleType =
    bundleLoopBodyFnArgsForIteratorFnCall(iteratorFnCall, loopBodyFnCall, loopBodyFnWrapper);

  iteratorFn->body = iterator->body->copy();
  iterator->defPoint->insertBefore(new DefExpr(iteratorFn));
  Vec<BaseAST*> asts;
  collect_asts(iteratorFn, asts);
  ArgSymbol* icArg = new ArgSymbol(INTENT_BLANK, "_ic", ic->type);
  iteratorFn->insertFormalAtTail(icArg);
  replaceIteratorFormalsWithIteratorFields(iterator, icArg, asts);
  ArgSymbol* loopBodyFnIDArg = new ArgSymbol(INTENT_BLANK, "_loopBodyFnID", dtInt[INT_SIZE_DEFAULT]);
  iteratorFn->insertFormalAtTail(loopBodyFnIDArg);
  ArgSymbol* loopBodyFnArgArgs = new ArgSymbol(INTENT_BLANK, "_loopBodyFnArgs", argsBundleType);
  iteratorFn->insertFormalAtTail(loopBodyFnArgArgs);

  localizeReturnSymbols(iteratorFn, asts);

  convertYieldsAndReturns(asts, index, loopBodyFnIDArg, loopBodyFnArgArgs);

  iteratorFn->retType = dtVoid;
  iteratorFn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  iteratorFn->removeFlag(FLAG_INLINE_ITERATOR);
  iteratorFn->removeFlag(FLAG_ITERATOR_FN);

  return iteratorFn;
}


static void
expandRecursiveIteratorInline(CallExpr* call)
{
  SET_LINENO(call);

  //
  // create a nested function for the loop body (call->parentExpr),
  // and then transform the iterator into a function that takes this
  // nested function as an argument
  //
  FnSymbol* loopBodyFn = new FnSymbol(astr("_rec_iter_loop_", call->parentSymbol->name));
  call->parentExpr->insertBefore(new DefExpr(loopBodyFn));

  // The index is passed to the loop body function as its first argument.
  Symbol* index = toSymExpr(call->get(1))->var;
  ArgSymbol* indexArg = new ArgSymbol(INTENT_BLANK, "_index", index->type);
  loopBodyFn->insertFormalAtTail(indexArg);

  // The recursive iterator loop wrapper is ... .
  FnSymbol* loopBodyFnWrapper = new FnSymbol(astr("_rec_iter_loop_wrapper_", call->parentSymbol->name));
  call->parentSymbol->defPoint->insertBefore(new DefExpr(loopBodyFnWrapper));
  ftableVec.add(loopBodyFnWrapper);
  ftableMap.put(loopBodyFnWrapper, ftableVec.n-1);

  //
  // insert a call to the iterator function (using iterator as a
  // placeholder); build a call to loopBodyFnCall which will be
  // removed later and its arguments passed to the iteratorFn (we
  // build this to capture the actual arguments that should be
  // passed to it when this function is flattened)
  //
  Symbol* ic = toSymExpr(call->get(2))->var;
  FnSymbol* iterator = ic->type->initializer->getFormal(1)->type->initializer;
  CallExpr* iteratorFnCall = new CallExpr(iterator, ic, new_IntSymbol(ftableMap.get(loopBodyFnWrapper)));
  // replace function in iteratorFnCall with iterator function once that is created

  CallExpr* loopBodyFnCall = new CallExpr(loopBodyFn, gVoid);
  // use and remove loopBodyFnCall later
  call->parentExpr->insertBefore(loopBodyFnCall);
  call->parentExpr->insertBefore(iteratorFnCall);
  // Move the expression containing the call to the end of the (new) loop body function.
  loopBodyFn->insertAtTail(call->parentExpr->remove());
  call->remove();    // The call was replaced by a call to the (new) loop body function.

  // Now populate the loop body function.
  // Load the index arg.
  loopBodyFn->insertAtHead(new CallExpr(PRIM_MOVE, index, indexArg));
  loopBodyFn->insertAtHead(index->defPoint->remove());
  // Return nothing (void).
  loopBodyFn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  loopBodyFn->retType = dtVoid;

  Vec<FnSymbol*> nestedFunctions;
  nestedFunctions.add(loopBodyFn);
  flattenNestedFunctions(nestedFunctions);

  FnSymbol* iteratorFn = iteratorFnMap.get(iterator);
  if (!iteratorFn) {
    iteratorFn = createIteratorFn(iterator, iteratorFnCall, index,
                                  loopBodyFnCall, loopBodyFnWrapper, ic);
    iteratorFnMap.put(iterator, iteratorFn);
  } else {
    iteratorFnCall->baseExpr->replace(new SymExpr(iteratorFn));
    bundleLoopBodyFnArgsForIteratorFnCall(iteratorFnCall, loopBodyFnCall, loopBodyFnWrapper);
    Symbol* argBundleTmp = newTemp("argBundleTmp", iteratorFn->getFormal(3)->type);
    iteratorFnCall->insertBefore(new DefExpr(argBundleTmp));
    iteratorFnCall->insertBefore(new CallExpr(PRIM_MOVE, argBundleTmp, new CallExpr(PRIM_CAST, argBundleTmp->type->symbol, iteratorFnCall->get(3)->remove())));
    iteratorFnCall->insertAtTail(argBundleTmp);
  }
}


static void
expandIteratorInline(CallExpr* call) {
  Symbol* ic = toSymExpr(call->get(2))->var;
  FnSymbol* iterator = ic->type->initializer->getFormal(1)->type->initializer;

  if (iterator->hasFlag(FLAG_RECURSIVE_ITERATOR)) {

    //
    // loops over recursive iterators in recursive iterators only need
    // to be handled in the recursive iterator function
    //
    if (call->parentSymbol->hasFlag(FLAG_RECURSIVE_ITERATOR))
      return;

    expandRecursiveIteratorInline(call);
    return;
  }

  SET_LINENO(call);

  Symbol* index = toSymExpr(call->get(1))->var;
  BlockStmt* ibody = iterator->body->copy();
  if (!preserveInlinedLineNumbers)
    reset_ast_loc(ibody, call);

  // Body is the expression containing the iterator call.
  BlockStmt* body = toBlockStmt(call->parentExpr);

  // The iterator call is removed
  call->remove();
  // and the expression enclosing the call is replaced by the iterator body.
  body->replace(ibody);

  // Now replace yield statements in the inlined body with copies of the
  // expression that called the iterator, substituting the yielded index for the
  // iterator formal.
  Vec<BaseAST*> asts;
  collect_asts(ibody, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIM_YIELD)) {
        SymbolMap map;

        Symbol* yieldedIndex = newTemp("_yieldedIndex", index->type);
        map.put(index, yieldedIndex);
        BlockStmt* bodyCopy = body->copy(&map);

        // ??? Is this really necessary?  Why?
        if (int count = countEnclosingLocalBlocks(call, ibody)) {
          for (int i = 0; i < count; i++) {
            bodyCopy = new BlockStmt(bodyCopy);
            bodyCopy->blockInfo = new CallExpr(PRIM_BLOCK_UNLOCAL);
          }
        }

        Symbol* yieldedSymbol = toSymExpr(call->get(1))->var;

        // remove move to return-temp that is defined at top of iterator
        bool inserted = false;
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
        if (!inserted) {
          bodyCopy->insertBefore(new DefExpr(yieldedIndex));
          bodyCopy->insertBefore(new CallExpr(PRIM_MOVE, yieldedIndex, call->get(1)));
        }
      }
      if (call->isPrimitive(PRIM_RETURN)) // remove return
        call->remove();
    }
  }
  replaceIteratorFormalsWithIteratorFields(iterator, ic, asts);
}


// Returns true if the iterator can be inlined; false otherwise.
//
// It can be inlined if it contains exactly one yield statement.
static bool
canInlineIterator(FnSymbol* iterator) {
  Vec<BaseAST*> asts;
  collect_asts(iterator, asts);
  int count = 0;
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast))
      if (call->isPrimitive(PRIM_YIELD))
        count++;
  }
  if (count == 1)
    return true;
  return false;
}


static void
getRecursiveIterators(Vec<Symbol*>& iterators, Symbol* gIterator) {
  if (gIterator->type->symbol->hasFlag(FLAG_TUPLE)) {
    ClassType* iteratorType = toClassType(gIterator->type);
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
    FnSymbol* iterator= iterators.v[i]->type->initializer->getFormal(1)->type->initializer;
    BlockStmt *block = iterator->body;

    INT_ASSERT(block);
    Vec<BaseAST*> asts;
    collect_asts(block, asts);

    int numYields = 0;
    forv_Vec(BaseAST, ast, asts) {
      CallExpr* call = toCallExpr(ast);
      if (call && call->isPrimitive(PRIM_YIELD)) {
        numYields++;
        if (iterator->body != call->parentExpr) return false;
      }
    }
    if (numYields != 1) return false;
  }
  return true;
}


static void
setupSimultaneousIterators(Vec<Symbol*>& iterators,
                           Vec<Symbol*>& indices,
                           Symbol* gIterator,
                           Symbol* gIndex,
                           BlockStmt* loop) {
  if (gIterator->type->symbol->hasFlag(FLAG_TUPLE)) {
    ClassType* iteratorType = toClassType(gIterator->type);
    ClassType* indexType = toClassType(gIndex->type);
    for (int i=1; i <= iteratorType->fields.length; i++) {
      Symbol* iterator = newTemp("_iterator", iteratorType->getField(i)->type);
      loop->insertBefore(new DefExpr(iterator));
      loop->insertBefore(new CallExpr(PRIM_MOVE, iterator, new CallExpr(PRIM_GET_MEMBER_VALUE, gIterator, iteratorType->getField(i))));

      Symbol* index = newTemp("_index", indexType->getField(i)->type);
      loop->insertAtHead(new CallExpr(PRIM_SET_MEMBER, gIndex, indexType->getField(i), index));
      loop->insertAtHead(new DefExpr(index));
      setupSimultaneousIterators(iterators, indices, iterator, index, loop);
    }
  } else {
    iterators.add(gIterator);
    indices.add(gIndex);
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


static void
buildIteratorCallInner(BlockStmt* block, Symbol* ret, int fnid, Symbol* iterator) {
  IteratorInfo* ii = iterator->type->initializer->getFormal(1)->type->initializer->iteratorInfo;
  FnSymbol* fn = NULL;
  switch (fnid) {
  case ZIP1: fn = ii->zip1; break;
  case ZIP2: fn = ii->zip2; break;
  case ZIP3: fn = ii->zip3; break;
  case ZIP4: fn = ii->zip4; break;
  case HASMORE: fn = ii->hasMore; break;
  case GETVALUE: fn = ii->getValue; break;
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
inlineSingleYieldIterator(CallExpr* call) {
  BlockStmt* block = toBlockStmt(call->parentExpr);
  SymExpr* se1 = toSymExpr(call->get(1));
  SymExpr* se2 = toSymExpr(call->get(2));
  VarSymbol* index = toVarSymbol(se1->var);
  VarSymbol* iterator = toVarSymbol(se2->var);

  SET_LINENO(call);
  Vec<Symbol*> iterators;
  Vec<Symbol*> indices;
  setupSimultaneousIterators(iterators, indices, iterator, index, block);

  CallExpr *noop = new CallExpr(PRIM_NOOP);
  block->insertAtHead(noop);
  for (int i = 0; i < iterators.n; i++) {
    FnSymbol *iterator = iterators.v[i]->type->initializer->getFormal(1)->type->initializer;
    Vec<Expr*> exprs;

    BlockStmt *ibody = iterator->body->copy();

    Vec<BaseAST*> asts;
    collect_asts(ibody, asts);
    bool afterYield = false;
    for_alist(expr, ibody->body) {
      if (CallExpr *curr_expr = toCallExpr(expr)) {
        if (curr_expr->isPrimitive(PRIM_YIELD)) {
          afterYield = true;
          noop->insertAfter(new CallExpr(PRIM_MOVE, indices.v[i], curr_expr->get(1)->remove()));
        } else if (!curr_expr->isPrimitive(PRIM_RETURN)) {
          if (!afterYield)
            noop->insertBefore(curr_expr->remove());
          else 
            block->insertAtTail(curr_expr->remove());
        }
      } else {
        if (!afterYield)
          noop->insertBefore(expr->remove());
        else 
          block->insertAtTail(expr->remove());
      }
    }

    int count = 1;
    for_formals(formal, iterator) {
      forv_Vec(BaseAST, ast, asts) {
        if (SymExpr* se = toSymExpr(ast)) {
          if (se->var == formal) {
            //if ((se->var->type == formal->type) && (!strcmp(se->var->name, formal->name))) {
            // count is used to get the nth field out of the iterator class;
            // it is replaced by the field once the iterator class is created
            Expr* stmt = se->getStmtExpr();
            VarSymbol* tmp = newTemp(formal->name, formal->type);
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
  call->remove();
}


static void
expand_for_loop(CallExpr* call) {
  BlockStmt* block = toBlockStmt(call->parentExpr);
  if (!block || block->blockInfo != call)
    INT_FATAL(call, "bad for loop primitive");
  SymExpr* se1 = toSymExpr(call->get(1));
  SymExpr* se2 = toSymExpr(call->get(2));
  if (!se1 || !se2)
    INT_FATAL(call, "bad for loop primitive");
  VarSymbol* index = toVarSymbol(se1->var);
  VarSymbol* iterator = toVarSymbol(se2->var);
  if (!index || !iterator)
    INT_FATAL(call, "bad for loop primitive");

  if (!fNoInlineIterators &&
      iterator->type->initializer->getFormal(1)->type->initializer->iteratorInfo &&
      canInlineIterator(iterator->type->initializer->getFormal(1)->type->initializer) &&
      (iterator->type->dispatchChildren.n == 0 ||
       (iterator->type->dispatchChildren.n == 1 &&
        iterator->type->dispatchChildren.v[0] == dtObject))) {
    expandIteratorInline(call);
  } else if (!fNoInlineIterators && canInlineSingleYieldIterator(iterator)) {
    inlineSingleYieldIterator(call);
  } else {
    SET_LINENO(call);
    Vec<Symbol*> iterators;
    Vec<Symbol*> indices;
    setupSimultaneousIterators(iterators, indices, iterator, index, block);

    VarSymbol* firstCond = NULL;
    for (int i = 0; i < iterators.n; i++) {
      Vec<Type*> children;
      getIteratorChildren(children, iterators.v[i]->type);
      VarSymbol* cond = newTemp("_cond", dtBool);

      block->insertBefore(buildIteratorCall(NULL, ZIP1, iterators.v[i], children));

      block->insertBefore(new DefExpr(cond));
      block->insertBefore(buildIteratorCall(cond, HASMORE, iterators.v[i], children));
      block->insertAtHead(buildIteratorCall(indices.v[i], GETVALUE, iterators.v[i], children));

      block->insertAtTail(buildIteratorCall(NULL, ZIP3, iterators.v[i], children));
      block->insertAtTail(buildIteratorCall(cond, HASMORE, iterators.v[i], children));

      // hilde sez: This seems like a kludge.
      // Why is the pragma applied to the initializer rather than the type itself?
      if (isBoundedIterator(iterators.v[i]->type->initializer->getFormal(1)->type->initializer)) {
        if (!firstCond) {
          firstCond = cond;
        } else if (!fNoBoundsChecks) {
          VarSymbol* hasMore = newTemp("hasMore", dtBool);
          block->insertAtHead(new CondStmt(new SymExpr(hasMore), new CallExpr(PRIM_RT_ERROR, new_StringSymbol("zippered iterations have non-equal lengths"))));
          block->insertAtHead(new CallExpr(PRIM_MOVE, hasMore, new CallExpr(PRIM_EQUAL, cond, new_IntSymbol(0))));
          block->insertAtHead(new DefExpr(hasMore));
          block->insertAfter(new CondStmt(new SymExpr(cond), new CallExpr(PRIM_RT_ERROR, new_StringSymbol("zippered iterations have non-equal lengths"))));
        }
      }
      block->insertAtHead(buildIteratorCall(NULL, ZIP2, iterators.v[i], children));
      block->insertAfter(buildIteratorCall(NULL, ZIP4, iterators.v[i], children));
    }
    call->get(2)->remove();
    call->get(1)->remove();
    if (firstCond)
      call->insertAtTail(firstCond);
    else
      call->insertAtTail(gTrue);

    block->insertAtHead(index->defPoint->remove());
  }
}


static void
inlineIterators() {
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (block->parentSymbol) {
      if (block->blockInfo && block->blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP)) {
        Symbol* iterator = toSymExpr(block->blockInfo->get(2))->var;
        FnSymbol* ifn = iterator->type->initializer->getFormal(1)->type->initializer;
        if (ifn->hasFlag(FLAG_INLINE_ITERATOR)) {
          expandIteratorInline(block->blockInfo);
        }
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
      ClassType* ct = toClassType(call->get(1)->getValType());
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
              !strcmp(call->parentSymbol->name, "_toFastFollower")) {
            ArgSymbol* iterator = toFnSymbol(call->parentSymbol)->getFormal(1);
            Type* iteratorType = iterator->getValType();
            int i = 2; // first field is super
            for_actuals(actual, call) {
              SymExpr* se = toSymExpr(actual);
              if (isArgSymbol(se->var) && call->parentSymbol != se->var->defPoint->parentSymbol) {
                Symbol* field = toClassType(iteratorType)->getField(i);
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
      ClassType* irecord = fn->iteratorInfo->irecord;
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
          VarSymbol* classTmp = newTemp("classTmp", type->initializer->iteratorInfo->getIterator->retType);
          thenStmt->insertAtTail(new DefExpr(recordTmp));
          thenStmt->insertAtTail(new DefExpr(classTmp));

          ClassType* ct = toClassType(type);
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
          thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, classTmp, new CallExpr(type->initializer->iteratorInfo->getIterator, recordTmp)));
          thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, ret, new CallExpr(PRIM_CAST, ret->type->symbol, classTmp)));
          thenStmt->insertAtTail(new GotoStmt(GOTO_GETITER_END, label));
          ret->defPoint->insertAfter(new CondStmt(new SymExpr(cid), thenStmt));
          ret->defPoint->insertAfter(new CallExpr(PRIM_MOVE, cid, new CallExpr(PRIM_TESTCID, tmp, type->initializer->iteratorInfo->irecord->getField(1)->type->symbol)));
          ret->defPoint->insertAfter(new DefExpr(cid));
          ret->defPoint->insertAfter(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, getIterator->getFormal(1), irecord->getField(1))));
          ret->defPoint->insertAfter(new DefExpr(tmp));
        }
      }
    }
  }
}


static void reconstructIRautoCopyAutoDestroy()
{
  //
  // reconstruct autoCopy and autoDestroy for iterator records
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->numFormals() == 1 && fn->getFormal(1)->type->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
      SET_LINENO(fn);
      if (!strcmp(fn->name, "chpl__autoCopy")) {
        Symbol* arg = fn->getFormal(1);
        Symbol* ret = fn->getReturnSymbol();
        BlockStmt* block = new BlockStmt();
        block->insertAtTail(ret->defPoint->remove());
        ClassType* irt = toClassType(arg->type);
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
      if (!strcmp(fn->name, "chpl__autoDestroy")) {
        SET_LINENO(fn);
        Symbol* arg = fn->getFormal(1);
        BlockStmt* block = new BlockStmt();
        ClassType* irt = toClassType(arg->type);
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
    }
  }
}


void lowerIterators() {
  nonLeaderParCheck();

  computeRecursiveIteratorSet();

  inlineIterators();
  
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ITERATOR_FN)) {
      collapseBlocks(fn->body);
      removeUnnecessaryGotos(fn);
    }
  }

  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol)
      if (call->isPrimitive(PRIM_BLOCK_FOR_LOOP))
        if (call->numActuals() > 1)
          expand_for_loop(call);
  }

  fragmentLocalBlocks();

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ITERATOR_FN)) {
      collapseBlocks(fn->body);
      removeUnnecessaryGotos(fn);
      if (!fNoCopyPropagation)
        localCopyPropagation(fn);
      if (!fNoDeadCodeElimination) {
        deadCodeElimination(fn);
      }
    }
  }
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ITERATOR_FN)) {
      lowerIterator(fn);
    }
  }

  fixNumericalGetMemberPrims();

  cleanupLeaderFollowerIteratorCalls();

  handlePolymorphicIterators();

  reconstructIRautoCopyAutoDestroy();
}


