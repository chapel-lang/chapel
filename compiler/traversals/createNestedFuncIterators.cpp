#include "createNestedFuncIterators.h"
#include "findEnclosingScopeVarUses.h"
#include "stringutil.h"
#include "symtab.h"
#include "symscope.h"

class InsertNestedFuncInIterator : public Traversal {
  public :
  FnSymbol* _fn_call_sym;
  AList<DefExpr>* _encl_var_formals;

  InsertNestedFuncInIterator(FnSymbol* fn_call_sym, AList<DefExpr>* encl_var_formals) {
    _fn_call_sym = fn_call_sym;
    _encl_var_formals = encl_var_formals;
  }

  void postProcessExpr(Expr* expr) {
    //replace seq yield call in iterator with call to nested function
    if (FnCall* fc = dynamic_cast<FnCall*>(expr)) {
      FnSymbol* fn_sym = fc->findFnSymbol();
      if (!strcmp(fn_sym->name, "_yield")) {
        AList<Expr>* new_arg_list = getNewArgList(fc->argList);
        FnCall* fn_call = new FnCall(new Variable(_fn_call_sym), new_arg_list->copy());
        fc->replace(fn_call);
      }
    }
  }

  void postProcessStmt(Stmt* stmt) {
    if (ReturnStmt* ret = dynamic_cast<ReturnStmt*>(stmt))
      //iterator now has a void return
      ret->expr->remove();  
  }
  
  AList<Expr>* getNewArgList (AList<Expr>* old_arg_list) {
    AList<Expr>* new_arg_list = old_arg_list->copy();
    //first argument to seq yield is the sequence, second is the element 
    //returned to the old iterator for loop
    new_arg_list->popHead();
    //add local var uses of enclosing scope found in loop body that 
    //were passed to the iterator function def
    for_alist(DefExpr, def, _encl_var_formals)
      new_arg_list->insertAtTail(new Variable(def->sym));
   
    return new_arg_list;
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
        //visit the body of the iterator forloop first to transform
        //inner iterator for loops first
        fls->innerStmt->traverse(this);
          
        //copy iterator function def
        Vec<Symbol*>* encl_scope_var_uses = getEnclosingScopeVarUses(fls);
        FnSymbol* func_it_sym = copyIteratorDef(fn_sym);
        AList<DefExpr>* encl_var_formals = addEnclVarFormals(func_it_sym, encl_scope_var_uses, new Map<BaseAST*,BaseAST*>());
        //insert nested function created using the body of the iterator loop
        
        ParenOpExpr* new_func_call = new FnCall(new Variable(func_it_sym), paren_op->argList->copy());
        addFuncActuals(new_func_call, encl_scope_var_uses);
        paren_op->parentStmt->insertBefore(new ExprStmt(new_func_call));
        //place body of for loop in a nested function definition
        DefExpr* nested_func_def = copyLoopBodyToNestedFuncDef(fls, encl_scope_var_uses);
        FnSymbol* fn_call_sym = dynamic_cast<FnSymbol*>(nested_func_def->sym);
       
        //replace calls to seq append in iterator body with nested function call
        func_it_sym->body->traverse(new InsertNestedFuncInIterator(fn_call_sym, encl_var_formals));
        paren_op->parentStmt->remove();
        }
    }
}

Vec<Symbol*>* CreateNestedFuncIterators::getEnclosingScopeVarUses(ForLoopStmt* fls) {
  //find local variable uses in the loop body becoming to an enclosing scope and add 
  //formal parameters to the iterator function for each variable
  if (fls->parentScope->type != SCOPE_MODULE) {
     FindEnclosingScopeVarUses* fesv = new FindEnclosingScopeVarUses(fls->parentScope);
     fls->innerStmt->traverse(fesv);
     return fesv->getVarUses();
  }
  return NULL;
}

AList<DefExpr>* CreateNestedFuncIterators::addEnclVarFormals(FnSymbol* fn_sym, Vec<Symbol*>* encl_scope_var_uses, 
                                                             Map<BaseAST*,BaseAST*>* update_map) {
  AList<DefExpr>* encl_var_formals;
  
  //found local var uses of an encl scope
  if (encl_scope_var_uses && encl_scope_var_uses->length()) {
    //save state
    SymScope* save_state = Symboltable::setCurrentScope(fn_sym->parentScope);
    encl_var_formals = new AList<DefExpr>();
    forv_Vec(Symbol, sym, *encl_scope_var_uses) {
      if (sym) {
        //create formal and add to nested function
        DefExpr* formal = Symboltable::defineParam(PARAM_BLANK,sym->name,NULL,NULL);
        formal->sym->type = sym->type;
        fn_sym->formals->insertAtTail(formal);
        encl_var_formals->insertAtTail(formal->copy());
        //will need to perform an update to map enclosing variables to formals
        update_map->put(sym, formal->sym);
      }
    }
    fn_sym->body->traverse(new UpdateSymbols(update_map, NULL));
    //restore state
    Symboltable::setCurrentScope(save_state); 
    return encl_var_formals;
  }
  else {
    //still may need to update symbols in body due to the formals created from
    //indices of the iterator for loop converted to a function definition
    fn_sym->body->traverse(new UpdateSymbols(update_map, NULL));
    return NULL;
  }
}

void CreateNestedFuncIterators::addFuncActuals(ParenOpExpr* paren_op, Vec<Symbol*>* encl_scope_var_uses) {
  //build iterator function actuals list
  forv_Vec(Symbol, sym, *encl_scope_var_uses) {
    if (sym) 
      paren_op->argList->insertAtTail(new Variable(sym));
  }
}

DefExpr* CreateNestedFuncIterators::copyLoopBodyToNestedFuncDef(ForLoopStmt* fls,Vec<Symbol*>* encl_scope_var_uses) {
  //create a nested function definition
  static int id = 1;
  char* func_name =  glomstrings(2, "_nested_func_", intstring(id++));
  BlockStmt* func_def_body = fls->innerStmt->copy();
  AList<DefExpr>* func_formals = new AList<DefExpr>();

  //add to global scope
  ModuleSymbol* curr_module = fls->indexScope->getModule();
  AList<Stmt>* module_stmts = curr_module->stmts;
  SymScope* saveScope = Symboltable::setCurrentScope(curr_module->modScope);
  Map<BaseAST*,BaseAST*>* update_map = new Map<BaseAST*,BaseAST*>();
  for_alist(DefExpr, def, fls->indices) {
    ParamSymbol* param = new ParamSymbol(PARAM_BLANK, def->sym->name, def->sym->type);
    func_formals->insertAtTail(new DefExpr(param));
    update_map->put(def->sym,param);
  }
  DefExpr* nested_func = Symboltable::defineFunction(func_name,func_formals,dtVoid,func_def_body);
  //add formals for each local variable uses that was defined in an enclosing scope
  addEnclVarFormals(dynamic_cast<FnSymbol*>(nested_func->sym), encl_scope_var_uses,update_map);
  module_stmts->insertAtTail(new ExprStmt(nested_func));

  //to inline these nested function calls in the iterator, uncomment next line
  nested_func->sym->addPragma("inline");

  //restore scope
  Symboltable::setCurrentScope(saveScope);
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
  //copy iterator function
  BlockStmt* func_body = old_iterator_sym->body->copy(true, update_sym_map);
  DefExpr* func_def = Symboltable::defineFunction(func_name, func_formals, dtVoid, func_body);  
  //add cloned iterator
  old_iterator_def->insertBefore(new ExprStmt(func_def));
  //restore state
  Symboltable::setCurrentScope(save_state);
  return dynamic_cast<FnSymbol*>(func_def->sym);
}
