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

  std::stack<TryInfo> tryStack;

  virtual bool enterTryStmt (TryStmt*  node);
  virtual void exitTryStmt  (TryStmt*  node);
  virtual bool enterCallExpr(CallExpr* node);

private:
  static ErrorCond ifErrorThen        (VarSymbol* tempError,
                                       Stmt*      thenStmt);

  static void      insertErrorCond    (CallExpr*  node,
                                       ErrorCond  errorCond);

  static void      insertFallbackTry  (BlockStmt* insert,
                                       TryInfo    info);

  static void      insertFallbackBare (BlockStmt* insert,
                                       FnSymbol*  parentFn,
                                       VarSymbol* tempError);

  static void      insertHalt         (BlockStmt* insert);

  static void      insertSetAndReturn (BlockStmt* insert,
                                       FnSymbol*  parentFn,
                                       VarSymbol* tempError);
};

bool ErrorHandlingVisitor::enterTryStmt(TryStmt* node) {
  SET_LINENO(node);

  VarSymbol*   tempError = newTemp("error", dtObject);
  LabelSymbol* catches   = new LabelSymbol("catches");
  TryInfo      info      = {tempError, catches, node};
  tryStack.push(info);

  BlockStmt* tryBlock = node->body();
  tryBlock->insertAtHead(new DefExpr(tempError));
  tryBlock->insertAtTail(new DefExpr(catches));

  return true;
}

void ErrorHandlingVisitor::exitTryStmt(TryStmt* node) {
  SET_LINENO(node);

  BlockStmt* tryBlock = node->body();
  TryInfo    info     = tryStack.top();

  // TODO: create catches
  BlockStmt* catchesBlock = new BlockStmt();
  ErrorCond  errorCond    = ifErrorThen(info.tempError, catchesBlock);

  tryBlock->insertAtTail(errorCond.defErrorExists);
  tryBlock->insertAtTail(errorCond.setErrorExists);
  tryBlock->insertAtTail(errorCond.checkError);

  BlockStmt* fallbackBlock = new BlockStmt();
  catchesBlock->insertAtTail(fallbackBlock);
  insertFallbackTry(fallbackBlock, info);

  tryBlock->remove();
  node    ->replace(tryBlock);
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

        node->getStmtExpr()->insertAfter(tempError);

        insertErrorCond(node, ifErrorThen(tempError, gotoCatches));
      } else {
        VarSymbol* tempError     = newTemp("error", dtObject);
        BlockStmt* fallbackBlock = new BlockStmt();

        node->getStmtExpr()->insertBefore(new DefExpr(tempError));
        node->getStmtExpr()->insertAfter(tempError);

        insertErrorCond(node, ifErrorThen(tempError, fallbackBlock));
        insertFallbackBare(fallbackBlock, node->getFunction(), tempError);
      }
    }
  } else if (node->isPrimitive(PRIM_THROW)) {
    SET_LINENO(node);

    // throw is equivalent to a fallback
    BlockStmt* throwBlock = new BlockStmt();
    node->replace(throwBlock);

    Expr* thrownError = node->get(1)->remove();
    Expr* castError   = new CallExpr(PRIM_CAST, dtObject->symbol, thrownError);

    if (insideTry) {
      TryInfo info   = tryStack.top();
      Expr* setError = new CallExpr(PRIM_MOVE, info.tempError, castError);
      throwBlock->insertAtTail(setError);

      insertFallbackTry(throwBlock, info);
    } else {
      VarSymbol* tempError = newTemp("error", dtObject);
      Expr*      setError  = new CallExpr(PRIM_MOVE, tempError, castError);

      throwBlock->insertAtTail(new DefExpr(tempError));
      throwBlock->insertAtTail(setError);

      insertFallbackBare(throwBlock, throwBlock->getFunction(), tempError);
    }
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

void ErrorHandlingVisitor::insertErrorCond(
    CallExpr* node, ErrorCond errorCond) {

  // insert in reverse order
  node->getStmtExpr()->insertAfter(errorCond.checkError);
  node->getStmtExpr()->insertAfter(errorCond.defErrorExists);
  node->getStmtExpr()->insertAfter(errorCond.setErrorExists);
}

void ErrorHandlingVisitor::insertFallbackTry(BlockStmt* insert, TryInfo info) {
  // TODO: need callsite for SET_LINENO

  TryStmt* tryStmt = info.tryStmt;
  if (tryStmt->tryBang()) {
    insertHalt(insert);
  } else {
    FnSymbol* parentFn = tryStmt->getFunction();
    insertSetAndReturn(insert, parentFn, info.tempError);
  }
  // TODO: propagation of nested trys
  // set enclosing error and jump
}

void ErrorHandlingVisitor::insertFallbackBare(BlockStmt* insert,
    FnSymbol* parentFn, VarSymbol* tempError) {
  // TODO: need callsite for SET_LINENO

  bool halt;
  if (parentFn->throwsError()) {
    // TODO: strict mode, missing try
    // default mode, auto-propagate
    halt = false;
  } else {
    // TODO: strict mode, missing try!
    // default mode, auto-halt
    halt = true;
  }

  if (halt) {
    insertHalt(insert);
  } else {
    insertSetAndReturn(insert, parentFn, tempError);
  }
}

void ErrorHandlingVisitor::insertHalt(BlockStmt* insert) {
  Expr* haltOnError = new CallExpr(PRIM_RT_ERROR,
                        new_CStringSymbol("uncaught error"));
  insert->insertAtTail(haltOnError);
}

void ErrorHandlingVisitor::insertSetAndReturn(BlockStmt* insert,
    FnSymbol* parentFn, VarSymbol* tempError) {

  LabelSymbol* label = parentFn->getOrCreateEpilogueLabel();
  INT_ASSERT(label); // error handling needs to jump to the epilogue label

  DefExpr* outErrorExpr = toDefExpr(parentFn->formals.last());
  Symbol*  outError     = outErrorExpr->sym;

  Expr* castError  = new CallExpr(PRIM_CAST, dtObject->symbol, tempError);
  Expr* setError   = new CallExpr(PRIM_MOVE, outError, castError);
  Expr* gotoReturn = new GotoStmt(GOTO_RETURN, label);

  insert->insertAtTail(setError);
  insert->insertAtTail(gotoReturn);
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
