#include "findUnknownTypes.h"
#include "expr.h"
#include "symbol.h"
#include "stmt.h"
#include "symtab.h"
#include "runtime.h"


void RemoveTypeVariableActuals::postProcessExpr(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (SymExpr* variable = dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (variable->var->hasPragma("keep types")) {
        return;
      }
    }
    DefExpr* formalDef = NULL;
    DefExpr* nextFormalDef = NULL;
    if (call->findFnSymbol())
      nextFormalDef = call->findFnSymbol()->formals->first();
    for_alist(Expr, arg, call->argList) {
      ArgSymbol* formal = NULL;
      if (nextFormalDef) {
        formalDef = nextFormalDef;
        formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        nextFormalDef = call->findFnSymbol()->formals->next();
      }
      if (SymExpr* var_arg = dynamic_cast<SymExpr*>(arg)) {
        if (dynamic_cast<TypeSymbol*>(var_arg->var)) {
          arg->remove();
          continue;
        } else if (ArgSymbol *p = dynamic_cast<ArgSymbol*>(var_arg->var)) {
          if (p->genericSymbol) {
            arg->remove();
            continue;
          }
        } else if (var_arg->var == methodToken) {
          arg->remove();
          continue;
        } else if (var_arg->var == setterToken) {
          arg->remove();
          continue;
        }
      }
      if (no_infer && formal && formal->intent == INTENT_TYPE) {
        arg->remove();
        continue;
      }
    }
  }
  if (no_infer) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
      if (dynamic_cast<TypeSymbol*>(def->parentSymbol)) {
        if (def->exprType) {
          if (def->sym->type == dtUnknown)
            def->sym->type = def->exprType->typeInfo();
          def->exprType = NULL;
        }
      }
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


void removeTypeVariableActuals(void) {
  Pass* pass = new RemoveTypeVariableActuals();
  pass->run(Symboltable::getModules(pass->whichModules));
}


void removeTypeVariableFormals(void) {
  Pass* pass = new RemoveTypeVariableFormals();
  pass->run(Symboltable::getModules(pass->whichModules));
}
