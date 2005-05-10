#include "removeNamedParameters.h"
#include "expr.h"
#include "stmt.h"

void RemoveNamedParameters::postProcessExpr(Expr* expr) {
  if (NamedExpr* named_expr = dynamic_cast<NamedExpr*>(expr)) {
    named_expr->replace(named_expr->actual->copy());
  }

  if (VarInitExpr* var_init = dynamic_cast<VarInitExpr*>(expr)) {
    if (var_init->typeInfo() == dtVoid ||
        dynamic_cast<ArrayType*>(var_init->typeInfo()) ||
        dynamic_cast<DomainType*>(var_init->typeInfo())) {
      // No Default Initialization for Arrays/Domains
      var_init->parentStmt->replace(new NoOpStmt());
    } else {
      if (var_init->typeInfo()->defaultVal) {
        var_init->replace(var_init->typeInfo()->defaultVal->copy());
      } else if (var_init->typeInfo()->defaultConstructor) {
        var_init->replace(new FnCall(new Variable(var_init->typeInfo()->defaultConstructor), NULL));
      } else {
        INT_FATAL(expr, "VarInitExpr has no default initialization");
      }
    }
  }
}
