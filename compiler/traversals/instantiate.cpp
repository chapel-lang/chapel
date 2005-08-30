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
  if (!instantiate) {
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
              if (SymExpr* variable = dynamic_cast<SymExpr*>(actual)) {
                if (TypeSymbol* actualArg = dynamic_cast<TypeSymbol*>(variable->var)) {
                  if (!dynamic_cast<VariableType*>(actualArg->definition)) {
                    substitutions.put(formalArg->variableTypeSymbol->definition, actualArg->definition);
                    sub = true;
                  }
                }
              }
            } else {
              substitutions.put(formalArg, actual);
              sub = true;
            }
          }
          if (!sub)
            newActuals->insertAtTail(actual->copy());
          formal = fn->formals->next();
        }
        if (substitutions.n) {
          Map<BaseAST*,BaseAST*> map;
          FnSymbol* new_fn = fn->instantiate_generic(&map, &substitutions);
          if (newActuals->length() == 0) {
            call->replace(new SymExpr(new_fn->retType->symbol));
          } else {
            call->replace(new CallExpr(new_fn, newActuals));
          }
        }
      }
    }
  }
}
