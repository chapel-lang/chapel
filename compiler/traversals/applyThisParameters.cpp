#include "applyThisParameters.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

ApplyThisParameters::ApplyThisParameters(void) {
  CurrentClass = NULL;
}

void ApplyThisParameters::preProcessStmt(Stmt* stmt) {
  if (FnDefStmt* fds = dynamic_cast<FnDefStmt*>(stmt)) {
    if (fds->fn->classBinding) {
      CurrentClass = dynamic_cast<ClassType*>(fds->fn->classBinding->type);
    }
  }
}

void ApplyThisParameters::postProcessStmt(Stmt* stmt) {
  if (FnDefStmt* fds = dynamic_cast<FnDefStmt*>(stmt)) {
    if (fds->fn->classBinding) {
      CurrentClass = NULL;
    }
  }
}

void ApplyThisParameters::preProcessExpr(Expr* expr) {
  if (CurrentClass) {
    if (Variable* member = dynamic_cast<Variable*>(expr)) {
      if (Symboltable::lookupInScope(member->var->name, 
				     CurrentClass->classScope)) {

	/* replacement of expr variable by memberaccess */
	if (FnSymbol* parentFn =
	    dynamic_cast<FnSymbol*>(member->stmt->parentSymbol)) {
	  if (member->var == parentFn->_this) {
	    return;
	  }
	  MemberAccess* repl =
	    new MemberAccess(new Variable(parentFn->_this), member->var);
	  repl->lineno = expr->lineno;
	  repl->filename = expr->filename;
	  expr->replace(repl);
	}
	else {
	  INT_FATAL(expr, "Statement is not in method in ApplyThisParameters");
	}
      }
    }
  }
}
