#include "removeNestedFunctions.h"
#include "symscope.h"
#include "expr.h"
#include "symtab.h"


void RemoveNestedFunctions::postProcessStmt(Stmt* stmt) {
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    //function definition
    if (FnSymbol* fn_sym = dynamic_cast<FnSymbol*>(def_stmt->defExpr->sym)){
      //nested function definition
      if (hasEnclosingFunction(def_stmt)) {
        printf("Found nested function: %s \n", fn_sym->name);
        //add to global scope
        ModuleSymbol* curr_module = fn_sym->parentScope->getModule();
        AList<Stmt>* module_stmts = curr_module->stmts;
        SymScope* saveScope =
          Symboltable::setCurrentScope(curr_module->modScope);
        DefStmt* fn_copy = def_stmt->copy(true);
        module_stmts->insertAtTail(fn_copy);
        def_stmt->remove();
        fn_sym->parentScope->remove(fn_sym);
        Symboltable::removeScope(fn_sym->paramScope);
        Symboltable::setCurrentScope(saveScope);
      }
    }   
  }
}

FnSymbol* RemoveNestedFunctions::hasEnclosingFunction(DefStmt* fn_def) {
    return (dynamic_cast<FnSymbol*>(fn_def->parentSymbol));
}
