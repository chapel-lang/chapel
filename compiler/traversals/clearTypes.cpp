#include "clearTypes.h"
#include "expr.h"
#include "symbol.h"

void ClearTypes::preProcessExpr(Expr* expr) {
  DefExpr* defExpr = dynamic_cast<DefExpr*>(expr);

  if (!defExpr) {
    return;
  }

  defExpr->sym->type = dtUnknown;

  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
    for (Symbol* tmp = fn->formals; tmp; tmp = nextLink(Symbol, tmp)) {
      tmp->type = dtUnknown;
    }
    fn->retType = dtUnknown;
  }
}
