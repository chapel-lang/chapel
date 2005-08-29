#include "removeDeadSymbols.h"
#include "analysis.h"
#include "symtab.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/view.h"


static void markAsDeadAndExtract(Symbol* sym) {
  if (sym->defPoint) {
    sym->defPoint->parentStmt->remove();
  } else {
    sym->parentScope->undefine(sym);
  }
}


void RemoveDeadSymbols::processSymbol(Symbol* sym) {
  if (sym->parentScope->type == SCOPE_INTRINSIC) {
    return;
  }
  if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(sym)) {
    if (!type_is_used(typeSym)) {
      markAsDeadAndExtract(sym);
    }
  } else if (FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym)) {
    if (!function_is_used(fnSym)) {
      markAsDeadAndExtract(sym);
    }
  }
}
