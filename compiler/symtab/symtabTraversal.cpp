#include "symtab.h"
#include "symtabTraversal.h"

void SymtabTraversal::preProcessScope(SymScope* scope) {
}


void SymtabTraversal::postProcessScope(SymScope* scope) {
}


void SymtabTraversal::processSymbol(Symbol* sym) {
}


void SymtabTraversal::run(Vec<ModuleSymbol*>* modules) {
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
      forv_Vec(ModuleSymbol, mod, *modules) {
        if (mod->modtype == MOD_USER) {
          Symboltable::traverseFromScope(this, mod->modScope);
        }
      }
    }
    break;
  default:
    INT_FATAL("Unexpected case in SymtabTraversal::run");
  }
}


void SymtabTraversal::run(FnSymbol* function) {
  Symboltable::traverseFromScope(this, function->argScope);
}

void SymtabTraversal::run(Type* type) {
  if (ClassType *st = dynamic_cast<ClassType*>(type))
    Symboltable::traverseFromScope(this, st->structScope);
}
