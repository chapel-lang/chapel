#include "astutil.h"
#include "bb.h"
#include "passes.h"
#include "symbol.h"

void copy_propagation(void) {
  Vec<FnSymbol*> fns;
  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    buildBasicBlocks(fn);
  }
}
