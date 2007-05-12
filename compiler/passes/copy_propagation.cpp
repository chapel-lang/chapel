#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

void copyPropagation(void) {
  if (unoptimized)
    return;
  Vec<FnSymbol*> fns;
  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    collapseBlocks(fn);
    removeUnnecessaryGotos(fn);
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
