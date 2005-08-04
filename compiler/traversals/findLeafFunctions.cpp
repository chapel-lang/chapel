#include <stdio.h>
#include "findLeafFunctions.h"
#include "expr.h"

void FindLeafFunctions::preProcessExpr(Expr* expr) {
  //not handling nested function definitions yet
  if(DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)){
    FnSymbol* func_sym = dynamic_cast<FnSymbol*>(def_expr->sym);
    if (func_sym)
      _inside_leaf_func = true;
  }
  //leaf functions cannot have function calls 
  else {
    ParenOpExpr* fn_call = dynamic_cast<ParenOpExpr*>(expr);
    if (fn_call)
      _inside_leaf_func = false;
  }
}

void FindLeafFunctions::postProcessExpr(Expr* expr) {
  if(DefExpr* def_expr = dynamic_cast<DefExpr*>(expr))
    if(FnSymbol* func_sym = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      //Finished traversing function definition, is it a leaf function?
      if (_inside_leaf_func)
        //add to list of leaf nodes
        addLeaf(func_sym);
      _inside_leaf_func = false;
    }
}

void FindLeafFunctions::addLeaf(FnSymbol* fsym) { 
  FuncLeaves* tmp_leaf = new FuncLeaves;
  tmp_leaf->first = fsym;
  tmp_leaf->next = _leaf_functions;
  _leaf_functions = tmp_leaf;
}

void FindLeafFunctions::printLeaves() {
  FuncLeaves* tmp_leaf = _leaf_functions;
  printf("PRINTING FUNCTION LEAVES \n"); 
  while(tmp_leaf) {
    printf("function leaf: %s \n", tmp_leaf->first->name); 
    tmp_leaf = tmp_leaf->next;
  }
  printf("DONE PRINTING FUNCTION LEAVES \n"); 
}

bool FindLeafFunctions::isLeafFunction(FnSymbol* fs) {
  FuncLeaves* temp_func_leaves = _leaf_functions;
  while(temp_func_leaves) {
    if (fs == temp_func_leaves->first)
      return true;
    temp_func_leaves = temp_func_leaves->next;
  }
  return false;
}

FindLeafFunctions::~FindLeafFunctions() {
  FuncLeaves* tmp = _leaf_functions;
  FuncLeaves* delete_leaf;
  while(tmp) {
    delete_leaf = tmp;
    tmp = tmp->next;
    delete delete_leaf;
  }
}
