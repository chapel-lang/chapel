#include "methodsToFunctions.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"


/***
 ***  Mangles names of methods with "_"
 ***  Moves method's function definition statement after class definition statement
 ***/
void MethodsToFunctions::preProcessStmt(Stmt* stmt) {
  TypeDefStmt* tds;
  ClassType* ctype;

  if (tds = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (ctype = dynamic_cast<ClassType*>(tds->type)) {
      Stmt* stmt = ctype->definition;

      while (stmt) {
	Stmt* next = nextLink(Stmt, stmt);
	if (FnDefStmt* method = dynamic_cast<FnDefStmt*>(stmt)) {
	  /* Done before analysis for now -SJD
	    VarSymbol* this_insert = new VarSymbol("_this", ctype); THIS NOT CORRECT
	    this_insert->next = method->fn->formals;                COPY BACK IF COPYING BACK
	    method->fn->formals = this_insert;                      FROM fieldsToMemberAccesses
	  */
	  method->fn->cname = glomstrings(4, "_", ctype->name->name, "_", method->fn->name);
	  method->next = tds->next;
	  tds->next = method;

	  /***  SJD: DON'T do this here unless this should be put in after
		analysis (for not it is its own pass before analysis
	      CurrentClass= ctype;
	      FieldsToMemberAccesses* FTMA = new FieldsToMemberAccesses();
	      method->fn->body->traverse(FTMA);
	  ***/
	}
	stmt = next;
      }
    }
  }
}


/***
 ***  Adds base in MemberAccess for method to a new first parameter to the method
 ***  Makes MemberAccess for method a function on its own
 ***/
void MethodsToFunctions::preProcessExpr(Expr* expr) {
  MemberAccess* method;
  ParenOpExpr* function;
  Expr* newArgList;

  if (method = dynamic_cast<MemberAccess*>(expr)) {
    if (dynamic_cast<FnSymbol*>(method->member)) {
      if (function = dynamic_cast<ParenOpExpr*>(method->parent)) {
	newArgList = method->base->copy();
	newArgList->append(function->argList);
	function->argList = newArgList;
	function->baseExpr = new Variable(method->member);
      }
      else {
	INT_FATAL(expr, "Bad Method Call?");
      }
    }
  }
}
