#include "createNestedFuncIterators.h"
#include "symbol.h"
#include "stringutil.h"
#include "symtab.h"
#include "alist.h"

class InsertNestedFuncInIterator : public Traversal {
  public :
  FnSymbol* _fn_call_sym;

  InsertNestedFuncInIterator(FnSymbol* fn_call_sym) {
    _fn_call_sym = fn_call_sym;
  }

  void postProcessExpr(Expr* expr) {
    //replace seq append call in iterator with call to nested function
    if (FnCall* fc = dynamic_cast<FnCall*>(expr)) {
      FnSymbol* fn_sym = fc->findFnSymbol();
      if (!strcmp(fn_sym->name, "_append_in_place")) {
        printf("found seq append \n");
        
        AList<Expr>* new_arg_list = getNewArgList(fc->argList);
        fc->replace(new FnCall(new Variable(_fn_call_sym), new_arg_list));
      }
      
    }
  }

  void postProcessStmt(Stmt* stmt) {
    if (ReturnStmt* ret = dynamic_cast<ReturnStmt*>(stmt)) {
      printf("found return \n");
      //iterator now has a void return
      ret->expr->remove();
    }
  }
  
  AList<Expr>* getNewArgList (AList<Expr>* old_arg_list) {
    //first argument to seq append is the sequence, second is the element
    old_arg_list->popHead();
    return old_arg_list->copy();
  }
};


void CreateNestedFuncIterators::postProcessExpr(Expr* expr) {
  if (ParenOpExpr* paren_op = dynamic_cast<ParenOpExpr*>(expr)) 
    if (Variable* variable = dynamic_cast<Variable*>(paren_op->baseExpr)){
      FnSymbol* fn_sym = dynamic_cast<FnSymbol*>(variable->var);
      ForLoopStmt* fls = dynamic_cast<ForLoopStmt*>(paren_op->parentStmt);
      //found iterator
        if (fn_sym && (fn_sym->fnClass == FN_ITERATOR) && fls && 
            (fn_sym->paramScope->getModule()->modtype == MOD_USER)) {
        printf("found iterator %s \n",fn_sym->name);
        //copy iterator function def
        FnSymbol* func_it_sym = copyIteratorDef(fn_sym);
        //insert nested function created using the body of the iterator loop
        ExprStmt* new_func_call = new ExprStmt(new FnCall(new Variable(func_it_sym), paren_op->argList->copy()));
        paren_op->parentStmt->insertBefore(new_func_call);
        //place body of for loop in a nested function definition
        DefExpr* nested_func_def = copyLoopBodyToNestedFuncDef(fls);
        FnSymbol* fn_call_sym = dynamic_cast<FnSymbol*>(nested_func_def->sym);
        paren_op->parentStmt->insertBefore(new ExprStmt(nested_func_def));
        paren_op->parentStmt->remove();
        //replace calls to seq append in iterator body with nested function call
        func_it_sym->body->traverse(new InsertNestedFuncInIterator(fn_call_sym));
      }
    }
}

DefExpr* CreateNestedFuncIterators::copyLoopBodyToNestedFuncDef(ForLoopStmt* fls) {
  //create a nested function definition
  static int id = 1;
  char* func_name =  glomstrings(2, "_nested_func_", intstring(id++));
  BlockStmt* func_def_body = fls->innerStmt->copy();
  AList<DefExpr>* func_formals = new AList<DefExpr>();
  for_alist(DefExpr, def, fls->indices) {
    ParamSymbol* param = new ParamSymbol(PARAM_BLANK, def->sym->name, def->sym->type);
    func_formals->insertAtTail(new DefExpr(param));
  }
  DefExpr* nested_func = Symboltable::defineFunction(func_name,func_formals,dtVoid,func_def_body);
  //inline these nested function calls in the iterator
  //nested_func->sym->addPragma("inline");
  return nested_func;
}

FnSymbol* CreateNestedFuncIterators::copyIteratorDef(FnSymbol* old_iterator_sym) {
  static int it_id = 1;
  DefExpr* new_formal;
  //save state
  SymScope* save_state = Symboltable::setCurrentScope(old_iterator_sym->parentScope);
  char* func_name = glomstrings(2, old_iterator_sym->name, intstring(it_id++));
  ExprStmt* old_iterator_def =  dynamic_cast<ExprStmt*>(old_iterator_sym->defPoint->parentStmt);
  AList<DefExpr>* func_formals = new AList<DefExpr>();
  //copy formals and update body of cloned iterator function
  Map<BaseAST*,BaseAST*>* update_sym_map = new Map<BaseAST*,BaseAST*>();
  for_alist(DefExpr, def, old_iterator_sym->formals) {
    new_formal = def->copy(true);
    func_formals->insertAtTail(new_formal);
    update_sym_map->put(def->sym,new_formal->sym);
  }
  BlockStmt* func_body = old_iterator_sym->body->copy(true, update_sym_map);
  DefExpr* func_def = Symboltable::defineFunction(func_name, func_formals, dtVoid, func_body);  
  //add cloned iterator
  old_iterator_def->insertBefore(new ExprStmt(func_def));
  //restore state
  Symboltable::setCurrentScope(save_state);
  return dynamic_cast<FnSymbol*>(func_def->sym);
}
