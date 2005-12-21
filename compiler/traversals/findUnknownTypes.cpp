#include "findUnknownTypes.h"
#include "expr.h"
#include "symbol.h"
#include "stmt.h"
#include "symtab.h"
#include "runtime.h"


void RemoveTypeVariableActuals::postProcessExpr(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (FnSymbol* fn = call->isResolved()) {
      if (fn->hasPragma("keep types"))
        return;
      DefExpr* formalDef = fn->formals->first();
      for_alist(Expr, actual, call->argList) {
        ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        if (formal->intent == INTENT_TYPE) {
          actual->remove();
        } else if (formal->type == dtMethodToken) {
          actual->remove();
        } else if (formal->type == dtSetterToken) {
          actual->remove();
        }
        formalDef = fn->formals->next();
      }
    }
  }
}


void RemoveTypeVariableFormals::postProcessExpr(Expr* expr) {
  if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def->sym)) {
      if (fn->hasPragma("keep types")) {
        return;
      }
      for_alist(DefExpr, formalDef, fn->formals) {
        ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        if (formal->intent == INTENT_TYPE) {
          formalDef->remove();
        } else if (formal->type == dtMethodToken) {
          formalDef->remove();
        } else if (formal->type == dtSetterToken) {
          formalDef->remove();
        }
      }
    }
  }
}


void removeTypeVariableActuals(void) {
  Pass* pass = new RemoveTypeVariableActuals();
  pass->run(Symboltable::getModules(pass->whichModules));
}


void removeTypeVariableFormals(void) {
  Pass* pass = new RemoveTypeVariableFormals();
  pass->run(Symboltable::getModules(pass->whichModules));
}
