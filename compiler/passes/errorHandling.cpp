/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#include "errorHandling.h"

#include "AstVisitorTraverse.h"
#include "CatchStmt.h"
#include "DecoratedClassType.h"
#include "DeferStmt.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "InitErrorHandling.h"
#include "driver.h"
#include "resolution.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "TryStmt.h"
#include "wellknown.h"

#include "global-ast-vecs.h"

#include <stack>

/*
This is a pseudo-code example of what this pass is supposed to do with a
throwing function, transforming error handling constructs.

If using try in functions that do not propagate, a catch-all is required
(catch or catch err). This can be avoided by using try!

Note that errors are not yet automatically deallocated when the error
is consumed by a catch.

// given code
proc propagate() throws {
  try {
    a(); // throws
    b(); // does not throw
    c(); // throws
  } catch e: SubError {
    f();
  } catch e: AnotherSubError {
    g();
  }
}

// after this pass
proc propagate(out error_out: Error) {
  var error: Error;
  a(error);
  if error then
    goto handler;
  b();
  c(error);
  if error then
    goto handler;

  label handler:
  if error {
    var e = error: SubError;
    if _cast {
      f();
      delete e;
    } else {
      var e = error: AnotherSubError;
      if e {
        g();
        delete e;
      } else {
        // set and return
        error_out = error;
        goto epilogue_label;
      }
    }
  }
}

nested try:
try {
  try {
    throwingCall();
  } catch e: SpecificError {
    handleGracefully();
  }
  otherThrowingCall();
} catch {
  handleSomehow();
}

{
  var _e1: Error;
  {
    var _e2: Error;
    throwingCall(_e2);
    if _e2 then
      goto handler2;

    label handler2:
    if _e2 {
      var _cast = _e2: SpecificError;
      if _cast {
        handleGracefully();
        delete _cast;
      } else {
        _e1 = _e2;
        goto handler1;
      }
    }
  }
  otherThrowingCall(_e1);
  if _e1 then
    goto handler1;

  label handler1:
  if _e1 {
    handleSomehow();
    delete _e1;
  }
}
*/


// This is the type for a map that is used when printing out the reason
// that a function is marked throws. Knowing the reason a function
// is marked throws implicitly is relevant for example with a coforall
// task function. In that case, the location of coforall itself
// isn't as relevant to the user as the location in the body
// of the coforall of a call to a throwing function.
//
// Here the key is a FnSymbol and the value is the reason it was
// marked throwing (typically a CallExpr).
typedef std::map<FnSymbol*, BaseAST*> implicitThrowsReasons_t;

// Static functions
static void markImplicitThrows(FnSymbol* fn, std::set<FnSymbol*>* visited, implicitThrowsReasons_t* reasons);
static bool canBlockStmtThrow(BlockStmt* block);
static void checkErrorHandling(FnSymbol* fn, implicitThrowsReasons_t * reasons);
static bool isCompilerGeneratedFunction(FnSymbol* fn);
static bool isUncheckedThrowsFunction(FnSymbol* fn);

static Type* dtErrorNilable() {
  return getDecoratedClass(dtError, ClassTypeDecorator::UNMANAGED_NILABLE);
}

namespace {


// Static class helper functions
static bool catchesNotExhaustive(TryStmt* tryStmt);
static bool shouldEnforceStrict(CallExpr* node, int taskFunctionDepth);
static AList castToErrorNilable(Symbol* error, SymExpr* &castedError);

class ErrorHandlingVisitor final : public AstVisitorTraverse {

public:
  InitErrorHandling*  state;

  ErrorHandlingVisitor       (ArgSymbol* _outFormal, LabelSymbol* _epilogue,
                              InitErrorHandling* _state);

  bool enterTryStmt  (TryStmt*   node) override;
  void exitTryStmt   (TryStmt*   node) override;
  void exitCatchStmt (CatchStmt* node) override;
  bool enterCallExpr (CallExpr*  node) override;
  bool enterForLoop  (ForLoop*   node) override;
  void exitForLoop   (ForLoop*   node) override;
  bool enterForallStmt(ForallStmt* node) override;
  void exitForallStmt (ForallStmt* node) override;
  bool enterDeferStmt(DeferStmt* node) override;
  void exitDeferStmt (DeferStmt* node) override;

  // Specifically for ensuring we track initializer state appropriately
  bool enterCondStmt(CondStmt* node) override;

private:
  struct TryInfo {
    VarSymbol*   errorVar;
    LabelSymbol* handlerLabel;
    TryStmt*     tryStmt;
    Expr*        throwingForall;
    BlockStmt*   tryBody;
  };

  std::stack<TryInfo> tryStack;
  std::stack<TryInfo> catchesStack;
  int                 deferDepth;
  ArgSymbol*          outError;
  LabelSymbol*        epilogue;

  void   lowerCatches      (const TryInfo& info);
  AList  setOutGotoEpilogue(VarSymbol*     error);
  GotoStmt*  gotoHandler();
  AList  setOuterErrorAndGotoHandler(VarSymbol* error);
  AList  errorCond         (VarSymbol*     errorVar,
                            BlockStmt*     thenBlock,
                            BlockStmt*     elseBlock = NULL);
  CallExpr* haltExpr       (VarSymbol*     error, bool tryBang);
  void setupForThrowingLoop(Stmt* node,
                            LabelSymbol* handlerLabel,
                            BlockStmt* body);
  void exitForallLoop(Stmt* node);

  void checkThrowingFuncInInit(CallExpr* node, bool insideTryStack);

  ErrorHandlingVisitor();
};

ErrorHandlingVisitor::ErrorHandlingVisitor(ArgSymbol*   _outError,
                                           LabelSymbol* _epilogue,
                                           InitErrorHandling* _state) {
  deferDepth = 0;
  outError   = _outError;
  epilogue   = _epilogue;
  state = _state;
}

bool ErrorHandlingVisitor::enterTryStmt(TryStmt* node) {
  SET_LINENO(node);

  VarSymbol*   errorVar     = newTemp("error", dtErrorNilable());
  errorVar->addFlag(FLAG_ERROR_VARIABLE);
  LabelSymbol* handlerLabel = new LabelSymbol("handler");
  handlerLabel->addFlag(FLAG_ERROR_LABEL);
  TryInfo info = {errorVar, handlerLabel, node, NULL, node->body()};
  tryStack.push(info);

  return true;
}

void ErrorHandlingVisitor::exitTryStmt(TryStmt* node) {
  SET_LINENO(node);

  TryInfo info = tryStack.top();
  tryStack.pop();

  BlockStmt* tryBody = info.tryBody;

  tryBody->insertAtHead(new CallExpr(PRIM_MOVE, info.errorVar, gNil));
  tryBody->insertAtHead(new DefExpr(info.errorVar));
  tryBody->insertAtTail(new DefExpr(info.handlerLabel));

  if (node->_catches.empty()) {
    lowerCatches(info); // no exitCatchStmt, so called here
  } else {
    catchesStack.push(info);
  }
}

void ErrorHandlingVisitor::exitCatchStmt(CatchStmt* node) {
  // last CatchStmt to have its contents lowered; lower catches structure
  if (node->next == NULL) {
    TryInfo info = catchesStack.top();
    catchesStack.pop();
    lowerCatches(info);
  }
}

void ErrorHandlingVisitor::lowerCatches(const TryInfo& info) {
  TryStmt*   tryStmt  = info.tryStmt;
  VarSymbol* errorVar = info.errorVar;

  SET_LINENO(tryStmt);

  BlockStmt* handlers    = new BlockStmt();
  BlockStmt* currHandler = handlers;
  bool       hasCatchAll = false;

  for_alist(c, tryStmt->_catches) {
    if (hasCatchAll)
      INT_FATAL(c->prev, "catchall placed before the end of a catch list");

    SET_LINENO(c);

    CatchStmt* catchStmt = toCatchStmt(c);

    BlockStmt* catchBody = catchStmt->body();

    // PRIM_CURRENT_ERROR should be already replaced by info.errorVar

    // named or unnamed catchall
    if (catchStmt->isCatchall()) {
      hasCatchAll = true;
      currHandler->insertAtTail(catchBody->remove());
    } else {
      // Find the else body in the last CondStmt in the catch block body.
      // This should always exist after CatchStmt::cleanup
      // for non-catchall errors.

      CondStmt* finalCond = NULL;
      for_alist_backward(node, catchBody->body) {
        if (CondStmt* cond = toCondStmt(node)) {
          finalCond = cond;
          break;
        }
      }
      INT_ASSERT(finalCond != NULL && finalCond->elseStmt != NULL);

      BlockStmt* nextHandler = finalCond->elseStmt;

      // Clear everything in the elseStmt
      // (sometimes has PRIM_RT_ERROR to satisfy isDefinedAllPaths)
      for_alist(stmt, nextHandler->body)
        stmt->remove();

      // Remove the catch body and place it in the currHandler block
      currHandler->insertAtTail(catchBody->remove());

      // Set currHandler to the else block
      currHandler = nextHandler;
    }
  }

  if (!hasCatchAll) {
    if (tryStmt->tryBang()) {
      currHandler->insertAtTail(haltExpr(errorVar, true));
    } else if (!tryStack.empty()) {
      currHandler->insertAtTail(setOuterErrorAndGotoHandler(errorVar));
    } else if (outError != NULL) {
      currHandler->insertAtTail(setOutGotoEpilogue(errorVar));
    } else {
      INT_FATAL(tryStmt, "try without a catchall in a non-throwing function");
    }
  }

  BlockStmt* tryBody = info.tryBody;
  tryBody->insertAtTail(errorCond(errorVar, handlers));

  // after lowering its catches, replace the TryStmt with its body
  tryBody->remove();
  info.tryStmt->replace(tryBody);
}

static VarSymbol* createAndInsertErrorVar(Expr* insert) {
  VarSymbol* errorVar = newTemp("error", dtErrorNilable());
  errorVar->addFlag(FLAG_ERROR_VARIABLE);
  insert->insertBefore(new DefExpr(errorVar));
  insert->insertBefore(new CallExpr(PRIM_MOVE, errorVar, gNil));
  return errorVar;
}

bool ErrorHandlingVisitor::enterCallExpr(CallExpr* node) {
  bool insideTry = !tryStack.empty();

  // The common case of a user-level call to a resolved function
  FnSymbol* calledFn = node->resolvedOrVirtualFunction();

  if (calledFn != NULL) {
    if (calledFn->throwsError()) {
      checkThrowingFuncInInit(node, insideTry);

      SET_LINENO(node);

      VarSymbol* errorVar    = NULL;
      BlockStmt* errorPolicy = new BlockStmt();
      Expr*      insert      = node->getStmtExpr();

      if (insideTry && node->tryTag != TRY_TAG_IN_TRYBANG) {
        TryInfo info = tryStack.top();
        errorVar = info.errorVar;

        // (a) an enclosing try/try!
        errorPolicy->insertAtTail(gotoHandler());
      } else {
        // without try, need an error variable
        errorVar = createAndInsertErrorVar(insert);

        // TODO: if deferDepth > 0, either implement error handling
        // or make the program halt regardless of insideTry or outError.
        if (outError != NULL && node->tryTag != TRY_TAG_IN_TRYBANG &&
            deferDepth == 0 && !node->parentSymbol->hasFlag(FLAG_OUTSIDE_TRY)) {
          // (b) throw from the enclosing function
          errorPolicy->insertAtTail(setOutGotoEpilogue(errorVar));
        }
        else if (calledFn->hasFlag(FLAG_TASK_JOIN_IMPL_FN)) {
          if (node->parentSymbol->hasFlag(FLAG_ITERATOR_FN))
            // (c) coforall or similar in a non-throwing iterator
            // ==> we will propagate the error when the iterator is inlined
            errorPolicy->insertAtTail(haltExpr(errorVar, false));
          else if (node->parentSymbol->hasFlag(FLAG_TASK_FN_FROM_ITERATOR_FN))
            // (d) coforall/... in a task function in a non-throwing iterator
            // ==> propagate the error through the task function
            errorPolicy->insertAtTail(setOutGotoEpilogue(errorVar));
          else
            // (e) coforall or similar in a non-throwing procedure
            // ==> halt right away
            errorPolicy->insertAtTail(haltExpr(errorVar, true));
        }
        else {
          // (f) a throwing call in a non-throwing function ==> halt right away
          errorPolicy->insertAtTail(haltExpr(errorVar, true));
        }
      }

      node->insertAtTail(errorVar); // adding error argument to call

      // If we are calling a non-blocking task function,
      // we'll lower the error handling in parallel.cpp,
      // at the end of parallel(),  using lowerCheckErrorPrimitive.
      if (calledFn->hasFlag(FLAG_NON_BLOCKING) ||
          calledFn->hasFlag(FLAG_BEGIN) ||
          calledFn->hasFlag(FLAG_COBEGIN_OR_COFORALL)) {
        // Don't add errorPolicy block or conditional.
      } else {
        // Regular operation
        insert->insertAfter(new CondStmt(new CallExpr(PRIM_CHECK_ERROR, errorVar), errorPolicy));
      }
    }
  } else if (node->isPrimitive(PRIM_THROW)) {
    SET_LINENO(node);

    BlockStmt* throwBlock = new BlockStmt();
    node->replace(throwBlock);

    SymExpr*   thrownExpr  = toSymExpr(node->get(1)->remove());
    VarSymbol* thrownError = toVarSymbol(thrownExpr->symbol());

    Type* thrownType = thrownError->typeInfo();
    if (DecoratedClassType* dt = toDecoratedClassType(thrownType))
      thrownType = dt->getCanonicalClass();

    // normalizeThrows should give us this invariant earlier
    INT_ASSERT(thrownType == dtError);

    VarSymbol* fixedError = thrownError;

    if (insideTry) {
      throwBlock->insertAtTail(setOuterErrorAndGotoHandler(fixedError));
    } else if (outError != NULL) {
      throwBlock->insertAtTail(setOutGotoEpilogue(fixedError));
    } else {
      INT_FATAL(node, "cannot throw in a non-throwing function");
    }
  } else if (node->isPrimitive(PRIM_CURRENT_ERROR)) {
    INT_ASSERT(!catchesStack.empty());
    TryInfo info = catchesStack.top();

    SET_LINENO(node);
    node->replace(new SymExpr(info.errorVar));
  }

  // If appropriate, advance the phase after we've done our other checks.
  // This will allow us to check if the `this.init` or `super.init` call would
  // throw (which shouldn't be allowed yet)
  if (state != NULL) {
    if (isInitStmt(node) == true) {
      if (isResolvedThisInit(node) == true) {
        state->completePhase1(node);
      } else if (isResolvedSuperInit(node) == true) {
        state->completePhase0(node);
      }
    } else if (state->isInitDone(node) == true) {
      state->completePhase1(node);
    }
  }

  return true;
}

// setupForThrowingLoop() and exitForallLoop() factor out the handling
// of a ForLoop and a ForallStmt.
// When a ForLoop can no longer represent a forall loop, we can undo the
// refactoring by inlining them into enterForallStmt()/exitForallStmt().

void ErrorHandlingVisitor::setupForThrowingLoop(Stmt* node,
                                                LabelSymbol* handlerLabel,
                                                BlockStmt* body)
{
  VarSymbol* errorVar = createAndInsertErrorVar(node);
  TryInfo info = {errorVar, handlerLabel, NULL, node, body};
  tryStack.push(info);
}

bool ErrorHandlingVisitor::enterForLoop(ForLoop* node) {
  if (!node->isLoweredForallLoop())
    return true;
  if (!canBlockStmtThrow(node))
    return true;

  SET_LINENO(node);

   // Make sure that there's a break label.
  if (node->breakLabelGet() == NULL) {
    LabelSymbol* b = new LabelSymbol("forall_break_label");
    // intentionally *not* marked with FLAG_ERROR_LABEL so that
    // we don't auto-destroy everything just before it.
    b->addFlag(FLAG_FORALL_BREAK_LABEL);
    node->breakLabelSet(b);
    node->insertAfter(new DefExpr(b));
  }

  setupForThrowingLoop(node, node->breakLabelGet(), node);

  return true;
}

static void checkTPVs(ForallStmt* fs) {
  // Do this first to check for throwing non-POD initializer exprs.
  for_shadow_vars(svar, temp, fs) {
    if (BlockStmt* IB = svar->initBlock()) {
      if (canBlockStmtThrow(IB)) {
        if (! isPOD(svar->type))
          // The AST currently created by the compiler executes all TPVs'
          // deinitializers when any of their initializing exprs throws.
          // This will deinitialize at least one not-yet-initialized TPV. Bad.
          USR_FATAL_CONT(IB, "the initialization expression of the task-private"
            " variable '%s' throws - this is currently not supported"
            " for variables of non-POD types", svar->name);
      }
    }
    if (BlockStmt* DB = svar->deinitBlock()) {
      if (canBlockStmtThrow(DB))
        // Error handling for the deinit blocks may be unimplemented.
        USR_FATAL_CONT(DB, "the deinitializer of the task-private variable '%s'"
                       " throws - this is currently not supported", svar->name);
    }
  }
}

bool ErrorHandlingVisitor::enterForallStmt(ForallStmt* node) {
  checkTPVs(node);

  // If 'node' is outside an error handling context:, the body will need to
  // halt on an error, so treat this as non-throwing.
  if (tryStack.empty() &&
      (outError == NULL || node->parentSymbol->hasFlag(FLAG_OUTSIDE_TRY)))
    return true;

  // We assume that fRecIterGetIterator/fRecIterFreeIterator do not throw.
  if (!canBlockStmtThrow(node->loopBody()))
    return true;

  SET_LINENO(node);

   // Make sure that there's a break label.
  if (node->fErrorHandlerLabel == NULL) {
    LabelSymbol* b = new LabelSymbol("forall_break_label");
    // intentionally *not* marked with FLAG_ERROR_LABEL so that
    // we don't auto-destroy everything just before it.
    b->addFlag(FLAG_FORALL_BREAK_LABEL);
    node->fErrorHandlerLabel = b;
    node->insertAfter(new DefExpr(b));
  }

  setupForThrowingLoop(node, node->fErrorHandlerLabel, node->loopBody());

  return true;
}

void ErrorHandlingVisitor::exitForallLoop(Stmt* node)
{
  if (tryStack.empty())
    return;

  TryInfo& info = tryStack.top();
  if (info.throwingForall == NULL)
    return;
  else
    INT_ASSERT(info.throwingForall == node);

  tryStack.pop();

  SET_LINENO(node);

  BlockStmt* handler = new BlockStmt();
  // Always wrap errors from foralls in a TaskErrors
  VarSymbol* err = info.errorVar;
  VarSymbol* normErr = newTemp("forall_error", dtErrorNilable());
  handler->insertAtTail(new DefExpr(normErr));

  handler->insertAtTail(new CallExpr(PRIM_MOVE, normErr,
                                     new CallExpr(gChplForallError, err)));

  if (!tryStack.empty()) {
    handler->insertAtTail(setOuterErrorAndGotoHandler(normErr));
  } else if (outError != NULL) {
    handler->insertAtTail(setOutGotoEpilogue(normErr));
  } else {
    handler->insertAtTail(haltExpr(normErr, false));
  }
  info.handlerLabel->defPoint->insertAfter(errorCond(err, handler));
}

void ErrorHandlingVisitor::exitForLoop(ForLoop* node) {
  if (!node->isLoweredForallLoop())
    return;

  exitForallLoop(node);
}

void ErrorHandlingVisitor::exitForallStmt(ForallStmt* node) {
  exitForallLoop(node);
}

bool ErrorHandlingVisitor::enterDeferStmt(DeferStmt* node) {
  deferDepth++;

  return true;
}

void ErrorHandlingVisitor::exitDeferStmt(DeferStmt* node) {
  deferDepth--;
}

bool ErrorHandlingVisitor::enterCondStmt(CondStmt* node) {
  if (state != NULL) {
    InitErrorHandling* oldState = state;
    InitErrorHandling* thenState = new InitErrorHandling(node, *state);
    state = thenState;

    node->thenStmt->accept(this);

    if (node->elseStmt != NULL) {
      InitErrorHandling* elseState = new InitErrorHandling(node, *oldState);
      state = elseState;
      node->elseStmt->accept(this);

      // Handling during normalize should ensure that both branches result in
      // the same final state
      INT_ASSERT(thenState->currPhase() == elseState->currPhase());
      thenState->merge(*elseState);
      delete elseState;
    }
    state = oldState; // To ensure we don't strand the memory

    state->merge(*thenState);
    delete thenState;

    return false; // Already handled, no need to traverse again
  } else {
    return true; // Normal behavior
  }
}

void ErrorHandlingVisitor::checkThrowingFuncInInit(CallExpr* node,
                                                   bool insideTryStack) {
  if (state != NULL) {
    FnSymbol* fn = state->theFn();
    INT_ASSERT(fn);

    if (insideTryStack && node->tryTag != TRY_TAG_IN_TRYBANG) {
      TryInfo info = tryStack.top();
      if (info.tryStmt->tryBang()) {
        if (info.tryStmt->_catches.length != 0) {
          if (!state->isPhase2()) {
            USR_FATAL_CONT(node,
                           "cannot call a throwing function in a try! with catch clauses before phase 2");
          }
        }
      } else {
        if (!state->isPhase2() && fn->throwsError() == true) {
          USR_FATAL_CONT(node,
                         "cannot call a throwing function outside of a try! before phase 2");
        }
      }
    } else {
      if (!state->isPhase2() && node->tryTag != TRY_TAG_IN_TRYBANG &&
          fn->throwsError() == true) {
        USR_FATAL_CONT(node,
                       "cannot call a throwing function outside of a try! before phase 2");
      }
    }
  } // not in an initializer
}


// Sets the fn out variable with the given error, then goes to the fn epilogue.
AList ErrorHandlingVisitor::setOutGotoEpilogue(VarSymbol* error) {

  SymExpr* castedError = NULL;
  AList    ret         = castToErrorNilable(error, castedError);
  // Using PRIM_ASSIGN instead of PRIM_MOVE here to work around
  // errors that come up in C compilation.
  ret.insertAtTail(new CallExpr(PRIM_ASSIGN, outError, castedError));
  ret.insertAtTail(new GotoStmt(GOTO_ERROR_HANDLING_RETURN, epilogue));

  return ret;
}

GotoStmt* ErrorHandlingVisitor::gotoHandler() {

  INT_ASSERT(!tryStack.empty());
  TryInfo& outerTry = tryStack.top();
  if (outerTry.throwingForall != NULL)
    return new GotoStmt(GOTO_BREAK_ERROR_HANDLING, outerTry.handlerLabel);
  else
    return new GotoStmt(GOTO_ERROR_HANDLING, outerTry.handlerLabel);
}

AList ErrorHandlingVisitor::setOuterErrorAndGotoHandler(VarSymbol* error) {

  INT_ASSERT(!tryStack.empty());
  TryInfo& outerTry    = tryStack.top();
  SymExpr* castedError = NULL;
  AList    ret         = castToErrorNilable(error, castedError);
  ret.insertAtTail(new CallExpr(PRIM_MOVE, outerTry.errorVar, castedError));
  ret.insertAtTail(gotoHandler());

  return ret;
}

static AList errorCondHelper(VarSymbol* errorVar,
                             BlockStmt* thenBlock, BlockStmt* elseBlock);

AList ErrorHandlingVisitor::errorCond(VarSymbol* errorVar,
                                      BlockStmt* thenBlock,
                                      BlockStmt* elseBlock) {
  return errorCondHelper(errorVar, thenBlock, elseBlock);
}

static AList errorCondHelper(VarSymbol* errorVar,
                             BlockStmt* thenBlock, BlockStmt* elseBlock)
{
  VarSymbol* errorExistsVar = newTemp("shouldHandleError", dtBool);
  CallExpr*  errorExists    = new CallExpr(PRIM_CHECK_ERROR, errorVar);

  AList ret;
  ret.insertAtTail(new DefExpr(errorExistsVar));
  ret.insertAtTail(new CallExpr(PRIM_MOVE, errorExistsVar, errorExists));
  ret.insertAtTail(new CondStmt(new SymExpr(errorExistsVar),
                                thenBlock, elseBlock));
  return ret;
}

// Emit code that can halt with the error.
// The tryBang argument indicates if the user requested a halt-on-error
// (with try!). If not, the compiler is adding the halt-on-error for one
// reason or another and later passes should be able to change the halt
// into other error handling (as with, say, iterator inlining).
CallExpr* ErrorHandlingVisitor::haltExpr(VarSymbol* errorVar, bool tryBang) {
  if (tryBang)
    return new CallExpr(gChplUncaughtError, errorVar);

  return new CallExpr(gChplPropagateError, errorVar);
}


static void printReason(BaseAST* node, implicitThrowsReasons_t* reasons)
{
  if (reasons == NULL)
    return;

  if (CallExpr* call = toCallExpr(node)) {
    if (FnSymbol* calledFn = call->resolvedFunction()) {
      if (calledFn->throwsError()) {
        if (reasons->count(calledFn)) {
          BaseAST* reason = (*reasons)[calledFn];
          USR_PRINT(reason, "call to throwing function here");
          printReason(reason, reasons);
        }
      }
    }
  }
}

// Returns true if the catches don't cover all of the cases.
static bool catchesNotExhaustive(TryStmt* tryStmt) {
  if (tryStmt->tryBang())
    return false;

  bool hasCatchAll = false;

  for_alist(c, tryStmt->_catches) {
    CatchStmt* catchStmt = toCatchStmt(c);

    // catchall
    if (catchStmt->isCatchall()) {
      hasCatchAll = true;
    }
  }

  return !hasCatchAll;
}

// Returns true if we should raise strict-mode errors
// for this call.
static bool shouldEnforceStrict(CallExpr* node, int taskFunctionDepth) {
  if (FnSymbol* calledFn = node->resolvedFunction()) {
    bool inCompilerGeneratedFn = false;
    bool inDefaultActualFn = false;
    if (FnSymbol* parentFn = toFnSymbol(node->parentSymbol)) {
      // Don't check wrapper functions in strict mode, unless they are task
      // functions and we know the caller of the task function is not declared
      // as throws.
      inCompilerGeneratedFn = isCompilerGeneratedFunction(parentFn) &&
        !(isTaskFun(parentFn) && taskFunctionDepth > 0);
      inDefaultActualFn = parentFn->hasFlag(FLAG_DEFAULT_ACTUAL_FUNCTION);
    }
    bool callsUncheckedThrowsFn = isUncheckedThrowsFunction(calledFn);
    bool strictError = !((inCompilerGeneratedFn && !inDefaultActualFn) ||
                         callsUncheckedThrowsFn);

    return strictError;
  }
  return false;
}


static AList castToErrorNilable(Symbol* error, SymExpr* &castedError) {
  AList ret;

  Type* nilableError = dtErrorNilable();

  if (error->type == nilableError) {
    castedError = new SymExpr(error);
  } else {
    VarSymbol* castedErrorVar = newTemp("castedError", nilableError);
    castedError = new SymExpr(castedErrorVar);

    CallExpr* castError = new CallExpr(PRIM_CAST, nilableError->symbol, error);
    ret.insertAtTail(new DefExpr(castedErrorVar));
    ret.insertAtTail(new CallExpr(PRIM_MOVE, castedErrorVar, castError));
  }

  return ret;
}

class ImplicitThrowsVisitor final : public AstVisitorTraverse {

public:
  ImplicitThrowsVisitor(std::set<FnSymbol*>* visited, implicitThrowsReasons_t* reasons);

  // possibly record a throwing function call
  void handleCallToFunction(FnSymbol* calledFn, Expr* forExpr);

  bool enterTryStmt  (TryStmt*   node) override;
  void exitTryStmt   (TryStmt*   node) override;
  bool enterCallExpr (CallExpr*  node) override;
  bool enterForLoop  (ForLoop*  node) override;

  // Does this function throw?
  bool throws() { return canThrow; }
  // Is the visited function only throwing because of calls to functions
  // marked with FLAG_UNCHECKED_THROWS?
  bool unchecked() { return onlyUnchecked; }
  // What made us decide that this function should be marked throws?
  BaseAST* reason() { return reasonThrows; }

private:

  int  tryDepth;
  bool canThrow;
  bool onlyUnchecked;
  BaseAST* reasonThrows; // one of the reasons it throws, for errors
  std::set<FnSymbol*>* visited;
  implicitThrowsReasons_t* reasons;
};

ImplicitThrowsVisitor::ImplicitThrowsVisitor(std::set<FnSymbol*>* visitedIn, implicitThrowsReasons_t* reasonsIn) {
  tryDepth = 0;
  canThrow = false;
  onlyUnchecked = true;
  reasonThrows = NULL;
  visited = visitedIn;
  reasons = reasonsIn;
}

void ImplicitThrowsVisitor::handleCallToFunction(FnSymbol* calledFn,
                                                 Expr* forExpr) {
  bool insideTry = (tryDepth > 0);

  // We might be calling a function that could be implicitly
  // throwing. For example, consider nested coforalls.
  // That will appear to be a call to coforall_fn1, and
  // that in turn appears to be a call to coforall_fn2.
  //
  // In that example, this enterCallExpr might be visiting
  // a call to coforall_fn2. We don't know yet if it throws
  // if we haven't visited it yet.
  if (calledFn->throwsError() == false)
    markImplicitThrows(calledFn, visited, reasons);

  CallExpr* call = toCallExpr(forExpr);
  TryTag tryTag = TRY_TAG_NONE;
  if (call)
    tryTag = call->tryTag;

  if (calledFn->throwsError()) {
    if (insideTry || tryTag == TRY_TAG_IN_TRYBANG) {
      // OK
    } else {

      if (call && shouldEnforceStrict(call, /*taskFunctionDepth=*/0)) {
        if (reasonThrows == NULL)
          reasonThrows = forExpr;
      }

      // not in a try
      canThrow = true;
      if (!calledFn->hasFlag(FLAG_UNCHECKED_THROWS))
        onlyUnchecked = false;
    }
  }
}

bool ImplicitThrowsVisitor::enterTryStmt(TryStmt* node) {
  tryDepth++;

  return true;
}

void ImplicitThrowsVisitor::exitTryStmt(TryStmt* node) {
  tryDepth--;

  // is it a non-exhaustive catch?
  bool nonExhaustive = catchesNotExhaustive(node);

  if (tryDepth == 0)
    canThrow = canThrow || nonExhaustive;
  if (nonExhaustive)
    onlyUnchecked = false;
  if (reasonThrows == NULL)
    reasonThrows = node;
}

bool ImplicitThrowsVisitor::enterCallExpr(CallExpr* node) {
  bool insideTry = (tryDepth > 0);

  if (FnSymbol* calledFn = node->resolvedFunction()) {
    handleCallToFunction(calledFn, node);
  } else if (node->isPrimitive(PRIM_THROW)) {
    if (insideTry || node->tryTag == TRY_TAG_IN_TRYBANG) {
      // OK
    } else {
      canThrow = true;
      onlyUnchecked = false;
      if (reasonThrows == NULL)
        reasonThrows = node;
    }
  }
  return true;
}

bool ImplicitThrowsVisitor::enterForLoop (ForLoop*  node) {

  SymExpr* it = node->iteratorGet();

  FnSymbol* itFn = getTheIteratorFn(it->symbol()->type);

  handleCallToFunction(itFn, node);

  // Does the for loop run an iterator which throws?
  return true;
}


typedef enum {
  ERROR_MODE_UNKNOWN,
  ERROR_MODE_FATAL,
  ERROR_MODE_RELAXED,
  ERROR_MODE_STRICT,
} error_checking_mode_t;

class ErrorCheckingVisitor final : public AstVisitorTraverse {

public:
  ErrorCheckingVisitor(bool inThrowingFn, error_checking_mode_t inMode,
                       implicitThrowsReasons_t* reasons);

  bool enterTryStmt (TryStmt*   node) override;
  void exitTryStmt  (TryStmt*   node) override;
  bool enterCallExpr(CallExpr*  node) override;
  void exitDeferStmt(DeferStmt* node) override;

private:
  int  tryDepth;
  bool fnCanThrow;
  error_checking_mode_t mode;

  int taskFunctionDepth;

  implicitThrowsReasons_t* reasons;

  void checkCatches(TryStmt* tryStmt);
};

ErrorCheckingVisitor::ErrorCheckingVisitor(bool inThrowingFn,
    error_checking_mode_t inMode, implicitThrowsReasons_t* inReasons) {

  tryDepth = 0;
  fnCanThrow = inThrowingFn;
  mode = inMode;
  taskFunctionDepth = 0;
  reasons = inReasons;
}

bool ErrorCheckingVisitor::enterTryStmt(TryStmt* node) {
  if (!node->isSyncTry()) {
    tryDepth++;
  }

  return true;
}

void ErrorCheckingVisitor::exitTryStmt(TryStmt* node) {
  if (!node->isSyncTry()) {
    tryDepth--;
  }

  checkCatches(node);

  // is it an exhaustive catch?
  bool nonExhaustive = catchesNotExhaustive(node);

  if (tryDepth==0 && nonExhaustive && !fnCanThrow) {
    USR_FATAL_CONT(node, "try without a catchall in a non-throwing function");
  }
}

void ErrorCheckingVisitor::checkCatches(TryStmt* tryStmt) {

  bool hasCatchAll = false;

  for_alist(c, tryStmt->_catches) {
    if (hasCatchAll)
      USR_FATAL_CONT(c->prev, "catchall placed before the end of a catch list");

    CatchStmt* catchStmt = toCatchStmt(c);

    if (catchStmt->isCatchall())
      hasCatchAll = true;
  }
}

static void issueThrowingFnError(FnSymbol* calledFn,
                                 CallExpr* node,
                                 implicitThrowsReasons_t* reasons,
                                 const char* problem) {
  const char* desc = "cast";
  bool cast = true;
  if (calledFn->name != astrScolon) {
    desc = astr("function ", calledFn->name);
    cast = false;
  }
  USR_FATAL_CONT(node, "call to throwing %s %s", desc, problem);
  if (!cast) {
    USR_PRINT(calledFn, "throwing function %s defined here", calledFn->name);
  }
  printReason(node, reasons);
}

/*
If this is a call to a task function TF:
* The call itself is (correctly) not checked for errors.
* If TF is not implicitly-throwing
  (which happens when all errors inside it, if any, are handled):
  - no error checking inside TF is performed here;
  - error checking inside TF is done by checkErrorHandling(TF),
    at which point TF is treated as a non-throwing function;
    however the only errors it can generate are an improper catchall
    or "throwing call without try or try! (strict mode)",
    if there were any other offenders, it would have been marked
    implicitly-throwing.
* If TF is implicitly-throwing:
  - TF is checked recursively here if its enclosing non-task function
    is non-throwing, including the case where taskFunctionDepth > 0,
    which can occur only under this same condition;
    TODO: simplify this check to 'if (!fnCanThrow)'
  - TF is checked again by checkErrorHandling(TF),
    at which point TF is treated as a throwing function; for this reason
    the only errors it can generate are again an improper catchall
    or "throwing call without try or try! (strict mode)", all other
    conditions are acceptable within a throwing function.
TODO: simplify this logic and do not descend into TF here.
Instead check inside a TF only in checkErrorHandling(). Use FLAG_OUTSIDE_TRY
to determine whether the enclosing function is non-throwing.
*/
bool ErrorCheckingVisitor::enterCallExpr(CallExpr* node) {
  bool insideTry = (tryDepth > 0);

  if (FnSymbol* calledFn = node->resolvedFunction()) {
    if (calledFn->throwsError()) {

      bool inThrowingFunction = false;
      if (FnSymbol* parentFn = toFnSymbol(node->parentSymbol)) {
        inThrowingFunction = parentFn->throwsError();

        if (!inThrowingFunction && isTaskFun(calledFn)) {
          taskFunctionDepth++;
          calledFn->body->accept(this);

          taskFunctionDepth--;
          return true;
        } else if (taskFunctionDepth > 0) {
          if (isTaskFun(calledFn)) {
            taskFunctionDepth++;
            calledFn->body->accept(this);

            taskFunctionDepth--;
            return true;
          } else {
            // The above logic never descends into a task function - therefore
            // never gets to this point - unless the top-most non-task parentFn
            // is non-throwing. Cf. at this point we are in a throwing task fn.
            // So, adjust 'inThrowingFunction' to correspond to the top-most
            // non-task parentFn. This will make it equivalent to 'fnCanThrow'.
            inThrowingFunction = false;
          }
        } else if (isTaskFun(calledFn)) {
          // One way or another, we should not be going further
          // if it is a task function.
          return true;
        } else {
        }
      }
      INT_ASSERT(inThrowingFunction == fnCanThrow);

      if (insideTry || node->tryTag == TRY_TAG_IN_TRYBANG) {

        // OK, in a try { } or marked with try!

      } else if(node->tryTag == TRY_TAG_IN_TRY) {
        if (!inThrowingFunction) {
          issueThrowingFnError(calledFn, node, reasons,
                               "is in a try but not handled");
        }

        // Otherwise, OK, a try in a throwing function

      } else {
        if (shouldEnforceStrict(node, taskFunctionDepth)) {
          if (mode == ERROR_MODE_STRICT) {
            issueThrowingFnError(calledFn, node, reasons,
                                 "without try or try! (strict mode)");
          } else if (mode == ERROR_MODE_RELAXED && !inThrowingFunction) {
            issueThrowingFnError(calledFn, node, reasons,
                                 "without throws, try, or try! (relaxed mode)");
          }
        }
      }
    }
  } else if (node->isPrimitive(PRIM_THROW)) {
    if (insideTry) {
      // OK, error checking for this case done in try handling
    } else if (fnCanThrow == true) {
      // OK, fn can throw
    } else {
      USR_FATAL_CONT(node, "cannot throw in a non-throwing function");
    }
  }
  return true;
}

void ErrorCheckingVisitor::exitDeferStmt(DeferStmt* node) {
  if (mode == ERROR_MODE_FATAL) {

    // OK, no checking needed

  } else if (canBlockStmtThrow(node->body())) {
    USR_FATAL_CONT(node, "error handling in defer blocks must be complete");
    printReason(node, reasons);
  }
}

} /* end anon namespace */

Symbol* findErrorVarForHandlerLabel(LabelSymbol* handlerLabel) {
  // find the error that this handlerLabel is working with
  for(Expr* e = handlerLabel->defPoint->next; e != NULL; e = e->next) {
    std::vector<CallExpr*> calls;
    collectCallExprs(e, calls);
    for_vector(CallExpr, call, calls) {
      if (call->isPrimitive(PRIM_CHECK_ERROR)) {
        SymExpr* se = toSymExpr(call->get(1));
        INT_ASSERT(se->symbol()->hasFlag(FLAG_ERROR_VARIABLE));
        return se->symbol();
      }
    }
  }

  INT_FATAL("Could not find error variable for handler");
  return NULL;
}

struct ErrorVarAndGoto { Symbol* errorVar; GotoStmt* gotoHandler; };

static ArgSymbol* errorFormal(FnSymbol* fn) {
  for_formals(formal, fn)
    if (formal->hasFlag(FLAG_ERROR_VARIABLE))
      return formal;
  return NULL;
}

//
// Returns (error variable, goto to the handler label) for the error handling
// block of 'fs'. If the error handling block does not exist, creates it.
//
// The newly-created error handler is to be used for an IBB only, i.e.,
// to handle an error thrown within loop body, whereas the ForallStmt
// and the IBB (which is right after a yield) are within an iterator.
// The error received by this error handler should not be passed to an
// enclosing try-block because errors thrown within loop body should not be
// handled by the iterator's try-blocks. Although this will happen currently,
// for example when the ForallStmt has an existing error handler that passes
// an error to an enclosing try-block.
//
// The label is created analogously to ErrorHandlingVisitor::enterForallStmt().
// The goto looks like the one created in ErrorHandlingVisitor::gotoHandler().
//
static ErrorVarAndGoto forallHandlerForIBB(ForallStmt* fs) {
  LabelSymbol* handlerLabel = fs->fErrorHandlerLabel;
  VarSymbol*   errorVar     = NULL;
  if (handlerLabel == nullptr) {
    // distinguish its name from regular forall_break_label, for debugging
    handlerLabel = new LabelSymbol("forall_IBB_break_label");
    handlerLabel->addFlag(FLAG_FORALL_BREAK_LABEL);
    fs->fErrorHandlerLabel = handlerLabel;
    fs->insertAfter(new DefExpr(handlerLabel));
    // 'fs' not having handlerLabel means it does not have errorVar either.
    errorVar = createAndInsertErrorVar(fs);

    // Pass errorVar to the enclosing error handling entity:
    // either the function's out_error or the forall's error.
    ErrorVarAndGoto parentEG = {NULL, NULL};
    if (ForallStmt* parentFS = enclosingForallStmt(fs)) {
      // connect to the enclosing ForallStmt
      parentEG = forallHandlerForIBB(parentFS);
    }
    else {
      // connect to the function's out_error
      FnSymbol* parentFn = toFnSymbol(fs->parentSymbol);
      parentEG.gotoHandler = new GotoStmt(GOTO_ERROR_HANDLING_RETURN,
                                   parentFn->getOrCreateEpilogueLabel());
      parentEG.errorVar = errorFormal(parentFn);
    }

    BlockStmt* errorPolicy = new BlockStmt();
    if (parentEG.errorVar == nullptr) {
      errorPolicy->insertAtTail(new CallExpr(gChplPropagateError, errorVar));
    }
    else {
      errorPolicy->insertAtTail(
                        new CallExpr(PRIM_ASSIGN, parentEG.errorVar, errorVar));
      errorPolicy->insertAtTail(parentEG.gotoHandler);
    }
    handlerLabel->defPoint->insertAfter(
                                errorCondHelper(errorVar, errorPolicy, NULL));
  }
  else {
    // Find the already-existing errorVar.
    errorVar = toVarSymbol(findErrorVarForHandlerLabel(handlerLabel));
  }

  ErrorVarAndGoto result =
    {errorVar, new GotoStmt(GOTO_BREAK_ERROR_HANDLING, handlerLabel)};
  return result;
}

// Returns a new GotoStmt to fs->fErrorHandlerLabel.
// If the label is not there, creates the label, the error variable,
// and the error handling block.
GotoStmt* gotoForallErrorHandler(ForallStmt* fs) {
  return forallHandlerForIBB(fs).gotoHandler;
}

bool canFunctionImplicitlyThrow(FnSymbol* fn)
{
  // task functions can be implicit throws
  if (isTaskFun(fn))
    return true;
  // loop-expr functions can be implicit throws
  if (isLoopExprFun(fn))
    return true;
  // initCopy promoting iterators to arrays can be too
  if (fn->hasFlag(FLAG_INIT_COPY_FN))
    if (fn->numFormals() >= 2)  // definedConst is always the last arg
      if (ArgSymbol* arg = fn->getFormal(1))
        if (arg->type->symbol->hasFlag(FLAG_ITERATOR_RECORD))
          return true;

  // otherwise, don't mark it
  return false;
}

// Returns `true` if a block can exit with an error
//  (e.g. by calling 'throw' or a throwing function,
//   when these are not handled by try! or catch).
// This function is useful to infer 'throws' for
// certain compiler-introduced functions.

static void markImplicitThrows(FnSymbol* fn, std::set<FnSymbol*>* visited, implicitThrowsReasons_t* reasons)
{
  // Currently, only task functions and if-exprs can be implicitly throws.
  if (!canFunctionImplicitlyThrow(fn))
    return;

  // If we already visited this function, don't visit it again.
  if (visited->count(fn) > 0)
    return;

  // Add to visited set first thing to prevent infinite recursion
  // if there ever is a cycle of calls.
  visited->insert(fn);

  ImplicitThrowsVisitor visit(visited, reasons);

  // Note that this function can recurse because
  // the ImplicitThrowsVisitor will call markImplicitThrows.
  fn->body->accept(&visit);

  if (visit.throws()) {
    // Update the reasons - for printing out error messages with
    // the user view of no task functions (say)
    (*reasons)[fn] = visit.reason();
    fn->throwsErrorInit();

    // If we only are throwing because of calls to functions marked
    // with FLAG_UNCHECKED_THROWS, this function should also be
    // so marked.
    if (visit.unchecked())
      fn->addFlag(FLAG_UNCHECKED_THROWS);
  }
}

static bool
canBlockStmtThrow(BlockStmt* block)
{
  std::set<FnSymbol*> visited;
  implicitThrowsReasons_t reasons;

  ImplicitThrowsVisitor visit(&visited, &reasons);
  block->accept(&visit);

  return visit.throws();
}

// Compute the error handling mode to use
//
// Go up symbols, starting with fn, looking for flag
// This allows the flag to be set on an outer module in the
// case of nested modules.
static error_checking_mode_t computeErrorCheckingMode(FnSymbol* fn)
{
  error_checking_mode_t mode = ERROR_MODE_UNKNOWN;

  Symbol* cur = fn;

  while (cur != NULL && cur->defPoint != NULL) {
    if (cur->hasFlag(FLAG_ERROR_MODE_FATAL)) {
      mode = ERROR_MODE_FATAL;
      break;
    }
    if (cur->hasFlag(FLAG_ERROR_MODE_RELAXED)) {
      mode = ERROR_MODE_RELAXED;
      break;
    }
    if (cur->hasFlag(FLAG_ERROR_MODE_STRICT)) {
      mode = ERROR_MODE_STRICT;
      break;
    }

    cur = cur->defPoint->parentSymbol;
  }

  if (mode == ERROR_MODE_UNKNOWN) {
    // No mode was chosen explicitly, find the default.

    ModuleSymbol* mod = fn->getModule();
    if (mod->hasFlag(FLAG_IMPLICIT_MODULE) ||
        fPermitUnhandledModuleErrors ||
        mod->hasFlag(FLAG_PROTOTYPE_MODULE))
      mode = ERROR_MODE_FATAL;
    else
      mode = ERROR_MODE_RELAXED;
  }

  return mode;
}

static void checkErrorHandling(FnSymbol* fn, implicitThrowsReasons_t* reasons)
{
  if (strcmp(fn->name, "deinit") == 0) {
    if (fn->throwsError())
      USR_FATAL_CONT(fn, "deinit is not permitted to throw");
  }

  error_checking_mode_t mode = computeErrorCheckingMode(fn);
  INT_ASSERT(mode != ERROR_MODE_UNKNOWN);

  ErrorCheckingVisitor visit(fn->throwsError(), mode, reasons);

  fn->body->accept(&visit);
}


static ArgSymbol* addOutErrorArg(FnSymbol* fn)
{
  ArgSymbol* outError = NULL;

  SET_LINENO(fn);

  outError = new ArgSymbol(INTENT_REF, "error_out", dtErrorNilable());
  outError->addFlag(FLAG_ERROR_VARIABLE);
  fn->insertFormalAtTail(outError);

  return outError;
}

static void lowerErrorHandling(FnSymbol* fn)
{
  ArgSymbol*   outError = NULL;
  LabelSymbol* epilogue = NULL;

  if (fn->throwsError()) {
    SET_LINENO(fn);

    outError = addOutErrorArg(fn);
    epilogue = fn->getOrCreateEpilogueLabel();
    INT_ASSERT(epilogue); // throws requires an epilogue
  }

  InitErrorHandling* state = NULL;
  if (fn->isInitializer() == true ||
      fn->isCopyInit() == true) {
    state = new InitErrorHandling(fn);
  }

  ErrorHandlingVisitor visitor = ErrorHandlingVisitor(outError, epilogue,
                                                      state);
  fn->accept(&visitor);

  if (state != NULL) {
    visitor.state->removeInitDone();
    visitor.state = NULL;
    delete state;
  }
}

void lowerCheckErrorPrimitive()
{
  forv_expanding_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_CHECK_ERROR) && call->inTree()) {
      SET_LINENO(call);

      SymExpr* errSe   = toSymExpr(call->get(1));
      Symbol*  errorVar= errSe->symbol();

      VarSymbol* errorExistsVar = newTemp("errorExists", dtBool);
      DefExpr*   def            = new DefExpr(errorExistsVar);
      CallExpr*  errorExists    = new CallExpr(PRIM_NOTEQUAL, errorVar, gNil);
      CallExpr*  move = new CallExpr(PRIM_MOVE, errorExistsVar, errorExists);

      Expr* stmt = call->getStmtExpr();
      stmt->insertBefore(def);
      def->insertAfter(move);
      call->replace(new SymExpr(errorExistsVar));
    }
  }
}

bool isCheckErrorStmt(Expr* e)
{
  if (CondStmt* cond = toCondStmt(e)) {
    if (CallExpr* call = toCallExpr(cond->condExpr)) {
      if (call->isPrimitive(PRIM_CHECK_ERROR)) {
        return true;
      }
    }
  }
  return false;
}

Symbol* getErrorSymbolFromCheckErrorStmt(Expr* e)
{
  if (CondStmt* cond = toCondStmt(e)) {
    if (CallExpr* call = toCallExpr(cond->condExpr)) {
      if (call->isPrimitive(PRIM_CHECK_ERROR)) {
        SymExpr* errSe   = toSymExpr(call->get(1));
        Symbol*  errorVar= errSe->symbol();
        return errorVar;
      }
    }
  }
  INT_FATAL("bad call to getErrorSymbolFromCheckErrorStmt");
  return NULL;
}

// Should we raise an error in strict mode if the error is not handled?
// No for calls inside of compiler-generated functions, wrapper functions,
// or task functions. No for functions marked with FLAG_UNCHECKED_THROWS.
static bool isCompilerGeneratedFunction(FnSymbol* fn)
{
  return isTaskFun(fn) ||
         fn->hasFlag(FLAG_WRAPPER) ||
         fn->hasFlag(FLAG_COMPILER_GENERATED);
}

static bool isUncheckedThrowsFunction(FnSymbol* fn)
{
  return fn->hasFlag(FLAG_UNCHECKED_THROWS);
}


void lowerErrorHandling() {
  if (!fMinimalModules)
    INT_ASSERT(dtError->inTree());

  std::set<FnSymbol*> visited;
  implicitThrowsReasons_t reasons;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    // Determine if compiler-generated fns should be marked 'throws'
    markImplicitThrows(fn, &visited, &reasons);
  }

  // Note, the above step can adjust the AST in a way
  // that impacts this step.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    // Otherwise, just check for error-handling errors.
    checkErrorHandling(fn, &reasons);
  }

  // Quit if fatal errors were encountered by checkErrorHandling above.
  USR_STOP();

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    lowerErrorHandling(fn);
  }

  // Note, PRIM_CHECK_ERROR will be lowered when a later
  // pass calls lowerCheckErrorPrimitive().
}
