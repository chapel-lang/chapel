#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "iterator.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "runtime.h"
#include "../ifa/prim_data.h"

static char* _init;
static char* _pass;
static char* _copy;
static char* _this;
static char* _assign;

Map<Symbol*,Symbol*> paramMap;
static Expr* dropUnnecessaryCast(CallExpr* call);
static void foldEnumOp(int op, EnumSymbol *e1, EnumSymbol *e2, Immediate *imm);
static Expr* preFold(Expr* expr);
static Expr* postFold(Expr* expr);

static FnSymbol* instantiate(FnSymbol* fn, ASTMap* subs);

static void setFieldTypes(FnSymbol* fn);

static int explainLine;
static ModuleSymbol* explainModule;

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

static void pruneResolvedTree();


//
// build reference type
//
static void makeRefType(Type* type) {
  if (type == dtMethodToken)
    return;

  if (type->refType || type->isGeneric || type->symbol->hasPragma("ref"))
    return;

  Vec<Type*> atypes;
  Vec<Symbol*> aparams;
  Vec<char*> anames;
  atypes.add(type);
  aparams.add(NULL);
  anames.add(NULL);
  CallExpr* call = new CallExpr("_construct__ref", type->symbol);
  chpl_main->insertAtHead(call); // add call to AST temporarily
  FnSymbol* fn = resolve_call(call, "_construct__ref",
                              &atypes, &aparams, &anames);
  type->refType = dynamic_cast<ClassType*>(fn->retType);
  type->refType->getField(1)->type = type;
  call->remove();
}


static bool
hasGenericArgs(FnSymbol* fn) {
  for_formals(formal, fn) {
    if (formal->type->isGeneric)
      return true;
    if (formal->defPoint->exprType &&
        formal->defPoint->exprType->typeInfo()->isGeneric)
      return true;
    if (formal->intent == INTENT_PARAM)
      return true;
  }
  return false;
}

bool
tag_generic(FnSymbol* fn) {
  if (fn->isGeneric)
    return false;
  if (hasGenericArgs(fn)) {
    fn->isGeneric = 1; 
    if (fn->retType != dtUnknown && fn->fnClass == FN_CONSTRUCTOR)
      fn->retType->isGeneric = true;
    return true;
  }
  return false;
}


static void
resolveSpecifiedReturnType(FnSymbol* fn) {
  fn->retType = resolve_type_expr(fn->retExprType);
  if (fn->retType != dtUnknown) {
    if (fn->retRef) {
      makeRefType(fn->retType);
      fn->retType = fn->retType->refType;
    }
    fn->retExprType->remove();
    if (fn->fnClass == FN_ITERATOR) {
      prototypeIteratorClass(fn);
      resolvedFns.set_add(fn->iteratorInfo->getHeadCursor);
      resolvedFns.set_add(fn->iteratorInfo->getNextCursor);
      resolvedFns.set_add(fn->iteratorInfo->isValidCursor);
      resolvedFns.set_add(fn->iteratorInfo->getValue);
      resolvedFns.set_add(fn->iteratorInfo->getZipCursor1);
      resolvedFns.set_add(fn->iteratorInfo->getZipCursor2);
      resolvedFns.set_add(fn->iteratorInfo->getZipCursor3);
      resolvedFns.set_add(fn->iteratorInfo->getZipCursor4);
    }
  }
}


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

      //
      // change type of this on record methods to reference type
      //
      if (formal->intent == INTENT_INOUT ||
          formal->intent == INTENT_OUT ||
          (formal == fn->_this &&
           (isRecordType(formal->type) || fn->hasPragma("ref this")))) {
        makeRefType(formal->type);
        formal->type = formal->type->refType;
      }
    }
    if (fn->retExprType)
      resolveSpecifiedReturnType(fn);
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
  return false;
}

static bool
fits_in_uint(int width, Immediate* imm) {
  if (imm->const_kind == NUM_KIND_INT && imm->num_index == INT_SIZE_32) {
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
      return true;
    }
  } else if (imm->const_kind == NUM_KIND_INT && imm->num_index == INT_SIZE_64) {
    int64 i = imm->int_value();
    if (i > 0 && width == 64)
      return true;
  }
  return false;
}


// Returns true iff dispatching the actualType to the formalType
// results in an instantiation.
static bool
canInstantiate(Type* actualType, Type* formalType) {
  if (actualType == dtMethodToken)
    return false;
  if (formalType == dtAny)
    return true;
  if (formalType == dtIntegral && (is_int_type(actualType) || is_uint_type(actualType)))
    return true;
  if (formalType == dtEnumerated && (is_enum_type(actualType)))
    return true;
  if (formalType == dtNumeric && (is_int_type(actualType) || is_uint_type(actualType) || is_imag_type(actualType) || is_real_type(actualType) || is_complex_type(actualType)))
    return true;
  if (formalType == dtIterator && actualType->symbol->hasPragma("iterator class"))
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
canCoerce(Type* actualType, Symbol* actualParam, Type* formalType, FnSymbol* fn) {
  if (actualType->symbol->hasPragma( "synchronization primitive")) {
    if (actualType->isGeneric) {
      return false;
    } else {
      Type *base_type = (Type*)(actualType->substitutions.v[0].value);
      return canDispatch(base_type, actualParam, formalType);
    }
  }

  if (actualType->symbol->hasPragma("ref"))
    return canDispatch(getValueType(actualType), actualParam, formalType, fn);

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
  if (is_real_type(formalType)) {
    if (is_int_type(actualType))
      return true;
    if (is_uint_type(actualType))
      return true;
    if (is_real_type(actualType) && 
        get_width(actualType) < get_width(formalType))
      return true;
  }
  if (is_complex_type(formalType)) {
    if (is_int_type(actualType))
      return true;
    if (is_uint_type(actualType))
      return true;
    if (is_real_type(actualType) && 
        (get_width(actualType) <= get_width(formalType)/2))
      return true;
    if (is_imag_type(actualType) && 
        (get_width(actualType) <= get_width(formalType)/2))
      return true;
    if (is_complex_type(actualType) && 
        (get_width(actualType) < get_width(formalType)))
      return true;
  }
  if (formalType == dtString) {
    if (is_int_type(actualType) || is_uint_type(actualType) || 
        is_real_type(actualType) || is_imag_type(actualType) ||
        is_complex_type(actualType) ||
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
  if (actualType->refType == formalType)
    return true;
  if (canCoerce(actualType, actualParam, formalType, fn))
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
      } else if (formal->defaultExpr) {
        if (SymExpr* se = dynamic_cast<SymExpr*>(formal->defaultExpr)) {
          if (se->var->isParam())
            subs.put(formal, se->var);
        } else
          INT_FATAL(fn, "unable to handle default parameter");
      }
    } else if (formal->type->isGeneric) {
      if (formal_actuals->v[i]) {
        if (canInstantiate(formal_actuals->v[i], formal->type)) {
          subs.put(formal, formal_actuals->v[i]);
        } else if (Type* vt = getValueType(formal_actuals->v[i])) {
          if (canInstantiate(vt, formal->type))
            subs.put(formal, vt);
        }
      } else if (formal->defaultExpr) {
        Type* defaultType = resolve_type_expr(formal->defaultExpr);
        if (canInstantiate(defaultType, formal->type) ||
            defaultType == gNil->type) { // constructor default
          subs.put(formal, defaultType);
        }
      }
    }
    i++;
  }
}


static Map<FnSymbol*,Vec<FnSymbol*>*> varArgsCache;

static FnSymbol*
expandVarArgs(FnSymbol* fn, int numActuals) {
  bool genericArg = false;
  for_formals(arg, fn) {
    if (!genericArg && arg->variableExpr &&
        !dynamic_cast<DefExpr*>(arg->variableExpr))
      resolve_type_expr(arg->variableExpr);

    //
    // set genericArg to true if a generic argument appears before the
    // argument with the variable expression
    //
    if (arg->type->isGeneric)
      genericArg = true;

    // handle unspecified variable number of arguments
    if (DefExpr* def = dynamic_cast<DefExpr*>(arg->variableExpr)) {

      // check for cached stamped out function
      if (Vec<FnSymbol*>* cfns = varArgsCache.get(fn)) {
        forv_Vec(FnSymbol, cfn, *cfns) {
          if (cfn->formals->length() == numActuals)
            return cfn;
        }
      }

      int numCopies = numActuals - fn->formals->length() + 1;
      if (numCopies <= 0)
        return NULL;

      ASTMap map;
      FnSymbol* newFn = fn->copy(&map);
      newFn->visible = false;
      fn->defPoint->insertBefore(new DefExpr(newFn));
      DefExpr* newDef = dynamic_cast<DefExpr*>(map.get(def));
      newDef->replace(new SymExpr(new_IntSymbol(numCopies)));

      ASTMap update;
      update.put(newDef->sym, new_IntSymbol(numCopies));
      update_symbols(newFn, &update);

      // add new function to cache
      Vec<FnSymbol*>* cfns = varArgsCache.get(fn);
      if (!cfns)
        cfns = new Vec<FnSymbol*>();
      cfns->add(newFn);
      varArgsCache.put(fn, cfns);

      return expandVarArgs(newFn, numActuals);
    } else if (SymExpr* sym = dynamic_cast<SymExpr*>(arg->variableExpr)) {

      // handle specified number of variable arguments
      if (VarSymbol* n_var = dynamic_cast<VarSymbol*>(sym->var)) {
        if (n_var->type == dtInt[INT_SIZE_32] && n_var->immediate) {
          int n = n_var->immediate->int_value();
          CallExpr* tupleCall = new CallExpr("_construct__tuple");
          for (int i = 0; i < n; i++) {
            DefExpr* new_arg_def = arg->defPoint->copy();
            ArgSymbol* new_arg = dynamic_cast<ArgSymbol*>(new_arg_def->sym);
            new_arg->variableExpr = NULL;
            tupleCall->insertAtTail(new SymExpr(new_arg));
            new_arg->name = astr("_e", intstring(i), "_", arg->name);
            new_arg->cname = stringcat("_e", intstring(i), "_", arg->cname);
            arg->defPoint->insertBefore(new_arg_def);
          }
          VarSymbol* var = new VarSymbol(arg->name);
          tupleCall->insertAtHead(new_IntSymbol(n));
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, var, tupleCall));
          fn->insertAtHead(new DefExpr(var));
          arg->defPoint->remove();
          ASTMap update;
          update.put(arg, var);
          update_symbols(fn->body, &update);
          if (fn->where) {
            VarSymbol* var = new VarSymbol(arg->name);
            fn->where->insertAtHead(
              new CallExpr(PRIMITIVE_MOVE, var, tupleCall->copy()));
            fn->where->insertAtHead(new DefExpr(var));
            ASTMap update;
            update.put(arg, var);
            update_symbols(fn->where, &update);
          }
        }
      }
    } else if (!fn->isGeneric && arg->variableExpr)
      INT_FATAL("bad variableExpr");
  }
  return fn;
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
  fn = expandVarArgs(fn, actual_types->n);

  if (!fn)
    return;

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
      FnSymbol* inst_fn = instantiate(fn, &subs);
      if (inst_fn)
        addCandidate(candidateFns, candidateActualFormals, inst_fn, actual_types, actual_params, actual_names, true);
    }
    delete actual_formals;
    return;
  }

  if (fn->isGeneric)
    INT_FATAL(fn, "unexpected generic function");

  resolveFormals(fn);

  int j = -1;
  for_formals(formal, fn) {
    j++;
    if (!strcmp(fn->name, "=")) {
      if (j == 0) {
        if (formal_actuals.v[j] != formal->type &&
            getValueType(formal_actuals.v[j]) != formal->type)
          //          delete actual_formals;
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
    if (actual_type != formal->type)
      if (canCoerce(actual_type, actual_param, formal->type, fn) || isDispatchParent(actual_type, formal->type))
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
  if (promotion_wrapper_required) {
    fn = fn->promotion_wrapper(&promoted_subs, isSquare);
    resolveFormals(fn);
  }
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

  if (scope->getModuleUses()) {
    forv_Vec(ModuleSymbol, module, *scope->getModuleUses()) {
      int dd = visibility_distance(module->block->blkScope, fn, d, alreadyVisited);
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
                if (arg->type == arg2->type && arg->instantiatedFrom && !arg2->instantiatedFrom) {
                  better = true;
                } else if (arg->type == arg2->type && !arg->instantiatedFrom && arg2->instantiatedFrom) {
                  as_good = false;
                } else {
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
                    } else if (is_int_type(arg2->type) &&
                               is_uint_type(arg->type)) {
                      better = true;
                    } else if (is_int_type(arg->type) &&
                               is_uint_type(arg2->type)) {
                      as_good = false;
                    }
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


char* type2string(Type* type) {
  if (type->symbol->hasPragma("ref"))
    return getValueType(type)->symbol->name;
  else
    return type->symbol->name;
}


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
      str = stringcat(str, type2string(atypes.v[1]), ".");
    else
      str = stringcat(str, ":", type2string(atypes.v[1]), ".");
  }
  if (!strcmp("this", name))
    _this = true;
  if (!strncmp("_construct_", name, 11)) {
    str = stringcat(str, name+11);
  } else if (!_this) {
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
    VarSymbol* var = dynamic_cast<VarSymbol*>(aparams.v[i]);
    char buff[512];
    if (aparams.v[i] && aparams.v[i]->isTypeVariable)
      str = stringcat(str, type2string(atypes.v[i]));
    else if (var && var->immediate) {
      sprint_imm(buff, *var->immediate);
      str = stringcat(str, buff);
    } else
      str = stringcat(str, ":", type2string(atypes.v[i]));
  }
  if (!call->methodTag)
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
      str = stringcat(":", type2string(fn->getFormal(1)->type));
      start = 1;
    } else {
      str = stringcat(":", type2string(fn->getFormal(2)->type), ".", fn->name);
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
      str = stringcat(str, arg->name, ": ", type2string(arg->type));
  }
  if (!fn->noParens)
    str = stringcat(str, ")");
  return str;
}


static bool
explainCallMatch(CallExpr* call) {
  if (!call->isNamed(fExplainCall))
    return false;
  if (explainModule && explainModule != call->getModule())
    return false;
  if (explainLine != -1 && explainLine != call->lineno)
    return false;
  return true;
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

  if (!call->isResolved())
    call->parentScope->getVisibleFunctions(&visibleFns, canonicalize_string(name));
  else
    visibleFns.add(call->isResolved());

  if (explainLine && explainCallMatch(call)) {
    USR_PRINT(call, "call: %s",
              call2string(call, name, *actual_types,
                          *actual_params, *actual_names));
    if (visibleFns.n == 0)
      USR_PRINT(call, "no visible functions found");
    bool first = true;
    forv_Vec(FnSymbol, visibleFn, visibleFns) {
      USR_PRINT(visibleFn, "%s %s",
                first ? "visible functions are:" : "                      ",
                fn2string(visibleFn));
      first = false;
    }
  }

  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    if (call->methodTag && !visibleFn->noParens)
      continue;
    addCandidate(&candidateFns, &candidateActualFormals, visibleFn,
                 actual_types, actual_params, actual_names);
  }

  if (explainLine && explainCallMatch(call)) {
    if (candidateFns.n == 0)
      USR_PRINT(call, "no candidates found");
    bool first = true;
    forv_Vec(FnSymbol, candidateFn, candidateFns) {
      USR_PRINT(candidateFn, "%s %s",
                first ? "candidates are:" : "               ",
                fn2string(candidateFn));
      first = false;
    }
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

  if (best && explainLine && explainCallMatch(call)) {
    USR_PRINT(best, "best candidate is: %s", fn2string(best));
  }

  if (!best && candidateFns.n > 0) {
    for (int i = 0; i < candidateFns.n; i++) {
      if (candidateFns.v[i]) {
        resolve_call_error_candidates.add(candidateFns.v[i]);
      }
    }
    resolve_call_error = CALL_AMBIGUOUS;
    best = NULL;
  } else if (call->partialTag && (!best || !best->noParens)) {
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

    FnSymbol* promoted = build_promotion_wrapper(best, actual_types, actual_params, call->square);
    if (promoted != best) {
      if (fWarnPromotion) {
        char* str = call2string(call, name, *actual_types, *actual_params, *actual_names);
        USR_WARN(call, "promotion on %s", str);
      }
      best = promoted;
    }
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
  bool stop = false;
  for_exprs_postorder(e, expr) {
    if (expr == e)
      stop = true;
    e = preFold(e);
    if (CallExpr* call = dynamic_cast<CallExpr*>(e)) {
      if (call->parentSymbol) {
        callStack.add(call);
        resolveCall(call);
        FnSymbol* fn = call->isResolved();
        if (fn && call->parentSymbol) {
          resolveFormals(fn);
          if (fn->isParam || fn->retType == dtUnknown)
            resolveFns(fn);
        }
        callStack.pop();
      }
    }
    e = postFold(e);
    if (stop) {
      expr = e;
      break;
    }
  }
  Type* t = expr->typeInfo();
  if (t == dtUnknown)
    INT_FATAL(expr, "Unable to resolve type expression");
  return t;
}


static void
checkUnaryOp(CallExpr* call, Vec<Type*>* atypes, Vec<Symbol*>* aparams) {
  if (call->primitive || call->argList->length() != 1)
    return;
  if (call->isNamed("-")) {
    if (atypes->v[0] == dtUInt[INT_SIZE_64]) {
      USR_FATAL(call, "illegal use of '-' on operand of type %s",
                type2string(atypes->v[0]));
    }
  }
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
      call->isNamed("<=")) {
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
                base->var->name, type2string(atypes->v[0]),
                type2string(atypes->v[1]));
    }
  }
}


static CallExpr*
userCall(CallExpr* call) {
  if (call->getModule()->modtype == MOD_STANDARD) {
    for (int i = callStack.n-1; i >= 0; i--) {
      if (callStack.v[i]->getModule()->modtype != MOD_STANDARD)
        return callStack.v[i];
    }
  }
  return call;
}


static void
makeNoop(CallExpr* call) {
  if (call->baseExpr)
    call->baseExpr->remove();
  while (call->argList->length())
    call->get(1)->remove();
  call->primitive = primitives[PRIMITIVE_NOOP];
}


static void
resolveCall(CallExpr* call) {
  if (!call->primitive) {

    //
    // special case cast of class w/ type variables that is not generic
    //   i.e. type variables are type definitions (have default types)
    //
    for_actuals(actual, call) {
      if (NamedExpr* ne = dynamic_cast<NamedExpr*>(actual))
        actual = ne->actual;
      if (SymExpr* te = dynamic_cast<SymExpr*>(actual)) {
        if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(te->var)) {
          if (ClassType* ct = dynamic_cast<ClassType*>(ts->type)) {
            if (ct->isGeneric) {
              CallExpr* cc = new CallExpr(ct->defaultConstructor->name);
              te->replace(cc);
              resolveCall(cc);
              cc->replace(new SymExpr(cc->typeInfo()->symbol));
            }
          }
        }
      }
    }

    if (SymExpr* sym = dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (dynamic_cast<VarSymbol*>(sym->var) ||
          dynamic_cast<ArgSymbol*>(sym->var)) {
        Expr* base = call->baseExpr;
        base->replace(new SymExpr("this"));
        call->insertAtHead(base);
      }
    }

    if (CallExpr* base = dynamic_cast<CallExpr*>(call->baseExpr)) {
      if (base->partialTag) {
        for_actuals_backward(actual, base) {
          actual->remove();
          call->insertAtHead(actual);
        }
        base->replace(base->baseExpr->remove());
      } else {
        VarSymbol* this_temp = new VarSymbol("this_temp");
        this_temp->isCompilerTemp = true;
        base->replace(new SymExpr("this"));
        CallExpr* move = new CallExpr(PRIMITIVE_MOVE, this_temp, base);
        call->insertAtHead(new SymExpr(this_temp));
        call->getStmtExpr()->insertBefore(new DefExpr(this_temp));
        call->getStmtExpr()->insertBefore(move);
        resolveCall(move);
      }
    }

    Vec<Type*> atypes;
    Vec<Symbol*> aparams;
    Vec<char*> anames;
    computeActuals(call, &atypes, &aparams, &anames);

    checkUnaryOp(call, &atypes, &aparams);
    checkBinaryOp(call, &atypes, &aparams);

    SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);
    char* name = base->var->name;
    FnSymbol* resolvedFn = resolve_call(call, name, &atypes, &aparams, &anames);
    if (call->partialTag) {
      if (!resolvedFn)
        return;
      call->partialTag = false;
    }
    if (resolvedFn && resolvedFn->hasPragma("data set error")) {
      Type* elt_type = dynamic_cast<Type*>(resolvedFn->getFormal(1)->type->substitutions.v[0].value);
      if (!elt_type)
        INT_FATAL(call, "Unexpected substitution of ddata class");
      USR_FATAL(userCall(call), "type mismatch in assignment from %s to %s",
                type2string(atypes.v[3]), type2string(elt_type));
    }
    if (!resolvedFn) {
      if (resolve_call_error == CALL_UNKNOWN || resolve_call_error == CALL_AMBIGUOUS) {
        if (!strcmp("_cast", name)) {
          USR_FATAL(userCall(call), "illegal cast from %s to %s",
                    type2string(atypes.v[1]),
                    type2string(atypes.v[0]));
        } else if (!strcmp("_construct__tuple", name)) {
          SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1));
          if (!sym || !sym->isParam())
            USR_FATAL(userCall(call), "tuple size must be static");
          else
            USR_FATAL(userCall(call), "invalid tuple");
        } else if (!strcmp("=", name)) {
          if (atypes.v[1] == dtNil) {
            USR_FATAL(userCall(call), "type mismatch in assignment of nil to %s",
                      type2string(atypes.v[0]));
          } else {
            USR_FATAL(userCall(call), "type mismatch in assignment from %s to %s",
                      type2string(atypes.v[1]),
                      type2string(atypes.v[0]));
          }
        } else if (!strcmp("this", name)) {
          USR_FATAL_CONT(userCall(call), "%s access of '%s' by '%s'",
                         (resolve_call_error == CALL_AMBIGUOUS) ? "ambiguous" : "unresolved",
                         type2string(atypes.v[0]),
                         call2string(call, name, atypes, aparams, anames));
          USR_STOP();
        } else {
          char* str = call2string(call, name, atypes, aparams, anames);
          USR_FATAL_CONT(userCall(call), "%s call '%s'",
                         (resolve_call_error == CALL_AMBIGUOUS) ? "ambiguous" : "unresolved",
                         str);
          if (resolve_call_error_candidates.n > 0) {
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
            bool printed_one = false;
            forv_Vec(FnSymbol, fn, resolve_call_error_candidates) {
              USR_PRINT(fn, "%s %s",
                        printed_one ? "               " : "candidates are:",
                        fn2string(fn));
              printed_one = true;
            }
          }
          USR_STOP();
        }
      } else {
        INT_FATAL(call, "Error in resolve_call");
      }
    }
    if (call->parentSymbol) {
      call->baseExpr->replace(new SymExpr(resolvedFn));
    }
  } else if (call->isPrimitive(PRIMITIVE_TUPLE_AND_EXPAND)) {
    SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1));
    Symbol* var = dynamic_cast<Symbol*>(sym->var);
    int size = 0;
    for (int i = 0; i < var->type->substitutions.n; i++) {
      if (var->type->substitutions.v[i].key) {
        if (!strcmp("size", dynamic_cast<Symbol*>(var->type->substitutions.v[i].key)->name)) {
          size = dynamic_cast<VarSymbol*>(var->type->substitutions.v[i].value)->immediate->int_value();
          break;
        }
      }
    }
    if (size == 0)
      INT_FATAL(call, "Invalid tuple expand primitive");
    CallExpr* noop = new CallExpr(PRIMITIVE_NOOP);
    call->getStmtExpr()->insertBefore(noop);
    VarSymbol* tmp = gTrue;
    for (int i = 1; i <= size; i++) {
      VarSymbol* tmp1 = new VarSymbol("_expand_temp1");
      tmp1->canParam = true;
      VarSymbol* tmp2 = new VarSymbol("_expand_temp2");
      tmp2->canParam = true;
      VarSymbol* tmp3 = new VarSymbol("_expand_temp3");
      tmp3->canParam = true;
      VarSymbol* tmp4 = new VarSymbol("_expand_temp4");
      tmp4->canParam = true;
      call->getStmtExpr()->insertBefore(new DefExpr(tmp1));
      call->getStmtExpr()->insertBefore(new DefExpr(tmp2));
      call->getStmtExpr()->insertBefore(new DefExpr(tmp3));
      call->getStmtExpr()->insertBefore(new DefExpr(tmp4));
      call->getStmtExpr()->insertBefore(
        new CallExpr(PRIMITIVE_MOVE, tmp1,
          new CallExpr(sym->copy(), new_IntSymbol(i))));
      call->getStmtExpr()->insertBefore(
        new CallExpr(PRIMITIVE_MOVE, tmp2,
          new CallExpr(get_string(call->get(2)), gMethodToken, tmp1)));
      call->getStmtExpr()->insertBefore(
        new CallExpr(PRIMITIVE_MOVE, tmp3,
          new CallExpr("==", tmp2, call->get(3)->copy())));
      call->getStmtExpr()->insertBefore(
        new CallExpr(PRIMITIVE_MOVE, tmp4,
          new CallExpr("&", tmp3, tmp)));
      tmp = tmp4;
    }
    call->replace(new SymExpr(tmp));
    noop->replace(call); // put call back in ast for function resolution
    makeNoop(call);
  } else if (call->isPrimitive(PRIMITIVE_TUPLE_EXPAND)) {
    SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1));
    Symbol* var = dynamic_cast<Symbol*>(sym->var);
    int size = 0;
    for (int i = 0; i < var->type->substitutions.n; i++) {
      if (var->type->substitutions.v[i].key) {
        if (!strcmp("size", dynamic_cast<Symbol*>(var->type->substitutions.v[i].key)->name)) {
          size = dynamic_cast<VarSymbol*>(var->type->substitutions.v[i].value)->immediate->int_value();
          break;
        }
      }
    }
    if (size == 0)
      INT_FATAL(call, "Invalid tuple expand primitive");
    CallExpr* parent = dynamic_cast<CallExpr*>(call->parentExpr);
    CallExpr* noop = new CallExpr(PRIMITIVE_NOOP);
    call->getStmtExpr()->insertBefore(noop);
    for (int i = 1; i <= size; i++) {
      VarSymbol* tmp = new VarSymbol("_expand_temp");
      DefExpr* def = new DefExpr(tmp);
      call->getStmtExpr()->insertBefore(def);
      CallExpr* e = new CallExpr(sym->copy(), new_IntSymbol(i));
      CallExpr* move = new CallExpr(PRIMITIVE_MOVE, tmp, e);
      call->getStmtExpr()->insertBefore(move);
      call->insertBefore(new SymExpr(tmp));
    }
    call->remove();
    noop->replace(call); // put call back in ast for function resolution
    makeNoop(call);
    // increase tuple rank
    if (parent && parent->isNamed("_construct__tuple")) {
      parent->get(1)->replace(new SymExpr(new_IntSymbol(parent->argList->length()-1)));
    }
  } else if (call->isPrimitive(PRIMITIVE_CAST)) {
    Type* t= call->get(1)->typeInfo();
    if (t == dtUnknown)
      INT_FATAL(call, "Unable to resolve type");
    if (t->scalarPromotionType) {
      // ignore for now to handle translation of A op= B of arrays
      // should be an error in general
      //   can't cast to an array type ...
      Expr* castee = call->get(2);
      castee->remove();
      call->replace(castee);
    } else {
      call->get(1)->replace(new SymExpr(t->symbol));
    }
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
        if (t == dtNil && field->type == dtUnknown)
          USR_FATAL(call->parentSymbol, "unable to determine type of field from nil");
        if (field->type == dtUnknown)
          field->type = t;
        if (t != field->type && t != dtNil && t != dtObject)
          USR_FATAL(userCall(call), "cannot assign expression of type %s to field of type %s", type2string(t), type2string(field->type));
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
      if (t == dtVoid) {
        USR_FATAL(call->get(2), "illegal use of function that does not return a value");
      }
      if (t == dtUnknown) {
        if (CallExpr* rhs = dynamic_cast<CallExpr*>(call->get(2))) {
          if (FnSymbol* rhsfn = rhs->isResolved()) {
            USR_FATAL_CONT(rhsfn, "unable to resolve return type of function '%s'", rhsfn->name);
            USR_FATAL(rhs, "called recursively at this point");
          }
        }
      }
      if (t == dtUnknown)
        INT_FATAL(call, "Unable to resolve type");

      // do not resolve function return type yet
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(call->parentSymbol)) {
        if (fn->getReturnSymbol() == sym->var) {
          if (sym->var->isCompilerTemp)
            sym->var->type = dtUnknown;
          return;
        }
      }

      ClassType* ct = dynamic_cast<ClassType*>(sym->var->type);
      if (t == dtNil && sym->var->type != dtNil && (!ct || ct->classTag != CLASS_CLASS))
        USR_FATAL(userCall(call), "type mismatch in assignment from nil to %s",
                  type2string(sym->var->type));
      if (t != dtNil && !(t == sym->var->type || t->refType == sym->var->type || t == sym->var->type->refType) && !isDispatchParent(t, sym->var->type))
        USR_FATAL(userCall(call), "type mismatch in assignment from %s to %s",
                  type2string(t), type2string(sym->var->type));
      if (t != sym->var->type && isDispatchParent(t, sym->var->type)) {
        Expr* rhs = call->get(2);
        rhs->remove();
        call->insertAtTail(new CallExpr(PRIMITIVE_CAST, sym->var->type->symbol, rhs));
      }
    }
  }
}

static bool
formalRequiresTemp(ArgSymbol* formal) {
  if (formal->intent == INTENT_PARAM ||
      formal->intent == INTENT_TYPE ||
      formal->intent == INTENT_REF ||
      formal->name == _this ||
      formal->isTypeVariable ||
      formal->instantiatedParam ||
      formal->type == dtMethodToken)
    return false;
  return true;
}

static void
insertFormalTemps(FnSymbol* fn) {
  if (fn->name == _pass || fn->name == _init ||
      fn->name == _assign || fn->name == _copy ||
      fn->hasPragma("ref"))
    return;
  ASTMap formals2vars;
  for_formals(formal, fn) {
    if (formalRequiresTemp(formal)) {
      VarSymbol* tmp = new VarSymbol(stringcat("_formal_tmp_", formal->name));
      if ((formal->intent == INTENT_BLANK ||
           formal->intent == INTENT_CONST) &&
          !formal->type->symbol->hasPragma("domain") &&
          !formal->type->symbol->hasPragma("array"))
        tmp->consClass = VAR_CONST;
      tmp->isCompilerTemp = true;
      formals2vars.put(formal, tmp);
    }
  }
  if (formals2vars.n > 0) {
    update_symbols(fn->body, &formals2vars);
    Vec<BaseAST*> formals;
    formals2vars.get_keys(formals);
    forv_Vec(BaseAST, ast, formals) {
      ArgSymbol* formal = dynamic_cast<ArgSymbol*>(ast);
      VarSymbol* tmp = dynamic_cast<VarSymbol*>(formals2vars.get(formal));
      if (formal->intent == INTENT_OUT) {
        if (formal->defaultExpr && formal->defaultExpr->typeInfo() != dtNil)
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, formal->defaultExpr->copy()));
        else
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr("_init", formal)));
      } else if (formal->intent == INTENT_INOUT || formal->intent == INTENT_IN)
        fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr("_copy", formal)));
      else
        fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr("_pass", formal)));
      fn->insertAtHead(new DefExpr(tmp));
      if (formal->intent == INTENT_INOUT || formal->intent == INTENT_OUT) {
        fn->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_MOVE, formal, new CallExpr("=", formal, tmp)));
      }
    }
  }
}

static bool
isType(Expr* expr) {
  if (SymExpr* sym = dynamic_cast<SymExpr*>(expr)) {
    if (sym->var->isTypeVariable)
      return true;
    if (dynamic_cast<TypeSymbol*>(sym->var))
      return true;
  } else if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (call->isPrimitive(PRIMITIVE_TYPEOF))
      return true;
  }
  return false;
}

static void fold_param_for(CallExpr* loop) {
  BlockStmt* block = dynamic_cast<BlockStmt*>(loop->parentExpr);
  if (!block || block->blockTag != BLOCK_PARAM_FOR || block->loopInfo != loop)
    INT_FATAL(loop, "bad param loop primitive");
  SymExpr* lse = dynamic_cast<SymExpr*>(loop->get(2));
  SymExpr* hse = dynamic_cast<SymExpr*>(loop->get(3));
  SymExpr* sse = dynamic_cast<SymExpr*>(loop->get(4));
  if (!lse || !hse || !sse)
    INT_FATAL(loop, "bad param loop primitive");
  VarSymbol* lvar = dynamic_cast<VarSymbol*>(lse->var);
  VarSymbol* hvar = dynamic_cast<VarSymbol*>(hse->var);
  VarSymbol* svar = dynamic_cast<VarSymbol*>(sse->var);
  if (!lvar || !hvar || !svar)
    INT_FATAL(loop, "bad param loop primitive");
  if (!lvar->immediate || !hvar->immediate || !svar->immediate)
    INT_FATAL(loop, "bad param loop primitive");
  int64 low = lvar->immediate->int_value();
  int64 high = hvar->immediate->int_value();
  int64 stride = svar->immediate->int_value();
  Expr* index_expr = loop->get(1);
  block->blockTag = BLOCK_NORMAL;
  loop->remove();
  CallExpr* noop = new CallExpr(PRIMITIVE_NOOP);
  block->insertAfter(noop);
  Symbol* index = dynamic_cast<SymExpr*>(index_expr)->var;
  if (stride <= 0)
    INT_FATAL("fix this");
  for (int i = low; i <= high; i += stride) {
    ASTMap map;
    map.put(index, new_IntSymbol(i));
    noop->insertBefore(block->copy(&map));
  }
  block->replace(loop);
  makeNoop(loop);
}

static bool
canExpandIterator(FnSymbol* iterator) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, iterator);
  int count = 0;
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast))
      if (call->isPrimitive(PRIMITIVE_YIELD))
        count++;
  }
  if (count == 1)
    return true;
  return false;
}

static Expr*
expand_for_loop(CallExpr* call) {
  CallExpr* result;
  BlockStmt* block = dynamic_cast<BlockStmt*>(call->parentExpr);
  if (!block || block->loopInfo != call)
    INT_FATAL(call, "bad for loop primitive");
  SymExpr* se1 = dynamic_cast<SymExpr*>(call->get(1));
  SymExpr* se2 = dynamic_cast<SymExpr*>(call->get(2));
  if (!se1 || !se2)
    INT_FATAL(call, "bad for loop primitive");
  VarSymbol* index = dynamic_cast<VarSymbol*>(se1->var);
  VarSymbol* iterator = dynamic_cast<VarSymbol*>(se2->var);
  if (!index || !iterator)
    INT_FATAL(call, "bad for loop primitive");
  if (!fNoExpandIteratorsInlineOpt) {
    if (canExpandIterator(iterator->type->defaultConstructor)) {
      result = call;
      call->primitive = primitives[PRIMITIVE_LOOP_INLINE];
      return result;
    }
  }
  VarSymbol* cursor = new VarSymbol("_cursor");
  cursor->isCompilerTemp = true;
  block->insertBefore(new DefExpr(cursor));
  result = new CallExpr("getHeadCursor", gMethodToken, iterator);
  result->partialTag = true;
  block->insertBefore(new CallExpr(PRIMITIVE_MOVE, cursor, new CallExpr(result)));
  VarSymbol* cond = new VarSymbol("_cond");
  cond->isCompilerTemp = true;
  block->insertBefore(new DefExpr(cond));
  CallExpr* partial = new CallExpr("isValidCursor", gMethodToken, iterator);
  partial->partialTag = true;
  block->insertBefore(new CallExpr(PRIMITIVE_MOVE, cond, new CallExpr(partial, cursor)));

  partial = new CallExpr("getValue", gMethodToken, iterator);
  partial->partialTag = true;
  block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, index,
                                   new CallExpr(partial, cursor)));

  partial = new CallExpr("getNextCursor", gMethodToken, iterator);
  partial->partialTag = true;
  block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cursor, new CallExpr(partial, cursor)));

  partial = new CallExpr("isValidCursor", gMethodToken, iterator);
  partial->partialTag = true;
  block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cond, new CallExpr(partial, cursor)));

  call->replace(new CallExpr(PRIMITIVE_LOOP_FOR, cond));
  return result;
}

static Expr* fold_cond_stmt(CondStmt* if_stmt) {
  Expr* result = NULL;
  if (SymExpr* cond = dynamic_cast<SymExpr*>(if_stmt->condExpr)) {
    if (VarSymbol* var = dynamic_cast<VarSymbol*>(cond->var)) {
      if (var->immediate &&
          var->immediate->const_kind == NUM_KIND_UINT &&
          var->immediate->num_index == INT_SIZE_1) {
        result = new CallExpr(PRIMITIVE_NOOP);
        if_stmt->insertBefore(result);
        if (var->immediate->v_bool == gTrue->immediate->v_bool) {
          Expr* then_stmt = if_stmt->thenStmt;
          then_stmt->remove();
          if_stmt->replace(then_stmt);
        } else if (var->immediate->v_bool == gFalse->immediate->v_bool) {
          Expr* else_stmt = if_stmt->elseStmt;
          if (else_stmt) {
            else_stmt->remove();
            if_stmt->replace(else_stmt);
          } else {
            if_stmt->remove();
          }
        }
      }
    }
  }
  return result;
}

static Expr* dropUnnecessaryCast(CallExpr* call) {
  // Check for and remove casts to the original type and size
  Expr* result = call;
  if (!call->isNamed("_cast"))
    INT_FATAL("dropUnnecessaryCasts called on non _cast call");

  if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2))) {
    if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var)) {
      if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
        Type* src = var->type;
        Type* dst = sym->var->type;

        if ((is_int_type(src) && is_int_type(dst)) ||
            (is_uint_type(src) && is_uint_type(dst)) ||
            (is_real_type(src) && is_real_type(dst)) ||
            (is_imag_type(src) && is_imag_type(dst)) ||
            (is_complex_type(src) && is_complex_type(dst))) {
          if (get_width(src) == get_width(dst)) {
            result = new SymExpr(var);
            call->replace(result);
          }
        } else if (dst == dtString && src == dtString) {
          result = new SymExpr(var);
          call->replace(result);
        }
      }
    } else if (EnumSymbol* e = dynamic_cast<EnumSymbol*>(sym->var)) {
      if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
        EnumType* src = dynamic_cast<EnumType*>(e->type);
        EnumType* dst = dynamic_cast<EnumType*>(sym->var->type);
        if (dst) {
          if (!strcmp(src->symbol->name, dst->symbol->name)) {
            result = new SymExpr(e);
            call->replace(result);
          }
        }
      }
    }
  }
  return result;
}

static Expr*
preFold(Expr* expr) {
  Expr* result = expr;
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (SymExpr* sym = dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(sym->var)) {
        if (call->argList->length() == 1) {
          if (SymExpr* arg = dynamic_cast<SymExpr*>(call->get(1))) {
            if (VarSymbol* var = dynamic_cast<VarSymbol*>(arg->var)) {
              if (var->immediate) {
                if (NUM_KIND_INT == var->immediate->const_kind ||
                    NUM_KIND_UINT == var->immediate->const_kind) {
                  int size;
                  if (NUM_KIND_INT == var->immediate->const_kind) {
                    size = var->immediate->int_value();
                  } else {
                    size = (int)var->immediate->uint_value();
                  }
                  TypeSymbol* tsize = NULL;
                  if (type == dtInt[INT_SIZE_32]->symbol) {
                    switch (size) {
                    case 8: tsize = dtInt[INT_SIZE_8]->symbol; break;
                    case 16: tsize = dtInt[INT_SIZE_16]->symbol; break;
                    case 32: tsize = dtInt[INT_SIZE_32]->symbol; break;
                    case 64: tsize = dtInt[INT_SIZE_64]->symbol; break;
                    default:
                      USR_FATAL( call, "illegal size %d for int", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtUInt[INT_SIZE_32]->symbol) {
                    switch (size) {
                    case  8: tsize = dtUInt[INT_SIZE_8]->symbol;  break;
                    case 16: tsize = dtUInt[INT_SIZE_16]->symbol; break;
                    case 32: tsize = dtUInt[INT_SIZE_32]->symbol; break;
                    case 64: tsize = dtUInt[INT_SIZE_64]->symbol; break;
                    default:
                      USR_FATAL( call, "illegal size %d for uint", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtReal[FLOAT_SIZE_64]->symbol) {
                    switch (size) {
                    case 32:  tsize = dtReal[FLOAT_SIZE_32]->symbol;  break;
                    case 64:  tsize = dtReal[FLOAT_SIZE_64]->symbol;  break;
                    case 128: tsize = dtReal[FLOAT_SIZE_128]->symbol; break;
                    default:
                      USR_FATAL( call, "illegal size %d for imag", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtImag[FLOAT_SIZE_64]->symbol) {
                    switch (size) {
                    case 32:  tsize = dtImag[FLOAT_SIZE_32]->symbol;  break;
                    case 64:  tsize = dtImag[FLOAT_SIZE_64]->symbol;  break;
                    case 128: tsize = dtImag[FLOAT_SIZE_128]->symbol; break;
                    default:
                      USR_FATAL( call, "illegal size %d for imag", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtComplex[COMPLEX_SIZE_128]->symbol) {
                    switch (size) {
                    case 64:  tsize = dtComplex[COMPLEX_SIZE_64]->symbol;  break;
                    case 128: tsize = dtComplex[COMPLEX_SIZE_128]->symbol; break;
                    case 256: tsize = dtComplex[COMPLEX_SIZE_256]->symbol; break;
                    default:
                      USR_FATAL( call, "illegal size %d for complex", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  }
                }
              }
            }
          }
        }
      }
    }

    if (call->argList->length() == 2) {
      if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->get(1))) {
        if (symExpr->var == gMethodToken) {
          Type* type = call->get(2)->typeInfo();
          if (type->symbol->hasPragma("ref"))
            type = getValueType(type);
          Vec<BaseAST*> keys;
          type->substitutions.get_keys(keys);
          forv_Vec(BaseAST, key, keys) {
            if (Symbol* var = dynamic_cast<Symbol*>(key)) {
              if (call->isNamed(var->name)) {
                if (Symbol* value = dynamic_cast<Symbol*>(type->substitutions.get(key))) {
                  result = new SymExpr(value);
                  call->replace(result);
                } else if (Type* value = dynamic_cast<Type*>(type->substitutions.get(key))) {
                  if (var->isTypeVariable) {
                    result = new SymExpr(value->symbol);
                    call->replace(result);
                  }
                }
              }
            } else if (Type* var = dynamic_cast<Type*>(key)) {
              INT_FATAL("type key encountered");
              if (call->isNamed(var->symbol->name)) {
                if (Type* value = dynamic_cast<Type*>(type->substitutions.get(key))) {
                  result = new SymExpr(value->symbol);
                  call->replace(result);
                }
              }
            }
          }
        }
      }
    }
    if (call->isNamed("_init")) {
      if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
        TypeSymbol* ts = dynamic_cast<TypeSymbol*>(sym->var);
        if (!ts && sym->var->isTypeVariable)
          ts = sym->var->type->symbol;
        if (ts) {
          if (ts->hasPragma("ref"))
            ts = getValueType(ts->type)->symbol;
          if (ts->type->defaultValue == gNil)
            result = new CallExpr("_cast", ts, ts->type->defaultValue);
          else if (ts->type->defaultValue)
            result = new SymExpr(ts->type->defaultValue);
          else if (ts->type->defaultConstructor)
            result = new CallExpr(ts->type->defaultConstructor);
          else
            INT_FATAL(ts, "type has neither defaultValue nor defaultConstructor");
          call->replace(result);
        }
      }
    } else if (call->isNamed("_copy")) {
      if (call->argList->length() == 1) {
        if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->get(1))) {
          if (VarSymbol* var = dynamic_cast<VarSymbol*>(symExpr->var)) {
            if (var->immediate) {
              result = new SymExpr(var);
              call->replace(result);
            }
          } else {
            if (EnumSymbol* var = dynamic_cast<EnumSymbol*>(symExpr->var)) {
              // Treat enum values as immediates
              result = new SymExpr(var);
              call->replace(result);
            }
          }
        }
      }
    } else if (call->isNamed("_cast")) {
      result = dropUnnecessaryCast(call);
      if (result == call) {
        // The cast was not dropped.  Remove integer casts on immediate values.
        if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2))) {
          if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var)) {
            if (var->immediate) {
              if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
                Type* src = var->type;
                Type* dst = sym->var->type;
                if (!is_real_type(dst) && !is_real_type(src) &&
                    !is_imag_type(dst) && !is_imag_type(src) &&
                    !is_complex_type(dst) && !is_complex_type(src) &&
                    dst != dtString && src != dtString) {
                  VarSymbol* typevar = 
                    dynamic_cast<VarSymbol*>(dst->defaultValue);
                  if (!typevar || !typevar->immediate)
                    INT_FATAL("unexpected case in cast_fold");

                  Immediate coerce = *typevar->immediate;
                  coerce_immediate(var->immediate, &coerce);
                  result = new SymExpr(new_ImmediateSymbol(&coerce));
                  call->replace(result);
                }
              }
            }
          }
        }
      }
    } else if (call->isNamed("==")) {
      if (isType(call->get(1)) || isType(call->get(2))) {
        Type* lt = call->get(1)->typeInfo();
        Type* rt = call->get(2)->typeInfo();
        if (lt != dtUnknown && rt != dtUnknown &&
            !lt->isGeneric && !rt->isGeneric) {
          result = (lt == rt) ? new SymExpr(gTrue) : new SymExpr(gFalse);
          call->replace(result);
        }
      }
    } else if (call->isNamed("!=")) {
      if (isType(call->get(1)) || isType(call->get(2))) {
        Type* lt = call->get(1)->typeInfo();
        Type* rt = call->get(2)->typeInfo();
        if (lt != dtUnknown && rt != dtUnknown &&
            !lt->isGeneric && !rt->isGeneric) {
          result = (lt != rt) ? new SymExpr(gTrue) : new SymExpr(gFalse);
          call->replace(result);
        }
      }
    } else if (call->isNamed("_construct__tuple") && !call->isResolved()) {
      if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
        if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var)) {
          if (var->immediate) {
            int rank = var->immediate->int_value();
            if (rank != call->argList->length() - 1) {
              if (call->argList->length() != 2)
                INT_FATAL(call, "bad homogeneous tuple");
              Expr* actual = call->get(2);
              for (int i = 1; i < rank; i++) {
                call->insertAtTail(actual->copy());
              }
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_LOOP_PARAM)) {
      fold_param_for(call);
    } else if (call->isPrimitive(PRIMITIVE_LOOP_FOR) &&
               call->argList->length() == 2) {
      result = expand_for_loop(call);
    } else if (call->isPrimitive(PRIMITIVE_LOGICAL_FOLDER)) {
      bool removed = false;
      SymExpr* sym1 = dynamic_cast<SymExpr*>(call->get(1));
      if (VarSymbol* sym = dynamic_cast<VarSymbol*>(sym1->var)) {
        if (sym->immediate || paramMap.get(sym)) {
          CallExpr* mvCall = dynamic_cast<CallExpr*>(call->parentExpr);
          SymExpr* sym = dynamic_cast<SymExpr*>(mvCall->get(1));
          VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var);
          removed = true;
          var->canParam = true;
          result = call->get(2)->remove();
          call->replace(result);
        }
      }
      if (!removed) {
        result = call->get(2)->remove();
        call->replace(result);
      }
    } else if (call->isPrimitive(PRIMITIVE_SET_REF)) {
      // remove set ref if already a reference
      if (call->get(1)->typeInfo()->symbol->hasPragma("ref")) {
        result = call->get(1)->remove();
        call->replace(result);
      } else if (CallExpr* move = dynamic_cast<CallExpr*>(call->parentExpr)) {
        // check legal var function return
        if (move->isPrimitive(PRIMITIVE_MOVE)) {
          SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1));
          if (lhs->var == move->getFunction()->getReturnSymbol()) {
            SymExpr* ret = dynamic_cast<SymExpr*>(call->get(1));
            assert(ret);
            if (ret->var->defPoint->getFunction() == move->getFunction())
              USR_FATAL(ret, "illegal return expression in var function");
          }
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_GET_REF)) {
      // remove get ref if already a value
      if (!call->get(1)->typeInfo()->symbol->hasPragma("ref")) {
        result = call->get(1)->remove();
        call->replace(result);
      }
    }
  }
  return result;
}

static void foldEnumOp(int op, EnumSymbol *e1, EnumSymbol *e2, Immediate *imm) {
  long val1 = -1, val2 = -1, count = 0;
  // ^^^ This is an assumption that "long" on the compiler host is at
  // least as big as "int" on the target.  This is not guaranteed to be true.
  EnumType *type1, *type2;

  type1 = dynamic_cast<EnumType*>(e1->type);
  type2 = dynamic_cast<EnumType*>(e2->type);
  assert(type1 && type2);

  // Loop over the enum values to find the int value of e1
  for_alist(DefExpr, constant, type1->constants) {
    if (!get_int(constant->init, &count)) {
      count++;
    }
    if (constant->sym == e1) {
      val1 = count;
      break;
    }
  }
  // Loop over the enum values to find the int value of e2
  count = 0;
  for_alist(DefExpr, constant, type2->constants) {
    if (!get_int(constant->init, &count)) {
      count++;
    }
    if (constant->sym == e2) {
      val2 = count;
      break;
    }
  }

  // All operators on enum types result in a bool
  imm->const_kind = NUM_KIND_UINT;
  imm->num_index = INT_SIZE_1;
  switch (op) {
    default: INT_FATAL("fold constant op not supported"); break;
    case P_prim_equal:
      imm->v_bool = val1 == val2;
      break;
    case P_prim_notequal:
      imm->v_bool = val1 != val2;
      break;
    case P_prim_less:
      imm->v_bool = val1 < val2;
      break;
    case P_prim_lessorequal:
      imm->v_bool = val1 <= val2;
      break;
    case P_prim_greater:
      imm->v_bool = val1 > val2;
      break;
    case P_prim_greaterorequal:
      imm->v_bool = val1 >= val2;
      break;
  }
}

#define FOLD_CALL1(prim)                                                \
  if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {            \
    if (VarSymbol* lhs = dynamic_cast<VarSymbol*>(sym->var)) {          \
      if (lhs->immediate) {                                             \
        Immediate i3;                                                   \
        fold_constant(prim, lhs->immediate, NULL, &i3);                 \
        result = new SymExpr(new_ImmediateSymbol(&i3));                 \
        call->replace(result);                                          \
      }                                                                 \
    }                                                                   \
  }

#define FOLD_CALL2(prim)                                                \
  if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {            \
    if (VarSymbol* lhs = dynamic_cast<VarSymbol*>(sym->var)) {          \
      if (lhs->immediate) {                                             \
        if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2))) {      \
          if (VarSymbol* rhs = dynamic_cast<VarSymbol*>(sym->var)) {    \
            if (rhs->immediate) {                                       \
              Immediate i3;                                             \
              fold_constant(prim, lhs->immediate, rhs->immediate, &i3); \
              result = new SymExpr(new_ImmediateSymbol(&i3));           \
              call->replace(result);                                    \
            }                                                           \
          }                                                             \
        }                                                               \
      }                                                                 \
    } else if (EnumSymbol* lhs = dynamic_cast<EnumSymbol*>(sym->var)) { \
      if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2))) {        \
        if (EnumSymbol* rhs = dynamic_cast<EnumSymbol*>(sym->var)) {    \
          Immediate imm;                                                \
          foldEnumOp(prim, lhs, rhs, &imm);                             \
          result = new SymExpr(new_ImmediateSymbol(&imm));              \
          call->replace(result);                                        \
        }                                                               \
      }                                                                 \
    }                                                                   \
  }

static bool
isSubType(Type* sub, Type* super) {
  if (sub == super)
    return true;
  forv_Vec(Type, parent, sub->dispatchParents) {
    if (isSubType(parent, super))
      return true;
  }
  return false;
}

static void
insertValueTemp(Expr* insertPoint, Expr* actual) {
  if (SymExpr* se = dynamic_cast<SymExpr*>(actual)) {
    if (!se->var->type->refType) {
      VarSymbol* tmp = new VarSymbol("tmp", getValueType(se->var->type));
      tmp->isCompilerTemp = true;
      insertPoint->insertBefore(new DefExpr(tmp));
      insertPoint->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_REF, se->var)));
      se->var = tmp;
    }
  }
}

static Expr*
postFold(Expr* expr) {
  Expr* result = expr;
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (FnSymbol* fn = call->isResolved()) {
      if (fn->isParam || fn->canParam) {
        VarSymbol* ret = dynamic_cast<VarSymbol*>(fn->getReturnSymbol());
        if (ret->immediate) {
          result = new SymExpr(ret);
          expr->replace(result);
        } else if (fn->isParam) {
          USR_FATAL(call, "param function does not resolve to a param symbol");
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
      bool set = false;
      if (SymExpr* lhs = dynamic_cast<SymExpr*>(call->get(1))) {
        if (lhs->var->canParam || lhs->var->isParam()) {
          if (paramMap.get(lhs->var))
            INT_FATAL(call, "parameter set multiple times");
          if (SymExpr* rhs = dynamic_cast<SymExpr*>(call->get(2))) {
            if (VarSymbol* rhsVar = dynamic_cast<VarSymbol*>(rhs->var)) {
              if (rhsVar->immediate) {
                paramMap.put(lhs->var, rhsVar);
                lhs->var->defPoint->remove();
                makeNoop(call);
                set = true;
              }
            } else if (EnumSymbol* rhsv = dynamic_cast<EnumSymbol*>(rhs->var)) {
              paramMap.put(lhs->var, rhsv);
              lhs->var->defPoint->remove();
              makeNoop(call);
              set = true;
            } 
          }
          if (!set && lhs->var->isParam())
            USR_FATAL(call, "Initializing parameter '%s' to value not known at compile time", lhs->var->name);
        }
        if (!set && lhs->var->canType) {
          if (SymExpr* rhs = dynamic_cast<SymExpr*>(call->get(2))) {
            if (rhs->var->isTypeVariable)
              lhs->var->isTypeVariable = true;
          }
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
      Type* baseType = call->get(1)->typeInfo();
      if (baseType->symbol->hasPragma("ref"))
        baseType = getValueType(baseType);
      char* memberName = get_string(call->get(2));
      Symbol* sym = baseType->getField(memberName);
      if (sym->isParam()) {
        Vec<BaseAST*> keys;
        baseType->substitutions.get_keys(keys);
        forv_Vec(BaseAST, key, keys) {
          if (Symbol* var = dynamic_cast<Symbol*>(key)) {
            if (!strcmp(sym->name, var->name)) {
              if (Symbol* value = dynamic_cast<Symbol*>(baseType->substitutions.get(key))) {
                result = new SymExpr(value);
                call->replace(result);
              }
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_ISSUBTYPE)) {
      if (isType(call->get(1)) || isType(call->get(2))) {
        Type* lt = call->get(2)->typeInfo(); // a:t cast is cast(t,a)
        Type* rt = call->get(1)->typeInfo();
        if (lt != dtUnknown && rt != dtUnknown && lt != dtAny &&
            rt != dtAny && !lt->isGeneric) {
          bool is_true = false;
          if (lt->instantiatedFrom == rt)
            is_true = true;
          if (isSubType(lt, rt))
            is_true = true;
          result = (is_true) ? new SymExpr(gTrue) : new SymExpr(gFalse);
          call->replace(result);
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_IS_ENUM)) {
      // Replace the "isEnumType" primitive with true if the type is
      // an enum, otherwise with false
      bool is_enum = false;
      CallExpr* c;
      SymExpr* symExpr;
      if ((c = dynamic_cast<CallExpr*>(call->get(1))) &&
          (c->isPrimitive(PRIMITIVE_TYPEOF))) {
        symExpr = dynamic_cast<SymExpr*>(c->get(1));
      } else {
        symExpr = dynamic_cast<SymExpr*>(call->get(1));
      }
      if (symExpr && dynamic_cast<EnumType*>(symExpr->var->type)) {
        is_enum = true;
      }
      result = (is_enum) ? new SymExpr(gTrue) : new SymExpr(gFalse);
      call->replace(result);
    } else if (call->isPrimitive(PRIMITIVE_IS_TUPLE)) {
      bool is_tuple = false;
      if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
        Symbol* typeSym;
        if (!sym->var->type->refType)
          typeSym = getValueType(sym->var->type)->symbol;
        else
          typeSym = sym->var->type->symbol;
        if (typeSym->hasPragma("tuple")) {
          is_tuple = true;
        }
      }
      result = (is_tuple) ? new SymExpr(gTrue) : new SymExpr(gFalse);
      call->replace(result);
    } else if (call->isPrimitive(PRIMITIVE_UNARY_MINUS)) {
      FOLD_CALL1(P_prim_minus);
    } else if (call->isPrimitive(PRIMITIVE_UNARY_PLUS)) {
      FOLD_CALL1(P_prim_plus);
    } else if (call->isPrimitive(PRIMITIVE_UNARY_NOT)) {
      FOLD_CALL1(P_prim_not);
    } else if (call->isPrimitive(PRIMITIVE_UNARY_LNOT)) {
      FOLD_CALL1(P_prim_lnot);
    } else if (call->isPrimitive(PRIMITIVE_ADD)) {
      FOLD_CALL2(P_prim_add);
    } else if (call->isPrimitive(PRIMITIVE_SUBTRACT)) {
      FOLD_CALL2(P_prim_subtract);
    } else if (call->isPrimitive(PRIMITIVE_MULT)) {
      FOLD_CALL2(P_prim_mult);
    } else if (call->isPrimitive(PRIMITIVE_DIV)) {
      FOLD_CALL2(P_prim_div);
    } else if (call->isPrimitive(PRIMITIVE_MOD)) {
      FOLD_CALL2(P_prim_mod);
    } else if (call->isPrimitive(PRIMITIVE_EQUAL)) {
      FOLD_CALL2(P_prim_equal);
    } else if (call->isPrimitive(PRIMITIVE_NOTEQUAL)) {
      FOLD_CALL2(P_prim_notequal);
    } else if (call->isPrimitive(PRIMITIVE_LESSOREQUAL)) {
      FOLD_CALL2(P_prim_lessorequal);
    } else if (call->isPrimitive(PRIMITIVE_GREATEROREQUAL)) {
      FOLD_CALL2(P_prim_greaterorequal);
    } else if (call->isPrimitive(PRIMITIVE_LESS)) {
      FOLD_CALL2(P_prim_less);
    } else if (call->isPrimitive(PRIMITIVE_GREATER)) {
      FOLD_CALL2(P_prim_greater);
    } else if (call->isPrimitive(PRIMITIVE_AND)) {
      FOLD_CALL2(P_prim_and);
    } else if (call->isPrimitive(PRIMITIVE_OR)) {
      FOLD_CALL2(P_prim_or);
    } else if (call->isPrimitive(PRIMITIVE_XOR)) {
      FOLD_CALL2(P_prim_xor);
    } else if (call->isPrimitive(PRIMITIVE_POW)) {
      FOLD_CALL2(P_prim_pow);
    } else if (call->isPrimitive(PRIMITIVE_LSH)) {
      FOLD_CALL2(P_prim_lsh);
    } else if (call->isPrimitive(PRIMITIVE_RSH)) {
      FOLD_CALL2(P_prim_rsh);
    } else if (call->isPrimitive(PRIMITIVE_LOOP_C_FOR)) {
      insertValueTemp(call->parentExpr, call->get(2));
      insertValueTemp(call->parentExpr, call->get(3));
      insertValueTemp(call->parentExpr, call->get(4));
    }
  } else if (SymExpr* sym = dynamic_cast<SymExpr*>(expr)) {
    if (Symbol* val = paramMap.get(sym->var)) {
      if (sym->var->type != dtUnknown && sym->var->type != val->type) {
        CallExpr* cast = new CallExpr("_cast", sym->var, val);
        sym->replace(cast);
        result = preFold(cast);
      } else {
        sym->var = val;
      }
    }
  }

  if (CondStmt* cond = dynamic_cast<CondStmt*>(result->parentExpr)) {
    if (cond->condExpr == result) {
      if (Expr* expr = fold_cond_stmt(cond)) {
        result = expr;
      }
    }
  }
  return result;
}

static void
resolveBody(Expr* body) {
  for_exprs_postorder(expr, body) {
    if (SymExpr* sym = dynamic_cast<SymExpr*>(expr))
      makeRefType(sym->var->type);
    expr = preFold(expr);
    if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
      if (call->isPrimitive(PRIMITIVE_ERROR)) {
        CallExpr* from = NULL;
        for (int i = callStack.n-1; i >= 0; i--) {
          from = callStack.v[i];
          if (from->lineno > 0 && from->getModule()->modtype != MOD_STANDARD)
            break;
        }
        char* str = "";
        for_actuals(actual, call) {
          if (SymExpr* sym = dynamic_cast<SymExpr*>(actual)) {
            if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var)) {
              if (var->immediate &&
                  var->immediate->const_kind == CONST_KIND_STRING) {
                str = stringcat(str, var->immediate->v_string);
                continue;
              }
            }
            if (sym->var->isTypeVariable) {
              str = stringcat(str, sym->var->type->symbol->name);
              continue;
            }
          }
          if (CallExpr* call = dynamic_cast<CallExpr*>(actual)) {
            if (call->isPrimitive(PRIMITIVE_TYPEOF)) {
              str = stringcat(str, call->get(1)->typeInfo()->symbol->name);
            }
          }
        }
        USR_FATAL(from, "%s", str);
      }
      callStack.add(call);
      resolveCall(call);
      if (call->isResolved())
        resolveFns(call->isResolved());
      callStack.pop();
    } else if (SymExpr* sym = dynamic_cast<SymExpr*>(expr)) {
      if (ClassType* ct = dynamic_cast<ClassType*>(sym->var->type)) {
        if (!ct->isGeneric && !ct->symbol->hasPragma("iterator class")) {
          resolveFormals(ct->defaultConstructor);
          resolveFns(ct->defaultConstructor);
        }
      }
    }
    expr = postFold(expr);
  }
}

static void
resolveFns(FnSymbol* fn) {
  if (resolvedFns.set_in(fn))
    return;
  resolvedFns.set_add(fn);

  insertFormalTemps(fn);

  resolveBody(fn->body);

  if (fn->fnClass == FN_CONSTRUCTOR)
    setFieldTypes(fn);

  Symbol* ret = fn->getReturnSymbol();
  Type* retType = ret->type;

  Vec<Type*> retTypes;
  Vec<Symbol*> retParams;

  for_exprs_postorder(expr, fn->body) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
          if (sym->var == ret) {
            if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2)))
              retParams.add(sym->var);
            else
              retParams.add(NULL);
            retTypes.add(call->get(2)->typeInfo());
          }
        }
      }
    }
  }

  if (retType == dtUnknown) {
    if (retTypes.n == 1)
      retType = retTypes.v[0];
    else if (retTypes.n > 1) {
      for (int i = 0; i < retTypes.n; i++) {
        bool best = true;
        for (int j = 0; j < retTypes.n; j++) {
          if (retTypes.v[i] != retTypes.v[j]) {
            if (!canCoerce(retTypes.v[j], retParams.v[j], retTypes.v[i], fn))
              best = false;
          }
        }
        if (best) {
          retType = retTypes.v[i];
          break;
        }
      }
    }
  }

  ret->type = retType;
  if (!fn->iteratorInfo) {
    fn->retType = retType;
    if (retTypes.n == 0 && fn->retType == dtUnknown)
      fn->retType = ret->type = dtVoid;
    else if (retType == dtUnknown)
      INT_FATAL(fn, "Unable to resolve return type");
  }

  if (fn->fnClass == FN_ITERATOR && !fn->iteratorInfo) {
    prototypeIteratorClass(fn);
    resolvedFns.set_add(fn->iteratorInfo->getHeadCursor);
    resolvedFns.set_add(fn->iteratorInfo->getNextCursor);
    resolvedFns.set_add(fn->iteratorInfo->isValidCursor);
    resolvedFns.set_add(fn->iteratorInfo->getValue);
    resolvedFns.set_add(fn->iteratorInfo->getZipCursor1);
    resolvedFns.set_add(fn->iteratorInfo->getZipCursor2);
    resolvedFns.set_add(fn->iteratorInfo->getZipCursor3);
    resolvedFns.set_add(fn->iteratorInfo->getZipCursor4);
  }

  if (fn->fnClass == FN_CONSTRUCTOR) {
    forv_Vec(Type, parent, fn->retType->dispatchParents) {
      if (dynamic_cast<ClassType*>(parent) && parent != dtValue && parent != dtObject && parent->defaultConstructor) {
        resolveFormals(parent->defaultConstructor);
        resolveFns(parent->defaultConstructor);
      }
    }
    if (ClassType* ct = dynamic_cast<ClassType*>(fn->retType)) {
      for_fields(field, ct) {
        if (ClassType* fct = dynamic_cast<ClassType*>(field->type)) {
          if (fct->defaultConstructor) {
            resolveFormals(fct->defaultConstructor);
            resolveFns(fct->defaultConstructor);
          }
        }
      }
    }
  }
}


static bool
possible_signature_match(FnSymbol* fn, FnSymbol* gn) {
  if (fn->name != gn->name)
    return false;
  if (fn->formals->length() != gn->formals->length())
    return false;
  for (int i = 3; i <= fn->formals->length(); i++) {
    ArgSymbol* fa = fn->getFormal(i);
    ArgSymbol* ga = gn->getFormal(i);
    if (strcmp(fa->name, ga->name))
      return false;
  }
  return true;
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
    if (cfn && possible_signature_match(pfn, cfn)) {
      if (ct->isGeneric) {
        ASTMap subs;
        forv_Vec(FnSymbol, cons, *ct->defaultConstructor->instantiatedTo) {
          subs.put(cfn->getFormal(2), cons->retType);
          for (int i = 3; i <= cfn->formals->length(); i++) {
            ArgSymbol* arg = cfn->getFormal(i);
            if (arg->intent == INTENT_PARAM) {
              INT_FATAL(arg, "unhandled case");
            } else if (arg->type->isGeneric) {
              if (!pfn->getFormal(i))
              subs.put(arg, pfn->getFormal(i)->type);
            }
          }
          FnSymbol* icfn = instantiate(cfn, &subs);
          resolveFormals(icfn);
          if (signature_match(pfn, icfn)) {
            resolveFns(icfn);
            if (icfn->retType != pfn->retType) {
              USR_FATAL_CONT(pfn, "conflicting return type specified for '%s: %s'", fn2string(pfn), pfn->retType->symbol->name);
              USR_FATAL_CONT(icfn, "  overridden by '%s: %s'", fn2string(icfn), icfn->retType->symbol->name);
              USR_STOP();
            }
            Vec<FnSymbol*>* fns = ddf.get(pfn);
            if (!fns) fns = new Vec<FnSymbol*>();
            fns->add(icfn);
            ddf.put(pfn, fns);
          }
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
          cfn = instantiate(cfn, &subs);
        resolveFormals(cfn);
        if (signature_match(pfn, cfn)) {
          resolveFns(cfn);
          if (cfn->retType != pfn->retType) {
            USR_FATAL_CONT(pfn, "conflicting return type specified for '%s: %s'", fn2string(pfn), pfn->retType->symbol->name);
            USR_FATAL_CONT(cfn, "  overridden by '%s: %s'", fn2string(cfn), cfn->retType->symbol->name);
            USR_STOP();
          }
          Vec<FnSymbol*>* fns = ddf.get(pfn);
          if (!fns) fns = new Vec<FnSymbol*>();
          fns->add(cfn);
          ddf.put(pfn, fns);
        }
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

void normalize_nested_logical_function_expressions(DefExpr* def);

void
resolve() {
  explainLine = 0;
  if (strcmp(fExplainCall, "")) {
    char *token, *str1 = NULL, *str2 = NULL;
    token = strstr(fExplainCall, ":");
    if (token) {
      *token = '\0';
      str1 = token+1;
      token = strstr(str1, ":");
      if (token) {
        *token = '\0';
        str2 = token + 1;
      }
    }
    if (str1) {
      if (str2 || !atoi(str1)) {
        forv_Vec(ModuleSymbol, mod, allModules) {
          if (!strcmp(mod->name, str1)) {
            explainModule = mod;
            break;
          }
        }
        if (!explainModule)
          USR_FATAL("invalid module name '%s' passed to --explain-call flag", str1);
      } else
        explainLine = atoi(str1);
      if (str2)
        explainLine = atoi(str2);
    }
    if (explainLine == 0)
      explainLine = -1;
  }

  // call _nilType nil so as to not confuse the user
  dtNil->symbol->name = gNil->name;

  _init = astr("_init");
  _pass = astr("_pass");
  _copy = astr("_copy");
  _this = astr("this");
  _assign = astr("=");

  bool changed = true;
  while (changed) {
    changed = false;
    forv_Vec(FnSymbol, fn, gFns) {
      changed = tag_generic(fn) || changed;
    }
  }

  resolveFns(chpl_main);

  // need to handle enumerated types better
  forv_Vec(TypeSymbol, type, gTypes) {
    if (EnumType* et = dynamic_cast<EnumType*>(type->type)) {
      for_alist(DefExpr, def, et->constants) {
        if (def->init) {
          resolve_type_expr(def->init);
        }
      }
    }
  }

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

  if (fPrintDispatch) {
    printf("dynamic dispatch functions:\n");
    for (int i = 0; i < ddf.n; i++) {
      if (ddf.v[i].key) {
        printf("  %s\n", fn2string(ddf.v[i].key));
        for (int j = 0; j < ddf.v[i].value->n; j++) {
          printf("    %s\n", fn2string(ddf.v[i].value->v[j]));
        }
      }
    }
  }

  Vec<CallExpr*> calls;
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast))
      if (call->parentSymbol)
        calls.add(call);
  }
  forv_Vec(CallExpr, call, calls) {
    if (FnSymbol* key = call->isResolved()) {
      if (Vec<FnSymbol*>* fns = ddf.get(key)) {
        forv_Vec(FnSymbol, fn, *fns) {
          Type* type = fn->getFormal(2)->type;
          CallExpr* subcall = call->copy();
          SymExpr* tmp = new SymExpr(gNil);
          FnSymbol* if_fn = new FnSymbol("_if_fn");
          if_fn->addPragma("inline");
          VarSymbol* _ret = new VarSymbol("_ret_if_fn_dd", key->retType);
          _ret->isCompilerTemp = true;
          if_fn->insertAtTail(new DefExpr(_ret));
          if_fn->insertAtTail(
            new CondStmt(
              new CallExpr(PRIMITIVE_GETCID,
                           call->get(2)->copy(),
                           new_IntSymbol(type->id)),
              new CallExpr(PRIMITIVE_MOVE, _ret, subcall),
              new CallExpr(PRIMITIVE_MOVE, _ret, tmp)));
          if_fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, _ret));
          if_fn->retType = key->retType;
          if (key->retType == dtUnknown)
            INT_FATAL(call, "bad parent virtual function return type");
          call->getStmtExpr()->insertBefore(new DefExpr(if_fn));
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

  pruneResolvedTree();
}


//
// remove arguments to calls; handle temporary normalization by
// recursively removing temporaries.
//
static void
removeActual(Expr* actual) {
  if (SymExpr* sym = dynamic_cast<SymExpr*>(actual)) {
    if (sym->var->isCompilerTemp) {
      forv_Vec(SymExpr, lhs, sym->var->defs) {
        if (CallExpr* call = dynamic_cast<CallExpr*>(lhs->parentExpr))
          if (call->parentSymbol && call->isPrimitive(PRIMITIVE_MOVE))
            call->remove();
      }
    }
  }
}


//
// pruneResolvedTree -- prunes and cleans the AST after all of the
// function calls and types have been resolved
//
static void
pruneResolvedTree() {
  // Remove unused functions
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      if (!resolvedFns.set_in(fn) || fn->isParam)
        fn->defPoint->remove();
    }
  }

  // Remove unused types
  forv_Vec(TypeSymbol, type, gTypes) {
    if (type->defPoint && type->defPoint->parentSymbol)
      if (!type->hasPragma("ref"))
        if (ClassType* ct = dynamic_cast<ClassType*>(type->type))
          if (!resolvedFns.set_in(ct->defaultConstructor))
            ct->symbol->defPoint->remove();
  }
  forv_Vec(TypeSymbol, type, gTypes) {
    if (type->defPoint && type->defPoint->parentSymbol)
      if (type->hasPragma("ref"))
        if (ClassType* ct = dynamic_cast<ClassType*>(getValueType(type->type)))
          if (!resolvedFns.set_in(ct->defaultConstructor))
            type->defPoint->remove();
  }

  compute_sym_uses();

  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    Expr* expr = dynamic_cast<Expr*>(ast);
    if (!expr || !expr->parentSymbol)
      continue;
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      // Remove unused global variables
      if (dynamic_cast<VarSymbol*>(def->sym))
        if (dynamic_cast<ModuleSymbol*>(def->parentSymbol))
          if (def->sym->type == dtUnknown)
            def->remove();
    } else if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isPrimitive(PRIMITIVE_NOOP)) {
        // Remove Noops
        call->remove();
      } else if (call->isPrimitive(PRIMITIVE_TYPEOF)) {
        // Replace PRIMITIVE_TYPEOF with argument
        call->replace(call->get(1)->remove());
      } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER) ||
                 call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        // Remove member accesses of types
        // Replace string literals with field symbols in member primitives
        Type* baseType = call->get(1)->typeInfo();
        if (!call->parentSymbol->hasPragma("ref") &&
            baseType->symbol->hasPragma("ref"))
          baseType = getValueType(baseType);
        char* memberName = get_string(call->get(2));
        Symbol* sym = baseType->getField(memberName);
        if (sym->isTypeVariable && call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
          if (sym->type->defaultValue)
            call->replace(new SymExpr(sym->type->defaultValue));
          else
            call->replace(new CallExpr(sym->type->defaultConstructor));
        } else if (sym->isTypeVariable || sym->isParam() ||
                   !strcmp(sym->name, "_promotionType"))
          call->remove();
        else
          call->get(2)->replace(new SymExpr(sym));
      } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
        // Remove types to enable --baseline
        SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2));
        if (sym && dynamic_cast<TypeSymbol*>(sym->var))
          call->remove();
      } else if (call->isNamed("_init")) {
        // Special handling of array constructors via array pragma
        if (ClassType* ct = dynamic_cast<ClassType*>(call->typeInfo())) {
          if (!ct->symbol->hasPragma("array")) {
            if (ct->defaultValue) {
              removeActual(call->get(1));
              call->replace(new CallExpr(PRIMITIVE_CAST, ct->symbol, gNil));
            } else if (!ct->symbol->hasPragma("array"))
              call->replace(call->get(1)->remove());
          }
        }
      } else if (FnSymbol* fn = call->isResolved()) {
        // Remove method token actuals
        for (int i = fn->formals->length(); i >= 1; i--) {
          ArgSymbol* formal = fn->getFormal(i);
          if (formal->type == dtMethodToken ||
              formal->isTypeVariable)
            call->get(i)->remove();
        }
      }
    } else if (NamedExpr* named = dynamic_cast<NamedExpr*>(ast)) {
      // Remove names of named actuals
      Expr* actual = named->actual;
      actual->remove();
      named->replace(actual);
    } else if (BlockStmt* block = dynamic_cast<BlockStmt*>(ast)) {
      // Remove type blocks--code that exists only to determine types
      if (block->blockTag == BLOCK_TYPE)
        block->remove();
    }
  }

  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      for_formals(formal, fn) {
        // Remove formal default values
        if (formal->defaultExpr)
          formal->defaultExpr->remove();
        // Remove formal type expressions
        if (formal->defPoint->exprType)
          formal->defPoint->exprType->remove();
        // Remove method token formals
        if (formal->type == dtMethodToken)
          formal->defPoint->remove();
        if (formal->isTypeVariable) {
          formal->defPoint->remove();
          VarSymbol* tmp = new VarSymbol("_removed_formal_tmp", formal->type);
          tmp->isCompilerTemp = true;
          fn->insertAtHead(new DefExpr(tmp));
          ASTMap map;
          map.put(formal, tmp);
          update_symbols(fn->body, &map);
        }
      }
    }
  }

  // Remove type fields, parameter fields, and _promotionType field
  forv_Vec(TypeSymbol, type, gTypes) {
    if (type->defPoint && type->defPoint->parentSymbol) {
      if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
        for_fields(field, ct) {
          if (field->isTypeVariable || field->isParam() ||
              !strcmp(field->name, "_promotionType"))
            field->defPoint->remove();
        }
      }
    }
  }

  //
  // Insert return temps for functions that return values if no
  // variable captures the result.
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->parentSymbol) {
        if (FnSymbol* fn = call->isResolved()) {
          if (fn->retType != dtVoid) {
            CallExpr* parent = dynamic_cast<CallExpr*>(call->parentExpr);
            if (!parent) { // no use
              VarSymbol* tmp = new VarSymbol("_dummy", fn->retType);
              DefExpr* def = new DefExpr(tmp);
              call->insertBefore(def);
              def->insertAfter(new CallExpr(PRIMITIVE_MOVE, tmp, call->remove()));
            }
          }
        }
      }
    }
  }

  //
  // Insert reference temps for function arguments that expect them.
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->parentSymbol && call->isResolved()) {
        for_formals_actuals(formal, actual, call) {
          if (formal->type == actual->typeInfo()->refType) {
            currentLineno = call->lineno;
            currentFilename = call->filename;
            VarSymbol* tmp = new VarSymbol("_tmp", formal->type);
            tmp->isCompilerTemp = true;
            call->getStmtExpr()->insertBefore(new DefExpr(tmp));
            actual->replace(new SymExpr(tmp));
            call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_SET_REF, actual)));
          }
        }
      }
    }
  }
}


static bool
is_array_type(Type* type) {
  forv_Vec(Type, t, type->dispatchParents) {
    if (t->symbol->hasPragma("BaseArray"))
      return true;
    else if (is_array_type(t))
      return true;
  }
  return false;
}


static void
fixTypeNames(ClassType* ct) {
  if (is_array_type(ct)) {
    char* domain_type = ct->getField("dom")->type->symbol->name;
    char* elt_type = ct->getField("eltType")->type->symbol->name;
    ct->symbol->defPoint->parentScope->undefine(ct->symbol);
    ct->symbol->name = astr("[", domain_type, "] ", elt_type);
    ct->symbol->defPoint->parentScope->define(ct->symbol);
  }
  if (ct->instantiatedFrom &&
      !strcmp(ct->instantiatedFrom->symbol->name, "SingleLocaleArithmeticDomain")) {
    ct->symbol->defPoint->parentScope->undefine(ct->symbol);
    ct->symbol->name = astr("domain", ct->symbol->name+28);
    ct->symbol->defPoint->parentScope->define(ct->symbol);
  }
  if (ct->symbol->hasPragma("array") || ct->symbol->hasPragma("domain")) {
    char* name = ct->getField(1)->type->symbol->name;
    ct->symbol->defPoint->parentScope->undefine(ct->symbol);
    ct->symbol->name = name;
    ct->symbol->defPoint->parentScope->define(ct->symbol);
  }
}


static void
setFieldTypes(FnSymbol* fn) {
  ClassType* ct = dynamic_cast<ClassType*>(fn->retType);
  if (!ct)
    INT_FATAL(fn, "Constructor has no class type");
  //  for_formals(formal, fn) {
//     Type* t = formal->type;
//     if (t == dtUnknown && formal->defPoint->exprType)
//       t = formal->defPoint->exprType->typeInfo();
//     if (t == dtUnknown)
//       INT_FATAL(formal, "Unable to resolve field type");
//     if (t == dtNil)
//       USR_FATAL(formal, "unable to determine type of field from nil");
//     bool found = false;
//     if (!strcmp(formal->name, "_mt")) {
//       continue;
//     }
//     if (!strcmp(formal->name, "this")) {
//       continue;
//     }
  for_fields(field, ct) {
    //    if (!strcmp(field->name, formal->name)) {
      //        field->type = t;
      //        found = true;
      if (!strcmp(field->name, "_promotionType"))
        ct->scalarPromotionType = field->type;
      //    }
  }
    //    if (!found)
    //      INT_FATAL(formal, "Unable to find field in constructor");
    //  }
  fixTypeNames(ct);
}


static FnSymbol*
instantiate(FnSymbol* fn, ASTMap* subs) {
  FnSymbol* ifn = fn->instantiate_generic(subs);
  if (!ifn->isGeneric && ifn->where) {
    resolveFormals(ifn);
    resolveBody(ifn->where);
    SymExpr* symExpr = dynamic_cast<SymExpr*>(ifn->where->body->last());
    if (!symExpr)
      USR_FATAL(ifn->where, "Illegal where clause");
    if (!strcmp(symExpr->var->name, "false"))
      return NULL;
    if (strcmp(symExpr->var->name, "true"))
      USR_FATAL(ifn->where, "Illegal where clause");
  }
  return ifn;
}

