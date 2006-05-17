#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"
#include "stringutil.h"
#include "runtime.h"

resolve_call_error_type resolve_call_error;
Vec<FnSymbol*> resolve_call_error_candidates;

bool can_instantiate(Type* actualType, Type* formalType) {
  if (actualType == formalType)
    return true;
  forv_Vec(Type, parent, actualType->typeParents) {
    if (parent == formalType || can_instantiate(parent, formalType))
      return true;
  }
  return false;
}

bool require_scalar_promotion = false;

bool can_dispatch(Symbol* actualParam, Type* actualType, Type* formalType);
bool can_dispatch_ne(Symbol* actualParam, Type* actualType, Type* formalType) {
  if ((actualType != dtAny && actualType != dtUnknown) &&
      (formalType == dtAny || formalType == dtUnknown))
    return true;
  if (actualType == dtNil)
    if (ClassType* ct = dynamic_cast<ClassType*>(formalType))
      if (ct->classTag == CLASS_CLASS)
        return true;
  if ((formalType == dtNumeric && actualType == dtBool) ||
      (formalType == dtNumeric && actualType == dtInt[IF1_INT_TYPE_64]) ||
      (formalType == dtNumeric && actualType == dtFloat[IF1_FLOAT_TYPE_64]))
    return true;
  if ((formalType == dtFloat[IF1_FLOAT_TYPE_64] && 
       actualType == dtInt[IF1_INT_TYPE_64]) ||
      (formalType == dtInt[IF1_INT_TYPE_64] && actualType == dtBool))
    return true; // need coercion wrapper
  if (LiteralType* lt = dynamic_cast<LiteralType*>(formalType))
    if (lt->literal == actualParam)
      return true;
  forv_Vec(Type, parent, actualType->dispatchParents) {
    if (parent == formalType || can_dispatch(actualParam, parent, formalType))
      return true;
  }
  if (actualType->scalarPromotionType && can_dispatch(NULL, actualType->scalarPromotionType, formalType)) {
    require_scalar_promotion = true;
    return true;
  }
  return false;
}

bool can_dispatch(Symbol* actualParam, Type* actualType, Type* formalType) {
  return (actualType == formalType) ||
    can_dispatch_ne(actualParam, actualType, formalType);
}

Vec<FnSymbol*> fns; // live functions list

bool actual_formal_match(Symbol* actual_param, Type* actual_type, ArgSymbol* formal) {
  if (formal->intent == INTENT_TYPE || formal->type == dtUnknown)
    return true;
  if (can_dispatch(actual_param, actual_type, formal->type))
    return true;
  return false;
}


// Return actual-formal map if FnSymbol is viable candidate to call
static void
add_candidate(Map<FnSymbol*,Vec<ArgSymbol*>*>* candidateFns,
              FnSymbol* fn,
              Vec<Type*>* actual_types,
              Vec<Symbol*>* actual_params,
              Vec<char*>* actual_names,
              bool inst = false) {
  int num_actuals = actual_types->n;
  int num_formals = fn->formals ? fn->formals->length() : 0;

  Vec<ArgSymbol*>* actual_formals = new Vec<ArgSymbol*>();
  Vec<Type*> formal_actuals;
  Vec<Symbol*> formal_params;
  for (int i = 0; i < num_formals; i++) {
    formal_actuals.add(NULL);
    formal_params.add(NULL);
  }
  for (int i = 0; i < num_actuals; i++)
    actual_formals->add(NULL);
  for (int i = 0; i < num_actuals; i++) {
    if (actual_names->v[i]) {
      bool match = false;
      int j = -1;
      for_alist(DefExpr, formalDef, fn->formals) {
        ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        j++;
        if (!strcmp(actual_names->v[i], formal->name)) {
          match = true;
          actual_formals->v[i] = formal;
          formal_actuals.v[j] = actual_types->v[i];
          formal_params.v[j] = actual_params->v[i];
          break;
        }
      }
      if (!match)
        return;
    }
  }
  for (int i = 0; i < num_actuals; i++) {
    if (!actual_names->v[i]) {
      bool match = false;
      int j = -1;
      for_alist(DefExpr, formalDef, fn->formals) {
        ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        j++;
        if (!formal_actuals.v[j]) {
          match = true;
          actual_formals->v[i] = formal;
          formal_actuals.v[j] = actual_types->v[i];
          formal_params.v[j] = actual_params->v[i];
          break;
        }
      }
      if (!match && !fn->isGeneric)
        return;
    }
  }
  if (fn->isGeneric) {
    ASTMap subs;
    for (int i = 0; i < num_actuals; i++) {
      if (actual_formals->v[i]) {
        if (actual_formals->v[i]->isGeneric) {
          if (actual_formals->v[i]->intent == INTENT_TYPE) {
            TypeSymbol* ts =
              dynamic_cast<TypeSymbol*>(actual_formals->v[i]->genericSymbol);
            if (!ts)
              INT_FATAL(actual_formals->v[i], "Unanticipated genericSymbol");
            subs.put(ts->definition, actual_types->v[i]);
          } else if (actual_formals->v[i]->intent == INTENT_PARAM) {
            if (actual_params->v[i])
              subs.put(actual_formals->v[i], actual_params->v[i]);
          }
        } else if (fn->isGeneric) {
          TypeSymbol* formalType = actual_formals->v[i]->type->symbol;
          if (fn->genericSymbols.set_in(formalType)) {
            if (can_instantiate(actual_types->v[i], actual_formals->v[i]->type)) {
              subs.put(actual_formals->v[i]->type, actual_types->v[i]);
            }
          }
        }
      }
    }
    if (subs.n && !fn->isPartialInstantiation(&subs)) {
      FnSymbol* inst_fn = fn->instantiate_generic(&subs);
      if (inst_fn)
        add_candidate(candidateFns, inst_fn, actual_types, actual_params, actual_names, true);
    }
    return;
  }

  int j = -1;
  for_alist(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    j++;
    if (formal_actuals.v[j] &&
        !actual_formal_match(formal_params.v[j], formal_actuals.v[j], formal))
      return;
    if (!formal_actuals.v[j] && !formal->defaultExpr)
      return;
  }
  candidateFns->put(fn, actual_formals);
}


CallExpr* new_default_constructor_call(Type* type) {
  if (!type->defaultConstructor)
    INT_FATAL(type, "Cannot build default constructor for type");
  CallExpr* call = new CallExpr(type->defaultConstructor->name);
  if (type->substitutions.n > 0) {
    Vec<BaseAST*> keys;
    type->substitutions.get_keys(keys);
    forv_Vec(BaseAST, key, keys) {
      char* name = NULL;
      if (Type* k = dynamic_cast<Type*>(key)) {
        name = stringcpy(k->symbol->name);
      } else if (Symbol* k = dynamic_cast<Symbol*>(key)) {
        name = stringcpy(k->name);
      } else {
        INT_FATAL(type, "Unexpected case in new_default_constructor_call");
      }
      Expr* actual = NULL;
      BaseAST* value = type->substitutions.get(key);
      if (Type* v = dynamic_cast<Type*>(value)) {
        if (v->defaultConstructor) {
          actual = new_default_constructor_call(v);
        } else {
          actual = new SymExpr(v->symbol);
        }
      } else if (Symbol* v = dynamic_cast<Symbol*>(value)) {
        actual = new SymExpr(v);
      } else {
        INT_FATAL(type, "Unexpected case in new_default_constructor_call");
      }
      call->insertAtTail(new NamedExpr(name, actual));
    }
  }
  return call;
}

static FnSymbol*
build_default_wrapper(FnSymbol* fn,
                      Vec<ArgSymbol*>* actual_formals) {
  FnSymbol* wrapper = fn;
  int num_actuals = actual_formals->n;
  int num_formals = fn->formals ? fn->formals->length() : 0;
  if (num_formals > num_actuals) {
    Vec<Symbol*> defaults;
    for_alist(DefExpr, formalDef, fn->formals) {
      ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
      bool used = false;
      forv_Vec(ArgSymbol, arg, *actual_formals) {
        if (arg == formal)
          used = true;
      }
      if (!used)
        defaults.add(formal);
    }
    wrapper = fn->default_wrapper(&defaults);

    // update actual_formals for use in build_order_wrapper
    DefExpr* newFormalDef = wrapper->formals->first();
    for_alist(DefExpr, formalDef, fn->formals) {
      ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
      for (int i = 0; i < actual_formals->n; i++) {
        if (actual_formals->v[i] == formal) {
          ArgSymbol* newFormal = dynamic_cast<ArgSymbol*>(newFormalDef->sym);
          actual_formals->v[i] = newFormal;
          newFormalDef = wrapper->formals->next();
        }
      }
    }
  }
  return wrapper;
}


static FnSymbol*
build_order_wrapper(FnSymbol* fn,
                    Vec<ArgSymbol*>* actual_formals) {
  bool order_wrapper_required = false;
  Map<Symbol*,Symbol*> formals_to_formals;
  int i = 0;
  for_alist(DefExpr, formalDef, fn->formals) {
    i++;
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);

    int j = 0;
    forv_Vec(ArgSymbol, af, *actual_formals) {
      j++;
      if (af == formal) {
        if (i != j)
          order_wrapper_required = true;
        formals_to_formals.put(formal, actual_formals->v[i-1]);
      }
    }
  }
  if (order_wrapper_required) {
    fn = fn->order_wrapper(&formals_to_formals);
  }
  return fn;
}


static FnSymbol*
build_promotion_wrapper(FnSymbol* fn,
                        Vec<Type*>* actual_types) {
  if (!strcmp(fn->name, "="))
    return fn;
  bool promotion_wrapper_required = false;
  Map<Symbol*,Symbol*> promoted_subs;
  int j = -1;
  for_alist(DefExpr, formalDef, fn->formals) {
    j++;
    Type* actual_type = actual_types->v[j];
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    require_scalar_promotion = false;
    if (can_dispatch(NULL, actual_type, formal->type)) {
      if (require_scalar_promotion) {
        promotion_wrapper_required = true;
        promoted_subs.put(formal, actual_type->symbol);
      }
    }
  }
  if (promotion_wrapper_required)
    fn = fn->promotion_wrapper(&promoted_subs);
  return fn;
}


FnSymbol*
resolve_call(CallExpr* call,
             Vec<Type*>* actual_types,
             Vec<Symbol*>* actual_params,
             Vec<char*>* actual_names) {
  resolve_call_error = CALL_NO_ERROR;
  resolve_call_error_candidates.clear();

  if (call->isNamed("_chpl_alloc"))
    return dynamic_cast<FnSymbol*>(prelude->lookup("_chpl_alloc"));

  if (call->isResolved())
    return call->isResolved();

  SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);
  char* name = base->var->name;

  FnSymbol *fn = 
    resolve_call(call, name, actual_types, actual_params, actual_names, call->partialTag);
  if (!fn) {
    switch (resolve_call_error) {
      default: break;
      case CALL_AMBIGUOUS:
        USR_FATAL_CONT(call, "Ambiguous call '%s'", name);
        USR_PRINT(call, "  Candidates are:");
        forv_Vec(FnSymbol, fn, resolve_call_error_candidates) {
          USR_PRINT(fn, "    %s", fn->name);
        }
        USR_STOP();
        break;
      case CALL_UNKNOWN:
        USR_FATAL(call, "Unresolved function call");
        break;
    }
  }
  return fn;
}

FnSymbol*
resolve_call(BaseAST* ast,
             char *name,
             Vec<Type*>* actual_types,
             Vec<Symbol*>* actual_params,
             Vec<char*>* actual_names,
             PartialTag partialTag,
             FnSymbol *fnSymbol)
{
  Map<FnSymbol*,Vec<ArgSymbol*>*> candidateFns; // candidate functions

  if (!fnSymbol) {
    char* canon_name = cannonicalize_string(name);
    Vec<FnSymbol*> visibleFns;                    // visible functions
    ast->parentScope->getVisibleFunctions(&visibleFns, canon_name);
    forv_Vec(FnSymbol, visibleFn, visibleFns)
      add_candidate(&candidateFns, visibleFn, actual_types, actual_params, actual_names);
  } else 
    add_candidate(&candidateFns, fnSymbol, actual_types, actual_params, actual_names);

  FnSymbol* best = NULL;
  Vec<ArgSymbol*>* actual_formals = 0;
  for (int i = 0; i < candidateFns.n; i++) {
    if (candidateFns.v[i].key) {
      best = candidateFns.v[i].key;
      actual_formals = candidateFns.v[i].value;
      for (int j = 0; j < candidateFns.n; j++) {
        if (i != j && candidateFns.v[j].key) {
          bool better = false;
          bool as_good = true;
          Vec<ArgSymbol*>* actual_formals2 = candidateFns.v[j].value;
          for (int k = 0; k < actual_formals->n; k++) {
            ArgSymbol* arg = actual_formals->v[k];
            ArgSymbol* arg2 = actual_formals2->v[k];
            if (arg->intent != INTENT_TYPE && arg2->intent != INTENT_TYPE) {
              if (can_dispatch_ne(NULL, arg2->type, arg->type)) {
                better = true;
              }
              if (!can_dispatch(NULL, arg2->type, arg->type)) {
                as_good = false;
              }
            }
            if (arg->intent == INTENT_TYPE && arg2->intent != INTENT_TYPE)
              better = true;
            if (arg->intent != INTENT_TYPE && arg2->intent == INTENT_TYPE)
              as_good = false;
          }
          if (better || as_good) {
            best = NULL;
            break;
          }
        }
      }
      if (best)
        break;
    }
  }

  if (!best && candidateFns.n > 0) {
    for (int i = 0; i < candidateFns.n; i++) {
      if (candidateFns.v[i].key) {
        resolve_call_error_candidates.add(candidateFns.v[i].key);
      }
    }
    resolve_call_error = CALL_AMBIGUOUS;
    return NULL;
  }

  if (partialTag == PARTIAL_OK && (!best || !best->noParens)) {
    resolve_call_error = CALL_PARTIAL;
    return NULL;
  }

  if (!best) {
    resolve_call_error = CALL_UNKNOWN;
    return NULL;
  }

  best = build_default_wrapper(best, actual_formals);
  best = build_order_wrapper(best, actual_formals);
  best = build_promotion_wrapper(best, actual_types);
  // need to implement build coercion wrapper
  return best;
}

void
resolve_type_expr(BaseAST* base) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (!call->parentSymbol)
        continue;
      Vec<Type*> actual_types;
      Vec<Symbol*> actual_params;
      Vec<char*> actual_names;
      for_alist(Expr, actual, call->argList) {
        actual_types.add(actual->typeInfo());
        SymExpr* symExpr;
        if (NamedExpr* named = dynamic_cast<NamedExpr*>(actual)) {
          actual_names.add(named->name);
          symExpr = dynamic_cast<SymExpr*>(named->actual);
        } else {
          actual_names.add(NULL);
          symExpr = dynamic_cast<SymExpr*>(actual);
        }
        if (symExpr && symExpr->var->isParam()) {
          actual_params.add(symExpr->var);
        } else {
          actual_params.add(NULL);
        }
      }
      FnSymbol* fn = resolve_call(call, &actual_types,
                                  &actual_params, &actual_names);
      if (fn) {
        call->replace(new SymExpr(fn->retType->symbol));
      } else {
        INT_FATAL(call, "Error resolving formal argument type");
      }
    }
  }
}
