#include "symtab.h"
#include "symtabTraversal.h"

void SymtabTraversal::preProcessScope(SymScope* scope) {
}


void SymtabTraversal::postProcessScope(SymScope* scope) {
}


void SymtabTraversal::processSymbol(Symbol* sym) {
}


void SymtabTraversal::run(ModuleSymbol* moduleList) {
  if (whichModules == MODULES_ALL) {
    // start from root of symboltable rather than internal scopes in
    // this case
    Symboltable::traverse(this);
  } else {
    ModuleSymbol* mod = moduleList;
    while (mod) {
      Symboltable::traverseFromScope(this, mod->modScope);

      mod = nextLink(ModuleSymbol, mod);
    }
  }
}


void SymtabTraversal::run(FnSymbol* function) {
  Symboltable::traverseFromScope(this, function->paramScope);
}
