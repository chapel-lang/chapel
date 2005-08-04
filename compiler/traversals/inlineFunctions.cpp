#include <stdio.h>
#include <typeinfo>
#include "inlineFunctions.h"
#include "stringutil.h"
#include "symtab.h"
#include "replaceReturns.h"

void InlineFunctions::postProcessExpr(Expr* expr) {
  //no inlining compiler flag was used
  if (no_inline)
    return;
  //function call
  if (ParenOpExpr* fn_call = dynamic_cast<ParenOpExpr*>(expr)) {
    if (fn_call->isPrimitive() || fn_call->opTag != OP_NONE) {
      return;
    }
    //for now, only leaf getter/setter functions will be inlined 
    FnSymbol* fn_sym = fn_call->findFnSymbol(); 
    if (fn_sym->hasPragma("inline") && isCodegened(fn_sym)) { 
      _ok_to_inline = true;
      //map formal parameters to actual arguments
      Map<BaseAST*,BaseAST*>* formal_to_actual_arg_map = createFormalToActualArgMappings(fn_call, fn_sym->formals); 
      //inline body if it is ok to do so even after examining the actual arguments
      if (_ok_to_inline) {
        Stmt* inlined_body = fn_sym->body->copy(true,formal_to_actual_arg_map,NULL);
        ReplaceReturns* rep_returns;
        if (fn_sym->retType && (fn_sym->retType != dtVoid)) {
          DefExpr* temp_def = createTempVariable(fn_sym->retType);
          fn_call->parentStmt->insertBefore(new ExprStmt(temp_def));
          //replace all returns in the inlined function body 
          //with an assignment the return expression
          rep_returns = new ReplaceReturns(temp_def->sym);
          inlined_body->traverse(rep_returns);
          //inlined function
          fn_call->parentStmt->insertBefore(inlined_body);
          fn_call->replace(new Variable(temp_def->sym));
        }
        else {
          //inlined function
          fn_call->parentStmt->insertBefore(inlined_body);
          fn_call->parentStmt->remove();
        }
      }
    }
  }
}

bool InlineFunctions::isCodegened(FnSymbol* fn_sym) {
  //do not inline methods that are not codegened (e.g. prelude)
  ModuleSymbol* mod_sym = fn_sym->paramScope->getModule();
  return (!fn_sym->hasPragma("no codegen") && (mod_sym->modtype != MOD_INTERNAL));  
}

DefExpr* InlineFunctions::createTempVariable(Type* type, Expr* init) {
  static int id = 1;
  char* temp_name =  glomstrings(2, "_inline_temp_", intstring(id++));
  DefExpr* temp_def = Symboltable::defineSingleVarDef(temp_name,
                                                      type,
                                                      init,
                                                      VAR_NORMAL,
                                                      VAR_VAR);
  dynamic_cast<VarSymbol*>(temp_def->sym)->noDefaultInit = true;
  return temp_def;
}

bool InlineFunctions::isFormalParamOut(ParamSymbol* p_sym) {
  return ((p_sym->intent == PARAM_INOUT) || (p_sym->intent == PARAM_OUT));
}

bool InlineFunctions::isFormalParamRef(ParamSymbol* p_sym) {
  return (p_sym->intent == PARAM_REF) || dynamic_cast<ClassType*>(p_sym->type);
}

bool InlineFunctions::isTypeVar(ParamSymbol* p_sym) {
  return (p_sym->typeVariable != NULL);
}

Map<BaseAST*,BaseAST*>* InlineFunctions::createFormalToActualArgMappings(ParenOpExpr* fn_call, AList<DefExpr>* formal_params) {
  Expr* curr_arg;
  DefExpr* curr_param;
  AList<Expr>* actual_args = fn_call->argList;
  
  if (actual_args) {
    curr_arg = actual_args->first();
    curr_param = formal_params->first();
  }
  //go through all the actual arguments
  Map<BaseAST*,BaseAST*>* formal_to_actual_arg_map = new Map<BaseAST*,BaseAST*>();
  while(curr_arg) {
    bool param_ref = isFormalParamRef(dynamic_cast<ParamSymbol*>(curr_param->sym));
    bool param_intent_out = isFormalParamOut(dynamic_cast<ParamSymbol*>(curr_param->sym));
    bool typeVar = isTypeVar(dynamic_cast<ParamSymbol*>(curr_param->sym)); 
    //do not inline the function call if an argument is a ref to a expression that is no
    //a variable
    Variable* variable;
    if (param_ref || typeVar) {
      variable = dynamic_cast<Variable*>(curr_arg);
      if (!variable) {
        _ok_to_inline = false;
        return NULL;
      }
    }
    
    //create temporary variable and initialize it with the actual argument 
    DefExpr* temp_def;
    if (!param_ref && !typeVar) {
      temp_def = createTempVariable(curr_arg->typeInfo(), curr_arg->copy());
      fn_call->parentStmt->insertBefore(new ExprStmt(temp_def));
      //map variable of param symbol to temp symbol so that when copy is passed the map, it
      //will replace the formal parameter symbol with the temp symbol                               
      formal_to_actual_arg_map->put(curr_param->sym, temp_def->sym);
    }
    //since a temporary variable was not created, map the actual argument to the formal parameter
    else 
      formal_to_actual_arg_map->put(curr_param->sym, variable->var);
      
      //copy temp back to actual arg if formal param out
    if (param_intent_out)
      if (Variable* v = dynamic_cast<Variable*>(curr_arg))
        fn_call->parentStmt->insertAfter(new ExprStmt(new ParenOpExpr(OP_GETSNORM, new Variable(v->var), new Variable(temp_def->sym))));
    

    curr_arg = actual_args->next();
    curr_param = formal_params->next();
  }
  return formal_to_actual_arg_map;
}




