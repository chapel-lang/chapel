/*
<<<<<<< 8cf5ee5b91e8ea7b5bcdaca97a69947939eb39a6
 * Copyright 2004-2017 Cray Inc.
=======
 * Copyright 2004-2016 Cray Inc.
>>>>>>> error handling pass skeleton
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

// for each function that 'throws':
  // add an error_out formal
  // for each call to one of these functions:
    // create and pass a variable to receive error_out
    // check error after the call
      // halt() if needed
  // for each 'throw' in the function:
    // replace it with setting error_out

// TODO: dtObject should be dtError, but we don't have that right now
// TODO: how do we find Error temps from a call to a throwing function?
void lowerErrorHandling() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->throwsError()) {
      SET_LINENO(fn);

      ArgSymbol* outFormal = new ArgSymbol(INTENT_REF, "error_out", dtObject);
      fn->insertFormalAtTail(outFormal);

      for_SymbolSymExprs(se, fn) {
        if (CallExpr* call = toCallExpr(se->parentExpr)) {
          if (fn == call->resolvedFunction()) {
            VarSymbol* tempError   = newTemp("error", dtObject);
            CallExpr*  errorExists = new CallExpr(PRIM_NOTEQUAL,
                                       tempError, gNil);

            VarSymbol* tempErrorExists = newTemp("errorExists", dtBool);
            DefExpr*   defErrorExists  = new DefExpr(tempErrorExists);
            Expr*      setErrorExists  = new CallExpr(PRIM_MOVE,
                                           tempErrorExists, errorExists);

            // TODO: better error message
            Expr* haltOnError = new CallExpr(PRIM_RT_ERROR,
                                  new_CStringSymbol("uncaught error"));
            Expr* checkError  = new CondStmt(new SymExpr(tempErrorExists),
                                  haltOnError);

            call          ->insertAtTail(tempError);
            call          ->getStmtExpr()->insertBefore(new DefExpr(tempError));
            call          ->getStmtExpr()->insertAfter(defErrorExists);
            defErrorExists->getStmtExpr()->insertAfter(setErrorExists);
            setErrorExists->getStmtExpr()->insertAfter(checkError);
          }
        }
      }

      for_exprs_postorder(expr, fn->body) {
        if (CallExpr* call = toCallExpr(expr)) {
          if (call->isPrimitive(PRIM_THROW)) {
            // TODO: need callsite for SET_LINENO

            Expr* error     = call->get(1)->remove();
            Expr* castError = new CallExpr(PRIM_CAST, dtObject->symbol, error);
            Expr* setError  = new CallExpr(PRIM_MOVE, outFormal, castError);

            call->replace(setError);
          }
        }
      }
    }
  }
}
