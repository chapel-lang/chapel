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

#include "AstVisitorTraverse.h"

#include "errorHandling.h"
#include "stmt.h"
#include "symbol.h"
#include "TryStmt.h"
#include "view.h"

#include <stack>

// for each function that 'throws':
  // add an error_out formal

/*
try {
  a();
  b(); // doesn't throw
  c();
} catch e: SubError {
  f();
} catch e: AnotherSubError {
  g();
} // propagates

// suppose this is in a throwing fn
{
  var _e: Error;
  a(_e);
  if _e then
    goto catches;
  b();
  c(_e);
  if _e then
    goto catches;

  label catches:
  if _e {
    if _e: SubError {
      f();
    } else if _e: AnotherSubError {
      g();
    } else {
      _e_out = _e;
      return (default val);
    }
  }
}
*/

struct TryInfo {
  VarSymbol*   tempError;
  LabelSymbol* catches;
  TryStmt*     tryStmt;
};

class ErrorHandlingVisitor : public AstVisitorTraverse {
  std::stack<TryInfo> tryStack;

  virtual bool enterTryStmt (TryStmt*  node);
  virtual void exitTryStmt  (TryStmt*  node);
  virtual bool enterCallExpr(CallExpr* node);
};

// TODO: default/strict mode
// TODO: need callsite for SET_LINENO
static void haltOrPropagateError(BlockStmt* haltOrProp, FnSymbol* parentFn,
                                 VarSymbol* tempError, TryStmt* tryStmt) {
  bool halt;
  if (parentFn->throwsError()) {
    if (tryStmt) {
      halt = tryStmt->tryBang();
    } else {
      // TODO: strict mode, throws without try
      halt = false;
    }
  } else {
    // TODO: strict mode, propagating try without throws
    halt = true;
  }

  if (halt) {
    Expr* haltOnError = new CallExpr(PRIM_RT_ERROR,
                          new_CStringSymbol("uncaught error"));
    haltOrProp->insertAtTail(haltOnError);
  } else {
    LabelSymbol* label = parentFn->getOrCreateEpilogueLabel();
    INT_ASSERT(label); // error handling needs an epilogue label

    DefExpr* outErrorExpr = toDefExpr(parentFn->formals.last());
    Symbol*  outError     = outErrorExpr->sym;
    // TODO: check with flag

    Expr* castError  = new CallExpr(PRIM_CAST, dtObject->symbol, tempError);
    Expr* setError   = new CallExpr(PRIM_MOVE, outError, castError);
    Expr* gotoReturn = new GotoStmt(GOTO_RETURN, label);

    haltOrProp->insertAtTail(setError);
    haltOrProp->insertAtTail(gotoReturn);
  }
}

static void ifErrorExists(Expr* insert, VarSymbol* tempError, Stmt* thenStmt) {
  CallExpr*  errorExists     = new CallExpr(PRIM_NOTEQUAL, tempError, gNil);
  VarSymbol* tempErrorExists = newTemp("errorExists", dtBool);

  DefExpr* defErrorExists = new DefExpr(tempErrorExists);
  Expr*    setErrorExists = new CallExpr(PRIM_MOVE,
                              tempErrorExists, errorExists);
  Expr*    checkError     = new CondStmt(new SymExpr(tempErrorExists),
                              thenStmt);

  insert        ->getStmtExpr()->insertAfter(defErrorExists);
  defErrorExists->getStmtExpr()->insertAfter(setErrorExists);
  setErrorExists->getStmtExpr()->insertAfter(checkError);
}

// enterTryStmt
  // create the error var
  // create catches label at end of the block
  // associate error var with present state
    // push (error var, catches label, try) onto back of stack
  // return true to traverse deeper

bool ErrorHandlingVisitor::enterTryStmt(TryStmt* node) {
  //printf("visiting try\n");
  SET_LINENO(node);

  VarSymbol*   tempError = newTemp("error", dtObject);
  LabelSymbol* catches   = new LabelSymbol("catches");
  BlockStmt*   tryBody   = node->body();

  tryBody->insertAtHead(new DefExpr(tempError));
  tryBody->insertAtTail(new DefExpr(catches));

  TryInfo info = {tempError, catches, node};
  tryStack.push(info);

  return true;
}

// exitTryStmt
  // create catches under the label:
    // if there is an error:
      // check each catch conditional, call proper fn
    // if there is no catchall:
      // halt or propagate error
  // pop (error var, try) off the stack

void ErrorHandlingVisitor::exitTryStmt(TryStmt* node) {
  //printf("exiting try\n");
  SET_LINENO(node);

  BlockStmt* tryBody = node->body();
  tryBody->remove();
  node   ->replace(tryBody);

  TryInfo info = tryStack.top();

  VarSymbol* tempError    = info.tempError;
  BlockStmt* haltOrProp   = new BlockStmt();

  // TODO: create catches
  BlockStmt* catchesBlock = new BlockStmt(haltOrProp);

  ifErrorExists(tryBody, tempError, catchesBlock);
  haltOrPropagateError(haltOrProp, tryBody->getFunction(),
    tempError, info.tryStmt);

  tryStack.pop();
}

// enterCallExpr
  // if the called fn throws:
    // if the stack isn't empty:
      // wire up the call to the error var at the top of the stack
      // create conditional after call: if error goto catches
    // otherwise:
      // if Default mode:
        // halt or propagate error
      // otherwise:
        // compilerError()
  // if the call is PRIM_THROW:
    // TODO

bool ErrorHandlingVisitor::enterCallExpr(CallExpr* node) {
  bool insideTry = !tryStack.empty();

  if (FnSymbol* fn = node->resolvedFunction()) {
    if (fn->throwsError()) {
      gdbShouldBreakHere();

      SET_LINENO(node);
      if (insideTry) {
        // printf("throwing call inside try\n");
        TryInfo    info        = tryStack.top();
        VarSymbol* tempError   = info.tempError;
        GotoStmt*  gotoCatches = new GotoStmt(GOTO_ERROR_HANDLING,
                                   info.catches);

        node->insertAtTail(tempError);

        ifErrorExists(node, tempError, gotoCatches);
      } else {
        // printf("throwing call outside try\n");
        VarSymbol* tempError  = newTemp("error", dtObject);
        BlockStmt* haltOrProp = new BlockStmt();

        node->getStmtExpr()->insertBefore(new DefExpr(tempError));
        node->insertAtTail(tempError);

        ifErrorExists(node, tempError, haltOrProp);
        haltOrPropagateError(haltOrProp, node->getFunction(), tempError, NULL);
      }
    }
  } else if (node->isPrimitive(PRIM_THROW)) {
    SET_LINENO(node);

    VarSymbol* tempError;
    TryStmt*   tryStmt;
    BlockStmt* throwBlock = new BlockStmt();
    if (insideTry) {
      // printf("throw inside try\n");
      TryInfo info   = tryStack.top();
      tempError      = info.tempError;
      tryStmt        = info.tryStmt;
    } else {
      // printf("throw outside try\n");
      tempError = newTemp("error", dtObject);
      tryStmt   = NULL;

      throwBlock->insertAtTail(new DefExpr(tempError));
    }

    node->replace(throwBlock);

    Expr* thrownError = node->get(1)->remove();
    Expr* castError   = new CallExpr(PRIM_CAST, dtObject->symbol, thrownError);
    Expr* setError    = new CallExpr(PRIM_MOVE, tempError, castError);

    throwBlock->insertAtTail(setError);
    haltOrPropagateError(throwBlock, throwBlock->getFunction(), tempError, tryStmt);
  }
  return true;
}

// TODO: dtObject should be dtError, but we don't have that right now
void lowerErrorHandling() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->throwsError()) {
      //printf("out arg to throwing fn\n");
      SET_LINENO(fn);

      ArgSymbol* outFormal = new ArgSymbol(INTENT_REF, "error_out", dtObject);
      fn->insertFormalAtTail(outFormal);
    }

    ErrorHandlingVisitor visitor;
    fn->accept(&visitor);
  }
}
