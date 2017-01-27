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
#include "stmt.h"
#include "symbol.h"
#include "TryStmt.h"
#include "view.h"

#include <stack>

class ErrorHandlingVisitor : public AstVisitorTraverse {
  struct TryInfo {
    VarSymbol*   tempError;
    LabelSymbol* catches;
    TryStmt*     tryStmt;
  };

  struct ErrorCond {
    DefExpr*  defErrorExists;
    CallExpr* setErrorExists;
    CondStmt* checkError;
  };

  VarSymbol*          tempError;
  std::stack<TryInfo> tryStack;

  virtual bool enterTryStmt (TryStmt*  node);
  virtual void exitTryStmt  (TryStmt*  node);
  virtual bool enterCallExpr(CallExpr* node);

private:
  static ErrorCond ifErrorThen(VarSymbol* tempError, Stmt* thenStmt);
  static void insertErrorCond(CallExpr* node, ErrorCond errorCond);
  static void haltOrPropagateError(BlockStmt* haltOrProp, FnSymbol* parentFn,
                                   VarSymbol* tempError, TryStmt* tryStmt);
};

bool ErrorHandlingVisitor::enterTryStmt(TryStmt* node) {
  SET_LINENO(node);
  VarSymbol* tempError;
  if (tryStack.empty()) {
    tempError = newTemp("error", dtObject);
  } else {
    tempError = tryStack.top().

  LabelSymbol* catches   = new LabelSymbol("catches" + tryStack.size());
  TryInfo      info      = {tempError, catches, node};

  tryStack.push(info);

  BlockStmt* tryBody = node->body();
  tryBody->insertAtHead(new DefExpr(tempError));
  tryBody->insertAtTail(new DefExpr(catches));

  return true;
}

void ErrorHandlingVisitor::exitTryStmt(TryStmt* node) {
  SET_LINENO(node);

  BlockStmt* tryBody = node->body();
  tryBody->remove();
  node   ->replace(tryBody);

  TryInfo info = tryStack.top();

  VarSymbol* tempError    = info.tempError;
  BlockStmt* haltOrProp   = new BlockStmt();

  // TODO: create catches
  BlockStmt* catchesBlock = new BlockStmt(haltOrProp);

  ErrorCond errorCond = ifErrorThen(tempError, catchesBlock);
  tryBody->insertAtTail(errorCond.defErrorExists);
  tryBody->insertAtTail(errorCond.setErrorExists);
  tryBody->insertAtTail(errorCond.checkError);

  haltOrPropagateError(haltOrProp, tryBody->getFunction(),
    tempError, info.tryStmt);

  tryStack.pop();
}

bool ErrorHandlingVisitor::enterCallExpr(CallExpr* node) {
  bool insideTry = !tryStack.empty();

  if (FnSymbol* fn = node->resolvedFunction()) {
    if (fn->throwsError()) {
      SET_LINENO(node);
      if (insideTry) {
        TryInfo    info        = tryStack.top();
        VarSymbol* tempError   = info.tempError;
        GotoStmt*  gotoCatches = new GotoStmt(GOTO_ERROR_HANDLING,
                                   info.catches);

        node->insertAtTail(tempError);

        insertErrorCond(node, ifErrorThen(tempError, gotoCatches));
      } else {
        VarSymbol* tempError  = newTemp("error", dtObject);
        BlockStmt* haltOrProp = new BlockStmt();

        node->getStmtExpr()->insertBefore(new DefExpr(tempError));
        node->insertAtTail(tempError);

        insertErrorCond(node, ifErrorThen(tempError, haltOrProp));

        haltOrPropagateError(haltOrProp, node->getFunction(), tempError, NULL);
      }
    }
  } else if (node->isPrimitive(PRIM_THROW)) {
    SET_LINENO(node);

    VarSymbol* tempError;
    TryStmt*   tryStmt;
    BlockStmt* throwBlock = new BlockStmt();
    if (insideTry) {
      TryInfo info   = tryStack.top();
      tempError      = info.tempError;
      tryStmt        = info.tryStmt;
    } else {
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

ErrorHandlingVisitor::ErrorCond ErrorHandlingVisitor::ifErrorThen(
    VarSymbol* tempError, Stmt* thenStmt) {

  CallExpr*  errorExists     = new CallExpr(PRIM_NOTEQUAL, tempError, gNil);
  VarSymbol* tempErrorExists = newTemp("errorExists", dtBool);

  DefExpr*  defErrorExists = new DefExpr(tempErrorExists);
  CallExpr* setErrorExists = new CallExpr(PRIM_MOVE,
                               tempErrorExists, errorExists);
  CondStmt* checkError     = new CondStmt(new SymExpr(tempErrorExists),
                               thenStmt);

  ErrorCond ret = {defErrorExists, setErrorExists, checkError};
  return ret;
}

void ErrorHandlingVisitor::insertErrorCond(CallExpr* node, ErrorCond errorCond) {
  // insert in reverse order
  node->getStmtExpr()->insertAfter(errorCond.checkError);
  node->getStmtExpr()->insertAfter(errorCond.defErrorExists);
  node->getStmtExpr()->insertAfter(errorCond.setErrorExists);
}

// TODO: need callsite for SET_LINENO
void ErrorHandlingVisitor::UnhandledErrorInsideTry(BlockStmt* haltOrProp,
    FnSymbol* parentFn, TryInfo info) {

  VarSymbol* tempError = info.tempError;
  TryStmt* tryStmt = info.tryStmt;

  bool tryBang = tryStmt->tryBang();
  if (!tryBang && !parentFn->throwsError())
    compilerError("no catch-all for try in function that does not throw");

  if (parentFn->throwsError()) {
    halt = tryStmt->tryBang();
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

    Expr* castError  = new CallExpr(PRIM_CAST, dtObject->symbol, tempError);
    Expr* setError   = new CallExpr(PRIM_MOVE, outError, castError);
    Expr* gotoReturn = new GotoStmt(GOTO_RETURN, label);

    haltOrProp->insertAtTail(setError);
    haltOrProp->insertAtTail(gotoReturn);
  }
}

// TODO: need callsite for SET_LINENO
void ErrorHandlingVisitor::UnhandledErrorOutsideTry(BlockStmt* haltOrProp,
    FnSymbol* parentFn, VarSymbol* tempError) {

  bool halt;
  if (parentFn->throwsError()) {
    // TODO: strict mode, throws without try
    halt = false;
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

    Expr* castError  = new CallExpr(PRIM_CAST, dtObject->symbol, tempError);
    Expr* setError   = new CallExpr(PRIM_MOVE, outError, castError);
    Expr* gotoReturn = new GotoStmt(GOTO_RETURN, label);

    haltOrProp->insertAtTail(setError);
    haltOrProp->insertAtTail(gotoReturn);
  }
}

// SOURCE
void ErrorHandlingVisitor::haltOrPropagateError(BlockStmt* haltOrProp,
    FnSymbol* parentFn, VarSymbol* tempError, TryStmt* tryStmt) {

  bool halt;
  if (parentFn->throwsError()) {
    halt = tryStmt->tryBang();
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

    Expr* castError  = new CallExpr(PRIM_CAST, dtObject->symbol, tempError);
    Expr* setError   = new CallExpr(PRIM_MOVE, outError, castError);
    Expr* gotoReturn = new GotoStmt(GOTO_RETURN, label);

    haltOrProp->insertAtTail(setError);
    haltOrProp->insertAtTail(gotoReturn);
  }
}
// TODO: dtObject should be dtError, but we don't have that right now
void lowerErrorHandling() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->throwsError()) {
      SET_LINENO(fn);

      ArgSymbol* outFormal = new ArgSymbol(INTENT_REF, "error_out", dtObject);
      fn->insertFormalAtTail(outFormal);
    }

    ErrorHandlingVisitor visitor;
    fn->accept(&visitor);
  }
}
