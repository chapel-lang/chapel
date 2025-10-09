/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "pass-manager-passes.h"

#include "CallExpr.h"
#include "FnSymbol.h"
#include "astutil.h"
#include "baseAST.h"
#include "driver.h"
#include "expr.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "virtualDispatch.h"

#include <algorithm>
#include <queue>
#include <unordered_set>
#include <vector>

// ---------- InsertNilChecks ----------
//
// insert nil checks primitives in front of most member accesses
//
// Exceptions:
//   1) The Chapel specification indicates that it is acceptable to
//      invoke the destructor on NIL, so we avoid doing so when
//      handling a call to a destructor.
//

static bool isClassMethodCall(CallExpr* call) {
  FnSymbol* fn     = call->resolvedFunction();
  bool      retval = false;

  if (fn && fn->isMethod() && fn->_this) {
    if (AggregateType* ct = toAggregateType(fn->_this->typeInfo())) {
      if (fn->numFormals()             >  0 &&
          fn->getFormal(1)->typeInfo() == fn->_this->typeInfo()) {
        if (isClass(ct) || ct->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          retval = true;
        }
      }
    }
  }
  return retval;
}

bool InsertNilChecks::shouldProcess(CallExpr *call) {
  if (!(call->isPrimitive(PRIM_GET_MEMBER) ||
        call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
        call->isPrimitive(PRIM_SET_MEMBER) ||
        call->isPrimitive(PRIM_GETCID) ||
        call->isPrimitive(PRIM_TESTCID) ||
        isClassMethodCall(call))) {
    return false;
  }

  Expr* arg0 = call->get(1);
  AggregateType* ct = toAggregateType(arg0->typeInfo());

  if (ct && (isClass(ct) || ct->symbol->hasFlag(FLAG_WIDE_CLASS))) {
    // Avoid inserting a nil-check if this is a call to a destructor
    if (FnSymbol* fn = call->resolvedFunction()) {
      return !fn->hasFlag(FLAG_DESTRUCTOR);
    }

    return true;
  }

  return false;
}

void InsertNilChecks::process(CallExpr* call) {
  Expr* arg0 = call->get(1);
  Expr* stmt = call->getStmtExpr();

  SET_LINENO(stmt);

  stmt->insertBefore(new CallExpr(PRIM_CHECK_NIL, arg0->copy()));
}
