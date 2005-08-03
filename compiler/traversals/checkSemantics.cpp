#include "checkSemantics.h"
#include "expr.h"

static void checkFnCall(FnCall* fncall) {
  
}

static void checkAssignOp(AssignOp* assign) {
  if (assign->left->isConst() || assign->left->isParam()) {
    USR_FATAL(assign, "Assigning to a constant expression");
  }
}

void CheckSemantics::preProcessExpr(Expr* expr) {
  FnCall* fncall = dynamic_cast<FnCall*>(expr);
  if (fncall) checkFnCall(fncall);

  AssignOp* assign = dynamic_cast<AssignOp*>(expr);
  if (assign){
    //printf("Assignment expression.\n"); 
    if (!dynamic_cast<DefExpr*>(expr->parentExpr)) { // initialization okay
      checkAssignOp(assign);
    }
  }

  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    if (defExpr->sym->isParam()) {
      if (defExpr->init && !dynamic_cast<IntLiteral*>(defExpr->init)) {
        USR_FATAL(defExpr, "Initializing parameter to a variable expression.");
      }
    }
  }
}

