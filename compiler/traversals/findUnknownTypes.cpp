#include "findUnknownTypes.h"
#include "expr.h"
#include "symbol.h"
#include "stmt.h"


void RemoveTypeVariableActuals::preProcessExpr(Expr* expr) {
  if (FnCall* call = dynamic_cast<FnCall*>(expr)) {
    Expr* arg = call->argList;
    while (arg) {
      Expr* next_arg = nextLink(Expr, arg);
      if (Variable* var_arg = dynamic_cast<Variable*>(arg)) {
        if (dynamic_cast<TypeSymbol*>(var_arg->var)) {
          arg->extract();
        }
      }
      arg = next_arg;
    }
  }
}


void RemoveTypeVariableFormals::preProcessSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    Symbol* old_formals = fn->formals;
    Symbol* new_formals = NULL;
    while (old_formals) {
      Symbol* next_old_formals = nextLink(Symbol, old_formals);
      old_formals->next = NULL;
      old_formals->prev = NULL;
      if (dynamic_cast<ParamSymbol*>(old_formals)) {
        new_formals = appendLink(new_formals, old_formals);
      } else {
        old_formals->next = 0;
        fn->body->body->insertBefore(new DefStmt(new DefExpr(old_formals)));
      }
      old_formals = next_old_formals;
    }
    fn->formals = new_formals;
  }
}
