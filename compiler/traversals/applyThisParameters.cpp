#include "applyThisParameters.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


void ApplyThisParameters::postProcessExpr(Expr* expr) {
  if (Variable* member = dynamic_cast<Variable*>(expr)) {
    if (member->parentStmt &&
        member->parentStmt->parentFunction() &&
        member->parentStmt->parentFunction()->typeBinding) {
      FnSymbol* currentFunction = member->parentStmt->parentFunction();
      if (member->var->parentScope &&
          member->var->parentScope->type == SCOPE_CLASS) {
        if (FnSymbol* constructor = dynamic_cast<FnSymbol*>(member->var)) {
          if (constructor->fnClass == FN_CONSTRUCTOR) {
            return;
          }
        }
        if (member->var == currentFunction->_this) {
          return;
        }
        if (dynamic_cast<TypeSymbol*>(member->var)) {
          return;
        }
        Variable* base = new Variable(currentFunction->_this);
        MemberAccess* memberAccess = new MemberAccess(base, member->var);
        memberAccess->lineno = expr->lineno;
        memberAccess->filename = expr->filename;
        expr->replace(memberAccess);
      }
    }
  }
}
