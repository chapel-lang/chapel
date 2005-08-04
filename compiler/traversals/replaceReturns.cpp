#include "replaceReturns.h"
#include "stmt.h"



ReplaceReturns::ReplaceReturns(Symbol* sym){_sym = sym;}

void ReplaceReturns::postProcessStmt(Stmt* stmt) {
  if (ReturnStmt* ret_stmt = dynamic_cast<ReturnStmt*>(stmt)) {
    //replace return with an assignment to temporary variable
    ret_stmt->replace(new ExprStmt(
                        new CallExpr(OP_GETSNORM,
                                        new Variable(_sym),
                                        ret_stmt->expr)));
  }
}


