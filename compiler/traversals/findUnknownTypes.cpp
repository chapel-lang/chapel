#include "findUnknownTypes.h"
#include "expr.h"
#include "symbol.h"
#include "stmt.h"
#include "symtab.h"


void RemoveTypeVariableActuals::preProcessExpr(Expr* expr) {
  if (FnCall* call = dynamic_cast<FnCall*>(expr)) {
    if (Variable* variable = dynamic_cast<Variable*>(call->baseExpr)) {
      if (variable->var->defPoint->parentStmt->hasPragma("keep types")) {
        return;
      }
    }
    Expr* arg = call->argList->first();
    while (arg) {
      Expr* next_arg = call->argList->next();
      if (Variable* var_arg = dynamic_cast<Variable*>(arg)) {
        if (dynamic_cast<TypeSymbol*>(var_arg->var)) {
          arg->remove();
        } else if (ParamSymbol *p = dynamic_cast<ParamSymbol*>(var_arg->var)) {
          if (p->typeVariable)
            arg->remove();
        } else if (var_arg->var == Symboltable::lookupInternal("_methodToken")) {
          arg->remove();
        }
      }
      arg = next_arg;
    }
  }
}


void RemoveTypeVariableFormals::preProcessSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (fn->defPoint->parentStmt->hasPragma("keep types")) {
      return;
    }
    for_alist(DefExpr, formal, fn->formals) {
      if (dynamic_cast<ParamSymbol*>(formal->sym)->typeVariable) {
        formal->remove();
        fn->body->body->insertAtHead(new DefStmt(formal->copy()));
      } else if (formal->sym->type->symbol ==
                 Symboltable::lookupInternal("_methodTokenType")) {
        formal->sym->parentScope->remove(formal->sym);
        formal->remove();
      }
    }
  }
}
