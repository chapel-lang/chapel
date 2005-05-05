#include "insertThisParameters.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

void InsertThisParameters::preProcessStmt(Stmt* stmt) {
  DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt);

  if (!def_stmt) {
    return;
  }

  FnSymbol* fn = def_stmt->fnDef();

  if (!fn) {
    /*** mangle type names in class types ***/
    if (TypeSymbol* type_sym = def_stmt->typeDef()) {
      if (StructuralType* classType = dynamic_cast<StructuralType*>(type_sym->type)) {
        forv_Vec(TypeSymbol, type, classType->types) {
          type->cname = glomstrings(4, "_", classType->symbol->cname, "_", type->cname);
        }
      }
    }
    return;
  }

  /***
   *** Resolve classBinding
   ***/
  if (dynamic_cast<UnresolvedSymbol*>(fn->classBinding)) {
    Symbol* classBinding = Symboltable::lookup(fn->classBinding->name);
    if (classBinding) {
      if (TypeSymbol* classBindingTypeSymbol =
          dynamic_cast<TypeSymbol*>(classBinding)) {
        if (StructuralType* classBindingType =
            dynamic_cast<StructuralType*>(classBindingTypeSymbol->type)) {
          fn->classBinding = classBinding;
          fn->method_type = SECONDARY_METHOD;
          classBindingType->methods.add(fn);
          fn->paramScope->parent = classBindingType->structScope;
        } else {
          USR_FATAL(fn, "Function is not bound to legal class");
        }
      } else {
        USR_FATAL(fn, "Function is not bound to legal class");
      }
    } else {
      USR_FATAL(fn, "Function is not bound to legal class");
    }
  }
  
  /***
   *** Mangle code-generation name and insert this as first parameter
   ***/
  
  if (TypeSymbol* class_symbol = dynamic_cast<TypeSymbol*>(fn->classBinding)) {
    if (StructuralType* class_type = dynamic_cast<StructuralType*>(class_symbol->type)) {
      fn->cname = glomstrings(4, "_", class_type->symbol->cname, "_", fn->cname);
      SymScope* saveScope = Symboltable::getCurrentScope();
      Symboltable::setCurrentScope(fn->paramScope);
      Symbol* this_insert = new ParamSymbol(PARAM_REF, "this", class_type);

      /* SJD: Should ParamSymbols have defExprs of their own? */
      this_insert->setDefPoint(def_stmt->defExprls);

      Symboltable::setCurrentScope(saveScope);
      this_insert = appendLink(this_insert, fn->formals);
      fn->formals = this_insert;
      fn->_this = this_insert;
    }
  }

  /***
   *** Move function out of class scope
   ***/
  if (TypeSymbol* class_symbol = dynamic_cast<TypeSymbol*>(fn->classBinding)) {
    if (def_stmt->parentSymbol == class_symbol) {
      if (StructuralType* class_type = dynamic_cast<StructuralType*>(class_symbol->type)) {
        def_stmt->extract();
        class_symbol->defPoint->parentStmt->insertBefore(def_stmt->copy());
        Symboltable::undefineInScope(fn, class_type->structScope);
        fn->overload = NULL; // Drop it on the ground since we're
                             // going to put all of the functions in
                             // the class scope into the scope before.
                             // They'll be rebuilt.
        Symboltable::defineInScope(fn, class_symbol->parentScope);
      }
    }
  }
}
