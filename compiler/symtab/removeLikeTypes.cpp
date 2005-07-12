#include "removeLikeTypes.h"
#include "type.h"
#include "expr.h"

void RemoveLikeTypes::processSymbol(Symbol* sym) {
  if (!sym->defPoint) {
    return;
  }
  Expr* expr = sym->defPoint->exprType;
  if (ParenOpExpr* parenOpExpr = dynamic_cast<ParenOpExpr*>(expr)) {
    if (Variable* variable = dynamic_cast<Variable*>(parenOpExpr->baseExpr)) {
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
