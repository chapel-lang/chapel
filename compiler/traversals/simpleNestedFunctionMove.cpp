#include "simpleNestedFunctionMove.h"
#include "removeNestedFunctions.h"
#include "stmt.h"



void SimpleNestedFunctionMove::postProcessExpr(Expr* expr) {
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    if(dynamic_cast<FnSymbol*>(defExpr->sym)){
      //nested function 
      if (RemoveNestedFunctions::hasEnclosingFunction(defExpr)) {
        Stmt* nested_func_copy = defExpr->parentStmt->copy();
        BlockStmt* encl_function_body = dynamic_cast<BlockStmt*>(defExpr->parentStmt->parentStmt);
        //move nested function definition to the front of the block
        encl_function_body->body->insertAtHead(nested_func_copy);
        defExpr->parentStmt->remove();
      } 
    }
  }
}
