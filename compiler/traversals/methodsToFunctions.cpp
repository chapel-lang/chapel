#include "methodsToFunctions.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

void MethodsToFunctions::preProcessStmt(Stmt* stmt) {

}


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
      }
      else {
	INT_FATAL(expr, "Bad Method Call?");
      }
    }
  }
}


void MethodsToFunctions::preProcessSymbol(Symbol* symbol) {

}


void MethodsToFunctions::preProcessType(Type* type) {

}


void MethodsToFunctions::run(ModuleSymbol* moduleList) {
  Traversal::run(moduleList);



}
