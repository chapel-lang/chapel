#include "clearTypes.h"
#include "expr.h"
#include "symbol.h"

void ClearTypes::preProcessExpr(Expr* expr) {
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    defExpr->sym->type = dtUnknown;

    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
      for_alist(DefExpr, tmp, fn->formals) {
        tmp->sym->type = dtUnknown;
      }
      fn->retType = dtUnknown;
    }
  } else if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(expr)) {
    memberAccess->member = new UnresolvedSymbol(memberAccess->member->name);
  }
}
