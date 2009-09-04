#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"

//
// Remove autoCopy (and matching autoDestroy calls) that are
// unnecessary within a function
//
void removeUnnecessaryAutoCopyCalls(FnSymbol* fn) {
  buildBasicBlocks(fn);
  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    Map<Symbol*,Symbol*> equivalenceMap;
    Map<Symbol*,CallExpr*> autoCopyMap;
    forv_Vec(Expr, expr, bb->exprs) {
      if (CallExpr* call = toCallExpr(expr)) {
        if (call->isPrimitive(PRIM_MOVE)) {
          SymExpr* lhs = toSymExpr(call->get(1));
          INT_ASSERT(lhs);
          if (SymExpr* rhs = toSymExpr(call->get(2))) {
            if (Symbol* equiv = equivalenceMap.get(rhs->var))
              equivalenceMap.put(equiv, NULL);
            equivalenceMap.put(rhs->var, lhs->var);
            equivalenceMap.put(lhs->var, rhs->var);
            continue;
          } else if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (FnSymbol* ac = rhs->isResolved()) {
              if (ac->hasFlag(FLAG_REMOVABLE_AUTO_COPY) && rhs->argList.head) {
                SymExpr* se = toSymExpr(rhs->get(1));
                INT_ASSERT(se);
                autoCopyMap.put(se->var, rhs);
                continue;
              }
            }
          }
        } else if (FnSymbol* ad = call->isResolved()) {
          if (ad->hasFlag(FLAG_REMOVABLE_AUTO_DESTROY) && call->argList.head) {
            SymExpr* se = toSymExpr(call->get(1));
            INT_ASSERT(se);
            if (se->var->defPoint->parentSymbol == fn) {
              if (CallExpr* autoCopyCall = autoCopyMap.get(se->var)) {
                autoCopyCall->replace(autoCopyCall->get(1)->remove());
                call->replace(call->get(1)->remove());
                continue;
              } else if (Symbol* equiv = equivalenceMap.get(se->var)) {
                if (CallExpr* autoCopyCall = autoCopyMap.get(equiv)) {
                  autoCopyCall->replace(autoCopyCall->get(1)->remove());
                  call->replace(call->get(1)->remove());
                  continue;
                }
              }
            }
          }
        }
      }

      //
      // invalidate maps
      //
      Vec<SymExpr*> symExprs;
      collectSymExprs(expr, symExprs);
      forv_Vec(SymExpr, se, symExprs) {
        if (autoCopyMap.get(se->var))
          autoCopyMap.put(se->var, NULL);
        if (Symbol* equiv = equivalenceMap.get(se->var)) {
          equivalenceMap.put(equiv, NULL);
          equivalenceMap.put(se->var, NULL);
        }
      }
    }
  }
}

void removeUnnecessaryAutoCopyCalls() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    removeUnnecessaryAutoCopyCalls(fn);
  }
}
