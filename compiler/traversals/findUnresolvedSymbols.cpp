#include <typeinfo>
#include "expr.h"
#include "findUnresolvedSymbols.h"


void FindUnresolvedSymbols::preProcessSymbol(Symbol* sym) {
  if (typeid(*sym) == typeid(UnresolvedSymbol)) {
    fprintf(stdout, "***Found unresolved symbol: %s***\n", sym->name);
  }
}
