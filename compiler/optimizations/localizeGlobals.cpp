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

#include "passes.h"

#include "astutil.h"
#include "stlUtil.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"

#include <cstdio>

// This pass pulls out any global constants to the top of the given
// function (with the motivation to avoid loads during loops).

// Note that this work cannot be done within an init function s.t. the
// global constant still retains its value (otherwise, the init function
// would set a local copy of a given value, and other functions wouldn't
// see the intended value.

void localizeGlobals() {
  // This pass is not implemented correctly, so disabled for now.  
  // Generally, it is safe to pass a global (module-level) variable by
  // reference, because its lifetime equals that of the program.  But this
  // localization makes a copy whose lifetime is only as long as the containing
  // routine.
  // With code such as:
  //  proc foo() : ref string {
  //    return "my_literal_string";
  //  }
  // we are in trouble, because the (localized) object being returned is
  // deleted before it is returned.
  // This would be true for any localized global, but is especially a problem
  // for strings, because all string literals are stored as globals.
  //
  // I think the reason for this optimization in the first place is to reduce
  // communication costs.  In which case, remote value forwarding or
  // replicating globals on each locale is a better approach.  Since the
  // globals being copied here are const, caching them (as locale-private data)
  // on each network node will work fine and the potential for references to
  // freed memory.
  // 
  // Under the new AMM, many arguments -- including those passed into routines
  // that create iterator records -- are passed by reference.  To avoid
  // creating an iterator that points to freed (stack() memory, disabling this
  // optimization is necessary.  The test case of interest is
  // beer-promoted-infer-explicit.  As of this writing there were other issues,
  // but once it works, re-enabling this optimization in its current form
  // should expose the error readily.
  if (fNoGlobalConstOpt) return;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    Map<Symbol*,VarSymbol*> globals;
    std::vector<BaseAST*> asts;
    collect_asts(fn->body, asts);
    for_vector(BaseAST, ast, asts) {
      if (SymExpr* se = toSymExpr(ast)) {
        Symbol* var = se->var;
        ModuleSymbol* parentmod = toModuleSymbol(var->defPoint->parentSymbol);

        // Is var a global constant?
        // Don't replace the var name in its init function since that's
        //      where we're setting the value.
        // If the parentSymbol is the rootModule, the var is 'void,'
        //      'false,' '0,' ...
        if (parentmod &&
            fn != parentmod->initFn &&
            var->hasFlag(FLAG_CONST) &&
            !var->type->symbol->hasFlag(FLAG_NOT_POD) &&
            var->defPoint->parentSymbol != rootModule) {
          VarSymbol* local_global = globals.get(var);
          SET_LINENO(se); // Set the se line number for output
          if (!local_global) {
            const char * newname = astr("local_", var->cname);
            local_global = newTemp(newname, var->type);
            fn->insertAtHead(new CallExpr(PRIM_MOVE, local_global, var));
            fn->insertAtHead(new DefExpr(local_global));

            globals.put(var, local_global);
          }
          se->replace(new SymExpr(toSymbol(local_global)));
        }
      }
    }
  }
}
