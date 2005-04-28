#include "removeDeadSymbols.h"
#include "analysis.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/view.h"


RemoveDeadSymbols::RemoveDeadSymbols(void) {
  //  whichModules = MODULES_USER;
}


static void markAsDeadAndExtract(Symbol* sym) {
  sym->isDead = true;
  if (!sym->defPoint->next && !sym->defPoint->prev) {
    sym->defPoint->parentStmt->extract();
  } else {
    sym->defPoint->extract();
  }
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
    markAsDeadAndExtract(sym);
    fprintf(stderr, "%s is dead\n", sym->name);
  }
#else
  if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(sym)) {
    if (!type_is_used(typeSym)) {
      // SJD: Don't want to remove function if type variable parameter
      // is dead
      if (dynamic_cast<FnSymbol*>(sym->defPoint->sym)) {
        return;
      }
      markAsDeadAndExtract(sym);
    }
  } else if (FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym)) {
    if (!function_is_used(fnSym)) {
      markAsDeadAndExtract(sym);
    }
  }
#endif
}
