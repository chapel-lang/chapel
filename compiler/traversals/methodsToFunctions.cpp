#include <typeinfo>
#include "methodsToFunctions.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "type.h"


/***
 ***  Mangles names of methods with "_"
 ***/
void MethodsToFunctions::preProcessStmt(Stmt* stmt) {
  TypeDefStmt* tds;
  ClassType* ctype;

  if (tds = dynamic_cast<TypeDefStmt*>(stmt)) {
    /** test cloning of classes
    if (!tds->type->name->parentScope->isInternal()) {
      Map<BaseAST*,BaseAST*> map;
      tds->clone(NULL, &map);
    }
    **/
    if (ctype = dynamic_cast<ClassType*>(tds->type)) {
      forv_Vec(FnSymbol, fn, ctype->methods) {
	fn->cname = glomstrings(4, "_", ctype->name->cname, "_", fn->cname);
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
