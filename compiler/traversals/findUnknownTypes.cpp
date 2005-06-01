#include "findUnknownTypes.h"
#include "expr.h"
#include "symbol.h"
#include "stmt.h"


void RemoveTypeVariableActuals::preProcessExpr(Expr* expr) {
  if (FnCall* call = dynamic_cast<FnCall*>(expr)) {
    Expr* arg = call->argList->first();
    while (arg) {
      Expr* next_arg = call->argList->next();
      if (Variable* var_arg = dynamic_cast<Variable*>(arg)) {
        if (dynamic_cast<TypeSymbol*>(var_arg->var)) {
          arg->remove();
        } else if (ParamSymbol *p = dynamic_cast<ParamSymbol*>(var_arg->var)) {
          if (p->typeVariable)
            arg->remove();
        }
      }
      arg = next_arg;
    }
  }
}


void RemoveTypeVariableFormals::preProcessSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    ParamSymbol* old_formal = fn->formals->popHead();
    AList<ParamSymbol>* new_formals = new AList<ParamSymbol>();
    while (old_formal) {
      if (old_formal->typeVariable) {
        fn->body->body->insertBefore(new DefStmt(new DefExpr(old_formal)));
      } else {
        new_formals->add(old_formal);
      }
      old_formal = fn->formals->popHead();
    }
    fn->formals = new_formals;
  }
}
