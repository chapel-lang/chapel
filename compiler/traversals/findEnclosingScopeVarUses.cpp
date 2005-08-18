#include "findEnclosingScopeVarUses.h"
#include "removeNestedFunctions.h"
#include "symscope.h"
#include "symtab.h"

FindEnclosingScopeVarUses::FindEnclosingScopeVarUses(SymScope* encl_scope, Map<FnSymbol*, Vec<Symbol*>*>* all_funcs_encl_var_uses_map, 
                                                     Vec<FnSymbol*>* in_process_fns) {
  _encl_scope = encl_scope;
  _all_funcs_encl_var_uses_map = all_funcs_encl_var_uses_map;
  _curr_var_uses_vec = new Vec<Symbol*>();
  _in_process_fns = in_process_fns;
 
}


void FindEnclosingScopeVarUses::preProcessStmt(Stmt* stmt) {
  if (ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr_stmt->expr))
      //function definition
      if (FnSymbol* fn_sym = dynamic_cast<FnSymbol*>(defExpr->sym))
        computeNestFuncEnclScopeVarUses(fn_sym);
  }
}

void FindEnclosingScopeVarUses::postProcessStmt(Stmt* stmt) {
  if (ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr_stmt->expr))
      //function definition
      if (FnSymbol* fn_sym = dynamic_cast<FnSymbol*>(defExpr->sym))
        //nested function, remove from list variables defined within this function scope
        if (RemoveNestedFunctions::hasEnclosingFunction(defExpr)) {
          _curr_var_uses_vec = removeCurrScopeVars(fn_sym);
        }
  }    
}

void FindEnclosingScopeVarUses::preProcessExpr(Expr* expr) {
  //is this an enclosing var use
  if (SymExpr* v = dynamic_cast<SymExpr*>(expr)) {
    checkIfEnclVar(v);
  }
  //get enclosing variables in function call and propagate up
  else if (CallExpr* ce = dynamic_cast<CallExpr*>(expr)) {
    computeNestFuncEnclScopeVarUses(ce->findFnSymbol());
  }
}

Vec<Symbol*>* FindEnclosingScopeVarUses::getVarUses() {return _curr_var_uses_vec;}

void FindEnclosingScopeVarUses::checkIfEnclVar(SymExpr* v) {
  Symbol* sym = NULL;
  if (VarSymbol* v_sym = dynamic_cast<VarSymbol*>(v->var))
    sym = v_sym;
  else if (ParamSymbol* p_sym = dynamic_cast<ParamSymbol*>(v->var))
    sym = p_sym; 
  if (sym) {
    //variable defined in enclosing function scope
    SymScope* var_sym_scope = sym->parentScope;
    SymScope* encl_scope = _encl_scope;
    //look up outer scopes until module scope is reached or symbol is found
    do {
      if (var_sym_scope == encl_scope) {
        _curr_var_uses_vec->add_exclusive(sym);
        return;
      }
      encl_scope = encl_scope->parent;
    }while (encl_scope->type != SCOPE_MODULE);
  }
}

void FindEnclosingScopeVarUses::computeNestFuncEnclScopeVarUses(FnSymbol* fn_sym) {
  if (fn_sym) {
    //nested function, or atleast used to be
    Vec<Symbol*>* fn_call_encl_var_uses = _all_funcs_encl_var_uses_map->get(fn_sym);
    if (RemoveNestedFunctions::hasEnclosingFunction(fn_sym->defPoint) || fn_call_encl_var_uses) {
      //already found encl var uses for function call
      if (fn_call_encl_var_uses) {
        forv_Symbol(symbol, *fn_call_encl_var_uses)
          _curr_var_uses_vec->add_exclusive(symbol);
      }
      //compute encl var uses for function call
      else if (!_in_process_fns->in(fn_sym)){
        //only compute once
        _in_process_fns->add(fn_sym);
        fn_call_encl_var_uses = RemoveNestedFunctions::getEnclosingFuncVarUses(fn_sym, _all_funcs_encl_var_uses_map, _in_process_fns);
        _all_funcs_encl_var_uses_map->put(fn_sym, fn_call_encl_var_uses);
        forv_Symbol(symbol, *fn_call_encl_var_uses)
          _curr_var_uses_vec->add_exclusive(symbol);
        _in_process_fns->pop();
      }
    }
  }
}

Vec<Symbol*>* FindEnclosingScopeVarUses::removeCurrScopeVars(FnSymbol* fn_sym) {
  //remove any symbols defined in scope from enclosing scope var use vector
  Vec<Symbol*>* new_encl_var_uses = new Vec<Symbol*>();
  forv_Symbol(symbol, *_curr_var_uses_vec) {
    if (!fn_sym->body->blkScope->symbols.in(symbol) && 
        !fn_sym->paramScope->symbols.in(symbol))
      new_encl_var_uses->add_exclusive(symbol);
  }
  //update map
  _all_funcs_encl_var_uses_map->put(fn_sym,new_encl_var_uses);
  return new_encl_var_uses;
}

