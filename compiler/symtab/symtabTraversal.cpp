#include "symtab.h"
#include "symtabTraversal.h"

void SymtabTraversal::preProcessScope(SymScope* scope) {
}


void SymtabTraversal::postProcessScope(SymScope* scope) {
}


void SymtabTraversal::processSymbol(Symbol* sym) {
}


void SymtabTraversal::run(void) {
  Symboltable::traverse(this);
}


void SymtabTraversal::run(ModuleSymbol* moduleList) {
  run();
}
