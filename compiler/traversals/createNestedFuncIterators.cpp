#include "createNestedFuncIterators.h"
#include "findEnclosingScopeVarUses.h"
#include "stringutil.h"
#include "symtab.h"
#include "symscope.h"

class InsertNestedFuncInIterator : public Traversal {
  public :
  FnSymbol* _fn_call_sym;

  InsertNestedFuncInIterator(FnSymbol* fn_call_sym) {
    _fn_call_sym = fn_call_sym;
  }

  void postProcessExpr(Expr* expr) {
    //replace seq yield call in iterator with call to nested function
    if (CallExpr* fc = dynamic_cast<CallExpr*>(expr)) {
     if (Variable*  v = dynamic_cast<Variable*>(fc->baseExpr))
      if (!strcmp(v->var->name, "_yield")) {
        AList<Expr>* new_arg_list = getNewArgList(fc->argList);
        CallExpr* fn_call = new CallExpr(_fn_call_sym, new_arg_list->copy());
        fc->replace(fn_call);
      }
    }
  }

  void postProcessStmt(Stmt* stmt) {
    if (ReturnStmt* ret = dynamic_cast<ReturnStmt*>(stmt))
      //iterator now has a void return
      if (ret->expr)
        ret->expr->remove();  
  }
  
  AList<Expr>* getNewArgList (AList<Expr>* old_arg_list) {
    AList<Expr>* new_arg_list = old_arg_list->copy();
    //first argument to seq yield is the sequence, second is the element 
    //returned to the old iterator for loop
    new_arg_list->popHead();
   
    return new_arg_list;
  }
};


void CreateNestedFuncIterators::postProcessStmt(Stmt* stmt) {
    if (ForLoopStmt* fls = dynamic_cast<ForLoopStmt*>(stmt)) {
      AList<Expr>* user_iter_call_list = getIteratorCallsHelper(fls->iterators,new AList<Expr>());
      //found user iterator calls
      for_alist(Expr, expr, user_iter_call_list) {
        //copy iterator function def
        CallExpr* paren_op = dynamic_cast<CallExpr*>(expr);
        FnSymbol* user_iter_fn_sym = paren_op->findFnSymbol();
        FnSymbol* func_it_sym = copyIteratorDef(user_iter_fn_sym);
        fls->insertBefore(new ExprStmt(new DefExpr(func_it_sym)));
        
        //insert nested function created using the body of the iterator loop   
        CallExpr* new_func_call = new CallExpr(new Variable(func_it_sym), paren_op->argList->copy());
        fls->insertBefore(new ExprStmt(new_func_call));
        
        //place body of for loop in a nested function definition
        DefExpr* nested_func_def = copyLoopBodyToNestedFuncDef(fls, func_it_sym);
        FnSymbol* fn_call_sym = dynamic_cast<FnSymbol*>(nested_func_def->sym);
        
        //replace calls to seq append in iterator body with nested function call
        func_it_sym->body->traverse(new InsertNestedFuncInIterator(fn_call_sym));
        
      }
      //found user iterator call
      if(!user_iter_call_list->isEmpty()) {
        //remove iterator for loop
        fls->remove();
      }
    }
}

AList<Expr>* CreateNestedFuncIterators::getIteratorCallsHelper(AList<Expr>* iterator_list, AList<Expr>* user_iterator_list) {
  //create list of user iterators
  for_alist(Expr, iterator, iterator_list) {
    if (CallExpr* paren_op = dynamic_cast<CallExpr*>(iterator)){
      if (Variable* variable = dynamic_cast<Variable*>(paren_op->baseExpr)){
        FnSymbol* fn_sym = dynamic_cast<FnSymbol*>(variable->var);
        if (fn_sym->paramScope->getModule()->modtype == MOD_USER)
          user_iterator_list->insertAtTail(paren_op->copy());
      }
      //check arglist for user iterators
      getIteratorCallsHelper(paren_op->argList, user_iterator_list);
    }
  }
  return user_iterator_list;
}


DefExpr* CreateNestedFuncIterators::copyLoopBodyToNestedFuncDef(ForLoopStmt* fls, FnSymbol* iterator_sym) {
  //create a nested function definition
  static int id = 1; 
  char* func_name =  glomstrings(2, "_nested_func_", intstring(id++));
  DefExpr* nested_func = copyFuncHelper(func_name, fls->indices, fls->innerStmt);
  iterator_sym->body->body->insertAtTail(new ExprStmt(nested_func));
  //to inline these nested function calls in the iterator, uncomment next line
  nested_func->sym->addPragma("inline");
  
  return nested_func;
}

FnSymbol* CreateNestedFuncIterators::copyIteratorDef(FnSymbol* old_iterator_sym) {
  static int it_id = 1;
  char* func_name = glomstrings(2, old_iterator_sym->name, intstring(it_id++));
  DefExpr* func_def = copyFuncHelper(func_name, old_iterator_sym->formals, old_iterator_sym->body);
  
  //return new_func_sym;
  return dynamic_cast<FnSymbol*>(func_def->sym);
}

DefExpr* CreateNestedFuncIterators::copyFuncHelper(char* new_name, AList<DefExpr>* copy_formals, BlockStmt* copy_body) {
  //create a function definition
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol(new_name));
  AList<DefExpr>* func_formals = new AList<DefExpr>();
  
  //copy formals for new function definition
  Map<BaseAST*,BaseAST*>* update_map = new Map<BaseAST*,BaseAST*>();
  for_alist(DefExpr, def, copy_formals) {
    ParamSymbol* param = new ParamSymbol(PARAM_BLANK, def->sym->name, def->sym->type);
    func_formals->insertAtTail(new DefExpr(param));
    update_map->put(def->sym,param);
  }
  //copy body for new function definition
  Symboltable::continueFnDef(fn, func_formals, dtVoid);
  AList<Stmt>* stmts = copy_body->body->copy(true, update_map);
  BlockStmt* fn_body = new BlockStmt(stmts);
  DefExpr* func_def = new DefExpr(Symboltable::finishFnDef(fn, fn_body));
  return func_def;
}
