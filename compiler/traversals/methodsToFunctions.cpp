#include <typeinfo>
#include "methodsToFunctions.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"


/***
 ***  Mangles names of methods with "_"
 ***/
void MethodsToFunctions::preProcessStmt(Stmt* stmt) {
  TypeDefStmt* tds;
  ClassType* ctype;

  if (tds = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (ctype = dynamic_cast<ClassType*>(tds->type)) {
      forv_Vec(FnSymbol, fn, ctype->primaryMethods) {
	fn->cname = glomstrings(4, "_", ctype->name->name, "_", fn->name);
      }
      forv_Vec(FnSymbol, fn, ctype->secondaryMethods) {
	fn->cname = glomstrings(4, "_", ctype->name->name, "_", fn->name);
      }
    }
  }
}


/***
 ***  Adds base in MemberAccess for method to a new first parameter to
 ***  the method, makes MemberAccess for method a function on its own
 ***/
void MethodsToFunctions::preProcessExpr(Expr* expr) {
  if (ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr)) {
    if (typeid(*paren) == typeid(ParenOpExpr)) {
      if (MemberAccess* method = dynamic_cast<MemberAccess*>(paren->baseExpr)) {
	if (dynamic_cast<FnSymbol*>(method->member)) {
	  Expr* newArgList = method->base;
	  newArgList->append(paren->argList);
	  FnCall* fn = new FnCall(new Variable(method->member), newArgList);
	  expr->replace(fn);
	}
      }
    }
  }
}
