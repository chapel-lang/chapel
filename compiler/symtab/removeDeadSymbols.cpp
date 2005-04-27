#include "removeDeadSymbols.h"
#include "analysis.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/view.h"


RemoveDeadSymbols::RemoveDeadSymbols(void) {
  //  whichModules = MODULES_USER;
}


void RemoveDeadSymbols::processSymbol(Symbol* sym) {
  if (sym->parentScope->type == SCOPE_INTRINSIC) {
    return;
  }
  if (sym->keepLive) {
    return;
  }

#ifdef USE_AST_IS_USED
  if (!AST_is_used(sym)) {
    sym->isDead = true;

    if (sym->defPoint) {
      // BLC: This is overkill, but mirrors what removeDeadSymbols did
      sym->defPoint->parentStmt->extract();
    }
    fprintf(stderr, "%s is dead\n", sym->name);
  }
#else
  if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(sym)) {
    if (!type_is_used(typeSym)) {
      typeSym->isDead = true;
      
      // BLC: this is a bad assumption: that if one DefExpr is dead
      // then the whole statement can be extracted; instead, we
      // should pull just the defExpr (but this breaks other stuff
      // that assumes that every defStmt).
      if (sym->defPoint) { // JBP 4/5/05 for intents-classes4.chpl this is NULL

        // SJD: Don't want to remove function is parameter is dead
        if (dynamic_cast<FnSymbol*>(sym->defPoint->sym)) {
          return;
        }

        sym->defPoint->parentStmt->extract();
      }
    }
  }
  if (FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym)) {
    if (!function_is_used(fnSym)) {
      fnSym->isDead = true;
      // BLC: see comment on previous conditional
      sym->defPoint->parentStmt->extract();
    }
  }
#endif
}
