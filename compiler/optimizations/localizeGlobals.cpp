/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

#include "FnSymbol.h"
#include "PassManager.h"
#include "baseAST.h"
#include "passes.h"

#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"


#include <cstdio>

// ---------- LocalizeGlobals ----------
// This pass pulls out any global constants to the top of the given
// function (with the motivation to avoid loads during loops).

// Note that this work cannot be done within an init function s.t. the
// global constant still retains its value (otherwise, the init function
// would set a local copy of a given value, and other functions wouldn't
// see the intended value.

void LocalizeGlobals::process(FnSymbol* fn) {
  // REFACTOR Q -- keeping these around is nice for re-use, but maybe we should
  // have a standard method like ::cleanup/teardown
  symExprs.clear();
  globals.clear();

  // TODO TRAVERSE
  collectSymExprs(fn->body, symExprs);
  for (SymExpr* se : symExprs) {
    Symbol* var = se->symbol();
    ModuleSymbol* parentmod = toModuleSymbol(var->defPoint->parentSymbol);
    CallExpr* parentExpr = toCallExpr(se->parentExpr);
    bool inAddrOf = parentExpr && (parentExpr->isPrimitive(PRIM_ADDR_OF) ||
                                   parentExpr->isPrimitive(PRIM_SET_REFERENCE));
    bool lhsOfMove =
        parentExpr && isMoveOrAssign(parentExpr) && (parentExpr->get(1) == se);

    // Is var a global constant?
    // Don't replace the var name in its init function since that's
    //      where we're setting the value. Similarly, dont replace them
    //      inside initStringLiterals
    // If the parentSymbol is the rootModule, the var is 'void,'
    //      'false,' '0,' ...
    // Also don't replace it when it's in an addr of primitive.
    if (parentmod &&
        fn != parentmod->initFn &&
        fn != initStringLiterals &&
        !inAddrOf &&
        !lhsOfMove &&
        var->hasFlag(FLAG_CONST) &&
        !var->hasFlag(FLAG_EXTERN) &&
        var->defPoint->parentSymbol != rootModule &&
        !var->type->symbol->hasFlag(FLAG_TUPLE)) {
      VarSymbol* local_global = globals.get(var);
      SET_LINENO(se); // Set the se line number for output
      if (!local_global) {
        const char* newname = astr("local_", var->cname);
        local_global = newTemp(newname, var->qualType());
        fn->insertAtHead(new CallExpr(PRIM_MOVE, local_global, var));
        fn->insertAtHead(new DefExpr(local_global));

        // Copy string immediates to localized strings so that
        // we can show the string value in comments next to uses.
        if (!fLlvmCodegen)
          if (VarSymbol* localVarSym = toVarSymbol(var))
            if (Immediate* immediate = localVarSym->immediate)
              if (immediate->const_kind == CONST_KIND_STRING)
                local_global->immediate = new Immediate(immediate);

        globals.put(var, local_global);
      }
      se->replace(new SymExpr(toSymbol(local_global)));
    }
  }
}

#include "global-ast-vecs.h"

void localizeGlobals() {
  PassManager pm;
  if (fNoGlobalConstOpt) return;
  pm.runPass<FnSymbol*>(LocalizeGlobals(), gFnSymbols);
}
