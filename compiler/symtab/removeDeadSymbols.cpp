#include "removeDeadSymbols.h"
#include "analysis.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/view.h"


RemoveDeadSymbols2::RemoveDeadSymbols2(void) {
  skipInternalScopes = true;
}


void RemoveDeadSymbols2::processSymbol(Symbol* sym) {
  if (!AST_is_used(sym)) {
    sym->isDead = true;

    if (sym->defPoint) {
      // BLC: This is overkill, but mirrors what removeDeadSymbols did
      sym->defPoint->stmt->extract();
    }
    fprintf(stderr, "%s is dead\n", sym->name);
  }
}
