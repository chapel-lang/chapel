#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"
#include "runtime.h"


static void setFieldTypes(FnSymbol* fn);


static Vec<FnSymbol*> resolvedFns;
Vec<CallExpr*> callStack;

static Map<FnSymbol*,Vec<FnSymbol*>*> ddf; // map of functions to
                                           // virtual children

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

static bool canDispatch(Type* actualType,
                        Symbol* actualParam,
                        Type* formalType,
                        FnSymbol* fn = NULL,
                        bool* require_scalar_promotion = NULL);

static void
resolveFormals(FnSymbol* fn) {
  static Vec<FnSymbol*> done;

  if (!fn->isGeneric) {
    if (done.set_in(fn))
      return;
    done.set_add(fn);

    for_formals(formal, fn) {
      if (formal->defPoint->exprType) {
        formal->type = resolve_type_expr(formal->defPoint->exprType);
        formal->defPoint->exprType->remove();
      }
    }
    if (fn->retExprType) {
      fn->retType = resolve_type_expr(fn->retExprType);
      fn->retExprType->remove();
    }
    if (fn->fnClass == FN_CONSTRUCTOR)
      setFieldTypes(fn);
  }
}

static bool
fits_in_int(int width, Immediate* imm) {
  if (imm->const_kind == NUM_KIND_INT && imm->num_index == INT_SIZE_32) {
    int64 i = imm->int_value();
    switch (width) {
    default: INT_FATAL("bad width in fits_in_int");
    case 8:
      return (i >= -128 && i <= 127);
    case 16:
      return (i >= -32768 && i <= 32767);
    case 32:
      return (i >= -2147483648ll && i <= 2147483647ll);
    case 64:
      return (i >= -9223372036854775807ll-1 && i <= 9223372036854775807ll);
    }
  }
  /* else if (imm->const_kind == NUM_KIND_UINT) {
    uint64 i = imm->uint_value();
    switch (width) {
    default: INT_FATAL("bad width in fits_in_int");
    case 8:
      return (i <= 127);
    case 16:
      return (i <= 32767);
    case 32:
      return (i <= 2147483647ll);
    case 64:
      return (i <= 9223372036854775807ll);
    }
    }*/
  return false;
}

static bool
fits_in_uint(int width, Immediate* imm) {
  if (imm->const_kind == NUM_KIND_INT) {
    int64 i = imm->int_value();
    if (i < 0)
      return false;
    uint64 u = (uint64)i;
    switch (width) {
    default: INT_FATAL("bad width in fits_in_uint");
    case 8:
      return (u <= 255);
    case 16:
      return (u <= 65535);
    case 32:
      return (u <= 2147483647ull);
    case 64:
      return (u <= 18446744073709551615ull);
    }
  }
  /* fold uint parameters?
 else if (imm->const_kind == NUM_KIND_UINT) {
    uint64 i = imm->uint_value();
    switch (width) {
    default: INT_FATAL("bad width in fits_in_int");
    case 8:
      return (i <= 255);
    case 16:
      return (i <= 65535);
    case 32:
      return (i <= 2147483647ull);
    case 64:
      return (i <= 18446744073709551615ull);
    }
  }
  */
  return false;
}


// Returns true iff dispatching the actualType to the formalType
// results in an instantiation.
static bool
canInstantiate(Type* actualType, Type* formalType) {
  if (formalType == dtAny)
    return true;
  if (formalType == dtAnyInt && (is_int_type(actualType) || is_uint_type(actualType)))
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
canCoerce(Type* actualType, Symbol* actualParam, Type* formalType) {
  if (actualType->symbol->hasPragma( "synchronization primitive")) {
    if (actualType->isGeneric) {
      return false;
    } else {
      Type *base_type = (Type*)(actualType->substitutions.v[0].value);
      return canDispatch(base_type, actualParam, formalType);
    }
  }

  if (is_int_type(formalType) && dynamic_cast<EnumType*>(actualType)) {
    return true;
  }
  if (is_int_type(formalType)) {
    if (actualType == dtBool)
      return true;
    if (is_int_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;
    if (is_uint_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;
    if (get_width(formalType) < 64)
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(actualParam))
        if (var->immediate)
          if (fits_in_int(get_width(formalType), var->immediate))
            return true;
  }
  if (is_uint_type(formalType)) {
    if (actualType == dtBool)
      return true;
    if (is_uint_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;
    if (VarSymbol* var = dynamic_cast<VarSymbol*>(actualParam))
      if (var->immediate)
        if (fits_in_uint(get_width(formalType), var->immediate))
          return true;
  }
  if (is_float_type(formalType)) {
    if (is_int_type(actualType))
      return true;
    if (is_uint_type(actualType))
      return true;
    if (is_float_type(actualType) && 
        get_width(actualType) < get_width(formalType))
      return true;
  }
  if (is_complex_type(formalType)) {
    if (is_int_type(actualType))
      return true;
    if (is_uint_type(actualType))
      return true;
    if (is_float_type(actualType) && 
        (get_width(actualType) <= get_width(formalType)/2))
      return true;
    if (is_complex_type(actualType) && 
        (get_width(actualType) < get_width(formalType)))
      return true;
  }
  if (formalType == dtString) {
    if (is_int_type(actualType) || is_uint_type(actualType) || 
        is_float_type(actualType) || is_complex_type(actualType) ||
        actualType == dtBool)
      return true;
  }
  return false;
}

// Returns true iff the actualType can dispatch to the formalType.
// The function symbol is used to avoid scalar promotion on =.
// param is set if the actual is a parameter (compile-time constant).
static bool
canDispatch(Type* actualType, Symbol* actualParam, Type* formalType, FnSymbol* fn, bool* require_scalar_promotion) {
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
  if (canCoerce(actualType, actualParam, formalType))
    return true;
  forv_Vec(Type, parent, actualType->dispatchParents) {
    if (parent == formalType || canDispatch(parent, actualParam, formalType, fn)) {
      return true;
    }
  }
  if (fn &&
      strcmp(fn->name, "=") && 
      actualType->scalarPromotionType && 
      (canDispatch(actualType->scalarPromotionType, actualParam, formalType, fn))) {
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
  if (canDispatch(actualType, NULL, formalType, fn))
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
      for_formals(formal, fn) {
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
      for_formals(formal, fn) {
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
  bool valid = computeActualFormalMap(fn, &formal_actuals, &formal_params,
                                      actual_formals, num_actuals,
                                      num_formals, actual_types,
                                      actual_params, actual_names);
  if (!valid) {
    delete actual_formals;
    return;
  }

  if (fn->isGeneric) {
    ASTMap subs;
    computeGenericSubs(subs, fn, num_formals, &formal_actuals, &formal_params);
    if (subs.n && !fn->isPartialInstantiation(&subs)) {
      FnSymbol* inst_fn = fn->instantiate_generic(&subs);
      if (inst_fn)
        addCandidate(candidateFns, candidateActualFormals, inst_fn, actual_types, actual_params, actual_names, true);
    }
    delete actual_formals;
    return;
  }

  if (!fn->isGeneric)
    resolveFormals(fn);

  int j = -1;
  for_formals(formal, fn) {
    j++;
    if (!strcmp(fn->name, "=")) {
      if (j == 0) {
        if (formal_actuals.v[j] != formal->type)
          return;
      }
    }
    if (formal_actuals.v[j] &&
        !canDispatch(formal_actuals.v[j], formal_params.v[j], formal->type, fn)) {
      delete actual_formals;
      return;
    }
    if (formal_params.v[j] && formal_params.v[j]->isTypeVariable && !formal->isTypeVariable) {
      delete actual_formals;
      return;
   }
//     if (formal_params.v[j] && !formal_params.v[j]->isTypeVariable && formal->isTypeVariable) {
//       delete actual_formals;
//       return;
//     }
    if (!formal_actuals.v[j] && !formal->defaultExpr) {
      delete actual_formals;
      return;
    }
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
    for_formals(formal, fn) {
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
    int j = 1;
    for_formals(formal, fn) {
      for (int i = 0; i < actual_formals->n; i++) {
        if (actual_formals->v[i] == formal) {
          ArgSymbol* newFormal = wrapper->getFormal(j);
          actual_formals->v[i] = newFormal;
          j++;
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
  for_formals(formal, fn) {
    i++;

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
build_coercion_wrapper(FnSymbol* fn,
                       Vec<Type*>* actual_types,
                       Vec<Symbol*>* actual_params) {
  ASTMap subs;
  int j = -1;
  for_formals(formal, fn) {
    j++;
    Type* actual_type = actual_types->v[j];
    Symbol* actual_param = actual_params->v[j];
    if (canCoerce(actual_type, actual_param, formal->type) || isDispatchParent(actual_type, formal->type))
      subs.put(formal, actual_type->symbol);
  }
  if (subs.n)
    fn = fn->coercion_wrapper(&subs);
  return fn;  
}


static FnSymbol*
build_promotion_wrapper(FnSymbol* fn,
                        Vec<Type*>* actual_types,
                        Vec<Symbol*>* actual_params,
                        bool isSquare) {
  if (!strcmp(fn->name, "="))
    return fn;
  bool promotion_wrapper_required = false;
  Map<Symbol*,Symbol*> promoted_subs;
  int j = -1;
  for_formals(formal, fn) {
    j++;
    Type* actual_type = actual_types->v[j];
    Symbol* actual_param = actual_params->v[j];
    bool require_scalar_promotion = false;
    if (canDispatch(actual_type, actual_param, formal->type, fn, &require_scalar_promotion)){
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


static int
visibility_distance(SymScope* scope, FnSymbol* fn,
                    int d = 1, Vec<SymScope*>* alreadyVisited = NULL) {
  Vec<SymScope*> localAlreadyVisited;

  if (!alreadyVisited)
    alreadyVisited = &localAlreadyVisited;

  if (alreadyVisited->set_in(scope))
    return 0;

  alreadyVisited->set_add(scope);

  if (Symbol* sym = scope->lookupLocal(fn->name)) {
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
      if (tmp == fn)
        return d;
    }
  }

  if (scope->astParent) {
    forv_Vec(ModuleSymbol, module, scope->astParent->modUses) {
      int dd = visibility_distance(module->modScope, fn, d, alreadyVisited);
      if (dd > 0)
        return dd;
    }
  }

  if (scope->parent)
    return visibility_distance(scope->parent, fn, d+1, alreadyVisited);

  return 0;
}


static void
disambiguate_by_scope(SymScope* scope, Vec<FnSymbol*>* candidateFns) {
  Vec<int> vds;
  forv_Vec(FnSymbol, fn, *candidateFns) {
    vds.add(visibility_distance(scope, fn));
  }
  int md = 0;
  for (int i = 0; i < vds.n; i++) {
    if (vds.v[i] != 0) {
      if (md) {
        if (vds.v[i] < md)
          md = vds.v[i];
      } else
        md = vds.v[i];
    }
  }
  for (int i = 0; i < vds.n; i++) {
    if (vds.v[i] != md)
      candidateFns->v[i] = 0;
  }
}


static FnSymbol*
disambiguate_by_match(Vec<FnSymbol*>* candidateFns,
                      Vec<Vec<ArgSymbol*>*>* candidateActualFormals,
                      Vec<Type*>* actual_types,
                      Vec<Symbol*>* actual_params,
                      Vec<ArgSymbol*>** ret_afs) {
  FnSymbol* best = NULL;
  Vec<ArgSymbol*>* actual_formals = 0;
  for (int i = 0; i < candidateFns->n; i++) {
    if (candidateFns->v[i]) {
      best = candidateFns->v[i];
      actual_formals = candidateActualFormals->v[i];
      for (int j = 0; j < candidateFns->n; j++) {
        if (i != j && candidateFns->v[j]) {
          bool better = false;
          bool as_good = true;
          Vec<ArgSymbol*>* actual_formals2 = candidateActualFormals->v[j];
          for (int k = 0; k < actual_formals->n; k++) {
            ArgSymbol* arg = actual_formals->v[k];
            ArgSymbol* arg2 = actual_formals2->v[k];
            if (arg->type == arg2->type && arg->instantiatedParam && !arg2->instantiatedParam)
              as_good = false;
            else if (arg->type == arg2->type && !arg->instantiatedParam && arg2->instantiatedParam)
              better = true;
            else {
              bool require_scalar_promotion1;
              bool require_scalar_promotion2;
              canDispatch(actual_types->v[k], actual_params->v[k], arg->type, best, &require_scalar_promotion1);
              canDispatch(actual_types->v[k], actual_params->v[k], arg2->type, best, &require_scalar_promotion2);
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
                  if (actual_types->v[k] == arg2->type &&
                      actual_types->v[k] != arg->type) {
                    better = true;
                  } else if (actual_types->v[k] == arg->type &&
                             actual_types->v[k] != arg2->type) {
                    as_good = false;
                  } else if (moreSpecific(best, arg2->type, arg->type) && 
                      arg2->type != arg->type) {
                    better = true;
                  } else if (moreSpecific(best, arg->type, arg2->type) &&
                      arg2->type != arg->type) {
                    as_good = false;
                  } else if (is_int_type(arg2->type) && is_uint_type(arg->type)) {
                    better = true;
                  } else if (is_int_type(arg->type) && is_uint_type(arg2->type)) {
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
  *ret_afs = actual_formals;
  return best;
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
    call->parentScope->getVisibleFunctions(&visibleFns, canonicalize_string(name));
  else
    visibleFns.add(call->isResolved());

  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    if (methodTag && !visibleFn->noParens)
      continue;
    addCandidate(&candidateFns, &candidateActualFormals, visibleFn,
                 actual_types, actual_params, actual_names);
  }

  FnSymbol* best = NULL;
  Vec<ArgSymbol*>* actual_formals = 0;
  best = disambiguate_by_match(&candidateFns, &candidateActualFormals,
                               actual_types, actual_params,
                               &actual_formals);

  // use visibility and then look for best again
  if (!best && candidateFns.n > 1) {
    disambiguate_by_scope(call->parentScope, &candidateFns);
    best = disambiguate_by_match(&candidateFns, &candidateActualFormals,
                                 actual_types, actual_params,
                                 &actual_formals);
  }

  if (!best && candidateFns.n > 0) {
    for (int i = 0; i < candidateFns.n; i++) {
      if (candidateFns.v[i]) {
        resolve_call_error_candidates.add(candidateFns.v[i]);
      }
    }
    resolve_call_error = CALL_AMBIGUOUS;
    best = NULL;
  } else if (call->partialTag == PARTIAL_OK && (!best || !best->noParens)) {
    resolve_call_error = CALL_PARTIAL;
    best = NULL;
  } else if (!best) {
    for (int i = 0; i < visibleFns.n; i++) {
      if (visibleFns.v[i]) {
        resolve_call_error_candidates.add(visibleFns.v[i]);
      }
    }
    resolve_call_error = CALL_UNKNOWN;
    best = NULL;
  } else {
    best = build_default_wrapper(best, actual_formals);
    best = build_order_wrapper(best, actual_formals);
    best = build_promotion_wrapper(best, actual_types, actual_params, call->square);
    best = build_coercion_wrapper(best, actual_types, actual_params);
  }

  for (int i = 0; i < candidateActualFormals.n; i++)
    delete candidateActualFormals.v[i];

  return best;
}

static void
computeActuals(CallExpr* call,
               Vec<Type*>* atypes,
               Vec<Symbol*>* aparams,
               Vec<char*>* anames) {
  for_actuals(actual, call) {
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
      if (call->parentSymbol) {
        callStack.add(call);
        resolveCall(call);
        if (call->parentSymbol &&
            (call->typeInfo() == dtUnknown))
          resolveFns(call->isResolved());
        callStack.pop();
      }
    }
  }
  Type* t = expr->typeInfo();
  if (t == dtUnknown)
    INT_FATAL(expr, "Unable to resolve type expression");
  return t;
}


// static void
// build_dispatch_tree(Map<Type*,FnSymbol*>* dispatchMap,
//                     CallExpr* call,
//                     char* name,
//                     Vec<Type*>* atypes,
//                     Vec<Symbol*>* aparams,
//                     Vec<char*>* anames) {
//   forv_Vec(Type, type, atypes->v[1]->dispatchChildren) {
//     atypes->v[1] = type;
//     FnSymbol* fn = resolve_call(call, name, atypes, aparams, anames);
//     if (fn) {
//       dispatchMap->put(type, fn);
//       build_dispatch_tree(dispatchMap, call, name, atypes, aparams, anames);
//     }
//   }
// }


char* call2string(CallExpr* call,
                  char* name,
                  Vec<Type*>& atypes,
                  Vec<Symbol*>& aparams,
                  Vec<char*>& anames) {
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
  bool setter = false;
  int start = 0;
  if (method)
    start = 2;
  if (_this)
    start = 1;
  for (int i = start; i < atypes.n; i++) {
    if (aparams.v[i] == gSetterToken) {
      str = stringcat(str, ") = ");
      setter = true;
      first = false;
      continue;
    }
    if (!first)
      first = true;
    else
      str = stringcat(str, ", ");
    if (anames.v[i])
      str = stringcat(str, anames.v[i], "=");
    VarSymbol* var = dynamic_cast<VarSymbol*>(aparams.v[i]);
    char buff[512];
    if (aparams.v[i] && aparams.v[i]->isTypeVariable)
      str = stringcat(str, atypes.v[i]->symbol->name);
    else if (var && var->immediate) {
      sprint_imm(buff, *var->immediate);
      str = stringcat(str, buff);
    } else
      str = stringcat(str, ":", atypes.v[i]->symbol->name);
  }
  if (!call->methodTag && !setter)
    str = stringcat(str, ")");
  return str;
}


char* fn2string(FnSymbol* fn) {
  char* str;
  int start = 0;
  if (fn->instantiatedFrom)
    fn = fn->instantiatedFrom;
  if (fn->isMethod) {
    if (!strcmp(fn->name, "this")) {
      str = stringcat(":", fn->getFormal(1)->type->symbol->name);
      start = 1;
    } else {
      str = stringcat(":", fn->getFormal(2)->type->symbol->name, ".", fn->name);
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
    ArgSymbol* arg = fn->getFormal(i+1);
    if (!first)
      first = true;
    else
      str = stringcat(str, ", ");
    if (arg->intent == INTENT_PARAM)
      str = stringcat(str, "param ");
    if (arg->isTypeVariable)
      str = stringcat(str, "type ", arg->name);
    else if (arg->type == dtUnknown) {
      if (SymExpr* sym = dynamic_cast<SymExpr*>(arg->defPoint->exprType))
        str = stringcat(str, arg->name, ": ", sym->var->name);
      else
        str = stringcat(str, arg->name);
    } else
      str = stringcat(str, arg->name, ": ", arg->type->symbol->name);
  }
  if (!fn->noParens)
    str = stringcat(str, ")");
  return str;
}


static void
checkBinaryOp(CallExpr* call, Vec<Type*>* atypes, Vec<Symbol*>* aparams) {
  if (call->primitive || call->argList->length() != 2)
    return;
  if (call->isNamed("+") ||
      call->isNamed("-") ||
      call->isNamed("*") ||
      call->isNamed("/") ||
      call->isNamed("**") ||
      call->isNamed("%") ||
      call->isNamed("&") ||
      call->isNamed("|") ||
      call->isNamed("^") ||
      call->isNamed("==") ||
      call->isNamed("!=") ||
      call->isNamed(">") ||
      call->isNamed("<") ||
      call->isNamed(">=") ||
      call->isNamed("<=") ||
      call->isNamed("&&") ||
      call->isNamed("||")) {
    if ((is_int_type(atypes->v[0]) && atypes->v[1] == dtUInt[INT_SIZE_64]) ||
        (is_int_type(atypes->v[1]) && atypes->v[0] == dtUInt[INT_SIZE_64])) {
      VarSymbol* var;
      if (atypes->v[1] == dtUInt[INT_SIZE_64]) {
        var = dynamic_cast<VarSymbol*>(aparams->v[0]);
      } else {
        var = dynamic_cast<VarSymbol*>(aparams->v[1]);
      }
      if (var && var->immediate && var->immediate->const_kind == NUM_KIND_INT) {
        int64 iconst = var->immediate->int_value();
        if (iconst >= 0)
          return;
      }
      SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);
      if (!base)
        INT_FATAL(call, "bad call baseExpr");
      USR_FATAL(call, "illegal use of '%s' on operands of type %s and %s",
                base->var->name, atypes->v[0]->symbol->name,
                atypes->v[1]->symbol->name);
    }
  }
}


static void
resolveCall(CallExpr* call) {
  if (!call->primitive) {
    if (SymExpr* sym = dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (dynamic_cast<VarSymbol*>(sym->var) || dynamic_cast<ArgSymbol*>(sym->var)) {
        Expr* base = call->baseExpr;
        base->replace(new SymExpr("this"));
        call->insertAtHead(base);
      }
    }
    if (dynamic_cast<CallExpr*>(call->baseExpr)) {
      VarSymbol* this_temp = new VarSymbol("this_temp");
      this_temp->isCompilerTemp = true;
      this_temp->canReference = true;
      Expr* base = call->baseExpr;
      base->replace(new SymExpr("this"));
      CallExpr* move = new CallExpr(PRIMITIVE_MOVE, this_temp, base);
      call->insertAtHead(new SymExpr(this_temp));
      call->parentStmt->insertBefore(new DefExpr(this_temp));
      call->parentStmt->insertBefore(move);
      resolveCall(move);
    }
    Vec<Type*> atypes;
    Vec<Symbol*> aparams;
    Vec<char*> anames;
    computeActuals(call, &atypes, &aparams, &anames);

    checkBinaryOp(call, &atypes, &aparams);


    // automatically replace calls with iterator arg with calls to _to_seq
    // if (SymExpr *se = dynamic_cast<SymExpr*>(call->baseExpr)) {
    // se->var
    if (dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (!(call->isNamed( "_to_seq") ||
            call->isNamed( "getNextCursor") ||
            call->isNamed( "getHeadCursor") ||
            call->isNamed( "getValue") ||
            call->isNamed( "isValidCursor?"))) {
        ASTMap subs;
        int    pos = 0;
        forv_Vec( Type, argtype, atypes) {
          ClassType *ct = dynamic_cast<ClassType*>( argtype);
          if (ct && ct->isIterator) {  // replace use with call to _to_seq
            // YAH, skip if method on self
            if (pos==1 && (atypes.v[0] == dtMethodToken))
              continue;

            VarSymbol *temp = new VarSymbol( stringcat( stringcat( "_to_seq_temp", intstring( call->id)), stringcat( "_", intstring( pos))));
            call->parentStmt->insertBefore( new DefExpr( temp));
            subs.put( aparams.v[pos], temp);
            CallExpr  *toseq = new CallExpr( "_to_seq", 
                                             aparams.v[pos]);
            CallExpr  *toseqass = new CallExpr( PRIMITIVE_MOVE,
                                                temp,
                                                toseq);
            // CallExpr  *toseqass = new CallExpr( "=", temp, toseq);
            call->parentStmt->insertBefore( toseqass);
            resolveCall( toseq);
            resolveFns( toseq->isResolved());
            resolveCall( toseqass);
          }
          pos++;
        }

        if (subs.n > 0) {
          update_symbols( call, &subs);
          resolveCall( call);
          return;
        }
      }
    }
    
    SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);
    char* name = base->var->name;
    FnSymbol* resolvedFn = resolve_call(call, name, &atypes, &aparams, &anames);
    if (!resolvedFn && call->partialTag == PARTIAL_OK) {
      CallExpr* parentCall = dynamic_cast<CallExpr*>(call->parentExpr);
      if (!parentCall)
        INT_FATAL(call, "Bad partial call");
      for_actuals_backward(actual, call) {
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
          char* str = call2string(call, name, atypes, aparams, anames);
          USR_FATAL_CONT(call, "%s call '%s'", (resolve_call_error == CALL_AMBIGUOUS) ? "Ambiguous" : "Unresolved", str);
          if (developer) {
            for (int i = callStack.n-1; i>=0; i--) {
              CallExpr* cs = callStack.v[i];
              FnSymbol* f = cs->getFunction();
              if (f->instantiatedFrom)
                USR_PRINT(callStack.v[i], "  instantiated from %s", f->name);
              else
                break;
            }
          }
          bool _this = false;
          if (!strcmp("this", name))
            _this = true;
          if (_this)
            USR_STOP();
          bool printed_one = false;
          forv_Vec(FnSymbol, fn, resolve_call_error_candidates) {
            if (fn->isSetter) 
              continue;
            if (!developer && fn->getModule()->modtype == MOD_STANDARD)
              continue;
            USR_PRINT(fn, "%s %s",
                      printed_one ? "               " : "candidates are:",
                      fn2string(fn));
            printed_one = true;
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
      call->baseExpr->replace(new SymExpr(resolvedFn));
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
      CallExpr* e = new CallExpr(sym->copy(), new_IntSymbol(i));
      CallExpr* move = new CallExpr(PRIMITIVE_MOVE, tmp, e);
      call->parentStmt->insertBefore(move);
      call->insertBefore(new SymExpr(tmp));
      callStack.add(e);
      resolveCall(e);
      if (e->isResolved())
        resolveFns(e->isResolved());
      callStack.pop();
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

      //
      // special case cast of class w/ type variables that is not generic
      //   i.e. types are type definitions (have default types)
      //
      if (CallExpr* cast = dynamic_cast<CallExpr*>(call->get(2))) {
        if (cast->isPrimitive(PRIMITIVE_CAST)) {
          if (SymExpr* te = dynamic_cast<SymExpr*>(cast->get(1))) {
            if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(te->var)) {
              if (ClassType* ct = dynamic_cast<ClassType*>(ts->type)) {
                if (ct->classTag == CLASS_CLASS && ct->isGeneric) {
                  CallExpr* cc = new CallExpr(ct->defaultConstructor->name);
                  te->replace(cc);
                  resolveCall(cc);
                  cc->replace(new SymExpr(cc->typeInfo()->symbol));
                }
              }
            }
          }
        }
      }

      Type* t = call->get(2)->typeInfo();
      if (sym->var->type == dtUnknown)
        sym->var->type = t;
      if (sym->var->type == dtNil)
        sym->var->type = t;
      if (t == dtVoid) {
        USR_FATAL(call->get(2), "illegal use of function that does not return a value");
      }
      if (t == dtUnknown) {
        if (CallExpr* rhs = dynamic_cast<CallExpr*>(call->get(2))) {
          if (FnSymbol* rhsfn = rhs->isResolved()) {
            USR_FATAL_CONT(rhsfn, "Unable to resolve return type of function '%s'", rhsfn->name);
            USR_FATAL(rhs, "Called recursively at this point");
          }
        }
      }
      if (call->get(2)->isRef() && sym->var->canReference) {
        sym->var->isReference = true;
        call->primitive = primitives[PRIMITIVE_REF];
      }
      if (sym->var->isReference && !strncmp(sym->var->name, "_ret_", 5))
        call->primitive = primitives[PRIMITIVE_REF];
      if (t == dtUnknown)
        INT_FATAL(call, "Unable to resolve type");

      // do not resolve function return type yet
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(call->parentSymbol)) {
        if (ReturnStmt* last = dynamic_cast<ReturnStmt*>(fn->body->body->last())) {
          if (SymExpr* ret = dynamic_cast<SymExpr*>(last->expr)) {
            if (ret->var == sym->var) {
              if (ret->var->isCompilerTemp)
                ret->var->type = dtUnknown;
              return;
            }
          }
        }
      }

      ClassType* ct = dynamic_cast<ClassType*>(sym->var->type);
      if (t == dtNil && sym->var->type != dtNil && (!ct || ct->classTag != CLASS_CLASS))
        USR_FATAL(call, "Type mismatch in assignment from nil to %s",
                  sym->var->type->symbol->name);
      if (t != dtNil && t != sym->var->type && !isDispatchParent(t, sym->var->type))
        USR_FATAL(call, "Type mismatch in assignment from %s to %s",
                  t->symbol->name, sym->var->type->symbol->name);
      if (t != sym->var->type && isDispatchParent(t, sym->var->type)) {
        Expr* rhs = call->get(2);
        rhs->remove();
        call->insertAtTail(new CallExpr(PRIMITIVE_CAST, sym->var->type->symbol, rhs));
      }
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
      if (call->isPrimitive(PRIMITIVE_ERROR)) {
        CallExpr* from;
        for (int i = callStack.n-1; i >= 0; i--) {
          from = callStack.v[i];
          if (from->lineno > 0)
            break;
        }
        SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1));
        VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var);
        USR_FATAL(from, "%s", var->immediate->v_string);
      }
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      callStack.add(call);
      resolveCall(call);
      if (call->isResolved())
        resolveFns(call->isResolved());
      if (call->isPrimitive(PRIMITIVE_MOVE))
        if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1)))
          if (dynamic_cast<ClassType*>(sym->var->type)) {
            resolveFormals(sym->var->type->defaultConstructor);
            resolveFns(sym->var->type->defaultConstructor);
          }
      callStack.pop();
    }
  }

  ReturnStmt* last = dynamic_cast<ReturnStmt*>(fn->body->body->last());
  if (!last)
    INT_FATAL(fn, "Function is not normal");
  SymExpr* ret = dynamic_cast<SymExpr*>(last->expr);
  if (!ret)
    INT_FATAL(fn, "Function is not normal");
  if (ret->var->isReference)
    fn->retRef = true;
  Type* rt = ret->var->type;

  Vec<Type*> types;
  Vec<Symbol*> params;
  if (rt == dtUnknown) {
    forv_Vec(BaseAST, ast, asts) {
      if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
        if (sym->var == ret->var) {
          if (CallExpr* call = dynamic_cast<CallExpr*>(sym->parentExpr)) {
            if (call->isPrimitive(PRIMITIVE_MOVE) || call->isPrimitive(PRIMITIVE_REF)) {
              if (call->get(1) == sym) {
                if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2)))
                  params.add(sym->var);
                else
                  params.add(NULL);
                types.add(call->get(2)->typeInfo());
              }
            }
          }
        }
      }
    }
    if (types.n == 1)
      rt = types.v[0];
    if (types.n > 1) {
      for (int i = 0; i < types.n; i++) {
        bool best = true;
        for (int j = 0; j < types.n; j++) {
          if (types.v[i] != types.v[j]) {
            if (!canCoerce(types.v[j], params.v[j], types.v[i]))
              best = false;
          }
        }
        if (best) {
          rt = types.v[i];
          break;
        }
      }
    }
  }

  ret->var->type = rt;
  if (fn->retType == dtUnknown)
    fn->retType = rt;
  if (rt == dtUnknown)
    INT_FATAL(fn, "Unable to resolve return type");

  if (fn->fnClass == FN_CONSTRUCTOR) {
    forv_Vec(Type, parent, fn->retType->dispatchParents) {
      if (dynamic_cast<ClassType*>(parent) && parent != dtValue && parent != dtObject && parent->defaultConstructor) {
        resolveFormals(parent->defaultConstructor);
        resolveFns(parent->defaultConstructor);
      }
    }
  }
}


static bool
signature_match(FnSymbol* fn, FnSymbol* gn) {
  if (fn->name != gn->name)
    return false;
  if (fn->formals->length() != gn->formals->length())
    return false;
  for (int i = 3; i <= fn->formals->length(); i++) {
    ArgSymbol* fa = fn->getFormal(i);
    ArgSymbol* ga = gn->getFormal(i);
    if (strcmp(fa->name, ga->name))
      return false;
    if (fa->type != ga->type)
      return false;
  }
  return true;
}


static void
add_to_ddf(FnSymbol* pfn, ClassType* pt, ClassType* ct) {
  forv_Vec(FnSymbol, cfn, ct->methods) {
    if (signature_match(pfn, cfn)) {
      if (ct->isGeneric) {
        ASTMap subs;
        forv_Vec(FnSymbol, cons, *ct->defaultConstructor->instantiatedTo) {
          subs.put(cfn->getFormal(2), cons->retType);
          for (int i = 3; i <= cfn->formals->length(); i++) {
            ArgSymbol* arg = cfn->getFormal(i);
            if (arg->intent == INTENT_PARAM) {
              INT_FATAL(arg, "unhandled case");
            } else if (arg->type->isGeneric) {
              subs.put(arg, pfn->getFormal(i)->type);
            }
          }
          FnSymbol* icfn = cfn->instantiate_generic(&subs);
          resolveFns(icfn);
          Vec<FnSymbol*>* fns = ddf.get(pfn);
          if (!fns) fns = new Vec<FnSymbol*>();
          fns->add(icfn);
          ddf.put(pfn, fns);
        }
      } else {
        ASTMap subs;
        for (int i = 3; i <= cfn->formals->length(); i++) {
          ArgSymbol* arg = cfn->getFormal(i);
          if (arg->intent == INTENT_PARAM) {
            INT_FATAL(arg, "unhandled case");
          } else if (arg->type->isGeneric) {
            subs.put(arg, pfn->getFormal(i)->type);
          }
        }
        if (subs.n)
          cfn = cfn->instantiate_generic(&subs);
        resolveFns(cfn);
        Vec<FnSymbol*>* fns = ddf.get(pfn);
        if (!fns) fns = new Vec<FnSymbol*>();
        fns->add(cfn);
        ddf.put(pfn, fns);
      }
    }
  }
}


static void
add_all_children_ddf_help(FnSymbol* fn, ClassType* pt, ClassType* ct) {
  if (ct->defaultConstructor->instantiatedTo ||
      resolvedFns.set_in(ct->defaultConstructor))
    add_to_ddf(fn, pt, ct);
  forv_Vec(Type, t, ct->dispatchChildren) {
    ClassType* ct = dynamic_cast<ClassType*>(t);
    if (!ct->instantiatedFrom)
      add_all_children_ddf_help(fn, pt, ct);
  }
}


static void
add_all_children_ddf(FnSymbol* fn, ClassType* pt) {
  forv_Vec(Type, t, pt->dispatchChildren) {
    ClassType* ct = dynamic_cast<ClassType*>(t);
    if (!ct->instantiatedFrom)
      add_all_children_ddf_help(fn, pt, ct);
  }
}


static void
build_ddf() {
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->isWrapper || !resolvedFns.set_in(fn))
      continue;
    if (fn->formals->length() > 1) {
      if (fn->getFormal(1)->type == dtMethodToken) {
        if (ClassType* pt = dynamic_cast<ClassType*>(fn->getFormal(2)->type)) {
          if (pt->classTag == CLASS_CLASS && !pt->isGeneric) {
            add_all_children_ddf(fn, pt);
          }
        }
      }
    }
  }
}


void
resolve() {
  resolveFns(chpl_main);

  int num_types;
  do {
    num_types = gTypes.n;
    Vec<FnSymbol*> keys;
    ddf.get_keys(keys);
    forv_Vec(FnSymbol, key, keys) {
      delete ddf.get(key);
    }
    ddf.clear();
    build_ddf();
  } while (num_types != gTypes.n);

//   printf("ddf:\n");
//   for (int i = 0; i < ddf.n; i++) {
//     if (ddf.v[i].key) {
//       printf("  %s\n", fn2string(ddf.v[i].key));
//       for (int j = 0; j < ddf.v[i].value->n; j++) {
//         printf("    %s\n", fn2string(ddf.v[i].value->v[j]));
//       }
//     }
//   }

  Vec<CallExpr*> calls;
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast))
      calls.add(call);
  }
  forv_Vec(CallExpr, call, calls) {
    if (FnSymbol* key = call->isResolved()) {
      if (Vec<FnSymbol*>* fns = ddf.get(key)) {
        forv_Vec(FnSymbol, fn, *fns) {
          Type* type = fn->getFormal(2)->type;
          CallExpr* subcall = call->copy();
          SymExpr* tmp = new SymExpr(gNil);
          FnSymbol* if_fn = build_if_expr(new CallExpr(PRIMITIVE_GETCID,
                                                       call->get(2)->copy(),
                                                       new_IntSymbol(type->id)),
                                          subcall, tmp);
          if_fn->retRef = false;
          if_fn->buildSetter = false;
          if_fn->retType = key->retType;
          if (key->retType == dtUnknown)
            INT_FATAL(call, "bad parent virtual function return type");
          call->parentStmt->insertBefore(new DefExpr(if_fn));
          call->replace(new CallExpr(if_fn));
          tmp->replace(call);
          subcall->baseExpr->replace(new SymExpr(fn));
          if (SymExpr* se = dynamic_cast<SymExpr*>(subcall->get(2)))
            se->replace(new CallExpr(PRIMITIVE_CAST, type->symbol, se->var));
          else if (CallExpr* ce = dynamic_cast<CallExpr*>(subcall->get(2)))
            if (ce->isPrimitive(PRIMITIVE_CAST))
              ce->get(1)->replace(new SymExpr(type->symbol));
            else
              INT_FATAL(subcall, "unexpected");
          else
            INT_FATAL(subcall, "unexpected");
          normalize(if_fn);
          resolvedFns.set_add(if_fn);
        }
      }
    }
  }

  Vec<FnSymbol*> keys;
  ddf.get_keys(keys);
  forv_Vec(FnSymbol, key, keys) {
    delete ddf.get(key);
  }

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
              } else if (!ct->symbol->hasPragma("array")) {
                call->replace(construct->remove());
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
