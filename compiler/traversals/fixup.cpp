#include <string.h>
#include <typeinfo>
#include "fixup.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"


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

  if (!stmt->back || *stmt->back != stmt) {
    INT_FATAL(stmt, "stmt back incorrect");
  }
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

  if (!expr->back || *expr->back != expr) {
    INT_FATAL(expr, "expr back incorrect");
  }
}


void Fixup::preProcessSymbol(Symbol* &sym) {
  int verify = !strcmp(args, "verify");
  if (verify) {
    if (typeid(*sym) == typeid(UnresolvedSymbol)) {
      return;
    }

    SymScope* parentScope = sym->parentScope;
    if (parentScope) {
      Symbol* match = Symboltable::lookupInScope(sym->name, parentScope);
      if (match != sym) {
	bool error = true;
	FnSymbol* fnMatch = dynamic_cast<FnSymbol*>(match);
	if (fnMatch) {
	  while (fnMatch) {
	    if (fnMatch == sym) {
	      error = false;
	    }
	    fnMatch = fnMatch->overload;
	  }
	}
	if (error) {
	  INT_FATAL(sym, "Symbol '%s' and Scope don't refer to each other",
		    sym->name);
	}
      }
    }
  }
}


void Fixup::run(ModuleSymbol* moduleList) {
  ModuleSymbol* mod = moduleList;
  while (mod) {
    stmtParent.add(mod);
    mod->startTraversal(this);
    if (mod != stmtParent.pop()) {
      INT_FATAL(mod, "Major error in Fixup traversal");
    }
    mod = nextLink(ModuleSymbol, mod);
  }
}
