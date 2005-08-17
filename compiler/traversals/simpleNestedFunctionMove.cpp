#include "simpleNestedFunctionMove.h"
#include "removeNestedFunctions.h"
#include "stmt.h"


void SimpleNestedFunctionMove::postProcessStmt(Stmt* stmt) {
  if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
      if(dynamic_cast<FnSymbol*>(defExpr->sym)){
        //nested function 
        if (FnSymbol* encl_func = RemoveNestedFunctions::hasEnclosingFunction(defExpr)) {
          //move nested function definition to the front of the block
          stmt->remove();
          encl_func->body->body->insertAtHead(stmt);
        }
      }
    }
  }
}
