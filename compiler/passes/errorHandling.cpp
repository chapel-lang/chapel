/*
 * Copyright 2004-2017 Cray Inc.
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
#include "stmt.h"
#include "symbol.h"
#include "TryStmt.h"

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
    } else {
      var e = error: AnotherSubError;
      if e {
        g();
      } else {
        // set and return
        error_out = error;
        goto epilogue_label;
      }
    }
  }
}
*/

class ErrorHandlingVisitor : public AstVisitorTraverse {

public:
  ErrorHandlingVisitor       (ArgSymbol* _outFormal, LabelSymbol* _epilogue);

  virtual bool enterTryStmt  (TryStmt*   node);
  virtual void exitTryStmt   (TryStmt*   node);
  virtual bool enterCatchStmt(CatchStmt*   node);
  virtual void exitCatchStmt (CatchStmt*   node);
  virtual bool enterCallExpr (CallExpr*  node);

private:
  struct TryInfo {
    VarSymbol*   errorVar;
    LabelSymbol* handlerLabel;
  };

  std::stack<TryInfo> tryStack;
  ArgSymbol*          outError;
  LabelSymbol*        epilogue;
  bool                insideCatch;

  AList     tryHandler         (TryStmt*   tryStmt,  VarSymbol* errorVar);
  AList     setOutGotoEpilogue (VarSymbol* error);
  AList     errorCond          (VarSymbol* errorVar, BlockStmt* thenBlock,
                                BlockStmt* elseBlock = NULL);
  CallExpr* haltExpr           ();

  ErrorHandlingVisitor();
};

ErrorHandlingVisitor::ErrorHandlingVisitor(ArgSymbol*   _outError,
                                           LabelSymbol* _epilogue) {
  outError = _outError;
  epilogue = _epilogue;
}

bool ErrorHandlingVisitor::enterTryStmt(TryStmt* node) {
  SET_LINENO(node);

  // TODO: nested try
  if (!tryStack.empty()) {
    USR_FATAL(node, "nested try is not yet supported");
  }

  VarSymbol*   errorVar     = newTemp("error", dtError);
  LabelSymbol* handlerLabel = new LabelSymbol("handler");
  TryInfo      info         = {errorVar, handlerLabel};
  tryStack.push(info);

  return true;
}

void ErrorHandlingVisitor::exitTryStmt(TryStmt* node) {
  SET_LINENO(node);

  TryInfo    info     = tryStack.top();
  BlockStmt* tryBlock = node->body();

  tryBlock->insertAtHead(new DefExpr(info.errorVar));

  tryBlock->insertAtTail(new DefExpr(info.handlerLabel));
  tryBlock->insertAtTail(tryHandler(node, info.errorVar));

  tryBlock->remove();
  node    ->replace(tryBlock);
  tryStack.pop();
}

AList ErrorHandlingVisitor::tryHandler(TryStmt* tryStmt, VarSymbol* errorVar) {
  BlockStmt* handlers    = new BlockStmt();

  bool       hasCatchAll = false;
  BlockStmt* currHandler = handlers;

  for_alist(c, tryStmt->_catches) {
    if (hasCatchAll) {
      USR_FATAL(c->prev, "catchall placed before the end of a catch list");
    }
    SET_LINENO(c);

    CatchStmt* catchStmt = toCatchStmt(c);
    BlockStmt* catchBody = catchStmt->body();
    DefExpr*   errorDef  = catchStmt->expr();
    Type*      errorType = errorDef ? errorDef->sym->type : NULL;

    catchBody->remove();

    // catchall
    if (errorType == NULL) {
      hasCatchAll = true;
      currHandler->insertAtTail(catchBody);

    // named catchall
    } else if (errorType == dtError) {
      hasCatchAll = true;
      CallExpr* moveError = new CallExpr(PRIM_MOVE, errorDef->sym, errorVar);

      errorDef->remove();
      currHandler->insertAtTail(errorDef);
      currHandler->insertAtTail(moveError);
      currHandler->insertAtTail(errorCond(toVarSymbol(errorDef->sym),
                                          catchBody));

    // specified catch
    } else {
      CallExpr*  castError   = new CallExpr(PRIM_DYNAMIC_CAST,
                                            new SymExpr(errorType->symbol),
                                            errorVar);
      CallExpr*  moveError   = new CallExpr(PRIM_MOVE, errorDef->sym,
                                            castError);
      BlockStmt* nextHandler = new BlockStmt();

      errorDef->remove();
      currHandler->insertAtTail(errorDef);
      currHandler->insertAtTail(moveError);
      currHandler->insertAtTail(errorCond(toVarSymbol(errorDef->sym),
                                          catchBody, nextHandler));

      currHandler = nextHandler;
    }
  }

  if (!hasCatchAll) {
    if (tryStmt->tryBang()) {
      currHandler->insertAtTail(haltExpr());
    } else if (outError) {
      currHandler->insertAtTail(setOutGotoEpilogue(errorVar));
    } else {
      USR_FATAL(tryStmt, "try without a catchall in a non-throwing function");
    }
  }

  return errorCond(errorVar, handlers);
}

bool ErrorHandlingVisitor::enterCatchStmt(CatchStmt* node) {
  insideCatch = true;
  return true;
}

void ErrorHandlingVisitor::exitCatchStmt(CatchStmt* node) {
  insideCatch = false;
}

bool ErrorHandlingVisitor::enterCallExpr(CallExpr* node) {
  bool insideTry = !tryStack.empty();

  if (FnSymbol* calledFn = node->resolvedFunction()) {
    if (calledFn->throwsError()) {
      SET_LINENO(node);

      VarSymbol* errorVar    = NULL;
      BlockStmt* errorPolicy = new BlockStmt();

      if (insideTry) {
        TryInfo info = tryStack.top();
        errorVar = info.errorVar;

        errorPolicy->insertAtTail(new GotoStmt(GOTO_ERROR_HANDLING,
                                               info.handlerLabel));
      } else if (fStrictErrorHandling) {
        USR_FATAL(node, "throwing call without try or try! (strict mode)");
      } else {
        // without try, need an error variable
        errorVar = newTemp("error", dtError);
        node->getStmtExpr()->insertBefore(new DefExpr(errorVar));

        if (outError)
          errorPolicy->insertAtTail(setOutGotoEpilogue(errorVar));
        else
          errorPolicy->insertAtTail(haltExpr());
      }

      node->insertAtTail(errorVar);
      node->insertAfter(errorCond(errorVar, errorPolicy));
    }
  } else if (node->isPrimitive(PRIM_THROW)) {
    SET_LINENO(node);

    BlockStmt* throwBlock = new BlockStmt();
    node->replace(throwBlock);

    SymExpr*   thrownExpr  = toSymExpr(node->get(1)->remove());
    VarSymbol* thrownError = toVarSymbol(thrownExpr->symbol());

    if (insideTry && !insideCatch) {
      TryInfo   info      = tryStack.top();
      CallExpr* castError = new CallExpr(PRIM_CAST, dtError->symbol,
                                         thrownError);
      throwBlock->insertAtTail(new CallExpr(PRIM_MOVE, info.errorVar,
                                            castError));
      throwBlock->insertAtTail(new GotoStmt(GOTO_ERROR_HANDLING,
                                            info.handlerLabel));
    } else if (outError) {
      throwBlock->insertAtTail(setOutGotoEpilogue(thrownError));
    } else {
      USR_FATAL(node, "cannot throw in a non-throwing function");
    }
  }
  return true;
}

// Sets the fn out variable with the given error, then goes to the fn epilogue.
AList ErrorHandlingVisitor::setOutGotoEpilogue(VarSymbol* error) {
  CallExpr* castError = new CallExpr(PRIM_CAST, dtError->symbol, error);

  AList ret;
  ret.insertAtTail(new CallExpr(PRIM_MOVE, outError, castError));
  ret.insertAtTail(new GotoStmt(GOTO_RETURN, epilogue));

  return ret;
}

AList ErrorHandlingVisitor::errorCond(VarSymbol* errorVar,
                                      BlockStmt* thenBlock,
                                      BlockStmt* elseBlock) {
  VarSymbol* errorExistsVar = newTemp("errorExists", dtBool);
  CallExpr*  errorExists    = new CallExpr(PRIM_NOTEQUAL, errorVar, gNil);

  AList ret;
  ret.insertAtTail(new DefExpr(errorExistsVar));
  ret.insertAtTail(new CallExpr(PRIM_MOVE, errorExistsVar, errorExists));
  ret.insertAtTail(new CondStmt(new SymExpr(errorExistsVar),
                                thenBlock, elseBlock));
  return ret;
}

// TODO: take in a halt message from the error
CallExpr* ErrorHandlingVisitor::haltExpr() {
  return new CallExpr(PRIM_RT_ERROR, new_CStringSymbol("uncaught error"));
}

void lowerErrorHandling() {
  if (!fMinimalModules)
    INT_ASSERT(dtError->inTree());

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    ArgSymbol*   outError = NULL;
    LabelSymbol* epilogue = NULL;

    if (fn->throwsError()) {
      SET_LINENO(fn);

      outError = new ArgSymbol(INTENT_REF, "error_out", dtError);
      fn->insertFormalAtTail(outError);

      epilogue = fn->getOrCreateEpilogueLabel();
      INT_ASSERT(epilogue); // throws requires an epilogue
    }

    ErrorHandlingVisitor visitor = ErrorHandlingVisitor(outError, epilogue);
    fn->accept(&visitor);
  }
}
