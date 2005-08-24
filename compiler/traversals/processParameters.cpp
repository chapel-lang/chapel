#include <typeinfo>
#include "expr.h"
#include "processParameters.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


ProcessParameters::ProcessParameters(void) {
  whichModules = MODULES_CODEGEN;
}


static bool tmpRequired(ArgSymbol* formal, Expr* actual) {
  if (SymExpr* var = dynamic_cast<SymExpr*>(actual)) {
    if (dynamic_cast<TypeSymbol*>(var->var))
      return false;
  }
  if (formal->type != dtUnknown) {
    return formal->requiresCTmp();
  } else {
    return actual->typeInfo()->requiresCParamTmp(formal->intent);
  }
}


void ProcessParameters::postProcessExpr(Expr* expr) {
  static int uid = 0;
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (call->opTag != OP_NONE) {
      return;
    }
    Stmt* stmt = expr->getStmt();
    FnSymbol* fn = call->findFnSymbol();
    DefExpr* formal_def = fn->formals->first();
    for_alist(Expr, actual, call->argList) {
      ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formal_def->sym);
      if (!formal || !actual) {
        INT_FATAL(expr, "Formal/actual mismatch in ProcessParameters");
      }
      if (tmpRequired(formal, actual)) {
        Expr* init = NULL;
        if (formal->intent != INTENT_OUT) {
          init = actual->copy();
        } else {
          init = COPY(formal->type->defaultValue);
        }
        char* tmp_name = glomstrings(2, "_argtmp", intstring(uid++));
        VarSymbol* tmp = new VarSymbol(tmp_name, formal->type);
        tmp->noDefaultInit = true;
        DefExpr* tmp_def = new DefExpr(tmp, init);
        stmt->insertBefore(new ExprStmt(tmp_def));
        actual->replace(new SymExpr(tmp));
        if (formal->requiresCopyBack()) {
          stmt->insertAfter(new ExprStmt(new CallExpr(OP_GETSNORM, actual, new SymExpr(tmp))));
        }
      }
      formal_def = fn->formals->next();
    }
  }
}
