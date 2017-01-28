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
  virtual bool enterTryStmt (TryStmt*  node);
  virtual void exitTryStmt  (TryStmt*  node);
  virtual bool enterCallExpr(CallExpr* node);

private:
  struct TryInfo {
    VarSymbol*   tempError;
    LabelSymbol* catches;
    TryStmt*     tryStmt;
  };

  std::stack<TryInfo> tryStack;

  struct ErrorCond {
    DefExpr*  defErrorExists;
    CallExpr* setErrorExists;
    CondStmt* checkError;
  };

  struct Fallback {
    bool       halt;
    FnSymbol*  parentFn;
    VarSymbol* tempError;
  };

  static ErrorCond ifErrorThen     (VarSymbol* tempError,
                                    Stmt*      thenStmt);

  static void      insertErrorCond (CallExpr*  insert,
                                    ErrorCond  errorCond);

  static Fallback  fallbackTry     (TryInfo    info);

  static Fallback  fallbackBare    (FnSymbol*  parentFn,
                                    VarSymbol* tempError);

  static void      insertFallback  (BlockStmt* insert,
                                    Fallback   fallback);
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

  insertFallback(fallbackBlock, fallbackTry(info));

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

        node->insertAtTail(tempError);

        insertErrorCond(node, ifErrorThen(tempError, gotoCatches));
      } else {
        VarSymbol* tempError     = newTemp("error", dtObject);
        BlockStmt* fallbackBlock = new BlockStmt();

        node->getStmtExpr()->insertBefore(new DefExpr(tempError));
        node->insertAtTail(tempError);

        insertErrorCond(node, ifErrorThen(tempError, fallbackBlock));
        insertFallback(fallbackBlock,
                       fallbackBare(node->getFunction(), tempError));
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

      insertFallback(throwBlock, fallbackTry(info));
    } else {
      VarSymbol* tempError = newTemp("error", dtObject);
      Expr*      setError  = new CallExpr(PRIM_MOVE, tempError, castError);

      throwBlock->insertAtTail(new DefExpr(tempError));
      throwBlock->insertAtTail(setError);

      insertFallback(throwBlock,
                     fallbackBare(throwBlock->getFunction(), tempError));
    }
  }
  return true;
}

ErrorHandlingVisitor::ErrorCond ErrorHandlingVisitor::ifErrorThen(
    VarSymbol* tempError, Stmt* thenStmt) {

  CallExpr*  errorExists     = new CallExpr(PRIM_NOTEQUAL, tempError, gNil);
  VarSymbol* tempErrorExists = newTemp("errorExists", dtBool);

  DefExpr*   defErrorExists  = new DefExpr(tempErrorExists);
  CallExpr*  setErrorExists  = new CallExpr(PRIM_MOVE, tempErrorExists,
                                            errorExists);
  CondStmt*  checkError      = new CondStmt(new SymExpr(tempErrorExists),
                                            thenStmt);

  ErrorCond ret = {
    .defErrorExists = defErrorExists,
    .setErrorExists = setErrorExists,
    .checkError     = checkError
  };

  return ret;
}

void ErrorHandlingVisitor::insertErrorCond(
    CallExpr* node, ErrorCond errorCond) {

  // insert in reverse order
  Expr* insert = node->getStmtExpr();
  insert->insertAfter(errorCond.checkError);
  insert->insertAfter(errorCond.defErrorExists);
  insert->insertAfter(errorCond.setErrorExists);
}

ErrorHandlingVisitor::Fallback ErrorHandlingVisitor::fallbackTry(TryInfo info) {
  // TODO: need callsite for SET_LINENO
  Fallback ret;
  if (info.tryStmt->tryBang()) {
    ret.halt      = true;
    ret.parentFn  = NULL;
    ret.tempError = NULL;
  } else {
    ret.halt      = false;
    ret.parentFn  = info.tryStmt->getFunction();
    ret.tempError = info.tempError;
  }

  // TODO: propagation of nested trys
  // set enclosing error and jump

  return ret;
}

ErrorHandlingVisitor::Fallback ErrorHandlingVisitor::fallbackBare(
    FnSymbol* parentFn, VarSymbol* tempError) {

  // TODO: need callsite for SET_LINENO
  Fallback ret;
  if (parentFn->throwsError()) {
    // TODO: strict mode, missing try
    // default mode, auto-propagate
    ret.halt      = false;
    ret.parentFn  = parentFn;
    ret.tempError = tempError;
  } else {
    // TODO: strict mode, missing try!
    // default mode, auto-halt
    ret.halt      = true;
    ret.parentFn  = NULL;
    ret.tempError = NULL;
  }
  return ret;
}

void ErrorHandlingVisitor::insertFallback(
    BlockStmt* insert, Fallback fallback) {

  if (fallback.halt) {
    Expr* haltOnError = new CallExpr(PRIM_RT_ERROR,
                                     new_CStringSymbol("uncaught error"));
    insert->insertAtTail(haltOnError);
  } else {
    LabelSymbol* label = fallback.parentFn->getOrCreateEpilogueLabel();
    INT_ASSERT(label); // error handling needs to jump to the epilogue label

    DefExpr* outErrorExpr = toDefExpr(fallback.parentFn->formals.last());
    Symbol*  outError     = outErrorExpr->sym;

    Expr* castError  = new CallExpr(PRIM_CAST, dtObject->symbol,
                                    fallback.tempError);
    Expr* setError   = new CallExpr(PRIM_MOVE, outError, castError);
    Expr* gotoReturn = new GotoStmt(GOTO_RETURN, label);

    insert->insertAtTail(setError);
    insert->insertAtTail(gotoReturn);
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
