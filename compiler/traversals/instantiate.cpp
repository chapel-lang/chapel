#include "instantiate.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"

void
Instantiate::postProcessExpr(Expr* expr) {
  if (!instantiate) {
    return;
  }
  if (CallExpr* parenOpExpr = dynamic_cast<CallExpr*>(expr)) {
    if (SymExpr* variable = dynamic_cast<SymExpr*>(parenOpExpr->baseExpr)) {
      if (TypeSymbol* typeSymbol = dynamic_cast<TypeSymbol*>(variable->var)) {
        if (FnSymbol* constructor = dynamic_cast<FnSymbol*>(typeSymbol->definition->defaultConstructor)) {
          Map<BaseAST*,BaseAST*> substitutions;
          DefExpr* formal = constructor->formals->first();
          for_alist(Expr, actual, parenOpExpr->argList) {
            ParamSymbol* formalArg = dynamic_cast<ParamSymbol*>(formal->sym);
            if (formalArg->isGeneric) {
              if (formalArg->variableTypeSymbol) {
                if (SymExpr* variable = dynamic_cast<SymExpr*>(actual)) {
                  if (TypeSymbol* actualArg = dynamic_cast<TypeSymbol*>(variable->var)) {
                    if (dynamic_cast<VariableType*>(actualArg->definition)) {
                      return;
                    }
                    substitutions.put(formalArg->variableTypeSymbol->definition, actualArg->definition);
                  }
                  else {
                    return;
                  }
                } else {
                  return;
                }
              } else {
                substitutions.put(formalArg, actual);
              }
            } else {
              return;
            }
            formal = constructor->formals->next();
          }
          if (substitutions.n == 0) {
            return;
          }
          Map<BaseAST*,BaseAST*> map;
          FnSymbol* new_constructor = constructor->instantiate_generic(&map, &substitutions);
          if (DefExpr* defExpr = dynamic_cast<DefExpr*>(parenOpExpr->parentExpr)) {
            if (defExpr->exprType == parenOpExpr) {
              defExpr->exprType = NULL; //->replace(new CallExpr(new_constructor));
              defExpr->sym->type = new_constructor->retType;
            }
          }
        }
      }
    }
  }
}
