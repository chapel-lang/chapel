#include "fieldsToMemberAccesses.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"


/***
 ***  Change fields and methods within class methods to this.field
 ***  Adds this to formal argument list for methods
 ***/


static ClassType* CurrentClass = NULL;


void FieldsToMemberAccesses::preProcessStmt(Stmt* stmt) {
  if (TypeDefStmt* tds = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (ClassType* ctype = dynamic_cast<ClassType*>(tds->type)) {
      CurrentClass = ctype;
      Stmt* stmt = ctype->definition;

      while (stmt) {
	Stmt* next = nextLink(Stmt, stmt);
	if (FnDefStmt* method = dynamic_cast<FnDefStmt*>(stmt)) {
	  Symbol* this_insert = new ParamSymbol(PARAM_INOUT, "this", ctype);
	  Symboltable::defineInScope(this_insert, method->fn->scope);
	  this_insert = appendLink(this_insert, method->fn->formals);
	  method->fn->formals = this_insert;
	}
	stmt = next;
      }
    }
  }
}


void FieldsToMemberAccesses::postProcessStmt(Stmt* stmt) {
  if (TypeDefStmt* tds = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (dynamic_cast<ClassType*>(tds->type)) {
      CurrentClass = NULL;
    }
  }
}


void FieldsToMemberAccesses::preProcessExpr(Expr* expr) {
  if (CurrentClass) {
    if (Variable* member = dynamic_cast<Variable*>(expr)) {
      if (Symboltable::lookupInScope(member->var->name, CurrentClass->scope)) {

	/* replacement of expr variable by memberaccess */
	if (FnSymbol* parentFn = dynamic_cast<FnSymbol*>(member->stmt->parentSymbol)) {
	  MemberAccess* repl = new MemberAccess(new Variable(parentFn->formals),
						member->var);
	  expr->parent->replace(expr, repl);
	}
	else {
	  INT_FATAL(expr, "Statement is not in method in FieldsToMemberAccesses");
	}
      }
    }
  }
}
