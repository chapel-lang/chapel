#include "astutil.h"
#include "build.h"
#include "caches.h"
#include "callInfo.h"
#include "expr.h"
#include "iterator.h"
#include "passes.h"
#include "resolution.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "../ifa/prim_data.h"

SymbolMap paramMap;
static Expr* dropUnnecessaryCast(CallExpr* call);
static void foldEnumOp(int op, EnumSymbol *e1, EnumSymbol *e2, Immediate *imm);
static Expr* preFold(Expr* expr);
static Expr* postFold(Expr* expr);

static void setScalarPromotionType(ClassType* ct);
static void fixTypeNames(ClassType* ct);
static void insertReturnTemps();
static bool canParamCoerce(Type* actualType, Symbol* actualSym, Type* formalType);

static int explainCallLine;
static ModuleSymbol* explainCallModule;

static Vec<CallExpr*> inits;
static Vec<FnSymbol*> resolvedFns;
static Vec<FnSymbol*> resolvedFormals;
Vec<CallExpr*> callStack;

static Map<FnSymbol*,Vec<FnSymbol*>*> ddf; // map of functions to
                                           // virtual children

static Map<Type*,Type*> runtimeTypeMap; // map static types to runtime types
                                        // e.g. array and domain runtime types
static Map<Type*,FnSymbol*> valueToRuntimeTypeMap; // convertValueToRuntimeType
static Map<Type*,FnSymbol*> runtimeTypeToValueMap; // convertRuntimeTypeToValue

static Type* resolve_type_expr(Expr* expr);

static void resolveFns(FnSymbol* fn);

static void pruneResolvedTree();


//
// build reference type
//
static void makeRefType(Type* type) {
  if (!type)
    return;

  if (type == dtMethodToken ||
      type == dtLeaderToken ||
      type == dtUnknown ||
      type->symbol->hasFlag(FLAG_REF) ||
      type->symbol->hasFlag(FLAG_GENERIC))
    return;

  if (type->refType)
    return;

  CallExpr* call = new CallExpr("_type_construct__ref", type->symbol);
  if (type->defaultConstructor) {
    if (type->defaultConstructor->instantiationPoint)
      type->defaultConstructor->instantiationPoint->insertAtHead(call);
    else
      type->symbol->defPoint->insertBefore(call);
  } else
    chpl_main->insertAtHead(call);
  resolveCall(call);
  call->remove();
  type->refType = toClassType(call->isResolved()->retType);
  type->refType->getField(1)->type = type;
}


const char* toString(Type* type) {
  if (type->symbol->hasFlag(FLAG_REF))
    return type->getValueType()->symbol->name;
  else
    return type->symbol->name;
}


const char* toString(CallInfo* info) {
  bool method = false;
  bool _this = false;
  const char *str = "";
  if (info->actuals.n > 1)
    if (info->actuals.v[0]->type == dtMethodToken)
      method = true;
  if (!strcmp("this", info->name)) {
    _this = true;
    method = false;
  }
  if (method) {
    if (info->actuals.v[1] && info->actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE))
      str = astr(str, "type ", toString(info->actuals.v[1]->type), ".");
    else
      str = astr(str, toString(info->actuals.v[1]->type), ".");
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
  for (int i = start; i < info->actuals.n; i++) {
    if (!first)
      first = true;
    else
      str = astr(str, ", ");
    if (info->actualNames.v[i])
      str = astr(str, info->actualNames.v[i], "=");
    VarSymbol* var = toVarSymbol(info->actuals.v[i]);
    if (info->actuals.v[i]->type->symbol->hasFlag(FLAG_ITERATOR_CLASS) &&
        info->actuals.v[i]->type->defaultConstructor->hasFlag(FLAG_PROMOTION_WRAPPER))
      str = astr(str, "promoted expression");
    else if (info->actuals.v[i] && info->actuals.v[i]->hasFlag(FLAG_TYPE_VARIABLE))
      str = astr(str, "type ", toString(info->actuals.v[i]->type));
    else if (var && var->immediate) {
      if (var->immediate->const_kind == CONST_KIND_STRING) {
        str = astr(str, "\"", var->immediate->v_string, "\"");
      } else {
        char buff[512];
        sprint_imm(buff, *var->immediate);
        str = astr(str, buff);
      }
    } else
      str = astr(str, toString(info->actuals.v[i]->type));
  }
  if (!info->call->methodTag) {
    if (info->call->square)
      str = astr(str, "]");
    else
      str = astr(str, ")");
  }
  return str;
}


const char* toString(FnSymbol* fn) {
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
  } else if (fn->hasFlag(FLAG_METHOD)) {
    if (!strcmp(fn->name, "this")) {
      str = astr(toString(fn->getFormal(2)->type));
      start = 1;
    } else {
      str = astr(toString(fn->getFormal(2)->type), ".", fn->name);
      start = 2;
    }
  } else
    str = astr(fn->name);
  
  if (!fn->hasFlag(FLAG_NO_PARENS) &&
      !(!strncmp("_type_construct_", fn->name, 16) && fn->numFormals() == 0))
    str = astr(str, "(");
  bool first = false;
  for (int i = start; i < fn->numFormals(); i++) {
    ArgSymbol* arg = fn->getFormal(i+1);
    if (arg->hasFlag(FLAG_IS_MEME))
      continue;
    if (!first)
      first = true;
    else
      str = astr(str, ", ");
    if (arg->intent == INTENT_PARAM)
      str = astr(str, "param ");
    if (arg->hasFlag(FLAG_TYPE_VARIABLE))
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
  if (!fn->hasFlag(FLAG_NO_PARENS) &&
      !(!strncmp("_type_construct_", fn->name, 16) && fn->numFormals() == 0))
    str = astr(str, ")");
  if (developer)
    str = astr(str, " [", istr(fn->id), "]");
  return str;
}


static FnSymbol*
protoIteratorMethod(IteratorInfo* ii, const char* name, Type* retType) {
  FnSymbol* fn = new FnSymbol(name);
  fn->addFlag(FLAG_AUTO_II); 
  if (strcmp(name, "advance"))
    fn->addFlag(FLAG_INLINE);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", ii->icType);
  fn->retType = retType;
  fn->insertFormalAtTail(fn->_this);
  ii->iterator->defPoint->insertBefore(new DefExpr(fn));
  return fn;
}


static void
protoIteratorClass(FnSymbol* fn) {
  INT_ASSERT(!fn->iteratorInfo);

  SET_LINENO(fn);

  IteratorInfo* ii = new IteratorInfo();
  fn->iteratorInfo = ii;
  fn->iteratorInfo->iterator = fn;

  const char* className = astr("_ic_", fn->name);
  if (fn->_this)
    className = astr(className, "_", fn->_this->type->symbol->cname);
  ii->icType = new ClassType(CLASS_CLASS);
  TypeSymbol* cts = new TypeSymbol(className, ii->icType);
  cts->addFlag(FLAG_ITERATOR_CLASS);
  if (fn->retTag == RET_VAR)
    cts->addFlag(FLAG_REF_ITERATOR_CLASS);
  fn->defPoint->insertBefore(new DefExpr(cts));

  ii->tag = it_iterator;
  ii->advance = protoIteratorMethod(ii, "advance", dtVoid);
  ii->zip1 = protoIteratorMethod(ii, "zip1", dtVoid);
  ii->zip2 = protoIteratorMethod(ii, "zip2", dtVoid);
  ii->zip3 = protoIteratorMethod(ii, "zip3", dtVoid);
  ii->zip4 = protoIteratorMethod(ii, "zip4", dtVoid);
  ii->hasMore = protoIteratorMethod(ii, "hasMore", dtInt[INT_SIZE_32]);
  ii->getValue = protoIteratorMethod(ii, "getValue", fn->retType);

  ii->icType->defaultConstructor = fn;
  ii->icType->scalarPromotionType = fn->retType;
  fn->retType = ii->icType;
  fn->retTag = RET_VALUE;

  makeRefType(fn->retType);

  resolvedFns.set_add(fn->iteratorInfo->zip1);
  resolvedFns.set_add(fn->iteratorInfo->zip2);
  resolvedFns.set_add(fn->iteratorInfo->zip3);
  resolvedFns.set_add(fn->iteratorInfo->zip4);
  resolvedFns.set_add(fn->iteratorInfo->advance);
  resolvedFns.set_add(fn->iteratorInfo->hasMore);
  resolvedFns.set_add(fn->iteratorInfo->getValue);

  VarSymbol* tmp = newTemp(ii->icType);
  fn->insertAtHead(new DefExpr(tmp));
  CallExpr* getIteratorCall = new CallExpr("_getIterator", tmp);
  fn->insertAtHead(getIteratorCall);
  resolveCall(getIteratorCall);
  resolveFns(getIteratorCall->isResolved());
  getIteratorCall->remove();
  tmp->defPoint->remove();
}


static void
resolveSpecifiedReturnType(FnSymbol* fn) {
  resolveBlock(fn->retExprType);
  fn->retType = fn->retExprType->body.tail->typeInfo();
  if (fn->retType != dtUnknown) {
    if (fn->retTag == RET_VAR) {
      makeRefType(fn->retType);
      fn->retType = fn->retType->refType;
    }
    fn->retExprType->remove();
    if (fn->hasFlag(FLAG_ITERATOR_FN) && !fn->iteratorInfo) {
      protoIteratorClass(fn);
    }
  }
}


void
resolveFormals(FnSymbol* fn) {
  static Vec<FnSymbol*> done;

  if (!fn->hasFlag(FLAG_GENERIC)) {
    if (done.set_in(fn))
      return;
    done.set_add(fn);

    for_formals(formal, fn) {
      if (formal->type == dtUnknown) {
        if (!formal->typeExpr) {
          formal->type = dtObject;
        } else {
          resolveBlock(formal->typeExpr);
          formal->type = formal->typeExpr->body.tail->typeInfo();
          if (formal->type->symbol->hasFlag(FLAG_REF))
            formal->type = formal->type->getValueType();
        }
      }

      //
      // change type of this on record methods to reference type
      //
      if (!formal->type->symbol->hasFlag(FLAG_REF) &&
          (formal->intent == INTENT_INOUT ||
           formal->intent == INTENT_OUT ||
           (formal == fn->_this &&
            (isUnionType(formal->type) ||
             isRecordType(formal->type) || fn->hasFlag(FLAG_REF_THIS))))) {
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
  if (formalType == dtIterator && actualType->symbol->hasFlag(FLAG_ITERATOR_CLASS))
    return true;
  if (actualType == formalType)
    return true;
  if (actualType->instantiatedFrom && canInstantiate(actualType->instantiatedFrom, formalType))
    return true;
  return false;
}

// Returns true iff dispatching the actualType to the formalType
// results in a coercion.
bool
canCoerce(Type* actualType, Symbol* actualSym, Type* formalType, FnSymbol* fn, bool* require_scalar_promotion) {
  if (actualType->symbol->hasFlag(FLAG_SYNC)) {
    if (actualType->symbol->hasFlag(FLAG_GENERIC)) {
      return false;
    } else {
      Type *base_type = actualType->substitutions.v[0].value->type;
      return canDispatch(base_type, actualSym, formalType, fn, require_scalar_promotion);
    }
  }

  if (actualType->symbol->hasFlag(FLAG_REF))
    return canDispatch(actualType->getValueType(), actualSym, formalType, fn, require_scalar_promotion);

  if (is_bool_type(formalType) && is_bool_type(actualType))
    return true;

  if (is_int_type(formalType)) {
    if (toEnumType(actualType))
      return true;
    if (is_bool_type(actualType))
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
    if (is_bool_type(actualType))
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
        is_bool_type(actualType))
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
  if (is_bool_type(formalType) && is_bool_type(actualType))
    return true;
  if (is_int_type(formalType)) {
    if (is_bool_type(actualType))
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
    if (is_bool_type(actualType))
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
        is_bool_type(actualType))
      return true;
  }
  return false;
}


// Returns true iff the actualType can dispatch to the formalType.
// The function symbol is used to avoid scalar promotion on =.
// param is set if the actual is a parameter (compile-time constant).
bool
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

bool
isDispatchParent(Type* t, Type* pt) {
  if (is_bool_type(t) && is_bool_type(pt))
    return true;
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
                       Vec<Symbol*>* actuals,
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
          formalActuals->v[j] = actuals->v[i]->type;
          formalSyms->v[j] = actuals->v[i];
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
          return (fn->hasFlag(FLAG_GENERIC)) ? true : false;
        j++;
        if (!formalActuals->v[j]) {
          match = true;
          actualFormals->v[i] = formal;
          formalActuals->v[j] = actuals->v[i]->type;
          formalSyms->v[j] = actuals->v[i];
          break;
        }
      }
      if (!match && !fn->hasFlag(FLAG_GENERIC))
        return false;
    }
  }
  return true;
}


//
// returns the type that a formal type should be instantiated to when
// instantiated by a given actual type
//
static Type*
getInstantiationType(Type* actualType, Type* formalType) {
  if (canInstantiate(actualType, formalType)) {
    return actualType;
  }
  if (Type* st = actualType->scalarPromotionType) {
    if (canInstantiate(st, formalType))
      return st;
  }
  if (Type* vt = actualType->getValueType()) {
    if (canInstantiate(vt, formalType))
      return vt;
    else if (Type* st = vt->scalarPromotionType)
      if (canInstantiate(st, formalType))
        return st;
  }
  return NULL;
}


static void
computeGenericSubs(SymbolMap &subs,
                   FnSymbol* fn,
                   Vec<Type*>* formalActuals,
                   Vec<Symbol*>* formalSyms) {
  int i = 0;
  for_formals(formal, fn) {
    if (formal->intent == INTENT_PARAM) {
      if (formalSyms->v[i] && formalSyms->v[i]->isParameter()) {
        if (!formal->type->symbol->hasFlag(FLAG_GENERIC) ||
            canInstantiate(formalActuals->v[i], formal->type))
          subs.put(formal, formalSyms->v[i]);
      } else if (!formalActuals->v[i] && formal->defaultExpr) {

        // break because default expression may reference generic
        // arguments earlier in formal list; make those substitutions
        // first (test/classes/bradc/paramInClass/weirdParamInit4)
        if (subs.n)
          break;

        resolveBlock(formal->defaultExpr);
        SymExpr* se = toSymExpr(formal->defaultExpr->body.tail);
        if (se && se->var->isParameter() &&
            (!formal->type->symbol->hasFlag(FLAG_GENERIC) || canInstantiate(se->var->type, formal->type)))
          subs.put(formal, se->var);
        else
          INT_FATAL(fn, "unable to handle default parameter");
      }
    } else if (formal->type->symbol->hasFlag(FLAG_GENERIC)) {

      //
      // check for field with specified generic type
      //
      if (!formal->hasFlag(FLAG_TYPE_VARIABLE) && strcmp(formal->name, "outer") && strcmp(formal->name, "meme") && formal->type != dtAny && (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) || fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)))
        USR_FATAL(formal, "invalid generic type specification on class field");

      if (formalActuals->v[i]) {
        if (Type* type = getInstantiationType(formalActuals->v[i], formal->type))
          subs.put(formal, type->symbol);
      } else if (formal->defaultExpr) {

        // break because default expression may reference generic
        // arguments earlier in formal list; make those substitutions
        // first (test/classes/bradc/genericTypes)
        if (subs.n)
          break;

        resolveBlock(formal->defaultExpr);
        Type* defaultType = formal->defaultExpr->body.tail->typeInfo();
        if (defaultType == dtNil)
          subs.put(formal, defaultType->symbol);
        else if (Type* type = getInstantiationType(defaultType, formal->type))
          subs.put(formal, type->symbol);
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
    if (!genericArg && arg->variableExpr && !isDefExpr(arg->variableExpr->body.tail))
      resolveBlock(arg->variableExpr);

    //
    // set genericArg to true if a generic argument appears before the
    // argument with the variable expression
    //
    if (arg->type->symbol->hasFlag(FLAG_GENERIC))
      genericArg = true;

    if (!arg->variableExpr)
      continue;

    // handle unspecified variable number of arguments
    if (DefExpr* def = toDefExpr(arg->variableExpr->body.tail)) {

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

      SymbolMap map;
      FnSymbol* newFn = fn->copy(&map);
      newFn->addFlag(FLAG_INVISIBLE_FN);
      fn->defPoint->insertBefore(new DefExpr(newFn));
      Symbol* sym = map.get(def->sym);
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
    } else if (SymExpr* sym = toSymExpr(arg->variableExpr->body.tail)) {

      // handle specified number of variable arguments
      if (VarSymbol* n_var = toVarSymbol(sym->var)) {
        if (n_var->type == dtInt[INT_SIZE_32] && n_var->immediate) {
          int n = n_var->immediate->int_value();
          CallExpr* tupleCall = new CallExpr((arg->hasFlag(FLAG_TYPE_VARIABLE)) ? "_type_construct__tuple" : "_construct__tuple");
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
          if (arg->hasFlag(FLAG_TYPE_VARIABLE))
            var->addFlag(FLAG_TYPE_VARIABLE);

          if (arg->intent == INTENT_OUT || arg->intent == INTENT_INOUT) {
            int i = 1;
            for_actuals(actual, tupleCall) {
              VarSymbol* tmp = newTemp();
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
            if (arg->hasFlag(FLAG_TYPE_VARIABLE))
              var->addFlag(FLAG_TYPE_VARIABLE);
            fn->where->insertAtHead(new CallExpr(PRIMITIVE_MOVE, var, tupleCall->copy()));
            fn->where->insertAtHead(new DefExpr(var));
            SymbolMap update;
            update.put(arg, var);
            update_symbols(fn->where, &update);
          }
        }
      }
      
    } else if (!fn->hasFlag(FLAG_GENERIC))
      INT_FATAL("bad variableExpr");
  }
  return fn;
}


// Return actual-formal map if FnSymbol is viable candidate to call
static void
addCandidate(Vec<FnSymbol*>* candidateFns,
             Vec<Vec<ArgSymbol*>*>* candidateActualFormals,
             FnSymbol* fn,
             CallInfo& info) {
  fn = expandVarArgs(fn, info.actuals.n);

  if (!fn)
    return;

  Vec<ArgSymbol*>* actualFormals = new Vec<ArgSymbol*>();

  int num_actuals = info.actuals.n;
  int num_formals = fn->numFormals();

  Vec<Type*> formalActuals;
  Vec<Symbol*> formalSyms;
  bool valid = computeActualFormalMap(fn, &formalActuals, &formalSyms,
                                      actualFormals, num_actuals,
                                      num_formals, &info.actuals, &info.actualNames);
  if (!valid) {
    delete actualFormals;
    return;
  }

  if (fn->hasFlag(FLAG_GENERIC)) {

    //
    // try to avoid excessive over-instantiation
    //
    if (fn->numFormals() == formalActuals.n) {
      int i = 0;
      for_formals(formal, fn) {
        if (!formal->type->symbol->hasFlag(FLAG_GENERIC) &&
            formal->type != dtUnknown &&
            formalActuals.v[i] &&
            !canDispatch(formalActuals.v[i], formalSyms.v[i], formal->type, fn, NULL, formal->instantiatedParam)) {
          delete actualFormals;
          return;
        } else if (formal->type->symbol->hasFlag(FLAG_GENERIC) &&
                   formal->type != dtUnknown &&
                   formalActuals.v[i]) {
          Type* vt = formalActuals.v[i]->getValueType();
          Type* st = formalActuals.v[i]->scalarPromotionType;
          Type* svt = (vt) ? vt->scalarPromotionType : NULL;
          if (!canInstantiate(formalActuals.v[i], formal->type) &&
              (!vt || !canInstantiate(vt, formal->type)) &&
              (!st || !canInstantiate(st, formal->type)) &&
              (!svt || !canInstantiate(svt, formal->type))) {
            delete actualFormals;
            return;
          }
        }
        i++;
      }
    }

    SymbolMap subs;
    computeGenericSubs(subs, fn, &formalActuals, &formalSyms);
    if (subs.n) {
      FnSymbol* inst_fn = instantiate(fn, &subs, info.call);
      if (inst_fn)
        addCandidate(candidateFns, candidateActualFormals, inst_fn, info);
    }
    delete actualFormals;
    return;
  }

  if (fn->hasFlag(FLAG_GENERIC))
    INT_FATAL(fn, "unexpected generic function");

  //
  // make sure that type constructor is resolved before other constructors
  //
  if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)) {
    CallExpr* typeConstructorCall = new CallExpr(astr("_type", fn->name));
    for_formals(formal, fn) {
      if (strcmp(formal->name, "meme")) {
        if (fn->_this->type->symbol->hasFlag(FLAG_TUPLE)) {
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
    info.call->insertBefore(typeConstructorCall);
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
            formalActuals.v[j]->getValueType() != formal->type) {
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
    if (formalSyms.v[j] && formalSyms.v[j]->hasFlag(FLAG_TYPE_VARIABLE) && !formal->hasFlag(FLAG_TYPE_VARIABLE)) {
      delete actualFormals;
      return;
   }
    if (formalSyms.v[j] && !formalSyms.v[j]->hasFlag(FLAG_TYPE_VARIABLE) && formal->hasFlag(FLAG_TYPE_VARIABLE)) {
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
    if (block && block->modUses) {
      for_actuals(expr, block->modUses) {
        SymExpr* se = toSymExpr(expr);
        INT_ASSERT(se);
        ModuleSymbol* mod = toModuleSymbol(se->var);
        INT_ASSERT(mod);
        Vec<BlockStmt*> visitedCopy;
        visitedCopy.copy(visited);
        int try_md = md + 1;
        int try_dd = visibility_distance(mod->block, fn, visitedCopy, try_md, d);
        if (try_dd > 0 && (new_md == 0 || try_md < new_md)) {
          new_md = try_md;
          new_dd = try_dd;
        }
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
                      Vec<Symbol*>* actuals,
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
              canDispatch(actuals->v[k]->type, actuals->v[k], arg->type, best, &require_scalar_promotion1);
              canDispatch(actuals->v[k]->type, actuals->v[k], arg2->type, best, &require_scalar_promotion2);
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
                  if (!arg->hasFlag(FLAG_TYPE_VARIABLE) && arg2->hasFlag(FLAG_TYPE_VARIABLE))
                    as_good = false;
                  else if (arg->hasFlag(FLAG_TYPE_VARIABLE) && !arg2->hasFlag(FLAG_TYPE_VARIABLE))
                    better = true;
                  else if (arg->instantiatedFrom==dtAny &&
                      arg2->instantiatedFrom!=dtAny) {
                    better = true;
                  } else if (arg->instantiatedFrom!=dtAny &&
                             arg2->instantiatedFrom==dtAny) {
                    as_good = false;
                  } else {
                    if (actuals->v[k]->type == arg2->type &&
                        actuals->v[k]->type != arg->type) {
                      better = true;
                    } else if (actuals->v[k]->type == arg->type &&
                               actuals->v[k]->type != arg2->type) {
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


static CallExpr*
userCall(CallExpr* call) {
  if (developer)
    return call;
  if (call->getFunction()->hasFlag(FLAG_TEMP) ||
      call->getModule()->modTag == MOD_STANDARD) {
    for (int i = callStack.n-1; i >= 0; i--) {
      if (!callStack.v[i]->getFunction()->hasFlag(FLAG_TEMP) &&
          callStack.v[i]->getModule()->modTag != MOD_STANDARD)
        return callStack.v[i];
    }
  }
  return call;
}


static void
printResolutionError(const char* error,
                     Vec<FnSymbol*>& candidates,
                     CallInfo* info) {
  CallExpr* call = userCall(info->call);
  if (!strcmp("_cast", info->name)) {
    if (!info->actuals.v[0]->hasFlag(FLAG_TYPE_VARIABLE)) {
      USR_FATAL(call, "illegal cast to non-type",
                toString(info->actuals.v[1]->type),
                toString(info->actuals.v[0]->type));
    } else {
      USR_FATAL(call, "illegal cast from %s to %s",
                toString(info->actuals.v[1]->type),
                toString(info->actuals.v[0]->type));
    }
  } else if (info->actuals.n == 2 &&
             info->actuals.v[0]->type == dtMethodToken &&
             !strcmp("these", info->name)) {
    USR_FATAL(call, "cannot iterate over values of type %s",
              toString(info->actuals.v[1]->type));
  } else if (!strcmp("_type_construct__tuple", info->name)) {
    SymExpr* sym = toSymExpr(info->call->get(1));
    if (!sym || !sym->isParameter()) {
      USR_FATAL(call, "tuple size must be static");
    } else {
      USR_FATAL(call, "invalid tuple");
    }
  } else if (!strcmp("=", info->name)) {
    if (info->actuals.v[0] && !info->actuals.v[0]->hasFlag(FLAG_TYPE_VARIABLE) &&
        info->actuals.v[1] && info->actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE)) {
      USR_FATAL(call, "illegal assignment of type to value");
    } else if (info->actuals.v[1]->type == dtNil) {
      USR_FATAL(call, "type mismatch in assignment of nil to %s",
                toString(info->actuals.v[0]->type));
    } else {
      USR_FATAL(call, "type mismatch in assignment from %s to %s",
                toString(info->actuals.v[1]->type),
                toString(info->actuals.v[0]->type));
    }
  } else if (!strcmp("this", info->name)) {
    Type* type = info->actuals.v[1]->type;
    if (type->symbol->hasFlag(FLAG_REF))
      type = type->getValueType();
    if (type->symbol->hasFlag(FLAG_ITERATOR_CLASS)) {
      USR_FATAL(call, "illegal access of iterator or promoted expression");
    } else {
      USR_FATAL(call, "%s access of '%s' by '%s'", error,
                toString(info->actuals.v[1]->type),
                toString(info));
    }
  } else {
    const char* entity = "call";
    if (!strncmp("_type_construct_", info->name, 16))
      entity = "type specifier";
    const char* str = toString(info);
    if (info->scope) {
      ModuleSymbol* mod = toModuleSymbol(info->scope->parentSymbol);
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
static Vec<BlockStmt*> standardModuleSet;

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
  for (int i = nVisibleFunctions; i < gFnSymbols.n; i++) {
    FnSymbol* fn = gFnSymbols.v[i];
    if (!fn->hasFlag(FLAG_INVISIBLE_FN) && fn->defPoint->parentSymbol && !isArgSymbol(fn->defPoint->parentSymbol)) {
      BlockStmt* block = NULL;
      if (fn->hasFlag(FLAG_AUTO_II)) {
        block = theProgram->block;
      } else {
        block = getVisibilityBlock(fn->defPoint);
        //
        // add all functions in standard modules to theProgram
        //
        if (standardModuleSet.set_in(block))
          block = theProgram->block;
      }
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
  nVisibleFunctions = gFnSymbols.n;
}

static BlockStmt*
getVisibleFunctions(BlockStmt* block,
                    const char* name,
                    Vec<FnSymbol*>& visibleFns,
                    Vec<BlockStmt*>& visited) {
  //
  // all functions in standard modules are stored in a single block
  //
  if (standardModuleSet.set_in(block))
    block = theProgram->block;

  //
  // avoid infinite recursion due to modules with mutual uses
  //
  if (visited.set_in(block))
    return NULL;
  else if (isModuleSymbol(block->parentSymbol))
    visited.set_add(block);

  bool canSkipThisBlock = true;

  VisibleFunctionBlock* vfb = visibleFunctionMap.get(block);
  if (vfb) {
    canSkipThisBlock = false; // cannot skip if this block defines functions
    Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name);
    if (fns) {
      visibleFns.append(*fns);
    }
  }

  if (block->modUses) {
    for_actuals(expr, block->modUses) {
      SymExpr* se = toSymExpr(expr);
      INT_ASSERT(se);
      ModuleSymbol* mod = toModuleSymbol(se->var);
      INT_ASSERT(mod);
      canSkipThisBlock = false; // cannot skip if this block uses modules
      getVisibleFunctions(mod->block, name, visibleFns, visited);
    }
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
    if (sym->var->hasFlag(FLAG_TYPE_VARIABLE))
      return true;
    if (isTypeSymbol(sym->var))
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
  SET_LINENO(call);
  for_actuals(actual, call) {
    if (NamedExpr* ne = toNamedExpr(actual))
      actual = ne->actual;
    if (SymExpr* te = toSymExpr(actual)) {
      if (TypeSymbol* ts = toTypeSymbol(te->var)) {
        if (ClassType* ct = toClassType(ts->type)) {
          if (ct->symbol->hasFlag(FLAG_GENERIC)) {
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


void
resolveCall(CallExpr* call, bool errorCheck) {
  if (!call->primitive) {

    resolveDefaultGenericType(call);

    CallInfo info(call);

    Vec<FnSymbol*> visibleFns;                    // visible functions
    Vec<FnSymbol*> candidateFns;
    Vec<Vec<ArgSymbol*>*> candidateActualFormals; // candidate functions

    //
    // update visible function map as necessary
    //
    if (gFnSymbols.n != nVisibleFunctions)
      buildVisibleFunctionMap();

    if (!call->isResolved()) {
      if (!info.scope) {
        Vec<BlockStmt*> visited;
        getVisibleFunctions(getVisibilityBlock(call), info.name, visibleFns, visited);
      } else {
        if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(info.scope))
          if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(info.name))
            visibleFns.append(*fns);
      }
    } else {
      visibleFns.add(call->isResolved());
    }

    if (explainCallLine && explainCallMatch(call)) {
      USR_PRINT(call, "call: %s", toString(&info));
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

      if (call->methodTag && !visibleFn->hasFlag(FLAG_NO_PARENS) && !visibleFn->hasFlag(FLAG_TYPE_CONSTRUCTOR))
        continue;
      addCandidate(&candidateFns, &candidateActualFormals, visibleFn, info);
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
                                 &info.actuals, &actualFormals);

    // use visibility and then look for best again
    if (!best && candidateFns.n > 1) {
      if (info.scope) {
        disambiguate_by_scope(info.scope, &candidateFns);
      } else {
        disambiguate_by_scope(getVisibilityBlock(call), &candidateFns);
      }
      best = disambiguate_by_match(&candidateFns, &candidateActualFormals,
                                   &info.actuals, &actualFormals);
    }

    if (best && explainCallLine && explainCallMatch(call)) {
      USR_PRINT(best, "best candidate is: %s", toString(best));
    }
    if (!best && candidateFns.n > 0) {
      if (errorCheck)
        printResolutionError("ambiguous", candidateFns, &info);
      best = NULL;
    } else if (call->partialTag && (!best || !best->hasFlag(FLAG_NO_PARENS))) {
      best = NULL;
    } else if (!best) {
      if (errorCheck)
        printResolutionError("unresolved", visibleFns, &info);
      best = NULL;
    } else {
      best = defaultWrap(best, actualFormals, call->square);
      best = orderWrap(best, actualFormals, call->square);
      best = coercionWrap(best, &info);
      best = promotionWrap(best, &info);
    }
    for (int i = 0; i < candidateActualFormals.n; i++)
      delete candidateActualFormals.v[i];
    FnSymbol* resolvedFn = best;

    if (!resolvedFn && !errorCheck) {
      return;
    }

    if (call->partialTag) {
      if (!resolvedFn) {
        return;
      }
      call->partialTag = false;
    }
    if (resolvedFn && resolvedFn->hasFlag(FLAG_DATA_SET_ERROR)) {
      Type* elt_type = resolvedFn->getFormal(1)->type->substitutions.v[0].value->type;
      if (!elt_type)
        INT_FATAL(call, "Unexpected substitution of ddata class");
      USR_FATAL(userCall(call), "type mismatch in assignment from %s to %s",
                toString(info.actuals.v[3]->type), toString(elt_type));
    }
    if (resolvedFn &&
        !strcmp("=", resolvedFn->name) &&
        isRecordType(resolvedFn->getFormal(1)->type) &&
        resolvedFn->getFormal(2)->type == dtNil)
      USR_FATAL(userCall(call), "type mismatch in assignment from nil to %s",
                toString(resolvedFn->getFormal(1)->type));
    if (!resolvedFn) {
      INT_FATAL(call, "unable to resolve call");
    }
    if (call->parentSymbol) {
      call->baseExpr->replace(new SymExpr(resolvedFn));
    }

    for_formals_actuals(formal, actual, call) {
      if (formal->intent == INTENT_OUT || formal->intent == INTENT_INOUT) {
        if (SymExpr* se = toSymExpr(actual)) {
          if (se->var->hasFlag(FLAG_EXPR_TEMP) || se->var->isConstant() || se->var->isParameter()) {
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
    int size = 0;
    for (int i = 0; i < sym->var->type->substitutions.n; i++) {
      if (sym->var->type->substitutions.v[i].key) {
        if (!strcmp("size", sym->var->type->substitutions.v[i].key->name)) {
          size = toVarSymbol(sym->var->type->substitutions.v[i].value)->immediate->int_value();
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
      VarSymbol* tmp1 = newTemp();
      tmp1->addFlag(FLAG_MAYBE_PARAM);
      tmp1->addFlag(FLAG_MAYBE_TYPE);
      VarSymbol* tmp2 = newTemp();
      tmp2->addFlag(FLAG_MAYBE_PARAM);
      tmp2->addFlag(FLAG_MAYBE_TYPE);
      VarSymbol* tmp3 = newTemp();
      tmp3->addFlag(FLAG_MAYBE_PARAM);
      tmp3->addFlag(FLAG_MAYBE_TYPE);
      VarSymbol* tmp4 = newTemp();
      tmp4->addFlag(FLAG_MAYBE_PARAM);
      tmp4->addFlag(FLAG_MAYBE_TYPE);
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
    if (isReferenceType(type))
      type = type->getValueType();
    int size = 0;
    for (int i = 0; i < type->substitutions.n; i++) {
      if (type->substitutions.v[i].key) {
        if (!strcmp("size", type->substitutions.v[i].key->name)) {
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
      VarSymbol* tmp = newTemp();
      tmp->addFlag(FLAG_MAYBE_TYPE);
      DefExpr* def = new DefExpr(tmp);
      call->getStmtExpr()->insertBefore(def);
      CallExpr* e = NULL;
      if (!call->parentSymbol->hasFlag(FLAG_EXPAND_TUPLES_WITH_VALUES)) {
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

    if (lhs->hasFlag(FLAG_TYPE_VARIABLE) && !isTypeExpr(rhs)) {
      if (!fn || fn->_this == lhs) { // ignore type constructor
        if (lhs == fn->getReturnSymbol()) {
          USR_FATAL(call, "illegal return of value where type is expected");
        } else {
          USR_FATAL(call, "illegal assignment of value to type");
        }
      }
    }

    if (!lhs->hasFlag(FLAG_TYPE_VARIABLE) && !lhs->hasFlag(FLAG_MAYBE_TYPE) && isTypeExpr(rhs)) {
      if (lhs == fn->getReturnSymbol()) {
        USR_FATAL(call, "illegal return of type where value is expected");
      } else {
        USR_FATAL(call, "illegal assignment of type to value");
      }
    }

    // do not resolve function return type yet
    // except for constructors
    if (fn && fn->getReturnSymbol() == lhs && fn->_this != lhs)
      if (fn->retType == dtUnknown) {
        return;
      }

    Type* rhsType = rhs->typeInfo();

    if (rhsType == dtVoid)
      USR_FATAL(userCall(call), "illegal use of function that does not return a value");

    if (lhs->type == dtUnknown || lhs->type == dtNil)
      lhs->type = rhsType;

    Type* lhsType = lhs->type;

    if (isReferenceType(lhsType))
      lhs->removeFlag(FLAG_EXPR_TEMP);
    if (lhsType->symbol->hasFlag(FLAG_REF_ITERATOR_CLASS))
      lhs->removeFlag(FLAG_EXPR_TEMP);
    if (CallExpr* call = toCallExpr(rhs)) {
      if (FnSymbol* fn = call->isResolved()) {
        if (fn->hasFlag(FLAG_VALID_VAR))
          lhs->removeFlag(FLAG_EXPR_TEMP);
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
    if (isReferenceType(lhsBaseType))
      lhsBaseType = lhsBaseType->getValueType();
    Type* rhsBaseType = rhsType;
    if (isReferenceType(rhsBaseType))
      rhsBaseType = rhsBaseType->getValueType();
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
      formal->hasFlag(FLAG_IS_MEME) ||
      (formal == toFnSymbol(formal->defPoint->parentSymbol)->_outer) ||
      formal->hasFlag(FLAG_TYPE_VARIABLE) ||
      formal->instantiatedParam ||
      formal->type == dtMethodToken ||
      formal->type == dtLeaderToken ||
      (formal->type->symbol->hasFlag(FLAG_REF) &&
       formal->intent == INTENT_BLANK) ||
      formal->hasFlag(FLAG_NO_FORMAL_TMP))
    return false;
  return true;
}

static void
insertFormalTemps(FnSymbol* fn) {
  if (!strcmp(fn->name, "_init") ||
      !strcmp(fn->name, "_copy") ||
      !strcmp(fn->name, "_getIterator") ||
      !strcmp(fn->name, "_getIteratorHelp") ||
      !strcmp(fn->name, "iteratorIndex") ||
      !strcmp(fn->name, "iteratorIndexHelp") ||
      !strcmp(fn->name, "=") ||
      !strcmp(fn->name, "_createFieldDefault") ||
      !strcmp(fn->name, "chpldev_refToString") ||
      fn->hasFlag(FLAG_ALLOW_REF) ||
      fn->hasFlag(FLAG_REF))
    return;
  SymbolMap formals2vars;
  for_formals(formal, fn) {
    if (formalRequiresTemp(formal)) {
      VarSymbol* tmp = newTemp(astr("_formal_tmp_", formal->name));
      Type* formalType = formal->type;
      if (formalType->symbol->hasFlag(FLAG_REF))
        formalType = formalType->getValueType();
      if ((formal->intent == INTENT_BLANK ||
           formal->intent == INTENT_CONST) &&
          !formalType->symbol->hasFlag(FLAG_DOMAIN) &&
          !formalType->symbol->hasFlag(FLAG_SYNC) &&
          !formalType->symbol->hasFlag(FLAG_ARRAY))
        tmp->addFlag(FLAG_CONST);
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
          VarSymbol* refTmp = newTemp();
          VarSymbol* typeTmp = newTemp();
          typeTmp->addFlag(FLAG_MAYBE_TYPE);
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_INIT, typeTmp)));
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, typeTmp, new CallExpr(PRIMITIVE_TYPEOF, refTmp)));
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, refTmp, new CallExpr(PRIMITIVE_GET_REF, formal)));
          fn->insertAtHead(new DefExpr(refTmp));
          fn->insertAtHead(new DefExpr(typeTmp));
        }
      } else if (formal->intent == INTENT_INOUT || formal->intent == INTENT_IN)
        fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr("_copy", formal)));
      else {
        TypeSymbol* ts = formal->type->symbol;
        if (!ts->hasFlag(FLAG_DOMAIN) &&
            !ts->hasFlag(FLAG_ARRAY) &&
            !ts->hasFlag(FLAG_ITERATOR_CLASS) &&
            !ts->hasFlag(FLAG_REF) &&
            !ts->hasFlag(FLAG_SYNC))
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr("_copy", formal)));
        else
          fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, formal));
      }
      fn->insertAtHead(new DefExpr(tmp));
      if (formal->intent == INTENT_INOUT || formal->intent == INTENT_OUT) {
        fn->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_MOVE, formal, new CallExpr("=", formal, tmp)));
      }
    }
  }
}

static void fold_param_for(CallExpr* loop) {
  BlockStmt* block = toBlockStmt(loop->parentExpr);
  SymExpr* lse = toSymExpr(loop->get(2));
  SymExpr* hse = toSymExpr(loop->get(3));
  SymExpr* sse = toSymExpr(loop->get(4));
  if (!block || !lse || !hse || !sse)
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
  if (block->blockTag != BLOCK_NORMAL)
    INT_FATAL("ha");
  loop->remove();
  CallExpr* noop = new CallExpr(PRIMITIVE_NOOP);
  block->insertAfter(noop);
  Symbol* index = toSymExpr(index_expr)->var;
  if (stride <= 0) {
    for (int i = high; i >= low; i += stride) {
      SymbolMap map;
      map.put(index, new_IntSymbol(i));
      noop->insertBefore(block->copy(&map));
    }
  } else {
    for (int i = low; i <= high; i += stride) {
      SymbolMap map;
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

        if ((is_bool_type(src) && is_bool_type(dst)) ||
            (is_int_type(src) && is_int_type(dst)) ||
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
                  if (type == dtBools[BOOL_SIZE_SYS]->symbol) {
                    switch (size) {
                    case 8: tsize = dtBools[BOOL_SIZE_8]->symbol; break;
                    case 16: tsize = dtBools[BOOL_SIZE_16]->symbol; break;
                    case 32: tsize = dtBools[BOOL_SIZE_32]->symbol; break;
                    case 64: tsize = dtBools[BOOL_SIZE_64]->symbol; break;
                    default:
                      USR_FATAL( call, "illegal size %d for bool", size);
                    }
                    result = new SymExpr(tsize);
                    call->replace(result);
                  } else if (type == dtInt[INT_SIZE_32]->symbol) {
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
        base->replace(new UnresolvedSymExpr("this"));
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
        VarSymbol* this_temp = newTemp("_this_temp");
        base->replace(new UnresolvedSymExpr("this"));
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
      if (isVarSymbol(base->var) && base->var->hasFlag(FLAG_TYPE_VARIABLE)) {
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
        if (t->symbol->hasFlag(FLAG_REF))
          t = t->getValueType();
        if (t->symbol->hasFlag(FLAG_TUPLE)) {
          if (call->numActuals() != 3)
            USR_FATAL(call, "illegal tuple indexing expression");
          Type* indexType = call->get(3)->typeInfo();
          if (indexType->symbol->hasFlag(FLAG_REF))
            indexType = indexType->getValueType();
          if (!is_int_type(indexType))
            USR_FATAL(call, "tuple indexing expression is not of integral type");
          long index;
          if (get_int(call->get(3), &index)) {
            char field[8];
            sprintf(field, "x%ld", index);
            if (index <= 0 || index >= toClassType(t)->fields.length)
              USR_FATAL(call, "tuple index out-of-bounds error (%ld)", index);
            if (toClassType(t)->getField(field)->type->symbol->hasFlag(FLAG_REF))
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
      if (!se->var->hasFlag(FLAG_TYPE_VARIABLE))
        USR_FATAL(call, "invalid type specification");
      Type* type = call->get(1)->typeInfo();
      if (type ->symbol->hasFlag(FLAG_REF))
        type = type->getValueType();
      if (type->symbol->hasFlag(FLAG_ITERATOR_CLASS)) {
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
    } else if (call->isPrimitive(PRIMITIVE_TYPEOF)) {
      Type* type = call->get(1)->typeInfo();
      if (type ->symbol->hasFlag(FLAG_REF))
        type = type->getValueType();
      if (type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
        result = new CallExpr("chpl__convertValueToRuntimeType", call->get(1)->remove());
        call->replace(result);
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
            !lt->symbol->hasFlag(FLAG_GENERIC) && !rt->symbol->hasFlag(FLAG_GENERIC)) {
          if (lt->symbol->hasFlag(FLAG_REF))
            lt = lt->getValueType();
          if (rt->symbol->hasFlag(FLAG_REF))
            rt = rt->getValueType();
          result = (lt == rt) ? new SymExpr(gTrue) : new SymExpr(gFalse);
          call->replace(result);
        }
      }
    } else if (call->isNamed("!=")) {
      if (isTypeExpr(call->get(1)) || isTypeExpr(call->get(2))) {
        Type* lt = call->get(1)->typeInfo();
        Type* rt = call->get(2)->typeInfo();
        if (lt != dtUnknown && rt != dtUnknown &&
            !lt->symbol->hasFlag(FLAG_GENERIC) && !rt->symbol->hasFlag(FLAG_GENERIC)) {
          if (lt->symbol->hasFlag(FLAG_REF))
            lt = lt->getValueType();
          if (rt->symbol->hasFlag(FLAG_REF))
            rt = rt->getValueType();
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
    } else if (call->isPrimitive(PRIMITIVE_BLOCK_PARAM_LOOP)) {
      fold_param_for(call);
//     } else if (call->isPrimitive(PRIMITIVE_BLOCK_FOR_LOOP) &&
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
          var->addFlag(FLAG_MAYBE_PARAM);
          result = call->get(2)->remove();
          call->replace(result);
        }
      }
      if (!removed) {
        result = call->get(2)->remove();
        call->replace(result);
      }
    } else if (call->isPrimitive(PRIMITIVE_SET_REF)) {
      // remove set ref if already a reference (or an iterator of references)
      if ((call->get(1)->typeInfo()->symbol->hasFlag(FLAG_ITERATOR_CLASS) &&
           call->get(1)->typeInfo()->defaultConstructor->iteratorInfo->getValue->retType->symbol->hasFlag(FLAG_REF)) ||
          call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
        result = call->get(1)->remove();
        call->replace(result);
      } else {
        FnSymbol* fn = call->getFunction();
        if (!fn->hasFlag(FLAG_WRAPPER) && !fn->hasFlag(FLAG_VALID_VAR)) {
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
            if (rhs->var->hasFlag(FLAG_EXPR_TEMP) || rhs->var->isConstant() || rhs->var->isParameter())
              USR_FATAL(call, "illegal lvalue in assignment");
          }
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_GET_REF)) {
      // remove get ref if already a value
      if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
        result = call->get(1)->remove();
        call->replace(result);
      }
    } else if (call->isPrimitive(PRIMITIVE_GET_LOCALEID)) {
      Type* type = call->get(1)->typeInfo();

      if (type->symbol->hasFlag(FLAG_REF))
        type = type->getValueType();

      //
      // ensure .locale (and on) are applied to lvalues or classes
      // (locale type is a class)
      //
      SymExpr* se = toSymExpr(call->get(1));
      ClassType* ct = toClassType(type);
      if (se->var->hasFlag(FLAG_EXPR_TEMP) && (!ct || ct->classTag != CLASS_CLASS))
        USR_WARN(se, "accessing the locale of a local expression");

      //
      // if .locale is applied to an expression of array or domain
      // wrapper type, apply .locale to the _value field of the
      // wrapper
      //
      if (type->symbol->hasFlag(FLAG_ARRAY) ||
          type->symbol->hasFlag(FLAG_DOMAIN)) {
        VarSymbol* tmp = newTemp();
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        result = new CallExpr("_value", gMethodToken, call->get(1)->remove());
        call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, result));
        call->insertAtTail(tmp);
      }
    } else if (call->isPrimitive(PRIMITIVE_TO_LEADER)) {
      FnSymbol* iterator = call->get(1)->typeInfo()->defaultConstructor;
      CallExpr* leaderCall = new CallExpr(iterator->name);
      leaderCall->insertAtTail(new NamedExpr("tag", new SymExpr(gLeaderTag)));
      for_formals(formal, iterator) {
        leaderCall->insertAtTail(new NamedExpr(formal->name, new SymExpr(formal)));
      }
      call->replace(leaderCall);
      result = leaderCall;
    } else if (call->isPrimitive(PRIMITIVE_TO_FOLLOWER)) {
      FnSymbol* iterator = call->get(1)->typeInfo()->defaultConstructor;
      CallExpr* followerCall = new CallExpr(iterator->name);
      followerCall->insertAtTail(new NamedExpr("tag", new SymExpr(gFollowerTag)));
      followerCall->insertAtTail(new NamedExpr("follower", call->get(2)->remove()));
      for_formals(formal, iterator) {
        followerCall->insertAtTail(new NamedExpr(formal->name, new SymExpr(formal)));
      }
      if (call->numActuals() > 1) {
        CallExpr* alignedFollowerCall = followerCall->copy();
        alignedFollowerCall->insertAtTail(new NamedExpr("aligned", call->get(2)->remove()));
        call->replace(followerCall);
        CallExpr* move = toCallExpr(followerCall->parentExpr);
        INT_ASSERT(move && move->isPrimitive(PRIMITIVE_MOVE));
        VarSymbol* tmp = newTemp();
        tmp->addFlag(FLAG_MAYBE_PARAM);
        move->insertBefore(new DefExpr(tmp));
        CallExpr* hasAlignedFollower = new CallExpr("_callSupportsAlignedFollower");
        for_formals(formal, iterator) {
          if (!strcmp(formal->name, "this"))
            hasAlignedFollower->insertAtTail(new SymExpr(formal));
        }
        move->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, hasAlignedFollower));
        result = hasAlignedFollower;
        move->insertBefore(
          new CondStmt(new SymExpr(tmp),
                       new CallExpr(PRIMITIVE_MOVE, move->get(1)->copy(),
                                    alignedFollowerCall),
                       new CallExpr(PRIMITIVE_MOVE, move->get(1)->copy(),
                                    followerCall->remove())));
        move->remove();
      } else {
        call->replace(followerCall);
        result = followerCall;
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
      VarSymbol* tmp = newTemp(se->var->type->getValueType());
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
      if (fn->retTag == RET_PARAM || fn->hasFlag(FLAG_MAYBE_PARAM)) {
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
      if (fn->hasFlag(FLAG_MAYBE_TYPE) && fn->getReturnSymbol()->hasFlag(FLAG_TYPE_VARIABLE))
        fn->retTag = RET_TYPE;
      if (fn->retTag == RET_TYPE) {
        Symbol* ret = fn->getReturnSymbol();
        if (!ret->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          result = new SymExpr(ret->type->symbol);
          expr->replace(result);
        }
      }
      if (!strcmp("=", fn->name) && !call->getFunction()->hasFlag(FLAG_WRAPPER)) {
        SymExpr* lhs = toSymExpr(call->get(1));
        if (!lhs)
          INT_FATAL(call, "unexpected case");
        if (lhs->var->hasFlag(FLAG_EXPR_TEMP) || lhs->var->isConstant() || lhs->var->isParameter())
          USR_FATAL(call, "illegal lvalue in assignment");
      }
    } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
      bool set = false;
      if (SymExpr* lhs = toSymExpr(call->get(1))) {
        if (lhs->var->hasFlag(FLAG_MAYBE_PARAM) || lhs->var->isParameter()) {
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
          if (lhs->var->hasFlag(FLAG_MAYBE_TYPE)) {
            if (SymExpr* rhs = toSymExpr(call->get(2))) {
              if (rhs->var->hasFlag(FLAG_TYPE_VARIABLE))
                lhs->var->addFlag(FLAG_TYPE_VARIABLE);
            }
            if (CallExpr* rhs = toCallExpr(call->get(2))) {
              if (FnSymbol* fn = rhs->isResolved()) {
                if (fn->retTag == RET_TYPE)
                  lhs->var->addFlag(FLAG_TYPE_VARIABLE);
              }
              if (rhs->isPrimitive(PRIMITIVE_GET_REF)) {
                if (isTypeExpr(rhs->get(1)))
                  lhs->var->addFlag(FLAG_TYPE_VARIABLE);
              }
            }
          }
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (rhs->isPrimitive(PRIMITIVE_TYPEOF)) {
              lhs->var->addFlag(FLAG_TYPE_VARIABLE);
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
      Type* baseType = call->get(1)->typeInfo();
      if (baseType->symbol->hasFlag(FLAG_REF))
        baseType = baseType->getValueType();
      const char* memberName = get_string(call->get(2));
      Symbol* sym = baseType->getField(memberName);
      if (sym->isParameter()) {
        Vec<Symbol*> keys;
        baseType->substitutions.get_keys(keys);
        forv_Vec(Symbol, key, keys) {
          if (!strcmp(sym->name, key->name)) {
            if (Symbol* value = baseType->substitutions.get(key)) {
              result = new SymExpr(value);
              call->replace(result);
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_ISSUBTYPE)) {
      if (isTypeExpr(call->get(1)) || isTypeExpr(call->get(2))) {
        Type* lt = call->get(2)->typeInfo(); // a:t cast is cast(t,a)
        Type* rt = call->get(1)->typeInfo();
        if (lt->symbol->hasFlag(FLAG_REF))
          lt = lt->getValueType();
        if (rt->symbol->hasFlag(FLAG_REF))
          rt = rt->getValueType();
        if (lt != dtUnknown && rt != dtUnknown && lt != dtAny &&
            rt != dtAny && !lt->symbol->hasFlag(FLAG_GENERIC)) {
          bool is_true = false;
          if (lt->instantiatedFrom == rt)
            is_true = true;
          if (isSubType(lt, rt))
            is_true = true;
          result = (is_true) ? new SymExpr(gTrue) : new SymExpr(gFalse);
          call->replace(result);
        }
      }
    } else if (call->isPrimitive(PRIMITIVE_CAST)) {
      Type* t= call->get(1)->typeInfo();
      if (t == dtUnknown)
        INT_FATAL(call, "Unable to resolve type");
      call->get(1)->replace(new SymExpr(t->symbol));
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
    } else if (call->isPrimitive(PRIMITIVE_IS_TUPLE)) {
      bool is_tuple = false;
      if (SymExpr* sym = toSymExpr(call->get(1))) {
        Symbol* typeSym;
        if (!sym->var->type->refType)
          typeSym = sym->var->type->getValueType()->symbol;
        else
          typeSym = sym->var->type->symbol;
        if (typeSym->hasFlag(FLAG_TUPLE)) {
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
    } else if (call->isPrimitive(PRIMITIVE_ARRAY_ALLOC) ||
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
               call->isPrimitive(PRIMITIVE_CHPL_FREE) ||
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
      if (sym->var->hasFlag(FLAG_TYPE_VARIABLE)) {
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

void
resolveBlock(Expr* body) {
  FnSymbol* fn = toFnSymbol(body->parentSymbol);
  for_exprs_postorder(expr, body) {
    SET_LINENO(expr);
    if (SymExpr* se = toSymExpr(expr))
      if (se->var)
        makeRefType(se->var->type);
    expr = preFold(expr);

    if (fn && fn->retTag == RET_PARAM) {
      if (BlockStmt* block = toBlockStmt(expr)) {
        if (block->blockInfo) {
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
          !sym->var->hasFlag(FLAG_TYPE_VARIABLE)) {

        if (ClassType* ct = toClassType(sym->typeInfo())) {
          if (!ct->symbol->hasFlag(FLAG_GENERIC) && !ct->symbol->hasFlag(FLAG_ITERATOR_CLASS)) {
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

  if (fn->hasFlag(FLAG_EXTERN))
    return;

  if (fn->hasFlag(FLAG_AUTO_II))
    return;

  //
  // build value function for var functions
  //
  if (fn->retTag == RET_VAR) {
    if (!fn->hasFlag(FLAG_ITERATOR_FN)) {
      FnSymbol* copy = fn->copy();
      copy->addFlag(FLAG_INVISIBLE_FN);
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
          if (fn->hasFlag(FLAG_ITERATOR_FN))
            USR_WARN(fn, "setter argument is not supported in iterators");
        }
      }
    }
  }

  insertFormalTemps(fn);

  resolveBlock(fn->body);

  if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
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

  //
  // insert casts as necessary
  //
  if (fn->retTag != RET_PARAM) {
    for_exprs_postorder(expr, fn->body) {
      if (CallExpr* call = toCallExpr(expr)) {
        if (call->isPrimitive(PRIMITIVE_MOVE)) {
          if (SymExpr* lhs = toSymExpr(call->get(1))) {
            Expr* rhs = call->get(2);
            Type* rhsType = rhs->typeInfo();
            if (rhsType != lhs->var->type &&
                rhsType->refType != lhs->var->type &&
                rhsType != lhs->var->type->refType) {
              rhs->remove();
              Symbol* tmp = NULL;
              if (SymExpr* se = toSymExpr(rhs)) {
                tmp = se->var;
              } else {
                tmp = newTemp(rhs->typeInfo());
                call->insertBefore(new DefExpr(tmp));
                call->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, rhs));
              }
              CallExpr* cast = new CallExpr("_cast", lhs->var->type->symbol, tmp);
              call->insertAtTail(cast);
              resolveCall(cast);
              if (cast->isResolved())
                resolveFns(cast->isResolved());
            }
          }
        }
      }
    }
  }

  //
  // mark leaders for inlining
  //
  if (fn->hasFlag(FLAG_ITERATOR_FN)) {
    for_formals(formal, fn) {
      if (formal->type == gLeaderTag->type &&
          paramMap.get(formal) == gLeaderTag) {
        fn->addFlag(FLAG_INLINE_ITERATOR);
      }
    }
  }



  if (fn->hasFlag(FLAG_ITERATOR_FN) && !fn->iteratorInfo) {
    protoIteratorClass(fn);
  }

  if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
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
          if (instantiatedFrom->hasFlag(FLAG_TUPLE)) {
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


    //
    // resolve destructor
    //
    if (ClassType* ct = toClassType(fn->retType)) {
      if (!ct->destructor &&
          !ct->symbol->hasFlag(FLAG_REF)) {
        VarSymbol* tmp = newTemp(ct);
        CallExpr* call = new CallExpr("~chpl_destroy", gMethodToken, tmp);
        fn->insertAtHead(new CallExpr(call));
        fn->insertAtHead(new DefExpr(tmp));
        resolveCall(call);
        resolveFns(call->isResolved());
        ct->destructor = call->isResolved();
        call->remove();
        tmp->defPoint->remove();
      }
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
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->type->defaultTypeConstructor)
      if (!ts->hasFlag(FLAG_GENERIC) &&
          ts->type->defaultTypeConstructor->instantiatedFrom ==
          ct->defaultTypeConstructor)
        icts.add(ts->type);
  }
}


static void
add_to_ddf(FnSymbol* pfn, ClassType* ct) {
  forv_Vec(FnSymbol, cfn, ct->methods) {
    if (cfn && !cfn->instantiatedFrom && possible_signature_match(pfn, cfn)) {
      Vec<Type*> types;
      if (ct->symbol->hasFlag(FLAG_GENERIC))
        collectInstantiatedClassTypes(types, ct);
      else
        types.add(ct);
      forv_Vec(Type, type, types) {
        SymbolMap subs;
        if (ct->symbol->hasFlag(FLAG_GENERIC))
          subs.put(cfn->getFormal(2), type->symbol);
        for (int i = 3; i <= cfn->numFormals(); i++) {
          ArgSymbol* arg = cfn->getFormal(i);
          if (arg->intent == INTENT_PARAM) {
            subs.put(arg, paramMap.get(pfn->getFormal(i)));
          } else if (arg->type->symbol->hasFlag(FLAG_GENERIC)) {
            subs.put(arg, pfn->getFormal(i)->type->symbol);
          }
        }
        FnSymbol* fn = cfn;
        if (subs.n) {
          fn = instantiate(fn, &subs, NULL);
          if (fn) {
            if (type->defaultTypeConstructor->instantiationPoint)
              fn->instantiationPoint = type->defaultTypeConstructor->instantiationPoint;
            else
              fn->instantiationPoint = toBlockStmt(type->defaultTypeConstructor->defPoint->parentExpr);
            INT_ASSERT(fn->instantiationPoint);
          }
        }
        if (fn) {
          resolveFormals(fn);
          if (signature_match(pfn, fn)) {
            resolveFns(fn);
            if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_CLASS) &&
                pfn->retType->symbol->hasFlag(FLAG_ITERATOR_CLASS)) {
              if (!isSubType(fn->retType->defaultConstructor->iteratorInfo->getValue->retType, pfn->retType->defaultConstructor->iteratorInfo->getValue->retType)) {
                USR_FATAL_CONT(pfn, "conflicting return type specified for '%s: %s'", toString(pfn), pfn->retType->defaultConstructor->iteratorInfo->getValue->retType->symbol->name);
                USR_FATAL_CONT(fn, "  overridden by '%s: %s'", toString(fn), fn->retType->defaultConstructor->iteratorInfo->getValue->retType->symbol->name);
                USR_STOP();
              } else {
                pfn->retType->dispatchChildren.add_exclusive(fn->retType);
                fn->retType->dispatchParents.add_exclusive(pfn->retType);
              }
            } else if (!isSubType(fn->retType, pfn->retType)) {
              USR_FATAL_CONT(pfn, "conflicting return type specified for '%s: %s'", toString(pfn), pfn->retType->symbol->name);
              USR_FATAL_CONT(fn, "  overridden by '%s: %s'", toString(fn), fn->retType->symbol->name);
              USR_STOP();
            }
            Vec<FnSymbol*>* fns = ddf.get(pfn);
            if (!fns) fns = new Vec<FnSymbol*>();
            fns->add(fn);
            ddf.put(pfn, fns);
          }
        }
      }
    }
  }
}


static void
add_all_children_ddf(FnSymbol* fn, ClassType* ct) {
  forv_Vec(Type, t, ct->dispatchChildren) {
    ClassType* ct = toClassType(t);
    if (ct->defaultTypeConstructor &&
        (ct->defaultTypeConstructor->hasFlag(FLAG_GENERIC) ||
         resolvedFns.set_in(ct->defaultTypeConstructor))) {
      add_to_ddf(fn, ct);
    }
    if (!ct->instantiatedFrom)
      add_all_children_ddf(fn, ct);
  }
}


static void
build_ddf() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fn->hasFlag(FLAG_WRAPPER) && resolvedFns.set_in(fn) && !fn->hasFlag(FLAG_NO_PARENS)
        && !fn->hasFlag(FLAG_DESTRUCTOR)) {
      if (fn->numFormals() > 1) {
        if (fn->getFormal(1)->type == dtMethodToken) {
          if (ClassType* pt = toClassType(fn->getFormal(2)->type)) {
            if (pt->classTag == CLASS_CLASS && !pt->symbol->hasFlag(FLAG_GENERIC)) {
              add_all_children_ddf(fn, pt);
            }
          }
        }
      }
    }
  }
}


void
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


static void
computeStandardModuleSet() {
  standardModuleSet.set_add(rootModule->block);
  standardModuleSet.set_add(theProgram->block);

  Vec<ModuleSymbol*> stack;
  stack.add(standardModule);

  while (ModuleSymbol* mod = stack.pop()) {
    if (mod->block->modUses) {
      for_actuals(expr, mod->block->modUses) {
        SymExpr* se = toSymExpr(expr);
        INT_ASSERT(se);
        ModuleSymbol* use = toModuleSymbol(se->var);
        INT_ASSERT(use);
        if (!standardModuleSet.set_in(use->block)) {
          stack.add(use);
          standardModuleSet.set_add(use->block);
        }
      }
    }
  }
}


void
resolve() {
  parseExplainFlag(fExplainCall, &explainCallLine, &explainCallModule);

  computeStandardModuleSet();

  // call _nilType nil so as to not confuse the user
  dtNil->symbol->name = gNil->name;

  bool changed = true;
  while (changed) {
    changed = false;
    forv_Vec(FnSymbol, fn, gFnSymbols) {
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
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    bool unmark = fn->hasFlag(FLAG_GENERIC);
    for_formals(formal, fn) {
      if (formal->type->hasGenericDefaults) {
        if (!formal->markedGeneric &&
            formal != fn->_this &&
            !formal->hasFlag(FLAG_IS_MEME)) {
          formal->typeExpr = new BlockStmt(new CallExpr(formal->type->defaultTypeConstructor));
          insert_help(formal->typeExpr, NULL, formal);
          formal->type = dtUnknown;
        } else {
          unmark = false;
        }
      } else if (formal->type->symbol->hasFlag(FLAG_GENERIC) || formal->intent == INTENT_PARAM) {
        unmark = false;
      }
    }
    if (unmark) {
      fn->removeFlag(FLAG_GENERIC);
      INT_ASSERT(false);
    }
  }

  resolveFns(chpl_main);

  if (fRuntime) {
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->hasFlag(FLAG_EXPORT)) {
        resolveFormals(fn);
        resolveFns(fn);
      }
    }
  }

  // need to handle enumerated types better
  forv_Vec(TypeSymbol, type, gTypeSymbols) {
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
    num_types = gTypeSymbols.n;
    Vec<Vec<FnSymbol*>*> values;
    ddf.get_values(values);
    forv_Vec(Vec<FnSymbol*>, value, values) {
      delete value;
    }
    ddf.clear();
    build_ddf();
  } while (num_types != gTypeSymbols.n);

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

  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->defPoint &&
        ts->defPoint->parentSymbol &&
        ts->hasFlag(FLAG_HAS_RUNTIME_TYPE) &&
        !ts->hasFlag(FLAG_GENERIC)) {
      VarSymbol* tmp = newTemp(ts->type);
      ts->type->defaultConstructor->insertAtTail(new DefExpr(tmp));
      CallExpr* call = new CallExpr("chpl__convertValueToRuntimeType", tmp);
      ts->type->defaultConstructor->insertAtTail(call);
      resolveCall(call);
      resolveFns(call->isResolved());
      valueToRuntimeTypeMap.put(ts->type, call->isResolved());
      call->remove();
      tmp->defPoint->remove();
    }
  }

  //
  // resolve PRIMITIVE_INITs for records
  //
  forv_Vec(CallExpr, init, inits) {
    if (init->parentSymbol) {
      Type* type = init->get(1)->typeInfo();
      if (!type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
        if (type->symbol->hasFlag(FLAG_REF))
          type = type->getValueType();
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
  }

  Vec<CallExpr*> calls;
  forv_Vec(CallExpr, call, gCallExprs) {
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
          if_fn->addFlag(FLAG_INLINE);
          VarSymbol* _ret = newTemp(key->retType);
          VarSymbol* cid = newTemp(dtBool);
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
            VarSymbol* castTemp = newTemp(fn->retType);
            trueBlock->insertAtTail(new DefExpr(castTemp));
            trueBlock->insertAtTail(new CallExpr(PRIMITIVE_MOVE, castTemp,
                                                 subcall));
            trueBlock->insertAtTail(new CallExpr(PRIMITIVE_MOVE, _ret,
                                      new CallExpr(PRIMITIVE_CAST,
                                                   key->retType->symbol,
                                                   castTemp)));
          } else {
            INT_FATAL(key, "unexpected case; return type mismatch");
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
            VarSymbol* tmp = newTemp(type);
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

  freeCache(defaultsCache);
  freeCache(genericsCache);
  freeCache(coercionsCache);
  freeCache(promotionsCache);

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
buildRuntimeTypeInfo(FnSymbol* fn) {
  ClassType* ct = new ClassType(CLASS_RECORD);
  TypeSymbol* ts = new TypeSymbol(astr("_RuntimeTypeInfo"), ct);
  for_formals(formal, fn) {
    if (!formal->instantiatedParam) {
      VarSymbol* field = new VarSymbol(formal->name, formal->type);
      ct->fields.insertAtTail(new DefExpr(field));
      if (formal->hasFlag(FLAG_TYPE_VARIABLE))
        field->addFlag(FLAG_TYPE_VARIABLE);
    }
  }
  theProgram->block->insertAtTail(new DefExpr(ts));
  ct->symbol->addFlag(FLAG_RUNTIME_TYPE_VALUE);
  return ct;
}


static void insertReturnTemps() {
  //
  // Insert return temps for functions that return values if no
  // variable captures the result. If the value is a sync var or a
  // reference to a sync var, pass it through the _statementLevelSymbol
  // function to get the semantics of reading a sync var.
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol) {
      if (FnSymbol* fn = call->isResolved()) {
        if (fn->retType != dtVoid) {
          CallExpr* parent = toCallExpr(call->parentExpr);
          if (!parent && !isDefExpr(call->parentExpr)) { // no use
            VarSymbol* tmp = newTemp(fn->retType);
            DefExpr* def = new DefExpr(tmp);
            call->insertBefore(def);
            if ((fn->retType->getValueType() && fn->retType->getValueType()->symbol->hasFlag(FLAG_SYNC)) || fn->retType->symbol->hasFlag(FLAG_SYNC)) {
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


//
// insert code to initialize a class or record
//
static void
initializeClass(Expr* stmt, Symbol* sym) {
  ClassType* ct = toClassType(sym->type);
  INT_ASSERT(ct);
  for_fields(field, ct) {
    if (!field->hasFlag(FLAG_SUPER_CLASS)) {
      if (field->type->defaultValue) {
        stmt->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, sym, field, field->type->defaultValue));
      } else if (isRecordType(field->type)) {
        VarSymbol* tmp = newTemp(field->type);
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
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      if (!resolvedFns.set_in(fn) || fn->retTag == RET_PARAM)
        fn->defPoint->remove();
    }
  }

  // Remove unused types
  forv_Vec(TypeSymbol, type, gTypeSymbols) {
    if (type->defPoint && type->defPoint->parentSymbol)
      if (!type->hasFlag(FLAG_REF))
        if (ClassType* ct = toClassType(type->type))
          if (!resolvedFns.set_in(ct->defaultConstructor) &&
              !resolvedFns.set_in(ct->defaultTypeConstructor)) {
            if (ct->symbol->hasFlag(FLAG_OBJECT_CLASS))
              dtObject = NULL;
            ct->symbol->defPoint->remove();
          }
  }
  forv_Vec(TypeSymbol, type, gTypeSymbols) {
    if (type->defPoint && type->defPoint->parentSymbol) {
      if (type->hasFlag(FLAG_REF) && type->type != dtNilRef) {
        if (ClassType* ct = toClassType(type->type->getValueType())) {
          if (!resolvedFns.set_in(ct->defaultConstructor) &&
              !resolvedFns.set_in(ct->defaultTypeConstructor)) {
            if (ct->symbol->hasFlag(FLAG_OBJECT_CLASS))
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
        if (type->symbol->hasFlag(FLAG_REF))
          type = type->getValueType();
        // Remove move(x, PRIMITIVE_TYPEOF(y)) calls -- useless after this
        CallExpr* parentCall = toCallExpr(call->parentExpr);
        if (parentCall && parentCall->isPrimitive(PRIMITIVE_MOVE) && 
            parentCall->get(2) == call) {
          parentCall->remove();
        } else {
          // Replace PRIMITIVE_TYPEOF with argument
          call->replace(call->get(1)->remove());
        }
      } else if (call->isPrimitive(PRIMITIVE_CAST)) {
        if (call->get(1)->typeInfo() == call->get(2)->typeInfo())
          call->replace(call->get(2)->remove());
      } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER) ||
                 call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
                 call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) {
        // Remove member accesses of types
        // Replace string literals with field symbols in member primitives
        Type* baseType = call->get(1)->typeInfo();
        if (!call->parentSymbol->hasFlag(FLAG_REF) &&
            baseType->symbol->hasFlag(FLAG_REF))
          baseType = baseType->getValueType();
        const char* memberName = get_string(call->get(2));
        Symbol* sym = baseType->getField(memberName);
        if ((sym->hasFlag(FLAG_TYPE_VARIABLE) && !sym->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) ||
            !strcmp(sym->name, "_promotionType") ||
            sym->isParameter())
          call->getStmtExpr()->remove();
        else
          call->get(2)->replace(new SymExpr(sym));
      } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
        // Remove types to enable --baseline
        SymExpr* sym = toSymExpr(call->get(2));
        if (sym && isTypeSymbol(sym->var))
          call->remove();
      } else if (FnSymbol* fn = call->isResolved()) {
        // Remove method and leader token actuals
        for (int i = fn->numFormals(); i >= 1; i--) {
          ArgSymbol* formal = fn->getFormal(i);
          if (formal->type == dtMethodToken ||
              formal->type == dtLeaderToken ||
              formal->instantiatedParam ||
              (formal->hasFlag(FLAG_TYPE_VARIABLE) &&
               !formal->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)))
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

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      if (fn->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
        INT_ASSERT(fn->retType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE));
        Type* runtimeType = buildRuntimeTypeInfo(fn);
        runtimeTypeMap.put(fn->retType, runtimeType);

        FnSymbol* runtimeTypeToValueFn = fn->copy();
        runtimeTypeToValueFn->name = astr("chpl__convertRuntimeTypeToValue");
        runtimeTypeToValueFn->cname = runtimeTypeToValueFn->name;
        runtimeTypeToValueFn->removeFlag(FLAG_HAS_RUNTIME_TYPE);
        runtimeTypeToValueFn->getReturnSymbol()->removeFlag(FLAG_TYPE_VARIABLE);
        runtimeTypeToValueFn->retTag = RET_VALUE;
        fn->defPoint->insertBefore(new DefExpr(runtimeTypeToValueFn));
        runtimeTypeToValueMap.put(runtimeType, runtimeTypeToValueFn);

        fn->retType = runtimeType;
        fn->getReturnSymbol()->type = runtimeType;
        BlockStmt* block = new BlockStmt();
        VarSymbol* var = newTemp(fn->retType);
        block->insertAtTail(new DefExpr(var));
        for_formals(formal, fn) {
          if (!formal->instantiatedParam) {
            Symbol* field = runtimeType->getField(formal->name);
            if (!formal->hasFlag(FLAG_TYPE_VARIABLE) || field->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE))
              block->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, var, field, formal));
          }
        }
        block->insertAtTail(new CallExpr(PRIMITIVE_RETURN, var));
        fn->body->replace(block);
      }
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      Vec<BaseAST*> asts;
      for_formals(formal, fn) {
        // Remove formal default values
        if (formal->defaultExpr)
          formal->defaultExpr->remove();
        // Remove formal type expressions
        if (formal->typeExpr)
          formal->typeExpr->remove();
        // Remove method and leader token formals
        if (formal->type == dtMethodToken ||
            formal->type == dtLeaderToken ||
            formal->instantiatedParam)
          formal->defPoint->remove();
        if (formal->hasFlag(FLAG_TYPE_VARIABLE) &&
            !formal->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          formal->defPoint->remove();
          VarSymbol* tmp = newTemp(formal->type);
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
        if (formal->hasFlag(FLAG_TYPE_VARIABLE) &&
            formal->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          FnSymbol* fn = valueToRuntimeTypeMap.get(formal->type);
          Type* rt = (fn->retType->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) ? fn->retType : runtimeTypeMap.get(fn->retType);
          INT_ASSERT(rt);
          formal->type =  rt;
          formal->removeFlag(FLAG_TYPE_VARIABLE);
        }
      }
      if (fn->where)
        fn->where->remove();
      if (fn->retTag == RET_TYPE) {
        VarSymbol* ret = toVarSymbol(fn->getReturnSymbol());
        if (ret && ret->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          FnSymbol* rtfn = valueToRuntimeTypeMap.get(ret->type);
          Type* rt = (rtfn->retType->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) ? rtfn->retType : runtimeTypeMap.get(rtfn->retType);
          INT_ASSERT(rt);
          ret->type = rt;
          fn->retType = ret->type;
          fn->retTag = RET_VALUE;
        }
      }
    }
  }

  asts.clear();
  collect_asts_postorder(rootModule, asts);

  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (isVarSymbol(def->sym) &&
          def->sym->hasFlag(FLAG_TYPE_VARIABLE) &&
          def->sym->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
        Type* rt = runtimeTypeMap.get(def->sym->type);
        INT_ASSERT(rt);
        def->sym->type = rt;
        def->sym->removeFlag(FLAG_TYPE_VARIABLE);
      }
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->parentSymbol && call->isPrimitive(PRIMITIVE_INIT)) {
        SymExpr* se = toSymExpr(call->get(1));
        Type* rt =se->var->type;
        if (rt->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
          SET_LINENO(call);
          FnSymbol* runtimeTypeToValueFn = runtimeTypeToValueMap.get(rt);
          INT_ASSERT(runtimeTypeToValueFn);
          CallExpr* runtimeTypeToValueCall = new CallExpr(runtimeTypeToValueFn);
          for_formals(formal, runtimeTypeToValueFn) {
            Symbol* field = rt->getField(formal->name);
            INT_ASSERT(field);
            VarSymbol* tmp = newTemp(field->type);
            call->getStmtExpr()->insertBefore(new DefExpr(tmp));
            call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, se->var, field)));
            if (formal->hasFlag(FLAG_TYPE_VARIABLE))
              tmp->addFlag(FLAG_TYPE_VARIABLE);
            runtimeTypeToValueCall->insertAtTail(tmp);
          }
          call->replace(runtimeTypeToValueCall);
        } else if (rt->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          //
          // This is probably related to a comment that used to handle
          // this case elsewhere:
          //
          // special handling of tuple constructor to avoid
          // initialization of array based on an array type symbol
          // rather than a runtime array type
          //
          // this code added during the introduction of the new
          // keyword; it should be removed when possible
          //
          call->getStmtExpr()->remove();
        } else {
          INT_FATAL(call, "PRIMITIVE_INIT should have already been handled");
        }
      }
    } else if (SymExpr* se = toSymExpr(ast)) {

      // remove dead type expressions
      if (se->getStmtExpr() == se)
        if (se->var->hasFlag(FLAG_TYPE_VARIABLE))
          se->remove();

    }
  }

  // Remove type fields, parameter fields, and _promotionType field
  forv_Vec(TypeSymbol, type, gTypeSymbols) {
    if (type->defPoint && type->defPoint->parentSymbol) {
      if (ClassType* ct = toClassType(type->type)) {
        for_fields(field, ct) {
          if (field->hasFlag(FLAG_TYPE_VARIABLE) ||
              field->isParameter() ||
              !strcmp(field->name, "_promotionType"))
            field->defPoint->remove();
        }
      }
    }
  }

  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol && call->isResolved()) {
      //
      // Insert reference temps for function arguments that expect them.
      //
      for_formals_actuals(formal, actual, call) {
        if (formal->type == actual->typeInfo()->refType) {
          SET_LINENO(call);
          VarSymbol* tmp = newTemp(formal->type);
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


static bool
is_array_type(Type* type) {
  forv_Vec(Type, t, type->dispatchParents) {
    if (t->symbol->hasFlag(FLAG_BASE_ARRAY))
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
      !strcmp(ct->instantiatedFrom->symbol->name, "DefaultArithmeticDomain")) {
    ct->symbol->name = astr("domain", ct->symbol->name+23);
  }
  if (ct->symbol->hasFlag(FLAG_ARRAY) || ct->symbol->hasFlag(FLAG_DOMAIN)) {
    ct->symbol->name = ct->getField("_valueField")->type->symbol->name;
  }
}


static void
setScalarPromotionType(ClassType* ct) {
  for_fields(field, ct) {
    if (!strcmp(field->name, "_promotionType"))
      ct->scalarPromotionType = field->type;
  }
}
