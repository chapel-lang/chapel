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
  } else if (MemberAccess* method = dynamic_cast<MemberAccess*>(expr)) {
    if (dynamic_cast<FnCall*>(method->parent)) {
      return;
    }
    if (dynamic_cast<FnSymbol*>(method->member)) {
      FnCall* fn = new FnCall(new Variable(method->member), method->base);
      expr->replace(fn);
    }
  }
}
