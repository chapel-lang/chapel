#include "symtab.h"
#include "symtabTraversal.h"

void SymtabTraversal::preProcessScope(SymScope* scope) {
}


void SymtabTraversal::postProcessScope(SymScope* scope) {
}


void SymtabTraversal::processSymbol(Symbol* sym) {
}


void SymtabTraversal::run(void) {
  if (skipInternalScopes) {
    // BLC: took a wild stab at what scope we should start at, but am
    // still not clear at how the common module is inserted
    Symboltable::traverseFromScope(this, commonModule->modScope->child);
  } else {
    Symboltable::traverse(this);
  }
}


void SymtabTraversal::run(ModuleSymbol* moduleList) {
  run();
  //  Symboltable::traverseFromScope(this, moduleList->modScope);
}


void SymtabTraversal::run(FnSymbol* function) {
  Symboltable::traverseFromScope(this, function->paramScope);
}
