#include "checkSemantics.h"
#include "expr.h"

static void checkFnCall(FnCall* fncall) {
  
}


static void checkAssignOp(AssignOp* assign) {
  if (assign->left->isConst()) {
    USR_FATAL(assign, "Assigning to a constant expression");
  }
}


void CheckSemantics::preProcessExpr(Expr* expr) {
  FnCall* fncall = dynamic_cast<FnCall*>(expr);
  if (fncall) checkFnCall(fncall);

  AssignOp* assign = dynamic_cast<AssignOp*>(expr);
  if (assign) checkAssignOp(assign);
}

