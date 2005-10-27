#include "findUnknownTypes.h"
#include "expr.h"
#include "symbol.h"
#include "stmt.h"
#include "symtab.h"


void RemoveTypeVariableActuals::preProcessExpr(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (SymExpr* variable = dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (variable->var->hasPragma("keep types")) {
        return;
      }
    }
    Expr* arg = call->argList->first();
    while (arg) {
      Expr* next_arg = call->argList->next();
      if (SymExpr* var_arg = dynamic_cast<SymExpr*>(arg)) {
        if (dynamic_cast<TypeSymbol*>(var_arg->var)) {
          arg->remove();
        } else if (ArgSymbol *p = dynamic_cast<ArgSymbol*>(var_arg->var)) {
          if (p->genericSymbol)
            arg->remove();
        } else if (var_arg->var == Symboltable::lookupInternal("_methodToken")) {
          arg->remove();
        } else if (var_arg->var == Symboltable::lookupInternal("_setterToken")) {
          arg->remove();
        }
      }
      arg = next_arg;
    }
  }
}


void RemoveTypeVariableFormals::preProcessSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (fn->hasPragma("keep types")) {
      return;
    }
    for_alist(DefExpr, formal, fn->formals) {
      if (dynamic_cast<ArgSymbol*>(formal->sym)->genericSymbol) {
        formal->remove();
      } else if (formal->sym->type == dtMethodToken) {
        formal->remove();
      } else if (formal->sym->type == dtSetterToken) {
        formal->remove();
      }
    }
  }
}
