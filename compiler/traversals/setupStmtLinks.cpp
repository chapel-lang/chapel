#include "expr.h"
#include "setupStmtLinks.h"
#include "stmt.h"

SetupStmtLinks::SetupStmtLinks(void) {
  stmts.add(NULL);
}


void SetupStmtLinks::preProcessStmt(Stmt* stmt) {
  stmts.add(stmt);
}


void SetupStmtLinks::postProcessStmt(Stmt* stmt) {
  stmts.pop();
}


void SetupStmtLinks::preProcessExpr(Expr* expr) {
  Stmt* parentStmt = stmts.v[stmts.n-1];
  if (parentStmt == NULL) {
    INT_FATAL(expr, "NULL in SetupStmtLinks()");
  }

  expr->stmt = parentStmt;
}
