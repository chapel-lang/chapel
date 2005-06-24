#include <stdio.h>
#include <typeinfo>
#include "inlineFunctions.h"
#include "findLeafFunctions.h"
#include "stringutil.h"
#include "symtab.h"
#include "replaceReturns.h"

extern FuncLeaves* _leaf_functions;

void InlineFunctions::postProcessExpr(Expr* expr) {
  //function call
  if (FnCall* fn_call = dynamic_cast<FnCall*>(expr)) {
    //for now, only leaf getter/setter functions will be inlined 
    FnSymbol* fn_sym = fn_call->findFnSymbol();
    // found getter/setter function call
    //if (fn_sym  && (fn_sym->_getter || fn_sym->_setter))
    if (isLeafFunction(fn_sym)) { 
      //map formal parameters to actual arguments
      Map<BaseAST*,BaseAST*>* formal_to_actual_arg_map = createFormalToActualArgMappings(fn_call, fn_sym->formals);
      Stmt* inlined_body = fn_sym->body->copy(false,formal_to_actual_arg_map,NULL);
        ReplaceReturns* rep_returns;
        DefExpr* temp_def_expr;
        //create a temp variable
        if (fn_sym->retType && (fn_sym->retType != dtVoid)) {
          DefStmt* temp_def_stmt = createTempVariable(fn_sym->retType);
          temp_def_expr = temp_def_stmt->defExprls->only();
  
          fn_call->parentStmt->insertBefore(temp_def_stmt);
          rep_returns = new ReplaceReturns(temp_def_expr->sym);
          inlined_body->traverse(rep_returns);
          //inlined function
          fn_call->parentStmt->insertBefore(inlined_body);
          fn_call->replace(new Variable(temp_def_expr->sym));
        }
        else {
          //inlined function
          fn_call->parentStmt->insertBefore(inlined_body);
          fn_call->parentStmt->replace(new NoOpStmt());
        }
      }
  }
}

DefStmt* InlineFunctions::createTempVariable(Type* type, Expr* init) {
  static int id = 1;
  char* temp_name =  glomstrings(2, "_inline_temp_", intstring(id++));
  DefStmt* temp_def_stmt = Symboltable::defineSingleVarDefStmt(temp_name,
                                                          type,
                                                          init,
                                                          VAR_NORMAL,
                                                          VAR_VAR);
  return temp_def_stmt;
}

Map<BaseAST*,BaseAST*>* InlineFunctions::createFormalToActualArgMappings(FnCall* fn_call, AList<ParamSymbol>* formal_params) {
  Expr* curr_arg;
  ParamSymbol* curr_param;
  AList<Expr>* actual_args = fn_call->argList;
  if (actual_args) {
    curr_arg = actual_args->first();
    curr_param = formal_params->first();
  }
  //go through all the actual arguments
  Map<BaseAST*,BaseAST*>* formal_to_actual_arg_map = new Map<BaseAST*,BaseAST*>();
  while(curr_arg) {
    //create temporary variable and initialize it with the actual argument 
    DefStmt* temp_def_stmt = createTempVariable(curr_arg->typeInfo(), curr_arg->copy());
    fn_call->parentStmt->insertBefore(temp_def_stmt);
    //map variable of param symbol to temp symbol so that when copy is passed the map, it
    //will replace the formal parameter symbol with the temp symbol
    DefExpr* temp_def_expr = temp_def_stmt->defExprls->only();
    formal_to_actual_arg_map->put(curr_param, temp_def_expr->sym);
    curr_arg = actual_args->next();
    curr_param = formal_params->next();
  }
  return formal_to_actual_arg_map;
}

bool InlineFunctions::isLeafFunction(FnSymbol* fs) {
  FuncLeaves* temp_func_leaves = _leaf_functions;
  while(temp_func_leaves) {
    if (fs == temp_func_leaves->first)
      return true;
    temp_func_leaves = temp_func_leaves->next;
  }
  return false;
}

InlineFunctions::~InlineFunctions() {
  FuncLeaves* tmp = _leaf_functions;
  FuncLeaves* delete_leaf;
  while(tmp) {
    delete_leaf = tmp;
    tmp = tmp->next;
    delete delete_leaf;
  }
}


