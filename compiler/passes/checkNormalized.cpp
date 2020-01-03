/*
 * Copyright 2004-2020 Cray Inc.
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

#include "passes.h"

#include "astutil.h"
#include "expr.h"
#include "initializerRules.h"
#include "stlUtil.h"


static void checkFunctionSignatures();
static void checkPrimNew();


void checkNormalized() {
  checkFunctionSignatures();
  checkPrimNew();
}


static void checkFunctionSignatures() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->isIterator()) {
      if (fn->retTag == RET_TYPE) {
        USR_FATAL_CONT(fn, "iterators may not yield or return types");
      }

      if (fn->retTag == RET_PARAM) {
        USR_FATAL_CONT(fn, "iterators may not yield or return parameters");
      }

    } else if (fn->hasFlag(FLAG_DESTRUCTOR) == true) {
      if (fn->retExprType != NULL) {
        USR_FATAL_CONT(fn, "destructors may not declare a return type");
      }
    }
  }
}

static void checkPrimNew() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->inTree() &&  call->isPrimitive(PRIM_NEW)) {

      if (call->numActuals() >= 1) {
        Expr*    arg1     = call->get(1);
        SymExpr* se       = toSymExpr(arg1);
        Expr*    typeExpr = NULL;

        // Extract the type expression
        if (se != NULL && se->symbol() == gModuleToken) {
          typeExpr = call->get(3);
        } else {
          typeExpr = call->get(1);
        }

        if        (isUnresolvedSymExpr(typeExpr) == true) {

        } else if (isTypeExpr(typeExpr)          == true) {

        } else if (isCallExpr(typeExpr)          == true) {

        } else if (SymExpr* se = toSymExpr(typeExpr)) {
          if (se->symbol()->hasFlag(FLAG_MAYBE_TYPE) == false) {
            USR_FATAL_CONT(call,
                           "'new' must be followed by a type expression");
          }

        } else {
          USR_FATAL_CONT(call, "'new' must be followed by a type expression");
        }

      } else {
        USR_FATAL_CONT(call, "invalid use of 'new'");
      }
    }
  }

  USR_STOP();
}
