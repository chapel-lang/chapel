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

#ifdef USE_AST_IS_USED
  if (!AST_is_used(sym)) {
    sym->isDead = true;

    if (sym->defPoint) {
      // BLC: This is overkill, but mirrors what removeDeadSymbols did
      sym->defPoint->stmt->extract();
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

      sym->defPoint->stmt->extract();
    }
  }
  if (FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym)) {
    if (!function_is_used(fnSym)) {
      fnSym->isDead = true;
      // BLC: see comment on previous conditional
      sym->defPoint->stmt->extract();
    }
  }
#endif
}
