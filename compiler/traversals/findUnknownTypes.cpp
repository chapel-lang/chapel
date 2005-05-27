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
          arg->extract();
        } else if (ParamSymbol *p = dynamic_cast<ParamSymbol*>(var_arg->var)) {
          if (p->typeVariable)
            arg->extract();
        }
      }
      arg = next_arg;
    }
  }
}


void RemoveTypeVariableFormals::preProcessSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    Symbol* old_formal = fn->formals->popHead();
    AList<Symbol>* new_formals = new AList<Symbol>();
    while (old_formal) {
      if (dynamic_cast<ParamSymbol*>(old_formal) &&
          dynamic_cast<ParamSymbol*>(old_formal)->typeVariable) {
        fn->body->body->insertBefore(new DefStmt(new DefExpr(old_formal)));
      } else {
        new_formals->add(old_formal);
      }
      old_formal = fn->formals->popHead();
    }
    fn->formals = new_formals;
  }
}
