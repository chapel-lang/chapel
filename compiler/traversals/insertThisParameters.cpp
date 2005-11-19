#include "insertThisParameters.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

void InsertThisParameters::preProcessExpr(Expr* expr) {
  DefExpr* defExpr = dynamic_cast<DefExpr*>(expr);

  if (!defExpr) {
    return;
  }

  /***
   *** Mangle type names in class types
   ***/
  if (dynamic_cast<TypeSymbol*>(defExpr->sym)) {
    if (ClassType* classType = dynamic_cast<ClassType*>(defExpr->sym->type)) {
      forv_Vec(TypeSymbol, type, classType->types) {
        type->cname = stringcat("_", classType->symbol->cname, "_", type->cname);
      }
    }
  }

  FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym);

  if (!fn) {
    return;
  }

  /***
   *** Resolve typeBinding
   ***/
  if (fn->typeBinding && fn->typeBinding->isUnresolved) {
    Symbol* typeBindingSymbol = Symboltable::lookup(fn->typeBinding->name);
    assert(!typeBindingSymbol->isUnresolved);
    if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(typeBindingSymbol)) {
      Type* typeBinding = ts->definition;
      fn->typeBinding = ts;
      if (fn->fnClass != FN_CONSTRUCTOR) {
        fn->method_type = SECONDARY_METHOD;
      }
      typeBinding->methods.add(fn);
    } else {
      USR_FATAL(fn, "Function is not bound to type");
    }
  }
  
  /***
   *** Mangle code-generation name and insert this as first argument
   ***/
  
  if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(fn->typeBinding)) {
    fn->cname = stringcat("_", typeSym->cname, "_", fn->cname);
    if (fn->fnClass != FN_CONSTRUCTOR) {
      ArgSymbol* this_insert = new ArgSymbol(INTENT_REF, "this", typeSym->definition);
      fn->formals->insertAtHead(new DefExpr(this_insert));
      fn->_this = this_insert;
      bool isThisMethod = !strcmp(fn->name, "this");
      if (!isThisMethod) {
        ArgSymbol* token_dummy = new ArgSymbol(INTENT_REF, "_methodTokenDummy",
                                               dtMethodToken);
        fn->formals->insertAtHead(new DefExpr(token_dummy));
      }
      if (fn->isSetter) {
        ArgSymbol* setter_dummy = new ArgSymbol(INTENT_REF, "_setterTokenDummy", 
                                                dtSetterToken);
        int len = fn->formals->length();
        fn->formals->get(len)->insertBefore(new DefExpr(setter_dummy));
      }
    }
  }
}


void insertThisParameters(void) {
  Pass* pass = new InsertThisParameters();
  pass->run(Symboltable::getModules(pass->whichModules));
}
