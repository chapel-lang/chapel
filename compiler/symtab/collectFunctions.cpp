#include "collectFunctions.h"
#include "symbol.h"
#include "symscope.h"
#include "symtab.h"

CollectFunctions::CollectFunctions(Vec<FnSymbol*>* init_functions) {
  functions = init_functions;
  functions->clear();
}

void CollectFunctions::processSymbol(Symbol* sym) {
  if (FnSymbol* function = dynamic_cast<FnSymbol*>(sym)) {
    functions->add(function);
  }
}

void collect_functions(Vec<FnSymbol*>* functions) {
  CollectFunctions* traversal = new CollectFunctions(functions);
  traversal->run();
}

void collectFunctionsFromScope(SymScope* scope, Vec<FnSymbol*>* functions) {
  CollectFunctions* traversal = new CollectFunctions(functions);
  Symboltable::traverseFromScope(traversal, scope);
}
