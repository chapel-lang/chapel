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
   *** Resolve typeBinding
   ***/
  if (dynamic_cast<UnresolvedSymbol*>(fn->typeBinding)) {
    Symbol* typeBindingSymbol = Symboltable::lookup(fn->typeBinding->name);
    if (dynamic_cast<TypeSymbol*>(typeBindingSymbol)) {
      Type* typeBinding = typeBindingSymbol->type;
      fn->typeBinding = typeBindingSymbol;
      fn->method_type = SECONDARY_METHOD;
      typeBinding->methods.add(fn);
    } else {
      USR_FATAL(fn, "Function is not bound to type");
    }
  }
  
  /***
   *** Mangle code-generation name and insert this as first parameter
   ***/
  
  if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(fn->typeBinding)) {
    fn->cname = glomstrings(4, "_", typeSym->cname, "_", fn->cname);
    SymScope* saveScope = Symboltable::getCurrentScope();
    Symboltable::setCurrentScope(fn->paramScope);
    Symbol* this_insert = new ParamSymbol(PARAM_REF, "this", typeSym->type);

    /* SJD: Should ParamSymbols have defExprs of their own? */
    this_insert->setDefPoint(def_stmt->defExprls);

    Symboltable::setCurrentScope(saveScope);
    this_insert = appendLink(this_insert, fn->formals);
    fn->formals = this_insert;
    fn->_this = this_insert;
  }

  /***
   *** Move function out of class scope, into module scope
   ***/

  if (fn->defPoint->parentSymbol == fn->typeBinding) {
    Stmt* insertPoint = fn->typeBinding->defPoint->parentStmt;
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
    StructuralType* structuralType = dynamic_cast<StructuralType*>(fn->typeBinding->type);
    for (int i = 0; i < structuralType->methods.n; i++) {
      if (structuralType->methods.v[i] == fn) {
        structuralType->methods.v[i] = newFn;
      }
    }
    Symboltable::setCurrentScope(saveScope);
  }
}
