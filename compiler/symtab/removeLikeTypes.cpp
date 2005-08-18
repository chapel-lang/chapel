#include "removeLikeTypes.h"
#include "type.h"
#include "expr.h"

void RemoveLikeTypes::processSymbol(Symbol* sym) {
  if (!sym->defPoint) {
    return;
  }
  Expr* expr = sym->defPoint->exprType;
  if (CallExpr* parenOpExpr = dynamic_cast<CallExpr*>(expr)) {
    if (SymExpr* variable = dynamic_cast<SymExpr*>(parenOpExpr->baseExpr)) {
      if (!strcmp("typeof", variable->var->name)) {
        sym->type = parenOpExpr->argList->only()->typeInfo();
        sym->defPoint->exprType = NULL;
        if (sym->type == dtUnknown) {
          USR_FATAL(sym, "Cannot resolve type, illegal use of 'like'");
        }
      }
    }
  }
}
