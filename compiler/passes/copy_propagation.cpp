#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

void copyPropagation(void) {
  forv_Vec(FnSymbol, fn, gFns) {
    collapseBlocks(fn->body);
    removeUnnecessaryGotos(fn);
  }
  if (unoptimized)
    return;
  forv_Vec(FnSymbol, fn, gFns) {
    //    if (!fDisableReferencePropagation)
      localReferencePropagation(fn);
    if (!fDisableCopyPropagation)
      localCopyPropagation(fn);
    deadVariableElimination(fn);
    deadExpressionElimination(fn);
    if (!fDisableCopyPropagation) {
      globalCopyPropagation(fn);
      deadVariableElimination(fn);
      deadExpressionElimination(fn);
    }
  }
}
