#include "fixup.h"
#include "ast_util.h"
#include "expr.h"
#include "stmt.h"
#include <string.h>


Fixup::Fixup(void) {
  exprParent.add(NULL);
  stmtParent.add(NULL);
}


void Fixup::preProcessStmt(Stmt* &stmt) {
  int verify = !strcmp(args, "verify");
  Symbol* tmp = stmtParent.v[stmtParent.n-1];
  if (tmp == NULL) {
    INT_FATAL(stmt, "NULL in Fixup()");
  }
  if (!verify) {
    stmt->parentSymbol = tmp;
  }
  else if (stmt->parentSymbol != tmp) {
    INT_FATAL(stmt, "Error during verification fixup: statement's parent is incorrect");
  }

  if (FnDefStmt* fstmt = dynamic_cast<FnDefStmt*>(stmt)) {
    stmtParent.add(fstmt->fn);
  }

  if (TypeDefStmt* tstmt = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (ClassType* ctype = dynamic_cast<ClassType*>(tstmt->type)) {
      stmtParent.add(ctype->name);
    }
  }

  exprParent.add(stmt);
}


void Fixup::postProcessStmt(Stmt* &stmt) {
  if (dynamic_cast<FnDefStmt*>(stmt)) {
    stmtParent.pop();
  }

  if (TypeDefStmt* tstmt = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (dynamic_cast<ClassType*>(tstmt->type)) {
      stmtParent.pop();
    }
  }

  exprParent.pop();
}


void Fixup::preProcessExpr(Expr* &expr) {
  int verify = !strcmp(args, "verify");
  Stmt* tmp = exprParent.v[exprParent.n-1];
  if (tmp == NULL) {
    INT_FATAL(expr, "NULL in Fixup()");
  }
  if (!verify) {
    expr->stmt = tmp;
  }
  else if (expr->stmt != tmp) {
    INT_FATAL(expr, "Error during verification fixup: expression's statement is incorrect");
  }
}


void Fixup::run(ModuleSymbol* moduleList) {
  ModuleSymbol* mod = moduleList;
  while (mod) {
    stmtParent.add(mod);
    TRAVERSE_LS(mod->stmts, this, true);
    if (mod != stmtParent.pop()) {
      INT_FATAL(mod, "Major error in Fixup traversal");
    }
    mod = nextLink(ModuleSymbol, mod);
  }
}
