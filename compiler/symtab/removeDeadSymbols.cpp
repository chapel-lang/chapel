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

    BaseAST* defPoint = sym->defPoint;
    DefExpr* defExpr = dynamic_cast<DefExpr*>(defPoint);
    if (defExpr) {
      // BLC: This is overkill, but mirrors what removeDeadSymbols did
      defExpr->stmt->extract();
    }
    fprintf(stderr, "%s is dead\n", sym->name);
  }
}
