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
#include <vector>

class ErrorHandlingVisitor : public AstVisitorTraverse {

public:
  ErrorHandlingVisitor      (ArgSymbol* _outFormal, LabelSymbol* _epilogue);

  virtual bool enterTryStmt (TryStmt*   node);
  virtual void exitTryStmt  (TryStmt*   node);
  virtual bool enterCallExpr(CallExpr*  node);

private:
  struct TryInfo {
    VarSymbol*   errorVar;
    LabelSymbol* handlerLabel;
    TryStmt*     tryStmt;
  };

  std::stack<TryInfo> tryStack;
  ArgSymbol*          outError;
  LabelSymbol*        epilogue;

         std::vector<Expr*> handler          (TryInfo            info);
         std::vector<Expr*> setOutGotoReturn (VarSymbol*         error);

  static CallExpr*          haltExpr         ();

  static std::vector<Expr*> errorCheck       (VarSymbol*         errorVar,
                                              BlockStmt*         thenBlock);

  static void               insertIntoBlock  (BlockStmt*         insert,
                                              std::vector<Expr*> exprs);

  static void               insertAfterCall  (CallExpr*          node,
                                              std::vector<Expr*> exprs);

  ErrorHandlingVisitor();
};

ErrorHandlingVisitor::ErrorHandlingVisitor(
    ArgSymbol* _outError, LabelSymbol* _epilogue) {

  outError = _outError;
  epilogue = _epilogue;
}

bool ErrorHandlingVisitor::enterTryStmt(TryStmt* node) {
  SET_LINENO(node);

  VarSymbol*   errorVar     = newTemp("error", dtObject);
  LabelSymbol* handlerLabel = new LabelSymbol("handler");
  TryInfo      info         = {errorVar, handlerLabel, node};
  tryStack.push(info);

  return true;
}

void ErrorHandlingVisitor::exitTryStmt(TryStmt* node) {
  SET_LINENO(node);

  TryInfo    info     = tryStack.top();
  BlockStmt* tryBlock = node->body();

  tryBlock->insertAtHead(new DefExpr(info.errorVar));
  tryBlock->insertAtTail(new DefExpr(info.handlerLabel));
  insertIntoBlock(tryBlock, handler(info));

  tryBlock->remove();
  node    ->replace(tryBlock);
  tryStack.pop();
}

bool ErrorHandlingVisitor::enterCallExpr(CallExpr* node) {
  bool insideTry = !tryStack.empty();

  if (FnSymbol* calledFn = node->resolvedFunction()) {
    if (calledFn->throwsError()) {
      SET_LINENO(node);

      VarSymbol* errorVar;
      BlockStmt* errorPolicy = new BlockStmt();

      if (insideTry) {
        TryInfo info     = tryStack.top();
                errorVar = info.errorVar;

        errorPolicy->insertAtTail(new GotoStmt(GOTO_ERROR_HANDLING,
                                               info.handlerLabel));
      } else {
        // without try, need an error variable
        errorVar = newTemp("error", dtObject);
        node->getStmtExpr()->insertBefore(new DefExpr(errorVar));

        // TODO: strict mode, missing try
        if (outError) {
          insertIntoBlock(errorPolicy, setOutGotoReturn(errorVar));

        // TODO: strict mode, missing try!
        } else {
          errorPolicy->insertAtTail(haltExpr());
        }
      }

      node->insertAtTail(errorVar);
      insertAfterCall(node, errorCheck(errorVar, errorPolicy));
    }
  } else if (node->isPrimitive(PRIM_THROW)) {
    SET_LINENO(node);

    BlockStmt* throwBlock = new BlockStmt();
    node->replace(throwBlock);

    SymExpr*   thrownExpr  = toSymExpr(node->get(1)->remove());
    VarSymbol* thrownError = toVarSymbol(thrownExpr->symbol());

    if (insideTry) {
      TryInfo   info      = tryStack.top();
      CallExpr* castError = new CallExpr(PRIM_CAST, dtObject->symbol,
                                         thrownError);
      throwBlock->insertAtTail(new CallExpr(PRIM_MOVE, info.errorVar,
                                            castError));
      throwBlock->insertAtTail(new GotoStmt(GOTO_ERROR_HANDLING,
                                            info.handlerLabel));
    } else if (outError) {
      insertIntoBlock(throwBlock, setOutGotoReturn(thrownError));
    } else {
      // TODO: compiler error, cannot throw unless callingFn throws
    }
  }
  return true;
}

std::vector<Expr*> ErrorHandlingVisitor::handler(TryInfo info) {
  BlockStmt* handler = new BlockStmt();

  // TODO: create catches
  // TODO: nested try

  if (info.tryStmt->tryBang()) {
    handler->insertAtTail(haltExpr());
  } else if (outError) {
    insertIntoBlock(handler, setOutGotoReturn(info.errorVar));
  } else {
    // TODO: inexhaustive try in non-throwing fn
  }

  return errorCheck(info.errorVar, handler);
}

std::vector<Expr*> ErrorHandlingVisitor::setOutGotoReturn(VarSymbol* error) {
  CallExpr* castError = new CallExpr(PRIM_CAST, dtObject->symbol, error);

  std::vector<Expr*> ret;
  ret.push_back(new CallExpr(PRIM_MOVE, outError, castError));
  ret.push_back(new GotoStmt(GOTO_RETURN, epilogue));

  return ret;
}

// TODO: take in a halt message from the error
CallExpr* ErrorHandlingVisitor::haltExpr() {
  return new CallExpr(PRIM_RT_ERROR, new_CStringSymbol("uncaught error"));
}

std::vector<Expr*> ErrorHandlingVisitor::errorCheck(
    VarSymbol* errorVar, BlockStmt* thenBlock) {

  VarSymbol* errorExistsVar = newTemp("errorExists", dtBool);
  CallExpr*  errorExists    = new CallExpr(PRIM_NOTEQUAL, errorVar, gNil);

  std::vector<Expr*> ret;
  ret.push_back(new DefExpr(errorExistsVar));
  ret.push_back(new CallExpr(PRIM_MOVE, errorExistsVar, errorExists));
  ret.push_back(new CondStmt(new SymExpr(errorExistsVar), thenBlock));

  return ret;
}

void ErrorHandlingVisitor::insertIntoBlock(BlockStmt* insert,
    std::vector<Expr*> exprs) {
  for (std::vector<Expr*>::iterator it = exprs.begin();
       it != exprs.end(); ++it) {
    insert->insertAtTail(*it);
  }
}

void ErrorHandlingVisitor::insertAfterCall(CallExpr* node,
    std::vector<Expr*> exprs) {

  Expr* prev = node;
  for (std::vector<Expr*>::iterator it = exprs.begin();
       it != exprs.end(); ++it) {

    Expr* curr = *it;
    prev->getStmtExpr()->insertAfter(curr);
    prev = curr;
  }
}

// TODO: dtObject should be dtError, but we don't have that right now
void lowerErrorHandling() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    ArgSymbol*   outError = NULL;
    LabelSymbol* epilogue = NULL;

    if (fn->throwsError()) {
      SET_LINENO(fn);

      outError = new ArgSymbol(INTENT_REF, "error_out", dtObject);
      fn->insertFormalAtTail(outError);

      epilogue = fn->getOrCreateEpilogueLabel();
      INT_ASSERT(epilogue); // throws requires an epilogue
    }

    ErrorHandlingVisitor visitor = ErrorHandlingVisitor(outError, epilogue);
    fn->accept(&visitor);
  }
}
