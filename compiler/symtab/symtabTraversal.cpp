#include "moduleList.h"
#include "symtab.h"
#include "symtabTraversal.h"

void SymtabTraversal::preProcessScope(SymScope* scope) {
}


void SymtabTraversal::postProcessScope(SymScope* scope) {
}


void SymtabTraversal::processSymbol(Symbol* sym) {
}


void SymtabTraversal::run(ModuleList* moduleList) {
  switch (whichModules) {
  case MODULES_ALL:
    Symboltable::traverse(this);
    break;
  case MODULES_CODEGEN:
    // start from the common module's scope in this case
    // all non-internal modules are sub modules of common as of 5/12/05
    Symboltable::traverseFromScope(this, commonModule->modScope);
    break;
  case MODULES_USER:
    {
      ModuleSymbol* mod = moduleList->first();
      while (mod) {
        if (mod->modtype == MOD_USER) {
          Symboltable::traverseFromScope(this, mod->modScope);
        }
        mod = moduleList->next();
      }
    }
    break;
  default:
    INT_FATAL("Unexpected case in SymtabTraversal::run");
  }
}


void SymtabTraversal::run(FnSymbol* function) {
  Symboltable::traverseFromScope(this, function->paramScope);
}

void SymtabTraversal::run(Type* type) {
  if (StructuralType *st = dynamic_cast<StructuralType*>(type))
    Symboltable::traverseFromScope(this, st->structScope);
}
