#include "findTypeVariables.h"
#include "type.h"
#include "symbol.h"
#include "symtab.h"

FindVariableType::FindVariableType(Vec<VariableType*>* init_variableTypes) {
  variableTypes = init_variableTypes;
}

void FindVariableType::preProcessSymbol(Symbol* symbol) {
  if (dynamic_cast<TypeSymbol*>(symbol)) {
    forv_Vec(VariableType, variableType, *variableTypes) {
      if (variableType == symbol->type) {
        found = true;
      }
    }
  }
}

void FindVariableType::preProcessType(Type* type) {
  if (dynamic_cast<VariableType*>(type)) {
    forv_Vec(VariableType, variableType, *variableTypes) {
      if (variableType == type) {
        found = true;
      }
    }
  }
}

bool functionContainsVariableType(FnSymbol* fn,
                                  Vec<VariableType*>* variableTypes) {
  FindVariableType* traversal = new FindVariableType(variableTypes);
  TRAVERSE(fn, traversal, true);
  return traversal->found;
}
