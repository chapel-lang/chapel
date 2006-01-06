#include "instantiate.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "analysis.h"

#define REMOVE_TYPE_PARAM

void
Instantiate::postProcessExpr(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (call->isResolved())
      return;
    if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
      Vec<FnSymbol*> functions;
      FnSymbol* fn = NULL;
      expr->parentScope->getVisibleFunctions(&functions, cannonicalize_string(base->var->name));
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
        ASTMap substitutions;
        DefExpr* formal = fn->formals->first();
        AList<Expr>* newActuals = new AList<Expr>();
        for_alist(Expr, actual, call->argList) {
          ArgSymbol* formalArg = dynamic_cast<ArgSymbol*>(formal->sym);
          bool sub = false;
          if (formalArg->isGeneric) {
            if (formalArg->genericSymbol) {
              Expr* actualactual = actual;
              if (NamedExpr* namedExpr = dynamic_cast<NamedExpr*>(actual)) {
                if (!strcmp(namedExpr->name, formalArg->name)) {
                  actualactual = namedExpr->actual;
                }
              }
              if (SymExpr* variable = dynamic_cast<SymExpr*>(actualactual)) {
                if (TypeSymbol* actualArg = dynamic_cast<TypeSymbol*>(variable->var)) {
                  if (!dynamic_cast<VariableType*>(actualArg->definition)) {
                    if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(formalArg->genericSymbol)) {
                      substitutions.put(ts->definition, actualArg->definition);
                      sub = true;
                    }
                  }
                } else if (dynamic_cast<VarSymbol*>(variable->var)) {
                  if (variable->var->defPoint) {
                    if (use_alloc) {
                      if (variable->var->type) {
                        if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(formalArg->genericSymbol)) {
                          substitutions.put(ts->definition, variable->var->type);
                          sub = true;
                        }
                      }
                    } else
                    if (CallExpr* call = dynamic_cast<CallExpr*>(variable->var->defPoint->init)) {
                      if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->baseExpr)) {
                        if (FnSymbol* cfn = dynamic_cast<FnSymbol*>(symExpr->var)) {
                          if (cfn->fnClass == FN_CONSTRUCTOR) {
                            if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(formalArg->genericSymbol)) {
                              substitutions.put(ts->definition, cfn->retType);
                              sub = true;
                            }
                          }
                        }
                      }
                    }
                  }
                }
              } else if (CallExpr* call = dynamic_cast<CallExpr*>(actualactual)) {
                if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->baseExpr)) {
                  if (FnSymbol* cfn = dynamic_cast<FnSymbol*>(symExpr->var)) {
                    if (cfn->fnClass == FN_CONSTRUCTOR) {
                      if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(formalArg->genericSymbol)) {
                        substitutions.put(ts->definition, cfn->retType);
                        sub = true;
                      }
                    }
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
                if (VarSymbol* var = dynamic_cast<VarSymbol*>(symExpr->var)) {
                  if (var->immediate) {
                    substitutions.put(formalArg, symExpr->var);
                    sub = true;
                  }
                }
              }
            }
          }
          if (!sub)
            newActuals->insertAtTail(actual->copy());
          formal = fn->formals->next();
        }
        if (substitutions.n) {

          Vec<FnSymbol*> new_functions;
          Vec<TypeSymbol*> new_types;
          FnSymbol* new_fn = 
            fn->instantiate_generic(&substitutions, &new_functions, &new_types);

          if (!new_fn)
            INT_FATAL(fn, "Preinstantiation error");

          forv_Vec(TypeSymbol, type, new_types) {
            TRAVERSE(type, new Instantiate(), true);
          }

          forv_Vec(FnSymbol, function, new_functions) {
            TRAVERSE(function, new Instantiate(), true);
          }

#ifdef REMOVE_TYPE_PARAM
          for_alist(DefExpr, formalDef, new_fn->formals) {
            ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
            if (formal->isGeneric || formal->genericSymbol) {
              formalDef->remove();
            }
          }
#endif
          bool handled = false;
          if (DefExpr* parentDef = dynamic_cast<DefExpr*>(call->parentExpr)) {
            if (parentDef->exprType == call) {
              parentDef->sym->type = new_fn->retType;
              call->remove();
              handled = true;
            }
          }
          if (use_alloc) {
            if (CallExpr* parentCall = dynamic_cast<CallExpr*>(call->parentExpr)) {
              if (parentCall->opTag == OP_MOVE) {
                if (SymExpr* sym = dynamic_cast<SymExpr*>(parentCall->get(1))) {
                  sym->var->type = new_fn->retType;
                }
              }
            }
          }
#ifdef REMOVE_TYPE_PARAM
          if (!handled)
            call->replace(new CallExpr(new_fn, newActuals));
#else
          if (!handled)
            call->baseExpr->replace(new SymExpr(new_fn));
#endif
        }
      }
    }
  }
}


void pre_instantiate(void) {
  if (!preinstantiate || no_infer)
    return;
  Pass* pass = new Instantiate();
  pass->run(Symboltable::getModules(pass->whichModules));
}
