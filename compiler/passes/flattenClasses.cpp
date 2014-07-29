#include "alist.h"
#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"


void flattenClasses(void) {
  //
  // collect nested classes
  //
  Vec<AggregateType*> nestedClasses;
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (AggregateType* ct = toAggregateType(ts->type))
      if (toAggregateType(ct->symbol->defPoint->parentSymbol->type))
        nestedClasses.add(ct);
  }

  //
  // move nested classes to module level
  //
  forv_Vec(AggregateType, ct, nestedClasses) {
    ModuleSymbol* mod = ct->getModule();
    DefExpr *def = ct->symbol->defPoint;
    def->remove();
    mod->block->insertAtTail(def);
  }
}
