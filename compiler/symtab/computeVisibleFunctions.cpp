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
    if (ClassType* class_type = dynamic_cast<ClassType*>(type_sym->definition)) {
      if (class_type->classTag == CLASS_CLASS) {
        forv_Vec(FnSymbol, method, class_type->methods) {
          for (Symbol* tmp = method; tmp; tmp = tmp->overload) {
            FnSymbol* method = tmp->getFnSymbol();
            classMethods->add(method);
          }
        }
        for (Symbol* tmp = class_type->defaultConstructor;
             tmp;
             tmp = tmp->overload) {
          FnSymbol* constructor = tmp->getFnSymbol();
          classMethods->add(constructor);
        }
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
