#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "iterator.h"

void lowerIterators() {
  if (!unoptimized) {
    forv_Vec(FnSymbol, fn, gFns) {
      if (fn->fnClass == FN_ITERATOR) {
        compressUnnecessaryScopes(fn);
        removeUnnecessaryGotos(fn);
        removeUnusedLabels(fn);
        localCopyPropagation(fn);
        deadVariableElimination(fn);
        deadExpressionElimination(fn);
      }
    }
  }
  compute_sym_uses();
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->fnClass == FN_ITERATOR) {
      lowerIterator(fn);
    }
  }
}
