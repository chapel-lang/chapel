#include <cstdio>
#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"

// This pass pulls out any global constants to the top of the given
// function (with the motivation to avoid loads during loops).

// Note that this work cannot be done within an init function s.t. the
// global constant still retains its value (otherwise, the init function
// would set a local copy of a given value, and other functions wouldn't
// see the intended value.

void localizeGlobals() {
  if (fNoGlobalConstOpt) return;
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    Map<Symbol*,VarSymbol*> globals;
    Vec<BaseAST*> asts;
    collect_asts(fn->body, asts);
    forv_Vec(BaseAST, ast, asts) {
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
