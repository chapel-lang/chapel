#include "computeVisibleFunctions.h"
#include "stmt.h"
#include "expr.h"


class GetClassMethods : public SymtabTraversal {
 public:
  Vec<FnSymbol*>* classMethods;
  GetClassMethods::GetClassMethods(Vec<FnSymbol*>* init_classMethods);
  void processSymbol(Symbol*);
};


GetClassMethods::GetClassMethods(Vec<FnSymbol*>* init_classMethods) {
  classMethods = init_classMethods;
}


void GetClassMethods::processSymbol(Symbol* sym) {
  if (TypeSymbol* type_sym = dynamic_cast<TypeSymbol*>(sym)) {
    if (ClassType* class_type = dynamic_cast<ClassType*>(type_sym->type)) {
      forv_Vec(FnSymbol, method, class_type->methods) {
        while (method) {
          classMethods->add(method);
          method = method->overload;
        }
      }
      FnSymbol* constructor = class_type->defaultConstructor;
      while (constructor) {
        classMethods->add(constructor);
        constructor = constructor->overload;
      }
    }
  }
}


ComputeVisibleFunctions::ComputeVisibleFunctions() {
  classMethods.clear();
  GetClassMethods* traversal = new GetClassMethods(&classMethods);
  traversal->run();
}


void ComputeVisibleFunctions::preProcessScope(SymScope* scope) {
  scope->setVisibleFunctions(&classMethods);
}
