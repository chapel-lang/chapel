#include "findTypeVariables.h"
#include "type.h"
#include "symbol.h"
#include "symtab.h"

FindVariableType::FindVariableType(Vec<BaseAST*>* init_asts) {
  found = false;
  asts = init_asts;
}

void FindVariableType::preProcessSymbol(Symbol* symbol) {
  if (asts->set_in(symbol))
    found = true;
}

void FindVariableType::preProcessType(Type* type) {
  if (asts->set_in(type))
    found = true;
}

bool functionContainsAnyAST(FnSymbol* fn, Vec<BaseAST*>* asts_set) {
  FindVariableType* traversal = new FindVariableType(asts_set);
  TRAVERSE(fn, traversal, true);
  return traversal->found;
}
