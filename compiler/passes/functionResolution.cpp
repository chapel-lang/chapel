#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "iterator.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "../ifa/prim_data.h"


class CallInfo {
 public:
  CallExpr*        call;        // call expression
  const char*      name;        // function name
  Vec<Symbol*>     actualSyms;  // symbols of actuals
  Vec<Type*>       actualTypes; // types of actuals
  Vec<const char*> actualNames; // named arguments
  CallInfo(CallExpr* icall);
};


CallInfo::CallInfo(CallExpr* icall) : call(icall) {
  name = toSymExpr(call->baseExpr)->getName();
  for_actuals(actual, call) {
    Type* t = actual->typeInfo();
    if (t == dtUnknown || t->isGeneric)
      INT_FATAL(call, "actual type is unknown or generic");
    actualTypes.add(t);
    if (NamedExpr* named = toNamedExpr(actual)) {
      actualNames.add(named->name);
      actual = named->actual;
    } else {
      actualNames.add(NULL);
    }
    if (SymExpr* se = toSymExpr(actual))
      actualSyms.add(se->var);
    else
      actualSyms.add(NULL);
  }
}


Map<Symbol*,Symbol*> paramMap;
static Expr* dropUnnecessaryCast(CallExpr* call);
static void foldEnumOp(int op, EnumSymbol *e1, EnumSymbol *e2, Immediate *imm);
static Expr* preFold(Expr* expr);
static Expr* postFold(Expr* expr);

static FnSymbol* instantiate(FnSymbol* fn, ASTMap* subs, CallExpr* call);

static void setScalarPromotionType(ClassType* ct);
static void fixTypeNames(ClassType* ct);
static void insertReturnTemps();
static bool canParamCoerce(Type* actualType, Symbol* actualSym, Type* formalType);

static int explainCallLine;
static ModuleSymbol* explainCallModule;
static int explainInstantiationLine;
static ModuleSymbol* explainInstantiationModule;

static Vec<CallExpr*> inits;
static Vec<FnSymbol*> resolvedFns;
static Vec<FnSymbol*> resolvedFormals;
Vec<CallExpr*> callStack;

static Map<CallExpr*,FnSymbol*> buildArrayMap;
static Map<Type*,FnSymbol*> wrapDomainMap;

static Map<FnSymbol*,Vec<FnSymbol*>*> ddf; // map of functions to
                                           // virtual children

// types contains the types of the actuals
// names contains the name if it is a named argument, otherwise NULL
// e.g.  foo(arg1=12, "hi");
//  types = int, string
//  names = arg1, NULL
static FnSymbol* resolve_call(CallInfo* info, BlockStmt* scope = NULL);
static Type* resolve_type_expr(Expr* expr);

static void resolveCall(CallExpr* call);
static void resolveBody(Expr* body);
static void resolveFns(FnSymbol* fn);

static bool canDispatch(Type* actualType,
                        Symbol* actualSym,
                        Type* formalType,
                        FnSymbol* fn = NULL,
                        bool* require_scalar_promotion = NULL,
                        bool paramCoerce = false);

static void pruneResolvedTree();


//
// build reference type
//
static void makeRefType(Type* type) {
  if (!type || type == dtMethodToken || type == dtUnknown)
    return;

  if (type->refType || type->isGeneric || type->symbol->hasPragma("ref"))
    return;

  //  DefExpr* def = new DefExpr(new VarSymbol("tmp", type));
  CallInfo info(new CallExpr("_type_construct__ref", type->symbol));
  // add call to AST temporarily
  if (type->defaultConstructor) {
    if (type->defaultConstructor->instantiationPoint)
      type->defaultConstructor->instantiationPoint->insertAtHead(info.call);
    else
      type->symbol->defPoint->insertBefore(info.call);
  } else
    chpl_main->insertAtHead(info.call);
  FnSymbol* fn = resolve_call(&info);
  info.call->remove();
  type->refType = toClassType(fn->retType);
  type->refType->getField(1)->type = type;
}


static void
resolveSpecifiedReturnType(FnSymbol* fn) {
  fn->retType = resolve_type_expr(fn->retExprType);
  if (fn->retType != dtUnknown) {
    if (fn->retTag == RET_VAR) {
      makeRefType(fn->retType);
      fn->retType = fn->retType->refType;
    }
    fn->retExprType->remove();
    if (fn->fnTag == FN_ITERATOR) {
      prototypeIteratorClass(fn);
      makeRefType(fn->retType);
      resolvedFns.set_add(fn->iteratorInfo->advance);
      resolvedFns.set_add(fn->iteratorInfo->hasMore);
      resolvedFns.set_add(fn->iteratorInfo->getValue);
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
      if (formal->type == dtUnknown) {
        resolveBody(formal->typeExpr);
        formal->type = formal->typeExpr->body.tail->typeInfo();
      }

      //
      // change type of this on record methods to reference type
      //
      if (formal->intent == INTENT_INOUT ||
          formal->intent == INTENT_OUT ||
          (formal == fn->_this &&
           (isUnionType(formal->type) ||
            isRecordType(formal->type) || fn->hasPragma("ref this")))) {
        makeRefType(formal->type);
        formal->type = formal->type->refType;
      }
    }
    if (fn->retExprType)
      resolveSpecifiedReturnType(fn);

    resolvedFormals.set_add(fn);
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
canCoerce(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn, bool* require_scalar_promotion = NULL) {
  if (actualType->symbol->hasPragma( "sync")) {
    if (actualType->isGeneric) {
      return false;
    } else {
      Type *base_type = (Type*)(actualType->substitutions.v[0].value);
      return canDispatch(base_type, actualSym, formalType, fn, require_scalar_promotion);
    }
  }

  if (actualType->symbol->hasPragma("ref"))
    return canDispatch(getValueType(actualType), actualSym, formalType, fn, require_scalar_promotion);

  if (is_int_type(formalType)) {
    if (toEnumType(actualType))
      return true;
    if (actualType == dtBool)
      return true;
    if (is_int_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;
    if (is_uint_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;
    if (get_width(formalType) < 64)
      if (VarSymbol* var = toVarSymbol(actualSym))
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
    if (VarSymbol* var = toVarSymbol(actualSym))
      if (var->immediate)
        if (fits_in_uint(get_width(formalType), var->immediate))
          return true;
  }
  if (is_real_type(formalType)) {
    if ((is_int_type(actualType) || is_uint_type(actualType))
        && get_width(formalType) >= 64)
      return true;
    if (is_real_type(actualType) && 
        get_width(actualType) < get_width(formalType))
      return true;
  }
  if (is_complex_type(formalType)) {
    if ((is_int_type(actualType) || is_uint_type(actualType))
        && get_width(formalType) >= 128)
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
    if (toEnumType(actualType))
      return true;
  }
  return false;
}

//
// Check if actualType can be coerced to formalType at compile time. This is
// more restrictive than canCoerce, since, for example, a real(32) cannot be
// coerced to a real(64) at compile time. 
//
static bool canParamCoerce(Type* actualType, Symbol* actualSym, Type* formalType) {
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
      if (VarSymbol* var = toVarSymbol(actualSym))
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
    if (VarSymbol* var = toVarSymbol(actualSym))
      if (var->immediate)
        if (fits_in_uint(get_width(formalType), var->immediate))
          return true;
  }
  if (is_complex_type(formalType)) {
    if (is_real_type(actualType) &&
        (get_width(actualType) == get_width(formalType)/2))
      return true;
    if (is_imag_type(actualType) &&
        (get_width(actualType) == get_width(formalType)/2))
      return true;
  }
  if (formalType == dtString) {
    if (is_int_type(actualType) || is_uint_type(actualType) ||
        actualType == dtBool)
      return true;
  }
  return false;
}


// Returns true iff the actualType can dispatch to the formalType.
// The function symbol is used to avoid scalar promotion on =.
// param is set if the actual is a parameter (compile-time constant).
static bool
canDispatch(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn, bool* require_scalar_promotion, bool paramCoerce) {
  if (require_scalar_promotion)
    *require_scalar_promotion = false;
  if (actualType == formalType)
    return true;
  if (actualType == dtNil && formalType == dtObject)
    return true;
  if (actualType == dtNil)
    if (ClassType* ct = toClassType(formalType))
      if (ct->classTag == CLASS_CLASS)
        return true;
  if (actualType->refType == formalType)
    return true;
  if (!paramCoerce && canCoerce(actualType, actualSym, formalType, fn, require_scalar_promotion))
    return true;
  if (paramCoerce && canParamCoerce(actualType, actualSym, formalType))
    return true;
  forv_Vec(Type, parent, actualType->dispatchParents) {
    if (parent == formalType || canDispatch(parent, actualSym, formalType, fn, require_scalar_promotion)) {
      return true;
    }
  }
  if (fn &&
      strcmp(fn->name, "=") && 
      actualType->scalarPromotionType && 
      (canDispatch(actualType->scalarPromotionType, actualSym, formalType, fn))) {
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
                       Vec<Type*>* formalActuals,
                       Vec<Symbol*>* formalSyms,
                       Vec<ArgSymbol*>* actualFormals,
                       int num_actuals,
                       int num_formals,
                       Vec<Type*>* actualTypes,
                       Vec<Symbol*>* actualSyms,
                       Vec<const char*>* actualNames) {
  for (int i = 0; i < num_formals; i++) {
    formalActuals->add(NULL);
    formalSyms->add(NULL);
  }
  for (int i = 0; i < num_actuals; i++)
    actualFormals->add(NULL);
  for (int i = 0; i < num_actuals; i++) {
    if (actualNames->v[i]) {
      bool match = false;
      int j = -1;
      for_formals(formal, fn) {
        j++;
        if (!strcmp(actualNames->v[i], formal->name)) {
          match = true;
          actualFormals->v[i] = formal;
          formalActuals->v[j] = actualTypes->v[i];
          formalSyms->v[j] = actualSyms->v[i];
          break;
        }
      }
      if (!match)
        return false;
    }
  }
  for (int i = 0; i < num_actuals; i++) {
    if (!actualNames->v[i]) {
      bool match = false;
      int j = -1;
      for_formals(formal, fn) {
        if (formal->variableExpr)
          return (fn->isGeneric) ? true : false;
        j++;
        if (!formalActuals->v[j]) {
          match = true;
          actualFormals->v[i] = formal;
          formalActuals->v[j] = actualTypes->v[i];
          formalSyms->v[j] = actualSyms->v[i];
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
                   Vec<Type*>* formalActuals,
                   Vec<Symbol*>* formalSyms) {
  int i = 0;
  for_formals(formal, fn) {
    if (formal->intent == INTENT_PARAM) {
      if (formalSyms->v[i] && formalSyms->v[i]->isParameter()) {
        if (!formal->type->isGeneric ||
            canInstantiate(formalActuals->v[i], formal->type))
          subs.put(formal, formalSyms->v[i]);
      } else if (formal->defaultExpr) {

        // break because default expression may reference generic
        // arguments earlier in formal list; make those substitutions
        // first (test/classes/bradc/paramInClass/weirdParamInit4)
        if (subs.n)
          break;

        resolveBody(formal->defaultExpr);
        if (SymExpr* se = toSymExpr(formal->defaultExpr->body.tail)) {
          if (se->var->isParameter())
            subs.put(formal, se->var);
        } else
          INT_FATAL(fn, "unable to handle default parameter");
      }
    } else if (formal->type->isGeneric) {

      //
      // check for field with specified generic type
      //
      if (!formal->isTypeVariable && strcmp(formal->name, "outer") && strcmp(formal->name, "meme") && formal->type != dtAny && (fn->hasPragma("default constructor") || fn->hasPragma("type constructor")))
        USR_FATAL(formal, "invalid generic type specification on class field");

      if (formalActuals->v[i]) {
        if (canInstantiate(formalActuals->v[i], formal->type)) {
          subs.put(formal, formalActuals->v[i]);
        } else if (Type* st = formalActuals->v[i]->scalarPromotionType) {
          if (canInstantiate(st, formal->type))
            subs.put(formal, st);
        } else if (Type* vt = getValueType(formalActuals->v[i])) {
          if (canInstantiate(vt, formal->type))
            subs.put(formal, vt);
          else if (Type* st = vt->scalarPromotionType)
            if (canInstantiate(st, formal->type))
              subs.put(formal, st);
        }
      } else if (formal->defaultExpr) {

        // break because default expression may reference generic
        // arguments earlier in formal list; make those substitutions
        // first (test/classes/bradc/genericTypes)
        if (subs.n)
          break;

        resolveBody(formal->defaultExpr);
        Type* defaultType = formal->defaultExpr->body.tail->typeInfo();
        if (canInstantiate(defaultType, formal->type) ||
            defaultType == gNil->type) { // constructor default
          subs.put(formal, defaultType);
        }
      }
    }
    i++;
  }
}


static FnSymbol*
expandVarArgs(FnSymbol* fn, int numActuals) {
  static Map<FnSymbol*,Vec<FnSymbol*>*> cache;

  bool genericArg = false;
  for_formals(arg, fn) {
    if (!genericArg && arg->variableExpr &&
        !toDefExpr(arg->variableExpr))
      resolve_type_expr(arg->variableExpr);

    //
    // set genericArg to true if a generic argument appears before the
    // argument with the variable expression
    //
    if (arg->type->isGeneric)
      genericArg = true;

    // handle unspecified variable number of arguments
    if (DefExpr* def = toDefExpr(arg->variableExpr)) {

      // check for cached stamped out function
      if (Vec<FnSymbol*>* cfns = cache.get(fn)) {
        forv_Vec(FnSymbol, cfn, *cfns) {
          if (cfn->numFormals() == numActuals)
            return cfn;
        }
      }

      int numCopies = numActuals - fn->numFormals() + 1;
      if (numCopies <= 0)
        return NULL;

      Map<Symbol*,Symbol*> map;
      FnSymbol* newFn = fn->copy(&map);
      newFn->isExtern = fn->isExtern; // preserve externess of expanded varargs fn
      newFn->visible = false;
      fn->defPoint->insertBefore(new DefExpr(newFn));
      Symbol* sym = toSymbol(map.get(def->sym));
      sym->defPoint->replace(new SymExpr(new_IntSymbol(numCopies)));

      SymbolMap update;
      update.put(sym, new_IntSymbol(numCopies));
      update_symbols(newFn, &update);

      // add new function to cache
      Vec<FnSymbol*>* cfns = cache.get(fn);
      if (!cfns)
        cfns = new Vec<FnSymbol*>();
      cfns->add(newFn);
      cache.put(fn, cfns);

      return expandVarArgs(newFn, numActuals);
    } else if (SymExpr* sym = toSymExpr(arg->variableExpr)) {

      // handle specified number of variable arguments
      if (VarSymbol* n_var = toVarSymbol(sym->var)) {
        if (n_var->type == dtInt[INT_SIZE_32] && n_var->immediate) {
          int n = n_var->immediate->int_value();
          CallExpr* tupleCall = new CallExpr((arg->isTypeVariable) ? "_type_construct__tuple" : "_construct__tuple");
          for (int i = 0; i < n; i++) {
            DefExpr* new_arg_def = arg->defPoint->copy();
            ArgSymbol* new_arg = toArgSymbol(new_arg_def->sym);
            new_arg->variableExpr = NULL;
            tupleCall->insertAtTail(new SymExpr(new_arg));
            new_arg->name = astr("_e", istr(i), "_", arg->name);
            new_arg->cname = astr("_e", istr(i), "_", arg->cname);
            arg->defPoint->insertBefore(new_arg_def);
          }
          VarSymbol* var = new VarSymbol(arg->name);
          var->isTypeVariable = arg->isTypeVariable;

          if (arg->intent == INTENT_OUT || arg->intent == INTENT_INOUT) {
            int i = 1;
            for_actuals(actual, tupleCall) {
              VarSymbol* tmp = new VarSymbol("_tmp");
              fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
              fn->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(var, new_IntSymbol(i))));
              fn->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_MOVE, actual->copy(), new CallExpr("=", actual->copy(), tmp)));
              i++;
            }
          }

          tupleCall->insertAtHead(new_IntSymbol(n));
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, var, tupleCall));
          fn->insertAtHead(new DefExpr(var));
          arg->defPoint->remove();
          SymbolMap update;
          update.put(arg, var);
          update_symbols(fn->body, &update);
          if (fn->where) {
            VarSymbol* var = new VarSymbol(arg->name);
            var->isTypeVariable = arg->isTypeVariable;
            fn->where->insertAtHead(
                                    new CallExpr(PRIMITIVE_MOVE, var, tupleCall->copy()));
            fn->where->insertAtHead(new DefExpr(var));
            SymbolMap update;
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
             Vec<Type*>* actualTypes,
             Vec<Symbol*>* actualSyms,
             Vec<const char*>* actualNames,
             CallExpr* call) {
  fn = expandVarArgs(fn, actualTypes->n);

  if (!fn)
    return;

  Vec<ArgSymbol*>* actualFormals = new Vec<ArgSymbol*>();

  int num_actuals = actualTypes->n;
  int num_formals = fn->numFormals();

  Vec<Type*> formalActuals;
  Vec<Symbol*> formalSyms;
  bool valid = computeActualFormalMap(fn, &formalActuals, &formalSyms,
                                      actualFormals, num_actuals,
                                      num_formals, actualTypes,
                                      actualSyms, actualNames);
  if (!valid) {
    delete actualFormals;
    return;
  }

  if (fn->isGeneric) {
    ASTMap subs;
    computeGenericSubs(subs, fn, &formalActuals, &formalSyms);
    if (subs.n) {
      FnSymbol* inst_fn = instantiate(fn, &subs, call);
      if (inst_fn)
        addCandidate(candidateFns, candidateActualFormals, inst_fn, actualTypes, actualSyms, actualNames, call);
    }
    delete actualFormals;
    return;
  }

  if (fn->isGeneric)
    INT_FATAL(fn, "unexpected generic function");

  //
  // make sure that type constructor is resolved before other constructors
  //
  if (fn->hasPragma("default constructor")) {
    CallExpr* typeConstructorCall = new CallExpr(astr("_type", fn->name));
    for_formals(formal, fn) {
      if (strcmp(formal->name, "meme")) {
        if (fn->_this->type->symbol->hasPragma("tuple")) {
          if (formal->instantiatedFrom) {
            typeConstructorCall->insertAtTail(formal->type->symbol);
          } else if (formal->instantiatedParam) {
            typeConstructorCall->insertAtTail(paramMap.get(formal));
          }
        } else {
          if (!strcmp(formal->name, "outer") || formal->type == dtMethodToken) {
            typeConstructorCall->insertAtTail(formal);
          } else if (formal->instantiatedFrom) {
            typeConstructorCall->insertAtTail(new NamedExpr(formal->name, new SymExpr(formal->type->symbol)));
          } else if (formal->instantiatedParam) {
            typeConstructorCall->insertAtTail(new NamedExpr(formal->name, new SymExpr(paramMap.get(formal))));
          }
        }
      }
    }
    call->insertBefore(typeConstructorCall);
    resolveCall(typeConstructorCall);
    INT_ASSERT(typeConstructorCall->isResolved());
    resolveFns(typeConstructorCall->isResolved());
    fn->_this->type = typeConstructorCall->isResolved()->retType;
    typeConstructorCall->remove();
  }

  resolveFormals(fn);

  int j = -1;
  for_formals(formal, fn) {
    j++;
    if (!strcmp(fn->name, "=")) {
      if (j == 0) {
        if (formalActuals.v[j] != formal->type &&
            getValueType(formalActuals.v[j]) != formal->type) {
          delete actualFormals;
          return;
        }
      }
    }
    if (formalActuals.v[j] &&
        !canDispatch(formalActuals.v[j], formalSyms.v[j], formal->type, fn, NULL, formal->instantiatedParam)) {
      delete actualFormals;
      return;
    }
    if (formalSyms.v[j] && formalSyms.v[j]->isTypeVariable && !formal->isTypeVariable) {
      delete actualFormals;
      return;
   }
    if (formalSyms.v[j] && !formalSyms.v[j]->isTypeVariable && formal->isTypeVariable) {
      delete actualFormals;
      return;
    }
    if (!formalActuals.v[j] && !formal->defaultExpr) {
      delete actualFormals;
      return;
    }
  }
  candidateFns->add(fn);
  candidateActualFormals->add(actualFormals);
}


static FnSymbol*
build_default_wrapper(FnSymbol* fn,
                      Vec<ArgSymbol*>* actualFormals,
                      bool isSquare) {
  FnSymbol* wrapper = fn;
  int num_actuals = actualFormals->n;
  int num_formals = fn->numFormals();
  if (num_formals > num_actuals) {
    Vec<Symbol*> defaults;
    for_formals(formal, fn) {
      bool used = false;
      forv_Vec(ArgSymbol, arg, *actualFormals) {
        if (arg == formal)
          used = true;
      }
      if (!used)
        defaults.add(formal);
    }
    wrapper = fn->default_wrapper(&defaults, &paramMap, isSquare);

    // update actualFormals for use in build_order_wrapper
    int j = 1;
    for_formals(formal, fn) {
      for (int i = 0; i < actualFormals->n; i++) {
        if (actualFormals->v[i] == formal) {
          ArgSymbol* newFormal = wrapper->getFormal(j);
          actualFormals->v[i] = newFormal;
          j++;
        }
      }
    }
  }
  return wrapper;
}


static FnSymbol*
build_order_wrapper(FnSymbol* fn,
                    Vec<ArgSymbol*>* actualFormals,
                    bool isSquare) {
  bool order_wrapper_required = false;
  Map<Symbol*,Symbol*> formals_to_formals;
  int i = 0;
  for_formals(formal, fn) {
    i++;

    int j = 0;
    forv_Vec(ArgSymbol, af, *actualFormals) {
      j++;
      if (af == formal) {
        if (i != j)
          order_wrapper_required = true;
        formals_to_formals.put(formal, actualFormals->v[i-1]);
      }
    }
  }
  if (order_wrapper_required) {
    fn = fn->order_wrapper(&formals_to_formals, isSquare);
  }
  return fn;
}


static FnSymbol*
build_coercion_wrapper(FnSymbol* fn,
                       Vec<Type*>* actualTypes,
                       Vec<Symbol*>* actualSyms,
                       bool isSquare) {
  ASTMap subs;
  Map<ArgSymbol*,bool> coercions;
  int j = -1;
  bool coerce = false;
  for_formals(formal, fn) {
    j++;
    Type* actualType = actualTypes->v[j];
    Symbol* actualSym = actualSyms->v[j];
    if (actualType != formal->type) {
      if (canCoerce(actualType, actualSym, formal->type, fn) || isDispatchParent(actualType, formal->type)) {
        subs.put(formal, actualType->symbol);
        coercions.put(formal,true);
        coerce = true;
      } else {
        subs.put(formal, actualType->symbol);
      }
    }
  }
  if (coerce)
    fn = fn->coercion_wrapper(&subs, &coercions, isSquare);
  return fn;  
}


static FnSymbol*
build_promotion_wrapper(FnSymbol* fn,
                        Vec<Type*>* actualTypes,
                        Vec<Symbol*>* actualSyms,
                        bool isSquare) {
  if (!strcmp(fn->name, "="))
    return fn;
  bool promotion_wrapper_required = false;
  Map<Symbol*,Symbol*> promoted_subs;
  int j = -1;
  for_formals(formal, fn) {
    j++;
    Type* actualType = actualTypes->v[j];
    Symbol* actualSym = actualSyms->v[j];
    bool require_scalar_promotion = false;
    if (canDispatch(actualType, actualSym, formal->type, fn, &require_scalar_promotion)){
      if (require_scalar_promotion) {
        promotion_wrapper_required = true;
        promoted_subs.put(formal, actualType->symbol);
      }
    }
  }
  if (promotion_wrapper_required) {
    fn = fn->promotion_wrapper(&promoted_subs, isSquare);
    resolveFormals(fn);
  }
  return fn;
}


//
// d is distance up via parentExpr/parentSymbol
// md is number of modules used
//
//  sjd: not sure this is perfect; can this be computed when visible
//  functions are grabbed for a negligible cost
//
static int
visibility_distance(Expr* expr, FnSymbol* fn,
                    Vec<BlockStmt*>& visited,
                    int& md, int d=1) {
  if (BlockStmt* block = toBlockStmt(expr)) {

    if (visited.set_in(block))
      return 0;
    visited.set_add(block);

    if (fn->defPoint->parentExpr == block)
      return d;

    int new_dd = 0;
    int new_md = 0;
    forv_Vec(ModuleSymbol, module, block->modUses) {
      Vec<BlockStmt*> visitedCopy;
      visitedCopy.copy(visited);
      int try_md = md + 1;
      int try_dd = visibility_distance(module->block, fn, visitedCopy, try_md, d);
      if (try_dd > 0 && (new_md == 0 || try_md < new_md)) {
        new_md = try_md;
        new_dd = try_dd;
      }
    }
    if (new_dd > 0 && new_md > 0) {
      md = new_md;
      return new_dd;
    }

    if (expr->parentExpr)
      return visibility_distance(expr->parentExpr, fn, visited, md, d+1);
    else if (expr->parentSymbol->defPoint)
      return visibility_distance(expr->parentSymbol->defPoint->parentExpr, fn, visited, md, d+1);

    return 0;

  } else {
    return visibility_distance(expr->parentExpr, fn, visited, md, d+1);
  }
}


static void
disambiguate_by_scope(Expr* parent, Vec<FnSymbol*>* candidateFns) {
  Vec<int> vds;
  Vec<int> vmds;
  forv_Vec(FnSymbol, fn, *candidateFns) {
    Vec<BlockStmt*> visited;
    int md = 0;
    vds.add(visibility_distance(parent, fn, visited, md));
    vmds.add(md);
  }
  int ld = 0, lmd = 0;
  for (int i = 0; i < vds.n; i++) {
    if (vds.v[i] != 0) {
      if (ld) {
        if (vds.v[i] < ld) {
          ld = vds.v[i];
          lmd = vmds.v[i];
        } else if (vds.v[i] == ld && vmds.v[i] < lmd) {
          lmd = vmds.v[i];
        }
      } else {
        ld = vds.v[i];
        lmd = vmds.v[i];
      }
    }
  }
  for (int i = 0; i < vds.n; i++) {
    if (vds.v[i] != ld || vmds.v[i] != lmd)
      candidateFns->v[i] = 0;
  }
}


static FnSymbol*
disambiguate_by_match(Vec<FnSymbol*>* candidateFns,
                      Vec<Vec<ArgSymbol*>*>* candidateActualFormals,
                      Vec<Type*>* actualTypes,
                      Vec<Symbol*>* actualSyms,
                      Vec<ArgSymbol*>** ret_afs) {
  FnSymbol* best = NULL;
  Vec<ArgSymbol*>* actualFormals = 0;
  for (int i = 0; i < candidateFns->n; i++) {
    if (candidateFns->v[i]) {
      best = candidateFns->v[i];
      actualFormals = candidateActualFormals->v[i];
      for (int j = 0; j < candidateFns->n; j++) {
        if (i != j && candidateFns->v[j]) {
          bool better = false;
          bool as_good = true;
          bool promotion1 = false;;
          bool promotion2 = false;
          Vec<ArgSymbol*>* actualFormals2 = candidateActualFormals->v[j];
          for (int k = 0; k < actualFormals->n; k++) {
            ArgSymbol* arg = actualFormals->v[k];
            ArgSymbol* arg2 = actualFormals2->v[k];
            if (arg->type == arg2->type && arg->instantiatedParam && !arg2->instantiatedParam)
              as_good = false;
            else if (arg->type == arg2->type && !arg->instantiatedParam && arg2->instantiatedParam)
              better = true;
            else {
              bool require_scalar_promotion1;
              bool require_scalar_promotion2;
              canDispatch(actualTypes->v[k], actualSyms->v[k], arg->type, best, &require_scalar_promotion1);
              canDispatch(actualTypes->v[k], actualSyms->v[k], arg2->type, best, &require_scalar_promotion2);
              promotion1 |= require_scalar_promotion1;
              promotion2 |= require_scalar_promotion2;
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
                  if (!arg->isTypeVariable && arg2->isTypeVariable)
                    as_good = false;
                  else if (arg->isTypeVariable && !arg2->isTypeVariable)
                    better = true;
                  else if (arg->instantiatedFrom==dtAny &&
                      arg2->instantiatedFrom!=dtAny) {
                    better = true;
                  } else if (arg->instantiatedFrom!=dtAny &&
                             arg2->instantiatedFrom==dtAny) {
                    as_good = false;
                  } else {
                    if (actualTypes->v[k] == arg2->type &&
                        actualTypes->v[k] != arg->type) {
                      better = true;
                    } else if (actualTypes->v[k] == arg->type &&
                               actualTypes->v[k] != arg2->type) {
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
            if (!promotion1 && promotion2)
              continue;
            best = NULL;
            break;
          }
        }
      }
      if (best)
        break;
    }
  }
  *ret_afs = actualFormals;
  return best;
}


static const char* toString(Type* type) {
  if (type->symbol->hasPragma("ref"))
    return getValueType(type)->symbol->name;
  else
    return type->symbol->name;
}


static const char* toString(CallInfo* info) {
  bool method = false;
  bool _this = false;
  const char *str = "";
  if (info->actualTypes.n > 1)
    if (info->actualTypes.v[0] == dtMethodToken)
      method = true;
  if (!strcmp("this", info->name)) {
    _this = true;
    method = false;
  }
  if (method) {
    if (info->actualSyms.v[1] && info->actualSyms.v[1]->isTypeVariable)
      str = astr(str, "type ", toString(info->actualTypes.v[1]), ".");
    else
      str = astr(str, toString(info->actualTypes.v[1]), ".");
  }
  if (!strncmp("_type_construct_", info->name, 16)) {
    str = astr(str, info->name+16);
  } else if (!strncmp("_construct_", info->name, 11)) {
    str = astr(str, info->name+11);
  } else if (!_this) {
    str = astr(str, info->name);
  }
  if (!info->call->methodTag) {
    if (info->call->square)
      str = astr(str, "[");
    else
      str = astr(str, "(");
  }
  bool first = false;
  int start = 0;
  if (method)
    start = 2;
  if (_this)
    start = 2;
  for (int i = start; i < info->actualTypes.n; i++) {
    if (!first)
      first = true;
    else
      str = astr(str, ", ");
    if (info->actualNames.v[i])
      str = astr(str, info->actualNames.v[i], "=");
    VarSymbol* var = toVarSymbol(info->actualSyms.v[i]);
    char buff[512];
    if (info->actualTypes.v[i]->symbol->hasPragma("iterator class") &&
        info->actualTypes.v[i]->defaultConstructor->hasPragma("promotion wrapper"))
      str = astr(str, "promoted expression");
    else if (info->actualSyms.v[i] && info->actualSyms.v[i]->isTypeVariable)
      str = astr(str, "type ", toString(info->actualTypes.v[i]));
    else if (var && var->immediate) {
      sprint_imm(buff, *var->immediate);
      str = astr(str, buff);
    } else
      str = astr(str, toString(info->actualTypes.v[i]));
  }
  if (!info->call->methodTag) {
    if (info->call->square)
      str = astr(str, "]");
    else
      str = astr(str, ")");
  }
  return str;
}


static const char* toString(FnSymbol* fn) {
  if (fn->userString) {
    if (developer)
      return astr(fn->userString, " [", istr(fn->id), "]");
    else
      return fn->userString;
  }
  const char* str;
  int start = 0;
  if (fn->instantiatedFrom)
    fn = fn->instantiatedFrom;
  if (!strncmp("_type_construct_", fn->name, 16)) {
    str = astr(fn->name+16);
  } else if (!strncmp("_construct_", fn->name, 11)) {
    str = astr(fn->name+11);
  } else if (fn->isMethod) {
    if (!strcmp(fn->name, "this")) {
      str = astr(toString(fn->getFormal(2)->type));
      start = 1;
    } else {
      str = astr(toString(fn->getFormal(2)->type), ".", fn->name);
      start = 2;
    }
  } else
    str = astr(fn->name);
  
  if (!fn->noParens &&
      !(!strncmp("_type_construct_", fn->name, 16) && fn->numFormals() == 0))
    str = astr(str, "(");
  bool first = false;
  for (int i = start; i < fn->numFormals(); i++) {
    ArgSymbol* arg = fn->getFormal(i+1);
    if (arg->hasPragma("is meme"))
      continue;
    if (!first)
      first = true;
    else
      str = astr(str, ", ");
    if (arg->intent == INTENT_PARAM)
      str = astr(str, "param ");
    if (arg->isTypeVariable)
      str = astr(str, "type ", arg->name);
    else if (arg->type == dtUnknown) {
      SymExpr* sym = NULL;
      if (arg->typeExpr)
        sym = toSymExpr(arg->typeExpr->body.tail);
      if (sym)
        str = astr(str, arg->name, ": ", sym->var->name);
      else
        str = astr(str, arg->name);
    } else if (arg->type == dtAny) {
      str = astr(str, arg->name);
    } else
      str = astr(str, arg->name, ": ", toString(arg->type));
    if (arg->variableExpr)
      str = astr(str, " ...");
  }
  if (!fn->noParens &&
      !(!strncmp("_type_construct_", fn->name, 16) && fn->numFormals() == 0))
    str = astr(str, ")");
  if (developer)
    str = astr(str, " [", istr(fn->id), "]");
  return str;
}


static bool
explainCallMatch(CallExpr* call) {
  if (!call->isNamed(fExplainCall))
    return false;
  if (explainCallModule && explainCallModule != call->getModule())
    return false;
  if (explainCallLine != -1 && explainCallLine != call->lineno)
    return false;
  return true;
}


static bool
explainInstantiationMatch(FnSymbol* fn) {
  if (strcmp(fn->name, fExplainInstantiation) &&
      (strncmp(fn->name, "_construct_", 11) ||
       strcmp(fn->name+11, fExplainInstantiation)))
    return false;
  if (explainInstantiationModule && explainInstantiationModule != fn->defPoint->getModule())
    return false;
  if (explainInstantiationLine != -1 && explainInstantiationLine != fn->defPoint->lineno)
    return false;
  return true;
}


static CallExpr*
userCall(CallExpr* call) {
  if (developer)
    return call;
  if (call->getFunction()->isCompilerTemp ||
      call->getModule()->modTag == MOD_STANDARD) {
    for (int i = callStack.n-1; i >= 0; i--) {
      if (!callStack.v[i]->getFunction()->isCompilerTemp &&
          callStack.v[i]->getModule()->modTag != MOD_STANDARD)
        return callStack.v[i];
    }
  }
  return call;
}


static void
printResolutionError(const char* error,
                     Vec<FnSymbol*>& candidates,
                     CallInfo* info,
                     BlockStmt* scope) {
  CallExpr* call = userCall(info->call);
  if (!strcmp("_cast", info->name)) {
    if (!info->actualSyms.v[0]->isTypeVariable) {
      USR_FATAL(call, "illegal cast to non-type",
                toString(info->actualTypes.v[1]),
                toString(info->actualTypes.v[0]));
    } else {
      USR_FATAL(call, "illegal cast from %s to %s",
                toString(info->actualTypes.v[1]),
                toString(info->actualTypes.v[0]));
    }
  } else if (info->actualTypes.n == 2 &&
             info->actualTypes.v[0] == dtMethodToken &&
             !strcmp("these", info->name)) {
    USR_FATAL(call, "cannot iterate over values of type %s",
              toString(info->actualTypes.v[1]));
  } else if (!strcmp("_type_construct__tuple", info->name)) {
    SymExpr* sym = toSymExpr(info->call->get(1));
    if (!sym || !sym->isParameter()) {
      USR_FATAL(call, "tuple size must be static");
    } else {
      USR_FATAL(call, "invalid tuple");
    }
  } else if (!strcmp("=", info->name)) {
    if (info->actualSyms.v[0] && !info->actualSyms.v[0]->isTypeVariable &&
        info->actualSyms.v[1] && info->actualSyms.v[1]->isTypeVariable) {
      USR_FATAL(call, "illegal assignment of type to value");
    } else if (info->actualTypes.v[1] == dtNil) {
      USR_FATAL(call, "type mismatch in assignment of nil to %s",
                toString(info->actualTypes.v[0]));
    } else {
      USR_FATAL(call, "type mismatch in assignment from %s to %s",
                toString(info->actualTypes.v[1]),
                toString(info->actualTypes.v[0]));
    }
  } else if (!strcmp("this", info->name)) {
    Type* type = info->actualTypes.v[1];
    if (type->symbol->hasPragma("ref"))
      type = getValueType(type);
    if (type->symbol->hasPragma("iterator class")) {
      USR_FATAL(call, "illegal access of iterator or promoted expression");
    } else {
      USR_FATAL(call, "%s access of '%s' by '%s'", error,
                toString(info->actualTypes.v[1]),
                toString(info));
    }
  } else {
    const char* entity = "call";
    if (!strncmp("_type_construct_", info->name, 16))
      entity = "type specifier";
    const char* str = toString(info);
    if (scope) {
      ModuleSymbol* mod = toModuleSymbol(scope->parentSymbol);
      INT_ASSERT(mod);
      str = astr(mod->name, ".", str);
    }
    USR_FATAL_CONT(call, "%s %s '%s'", error, entity, str);
    if (candidates.n > 0) {
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
      forv_Vec(FnSymbol, fn, candidates) {
        USR_PRINT(fn, "%s %s",
                  printed_one ? "               " : "candidates are:",
                  toString(fn));
        printed_one = true;
      }
    }
    if (candidates.n == 1 &&
        candidates.v[0]->numFormals() == 0
        && !strncmp("_type_construct_", info->name, 16))
      USR_PRINT(call, "did you forget the 'new' keyword?");
    USR_STOP();
  }
}


class VisibleFunctionBlock {
 public:
  Map<const char*,Vec<FnSymbol*>*> visibleFunctions;
  VisibleFunctionBlock() { }
};

static Map<BlockStmt*,VisibleFunctionBlock*> visibleFunctionMap;
static int nVisibleFunctions = 0; // for incremental build
static Map<BlockStmt*,BlockStmt*> visibilityBlockCache;

//
// return the innermost block for searching for visible functions
//
BlockStmt*
getVisibilityBlock(Expr* expr) {
  if (BlockStmt* block = toBlockStmt(expr->parentExpr)) {
    if (block->blockTag == BLOCK_SCOPELESS)
      return getVisibilityBlock(block);
    else
      return block;
  } else if (expr->parentExpr) {
    return getVisibilityBlock(expr->parentExpr);
  } else {
    FnSymbol* fn = toFnSymbol(expr->parentSymbol);
    if (fn && fn->instantiationPoint)
      return fn->instantiationPoint;
    else
      return getVisibilityBlock(expr->parentSymbol->defPoint);
  }
}

static void buildVisibleFunctionMap() {
  for (int i = nVisibleFunctions; i < gFns.n; i++) {
    FnSymbol* fn = gFns.v[i];
    if (fn->visible && !isArgSymbol(fn->defPoint->parentSymbol)) {
      BlockStmt* block =
        (fn->global) ? theProgram->block : getVisibilityBlock(fn->defPoint);
      VisibleFunctionBlock* vfb = visibleFunctionMap.get(block);
      if (!vfb) {
        vfb = new VisibleFunctionBlock();
        visibleFunctionMap.put(block, vfb);
      }
      Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(fn->name);
      if (!fns) {
        fns = new Vec<FnSymbol*>();
        vfb->visibleFunctions.put(fn->name, fns);
      }
      fns->add(fn);
    }
  }
  nVisibleFunctions = gFns.n;
}

static BlockStmt*
getVisibleFunctions(BlockStmt* block,
                    const char* name,
                    Vec<FnSymbol*>& visibleFns,
                    Vec<BlockStmt*>* visited = NULL) {
  if (!visited) { // first call only
    Vec<BlockStmt*> visited;
    getVisibleFunctions(block, name, visibleFns, &visited);
    return NULL;
  }

  if (visited->set_in(block)) {
    INT_ASSERT(isModuleSymbol(block->parentSymbol));
    return NULL;
  }
  visited->set_add(block);

  bool canSkipThisBlock = true;

  VisibleFunctionBlock* vfb = visibleFunctionMap.get(block);
  if (vfb) {
    canSkipThisBlock = false; // cannot skip if this block defines functions
    Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name);
    if (fns) {
      visibleFns.append(*fns);
    }
  }

  forv_Vec(ModuleSymbol, module, block->modUses) {
    canSkipThisBlock = false; // cannot skip if this block uses modules
    getVisibleFunctions(module->block, name, visibleFns, visited);
  }

  //
  // visibilityBlockCache contains blocks that can be skipped
  //
  if (BlockStmt* next = visibilityBlockCache.get(block)) {
    getVisibleFunctions(next, name, visibleFns, visited);
    return (canSkipThisBlock) ? next : block;
  }

  if (block != rootModule->block) {
    BlockStmt* next = getVisibilityBlock(block);
    BlockStmt* cache = getVisibleFunctions(next, name, visibleFns, visited);
    if (cache)
      visibilityBlockCache.put(block, cache);
    return (canSkipThisBlock) ? cache : block;
  }

  return NULL;
}

static FnSymbol*
resolve_call(CallInfo* info, BlockStmt* scope) {
  CallExpr* call = info->call;

  Vec<FnSymbol*> visibleFns;                    // visible functions

  Vec<FnSymbol*> candidateFns;
  Vec<Vec<ArgSymbol*>*> candidateActualFormals; // candidate functions

  //
  // update visible function map as necessary
  //
  if (gFns.n != nVisibleFunctions)
    buildVisibleFunctionMap();

  if (!call->isResolved()) {
    if (!scope) {
      getVisibleFunctions(getVisibilityBlock(call), info->name, visibleFns);
    } else {
      if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(scope))
        if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(info->name))
          visibleFns.append(*fns);
    }
  } else {
    visibleFns.add(call->isResolved());
  }

  if (explainCallLine && explainCallMatch(call)) {
    USR_PRINT(call, "call: %s", toString(info));
    if (visibleFns.n == 0)
      USR_PRINT(call, "no visible functions found");
    bool first = true;
    forv_Vec(FnSymbol, visibleFn, visibleFns) {
      USR_PRINT(visibleFn, "%s %s",
                first ? "visible functions are:" : "                      ",
                toString(visibleFn));
      first = false;
    }
  }

  forv_Vec(FnSymbol, visibleFn, visibleFns) {

    if (call->methodTag && !visibleFn->noParens && !visibleFn->hasPragma("type constructor"))
      continue;
    addCandidate(&candidateFns, &candidateActualFormals, visibleFn,
                 &info->actualTypes, &info->actualSyms, &info->actualNames,
                 call);
  }

  if (explainCallLine && explainCallMatch(call)) {
    if (candidateFns.n == 0)
      USR_PRINT(call, "no candidates found");
    bool first = true;
    forv_Vec(FnSymbol, candidateFn, candidateFns) {
      USR_PRINT(candidateFn, "%s %s",
                first ? "candidates are:" : "               ",
                toString(candidateFn));
      first = false;
    }
  }

  FnSymbol* best = NULL;
  Vec<ArgSymbol*>* actualFormals = 0;
  best = disambiguate_by_match(&candidateFns, &candidateActualFormals,
                               &info->actualTypes, &info->actualSyms,
                               &actualFormals);

  // use visibility and then look for best again
  if (!best && candidateFns.n > 1) {
    if (scope) {
      disambiguate_by_scope(scope, &candidateFns);
    } else {
      disambiguate_by_scope(getVisibilityBlock(call), &candidateFns);
    }
    best = disambiguate_by_match(&candidateFns, &candidateActualFormals,
                                 &info->actualTypes, &info->actualSyms,
                                 &actualFormals);
  }

  if (best && explainCallLine && explainCallMatch(call)) {
    USR_PRINT(best, "best candidate is: %s", toString(best));
  }

  if (!best && candidateFns.n > 0) {
    printResolutionError("ambiguous", candidateFns, info, scope);
    best = NULL;
  } else if (call->partialTag && (!best || !best->noParens)) {
    best = NULL;
  } else if (!best) {
    printResolutionError("unresolved", visibleFns, info, scope);
    best = NULL;
  } else {
    best = build_default_wrapper(best, actualFormals, call->square);
    best = build_order_wrapper(best, actualFormals, call->square);
    best = build_coercion_wrapper(best, &info->actualTypes, &info->actualSyms, call->square);

    FnSymbol* promoted = build_promotion_wrapper(best, &info->actualTypes, &info->actualSyms, call->square);
    if (promoted != best) {
      if (fWarnPromotion) {
        const char* str = toString(info);
        USR_WARN(call, "promotion on %s", str);
      }
      best = promoted;
    }
  }

  for (int i = 0; i < candidateActualFormals.n; i++)
    delete candidateActualFormals.v[i];

  return best;
}

static Type*
resolve_type_expr(Expr* expr) {
  bool stop = false;
  for_exprs_postorder(e, expr) {
    if (expr == e)
      stop = true;
    e = preFold(e);
    if (CallExpr* call = toCallExpr(e)) {
      if (call->parentSymbol) {
        callStack.add(call);
        resolveCall(call);
        FnSymbol* fn = call->isResolved();
        if (fn && call->parentSymbol) {
          resolveFormals(fn);
          if (fn->retTag == RET_PARAM || fn->retTag == RET_TYPE ||
              fn->retType == dtUnknown)
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
makeNoop(CallExpr* call) {
  if (call->baseExpr)
    call->baseExpr->remove();
  while (call->numActuals())
    call->get(1)->remove();
  call->primitive = primitives[PRIMITIVE_NOOP];
}


static bool
isTypeExpr(Expr* expr) {
  if (SymExpr* sym = toSymExpr(expr)) {
    if (sym->var->isTypeVariable)
      return true;
    if (toTypeSymbol(sym->var))
      return true;
  } else if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIMITIVE_TYPEOF))
      return true;
    if (FnSymbol* fn = call->isResolved())
      if (fn->retTag == RET_TYPE)
        return true;
  }
  return false;
}


//
// special case cast of class w/ type variables that is not generic
//   i.e. type variables are type definitions (have default types)
//
static void
resolveDefaultGenericType(CallExpr* call) {
  currentLineno = call->lineno;
  for_actuals(actual, call) {
    if (NamedExpr* ne = toNamedExpr(actual))
      actual = ne->actual;
    if (SymExpr* te = toSymExpr(actual)) {
      if (TypeSymbol* ts = toTypeSymbol(te->var)) {
        if (ClassType* ct = toClassType(ts->type)) {
          if (ct->isGeneric) {
            CallExpr* cc = new CallExpr(ct->defaultTypeConstructor->name);
            te->replace(cc);
            resolveCall(cc);
            cc->replace(new SymExpr(cc->typeInfo()->symbol));
          }
        }
      }
    }
  }
}


static void
resolveCall(CallExpr* call) {
  if (!call->primitive) {

    resolveDefaultGenericType(call);

    BlockStmt* scope = NULL;
    if (call->numActuals() >= 2) {
      if (SymExpr* se = toSymExpr(call->get(1))) {
        if (se->var == gModuleToken) {
          se->remove();
          se = toSymExpr(call->get(1));
          INT_ASSERT(se);
          ModuleSymbol* mod = toModuleSymbol(se->var);
          INT_ASSERT(mod);
          se->remove();
          scope = mod->block;
        }
      }
    }

    CallInfo info(call);

    FnSymbol* resolvedFn = resolve_call(&info, scope);

    if (call->partialTag) {
      if (!resolvedFn)
        return;
      call->partialTag = false;
    }
    if (resolvedFn && resolvedFn->hasPragma("data set error")) {
      Type* elt_type = toType(resolvedFn->getFormal(1)->type->substitutions.v[0].value);
      if (!elt_type)
        INT_FATAL(call, "Unexpected substitution of ddata class");
      USR_FATAL(userCall(call), "type mismatch in assignment from %s to %s",
                toString(info.actualTypes.v[3]), toString(elt_type));
    }
    if (resolvedFn &&
        !strcmp("=", resolvedFn->name) &&
        isRecordType(resolvedFn->getFormal(1)->type) &&
        resolvedFn->getFormal(2)->type == dtNil)
      USR_FATAL(userCall(call), "type mismatch in assignment from nil to %s",
                toString(resolvedFn->getFormal(1)->type));
    if (!resolvedFn) {
      INT_FATAL(call, "Error in resolve_call");
    }
    if (call->parentSymbol) {
      call->baseExpr->replace(new SymExpr(resolvedFn));
    }

    for_formals_actuals(formal, actual, call) {
      if (formal->intent == INTENT_OUT || formal->intent == INTENT_INOUT) {
        if (SymExpr* se = toSymExpr(actual)) {
          if (se->var->isExprTemp || se->var->isConstant() || se->var->isParameter()) {
            if (formal->intent == INTENT_OUT) {
              USR_FATAL(se, "non-lvalue actual passed to out argument");
            } else {
              USR_FATAL(se, "non-lvalue actual passed to inout argument");
            }
          }
        }
      }
    }

  } else if (call->isPrimitive(PRIMITIVE_TUPLE_AND_EXPAND)) {
    SymExpr* sym = toSymExpr(call->get(1));
    Symbol* var = toSymbol(sym->var);
    int size = 0;
    for (int i = 0; i < var->type->substitutions.n; i++) {
      if (var->type->substitutions.v[i].key) {
        if (!strcmp("size", toSymbol(var->type->substitutions.v[i].key)->name)) {
          size = toVarSymbol(var->type->substitutions.v[i].value)->immediate->int_value();
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
      tmp1->canType = true;
      VarSymbol* tmp2 = new VarSymbol("_expand_temp2");
      tmp2->canParam = true;
      tmp2->canType = true;
      VarSymbol* tmp3 = new VarSymbol("_expand_temp3");
      tmp3->canParam = true;
      tmp3->canType = true;
      VarSymbol* tmp4 = new VarSymbol("_expand_temp4");
      tmp4->canParam = true;
      tmp4->canType = true;
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
    SymExpr* sym = toSymExpr(call->get(1));
    Type* type = sym->var->type;
    if (isReference(type))
      type = getValueType(type);
    int size = 0;
    for (int i = 0; i < type->substitutions.n; i++) {
      if (type->substitutions.v[i].key) {
        if (!strcmp("size", toSymbol(type->substitutions.v[i].key)->name)) {
          size = toVarSymbol(type->substitutions.v[i].value)->immediate->int_value();
          break;
        }
      }
    }
    if (size == 0)
      INT_FATAL(call, "Invalid tuple expand primitive");
    CallExpr* parent = toCallExpr(call->parentExpr);
    CallExpr* noop = new CallExpr(PRIMITIVE_NOOP);
    call->getStmtExpr()->insertBefore(noop);
    for (int i = 1; i <= size; i++) {
      VarSymbol* tmp = new VarSymbol("_expand_temp");
      tmp->isCompilerTemp = true;
      tmp->canType = true;
      DefExpr* def = new DefExpr(tmp);
      call->getStmtExpr()->insertBefore(def);
      CallExpr* e = NULL;
      if (!call->parentSymbol->hasPragma("expand tuples with values")) {
        e = new CallExpr(sym->copy(), new_IntSymbol(i));
      } else {
        e = new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, sym->copy(),
                         new_StringSymbol(astr("x", istr(i))));
      }
      CallExpr* move = new CallExpr(PRIMITIVE_MOVE, tmp, e);
      call->getStmtExpr()->insertBefore(move);
      call->insertBefore(new SymExpr(tmp));
    }
    call->remove();
    noop->replace(call); // put call back in ast for function resolution
    makeNoop(call);
    // increase tuple rank
    if (parent && parent->isNamed("_type_construct__tuple")) {
      parent->get(1)->replace(new SymExpr(new_IntSymbol(parent->numActuals()-1)));
    }
  } else if (call->isPrimitive(PRIMITIVE_CAST)) {
    Type* t= call->get(1)->typeInfo();
    if (t == dtUnknown)
      INT_FATAL(call, "Unable to resolve type");
//     if (t->scalarPromotionType) {
//       // ignore for now to handle translation of A op= B of arrays
//       // should be an error in general
//       //   can't cast to an array type ...
//       Expr* castee = call->get(2);
//       castee->remove();
//       call->replace(castee);
//       makeNoop(call);
//       castee->getStmtExpr()->insertBefore(call);
//     } else {
      call->get(1)->replace(new SymExpr(t->symbol));
//    }
  } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
    SymExpr* sym = toSymExpr(call->get(2));
    if (!sym)
      INT_FATAL(call, "bad set member primitive");
    VarSymbol* var = toVarSymbol(sym->var);
    if (!var || !var->immediate)
      INT_FATAL(call, "bad set member primitive");
    const char* name = var->immediate->v_string;

    {
      long i;
      if (get_int(sym, &i)) {
        name = astr("x", istr(i));
        call->get(2)->replace(new SymExpr(new_StringSymbol(name)));
      }
    }

    ClassType* ct = toClassType(call->get(1)->typeInfo());
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
          USR_FATAL(userCall(call), "cannot assign expression of type %s to field of type %s", toString(t), toString(field->type));
        found = true;
      }
    }
    if (!found)
      INT_FATAL(call, "bad set member primitive");
  } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
    Expr* rhs = call->get(2);
    Symbol* lhs = NULL;
    if (SymExpr* se = toSymExpr(call->get(1)))
      lhs = se->var;
    INT_ASSERT(lhs);

    FnSymbol* fn = toFnSymbol(call->parentSymbol);

    if (lhs->isTypeVariable && !isTypeExpr(rhs)) {
      if (!fn || fn->_this == lhs) { // ignore type constructor
        if (lhs == fn->getReturnSymbol()) {
          USR_FATAL(call, "illegal return of value where type is expected");
        } else {
          USR_FATAL(call, "illegal assignment of value to type");
        }
      }
    }

    if (!lhs->isTypeVariable && !lhs->canType && isTypeExpr(rhs)) {
      if (lhs == fn->getReturnSymbol()) {
        USR_FATAL(call, "illegal return of type where value is expected");
      } else {
        USR_FATAL(call, "illegal assignment of type to value");
      }
    }

    // do not resolve function return type yet
    // except for constructors
    if (fn && fn->getReturnSymbol() == lhs && fn->_this != lhs)
      if (fn->retType == dtUnknown)
        return;

    Type* rhsType = rhs->typeInfo();

    if (rhsType == dtVoid)
      USR_FATAL(userCall(call), "illegal use of function that does not return a value");

    if (lhs->type == dtUnknown || lhs->type == dtNil)
      lhs->type = rhsType;

    Type* lhsType = lhs->type;

    if (isReference(lhsType))
      lhs->isExprTemp = false;
    if (lhsType->symbol->hasPragma("ref iterator class"))
      lhs->isExprTemp = false;
    if (CallExpr* call = toCallExpr(rhs)) {
      if (FnSymbol* fn = call->isResolved()) {
        if (fn->hasPragma("valid var"))
          lhs->isExprTemp = false;
        if (rhsType == dtUnknown) {
          USR_FATAL_CONT(fn, "unable to resolve return type of function '%s'", fn->name);
          USR_FATAL(rhs, "called recursively at this point");
        }
      }
    }
    if (rhsType == dtUnknown)
      USR_FATAL(call, "unable to resolve type");

    ClassType* ct = toClassType(lhsType);
    if (rhsType == dtNil && lhsType != dtNil && (!ct || ct->classTag != CLASS_CLASS))
      USR_FATAL(userCall(call), "type mismatch in assignment from nil to %s",
                toString(lhsType));
    Type* lhsBaseType = lhsType;
    if (isReference(lhsBaseType))
      lhsBaseType = getValueType(lhsBaseType);
    Type* rhsBaseType = rhsType;
    if (isReference(rhsBaseType))
      rhsBaseType = getValueType(rhsBaseType);
    if (rhsType != dtNil &&
        rhsBaseType != lhsBaseType &&
        !isDispatchParent(rhsBaseType, lhsBaseType))
      USR_FATAL(userCall(call), "type mismatch in assignment from %s to %s",
                toString(rhsType), toString(lhsType));
    if (rhsType != lhsType && isDispatchParent(rhsBaseType, lhsBaseType)) {
      rhs->remove();
      call->insertAtTail(new CallExpr(PRIMITIVE_CAST, lhsBaseType->symbol, rhs));
    }
  } else if (call->isPrimitive(PRIMITIVE_INIT)) {
    resolveDefaultGenericType(call);
  }
}

static bool
formalRequiresTemp(ArgSymbol* formal) {
  if (formal->intent == INTENT_PARAM ||
      formal->intent == INTENT_TYPE ||
      formal->intent == INTENT_REF ||
      !strcmp("this", formal->name) ||
      (formal == toFnSymbol(formal->defPoint->parentSymbol)->_outer) ||
      formal->isTypeVariable ||
      formal->instantiatedParam ||
      formal->type == dtMethodToken ||
      (formal->type->symbol->hasPragma("ref") &&
       formal->intent == INTENT_BLANK) ||
      formal->hasPragma("no formal tmp"))
    return false;
  return true;
}

static void
insertFormalTemps(FnSymbol* fn) {
  if (!strcmp(fn->name, "_pass") ||
      !strcmp(fn->name, "_init") ||
      !strcmp(fn->name, "_copy") ||
      !strcmp(fn->name, "_getIterator") ||
      !strcmp(fn->name, "_getIteratorHelp") ||
      !strcmp(fn->name, "iteratorIndex") ||
      !strcmp(fn->name, "iteratorIndexHelp") ||
      !strcmp(fn->name, "=") ||
      !strcmp(fn->name, "_createFieldDefault") ||
      !strcmp(fn->name, "chpldev_refToString") ||
      fn->hasPragma("allow ref") ||
      fn->hasPragma("ref"))
    return;
  SymbolMap formals2vars;
  for_formals(formal, fn) {
    if (formalRequiresTemp(formal)) {
      VarSymbol* tmp = new VarSymbol(astr("_formal_tmp_", formal->name));
      Type* formalType = formal->type;
      if (formalType->symbol->hasPragma("ref"))
        formalType = getValueType(formalType);
      if ((formal->intent == INTENT_BLANK ||
           formal->intent == INTENT_CONST) &&
          !formalType->symbol->hasPragma("domain") &&
          !formalType->symbol->hasPragma("sync") &&
          !formalType->symbol->hasPragma("array"))
        tmp->isConst = true;
      tmp->isCompilerTemp = true;
      formals2vars.put(formal, tmp);
    }
  }
  if (formals2vars.n > 0) {
    update_symbols(fn->body, &formals2vars);
    form_Map(SymbolMapElem, e, formals2vars) {
      ArgSymbol* formal = toArgSymbol(e->key);
      Symbol* tmp = e->value;
      if (formal->intent == INTENT_OUT) {
        if (formal->defaultExpr && formal->defaultExpr->body.tail->typeInfo() != dtNil) {
          BlockStmt* defaultExpr = formal->defaultExpr->copy();
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, defaultExpr->body.tail->remove()));
          fn->insertAtHead(defaultExpr);
        } else {
          VarSymbol* refTmp = new VarSymbol("_tmp");
          VarSymbol* typeTmp = new VarSymbol("_tmp");
          typeTmp->isCompilerTemp = true;
          typeTmp->canType = true;
          refTmp->isCompilerTemp = true;
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_INIT, typeTmp)));
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, typeTmp, new CallExpr(PRIMITIVE_TYPEOF, refTmp)));
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, refTmp, new CallExpr(PRIMITIVE_GET_REF, formal)));
          fn->insertAtHead(new DefExpr(refTmp));
          fn->insertAtHead(new DefExpr(typeTmp));
        }
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

static void fold_param_for(CallExpr* loop) {
  BlockStmt* block = toBlockStmt(loop->parentExpr);
  if (!block || block->blockTag != BLOCK_PARAM_FOR || block->loopInfo != loop)
    INT_FATAL(loop, "bad param loop primitive");
  SymExpr* lse = toSymExpr(loop->get(2));
  SymExpr* hse = toSymExpr(loop->get(3));
  SymExpr* sse = toSymExpr(loop->get(4));
  if (!lse || !hse || !sse)
    INT_FATAL(loop, "bad param loop primitive");
  VarSymbol* lvar = toVarSymbol(lse->var);
  VarSymbol* hvar = toVarSymbol(hse->var);
  VarSymbol* svar = toVarSymbol(sse->var);
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
  Symbol* index = toSymExpr(index_expr)->var;
  if (stride <= 0) {
    for (int i = high; i >= low; i += stride) {
      Map<Symbol*,Symbol*> map;
      map.put(index, new_IntSymbol(i));
      noop->insertBefore(block->copy(&map));
    }
  } else {
    for (int i = low; i <= high; i += stride) {
      Map<Symbol*,Symbol*> map;
      map.put(index, new_IntSymbol(i));
      noop->insertBefore(block->copy(&map));
    }
  }
  block->replace(loop);
  makeNoop(loop);
}


static Expr* dropUnnecessaryCast(CallExpr* call) {
  // Check for and remove casts to the original type and size
  Expr* result = call;
  if (!call->isNamed("_cast"))
    INT_FATAL("dropUnnecessaryCasts called on non _cast call");

  if (SymExpr* sym = toSymExpr(call->get(2))) {
    if (VarSymbol* var = toVarSymbol(sym->var)) {
      if (SymExpr* sym = toSymExpr(call->get(1))) {
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
    } else if (EnumSymbol* e = toEnumSymbol(sym->var)) {
      if (SymExpr* sym = toSymExpr(call->get(1))) {
        EnumType* src = toEnumType(e->type);
        EnumType* dst = toEnumType(sym->var->type);
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
  if (CallExpr* call = toCallExpr(expr)) {
    if (SymExpr* sym = toSymExpr(call->baseExpr)) {
      if (TypeSymbol* type = toTypeSymbol(sym->var)) {
        if (call->numActuals() == 1) {
          if (SymExpr* arg = toSymExpr(call->get(1))) {
            if (VarSymbol* var = toVarSymbol(arg->var)) {
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
                    default:
                      USR_FATAL( call, "illegal size %d for imag", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtImag[FLOAT_SIZE_64]->symbol) {
                    switch (size) {
                    case 32:  tsize = dtImag[FLOAT_SIZE_32]->symbol;  break;
                    case 64:  tsize = dtImag[FLOAT_SIZE_64]->symbol;  break;
                    default:
                      USR_FATAL( call, "illegal size %d for imag", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtComplex[COMPLEX_SIZE_128]->symbol) {
                    switch (size) {
                    case 64:  tsize = dtComplex[COMPLEX_SIZE_64]->symbol;  break;
                    case 128: tsize = dtComplex[COMPLEX_SIZE_128]->symbol; break;
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

    if (SymExpr* sym = toSymExpr(call->baseExpr)) {
      if (toVarSymbol(sym->var) || toArgSymbol(sym->var)) {
        Expr* base = call->baseExpr;
        base->replace(new SymExpr("this"));
        call->insertAtHead(base);
        call->insertAtHead(gMethodToken);
      }
    }

    if (CallExpr* base = toCallExpr(call->baseExpr)) {
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
        call->insertAtHead(gMethodToken);
        call->getStmtExpr()->insertBefore(new DefExpr(this_temp));
        call->getStmtExpr()->insertBefore(move);
        result = move;
        return result;
      }
    }

    if (call->isNamed("this")) {
      SymExpr* base = toSymExpr(call->get(2));
      INT_ASSERT(base);
      if (isVarSymbol(base->var) && base->var->isTypeVariable) {
        if (call->numActuals() == 2)
          USR_FATAL(call, "illegal call of type");
        long index;
        if (!get_int(call->get(3), &index))
          USR_FATAL(call, "illegal type index expression");
        char field[8];
        sprintf(field, "x%ld", index);
        result = new SymExpr(base->var->type->getField(field)->type->symbol);
        call->replace(result);
      } else if (base && (isVarSymbol(base->var) || isArgSymbol(base->var))) {
        //
        // resolve tuple indexing by an integral parameter
        //
        Type* t = base->var->type;
        if (t->symbol->hasPragma("ref"))
          t = getValueType(t);
        if (t->symbol->hasPragma("tuple")) {
          if (call->numActuals() != 3)
            USR_FATAL(call, "illegal tuple indexing expression");
          Type* indexType = call->get(3)->typeInfo();
          if (indexType->symbol->hasPragma("ref"))
            indexType = getValueType(indexType);
          if (!is_int_type(indexType))
            USR_FATAL(call, "tuple indexing expression is not of integral type");
          long index;
          if (get_int(call->get(3), &index)) {
            char field[8];
            sprintf(field, "x%ld", index);
            if (index <= 0 || index >= toClassType(t)->fields.length())
              USR_FATAL(call, "tuple index out-of-bounds error (%ld)", index);
            if (toClassType(t)->getField(field)->type->symbol->hasPragma("ref"))
              result = new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, base->var, new_StringSymbol(field));
            else
              result = new CallExpr(PRIMITIVE_GET_MEMBER, base->var, new_StringSymbol(field));
            call->replace(result);
          }
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_INIT)) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      if (!se->var->isTypeVariable)
        USR_FATAL(call, "invalid type specification");
      Type* type = call->get(1)->typeInfo();
      if (type ->symbol->hasPragma("ref"))
        type = getValueType(type);
      if (type->symbol->hasPragma("array")) {
        result = new CallExpr("_init", call->get(1)->remove());
        call->replace(result);
      } else if (type->symbol->hasPragma("iterator class")) {
        result = new CallExpr(PRIMITIVE_CAST, type->symbol, gNil);
        call->replace(result);
      } else if (type->defaultValue == gNil) {
        result = new CallExpr("_cast", type->symbol, type->defaultValue);
        call->replace(result);
      } else if (type->defaultValue) {
        result = new SymExpr(type->defaultValue);
        call->replace(result);
      } else {
        inits.add(call);
      }
    } else if (call->isNamed("_copy")) {
      if (call->numActuals() == 1) {
        if (SymExpr* symExpr = toSymExpr(call->get(1))) {
          if (VarSymbol* var = toVarSymbol(symExpr->var)) {
            if (var->immediate) {
              result = new SymExpr(var);
              call->replace(result);
            }
          } else {
            if (EnumSymbol* var = toEnumSymbol(symExpr->var)) {
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
        if (SymExpr* sym = toSymExpr(call->get(2))) {
          if (VarSymbol* var = toVarSymbol(sym->var)) {
            if (var->immediate) {
              if (SymExpr* sym = toSymExpr(call->get(1))) {
                Type* src = var->type;
                Type* dst = sym->var->type;
                if (!is_real_type(dst) && !is_real_type(src) &&
                    !is_imag_type(dst) && !is_imag_type(src) &&
                    !is_complex_type(dst) && !is_complex_type(src) &&
                    src != dtString) {
                  VarSymbol* typevar = toVarSymbol(dst->defaultValue);
                  EnumType* typeenum = toEnumType(dst);
                  if (typevar) {
                    if (!typevar->immediate)
                      INT_FATAL("unexpected case in cast_fold");

                    Immediate coerce = *typevar->immediate;
                    coerce_immediate(var->immediate, &coerce);
                    result = new SymExpr(new_ImmediateSymbol(&coerce));
                    call->replace(result);
                  } else if (typeenum) {
                    long value, count = 0;
                    bool replaced = false;
                    if (!get_int(call->get(2), &value)) {
                      INT_FATAL("unexpected case in cast_fold");
                    }
                    for_enums(constant, typeenum) {
                      if (!get_int(constant->init, &count)) {
                        count++;
                      }
                      if (count == value) {
                        result = new SymExpr(constant->sym);
                        call->replace(result);
                        replaced = true;
                        break;
                      }
                    }
                    if (!replaced) {
                      USR_FATAL(call->get(2), "enum cast out of bounds");
                    }
                  } else {
                    INT_FATAL("unexpected case in cast_fold");
                  }
                }
              }
            }
          }
        }
      }
    } else if (call->isNamed("==")) {
      if (isTypeExpr(call->get(1)) || isTypeExpr(call->get(2))) {
        Type* lt = call->get(1)->typeInfo();
        Type* rt = call->get(2)->typeInfo();
        if (lt != dtUnknown && rt != dtUnknown &&
            !lt->isGeneric && !rt->isGeneric) {
          result = (lt == rt) ? new SymExpr(gTrue) : new SymExpr(gFalse);
          call->replace(result);
        }
      }
    } else if (call->isNamed("!=")) {
      if (isTypeExpr(call->get(1)) || isTypeExpr(call->get(2))) {
        Type* lt = call->get(1)->typeInfo();
        Type* rt = call->get(2)->typeInfo();
        if (lt != dtUnknown && rt != dtUnknown &&
            !lt->isGeneric && !rt->isGeneric) {
          result = (lt != rt) ? new SymExpr(gTrue) : new SymExpr(gFalse);
          call->replace(result);
        }
      }
    } else if (call->isNamed("_type_construct__tuple") && !call->isResolved()) {
      if (SymExpr* sym = toSymExpr(call->get(1))) {
        if (VarSymbol* var = toVarSymbol(sym->var)) {
          if (var->immediate) {
            int rank = var->immediate->int_value();
            if (rank != call->numActuals() - 1) {
              if (call->numActuals() != 2)
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
//     } else if (call->isPrimitive(PRIMITIVE_LOOP_FOR) &&
//                call->numActuals() == 2) {
//       result = expand_for_loop(call);
    } else if (call->isPrimitive(PRIMITIVE_LOGICAL_FOLDER)) {
      bool removed = false;
      SymExpr* sym1 = toSymExpr(call->get(1));
      if (VarSymbol* sym = toVarSymbol(sym1->var)) {
        if (sym->immediate || paramMap.get(sym)) {
          CallExpr* mvCall = toCallExpr(call->parentExpr);
          SymExpr* sym = toSymExpr(mvCall->get(1));
          VarSymbol* var = toVarSymbol(sym->var);
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
      } else {
        FnSymbol* fn = call->getFunction();
        if (!fn->isWrapper && !fn->hasPragma("valid var")) {
          // check legal var function return
          if (CallExpr* move = toCallExpr(call->parentExpr)) {
            if (move->isPrimitive(PRIMITIVE_MOVE)) {
              SymExpr* lhs = toSymExpr(move->get(1));
              if (lhs->var == fn->getReturnSymbol()) {
                SymExpr* ret = toSymExpr(call->get(1));
                INT_ASSERT(ret);
                if (ret->var->defPoint->getFunction() == move->getFunction())
                  USR_FATAL(ret, "illegal return expression in var function");
                if (ret->var->isConstant() || ret->var->isParameter())
                  USR_FATAL(ret, "var function returns constant value");
              }
            }
          }
          // check legal lvalue
          if (SymExpr* rhs = toSymExpr(call->get(1))) {
            if (rhs->var->isExprTemp || rhs->var->isConstant() || rhs->var->isParameter())
              USR_FATAL(call, "illegal lvalue in assignment");
          }
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_GET_REF)) {
      // remove get ref if already a value
      if (!call->get(1)->typeInfo()->symbol->hasPragma("ref")) {
        result = call->get(1)->remove();
        call->replace(result);
      }
    } else if (call->isPrimitive(PRIMITIVE_GET_LOCALEID)) {
      Type* type = call->get(1)->typeInfo();

      if (type->symbol->hasPragma("ref"))
        type = getValueType(type);

      //
      // ensure .locale (and on) are applied to lvalues or classes
      // (locale type is a class)
      //
      SymExpr* se = toSymExpr(call->get(1));
      ClassType* ct = toClassType(type);
      if (se->var->isExprTemp && (!ct || ct->classTag != CLASS_CLASS))
        USR_WARN(se, "accessing the locale of a local expression");

      //
      // if .locale is applied to an expression of array or domain
      // wrapper type, apply .locale to the _value field of the
      // wrapper
      //
      if (type->symbol->hasPragma("array") ||
          type->symbol->hasPragma("domain")) {
        VarSymbol* tmp = new VarSymbol("_tmp");
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        result = new CallExpr("_value", gMethodToken, call->get(1)->remove());
        call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, result));
        call->insertAtTail(tmp);
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

  type1 = toEnumType(e1->type);
  type2 = toEnumType(e2->type);
  INT_ASSERT(type1 && type2);

  // Loop over the enum values to find the int value of e1
  for_enums(constant, type1) {
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
  for_enums(constant, type2) {
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
  if (SymExpr* sym = toSymExpr(call->get(1))) {            \
    if (VarSymbol* lhs = toVarSymbol(sym->var)) {          \
      if (lhs->immediate) {                                             \
        Immediate i3;                                                   \
        fold_constant(prim, lhs->immediate, NULL, &i3);                 \
        result = new SymExpr(new_ImmediateSymbol(&i3));                 \
        call->replace(result);                                          \
      }                                                                 \
    }                                                                   \
  }

#define FOLD_CALL2(prim)                                                \
  if (SymExpr* sym = toSymExpr(call->get(1))) {            \
    if (VarSymbol* lhs = toVarSymbol(sym->var)) {          \
      if (lhs->immediate) {                                             \
        if (SymExpr* sym = toSymExpr(call->get(2))) {      \
          if (VarSymbol* rhs = toVarSymbol(sym->var)) {    \
            if (rhs->immediate) {                                       \
              Immediate i3;                                             \
              fold_constant(prim, lhs->immediate, rhs->immediate, &i3); \
              result = new SymExpr(new_ImmediateSymbol(&i3));           \
              call->replace(result);                                    \
            }                                                           \
          }                                                             \
        }                                                               \
      }                                                                 \
    } else if (EnumSymbol* lhs = toEnumSymbol(sym->var)) { \
      if (SymExpr* sym = toSymExpr(call->get(2))) {        \
        if (EnumSymbol* rhs = toEnumSymbol(sym->var)) {    \
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
  if (SymExpr* se = toSymExpr(actual)) {
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
  if (CallExpr* call = toCallExpr(expr)) {
    if (FnSymbol* fn = call->isResolved()) {
      if (fn->retTag == RET_PARAM || fn->canParam) {
        VarSymbol* ret = toVarSymbol(fn->getReturnSymbol());
        if (ret && ret->immediate) {
          result = new SymExpr(ret);
          expr->replace(result);
        } else if (EnumSymbol* es = toEnumSymbol(fn->getReturnSymbol())) {
          result = new SymExpr(es);
          expr->replace(result);
        } else if (fn->retTag == RET_PARAM) {
          USR_FATAL(call, "param function does not resolve to a param symbol");
        }
      }
      if (fn->canType && fn->getReturnSymbol()->isTypeVariable)
        fn->retTag = RET_TYPE;
      if (fn->retTag == RET_TYPE) {
        Symbol* ret = fn->getReturnSymbol();
        if (!ret->type->symbol->hasPragma("array")) {
          result = new SymExpr(ret->type->symbol);
          expr->replace(result);
        }
      }
      if (!strcmp("=", fn->name) && !call->getFunction()->isWrapper) {
        SymExpr* lhs = toSymExpr(call->get(1));
        if (!lhs)
          INT_FATAL(call, "unexpected case");
        if (lhs->var->isExprTemp || lhs->var->isConstant() || lhs->var->isParameter())
          USR_FATAL(call, "illegal lvalue in assignment");
      }
    } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
      bool set = false;
      if (SymExpr* lhs = toSymExpr(call->get(1))) {
        if (lhs->var->canParam || lhs->var->isParameter()) {
          if (paramMap.get(lhs->var))
            INT_FATAL(call, "parameter set multiple times");
          if (VarSymbol* lhsVar = toVarSymbol(lhs->var))
            INT_ASSERT(!lhsVar->immediate);
          if (SymExpr* rhs = toSymExpr(call->get(2))) {
            if (VarSymbol* rhsVar = toVarSymbol(rhs->var)) {
              if (rhsVar->immediate) {
                paramMap.put(lhs->var, rhsVar);
                lhs->var->defPoint->remove();
                makeNoop(call);
                set = true;
              }
            } else if (EnumSymbol* rhsv = toEnumSymbol(rhs->var)) {
              paramMap.put(lhs->var, rhsv);
              lhs->var->defPoint->remove();
              makeNoop(call);
              set = true;
            } 
          }
          if (!set && lhs->var->isParameter())
            USR_FATAL(call, "Initializing parameter '%s' to value not known at compile time", lhs->var->name);
        }
        if (!set) {
          if (lhs->var->canType) {
            if (SymExpr* rhs = toSymExpr(call->get(2))) {
              if (rhs->var->isTypeVariable)
                lhs->var->isTypeVariable = true;
            }
            if (CallExpr* rhs = toCallExpr(call->get(2))) {
              if (FnSymbol* fn = rhs->isResolved()) {
                if (fn->retTag == RET_TYPE)
                  lhs->var->isTypeVariable = true;
              }
              if (rhs->isPrimitive(PRIMITIVE_GET_REF)) {
                if (isTypeExpr(rhs->get(1)))
                  lhs->var->isTypeVariable = true;
              }
            }
          }
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (rhs->isPrimitive(PRIMITIVE_TYPEOF)) {
              lhs->var->isTypeVariable = true;
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
      Type* baseType = call->get(1)->typeInfo();
      if (baseType->symbol->hasPragma("ref"))
        baseType = getValueType(baseType);
      const char* memberName = get_string(call->get(2));
      Symbol* sym = baseType->getField(memberName);
      if (sym->isParameter()) {
        Vec<BaseAST*> keys;
        baseType->substitutions.get_keys(keys);
        forv_Vec(BaseAST, key, keys) {
          if (Symbol* var = toSymbol(key)) {
            if (!strcmp(sym->name, var->name)) {
              if (Symbol* value = toSymbol(baseType->substitutions.get(key))) {
                result = new SymExpr(value);
                call->replace(result);
              }
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_ISSUBTYPE)) {
      if (isTypeExpr(call->get(1)) || isTypeExpr(call->get(2))) {
        Type* lt = call->get(2)->typeInfo(); // a:t cast is cast(t,a)
        Type* rt = call->get(1)->typeInfo();
        if (lt->symbol->hasPragma("ref"))
          lt = getValueType(lt);
        if (rt->symbol->hasPragma("ref"))
          rt = getValueType(rt);
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
      if ((c = toCallExpr(call->get(1))) &&
          (c->isPrimitive(PRIMITIVE_TYPEOF))) {
        symExpr = toSymExpr(c->get(1));
      } else {
        symExpr = toSymExpr(call->get(1));
      }
      if (symExpr && toEnumType(symExpr->var->type)) {
        is_enum = true;
      }
      result = (is_enum) ? new SymExpr(gTrue) : new SymExpr(gFalse);
      call->replace(result);
    } else if (call->isPrimitive(PRIMITIVE_IS_OPAQUE)) {
      Expr* arg = call->get(1);
      SymExpr* argSymExpr = toSymExpr(arg);
      result = (argSymExpr->var->type == dtOpaque) ? new SymExpr(gTrue) : new SymExpr(gFalse);
      call->replace(result);
    } else if (call->isPrimitive(PRIMITIVE_IS_TUPLE)) {
      bool is_tuple = false;
      if (SymExpr* sym = toSymExpr(call->get(1))) {
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
    } else if (call->isPrimitive("_string_compare")) {
      SymExpr* lhs = toSymExpr(call->get(1));
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(lhs && rhs);
      if (lhs->var->isParameter() && rhs->var->isParameter()) {
        const char* lstr = get_string(lhs);
        const char* rstr = get_string(rhs);
        int comparison = strcmp(lstr, rstr);
        result = new SymExpr(new_IntSymbol(comparison));
        call->replace(result);
      }
    } else if (call->isPrimitive("string_concat")) {
      SymExpr* lhs = toSymExpr(call->get(1));
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(lhs && rhs);
      if (lhs->var->isParameter() && rhs->var->isParameter()) {
        const char* lstr = get_string(lhs);
        const char* rstr = get_string(rhs);
        result = new SymExpr(new_StringSymbol(astr(lstr, rstr)));
        call->replace(result);
      }
    } else if (call->isPrimitive("string_length")) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      if (se->var->isParameter()) {
        const char* str = get_string(se);
        result = new SymExpr(new_IntSymbol(strlen(str), INT_SIZE_64));
        call->replace(result);
      }
    } else if (call->isPrimitive("ascii")) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      if (se->var->isParameter()) {
        const char* str = get_string(se);
        result = new SymExpr(new_IntSymbol((int)str[0], INT_SIZE_32));
        call->replace(result);
      }
    } else if (call->isPrimitive("string_contains")) {
      SymExpr* lhs = toSymExpr(call->get(1));
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(lhs && rhs);
      if (lhs->var->isParameter() && rhs->var->isParameter()) {
        const char* lstr = get_string(lhs);
        const char* rstr = get_string(rhs);
        result = new SymExpr(strstr(lstr, rstr) ? gTrue : gFalse);
        call->replace(result);
      }
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
    } else if (call->isPrimitive(PRIMITIVE_BUILD_ARRAY)) {
      BlockStmt* block = new BlockStmt();

      // since we are going to remove this block, better not let
      // buildArray function capture its instantiation point as this
      // block's scope
      block->blockTag = BLOCK_SCOPELESS;

      SymExpr* se = toSymExpr(call->get(1));
      VarSymbol* elt = new VarSymbol("_tmp", se->var->type->getField("eltType")->type);
      block->insertAtTail(new DefExpr(elt));
      elt->isTypeVariable = true;
      Type* domainValueType = se->var->type->getField("_value")->type->getField("dom")->type;
      VarSymbol* dom = new VarSymbol("_tmp", domainValueType);
      block->insertAtTail(new DefExpr(dom));
      CallExpr* wrap = new CallExpr("_wrapDomain", dom);
      block->insertAtTail(wrap);
      call->getStmtExpr()->insertBefore(block);
      resolveCall(wrap);
      if (!wrap->isResolved())
        INT_FATAL(call, "unable to build arrays");
      resolveFns(wrap->isResolved());
      dom->type = wrap->isResolved()->retType;
      CallExpr* build = new CallExpr("buildArray", gMethodToken, dom, elt);
      block->insertAtTail(build);
      resolveCall(build);
      if (!build->isResolved())
        INT_FATAL(call, "unable to build arrays");
      block->remove();
      wrapDomainMap.put(domainValueType, wrap->isResolved());
      buildArrayMap.put(call, build->isResolved());
    } else if (call->isPrimitive(PRIMITIVE_ARRAY_ALLOC) ||
               call->isPrimitive(PRIMITIVE_LOOP_C_FOR) ||
               call->isPrimitive(PRIMITIVE_SYNC_INIT) ||
               call->isPrimitive(PRIMITIVE_SYNC_LOCK) ||
               call->isPrimitive(PRIMITIVE_SYNC_UNLOCK) ||
               call->isPrimitive(PRIMITIVE_SYNC_WAIT_FULL) ||
               call->isPrimitive(PRIMITIVE_SYNC_WAIT_EMPTY) ||
               call->isPrimitive(PRIMITIVE_SYNC_SIGNAL_FULL) ||
               call->isPrimitive(PRIMITIVE_SYNC_SIGNAL_EMPTY) ||
               call->isPrimitive(PRIMITIVE_SINGLE_INIT) ||
               call->isPrimitive(PRIMITIVE_SINGLE_LOCK) ||
               call->isPrimitive(PRIMITIVE_SINGLE_UNLOCK) ||
               call->isPrimitive(PRIMITIVE_SINGLE_WAIT_FULL) ||
               call->isPrimitive(PRIMITIVE_SINGLE_SIGNAL_FULL) ||
               call->isPrimitive(PRIMITIVE_WRITEEF) ||
               call->isPrimitive(PRIMITIVE_WRITEFF) ||
               call->isPrimitive(PRIMITIVE_WRITEXF) ||
               call->isPrimitive(PRIMITIVE_SYNC_RESET) ||
               call->isPrimitive(PRIMITIVE_READFF) ||
               call->isPrimitive(PRIMITIVE_READFE) ||
               call->isPrimitive(PRIMITIVE_READXX) ||
               call->isPrimitive(PRIMITIVE_SYNC_ISFULL) ||
               call->isPrimitive(PRIMITIVE_SINGLE_WRITEEF) ||
               call->isPrimitive(PRIMITIVE_SINGLE_RESET) ||
               call->isPrimitive(PRIMITIVE_SINGLE_READFF) ||
               call->isPrimitive(PRIMITIVE_SINGLE_READXX) ||
               call->isPrimitive(PRIMITIVE_SINGLE_ISFULL) ||
               call->isPrimitive(PRIMITIVE_EXECUTE_TASKS_IN_LIST) ||
               call->isPrimitive(PRIMITIVE_FREE_TASK_LIST) ||
               (call->primitive && 
                (!strncmp("_fscan", call->primitive->name, 6) ||
                 !strcmp("fprintf", call->primitive->name) ||
                 !strcmp("fopen", call->primitive->name) ||
                 !strcmp("fclose", call->primitive->name) ||
                 !strcmp("fflush", call->primitive->name) ||
                 !strcmp("_readToEndOfLine", call->primitive->name) ||
                 !strcmp("_now_timer", call->primitive->name) ||
                 !strcmp("_format", call->primitive->name)))) {
      //
      // these primitives require temps to dereference actuals
      //   why not do this to all primitives?
      //
      for_actuals(actual, call) {
        insertValueTemp(call->getStmtExpr(), actual);
      }
    }
  } else if (SymExpr* sym = toSymExpr(expr)) {
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

  if (CondStmt* cond = toCondStmt(result->parentExpr)) {
    if (cond->condExpr == result) {
      if (Expr* expr = cond->fold_cond_stmt()) {
        result = expr;
      }
    }
  }
  return result;
}


static void issueCompilerError(CallExpr* call) {
  CallExpr* from = NULL;
  for (int i = callStack.n-1; i >= 0; i--) {
    from = callStack.v[i];
    if (from->lineno > 0 && from->getModule()->modTag != MOD_STANDARD)
      break;
  }
  const char* str = "";
  for_actuals(actual, call) {
    if (SymExpr* sym = toSymExpr(actual)) {
      if (VarSymbol* var = toVarSymbol(sym->var)) {
        if (var->immediate &&
            var->immediate->const_kind == CONST_KIND_STRING) {
          str = astr(str, var->immediate->v_string);
          continue;
        }
      }
      if (sym->var->isTypeVariable) {
        str = astr(str, sym->var->type->symbol->name);
        continue;
      }
    }
    if (CallExpr* call = toCallExpr(actual)) {
      if (call->isPrimitive(PRIMITIVE_TYPEOF)) {
        str = astr(str, call->get(1)->typeInfo()->symbol->name);
      }
    }
  }
  if (call->isPrimitive(PRIMITIVE_ERROR)) {
    USR_FATAL(from, "%s", str);
  } else {
    USR_WARN(from, "%s", str);
  }
}

static void
resolveBody(Expr* body) {
  FnSymbol* fn = toFnSymbol(body->parentSymbol);
  for_exprs_postorder(expr, body) {
    currentLineno = expr->lineno;
    if (SymExpr* sym = toSymExpr(expr))
      if (Type* type = sym->typeInfo())
        makeRefType(type);
    expr = preFold(expr);

    if (fn && fn->retTag == RET_PARAM) {
      if (BlockStmt* block = toBlockStmt(expr)) {
        if (block->loopInfo) {
          USR_FATAL(expr, "param function cannot contain a non-param loop");
        }
      }
      if (BlockStmt* block = toBlockStmt(expr->parentExpr)) {
        if (isCondStmt(block->parentExpr)) {
          USR_FATAL(block->parentExpr,
                    "param function cannot contain a non-param conditional");
        }
      }
      if (paramMap.get(fn->getReturnSymbol())) {
        CallExpr* call = toCallExpr(fn->body->body.tail);
        INT_ASSERT(call);
        INT_ASSERT(call->isPrimitive(PRIMITIVE_RETURN));
        call->get(1)->replace(new SymExpr(paramMap.get(fn->getReturnSymbol())));
        return; // param function is resolved
      }
    }

    if (CallExpr* call = toCallExpr(expr)) {
      if (call->isPrimitive(PRIMITIVE_ERROR) ||
          call->isPrimitive(PRIMITIVE_WARNING)) {
        issueCompilerError(call);
      }
      callStack.add(call);
      resolveCall(call);
      if (call->isResolved())
        resolveFns(call->isResolved());
      callStack.pop();
    } else if (SymExpr* sym = toSymExpr(expr)) {

      // avoid record constructors via cast
      //  should be fixed by out-of-order resolution
      CallExpr* parent = toCallExpr(sym->parentExpr);
      if (!parent ||
          !parent->isPrimitive(PRIMITIVE_ISSUBTYPE) ||
          !sym->var->isTypeVariable) {

        if (ClassType* ct = toClassType(sym->typeInfo())) {
          if (!ct->isGeneric && !ct->symbol->hasPragma("iterator class")) {
            resolveFormals(ct->defaultTypeConstructor);
            if (resolvedFormals.set_in(ct->defaultTypeConstructor))
              resolveFns(ct->defaultTypeConstructor);
          }
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

  if (fn->isExtern)
    return;

  if (fn->hasPragma("auto ii"))
    return;

  //
  // build value function for var functions
  //
  if (fn->retTag == RET_VAR) {
    if (fn->fnTag != FN_ITERATOR) {
      FnSymbol* copy = fn->copy();
      copy->visible = false;
      copy->retTag = RET_VALUE;
      fn->defPoint->insertBefore(new DefExpr(copy));
      fn->valueFunction = copy;
      Symbol* ret = copy->getReturnSymbol();
      Vec<BaseAST*> asts;
      collect_asts(copy, asts);
      forv_Vec(BaseAST, ast, asts) {
        if (SymExpr* se = toSymExpr(ast)) {
          if (se->var == copy->setter->sym)
            se->var = gFalse;
          else if (se->var == ret) {
            if (CallExpr* move = toCallExpr(se->parentExpr))
              if (move->isPrimitive(PRIMITIVE_MOVE))
                if (CallExpr* call = toCallExpr(move->get(2)))
                  if (call->isPrimitive(PRIMITIVE_SET_REF))
                    call->primitive = primitives[PRIMITIVE_GET_REF];
          }
        }
      }
      resolveFns(copy);
    }

    Vec<BaseAST*> asts;
    collect_asts(fn, asts);
    forv_Vec(BaseAST, ast, asts) {
      if (SymExpr* se = toSymExpr(ast)) {
        if (se->var == fn->setter->sym) {
          se->var = gTrue;
          if (fn->fnTag == FN_ITERATOR)
            USR_WARN(fn, "setter argument is not supported in iterators");
        }
      }
    }
  }

  insertFormalTemps(fn);

  resolveBody(fn->body);

  if (fn->hasPragma("type constructor")) {
    ClassType* ct = toClassType(fn->retType);
    if (!ct)
      INT_FATAL(fn, "Constructor has no class type");
    setScalarPromotionType(ct);
    fixTypeNames(ct);
  }

  Symbol* ret = fn->getReturnSymbol();
  Type* retType = ret->type;

  Vec<Type*> retTypes;
  Vec<Symbol*> retParams;

  for_exprs_postorder(expr, fn->body) {
    if (CallExpr* call = toCallExpr(expr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (SymExpr* sym = toSymExpr(call->get(1))) {
          if (sym->var == ret) {
            if (SymExpr* sym = toSymExpr(call->get(2)))
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
            bool requireScalarPromotion = false;
            if (!canDispatch(retTypes.v[j], retParams.v[j], retTypes.v[i], fn, &requireScalarPromotion))
              best = false;
            if (requireScalarPromotion)
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
      USR_FATAL(fn, "unable to resolve return type");
  }

  for_exprs_postorder(expr, fn->body) {
    if (CallExpr* call = toCallExpr(expr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (SymExpr* lhs = toSymExpr(call->get(1))) {
          if (lhs->var == ret) {
            if (SymExpr* rhs = toSymExpr(call->get(2))) {
              if (rhs->var->type != lhs->var->type) {
                CallExpr* cast = new CallExpr("_cast", lhs->var->type->symbol, rhs->remove());
                lhs->insertAfter(cast);
                resolveCall(cast);
                if (cast->isResolved())
                  resolveFns(cast->isResolved());
              }
            }
          }
        }
      }
    }
  }

  if (fn->fnTag == FN_ITERATOR && !fn->iteratorInfo) {
    prototypeIteratorClass(fn);
    makeRefType(fn->retType);
    resolvedFns.set_add(fn->iteratorInfo->advance);
    resolvedFns.set_add(fn->iteratorInfo->hasMore);
    resolvedFns.set_add(fn->iteratorInfo->getValue);
  }

  if (fn->hasPragma("type constructor")) {
    forv_Vec(Type, parent, fn->retType->dispatchParents) {
      if (toClassType(parent) && parent != dtValue && parent != dtObject && parent->defaultTypeConstructor) {
        resolveFormals(parent->defaultTypeConstructor);
        if (resolvedFormals.set_in(parent->defaultTypeConstructor))
          resolveFns(parent->defaultTypeConstructor);
      }
    }
    if (ClassType* ct = toClassType(fn->retType)) {
      for_fields(field, ct) {
        if (ClassType* fct = toClassType(field->type)) {
          if (fct->defaultTypeConstructor) {
            resolveFormals(fct->defaultTypeConstructor);
            if (resolvedFormals.set_in(fct->defaultTypeConstructor))
              resolveFns(fct->defaultTypeConstructor);
          }
        }
      }
    }

    //
    // instantiate default constructor
    //
    if (fn->instantiatedFrom) {
      INT_ASSERT(!fn->retType->defaultConstructor);
      FnSymbol* instantiatedFrom = fn->instantiatedFrom;
      while (instantiatedFrom->instantiatedFrom)
        instantiatedFrom = instantiatedFrom->instantiatedFrom;
      CallExpr* call = new CallExpr(instantiatedFrom->retType->defaultConstructor);
      for_formals(formal, fn) {
        if (formal->type == dtMethodToken || formal == fn->_this) {
          call->insertAtTail(formal);
        } else if (paramMap.get(formal)) {
          call->insertAtTail(new NamedExpr(formal->name, new SymExpr(paramMap.get(formal))));
        } else {
          Symbol* field = fn->retType->getField(formal->name);
          if (instantiatedFrom->hasPragma("tuple")) {
            call->insertAtTail(field);
          } else {
            call->insertAtTail(new NamedExpr(formal->name, new SymExpr(field)));
          }
        }
      }
      fn->insertBeforeReturn(call);
      resolveCall(call);
      fn->retType->defaultConstructor = call->isResolved();
      INT_ASSERT(fn->retType->defaultConstructor);
      //      resolveFns(fn->retType->defaultConstructor);
      call->remove();
    }
  }
}


static bool
possible_signature_match(FnSymbol* fn, FnSymbol* gn) {
  if (fn->name != gn->name)
    return false;
  if (fn->numFormals() != gn->numFormals())
    return false;
  for (int i = 3; i <= fn->numFormals(); i++) {
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
  if (fn->numFormals() != gn->numFormals())
    return false;
  for (int i = 3; i <= fn->numFormals(); i++) {
    ArgSymbol* fa = fn->getFormal(i);
    ArgSymbol* ga = gn->getFormal(i);
    if (strcmp(fa->name, ga->name))
      return false;
    if (fa->type != ga->type)
      return false;
  }
  return true;
}


//
// add to vector icts all types instantiated from ct
//
static void
collectInstantiatedClassTypes(Vec<Type*>& icts, Type* ct) {
  if (ct->defaultTypeConstructor->instantiatedTo)
    forv_Vec(FnSymbol, fn, *ct->defaultTypeConstructor->instantiatedTo) {
      Type* ict = fn->retType;
      if (ict->isGeneric)
        collectInstantiatedClassTypes(icts, ict);
      else
        icts.add(ict);
    }
}


static void
add_to_ddf(FnSymbol* pfn, ClassType* ct) {
  forv_Vec(FnSymbol, cfn, ct->methods) {
    if (cfn->instantiatedFrom)
      continue;
    if (cfn && possible_signature_match(pfn, cfn)) {
      if (ct->isGeneric) {
        ASTMap subs;
        Vec<Type*> icts;
        collectInstantiatedClassTypes(icts, ct);
        forv_Vec(Type, ict, icts) {
          subs.put(cfn->getFormal(2), ict);
          for (int i = 3; i <= cfn->numFormals(); i++) {
            ArgSymbol* arg = cfn->getFormal(i);
            if (arg->intent == INTENT_PARAM) {
              INT_FATAL(arg, "unhandled case");
            } else if (arg->type->isGeneric) {
              if (pfn->getFormal(i)) {
                subs.put(arg, pfn->getFormal(i)->type);
              }
            }
          }
          FnSymbol* icfn = instantiate(cfn, &subs, NULL);
          if (icfn) {
            icfn->instantiationPoint = ict->defaultTypeConstructor->instantiationPoint;
            INT_ASSERT(icfn->instantiationPoint);
            resolveFormals(icfn);
            if (signature_match(pfn, icfn)) {
              resolveFns(icfn);
              if (!isSubType(icfn->retType, pfn->retType)) {
                USR_FATAL_CONT(pfn, "conflicting return type specified for '%s: %s'", toString(pfn), pfn->retType->symbol->name);
                USR_FATAL_CONT(icfn, "  overridden by '%s: %s'", toString(icfn), icfn->retType->symbol->name);
                USR_STOP();
              }
              Vec<FnSymbol*>* fns = ddf.get(pfn);
              if (!fns) fns = new Vec<FnSymbol*>();
              fns->add(icfn);
              ddf.put(pfn, fns);
            }
          }
        }
      } else {
        ASTMap subs;
        for (int i = 3; i <= cfn->numFormals(); i++) {
          ArgSymbol* arg = cfn->getFormal(i);
          if (arg->intent == INTENT_PARAM) {
            subs.put(arg, paramMap.get(pfn->getFormal(i)));
          } else if (arg->type->isGeneric) {
            subs.put(arg, pfn->getFormal(i)->type);
          }
        }
        if (subs.n) {
          cfn = instantiate(cfn, &subs, NULL);
          if (cfn) {
            if (ct->defaultTypeConstructor->instantiationPoint)
              cfn->instantiationPoint = ct->defaultTypeConstructor->instantiationPoint;
            else
              cfn->instantiationPoint = toBlockStmt(ct->defaultTypeConstructor->defPoint->parentExpr);
            INT_ASSERT(cfn->instantiationPoint);
          }
        }
        if (cfn) {
          resolveFormals(cfn);
          if (signature_match(pfn, cfn)) {
            resolveFns(cfn);
            if (!isSubType(cfn->retType, pfn->retType)) {
              USR_FATAL_CONT(pfn, "conflicting return type specified for '%s: %s'", toString(pfn), pfn->retType->symbol->name);
              USR_FATAL_CONT(cfn, "  overridden by '%s: %s'", toString(cfn), cfn->retType->symbol->name);
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
}


static void
add_all_children_ddf_help(FnSymbol* fn, ClassType* pt, ClassType* ct) {
  if (ct->defaultTypeConstructor->instantiatedTo ||
      resolvedFns.set_in(ct->defaultTypeConstructor))
    add_to_ddf(fn, ct);
  forv_Vec(Type, t, ct->dispatchChildren) {
    ClassType* ct = toClassType(t);
    if (!ct->instantiatedFrom)
      add_all_children_ddf_help(fn, pt, ct);
  }
}


static void
add_all_children_ddf(FnSymbol* fn, ClassType* pt) {
  forv_Vec(Type, t, pt->dispatchChildren) {
    ClassType* ct = toClassType(t);
    if (!ct->instantiatedFrom)
      add_all_children_ddf_help(fn, pt, ct);
  }
}


static void
build_ddf() {
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->isWrapper || !resolvedFns.set_in(fn) || fn->noParens)
      continue;
    if (fn->numFormals() > 1) {
      if (fn->getFormal(1)->type == dtMethodToken) {
        if (ClassType* pt = toClassType(fn->getFormal(2)->type)) {
          if (pt->classTag == CLASS_CLASS && !pt->isGeneric) {
            add_all_children_ddf(fn, pt);
          }
        }
      }
    }
  }
}


static void
parseExplainFlag(char* flag, int* line, ModuleSymbol** module) {
  *line = 0;
  if (strcmp(flag, "")) {
    char *token, *str1 = NULL, *str2 = NULL;
    token = strstr(flag, ":");
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
            *module = mod;
            break;
          }
        }
        if (!*module)
          USR_FATAL("invalid module name '%s' passed to --explain-call flag", str1);
      } else
        *line = atoi(str1);
      if (str2)
        *line = atoi(str2);
    }
    if (*line == 0)
      *line = -1;
  }
}


void
resolve() {
  parseExplainFlag(fExplainCall, &explainCallLine, &explainCallModule);
  parseExplainFlag(fExplainInstantiation, &explainInstantiationLine, &explainInstantiationModule);

  // call _nilType nil so as to not confuse the user
  dtNil->symbol->name = gNil->name;

  bool changed = true;
  while (changed) {
    changed = false;
    forv_Vec(FnSymbol, fn, gFns) {
      changed = fn->tag_generic() || changed;
    }
  }

  //
  // make it so that arguments with types that have default values for
  // all generic arguments used those defaults
  //
  // markedGeneric is used to identify places where the user inserted
  // '?' (queries) to mark such a type as generic.
  //
  forv_Vec(FnSymbol, fn, gFns) {
    bool unmark = fn->isGeneric;
    for_formals(formal, fn) {
      if (formal->type->hasGenericDefaults) {
        if (!formal->markedGeneric &&
            formal != fn->_this &&
            !formal->hasPragma("is meme")) {
          formal->typeExpr = new BlockStmt(new CallExpr(formal->type->defaultTypeConstructor));
          insert_help(formal->typeExpr, NULL, formal);
          formal->type = dtUnknown;
        } else {
          unmark = false;
        }
      } else if (formal->type->isGeneric || formal->intent == INTENT_PARAM) {
        unmark = false;
      }
    }
    if (unmark) {
      fn->isGeneric = false;
      INT_ASSERT(false);
    }
  }

  resolveFns(chpl_main);

  // need to handle enumerated types better
  forv_Vec(TypeSymbol, type, gTypes) {
    if (EnumType* et = toEnumType(type->type)) {
      for_enums(def, et) {
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
        printf("  %s\n", toString(ddf.v[i].key));
        for (int j = 0; j < ddf.v[i].value->n; j++) {
          printf("    %s\n", toString(ddf.v[i].value->v[j]));
        }
      }
    }
  }

  //
  // resolve PRIMITIVE_INITs for records
  //
  forv_Vec(CallExpr, init, inits) {
    if (init->parentSymbol) {
      Type* type = init->get(1)->typeInfo();
      if (type->symbol->hasPragma("array"))
        INT_FATAL(init, "PRIMITIVE_INIT should have been replaced already");
      if (type->symbol->hasPragma("ref"))
        type = getValueType(type);
      if (type->defaultValue) {
        INT_FATAL(init, "PRIMITIVE_INIT should have been replaced already");
      } else {
        INT_ASSERT(type->defaultConstructor);
        CallExpr* call = new CallExpr(type->defaultConstructor);
        init->replace(call);
        resolveCall(call);
        if (call->isResolved())
          resolveFns(call->isResolved());
      }
    }
  }

  //
  // resolve buildArray functions
  //
  {
    Vec<FnSymbol*> fns;
    buildArrayMap.get_values(fns);
    forv_Vec(FnSymbol, fn, fns) {
      resolveFns(fn);
    }
  }


  Vec<CallExpr*> calls;
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast))
      if (call->parentSymbol && call->getStmtExpr())
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
          VarSymbol* cid = new VarSymbol("tmp", dtBool);
          cid->isCompilerTemp = true;
          if_fn->insertAtTail(new DefExpr(cid));
          if_fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cid,
                                new CallExpr(PRIMITIVE_GETCID,
                                             call->get(2)->copy(),
                                             type->symbol)));
          if_fn->insertAtTail(new DefExpr(_ret));
          BlockStmt* trueBlock = new BlockStmt();
          if (fn->retType == key->retType) {
            trueBlock->insertAtTail(new CallExpr(PRIMITIVE_MOVE, _ret, subcall));
          } else if (isSubType(fn->retType, key->retType)) {
            // Insert a cast to the overridden method's return type
            VarSymbol* castTemp = new VarSymbol("_castTemp", fn->retType);
            castTemp->isCompilerTemp = true;
            trueBlock->insertAtTail(new DefExpr(castTemp));
            trueBlock->insertAtTail(new CallExpr(PRIMITIVE_MOVE, castTemp,
                                                 subcall));
            trueBlock->insertAtTail(new CallExpr(PRIMITIVE_MOVE, _ret,
                                      new CallExpr(PRIMITIVE_CAST,
                                                   key->retType->symbol,
                                                   castTemp)));
          } else {
            USR_FATAL_CONT(key, "conflicting return type specified for '%s: %s'", toString(key), key->retType->symbol->name);
            USR_FATAL_CONT(fn, "  overridden by '%s: %s'", toString(fn), fn->retType->symbol->name);
            USR_STOP();
          }
          if_fn->insertAtTail(
            new CondStmt(
              new SymExpr(cid),
              trueBlock,
              new CallExpr(PRIMITIVE_MOVE, _ret, tmp)));
          if_fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, _ret));
          if_fn->retType = key->retType;
          if (key->retType == dtUnknown)
            INT_FATAL(call, "bad parent virtual function return type");
          call->getStmtExpr()->insertBefore(new DefExpr(if_fn));
          call->replace(new CallExpr(if_fn));
          tmp->replace(call);
          subcall->baseExpr->replace(new SymExpr(fn));
          if (SymExpr* se = toSymExpr(subcall->get(2))) {
            VarSymbol* tmp = new VarSymbol("_tmp", type);
            tmp->isCompilerTemp = true;
            se->getStmtExpr()->insertBefore(new DefExpr(tmp));
            se->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_CAST, type->symbol, se->var)));
            se->replace(new SymExpr(tmp));
          } else if (CallExpr* ce = toCallExpr(subcall->get(2)))
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

  insertReturnTemps(); // must be done before pruneResolvedTree is called.
  pruneResolvedTree();

  freeWrapperAndInstantiationCaches();

  Vec<VisibleFunctionBlock*> vfbs;
  visibleFunctionMap.get_values(vfbs);
  forv_Vec(VisibleFunctionBlock, vfb, vfbs) {
    Vec<Vec<FnSymbol*>*> vfns;
    vfb->visibleFunctions.get_values(vfns);
    forv_Vec(Vec<FnSymbol*>, vfn, vfns) {
      delete vfn;
    }
    delete vfb;
  }
  visibleFunctionMap.clear();
  visibilityBlockCache.clear();
}


static Type*
buildArrayTypeInfo(Type* type) {
  static Map<Type*,Type*> cache;
  if (cache.get(type))
    return cache.get(type);
  ClassType* ct = new ClassType(CLASS_RECORD);
  TypeSymbol* ts = new TypeSymbol(astr("_ArrayTypeInfo"), ct);
  Type* elt = type->getField("eltType")->type;
  if (elt->symbol->hasPragma("array"))
    elt = buildArrayTypeInfo(elt);
  ct->fields.insertAtTail(new DefExpr(new VarSymbol("elt", elt)));
  ct->fields.insertAtTail(new DefExpr(new VarSymbol("dom", type->getField("_value")->type->getField("dom")->type)));
  theProgram->block->insertAtTail(new DefExpr(ts));
  cache.put(type, ct);
  ct->symbol->pragmas.add("_ArrayTypeInfo");
  return ct;
}


static void insertReturnTemps() {
  //
  // Insert return temps for functions that return values if no
  // variable captures the result. If the value is a sync var or a
  // reference to a sync var, pass it through the _statementLevelSymbol
  // function to get the semantics of reading a sync var.
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->parentSymbol) {
        if (FnSymbol* fn = call->isResolved()) {
          if (fn->retType != dtVoid) {
            CallExpr* parent = toCallExpr(call->parentExpr);
            if (!parent && !isDefExpr(call->parentExpr)) { // no use
              VarSymbol* tmp = new VarSymbol("_dummy", fn->retType);
              DefExpr* def = new DefExpr(tmp);
              call->insertBefore(def);
              if ((getValueType(fn->retType) && getValueType(fn->retType)->symbol->hasPragma("sync")) || fn->retType->symbol->hasPragma("sync")) {
                CallExpr* sls = new CallExpr("_statementLevelSymbol", tmp);
                call->insertBefore(sls);
                reset_line_info(sls, call->lineno);
                resolveCall(sls);
                INT_ASSERT(sls->isResolved());
                resolveFns(sls->isResolved());
              }
              def->insertAfter(new CallExpr(PRIMITIVE_MOVE, tmp, call->remove()));
            }
          }
        }
      }
    }
  }
}


//
// insert code to initialize a class or record
//
static void
initializeClass(Expr* stmt, Symbol* sym) {
  ClassType* ct = toClassType(sym->type);
  INT_ASSERT(ct);
  for_fields(field, ct) {
    if (!field->hasPragma("super class")) {
      if (field->type->defaultValue) {
        stmt->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, sym, field, field->type->defaultValue));
      } else if (isRecordType(field->type)) {
        VarSymbol* tmp = new VarSymbol("_tmp", field->type);
        tmp->isCompilerTemp = true;
        stmt->insertBefore(new DefExpr(tmp));
        initializeClass(stmt, tmp);
        stmt->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, sym, field, tmp));
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
      if (!resolvedFns.set_in(fn) || fn->retTag == RET_PARAM)
        fn->defPoint->remove();
    }
  }

  // Remove unused types
  forv_Vec(TypeSymbol, type, gTypes) {
    if (type->defPoint && type->defPoint->parentSymbol)
      if (!type->hasPragma("ref"))
        if (ClassType* ct = toClassType(type->type))
          if (!resolvedFns.set_in(ct->defaultConstructor) &&
              !resolvedFns.set_in(ct->defaultTypeConstructor)) {
            if (ct->symbol->hasPragma("object class"))
              dtObject = NULL;
            ct->symbol->defPoint->remove();
          }
  }
  forv_Vec(TypeSymbol, type, gTypes) {
    if (type->defPoint && type->defPoint->parentSymbol) {
      if (type->hasPragma("ref") && type->type != dtNilRef) {
        if (ClassType* ct = toClassType(getValueType(type->type))) {
          if (!resolvedFns.set_in(ct->defaultConstructor) &&
              !resolvedFns.set_in(ct->defaultTypeConstructor)) {
            if (ct->symbol->hasPragma("object class"))
              dtObject = NULL;
            type->defPoint->remove();
          }
        }
        if (type->type->defaultTypeConstructor->defPoint->parentSymbol)
          type->type->defaultTypeConstructor->defPoint->remove();
      }
    }
  }

  Vec<BaseAST*> asts;
  collect_asts_postorder(rootModule, asts);
  forv_Vec(BaseAST, ast, asts) {
    Expr* expr = toExpr(ast);
    if (!expr || !expr->parentSymbol)
      continue;
    if (DefExpr* def = toDefExpr(ast)) {
      // Remove unused global variables
      if (toVarSymbol(def->sym))
        if (toModuleSymbol(def->parentSymbol))
          if (def->sym->type == dtUnknown)
            def->remove();
    } else if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_NOOP)) {
        // Remove Noops
        call->remove();
      } else if (call->isPrimitive(PRIMITIVE_TYPEOF)) {
        Type* type = call->get(1)->typeInfo();
        if (type->symbol->hasPragma("ref"))
          type = getValueType(type);
        if (!type->symbol->hasPragma("array")) {
          // Remove move(x, PRIMITIVE_TYPEOF(y)) calls -- useless after this
          CallExpr* parentCall = toCallExpr(call->parentExpr);
          if (parentCall && parentCall->isPrimitive(PRIMITIVE_MOVE) && 
              parentCall->get(2) == call) {
            parentCall->remove();
          } else {
            // Replace PRIMITIVE_TYPEOF with argument
            call->replace(call->get(1)->remove());
          }
        }
      } else if (call->isPrimitive(PRIMITIVE_ARRAY_ALLOC)) {
        // Capture array types for allocation before runtime array
        // types are inserted
        //        call->get(2)->replace(new SymExpr(call->get(2)->typeInfo()->symbol));
      } else if (call->isPrimitive(PRIMITIVE_CAST)) {
        if (call->get(1)->typeInfo() == call->get(2)->typeInfo())
          call->replace(call->get(2)->remove());
      } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER) ||
                 call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
                 call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) {
        // Remove member accesses of types
        // Replace string literals with field symbols in member primitives
        Type* baseType = call->get(1)->typeInfo();
        if (!call->parentSymbol->hasPragma("ref") &&
            baseType->symbol->hasPragma("ref"))
          baseType = getValueType(baseType);
        const char* memberName = get_string(call->get(2));
        Symbol* sym = baseType->getField(memberName);
        if ((sym->isTypeVariable && !sym->type->symbol->hasPragma("array")) ||
            !strcmp(sym->name, "_promotionType") ||
            sym->isParameter())
          call->getStmtExpr()->remove();
        else
          call->get(2)->replace(new SymExpr(sym));
      } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
        // Remove types to enable --baseline
        SymExpr* sym = toSymExpr(call->get(2));
        if (sym && toTypeSymbol(sym->var))
          call->remove();
//       } else if (call->isNamed("_init")) {
//         // Special handling of class init to avoid infinite recursion
//         if (ClassType* ct = toClassType(call->typeInfo())) {
//           if (ct->defaultValue) {
//             removeActual(call->get(1));
//             call->replace(new CallExpr(PRIMITIVE_CAST, ct->symbol, gNil));
//           }
//         }
      } else if (FnSymbol* fn = call->isResolved()) {
        // Remove method token actuals
        for (int i = fn->numFormals(); i >= 1; i--) {
          ArgSymbol* formal = fn->getFormal(i);
          if (formal->type == dtMethodToken ||
              (formal->isTypeVariable &&
               !formal->type->symbol->hasPragma("array")))
            call->get(i)->remove();
        }
      }
    } else if (NamedExpr* named = toNamedExpr(ast)) {
      // Remove names of named actuals
      Expr* actual = named->actual;
      actual->remove();
      named->replace(actual);
    } else if (BlockStmt* block = toBlockStmt(ast)) {
      // Remove type blocks--code that exists only to determine types
      if (block->blockTag == BLOCK_TYPE)
        block->remove();
    }
  }

  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      Vec<BaseAST*> asts;
      for_formals(formal, fn) {
        // Remove formal default values
        if (formal->defaultExpr)
          formal->defaultExpr->remove();
        // Remove formal type expressions
        if (formal->typeExpr)
          formal->typeExpr->remove();
        // Remove method token formals
        if (formal->type == dtMethodToken)
          formal->defPoint->remove();
        if (formal->isTypeVariable &&
            !formal->type->symbol->hasPragma("array")) {
          formal->defPoint->remove();
          VarSymbol* tmp = new VarSymbol("_removed_formal_tmp", formal->type);
          tmp->isCompilerTemp = true;
          fn->insertAtHead(new DefExpr(tmp));
          if (asts.n == 0)
            collect_asts(fn->body, asts);
          forv_Vec(BaseAST, ast, asts) {
            if (SymExpr* se = toSymExpr(ast)) {
              if (se->var == formal) {
                if (CallExpr* call = toCallExpr(se->parentExpr))
                  if (call->isPrimitive(PRIMITIVE_GET_REF))
                    se->getStmtExpr()->remove();
                se->var = tmp;
              }
            }
          }
        }
        if (formal->isTypeVariable &&
            formal->type->symbol->hasPragma("array")) {
          formal->type = buildArrayTypeInfo(formal->type);
          formal->isTypeVariable = false;
        }
      }
      if (fn->where)
        fn->where->remove();
      if (fn->retTag == RET_TYPE) {
        VarSymbol* ret = toVarSymbol(fn->getReturnSymbol());
        if (ret && ret->type->symbol->hasPragma("array")) {
          ret->type = buildArrayTypeInfo(ret->type);
          fn->retType = ret->type;
          fn->retTag = RET_VALUE;
        }
      }
    }
  }

  asts.clear();
  collect_asts_postorder(rootModule, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->parentSymbol && call->isPrimitive(PRIMITIVE_BUILD_ARRAY)) {
        SymExpr* se = toSymExpr(call->get(1));
        Symbol* eltField = se->var->type->getField("elt");
        VarSymbol* elt = new VarSymbol("_tmp", eltField->type);
        elt->isTypeVariable = true;
        call->getStmtExpr()->insertBefore(new DefExpr(elt));
        // BLC: if the field is an array, process it; otherwise, remove it
        if (elt->type->symbol->hasPragma("_ArrayTypeInfo"))
          call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, elt, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, se->var, eltField)));
        else
          eltField->defPoint->remove();
        VarSymbol* dom = new VarSymbol("_tmp", se->var->type->getField("dom")->type);
        call->getStmtExpr()->insertBefore(new DefExpr(dom));
        call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, dom, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, se->var, se->var->type->getField("dom"))));
        FnSymbol* wrapper = wrapDomainMap.get(dom->type);
        VarSymbol* tmp = new VarSymbol("_tmp", wrapper->retType);
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(wrapper, dom)));
        if (elt->type->symbol->hasPragma("_ArrayTypeInfo"))
          call->replace(new CallExpr(buildArrayMap.get(call), tmp, elt));
        else
          call->replace(new CallExpr(buildArrayMap.get(call), tmp));
      } else if (call->parentSymbol && call->isPrimitive(PRIMITIVE_TYPEOF)) {
        // handle ".type" on an array by building a runtime array type
        CallExpr* move = toCallExpr(call->parentExpr);
        INT_ASSERT(move);
        SymExpr* lhs = toSymExpr(move->get(1));
        SymExpr* rhs = toSymExpr(call->get(1));
        INT_ASSERT(lhs);
        INT_ASSERT(rhs);
        Type* rhsType = rhs->var->type;
        if (rhsType->symbol->hasPragma("ref"))
          rhsType = getValueType(rhsType);
        Type* lhsType = lhs->var->type;
        if (lhsType->symbol->hasPragma("ref"))
          lhsType = getValueType(lhsType);
        VarSymbol* _value = new VarSymbol("_tmp", rhsType->getField("_value")->type);
        call->getStmtExpr()->insertBefore(new DefExpr(_value));
        call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, _value, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, rhs->var, rhsType->getField("_value"))));
        // Query domain and copy over
        VarSymbol* dom = new VarSymbol("_tmp", _value->type->getField("dom")->type);
        call->getStmtExpr()->insertBefore(new DefExpr(dom));
        call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, dom, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, _value, _value->type->getField("dom"))));
        Symbol* domField = lhsType->getField("dom");
        INT_ASSERT(domField);
        CallExpr* domFieldAssign = new CallExpr(PRIMITIVE_SET_MEMBER, lhs->var, domField, dom);
        move->replace(domFieldAssign);

        // Query element type and check whether it is an array of arrays
        // If so, we need to copy its eltType into the runtime array type's elt
        Symbol* eltTypeField = _value->type->getField("eltType");
        Type* eltTypeType = eltTypeField->type;
        if (eltTypeType->symbol->hasPragma("array")) {
          VarSymbol* eltType = new VarSymbol("_tmp", buildArrayTypeInfo(eltTypeType));
          domFieldAssign->getStmtExpr()->insertBefore(new DefExpr(eltType));
          domFieldAssign->insertBefore(new CallExpr(PRIMITIVE_MOVE, eltType, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, _value, eltTypeField)));
          Symbol* eltField = lhsType->getField("elt");
          INT_ASSERT(eltField);
          domFieldAssign->insertAfter(new CallExpr(PRIMITIVE_SET_MEMBER, lhs->var, eltField, eltType));
        }
      } else if (call->parentSymbol && call->isNamed("_init")) {

        //
        // special handling of tuple constructor to avoid
        // initialization of array based on an array type symbol
        // rather than a runtime array type
        //
        // this code added during the introduction of the new keyword;
        // it should be removed when possible
        //
        if (!strcmp("_construct__tuple", call->parentSymbol->name))
          if (SymExpr* se = toSymExpr(call->get(1)))
            if (se->var->type->symbol->hasPragma("array"))
              call->parentExpr->remove();

      }
    } else if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (!strcmp(fn->name, "_build_array_type")) {
          BlockStmt* block = new BlockStmt();
          VarSymbol* tmp = new VarSymbol("_tmp", fn->getFormal(1)->type->getField("_value")->type);
          block->insertAtTail(new DefExpr(tmp));
          block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, fn->getFormal(1), fn->getFormal(1)->type->getField("_value"))));
          VarSymbol* var = new VarSymbol("_tmp", fn->retType);
          block->insertAtTail(new DefExpr(var));
          Symbol* domField = var->type->getField("dom");
          INT_ASSERT(domField);
          block->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, var, domField, tmp));
          if (fn->formals.length() > 1) {
            Symbol* eltField = var->type->getField("elt");
            INT_ASSERT(domField);
            block->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, var, eltField, fn->getFormal(2)));
          }
          block->insertAtTail(new CallExpr(PRIMITIVE_RETURN, var));
          fn->body->replace(block);
        }
      } else if (isVarSymbol(def->sym) &&
                 def->sym->isTypeVariable &&
                 def->sym->type->symbol->hasPragma("array")) {
        def->sym->type = buildArrayTypeInfo(def->sym->type);
        def->sym->isTypeVariable = false;
      }
    } else if (SymExpr* se = toSymExpr(ast)) {

      // remove dead type expressions
      if (se->getStmtExpr() == se)
        if (se->var->isTypeVariable)
          se->remove();

    }
  }

  // Remove type fields, parameter fields, and _promotionType field
  forv_Vec(TypeSymbol, type, gTypes) {
    if (type->defPoint && type->defPoint->parentSymbol) {
      if (ClassType* ct = toClassType(type->type)) {
        for_fields(field, ct) {
          if (field->isTypeVariable ||
              field->isParameter() ||
              !strcmp(field->name, "_promotionType"))
            field->defPoint->remove();
        }
      }
    }
  }

  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->parentSymbol && call->isResolved()) {
        //
        // Insert reference temps for function arguments that expect them.
        //
        for_formals_actuals(formal, actual, call) {
          if (formal->type == actual->typeInfo()->refType) {
            currentLineno = call->lineno;
            VarSymbol* tmp = new VarSymbol("_tmp", formal->type);
            tmp->isCompilerTemp = true;
            call->getStmtExpr()->insertBefore(new DefExpr(tmp));
            actual->replace(new SymExpr(tmp));
            call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_SET_REF, actual)));
          }
        }
      } else if (call->isPrimitive(PRIMITIVE_INIT_FIELDS)) {
        initializeClass(call, toSymExpr(call->get(1))->var);
        call->remove();
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
    const char* domain_type = ct->getField("dom")->type->symbol->name;
    const char* elt_type = ct->getField("eltType")->type->symbol->name;
    ct->symbol->name = astr("[", domain_type, "] ", elt_type);
  }
  if (ct->instantiatedFrom &&
      !strcmp(ct->instantiatedFrom->symbol->name, "SingleLocaleArithmeticDomain")) {
    ct->symbol->name = astr("domain", ct->symbol->name+28);
  }
  if (ct->symbol->hasPragma("array") || ct->symbol->hasPragma("domain")) {
    ct->symbol->name = ct->getField("_value")->type->symbol->name;
  }
}


static void
setScalarPromotionType(ClassType* ct) {
  for_fields(field, ct) {
    if (!strcmp(field->name, "_promotionType"))
      ct->scalarPromotionType = field->type;
  }
}


static FnSymbol*
instantiate(FnSymbol* fn, ASTMap* subs, CallExpr* call) {
  static Vec<FnSymbol*> reportSet;  // do not report cached instantiations
  static Vec<FnSymbol*> whereStack;
  FnSymbol* ifn = fn->instantiate_generic(subs, &paramMap, call);

  ifn->isExtern = fn->isExtern; // preserve extern-ness of instantiated fn
  if (!ifn->isGeneric && ifn->where) {
    forv_Vec(FnSymbol, where, whereStack) {
      if (where == ifn) {
        USR_FATAL_CONT(ifn->where, "illegal where clause due to infinite instantiation");
        FnSymbol* printOn = NULL;
        forv_Vec(FnSymbol, tmp, whereStack) {
          if (printOn)
            USR_PRINT(printOn->where, "evaluation of '%s' where clause results in instantiation of '%s'", printOn->name, tmp->name);
          if (printOn || tmp == where)
            printOn = tmp;
        }
        USR_PRINT(ifn->where, "evaluation of '%s' where clause results in instantiation of '%s'", printOn->name, ifn->name);
        USR_STOP();
      }
    }
    whereStack.add(ifn);
    resolveFormals(ifn);
    resolveBody(ifn->where);
    whereStack.pop();
    SymExpr* symExpr = toSymExpr(ifn->where->body.last());
    if (!symExpr)
      USR_FATAL(ifn->where, "illegal where clause");
    if (!strcmp(symExpr->var->name, "false"))
      return NULL;
    if (strcmp(symExpr->var->name, "true"))
      USR_FATAL(ifn->where, "illegal where clause");
  }

  if (!ifn->isGeneric &&
      explainInstantiationLine &&
      explainInstantiationMatch(fn) &&
      !reportSet.set_in(ifn)) {
    char msg[1024] = "";
    int len;
    if (!strncmp(fn->name, "_construct_", 11))
      len = sprintf(msg, "instantiated %s(", fn->_this->type->symbol->name);
    else
      len = sprintf(msg, "instantiated %s(", fn->name);
    bool first = true;
    for_formals(formal, ifn) {
      form_Map(ASTMapElem, e, ifn->substitutions) {
        ArgSymbol* arg = toArgSymbol(e->key);
        if (!strcmp(formal->name, arg->name)) {
          if (!strcmp(arg->name, "meme")) // do not show meme argument
            continue;
          if (first)
            first = false;
          else
            len += sprintf(msg+len, ", ");
          INT_ASSERT(arg);
          if (strcmp(ifn->name, "_construct__tuple"))
            len += sprintf(msg+len, "%s = ", arg->name);
          if (Type* t = toType(e->value))
            len += sprintf(msg+len, "%s", t->symbol->name);
          else if (Symbol* s = toSymbol(e->value)) {
            VarSymbol* vs = toVarSymbol(s);
            if (vs && vs->immediate && vs->immediate->const_kind == NUM_KIND_INT)
              len += sprintf(msg+len, "%lld", vs->immediate->int_value());
            else if (vs && vs->immediate && vs->immediate->const_kind == CONST_KIND_STRING)
              len += sprintf(msg+len, "\"%s\"", vs->immediate->v_string);
            else
              len += sprintf(msg+len, "%s", s->name);
          } else
            INT_FATAL("unexpected case using --explain-instantiation");
        }
      }
    }
    len += sprintf(msg+len, ")");
    if (callStack.n) {
      USR_PRINT(callStack.v[callStack.n-1], msg);
    } else {
      USR_PRINT(fn, msg);
    }
    reportSet.set_add(ifn);
  }
  return ifn;
}

