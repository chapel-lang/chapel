
#include "removeNestedFunctions.h"
#include "symscope.h"
#include "symtab.h"
#include "findEnclosingScopeVarUses.h"
#include "alist.h"


RemoveNestedFunctions::RemoveNestedFunctions() {
  _nested_func_args_map = new Map<FnSymbol*,Vec<Symbol*>*>();
  _nested_func_sym_map = new Map<FnSymbol*, FnSymbol*>();
}

void RemoveNestedFunctions::preProcessStmt(Stmt* stmt) {
  if (ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr_stmt->expr))
      //function definition
      if (FnSymbol* fn_sym = dynamic_cast<FnSymbol*>(defExpr->sym))
        //nested function definition
        if (hasEnclosingFunction(defExpr)) {
          Vec<Symbol*>* encl_func_var_uses;
          encl_func_var_uses = getEnclosingFuncVarUses(fn_sym);
          //store nested function actual arg info
          _nested_func_args_map->put(fn_sym, encl_func_var_uses);
        }
  }
}

void RemoveNestedFunctions::postProcessStmt(Stmt* stmt) {
  if (ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr_stmt->expr)) {
      //function definition
      if (FnSymbol* fn_sym = dynamic_cast<FnSymbol*>(defExpr->sym)){
        //nested function definition
        if(hasEnclosingFunction(defExpr)) {
          Vec<Symbol*>* encl_func_var_uses = _nested_func_args_map->get(fn_sym);
          printf("Found nested function: %s \n", fn_sym->name);
                 
          //add to global scope
          ModuleSymbol* curr_module = fn_sym->paramScope->getModule();
          AList<Stmt>* module_stmts = curr_module->stmts;
          SymScope* saveScope = Symboltable::setCurrentScope(curr_module->modScope);
          
          ExprStmt* fn_copy = expr_stmt->copy(true);
          //add formal parameters to copied nested function
          addNestedFuncFormals(fn_copy->expr, encl_func_var_uses, fn_sym);
          module_stmts->insertAtTail(fn_copy);
     
          expr_stmt->remove();
          fn_sym->parentScope->remove(fn_sym);
          Symboltable::removeScope(fn_sym->paramScope);
          Symboltable::setCurrentScope(saveScope);
        }
      }
    } 
  }
}

void RemoveNestedFunctions::postProcessExpr(Expr* expr) {
  if (ParenOpExpr* paren_op = dynamic_cast<ParenOpExpr*>(expr)) {
    if (Variable* v = dynamic_cast<Variable*>(paren_op->baseExpr))
      if (FnSymbol* fn_sym = dynamic_cast<FnSymbol*>(v->var)) {
        Vec<Symbol*>* encl_func_var_uses = _nested_func_args_map->get(fn_sym);
        //nested function call
        if (encl_func_var_uses)
          addNestedFuncActuals(paren_op, encl_func_var_uses);
      } 
  }
}

FnSymbol* RemoveNestedFunctions::hasEnclosingFunction(DefExpr* fn_def) {
    return (dynamic_cast<FnSymbol*>(fn_def->parentSymbol));
  return NULL;
}

Vec<Symbol*>* RemoveNestedFunctions::getEnclosingFuncVarUses(FnSymbol* fn_sym) {
  FindEnclosingScopeVarUses* fesv = new FindEnclosingScopeVarUses(fn_sym->parentScope);
  fn_sym->body->traverse(fesv);
  return fesv->getVarUses();
}

void RemoveNestedFunctions::addNestedFuncFormals(Expr* expr, Vec<Symbol*>* encl_var_uses, FnSymbol* old_func_sym) {
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn_sym = dynamic_cast<FnSymbol*>(defExpr->sym)) {
      //update old nested function symbol to new function symbol map
      _nested_func_sym_map->put(old_func_sym, fn_sym);
      Map<BaseAST*,BaseAST*>* update_map = new Map<BaseAST*,BaseAST*>();
      forv_Vec(Symbol, sym, *encl_var_uses) {
        if (sym) {
          printf("Symbol %s used in function %s is defined in the enclosing function\n", sym->name,fn_sym->name);
          //create formal and add to nested function
          DefExpr* formal = Symboltable::defineParam(PARAM_BLANK,sym->name,sym->type,NULL);
          fn_sym->formals->insertAtTail(formal);
          //will need to perform an update to map enclosing variables to formals
          update_map->put(sym, formal->sym);
        }
      }
      //not empty, added formals to nested function definition, perform symbol mapping
      if (encl_var_uses->n)
        fn_sym->body->traverse(new UpdateSymbols(update_map, NULL));
    }
  } 
}

void RemoveNestedFunctions::addNestedFuncActuals(ParenOpExpr* paren_op, Vec<Symbol*>* encl_var_uses) {
  AList<Expr>* nested_func_actuals = new AList<Expr>();
  //build nested function actuals list
  forv_Vec(Symbol, sym, *encl_var_uses) {
    if (sym) 
      nested_func_actuals->insertAtTail(new Variable(sym));
  }
  paren_op->argList = nested_func_actuals;
}
