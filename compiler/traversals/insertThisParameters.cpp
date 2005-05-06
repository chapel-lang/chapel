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
          // fn->paramScope->parent = classBindingType->structScope;
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
   *** Move function out of class scope, into module scope
   ***/

  if (fn->defPoint->parentSymbol == fn->classBinding) {
    Stmt* insertPoint = fn->classBinding->defPoint->parentStmt;
    while (!dynamic_cast<ModuleSymbol*>(insertPoint->parentSymbol)) {
      insertPoint = insertPoint->parentSymbol->defPoint->parentStmt;
    }
    ModuleSymbol* moduleSymbol = dynamic_cast<ModuleSymbol*>(insertPoint->parentSymbol);
    SymScope* saveScope =
      Symboltable::setCurrentScope(moduleSymbol->modScope);
    DefStmt* defStmt = dynamic_cast<DefStmt*>(fn->defPoint->parentStmt->copy(true));
    FnSymbol* newFn = defStmt->fnDef();
    newFn->cname = copystring(fn->cname);
    insertPoint->insertBefore(defStmt);
    fn->defPoint->parentStmt->extract();
    fn->parentScope->remove(fn);
    Symboltable::removeScope(fn->paramScope);
    StructuralType* structuralType = dynamic_cast<StructuralType*>(fn->classBinding->type);
    for (int i = 0; i < structuralType->methods.n; i++) {
      if (structuralType->methods.v[i] == fn) {
        structuralType->methods.v[i] = newFn;
      }
    }
    Symboltable::setCurrentScope(saveScope);
  }
}
