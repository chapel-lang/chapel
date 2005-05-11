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
  } else if (whichModules == MODULES_COMMON_AND_USER) {
    // start from the common module's scope in this case
    // all modules are sub modules of common as of 4/11/05
    Symboltable::traverseFromScope(this, commonModule->modScope);
  } else {
    ModuleSymbol* mod = moduleList;
    while (mod) {
      if (mod->modtype == MOD_USER) {
        Symboltable::traverseFromScope(this, mod->modScope);
      }
      mod = nextLink(ModuleSymbol, mod);
    }
  }
}


void SymtabTraversal::run(FnSymbol* function) {
  Symboltable::traverseFromScope(this, function->paramScope);
}

void SymtabTraversal::run(Type* type) {
  if (StructuralType *st = dynamic_cast<StructuralType*>(type))
    Symboltable::traverseFromScope(this, st->structScope);
}
