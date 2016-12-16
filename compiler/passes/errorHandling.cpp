/*
 * Copyright 2004-2016 Cray Inc.
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

#include "stmt.h"
#include "symbol.h"
#include "view.h"
#include <cstdio>

  // change 'throws' to 'out'
    // add an error_out formal to a function marked 'throws'
    // for all calls to such functions
      // create and pass a variable error to receive error_out
      // check error after the call
  // change 'throw' to set error_out

  // TODO: how do we find Error temps from a call to a throwing function?

// TODO: dtObject should be dtError, but we don't have that right now
void lowerErrorHandling(void) {
  printf("hello world\n");

  // give 'throws' an 'out'
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->throwsError()) {
      SET_LINENO(fn);

      // add an Error formal to a function marked 'throws'
      ArgSymbol* outFormal = new ArgSymbol(INTENT_REF, "error_out", dtObject);
      fn->insertFormalAtTail(outFormal);

      for_SymbolSymExprs(se, fn) {
        if (CallExpr* call = toCallExpr(se->parentExpr)) {
          if (fn == call->resolvedFunction()) {
            // create a variable to receive the Error arg
            VarSymbol* tempError= newTemp("error", dtObject);

            // have the call pass the variable
            call->insertAtTail(tempError);


            // check the Error arg after the call
            // TODO: do we need this temp?
            VarSymbol* tempErrorExists = newTemp("errorExists", dtBool);
            DefExpr* defErrorExists = new DefExpr(tempErrorExists);

            Expr* setErrorExists = new CallExpr(
              PRIM_MOVE,
              tempErrorExists,
              new CallExpr(PRIM_NOTEQUAL, tempError, gNil));

            // TODO: better error message
            Expr* haltOnError = new CallExpr(PRIM_RT_ERROR,
              new_CStringSymbol("uncaught error"));
            Expr* checkError = new CondStmt(new SymExpr(tempErrorExists), haltOnError);

            // (works for calls inside calls)
            call->getStmtExpr()->insertBefore(new DefExpr(tempError));
            call->getStmtExpr()->insertAfter(defErrorExists);
            defErrorExists->getStmtExpr()->insertAfter(setErrorExists);
            setErrorExists->getStmtExpr()->insertAfter(checkError);
          }
        }
      }

      // change 'throw' to set error_out
      // TODO: need "error" for SET_LINENO
      for_exprs_postorder(expr, fn->body) {
        if (CallExpr* call = toCallExpr(expr)) {
          if (call->isPrimitive(PRIM_THROW)) {
            SET_LINENO(call);

            Expr* error = call->get(1)->remove();
            Expr* castError = new CallExpr(PRIM_CAST, dtObject->symbol, error);
            Expr* setError = new CallExpr(PRIM_MOVE, outFormal, castError);
            call->replace(setError);
          }
        }
      }
    }
  }
}
