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
  if (fBaseline)
    return;
  forv_Vec(FnSymbol, fn, gFns) {
    //    if (!fNoReferencePropagation)
    localReferencePropagation(fn);
    if (!fNoCopyPropagation)
      localCopyPropagation(fn);
    deadVariableElimination(fn);
    deadExpressionElimination(fn);
    if (!fNoCopyPropagation && !fNoFlowAnalysis) {
      globalCopyPropagation(fn);
      deadVariableElimination(fn);
      deadExpressionElimination(fn);
    }
  }
}
