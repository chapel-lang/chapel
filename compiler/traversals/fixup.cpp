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
    INT_FATAL(stmt, "NULL in Fixup()");
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


void Fixup::run(ModuleSymbol* moduleList) {
  ModuleSymbol* mod = moduleList;
  while (mod) {
    stmtParent.add(mod);
    mod->stmts->traverseList(this);
    if (mod != stmtParent.pop()) {
      INT_FATAL(mod, "Major error in Fixup traversal");
    }
    mod = nextLink(ModuleSymbol, mod);
  }
}
