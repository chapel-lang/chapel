#include "fixup.h"
#include "stmt.h"
#include "expr.h"

Fixup::Fixup(void) {
  exprParent.add(NULL);
  stmtParent.add(NULL);
}


void Fixup::preProcessSymbol(Symbol* symbol) {
  stmtParent.add(symbol);
}


void Fixup::postProcessSymbol(Symbol* symbol) {
  stmtParent.pop();
}


void Fixup::preProcessType(Type* type) {
  stmtParent.add(type->name);
}


void Fixup::postProcessType(Type* type) {
  stmtParent.pop();
}


void Fixup::preProcessStmt(Stmt* stmt) {
  Symbol* tmp = stmtParent.v[stmtParent.n-1];
  if (tmp == NULL) {
    /** SJD: We want this eventually, but right now it seems that were
	not in a module to point back to.  In this case let's set the
	stmt parent to NilSymbol

    INT_FATAL(stmt, "NULL in Fixup()");

     **/
    stmt->parentSymbol = nilSymbol;
  }
  stmt->parentSymbol = tmp;

  if (FnDefStmt* fstmt = dynamic_cast<FnDefStmt*>(stmt)) {
    stmtParent.add(fstmt->fn);
  }

  exprParent.add(stmt);
}


void Fixup::postProcessStmt(Stmt* stmt) {
  if (dynamic_cast<FnDefStmt*>(stmt)) {
    stmtParent.pop();
  }

  exprParent.pop();
}


void Fixup::preProcessExpr(Expr* expr) {
  Stmt* tmp = exprParent.v[exprParent.n-1];
  if (tmp == NULL) {
    INT_FATAL(expr, "NULL in Fixup()");
  }
  expr->stmt = tmp;
}
