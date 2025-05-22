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

#include "optimizations.h"

#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"

// We can remove the calls to chpl__initCopy (should actually be chpl__autoCopy)
// and corresponding calls to chpl__autoDestroy for Plain-Old-Data (POD) types.
// See isPOD() and propagateNotPOD().

//
// remove pointless initCopy calls, e.g., initCopy calls on records of
// primitive types
//

bool RemoveUnnecessaryAutoCopyCalls::shouldProcess(FnSymbol* fn) {
  if (fn->hasEitherFlag(FLAG_INIT_COPY_FN, FLAG_AUTO_COPY_FN)) {
    if (fn->retType == dtNothing)
      // initCopy(none) and autoCopy(none) will have had their nothing
      // argument removed
      return false;

    // We expect both initCopy and autoCopy functions to have two arguments
    // second argument: whose type is the same as the return type
    // definedConst: whether the LHS was defined const
    INT_ASSERT(fn->numFormals() >= 2);

    if (fn->getFormal(1)->type != fn->retType)
      // In some cases, the autoCopy function has a different return type than
      // its argument type.
      // In that case, the replace() below won't work because it will cause a
      // type mismatch at the call sites, so we just punt.
      return false;

    return isPOD(fn->retType);
  }

  return false;
}

// TODO this uses removeInitOrAutoCopyPostResolution
void RemoveUnnecessaryAutoCopyCalls::process(FnSymbol* fn)
{
  calls.clear();
  assert(isPOD(fn->retType)); // precondition from shouldProcess

  // Gather calls of this function
  for_SymbolSymExprs(se, fn) { calls.push_back(toCallExpr(se->parentExpr)); }

  // Remove those calls we gathered
  for (CallExpr* call : calls) {
    Expr* actual = call->get(1);
    ArgSymbol* arg = actual_to_formal(actual);
    if (arg) {
      arg->removeFlag(FLAG_INSERT_AUTO_DESTROY);
      arg->removeFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW);
    } else
      INT_FATAL(arg, "Expected autoCopy argument to be a SymExpr.");

    // Bridge out the autoCopy call.

    Type* lhsType = NULL;
    Type* rhsType = actual->typeInfo();

    CallExpr* callParent = toCallExpr(call->parentExpr);

    // When processing 'in' intent variables passed to foreach loops, we
    // created a task independent captured version of the variable. The
    // exact way to do that depends on whether the loop is ultimately
    // cpu or gpu bound so at this point of compilation we represent
    // this using an assignment to PRIM_TASK_PRIVATE_SVAR_CAPTURE.
    // In cases where the 'in' intent'ed variable is an object we
    // may have AST like this:
    //
    //  task_ind_x = PRIM_TASK_PRIVATE_SVAR_CAPTURE(chpl__initCopy(x))
    //
    // To proceed, we basically ignore the primitive on the RHS of the
    // assignment and process as if it were written
    // `task_ind_x = chpl_initCopy(x)`.
    if (callParent->isPrimitive(PRIM_TASK_PRIVATE_SVAR_CAPTURE)) {
      callParent = toCallExpr(callParent->parentExpr);
    }
    INT_ASSERT(isMoveOrAssign(callParent));
    lhsType = callParent->get(1)->typeInfo();

    SET_LINENO(call);

    if (lhsType->getValType() != rhsType->getValType()) {
      INT_FATAL(actual, "Type mismatch in updateAutoCopy");
    } else {
      removeInitOrAutoCopyPostResolution(call);
    }
  }
}

#include "global-ast-vecs.h"

void removeUnnecessaryAutoCopyCalls() {
  if (fNoRemoveCopyCalls)
    return;

  PassManager pm;
  pm.runPass<FnSymbol*>(RemoveUnnecessaryAutoCopyCalls(), gFnSymbols);
}
