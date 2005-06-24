#include "replaceReturns.h"
#include "stmt.h"



ReplaceReturns::ReplaceReturns(Symbol* sym){_sym = sym;}

void ReplaceReturns::postProcessStmt(Stmt* stmt) {
  if (ReturnStmt* ret_stmt = dynamic_cast<ReturnStmt*>(stmt)) {
    //replace return with an assignment to temporary variable
    AssignOp* assign = new AssignOp(GETS_NORM, new Variable(_sym), ret_stmt->expr);
    ret_stmt->replace(new ExprStmt(assign));
  }
}


