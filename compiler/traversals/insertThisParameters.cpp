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

  DefExpr* def_expr = dynamic_cast<DefExpr*>(def_stmt->defExprList);
  FnSymbol* fn = def_stmt->fnDef();

  if (!fn) {
    /*** mangle type names in class types ***/
    if (TypeSymbol* type_sym = def_stmt->typeDef()) {
      if (ClassType* classType = dynamic_cast<ClassType*>(type_sym->type)) {
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
        if (ClassType* classBindingType =
            dynamic_cast<ClassType*>(classBindingTypeSymbol->type)) {
          fn->classBinding = classBinding;
          fn->method_type = SECONDARY_METHOD;
          classBindingType->methods.add(fn);
          Symboltable::defineInScope(fn, classBindingType->classScope);
          fn->paramScope->parent = classBindingType->classScope;
        }
        else {
          USR_FATAL(fn, "Function is not bound to legal class");
        }
      }
      else {
        USR_FATAL(fn, "Function is not bound to legal class");
      }
    }
    else {
      USR_FATAL(fn, "Function is not bound to legal class");
    }
  }
  
  /***
   *** Mangle code-generation name and insert this as first parameter
   ***/
  
  if (TypeSymbol* class_symbol = dynamic_cast<TypeSymbol*>(fn->classBinding)) {
    if (ClassType* class_type = dynamic_cast<ClassType*>(class_symbol->type)) {
      fn->cname = glomstrings(4, "_", class_type->symbol->cname, "_", fn->cname);
      SymScope* saveScope = Symboltable::getCurrentScope();
      Symboltable::setCurrentScope(fn->paramScope);
      Symbol* this_insert = new ParamSymbol(PARAM_REF, "this", class_type);
      this_insert->setDefPoint(def_expr); /* SJD: Should ParamSymbols have defExprs of their own? */
      Symboltable::setCurrentScope(saveScope);
      this_insert = appendLink(this_insert, fn->formals);
      fn->formals = this_insert;
      fn->_this = this_insert;
    }
  }
}
