#include "instantiate.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "if1.h"

void
Instantiate::postProcessExpr(Expr* expr) {
  if (!preinstantiate) {
    return;
  }
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
      Vec<FnSymbol*> functions;
      FnSymbol* fn = NULL;
      expr->parentScope->getVisibleFunctions(&functions, if1_cannonicalize_string(if1, base->var->name));
      forv_Vec(FnSymbol, f, functions) {
        if (f && !f->instantiatedFrom) {
          if (!fn) {
            fn = f;
          } else {
            fn = NULL;
            break;
          }
        }
      }
      if (fn && fn->fnClass == FN_CONSTRUCTOR) {
        Map<BaseAST*,BaseAST*> substitutions;
        DefExpr* formal = fn->formals->first();
        AList<Expr>* newActuals = new AList<Expr>();
        for_alist(Expr, actual, call->argList) {
          ArgSymbol* formalArg = dynamic_cast<ArgSymbol*>(formal->sym);
          bool sub = false;
          if (formalArg->isGeneric) {
            if (formalArg->variableTypeSymbol) {
              Expr* actualactual = actual;
              if (NamedExpr* namedExpr = dynamic_cast<NamedExpr*>(actual)) {
                if (!strcmp(namedExpr->name, formalArg->name)) {
                  actualactual = namedExpr->actual;
                }
              }
              if (SymExpr* variable = dynamic_cast<SymExpr*>(actualactual)) {
                if (TypeSymbol* actualArg = dynamic_cast<TypeSymbol*>(variable->var)) {
                  if (!dynamic_cast<VariableType*>(actualArg->definition)) {
                    substitutions.put(formalArg->variableTypeSymbol->definition, actualArg->definition);
                    sub = true;
                  }
                }
              }
            } else {
              Expr* actualactual = actual;
              if (NamedExpr* namedExpr = dynamic_cast<NamedExpr*>(actual)) {
                if (!strcmp(namedExpr->name, formalArg->name)) {
                  actualactual = namedExpr->actual;
                }
              }
              if (SymExpr* symExpr = dynamic_cast<SymExpr*>(actualactual)) {
                substitutions.put(formalArg, symExpr->var);
                sub = true;
              }
            }
          }
          if (!sub)
            newActuals->insertAtTail(actual->copy());
          formal = fn->formals->next();
        }
        if (substitutions.n) {
          FnSymbol* new_fn = fn->preinstantiate_generic(&substitutions);
          for_alist(DefExpr, formalDef, new_fn->formals) {
            ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
            if (formal->isGeneric || formal->variableTypeSymbol) {
              formalDef->remove();
            }
          }
          bool handled = false;
          if (DefExpr* parentDef = dynamic_cast<DefExpr*>(call->parentExpr)) {
            if (parentDef->exprType == call) {
              parentDef->sym->type = new_fn->retType;
              call->remove();
              handled = true;
            }
          }
          if (!handled)
            call->replace(new CallExpr(new_fn, newActuals));
        }
      }
    }
  }
}
