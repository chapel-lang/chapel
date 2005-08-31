#include "applyThisParameters.h"
#include "expr.h"
#include "symscope.h"

void ApplyThisParameters::postProcessExpr(Expr* expr) {
  SymExpr* symExpr = dynamic_cast<SymExpr*>(expr);
  if (!symExpr)
    return;
  Symbol* sym = symExpr->var;
  FnSymbol* method = symExpr->parentFunction();
  if (!method || !method->typeBinding)
    return;
  ClassType* ct = dynamic_cast<ClassType*>(method->typeBinding->definition);
  if (!ct)
    return;
  if (dynamic_cast<UnresolvedSymbol*>(sym)) {
    forv_Vec(FnSymbol, fn, ct->methods) {
      if (!strcmp(sym->name, fn->name)) {
        expr->replace(new MemberAccess(new SymExpr(method->_this), sym));
        return;
      }
    }
  } else if (sym->parentScope && sym->parentScope->type == SCOPE_CLASS) {
    if (sym != method->_this && !dynamic_cast<TypeSymbol*>(sym)) {
      expr->replace(new MemberAccess(new SymExpr(method->_this), sym));
      return;
    }
  }
}
