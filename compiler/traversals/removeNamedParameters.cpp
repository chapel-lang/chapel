#include "removeNamedParameters.h"
#include "expr.h"
#include "stmt.h"

void RemoveNamedParameters::postProcessExpr(Expr* expr) {
  if (NamedExpr* named_expr = dynamic_cast<NamedExpr*>(expr)) {
    named_expr->replace(named_expr->actual->copy());
  }

  if (DefExpr* def = dynamic_cast<DefExpr*>(expr))
    if (def->init) {
      Type *type = def->sym->type;
      if (type == dtVoid)
        def->init->parentStmt->remove();
      else if (SymExpr* var = dynamic_cast<SymExpr*>(def->init)) {
        if (var->var == gNil) {
          if (type->defaultValue)
            def->init->replace(type->defaultValue->copy());
          else if (type->defaultConstructor)
            def->init->replace(new CallExpr(type->defaultConstructor));
          else if (analyzeAST) {
            INT_FATAL(expr, "nil assigned to Type with no default initialization");
          }
        }
      }
    }

  if (CallExpr* p = dynamic_cast<CallExpr*>(expr)) {
    if (p->opTag == OP_GETSNORM) {
      Expr *lhs = p->get(1);
      Expr *rhs = p->get(2);
      Type *type = lhs->typeInfo();
      if (!is_Reference_Type(type)) {
        if (SymExpr* var = dynamic_cast<SymExpr*>(rhs)) {
          if (var->var == gNil) {
            if (type->defaultValue)
              rhs->replace(type->defaultValue->copy());
            else if (type->defaultConstructor)
              rhs->replace(new CallExpr(type->defaultConstructor));
            else if (analyzeAST) {
              INT_FATAL(expr, "nil assigned to Type with no default initialization");
            }
          }
        }
      }
    }
  }
}
