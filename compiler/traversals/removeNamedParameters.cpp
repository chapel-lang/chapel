#include "removeNamedParameters.h"
#include "expr.h"
#include "stmt.h"

void RemoveNamedParameters::postProcessExpr(Expr* expr) {
  if (NamedExpr* named_expr = dynamic_cast<NamedExpr*>(expr)) {
    named_expr->replace(named_expr->actual->copy());
  }

  if (VarInitExpr* var_init = dynamic_cast<VarInitExpr*>(expr)) {
    if (dynamic_cast<ArrayType*>(var_init->expr->typeInfo()) ||
        dynamic_cast<DomainType*>(var_init->expr->typeInfo())) {
      // No Default Initialization for Arrays/Domains
      var_init->parentStmt->replace(new NoOpStmt());
    } else {
      var_init->replace(new AssignOp(GETS_NORM, var_init->expr->copy(), var_init->expr->typeInfo()->defaultVal->copy()));
    }
  }
}
