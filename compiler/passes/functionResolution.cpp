#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"
#include "runtime.h"


static void setFieldTypes(FnSymbol* fn);


static Vec<FnSymbol*> resolvedFns;


// types contains the types of the actuals
// names contains the name if it is a named argument, otherwise NULL
// e.g.  foo(arg1=12, "hi");
//  types = int, string
//  names = arg1, NULL
enum resolve_call_error_type {
  CALL_NO_ERROR,
  CALL_PARTIAL,
  CALL_AMBIGUOUS,
  CALL_UNKNOWN
};
static resolve_call_error_type resolve_call_error;
static Vec<FnSymbol*> resolve_call_error_candidates;
static FnSymbol* resolve_call(CallExpr* call,
                              char *name,
                              Vec<Type*>* actual_types,
                              Vec<Symbol*>* actual_params,
                              Vec<char*>* actual_names);
static Type* resolve_type_expr(Expr* expr);

static void resolveCall(CallExpr* call);
static void resolveFns(FnSymbol* fn);

static bool canDispatch( Type* actualType, Type* formalType, FnSymbol* fn = NULL, bool* require_scalar_promotion = NULL);

static void
resolveFormals(FnSymbol* fn) {
  static Vec<FnSymbol*> done;

  if (!fn->isGeneric) {
    if (done.set_in(fn))
      return;
    done.set_add(fn);

    for_alist(DefExpr, formalDef, fn->formals) {
      if (formalDef->exprType) {
        formalDef->sym->type = resolve_type_expr(formalDef->exprType);
        formalDef->exprType->remove();
      }
    }
    if (fn->retExpr) {
      fn->retType = resolve_type_expr(fn->retExpr);
      fn->retExpr->remove();
    }
    if (fn->fnClass == FN_CONSTRUCTOR)
      setFieldTypes(fn);
  }
}

// Returns true iff dispatching the actualType to the formalType
// results in an instantiation.
static bool
canInstantiate(Type* actualType, Type* formalType) {
  if (formalType == dtAny)
    return true;
  if (actualType == formalType)
    return true;
  if (actualType->instantiatedFrom &&
      canInstantiate(actualType->instantiatedFrom, formalType))
    return true;
  return false;
}

// Returns true iff dispatching the actualType to the formalType
// results in a coercion.
static bool
canCoerce(Type* actualType, Type* formalType) {
  if (actualType->symbol->hasPragma( "sync var")) {
    if (actualType->isGeneric) {
      return false;
    } else {
      Type *base_type = (Type*)(actualType->substitutions.v[0].value);
      return canDispatch( base_type, formalType);
    }
  }

  if (is_int_type(formalType) && dynamic_cast<EnumType*>(actualType)) {
    return true;
  }
  if (is_int_type(formalType)) {
    if (actualType == dtBool)
      return true;
    if (is_int_type(actualType) && get_width(actualType) < get_width(formalType))
      return true;
  }
  if (is_uint_type(formalType)) {
    if (actualType == dtBool)
      return true;
    if (is_uint_type(actualType) && get_width(actualType) < get_width(formalType))
      return true;
    if (is_int_type(actualType) && get_width(actualType) <= get_width(formalType))
      return true;
  }
  if (is_float_type(formalType)) {
    if (is_int_type(actualType) && get_width(actualType) <= get_width(formalType))
      return true;
    if (is_float_type(actualType) && get_width(actualType) < get_width(formalType))
      return true;
  }
  if (formalType == dtString) {
    if (is_int_type(actualType) || is_float_type(actualType) || actualType == dtBool)
      return true;
  }
  return false;
}

// Returns true iff the actualType can dispatch to the formalType.
// The function symbol is used to avoid scalar promotion on =.
// param is set if the actual is a parameter (compile-time constant).
static bool
canDispatch( Type* actualType, Type* formalType, FnSymbol* fn, bool* require_scalar_promotion) {
  if (require_scalar_promotion)
    *require_scalar_promotion = false;
  if (actualType == formalType)
    return true;
  if (actualType == dtNil && formalType == dtObject)
    return true;
  if (actualType == dtNil)
    if (ClassType* ct = dynamic_cast<ClassType*>(formalType))
      if (ct->classTag == CLASS_CLASS)
        return true;
  if (canCoerce(actualType, formalType))
    return true;
  forv_Vec(Type, parent, actualType->dispatchParents) {
    if (parent == formalType || canDispatch(parent, formalType, fn)) {
      return true;
    }
  }
  if (fn &&
      strcmp(fn->name, "=") && 
      actualType->scalarPromotionType && 
      (canDispatch(actualType->scalarPromotionType, formalType, fn))) {
    if (require_scalar_promotion)
      *require_scalar_promotion = true;
    return true;
  }
  return false;
}

static bool
isDispatchParent(Type* t, Type* pt) {
  forv_Vec(Type, p, t->dispatchParents)
    if (p == pt || isDispatchParent(p, pt))
      return true;
  return false;
}

static bool
moreSpecific(FnSymbol* fn, Type* actualType, Type* formalType) {
  if (canDispatch( actualType, formalType, fn))
    return true;
  if (canInstantiate(actualType, formalType)) {
    return true;
  }
  return false;
}

static bool
computeActualFormalMap(FnSymbol* fn,
                       Vec<Type*>* formal_actuals,
                       Vec<Symbol*>* formal_params,
                       Vec<ArgSymbol*>* actual_formals,
                       int num_actuals,
                       int num_formals,
                       Vec<Type*>* actual_types,
                       Vec<Symbol*>* actual_params,
                       Vec<char*>* actual_names) {
  for (int i = 0; i < num_formals; i++) {
    formal_actuals->add(NULL);
    formal_params->add(NULL);
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
          formal_actuals->v[j] = actual_types->v[i];
          formal_params->v[j] = actual_params->v[i];
          break;
        }
      }
      if (!match)
        return false;
    }
  }
  for (int i = 0; i < num_actuals; i++) {
    if (!actual_names->v[i]) {
      bool match = false;
      int j = -1;
      for_alist(DefExpr, formalDef, fn->formals) {
        ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        if (formal->variableExpr)
          return (fn->isGeneric) ? true : false;
        j++;
        if (!formal_actuals->v[j]) {
          match = true;
          actual_formals->v[i] = formal;
          formal_actuals->v[j] = actual_types->v[i];
          formal_params->v[j] = actual_params->v[i];
          break;
        }
      }
      if (!match && !fn->isGeneric)
        return false;
    }
  }
  return true;
}


static void
computeGenericSubs(ASTMap &subs,
                   FnSymbol* fn,
                   int num_formals,
                   Vec<Type*>* formal_actuals,
                   Vec<Symbol*>* formal_params) {
  int i = 0;
  for_formals(formal, fn) {
    if (formal->intent == INTENT_PARAM) {
      if (formal_params->v[i] && formal_params->v[i]->isParam()) {
        subs.put(formal, formal_params->v[i]);
      }
    } else if (formal->type->isGeneric) {
      if (formal_actuals->v[i]) {
        if (canInstantiate(formal_actuals->v[i], formal->type)) {
          subs.put(formal, formal_actuals->v[i]);
        }
      } else if (formal->defaultExpr) {
        Type* defaultType = formal->defaultExpr->typeInfo();
        if (canInstantiate(defaultType, formal->type)) {
          subs.put(formal, defaultType);
        }
      }
    }
    i++;
  }
}


// Return actual-formal map if FnSymbol is viable candidate to call
static void
addCandidate(Vec<FnSymbol*>* candidateFns,
             Vec<Vec<ArgSymbol*>*>* candidateActualFormals,
             FnSymbol* fn,
             Vec<Type*>* actual_types,
             Vec<Symbol*>* actual_params,
             Vec<char*>* actual_names,
             bool inst = false) {
  Vec<ArgSymbol*>* actual_formals = new Vec<ArgSymbol*>();

  int num_actuals = actual_types->n;
  int num_formals = fn->formals ? fn->formals->length() : 0;

  Vec<Type*> formal_actuals;
  Vec<Symbol*> formal_params;
  bool valid = computeActualFormalMap(fn, &formal_actuals, &formal_params, actual_formals,
                                      num_actuals, num_formals, actual_types, actual_params,
                                      actual_names);
  if (!valid)
    return;

  if (fn->isGeneric) {
    ASTMap subs;
    computeGenericSubs(subs, fn, num_formals, &formal_actuals, &formal_params);
    if (subs.n && !fn->isPartialInstantiation(&subs)) {
      FnSymbol* inst_fn = fn->instantiate_generic(&subs);
      if (inst_fn)
        addCandidate(candidateFns, candidateActualFormals, inst_fn, actual_types, actual_params, actual_names, true);
    }
    return;
  }

  if (!fn->isGeneric)
    resolveFormals(fn);

  int j = -1;
  for_alist(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    j++;
    if (formal_actuals.v[j] &&
        !canDispatch(formal_actuals.v[j], formal->type, fn))
      return;
    if (formal_params.v[j] && formal_params.v[j]->isTypeVariable && !formal->isTypeVariable)
      return;
//     if (formal_params.v[j] && !formal_params.v[j]->isTypeVariable && formal->isTypeVariable)
//       return;
    if (!formal_actuals.v[j] && !formal->defaultExpr)
      return;
  }
  candidateFns->add(fn);
  candidateActualFormals->add(actual_formals);
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
build_coercion_wrapper(FnSymbol* fn, Vec<Type*>* actual_types) {
  ASTMap subs;
  int j = -1;
  for_alist(DefExpr, formalDef, fn->formals) {
    j++;
    Type* actual_type = actual_types->v[j];
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (canCoerce(actual_type, formal->type) || isDispatchParent(actual_type, formal->type))
      subs.put(formal, actual_type->symbol);
  }
  if (subs.n)
    fn = fn->coercion_wrapper(&subs);
  return fn;  
}


static FnSymbol*
build_promotion_wrapper(FnSymbol* fn,
                        Vec<Type*>* actual_types,
                        bool isSquare) {
  if (!strcmp(fn->name, "="))
    return fn;
  bool promotion_wrapper_required = false;
  Map<Symbol*,Symbol*> promoted_subs;
  int j = -1;
  for_alist(DefExpr, formalDef, fn->formals) {
    j++;
    Type* actual_type = actual_types->v[j];
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    bool require_scalar_promotion = false;
    if (canDispatch(actual_type, formal->type, fn, &require_scalar_promotion)){
      if (require_scalar_promotion) {
        promotion_wrapper_required = true;
        promoted_subs.put(formal, actual_type->symbol);
      }
    }
  }
  if (promotion_wrapper_required)
    fn = fn->promotion_wrapper(&promoted_subs, isSquare);
  return fn;
}


static FnSymbol*
resolve_call(CallExpr* call,
             char *name,
             Vec<Type*>* actual_types,
             Vec<Symbol*>* actual_params,
             Vec<char*>* actual_names) {
  Vec<FnSymbol*> visibleFns;                    // visible functions

  Vec<FnSymbol*> candidateFns;
  Vec<Vec<ArgSymbol*>*> candidateActualFormals; // candidate functions

  bool methodTag = call->methodTag;

  if (!call->isResolved())
    call->parentScope->getVisibleFunctions(&visibleFns, name);
  else
    visibleFns.add(call->isResolved());

  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    if (methodTag && !visibleFn->noParens)
      continue;
    addCandidate(&candidateFns, &candidateActualFormals, visibleFn, actual_types, actual_params, actual_names);
  }

  FnSymbol* best = NULL;
  Vec<ArgSymbol*>* actual_formals = 0;
  for (int i = 0; i < candidateFns.n; i++) {
    if (candidateFns.v[i]) {
      best = candidateFns.v[i];
      actual_formals = candidateActualFormals.v[i];
      for (int j = 0; j < candidateFns.n; j++) {
        if (i != j && candidateFns.v[j]) {
          bool better = false;
          bool as_good = true;
          Vec<ArgSymbol*>* actual_formals2 = candidateActualFormals.v[j];
          for (int k = 0; k < actual_formals->n; k++) {
            ArgSymbol* arg = actual_formals->v[k];
            ArgSymbol* arg2 = actual_formals2->v[k];
            if (arg->instantiatedParam && !arg2->instantiatedParam)
              as_good = false;
            else if (!arg->instantiatedParam && arg2->instantiatedParam)
              better = true;
            else {
              bool require_scalar_promotion1;
              bool require_scalar_promotion2;
              canDispatch( actual_types->v[k], arg->type, best, &require_scalar_promotion1);
              canDispatch( actual_types->v[k], arg2->type, best, &require_scalar_promotion2);
              if (require_scalar_promotion1 && !require_scalar_promotion2)
                better = true;
              else if (!require_scalar_promotion1 && require_scalar_promotion2)
                as_good = false;
              else {
                if (arg->instantiatedFrom==dtAny &&
                    arg2->instantiatedFrom!=dtAny) {
                  better = true;
                } else if (arg->instantiatedFrom!=dtAny &&
                           arg2->instantiatedFrom==dtAny) {
                  as_good = false;
                } else {
                  if (moreSpecific(best, arg2->type, arg->type) && 
                      arg2->type != arg->type) {
                    better = true;
                  }
                  if (!moreSpecific(best, arg2->type, arg->type)) {
                    as_good = false;
                  }
                }
              }
            }
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
      if (candidateFns.v[i]) {
        resolve_call_error_candidates.add(candidateFns.v[i]);
      }
    }
    resolve_call_error = CALL_AMBIGUOUS;
    return NULL;
  }

  if (call->partialTag == PARTIAL_OK && (!best || !best->noParens)) {
    resolve_call_error = CALL_PARTIAL;
    return NULL;
  }

  if (!best) {
    for (int i = 0; i < visibleFns.n; i++) {
      if (visibleFns.v[i]) {
        resolve_call_error_candidates.add(visibleFns.v[i]);
      }
    }
    resolve_call_error = CALL_UNKNOWN;
    return NULL;
  }

  best = build_default_wrapper(best, actual_formals);
  best = build_order_wrapper(best, actual_formals);
  best = build_promotion_wrapper(best, actual_types, call->square);
  best = build_coercion_wrapper(best, actual_types);
  return best;
}

static void
computeActuals(CallExpr* call,
               Vec<Type*>* atypes,
               Vec<Symbol*>* aparams,
               Vec<char*>* anames) {
  for_alist(Expr, actual, call->argList) {
    atypes->add(actual->typeInfo());
    SymExpr* symExpr;
    if (NamedExpr* named = dynamic_cast<NamedExpr*>(actual)) {
      anames->add(named->name);
      symExpr = dynamic_cast<SymExpr*>(named->actual);
    } else {
      anames->add(NULL);
      symExpr = dynamic_cast<SymExpr*>(actual);
    }
    if (symExpr)
      aparams->add(symExpr->var);
    else
      aparams->add(NULL);
  }
}

static Type*
resolve_type_expr(Expr* expr) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, expr);

  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      resolveCall(call);
      if (call->typeInfo() == dtUnknown)
        resolveFns(call->isResolved());
    }
  }
  Type* t = expr->typeInfo();
  if (t == dtUnknown)
    INT_FATAL(expr, "Unable to resolve type expression");
  return t;
}


static void
build_dispatch_tree(Map<Type*,FnSymbol*>* dispatchMap,
                    CallExpr* call,
                    char* name,
                    Vec<Type*>* atypes,
                    Vec<Symbol*>* aparams,
                    Vec<char*>* anames) {
  forv_Vec(Type, type, atypes->v[1]->dispatchChildren) {
    atypes->v[1] = type;
    FnSymbol* fn = resolve_call(call, name, atypes, aparams, anames);
    if (fn) {
      dispatchMap->put(type, fn);
      build_dispatch_tree(dispatchMap, call, name, atypes, aparams, anames);
    }
  }
}


static void
resolveCall(CallExpr* call) {
  if (!call->primitive) {
    bool already_resolved = false;
    bool is_this = false;
    if (SymExpr* sym = dynamic_cast<SymExpr*>(call->baseExpr))
      if (dynamic_cast<VarSymbol*>(sym->var) || dynamic_cast<ArgSymbol*>(sym->var))
        is_this = true;
    if (dynamic_cast<CallExpr*>(call->baseExpr))
      is_this = true;
    if (is_this) {
      Expr* base = call->baseExpr;
      base->replace(new SymExpr("this"));
      call->insertAtHead(base);
    }
    if (call->isResolved())
      already_resolved = true;
    Vec<Type*> atypes;
    Vec<Symbol*> aparams;
    Vec<char*> anames;
    computeActuals(call, &atypes, &aparams, &anames);

    SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);
    char* name = base->var->name;
    FnSymbol* resolvedFn = resolve_call(call, name, &atypes, &aparams, &anames);
    if (!resolvedFn && call->partialTag == PARTIAL_OK) {
      CallExpr* parentCall = dynamic_cast<CallExpr*>(call->parentExpr);
      if (!parentCall)
        INT_FATAL(call, "Bad partial call");
      for_alist_backward(Expr, actual, call->argList) {
        actual->remove();
        parentCall->insertAtHead(actual);
      }
      parentCall->baseExpr->replace(call->baseExpr->remove());
      return;
    }
    if (!resolvedFn) {
      if (resolve_call_error == CALL_UNKNOWN || resolve_call_error == CALL_AMBIGUOUS) {
        if (!strcmp("=", name)) {
          if (atypes.v[1] == dtNil) {
            USR_FATAL(call, "Illegal assignment of nil to structure of type %s",
                      atypes.v[0]->symbol->name);
          } else {
            USR_FATAL(call, "Type mismatch, assignment from %s to %s",
                      atypes.v[1]->symbol->name,
                      atypes.v[0]->symbol->name);
          }
        } else if (resolve_call_error_candidates.n > 0) {
          bool method = false;
          bool _this = false;
          char *str = "";
          if (atypes.n > 1)
            if (atypes.v[0] == dtMethodToken)
              method = true;
          if (method) {
            if (aparams.v[1] && aparams.v[1]->isTypeVariable)
              str = stringcat(str, atypes.v[1]->symbol->name, ".");
            else
              str = stringcat(str, ":", atypes.v[1]->symbol->name, ".");
          }
          if (!strcmp("this", name))
            _this = true;
          if (_this) {
            str = stringcat(str, ":", atypes.v[0]->symbol->name);
          } else if (!strncmp("_construct_", name, 11)) {
            str = stringcat(str, name+11);
          } else {
            str = stringcat(str, name);
          }
          if (!call->methodTag)
            str = stringcat(str, "(");
          bool first = false;
          int start = 0;
          if (method)
            start = 2;
          if (_this)
            start = 1;
          for (int i = start; i < atypes.n; i++) {
            if (!first)
              first = true;
            else
              str = stringcat(str, ", ");
            if (anames.v[i])
              str = stringcat(str, anames.v[i], "=");
            if (aparams.v[i] && aparams.v[i]->isTypeVariable)
              str = stringcat(str, atypes.v[i]->symbol->name);
            else
              str = stringcat(str, ":", atypes.v[i]->symbol->name);
          }
          if (!call->methodTag)
            str = stringcat(str, ")");
          USR_FATAL_CONT(call, "%s call '%s'", (resolve_call_error == CALL_AMBIGUOUS) ? "Ambiguous" : "Unresolved", str);
          if (_this)
            USR_STOP();
          USR_PRINT("Candidates are:");
          forv_Vec(FnSymbol, fn, resolve_call_error_candidates) {
            if (fn->isSetter || fn->_setter)
              continue;
            int start = 0;
            if (fn->instantiatedFrom)
              fn = fn->instantiatedFrom;
            if (fn->isMethod) {
              if (!strcmp(fn->name, "this")) {
                DefExpr* formalDef = fn->formals->get(1);
                str = stringcat(":", formalDef->sym->type->symbol->name);
                start = 1;
              } else {
                DefExpr* formalDef = fn->formals->get(2);
                str = stringcat(":", formalDef->sym->type->symbol->name, ".", fn->name);
                start = 2;
              }
            } else if (!strncmp("_construct_", fn->name, 11))
              str = stringcat(fn->name+11);
            else
              str = stringcat(fn->name);
            if (!fn->noParens)
              str = stringcat(str, "(");
            bool first = false;
            for (int i = start; i < fn->formals->length(); i++) {
              DefExpr* formalDef = fn->formals->get(i+1);
              if (!first)
                first = true;
              else
                str = stringcat(str, ", ");
              if (formalDef->sym->isTypeVariable)
                str = stringcat(str, "type ", formalDef->sym->name);
              else if (formalDef->sym->type == dtUnknown) {
                if (SymExpr* sym = dynamic_cast<SymExpr*>(formalDef->exprType))
                  str = stringcat(str, formalDef->sym->name, ": ", sym->var->name);
              } else
                str = stringcat(str, formalDef->sym->name, ": ", formalDef->sym->type->symbol->name);
            }
            if (!fn->noParens)
              str = stringcat(str, ")");
            USR_PRINT(fn, "  %s", str);
          }
          USR_STOP();
        } else {
          USR_FATAL(call, "Unresolved call '%s'", name);
        }
      } else {
        INT_FATAL(call, "Error in resolve_call");
      }
    }
    if (call->parentSymbol) {
      bool may_dispatch = false;
      if (atypes.n > 1) {
        if (atypes.v[0] == dtMethodToken) {
          if (ClassType* ct = dynamic_cast<ClassType*>(atypes.v[1])) {
            if (ct->classTag == CLASS_CLASS) {
              may_dispatch = true;
            }
          }
        }
      }
      if (may_dispatch && !already_resolved) {
        // handle dynamic dispatch tree
        Map<Type*,FnSymbol*> dispatchMap;
        build_dispatch_tree(&dispatchMap, call, name, &atypes, &aparams, &anames);
        Vec<Type*> types;
        dispatchMap.get_keys(types);
        CallExpr* nextcall = call->copy();
        call->baseExpr->replace(new SymExpr(resolvedFn));
        resolveFns(resolvedFn);
        Stmt* stmt = call->parentStmt;
        Expr* dynamicArg = call->get(2);
        forv_Vec(Type, type, types) {
          if (type->isGeneric) {
            USR_FATAL(type, "Cannot handle generic types with parent types");
          }
          resolveFormals(type->defaultConstructor);
          resolveFns(type->defaultConstructor);
          CallExpr* nextnextcall = nextcall->copy();
          resolveFns(dispatchMap.get(type));
          nextcall->baseExpr = new SymExpr(dispatchMap.get(type));
          if (resolvedFn->retType != dispatchMap.get(type)->retType) {
            INT_FATAL(call, "Illegal dispatch functions"); // make user error
          }
          nextcall->get(2)->replace(new CallExpr(PRIMITIVE_CAST, type->symbol, dynamicArg->copy()));
          FnSymbol* if_fn = build_if_expr(new CallExpr(PRIMITIVE_GETCID,
                                                       dynamicArg->copy(),
                                                       new_IntLiteral(type->id)),
                                          nextcall, call->copy());
          stmt->insertBefore(new DefExpr(if_fn));
          if_fn->retRef = false;
          nextcall = nextnextcall;
          CallExpr* tmp = new CallExpr(if_fn);
          call->replace(tmp);
          call = tmp;
          normalize(if_fn);
          resolveFns(if_fn);
        }
      } else {
        call->baseExpr->replace(new SymExpr(resolvedFn));
      }
    }

  } else if (call->isPrimitive(PRIMITIVE_TUPLE_EXPAND)) {
    SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1));
    Symbol* var = dynamic_cast<Symbol*>(sym->var);
    int size = 0;
    for (int i = 0; i < var->type->substitutions.n; i++) {
      if (var->type->substitutions.v[i].key) {
        if (!strcmp("size", dynamic_cast<Symbol*>(var->type->substitutions.v[i].key)->name)) {
          size = dynamic_cast<VarSymbol*>(var->type->substitutions.v[i].value)->immediate->v_int64;
          break;
        }
      }
    }
    if (size == 0)
      INT_FATAL(call, "Invalid tuple expand primitive");
    for (int i = 1; i <= size; i++) {
      VarSymbol* tmp = new VarSymbol("_expand_temp");
      DefExpr* def = new DefExpr(tmp);
      call->parentStmt->insertBefore(def);
      CallExpr* e = new CallExpr(sym->copy(), new_IntLiteral(i));
      CallExpr* move = new CallExpr(PRIMITIVE_MOVE, tmp, e);
      call->parentStmt->insertBefore(move);
      call->insertBefore(tmp);
      resolveCall(e);
      if (e->isResolved())
        resolveFns(e->isResolved());
      resolveCall(move);
    }
    call->remove();
  } else if (call->isPrimitive(PRIMITIVE_CAST)) {
    Type* t= call->get(1)->typeInfo();
    if (t == dtUnknown)
      INT_FATAL(call, "Unable to resolve type");
    call->get(1)->replace(new SymExpr(t->symbol));
  } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
    SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2));
    if (!sym)
      INT_FATAL(call, "bad set member primitive");
    VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var);
    if (!var || !var->immediate)
      INT_FATAL(call, "bad set member primitive");
    char* name = var->immediate->v_string;
    ClassType* ct = dynamic_cast<ClassType*>(call->get(1)->typeInfo());
    if (!ct)
      INT_FATAL(call, "bad set member primitive");
    bool found = false;
    for_fields(field, ct) {
      if (!strcmp(field->name, name)) {
        Type* t = call->get(3)->typeInfo();
        if (t == dtUnknown)
          INT_FATAL(call, "Unable to resolve field type");
        if (t != field->type && t != dtNil && t != dtObject)
          USR_FATAL(call, "Cannot assign expression of type %s to field of type %s", t->symbol->name, field->type->symbol->name);
        found = true;
      }
    }
    if (!found)
      INT_FATAL(call, "bad set member primitive");
  } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
    if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
      Type* t = call->get(2)->typeInfo();
      if (sym->var->type == dtUnknown)
        sym->var->type = t;
      if (sym->var->type == dtNil)
        sym->var->type = t;
      if (t == dtUnknown) {
        if (CallExpr* rhs = dynamic_cast<CallExpr*>(call->get(2))) {
          if (FnSymbol* rhsfn = rhs->isResolved()) {
            USR_FATAL_CONT(rhsfn, "Unable to resolve return type of function '%s'", rhsfn->name);
            USR_FATAL(rhs, "Called recursively at this point");
          }
        }
      }
      if (t == dtUnknown)
        INT_FATAL(call, "Unable to resolve type");
      if (t != sym->var->type && 
          t != dtNil && 
          t != dtObject)
        INT_FATAL(call, "Bad type detected");
    }
  }
}


static void
resolveFns(FnSymbol* fn) {
  if (resolvedFns.set_in(fn))
    return;
  resolvedFns.set_add(fn);

  Vec<BaseAST*> asts;
  collect_top_asts(&asts, fn->body);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      resolveCall(call);
      if (call->isResolved())
        resolveFns(call->isResolved());
      if (call->isPrimitive(PRIMITIVE_MOVE))
        if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1)))
          if (dynamic_cast<ClassType*>(sym->var->type)) {
            resolveFormals(sym->var->type->defaultConstructor);
            resolveFns(sym->var->type->defaultConstructor);
          }
    }
  }
  ReturnStmt* last = dynamic_cast<ReturnStmt*>(fn->body->body->last());
  if (!last)
    INT_FATAL(fn, "Function is not normal");
  Type* rt = last->expr->typeInfo();
  if (fn->retType == dtUnknown)
    fn->retType = rt;
  if (rt == dtUnknown)
    INT_FATAL(fn, "Unable to resolve return type");
  if (rt != fn->retType)
    USR_FATAL(fn, "Return type does not match type of returned expression");
}


void
resolve() {
  resolveFns(chpl_main);

  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* a = dynamic_cast<FnSymbol*>(ast)) {
      if (!resolvedFns.set_in(a))
        a->defPoint->parentStmt->remove();
    } else if (ClassType* a = dynamic_cast<ClassType*>(ast)) {
      if (!resolvedFns.set_in(a->defaultConstructor))
        a->symbol->defPoint->parentStmt->remove();
    }
  }
  remove_named_exprs();
  remove_static_actuals();
  remove_static_formals();
  asts.clear();
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (ArgSymbol* a = dynamic_cast<ArgSymbol*>(ast)) {
      if (a->defaultExpr)
        a->defaultExpr->remove();
      if (a->defPoint->exprType)
        a->defPoint->exprType->remove();
    }
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isPrimitive(PRIMITIVE_TYPEOF))
        call->replace(call->get(1)->remove());
      if (call->isNamed("_init")) {
        if (CallExpr* construct = dynamic_cast<CallExpr*>(call->get(1))) {
          if (FnSymbol* fn = construct->isResolved()) {
            if (ClassType* ct = dynamic_cast<ClassType*>(fn->retType)) {
              if (!ct->symbol->hasPragma("array") && ct->defaultValue) {
                call->replace(new CallExpr(PRIMITIVE_CAST, ct->symbol, gNil));
              }
            }
          }
        }
      }
      if (call->isPrimitive(PRIMITIVE_SET_MEMBER) ||
          call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2));
        if (!sym)
          INT_FATAL(call, "bad member primitive");
        VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var);
        if (!var || !var->immediate)
          INT_FATAL(call, "bad member primitive");
        char* name = var->immediate->v_string;
        call->get(2)->replace(new SymExpr(call->get(1)->typeInfo()->getField(name)));
      }
    }
  }
}


static void
setFieldTypes(FnSymbol* fn) {
  ClassType* ct = dynamic_cast<ClassType*>(fn->retType);
  if (!ct)
    INT_FATAL(fn, "Constructor has no class type");
  for_formals(formal, fn) {
    Type* t = formal->type;
    if (t == dtUnknown && formal->defPoint->exprType)
      t = formal->defPoint->exprType->typeInfo();
    if (t == dtUnknown)
      INT_FATAL(formal, "Unable to resolve field type");
    if (t == dtNil)
      USR_FATAL(formal, "Unable to determine type of field from nil");
    bool found = false;
    for_fields(field, ct) {
      if (!strcmp(field->name, formal->name)) {
        field->type = t;
        found = true;
      }
    }
    if (!found)
      INT_FATAL(formal, "Unable to find field in constructor");
  }
}
