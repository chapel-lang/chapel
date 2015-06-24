/*
 * Copyright 2004-2015 Cray Inc.
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

// checkNormalized.cpp

#include "passes.h"

#include "expr.h"
#include "astutil.h"
#include "stlUtil.h"


static void checkFunctionSignatures();
static void checkPrimNew();


void
checkNormalized(void) {
  checkFunctionSignatures();
  checkPrimNew();
}


static void checkFunctionSignatures()
{
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->isIterator()) {
      if (fn->retTag == RET_TYPE)
        USR_FATAL_CONT(fn, "iterators may not yield or return types");
      if (fn->retTag == RET_PARAM)
        USR_FATAL_CONT(fn, "iterators may not yield or return parameters");
    } else if (fn->hasFlag(FLAG_CONSTRUCTOR) &&
               !fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)) {
      if (fn->retExprType)
        USR_FATAL_CONT(fn, "constructors may not declare a return type");
      for_formals(formal, fn) {
        std::vector<SymExpr*> symExprs;
        collectSymExprs(formal, symExprs);
        for_vector(SymExpr, se, symExprs) {
          if (se->var == fn->_this) {
            USR_FATAL_CONT(se, "invalid access of class member in constructor header");
            break;
          }
        }
      }
    } else if (fn->hasFlag(FLAG_DESTRUCTOR)) {
      if (fn->retExprType)
        USR_FATAL_CONT(fn, "destructors may not declare a return type");
    }
  }
}


static void checkPrimNew()
{
  forv_Vec(CallExpr, call, gCallExprs)
  {
    // Ignore calls that are not in the tree.
    if (!call->parentSymbol)
      continue;

    // We are only interested in 'new' primitives.
    if (!call->isPrimitive(PRIM_NEW))
      continue;

    // The operand of a new should be a contructor call.
    if (CallExpr* ctorCall = toCallExpr(call->get(1)))
    {
      if (isUnresolvedSymExpr(ctorCall->baseExpr))
        // We can't know anything more about this symbol until resolution.
        // So let it pass
        continue;

      if (isTypeExpr(ctorCall))
        // If we know the expression represents a type, that's also good.
        continue;

      if (ctorCall->baseExpr && isTypeExpr(ctorCall->baseExpr))
        // This is of the form <type-expr>(<args>)
        // That is, it looks like a constructor.
        continue;

      // Fail by default
      // (We may need additional filters above, to pass expected cases.)
      USR_FATAL_CONT(call, "'new' must be followed by a type expression");
    }
    else
    {
      // 'new' must always have an operand.
      USR_FATAL_CONT(call, "invalid use of 'new'");
    }
  }
  USR_STOP();
}
