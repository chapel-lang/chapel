#ifndef _FIND_TYPE_VARIABLE_H_
#define _FIND_TYPE_VARIABLE_H_

#include "traversal.h"
#include "vec.h"
#include "type.h"

class FindVariableType : public Traversal {
 public:
  Vec<VariableType*>* variableTypes;
  bool found;
  FindVariableType(Vec<VariableType*>* init_variableTypes);
  void preProcessSymbol(Symbol* symbol);
  void preProcessType(Type* type);
};

bool
functionContainsVariableType(FnSymbol* fn, Vec<VariableType*>* variableTypes);

#endif
