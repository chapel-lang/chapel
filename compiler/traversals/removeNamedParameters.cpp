#include "removeNamedParameters.h"
#include "expr.h"

void RemoveNamedParameters::postProcessExpr(Expr* expr) {
  if (NamedExpr* named_expr = dynamic_cast<NamedExpr*>(expr)) {
    named_expr->replace(named_expr->actual->copy());
  }

  if (VarInitExpr* var_init = dynamic_cast<VarInitExpr*>(expr)) {
    var_init->replace(new AssignOp(GETS_NORM, new Variable(var_init->var), var_init->var->type->defaultVal->copy()));
  }
}
