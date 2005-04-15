#include "removeNamedParameters.h"
#include "expr.h"
#include "stmt.h"

void RemoveNamedParameters::postProcessExpr(Expr* expr) {
  if (NamedExpr* named_expr = dynamic_cast<NamedExpr*>(expr)) {
    named_expr->replace(named_expr->actual->copy());
  }

  if (VarInitExpr* var_init = dynamic_cast<VarInitExpr*>(expr)) {
    if (dynamic_cast<ArrayType*>(var_init->symbol->type) ||
        dynamic_cast<DomainType*>(var_init->symbol->type)) {
      // No Default Initialization for Arrays/Domains
      var_init->parentStmt->replace(new NoOpStmt());
    } else {
      if (var_init->symbol->type->defaultVal) {
        var_init->replace(var_init->symbol->type->defaultVal->copy());
      } else {
        var_init->replace(new FnCall(new Variable(var_init->symbol->type->defaultConstructor), NULL));
      }
    }
  }
}
