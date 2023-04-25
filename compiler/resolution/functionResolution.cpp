/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include "resolution.h"

#include "AstCount.h"
#include "astutil.h"
#include "build.h"
#include "caches.h"
#include "callInfo.h"
#include "CatchStmt.h"
#include "CForLoop.h"
#include "fcf-support.h"
#include "DecoratedClassType.h"
#include "DeferStmt.h"
#include "driver.h"
#include "fixupExports.h"
#include "forallOptimizations.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "ImportStmt.h"
#include "initializerResolution.h"
#include "initializerRules.h"
#include "iterator.h"
#include "lifetime.h"
#include "ModuleSymbol.h"
#include "optimizations.h"
#include "ParamForLoop.h"
#include "PartialCopyData.h"
#include "passes.h"
#include "postFold.h"
#include "preFold.h"
#include "ResolutionCandidate.h"
#include "resolveFunction.h"
#include "resolveIntents.h"
#include "scopeResolve.h"
#include "splitInit.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "symbol.h"
#include "TryStmt.h"
#include "typeSpecifier.h"
#include "view.h"
#include "virtualDispatch.h"
#include "visibleFunctions.h"
#include "wellknown.h"
#include "WhileStmt.h"
#include "wrappers.h"

#include "global-ast-vecs.h"

#include "../../frontend/lib/immediates/prim_data.h"

#include <algorithm>
#include <cmath>
#include <inttypes.h>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class DisambiguationState {
public:
        DisambiguationState();

  bool  fn1NonParamArgsPreferred;
  bool  fn2NonParamArgsPreferred;

  bool  fn1ParamArgsPreferred;
  bool  fn2ParamArgsPreferred;
};

// map: (block id) -> (map: sym -> sym)
typedef std::map<int, SymbolMap*> CapturedValueMap;

//#
//# Global Variables
//#

bool                               resolved                  = false;
int                                explainCallLine           = 0;

SymbolMap                          paramMap;

Vec<CallExpr*>                     callStack;

std::map<Type*,     FnSymbol*>     autoCopyMap;
std::map<Type*,     FnSymbol*>     initCopyMap;
std::map<Type*,     Serializers>   serializeMap;

Map<Type*,          FnSymbol*>     autoDestroyMap;
Map<FnSymbol*,      FnSymbol*>     coerceMoveFromCopyMap;
Map<Type*,          FnSymbol*>     valueToRuntimeTypeMap;
Map<FnSymbol*,      FnSymbol*>     iteratorLeaderMap;
Map<FnSymbol*,      FnSymbol*>     iteratorFollowerMap;

//#
//# Static Variables
//#
static ModuleSymbol*               explainCallModule;

static Map<Type*,     Type*>       runtimeTypeMap;

static std::map<FnSymbol*, const char*> innerCompilerWarningMap;
static std::map<FnSymbol*, const char*> outerCompilerWarningMap;

static std::map<FnSymbol*, const char*> innerCompilerErrorMap;
static std::map<FnSymbol*, const char*> outerCompilerErrorMap;

static CapturedValueMap            capturedValues;

// Enable coercions from nilable -> non-nilable to have easier errors
static int generousResolutionForErrors;

// Hide errors when trying to resolve something.

// aka error strategy if function not found or contained a compilerError
typedef enum {
  CHECK_NORMAL_CALL,
  CHECK_CALLABLE_ONLY, // check that the function is callable (but not body)
  CHECK_BODY_RESOLVES, // check that the body resolves (but not called fns)
  CHECK_FAILED         // indicates error encountered here
} check_state_t;

static int inTryResolve;
static std::vector<check_state_t> tryResolveStates;
static std::vector<FnSymbol*> tryResolveFunctions;
typedef std::map<FnSymbol*,std::pair<BaseAST*,const char*> > try_resolve_map_t;
try_resolve_map_t tryResolveErrors;

//#
//# Static Function Declarations
//#
static FnSymbol* resolveUninsertedCall(Type* type, CallExpr* call,
                                       bool errorOnFailure);
static bool hasUserAssign(Type* type);
static void resolveOther();
static bool fits_in_int(int width, Immediate* imm);
static bool is_negative(Immediate* imm);
static bool fits_in_uint(int width, Immediate* imm);
static bool fits_in_mantissa_exponent(int mantissa_width, int exponent_width, Immediate* imm, bool realPart=true);
static bool canParamCoerce(Type* actualType, Symbol* actualSym, Type* formalType, bool* paramNarrows);
static bool
moreSpecificCanDispatch(FnSymbol* fn, Type* actualType, Type* formalType);
static BlockStmt* getParentBlock(Expr* expr);

static void resolveTupleExpand(CallExpr* call);
static void resolveSetMember(CallExpr* call);
static void resolveInitField(CallExpr* call);
static void resolveMove(CallExpr* call);
static void resolveNew(CallExpr* call);
static void resolveForResolutionPoint(CallExpr* call);
static void resolveCoerce(CallExpr* call);
static void resolveAutoCopyEtc(AggregateType* at);
static FnSymbol* autoMemoryFunction(AggregateType* at, const char* fnName);

static Expr* foldTryCond(Expr* expr);

static void unmarkDefaultedGenerics();
static void resolveUsesAndModule(ModuleSymbol* mod, const char* path);
static void resolveSupportForModuleDeinits();
static void resolveExports();
static void resolveEnumTypes();
static void populateRuntimeTypeMap();
static void resolveAutoCopies();
static void resolveSerializers();
static void resolveDestructors();
static AggregateType* buildRuntimeTypeInfo(FnSymbol* fn);
static void insertReturnTemps();
static void initializeClass(Expr* stmt, Symbol* sym);
static void handleRuntimeTypes();
static void buildRuntimeTypeInitFns();
static void buildRuntimeTypeInitFn(FnSymbol* fn, Type* runtimeType);
static void replaceTypeFormalsWithRuntimeTypes();
static void replaceReturnedTypesWithRuntimeTypes();
static void replaceRuntimeTypeVariableTypes();
static FnSymbol* findGenMainFn();
static void printCallGraph(FnSymbol* startPoint = NULL,
                           int indent = 0,
                           std::set<FnSymbol*>* alreadyCalled = NULL);
static void printUnusedFunctions();

static void handleTaskIntentArgs(CallInfo& info, FnSymbol* taskFn);
static void lvalueCheckActual(CallExpr* call, Expr* actual, IntentTag intent, ArgSymbol* formal);

static bool  obviousMismatch(CallExpr* call, FnSymbol* fn);
static void  moveHaltMoveIsUnacceptable(CallExpr* call);
static  Expr* chaseTypeConstructorForActual(CallExpr* init,
                                            const char* subName,
                                            int subIdx,
                                            AggregateType* at);
static CallExpr* createGenericRecordVarDefaultInitCall(Symbol* val, AggregateType* at, Expr* call);

static void computeConversionInfo(ResolutionCandidate* candidate,
                                  const DisambiguationContext& DC);

static bool useLegacyNilability(Expr* at) {
  if (at != NULL) {
    FnSymbol* fn = at->getFunction();
    ModuleSymbol* mod = at->getModule();

    if (fn && fn->hasFlag(FLAG_UNSAFE))
      return true;

    if (mod && mod->hasFlag(FLAG_UNSAFE))
      return true;
  }

  return false;
}
static bool useLegacyNilability(Symbol* at) {
  if (at)
    return useLegacyNilability(at->defPoint);

  return false;
}


//
// Invoke resolveFunction(fn) with 'call' on top of 'callStack'.
//
void resolveFnForCall(FnSymbol* fn, CallExpr* call) {
  // If 'call' is already on the call stack, do not add it.
  // If this assertion fails, change it to 'if'.
  INT_ASSERT(callStack.n == 0 || call != callStack.v[callStack.n-1]);

  // When 'call' is on 'callStack', its parentSymbol etc. may be queried
  // in printCallStack(), which resolveFunction() may invoke.
  INT_ASSERT(call->inTree());

  // Push 'call' onto the stack. In case of an error or warning,
  // this allows the user to see how they got there.
  callStack.add(call);

  // do real work
  resolveFunction(fn, call);

  callStack.pop();
}

//
// Resolve 'call', then resolve its target function if applicable.
// 'call' must be resolved successfully, except when allowUnresolved==true.
//
void resolveCallAndCallee(CallExpr* call, bool allowUnresolved) {
  resolveCall(call);

  if (FnSymbol* callee = call->resolvedFunction()) {
    resolveFnForCall(callee, call);
  } else if (call->isPrimitive() &&
             !call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
    // OK, don't worry about resolving called functions for primitives
    // (This comes up in particular for PRIM_CAST in the event
    //  resolveBuiltinCastCall lowered a _cast to the primitive).
  } else if (!allowUnresolved) {
    INT_ASSERT(false);
  }
}

//
// Resolve 'fn' and return it upon success.
// If there are errors, DO NOT report then, just return NULL.
//
FnSymbol* tryResolveFunction(FnSymbol* fn) {
  inTryResolve++;
  tryResolveStates.push_back(CHECK_BODY_RESOLVES);
  tryResolveFunctions.push_back(fn);

  resolveFunction(fn);

  check_state_t state = tryResolveStates.back();

  tryResolveFunctions.pop_back();
  tryResolveStates.pop_back();
  inTryResolve--;

  if (state == CHECK_FAILED)
    return nullptr;
  else
    return fn;
}


// Fills in the refType field of a type
// with the type's corresponding reference type.
void makeRefType(Type* type) {
  if (!type)
    // Should this be an assert?
    return;

  if (type->refType) {
    // Already done.
    return;
  }

  if (type == dtMethodToken ||
      type == dtUnknown ||
      type == dtSplitInitType ||
      type->symbol->hasFlag(FLAG_REF) ||
      type->symbol->hasFlag(FLAG_GENERIC)) {

    return;
  }

  CallExpr* call = new CallExpr(dtRef->symbol, type->symbol);
  resolveUninsertedCall(type, call, false);
  type->refType  = toAggregateType(call->typeInfo());

  type->refType->getField(1)->type = type;

  if (type->symbol->hasFlag(FLAG_ATOMIC_TYPE))
    type->refType->symbol->addFlag(FLAG_ATOMIC_TYPE);
}

static bool
hasUserAssign(Type* type) {

  // Workaround for problems with resolution finding =
  // for tuples types causing compile failures.
  // See
  //  test/modules/sungeun/no-use-enum.chpl

  // In the future, hasUserAssign should just return
  // false if the = call does not resolve
  // (instead of causing a compile error).
  if( type->symbol->hasFlag(FLAG_TUPLE) ) return false;


  SET_LINENO(type->symbol);
  Symbol* tmp = newTemp(type);
  chpl_gen_main->insertAtHead(new DefExpr(tmp));
  CallExpr* call = new CallExpr("=", tmp, tmp);

  // Permit not finding an assignment function at all. This may happen if
  // the type was instantiated in a scope where its module wasn't used. E.g.:
  //   var x = new A.R(int);
  //
  FnSymbol* fn = resolveUninsertedCall(type, call, false);
  // Don't think we need to resolve the whole function
  // since we're just looking for a flag.
  //resolveFunction(fn);
  tmp->defPoint->remove();

  if (fn == NULL) {
    return false;
  } else {
    return !fn->hasFlag(FLAG_COMPILER_GENERATED);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool hasAutoCopyForType(Type* type) {
  std::map<Type*, FnSymbol*>::iterator it = autoCopyMap.find(type);

  return it != autoCopyMap.end() && it->second != NULL;
}

// This function is intended to protect gets from the autoCopyMap so that
// we can insert NULL values for a type and avoid segfaults
FnSymbol* getAutoCopyForType(Type* type) {
  std::map<Type*, FnSymbol*>::iterator it = autoCopyMap.find(type);

  if (it == autoCopyMap.end() || it->second == NULL) {
    INT_FATAL(type,
              "Trying to obtain autoCopy for type '%s', which defines none",
              type->symbol->name);
  }

  return it->second;
}

FnSymbol* getAutoCopy(Type* type) {
  std::map<Type*, FnSymbol*>::iterator it = autoCopyMap.find(type);

  if (it == autoCopyMap.end())
    return NULL;
  else
    return it->second;  // can also be NULL
}

void getAutoCopyTypeKeys(Vec<Type*>& keys) {
  std::map<Type*, FnSymbol*>::iterator it;

  for (it = autoCopyMap.begin(); it != autoCopyMap.end(); ++it) {
    keys.add(it->first);
  }
}

FnSymbol* getAutoDestroy(Type* t) {
  return autoDestroyMap.get(t);
}

Type* getCopyTypeDuringResolution(Type* t) {
  if (isSyncType(t) || isSingleType(t)) {
    Type* baseType = t->getField("valType")->type;
    return baseType;
  }
  if (isAliasingArrayType(t) || // avoid inf. loop in resolving array functions
      t->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    AggregateType* at = toAggregateType(t);
    INT_ASSERT(at);

    FnSymbol* fn = getInitCopyDuringResolution(at);
    INT_ASSERT(fn);

    if (fn->retType == t)
      INT_FATAL("Expected different return type for this initCopy");

    return fn->retType;
  }

  return t;
}

static Type* canCoerceToCopyType(Type* actualType, Symbol* actualSym,
                                 Type* formalType, ArgSymbol* formalSym,
                                 FnSymbol* fn) {

  Type* copyType = NULL;

  Type* actualValType = actualType->getValType();
  Type* formalValType = formalType->getValType();

  if (isSyncType(actualValType) || isSingleType(actualValType)) {
    copyType = getCopyTypeDuringResolution(actualValType);
  } else if (isAliasingArrayType(actualValType) ||
             actualValType->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    // The conditions below avoid infinite loops and problems
    // relating to resolving initCopy for iterators when not needed.
    if (formalValType == dtAny ||
        formalValType->symbol->hasFlag(FLAG_ARRAY)) {
      if (fn == NULL ||
          !(fn->name == astr_initCopy || fn->name == astr_autoCopy ||
            fn->name == astr_coerceMove || fn->name == astr_coerceCopy)) {

        if (formalSym == NULL || inOrOutFormalNeedingCopyType(formalSym)) {
          copyType = getCopyTypeDuringResolution(actualValType);
        }
      }
    }
  }

  if (copyType == dtUnknown) copyType = NULL;
  return copyType;
}

FnSymbol* getInitCopyDuringResolution(Type* type) {
  std::map<Type*, FnSymbol*>::iterator it = initCopyMap.find(type);

  FnSymbol* fn = NULL;
  if (it != initCopyMap.end())
    fn = it->second;  // can also be NULL

  if (fn != NULL)
    return fn;

  if (AggregateType* at = toAggregateType(type)) {
    fn = autoMemoryFunction(at, astr_initCopy);
    initCopyMap[at] = fn;
  }

  return fn;
}


FnSymbol* getCoerceMoveFromCoerceCopy(FnSymbol* coerceCopyFn) {
  return coerceMoveFromCopyMap.get(coerceCopyFn);
}

const char* getErroneousCopyError(FnSymbol* fn) {
  try_resolve_map_t::iterator it;
  it = tryResolveErrors.find(fn);
  if (it != tryResolveErrors.end()) {
    const char* err = it->second.second;
    return err;
  }

  return NULL;
}

void markCopyErroneous(FnSymbol* fn, const char* err) {
  fn->addFlag(FLAG_ERRONEOUS_COPY);
  if (err != NULL)
    tryResolveErrors[fn] = std::make_pair(fn,err);
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool isTupleContainingOnlyReferences(Type* t)
{
  if(t->symbol->hasFlag(FLAG_TUPLE)) {
    bool allRef = true;
    AggregateType* at = toAggregateType(t);
    int i = 0;
    for_fields(field, at) {
      if (i != 0) { // skip size field
        if (!isReferenceType(field->type))
          allRef = false;
      }
      i++;
    }
    return allRef;
  }

  return false;
}

bool isTupleContainingAnyReferences(Type* t)
{
  if(t->symbol->hasFlag(FLAG_TUPLE)) {
    bool anyRef = false;
    AggregateType* at = toAggregateType(t);
    int i = 0;
    for_fields(field, at) {
      if (i != 0) { // skip size field
        if (isReferenceType(field->type))
          anyRef = true;
        if (field->type->symbol->hasFlag(FLAG_TUPLE) &&
            isTupleContainingAnyReferences(field->type))
          anyRef = true;

      }
      i++;
    }
    return anyRef;
  }

  return false;
}

static bool fits_in_int_helper(int width, int64_t val) {
  switch (width) {
    default: INT_FATAL("bad width in fits_in_int_helper");
    case 1:
      return (val == 0 || val == 1);
    case 8:
      return (val >= INT8_MIN && val <= INT8_MAX);
    case 16:
      return (val >= INT16_MIN && val <= INT16_MAX);
    case 32:
      return (val >= INT32_MIN && val <= INT32_MAX);
    case 64:
      // As an int64_t will always fit within a 64 bit int.
      return true;
  }
}

static bool fits_in_int(int width, Immediate* imm) {
  // TODO: should this consider real or complex?
  if (imm->const_kind == NUM_KIND_INT) {
    int64_t i = imm->int_value();
    return fits_in_int_helper(width, i);
  } else if (imm->const_kind == NUM_KIND_UINT) {
    uint64_t u = imm->uint_value();
    if (u > INT64_MAX)
      return false;
    return fits_in_int_helper(width, (int64_t)u);
  }

  return false;
}

static bool is_negative(Immediate* imm) {
  if (imm->const_kind == NUM_KIND_INT) {
    int64_t i = imm->int_value();
    return (i < 0);
  }

  return false;
}

static bool fits_in_uint_helper(int width, uint64_t val) {
  switch (width) {
  default: INT_FATAL("bad width in fits_in_uint_helper");
  case 8:
    return (val <= UINT8_MAX);
  case 16:
    return (val <= UINT16_MAX);
  case 32:
    return (val <= UINT32_MAX);
  case 64:
    // As a uint64_t will always fit inside a 64 bit uint.
    return true;
  }
}

static bool fits_in_uint(int width, Immediate* imm) {
  // TODO: should this consider real or complex?
  if (imm->const_kind == NUM_KIND_INT) {
    int64_t i = imm->int_value();
    if (i < 0)
      return false;
    return fits_in_uint_helper(width, (uint64_t)i);
  } else if (imm->const_kind == NUM_KIND_UINT) {
    uint64_t u = imm->uint_value();
    return fits_in_uint_helper(width, u);
  }

  return false;
}

// Is this a legal actual argument where an l-value is required?
// I.e. for an out/inout/ref formal.
//
// If it returns false, actualConstOut will indicate if the actual was const.
static bool
isLegalLvalueActualArg(ArgSymbol* formal, Expr* actual,
                       bool &constnessErrorOut,
                       bool &exprTmpErrorOut) {
  Symbol* calledFn = NULL;
  if (formal)
    calledFn = formal->defPoint->parentSymbol;

  if (SymExpr* se = toSymExpr(actual)) {
    Symbol* sym = se->symbol();

    bool actualConst = false;

    if (sym->hasFlag(FLAG_REF_TO_CONST) ||
        sym->isConstant() ||
        sym->isParameter())
      actualConst = true;

    bool actualExprTmp = sym->hasFlag(FLAG_EXPR_TEMP) &&
                         !sym->type->symbol->hasFlag(FLAG_ARRAY);
    TypeSymbol* formalTS = NULL;
    bool formalCopyMutates = false;
    if (formal) {
      formalTS = formal->getValType()->symbol;
      formalCopyMutates = formalTS->hasFlag(FLAG_COPY_MUTATES);
    }
    bool isInitCoerceTmp = sym->name == astr_init_coerce_tmp;

    bool parameter = se->symbol()->isParameter();

    bool constnessError =
      (actualConst && !(formal && formal->hasFlag(FLAG_ARG_THIS))) ||
      parameter;

    bool exprTmpError =
        (actualExprTmp && !formalCopyMutates && !isInitCoerceTmp) ||
        parameter;

    if (constnessError || exprTmpError) {
      // But ignore for now errors with this argument
      // to functions marked with FLAG_REF_TO_CONST_WHEN_CONST_THIS.
      // These will be checked for later, along with ref-pairs.
      if (! (formal && formal->hasFlag(FLAG_ARG_THIS) &&
             calledFn && calledFn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS)))
      {
        constnessErrorOut = constnessError;
        exprTmpErrorOut = exprTmpError;
        return false;
      }
    }
  }

  return true;
}


// Is this a legal actual argument for a 'const ref' formal?
// At present, params cannot be passed to 'const ref'.
static bool
isLegalConstRefActualArg(ArgSymbol* formal, Expr* actual) {
  bool retval = true;

  if (SymExpr* se = toSymExpr(actual))
    if (se->symbol()->isParameter()                   ==  true &&
        isString(se->symbol())                        == false &&
        isBytes(se->symbol())                         == false)
      retval = false;

  return retval;
}

/* If we have a generic parent, e.g.:
   class Parent { type t; proc foo(){} }
   class Child : Parent { }

   when we have e.g. Child(int) and we call foo,
   we need we to instantiate Parent.foo() to Parent(int).foo().
   In that case, the actual is Child(int) and the formal is Parent.
   We need to return Parent(int), which should be the dispatch
   parent for Child(int).

   This table helps one to understand the situation

                                              (formal)
     Parent(int) ------ instantiatedFrom --->  Parent
         |                                      |
      dispatch child                         dispatch child
         |                                      |
         v                                      v
     Child(int)  ------ instantiatedFrom ---> Child
      (actual)

   This function detects that situation and returns the type that
   should be instantiated.
*/
Type* getConcreteParentForGenericFormal(Type* actualType, Type* formalType) {
  Type* retval = NULL;

  if (AggregateType* at = toAggregateType(actualType)) {
    forv_Vec(AggregateType, parent, at->dispatchParents) {
      if (isInstantiation(parent, formalType) == true) {
        retval = parent;
        break;

      } else if (Type* t = getConcreteParentForGenericFormal(parent,
                                                             formalType)) {
        retval = t;
        break;
      }
    }

    if (retval == NULL) {
      if (isClass(formalType)) {
        // Handle e.g. Owned(GenericClass) passed to a formal : GenericClass
        // TODO: Why is this here and not in getBasicInstantiationType?
        if (isManagedPtrType(at)) {
          Type* classType = getManagedPtrBorrowType(actualType);
          if (canInstantiate(classType, formalType)) {
            retval = classType;
          }
        }
      }
    }
  }

  return retval;
}

// For when getConcreteParentForGenericFormal wouldn't return but there's
// still a valid relationship between the types.
Type* getMoreInstantiatedParentForGenericFormal(Type* actualType,
                                                Type* formalType) {
  Type* retval = NULL;

  if (AggregateType* at = toAggregateType(actualType)) {
    forv_Vec(AggregateType, parent, at->dispatchParents) {
      if (isInstantiation(parent, formalType) == true) {
        retval = parent;
        break;

      } else if (parent == formalType) {
        // There might be a better match, so don't break, but it's still a
        // match
        retval = parent;

      } else if (Type* t = getMoreInstantiatedParentForGenericFormal(parent, formalType)) {
        retval = t;
        break;
      }
    }

    if (retval == NULL) {
      if (isClass(formalType)) {
        // Handle e.g. Owned(GenericClass) passed to a formal : GenericClass
        if (isManagedPtrType(at)) {
          Type* classType = getManagedPtrBorrowType(actualType);
          if (canInstantiate(classType, formalType)) {
            retval = classType;
          }
        }
      }
    }
  }

  return retval;
}

static bool instantiatedFieldsMatch(Type* actualType, Type* formalType) {

  AggregateType* actual = toAggregateType(actualType);
  AggregateType* formal = toAggregateType(formalType);
  if (actual == NULL || formal == NULL) {
    return false;
  }
  if (actual == formal) {
    return true;
  }

  AggregateType* root = actual->getRootInstantiation();
  if (root == formal->getRootInstantiation()) {
    unsigned int formalIdx = 0;
    std::vector<Symbol*>& fields = root->genericFields;
    std::vector<Symbol*>& formalFields = formal->genericFields;
    bool ret = true;
    for (unsigned int i = 0; i < fields.size(); i++) {
      if (formalIdx < formalFields.size() &&
          fields[i]->name == formalFields[formalIdx]->name) {
        formalIdx += 1;
      } else {
        const char* name = fields[i]->name;
        Symbol* actualField = actual->getField(name);
        Symbol* formalField = formal->getField(name);
        if (actualField->type == formalField->type) {
          if (formalField->hasFlag(FLAG_PARAM)) {
            ret = (paramMap.get(actualField) == paramMap.get(formalField));
          } else {
            ret = true;
          }
        } else if (formalField->type->symbol->hasFlag(FLAG_GENERIC) &&
                   isAggregateType(formalField->type) &&
                   isAggregateType(actualField->type) &&
                   instantiatedFieldsMatch(actualField->type, formalField->type)) {
          ret = true;
        } else {
          ret = false;
          break;
        }
      }
    }

    return ret;
  }

  return false;
}

// Returns true iff dispatching the actualType to the formalType
// results in an instantiation. The special exception is that
// it returns true when actualType == formalType (to simplify
// common patterns).
//
// If coercions are also necessary, this function should return false.
// Coercion+instantiation can be handled in getBasicInstantiationType.
bool canInstantiate(Type* actualType, Type* formalType) {
  if (actualType == dtMethodToken) {
    return false;
  }

  if (formalType == dtAny) {
    return true;
  }

  if (isConstrainedType(formalType)) {
    INT_ASSERT(formalType->symbol->hasFlag(FLAG_GENERIC)); //CG TODO: remove?
    return true;
  }

  if (formalType == dtIntegral &&
      (is_int_type(actualType) || is_uint_type(actualType))) {
    return true;
  }

  if (formalType == dtAnyEnumerated && is_enum_type(actualType)) {
    return true;
  }

  if (formalType == dtNumeric &&
      (is_int_type(actualType)  ||
       is_uint_type(actualType) ||
       is_imag_type(actualType) ||
       is_real_type(actualType) ||
       is_complex_type(actualType))) {
    return true;
  }

  if (formalType == dtAnyBool && is_bool_type(actualType)) {
    return true;
  }

  if (formalType == dtAnyComplex && is_complex_type(actualType)) {
    return true;
  }

  if (formalType == dtAnyImag && is_imag_type(actualType)) {
    return true;
  }

  if (formalType == dtAnyReal && is_real_type(actualType)) {
    return true;
  }

  if (formalType == dtAnyPOD && !propagateNotPOD(actualType)) {
    return true;
  }

  if (formalType                                        == dtIteratorRecord &&
      actualType->symbol->hasFlag(FLAG_ITERATOR_RECORD) == true) {
    return true;
  }

  if (formalType                                        == dtIteratorClass &&
      actualType->symbol->hasFlag(FLAG_ITERATOR_CLASS)  == true) {
    return true;
  }

  if (formalType == dtAnyRecord && isUserRecord(actualType)) {
    return true;
  }

  if (actualType == formalType) {
    return true;
  }

  if (isClassLikeOrManaged(actualType) && isClassLike(formalType)) {
    Type* actualC = canonicalClassType(actualType);
    ClassTypeDecoratorEnum actualDec = classTypeDecorator(actualType);

    Type* formalC = canonicalClassType(formalType);
    ClassTypeDecoratorEnum formalDec = classTypeDecorator(formalType);

    if (canInstantiateDecorators(actualDec, formalDec)) {
      // Now that the decorators are checked, verify that the
      // types inside are OK

      // are the decorated class types the same?
      if (actualC == formalC)
        return true;

      // These are all generic other than information in decorator
      if (isBuiltinGenericClassType(formalC))
        return true;

      if (isAggregateType(actualC)) {
        if (instantiatedFieldsMatch(actualType, formalType)) {
          return true;
        }
      }
    }
  } else {
    // Check for e.g. R(int) -> R (classes are handled above)
    // TODO: Is it correct to return true if the types match? That case isn't
    // really an instantiation.
    if (isAggregateType(actualType)) {
      if (instantiatedFieldsMatch(actualType, formalType)) {
        return true;
      }
    }
  }

  return false;
}


//
// returns true if dispatching from actualType to formalType results
// in a compile-time coercion; this is a subset of canCoerce below.
//
static bool canParamCoerce(Type*   actualType,
                           Symbol* actualSym,
                           Type*   formalType,
                           bool*   paramNarrows) {
  if (is_bool_type(formalType) && is_bool_type(actualType)) {
    return true;
  }

  if (is_int_type(formalType)) {
    if (is_bool_type(actualType)) {
      return true;
    }

    if (is_int_type(actualType) &&
        get_width(actualType) < get_width(formalType)) {
      return true;
    }

    if (is_uint_type(actualType) &&
        get_width(actualType) < get_width(formalType)) {
      return true;
    }

    //
    // If the argument is a param, does it
    // store a value that's small enough that it could dispatch to
    // this argument?
    //
    if (VarSymbol* var = toVarSymbol(actualSym)) {
      if (var->immediate) {
        if (fits_in_int(get_width(formalType), var->immediate)) {
          *paramNarrows = true;
          return true;
        }
      }
    }
  }

  if (is_uint_type(formalType)) {
    if (is_bool_type(actualType)) {
      return true;
    }

    if (is_uint_type(actualType) &&
        get_width(actualType) < get_width(formalType)) {
      return true;
    }

    Immediate* imm = nullptr;
    if (VarSymbol* var = toVarSymbol(actualSym)) {
      imm = var->immediate;
    }

    if (is_int_type(actualType) &&
        get_width(actualType) <= get_width(formalType)) {
      // int can coerce to uint
      return true;
    }

    if (imm) {
      if (fits_in_uint(get_width(formalType), imm)) {
        *paramNarrows = true;
        return true;
      }
    }
  }

  // param strings can coerce between string and c_string
  if ((formalType == dtString || formalType == dtStringC) &&
      (actualType == dtString || actualType == dtStringC)) {
    if (actualSym && actualSym->isImmediate()) {
      *paramNarrows = true;
      return true;
    }
  }

  // coerce fully representable integers into real / real part of complex
  if (is_real_type(formalType)) {
    int mantissa_width = get_mantissa_width(formalType);

    // don't coerce bools to reals (per spec: "unintended by programmer")

    if (is_int_type(actualType) || is_uint_type(actualType)) {
      // coerce any integer type to any width real
      return true;
    }

    // coerce real from smaller size
    if (is_real_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;

    // coerce literal/param ints that are exactly representable
    if (VarSymbol* var = toVarSymbol(actualSym)) {
      if (var->immediate) {
        // int/uint params would be handled by any-int-to-real rule above
        if (is_real_type(actualType)) {
          if (fits_in_mantissa_exponent(mantissa_width,
                                        get_exponent_width(formalType),
                                        var->immediate)) {
            *paramNarrows = true;
            return true;
          }
        }
      }
    }
  }

  if (is_imag_type(formalType)) {
    int mantissa_width = get_mantissa_width(formalType);

    // coerce imag from smaller size
    if (is_imag_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;

    // coerce literal/param imag that are exactly representable
    if (VarSymbol* var = toVarSymbol(actualSym)) {
      if (var->immediate) {
        if (is_imag_type(actualType)) {
          if (fits_in_mantissa_exponent(mantissa_width,
                                        get_exponent_width(formalType),
                                        var->immediate)) {
            *paramNarrows = true;
            return true;
          }
        }
      }
    }
  }


  if (is_complex_type(formalType)) {
    int mantissa_width = get_mantissa_width(formalType);

    // don't coerce bools to complexes (per spec: "unintended by programmer")

    // coerce any integer type to any width complex
    if (is_int_type(actualType) || is_uint_type(actualType)) {
      return true;
    }

    // coerce real/imag from smaller size
    if (is_real_type(actualType) &&
        get_width(actualType) <= get_width(formalType)/2)
      return true;
    if (is_imag_type(actualType) &&
        get_width(actualType) <= get_width(formalType)/2)
      return true;

    // coerce smaller complex types
    if (is_complex_type(actualType) &&
        (get_width(actualType) < get_width(formalType)))
      return true;

    // coerce literal/param complexes that are exactly representable
    if (VarSymbol* var = toVarSymbol(actualSym)) {
      if (var->immediate) {
        // int/uint params would be handled by any-int-to-complex rule above
        if (is_real_type(actualType)) {
          if (fits_in_mantissa_exponent(mantissa_width,
                                        get_exponent_width(formalType),
                                        var->immediate)) {
            *paramNarrows = true;
            return true;
          }
        }
        if (is_imag_type(actualType)) {
          if (fits_in_mantissa_exponent(mantissa_width,
                                        get_exponent_width(formalType),
                                        var->immediate)) {
            *paramNarrows = true;
            return true;
          }
        }
        if (is_complex_type(actualType)) {
          bool rePartFits = fits_in_mantissa_exponent(mantissa_width,
                                                      get_exponent_width(formalType),
                                                      var->immediate,
                                                      true);
          bool imPartFits = fits_in_mantissa_exponent(mantissa_width,
                                                      get_exponent_width(formalType),
                                                      var->immediate,
                                                      false);
          if (rePartFits && imPartFits) {
            *paramNarrows = true;
            return true;
          }
        }
      }
    }
  }

  return false;
}

static
bool canCoerceTuples(Type*     actualType,
                     Symbol*   actualSym,
                     Type*     formalType,
                     ArgSymbol* formalSym,
                     FnSymbol* fn) {

  actualType = actualType->getValType();
  formalType = formalType->getValType();

  if (actualType->symbol->hasFlag(FLAG_TUPLE) &&
      formalType->symbol->hasFlag(FLAG_TUPLE)) {
    // Both are tuple types, but the types do not match.
    // Could we coerce each individual tuple element?
    // If so, we can coerce the tuples.
    AggregateType *at = toAggregateType(actualType);
    AggregateType *ft = toAggregateType(formalType);

    Type* atFieldType = NULL;
    Type* ftFieldType = NULL;

    bool starTuple = (actualType->symbol->hasFlag(FLAG_STAR_TUPLE) &&
                      formalType->symbol->hasFlag(FLAG_STAR_TUPLE));

    int i = 1;

    if (at->numFields() != ft->numFields()) {
      // Number of fields differs, so not coercible.
      return false;
    }

    for_fields(atField, at) {
      Symbol* ftField = ft->getField(i);

      bool prom = false;
      bool ok;

      atFieldType = atField->type;
      ftFieldType = ftField->type;

      // Can we coerce without promotion?
      // If the types are the same, yes
      if (atFieldType != ftFieldType) {
        ok = canDispatch(atFieldType, actualSym, ftFieldType, formalSym, fn,
                         &prom, NULL, false);

        // If we couldn't coerce or the coercion would promote, no
        if (!ok || prom)
          return false;
      }

      // For star tuples, we only needed to consider first 2 fields
      // (size and 1st element)
      if (starTuple && i == 2)
        return true;

      i++;
    }

    return true;

  }

  return false;
}

static Type* normalizeTupleTypeToValueTuple(Type* t) {
  if (AggregateType* at = toAggregateType(t->getValType())) {
    if (at->symbol->hasFlag(FLAG_TUPLE)) {
      return computeTupleWithIntent(INTENT_IN, at);
    }
  }

  return t;
}

static
ClassTypeDecoratorEnum removeGenericNilability(ClassTypeDecoratorEnum actual) {
  // Normalize actuals to remove generic-ness
  if (actual == ClassTypeDecorator::BORROWED)
    actual = ClassTypeDecorator::BORROWED_NONNIL;
  if (actual == ClassTypeDecorator::UNMANAGED)
    actual = ClassTypeDecorator::UNMANAGED_NONNIL;
  if (actual == ClassTypeDecorator::MANAGED)
    actual = ClassTypeDecorator::MANAGED_NONNIL;

  return actual;
}

/* ClassTypeDecorator::BORROWED e.g. can represent any nilability,
   but this function assumes that an actual with type
   ClassTypeDecorator::BORROWED
   is actually the same as ClassTypeDecorator::BORROWED_NONNIL.
 */
bool canCoerceDecorators(ClassTypeDecoratorEnum actual,
                         ClassTypeDecoratorEnum formal,
                         bool allowNonSubtypes,
                         bool implicitBang) {

  if (actual == formal)
    return true;

  // Normalize actuals to remove generic-ness
  actual = removeGenericNilability(actual);

  if (actual == formal)
    return true;

  // Don't consider implicit coercion from nilable to non-nilable as subtyping.
  if (allowNonSubtypes == false)
    implicitBang = false;

  switch (formal) {
    case ClassTypeDecorator::BORROWED:
      // borrowed but generic nilability
      // This would be instantiation
      return false;
    case ClassTypeDecorator::BORROWED_NONNIL:
      // Can't coerce away nilable
      return isDecoratorNonNilable(actual) || implicitBang;
    case ClassTypeDecorator::BORROWED_NILABLE:
      // Everything can coerce to a nilable borrowed
      // but only subtypes if the actual is already nilable.
      return allowNonSubtypes || isDecoratorNilable(actual);
    case ClassTypeDecorator::UNMANAGED:
      // unmanaged but generic nilability
      // This would be instantiation
      return false;
    case ClassTypeDecorator::UNMANAGED_NONNIL:
      // Can't coerce away nilable
      // Can't coerce borrowed to unmanaged
      return (implicitBang && actual == ClassTypeDecorator::UNMANAGED_NILABLE);
    case ClassTypeDecorator::UNMANAGED_NILABLE:
      // Can't coerce borrowed to unmanaged
      return (allowNonSubtypes && actual == ClassTypeDecorator::UNMANAGED_NONNIL);

    case ClassTypeDecorator::MANAGED:
      // managed but generic nilability
      // this would be instantiation
      return false;
    case ClassTypeDecorator::MANAGED_NONNIL:
      // Can't coerce away nilable
      // Can't coerce borrowed to managed
      return (implicitBang && actual == ClassTypeDecorator::MANAGED_NILABLE);
    case ClassTypeDecorator::MANAGED_NILABLE:
      // Can't coerce borrowed to managed
      return (allowNonSubtypes && actual == ClassTypeDecorator::MANAGED_NONNIL);

    case ClassTypeDecorator::GENERIC:
      return false; // instantiation not coercion
    case ClassTypeDecorator::GENERIC_NONNIL:
      // generally instantiation
      return implicitBang && actual == ClassTypeDecorator::GENERIC_NILABLE;
    case ClassTypeDecorator::GENERIC_NILABLE:
      // generally instantiation
      return allowNonSubtypes && actual == ClassTypeDecorator::GENERIC_NONNIL;

    // no default for compiler warnings to know when to update it
  }

  return false;
}

// Returns true if actual has the same meaning as formal or
// if passing actual to formal should result in instantiation.
bool canInstantiateDecorators(ClassTypeDecoratorEnum actual,
                              ClassTypeDecoratorEnum formal) {

  if (actual == formal)
    return true;

  // Normalize actuals to remove generic-ness
  actual = removeGenericNilability(actual);

  if (actual == formal)
    return true;

  switch (formal) {
    case ClassTypeDecorator::BORROWED:
      return actual == ClassTypeDecorator::BORROWED_NONNIL ||
             actual == ClassTypeDecorator::BORROWED_NILABLE;
    case ClassTypeDecorator::BORROWED_NONNIL:
    case ClassTypeDecorator::BORROWED_NILABLE:
      return false;

    case ClassTypeDecorator::UNMANAGED:
      return actual == ClassTypeDecorator::UNMANAGED_NONNIL ||
             actual == ClassTypeDecorator::UNMANAGED_NILABLE;
    case ClassTypeDecorator::UNMANAGED_NONNIL:
    case ClassTypeDecorator::UNMANAGED_NILABLE:
      return false;

    case ClassTypeDecorator::MANAGED:
      return actual == ClassTypeDecorator::MANAGED_NONNIL ||
             actual == ClassTypeDecorator::MANAGED_NILABLE;
    case ClassTypeDecorator::MANAGED_NONNIL:
    case ClassTypeDecorator::MANAGED_NILABLE:
      return false;

    case ClassTypeDecorator::GENERIC:
      return true;
    case ClassTypeDecorator::GENERIC_NONNIL:
      return actual == ClassTypeDecorator::GENERIC_NONNIL ||
             actual == ClassTypeDecorator::BORROWED_NONNIL ||
             actual == ClassTypeDecorator::UNMANAGED_NONNIL ||
             actual == ClassTypeDecorator::MANAGED_NONNIL;
    case ClassTypeDecorator::GENERIC_NILABLE:
      return actual == ClassTypeDecorator::GENERIC_NILABLE ||
             actual == ClassTypeDecorator::BORROWED_NILABLE||
             actual == ClassTypeDecorator::UNMANAGED_NILABLE||
             actual == ClassTypeDecorator::MANAGED_NILABLE;

    // no default for compiler warnings to know when to update it
  }

  return false;
}

// Can we instantiate or coerce or both?
bool canInstantiateOrCoerceDecorators(ClassTypeDecoratorEnum actual,
                                      ClassTypeDecoratorEnum formal,
                                      bool allowNonSubtypes,
                                      bool implicitBang) {
  if (actual == formal)
    return true;

  // Normalize actuals to remove generic-ness
  actual = removeGenericNilability(actual);

  if (actual == formal)
    return true;

  // Don't consider implicit coercion from nilable to non-nilable as subtyping.
  if (allowNonSubtypes == false)
    implicitBang = false;

  switch (formal) {
    case ClassTypeDecorator::BORROWED:
      // can borrow from anything, could instantiate as borrowed?
      return true;
    case ClassTypeDecorator::BORROWED_NONNIL:
      // can borrow from anything, but can't coerce away nilability
      return isDecoratorNonNilable(actual) || implicitBang;
    case ClassTypeDecorator::BORROWED_NILABLE:
      // can borrow from anything, can always coerce to nilable
      return allowNonSubtypes || isDecoratorNilable(actual);;

    case ClassTypeDecorator::UNMANAGED:
      // no coercions to unmanaged
      return actual == ClassTypeDecorator::UNMANAGED_NONNIL ||
             actual == ClassTypeDecorator::UNMANAGED_NILABLE;
    case ClassTypeDecorator::UNMANAGED_NONNIL:
      return (implicitBang && actual == ClassTypeDecorator::UNMANAGED_NILABLE);
    case ClassTypeDecorator::UNMANAGED_NILABLE:
      return (allowNonSubtypes && actual == ClassTypeDecorator::UNMANAGED_NONNIL);

    case ClassTypeDecorator::MANAGED:
      return actual == ClassTypeDecorator::MANAGED_NONNIL ||
             actual == ClassTypeDecorator::MANAGED_NILABLE;
    case ClassTypeDecorator::MANAGED_NONNIL:
      return (implicitBang && actual == ClassTypeDecorator::MANAGED_NILABLE);
    case ClassTypeDecorator::MANAGED_NILABLE:
      return (allowNonSubtypes && actual == ClassTypeDecorator::MANAGED_NONNIL);

    case ClassTypeDecorator::GENERIC:
      // accepts anything
      return true;
    case ClassTypeDecorator::GENERIC_NONNIL:
      // accepts anything nonnil
      return isDecoratorNonNilable(actual) || implicitBang;
    case ClassTypeDecorator::GENERIC_NILABLE:
      return allowNonSubtypes || isDecoratorNilable(actual);

    // no default for compiler warnings to know when to update it
  }

  return false;
}

bool allowImplicitNilabilityRemoval(Type* actualType,
                                    Symbol* actualSym,
                                    Type* formalType,
                                    Symbol* formalSym) {

  // If we are trying again for better nilability errors,
  // implicit nilability removal is OK.
  if (inGenerousResolutionForErrors())
    return true;

  return false;
}

// The compiler considers many patterns of "subtyping" as things
// that require coercions (they often require coercions in the generated C).
// However not all coercions are created equal. Some of them are implementing
// subtyping.
// Here we consider a coercion to be implementing "subtyping" and return
// true for this call if, in an ideal implementation, the actual could
// be passed to a `const ref` argument of the formal type.
bool canCoerceAsSubtype(Type*     actualType,
                        Symbol*   actualSym,
                        Type*     formalType,
                        ArgSymbol* formalSym,
                        FnSymbol* fn,
                        bool*     promotes,
                        bool*     paramNarrows) {

  if (actualType == dtNil && isClassLikeOrPtr(formalType) &&
      formalType != dtStringC &&
      (!isNonNilableClassType(formalType) || useLegacyNilability(actualSym)))
    return true;

  if (isManagedPtrType(actualType)) {
    Type* actualBaseType = getManagedPtrBorrowType(actualType);
    AggregateType* actualOwnedShared = toAggregateType(actualType);
    while (actualOwnedShared && actualOwnedShared->instantiatedFrom != NULL)
      actualOwnedShared = actualOwnedShared->instantiatedFrom;

    Type* formalBaseType = NULL;
    AggregateType* formalOwnedShared = toAggregateType(formalType);
    if (isManagedPtrType(formalType)) {
      formalBaseType = getManagedPtrBorrowType(formalType);
      while (formalOwnedShared && formalOwnedShared->instantiatedFrom != NULL)
        formalOwnedShared = formalOwnedShared->instantiatedFrom;
    }

    if (isManagedPtrType(formalType) &&
        actualOwnedShared == formalOwnedShared) {
      // e.g. Owned(Child) coerces to Owned(Parent)
      return canDispatch(actualBaseType, actualSym,
                         formalBaseType, formalSym, fn,
                         promotes, paramNarrows);
    } else if (isClassLike(formalType)) {
      // e.g. Owned(SomeClass) to SomeClass (borrow type)
      return canDispatch(actualBaseType, actualSym,
                         formalType, formalSym, fn,
                         promotes, paramNarrows);
    }
  }

  // Handle coercions nil -> owned/shared
  if (actualType == dtNil && isManagedPtrType(formalType->getValType())) {
    Type* formalBorrowType = getManagedPtrBorrowType(formalType->getValType());
    if (isNilableClassType(formalBorrowType) || useLegacyNilability(actualSym))
      return true;
  }

  // coerce c_ptr to c_ptrConst
  if (actualType->symbol->hasFlag(FLAG_C_PTR_CLASS) &&
      !actualType->symbol->hasFlag(FLAG_C_PTRCONST_CLASS) &&
      formalType->symbol->hasFlag(FLAG_C_PTR_CLASS) &&
      formalType->symbol->hasFlag(FLAG_C_PTRCONST_CLASS)) {
    // check element types match
    Type* actualElt = getDataClassType(actualType->symbol)->typeInfo();
    Type* formalElt = getDataClassType(formalType->symbol)->typeInfo();
    if (actualElt && formalElt && (actualElt == formalElt)) {
      return true;
    }
  }

  // coerce c_ptr to c_void_ptr
  if (actualType->symbol->hasFlag(FLAG_C_PTR_CLASS) && formalType == dtCVoidPtr)
    return true;

  // coerce c_array to c_void_ptr
  if (actualType->symbol->hasFlag(FLAG_C_ARRAY) && formalType == dtCVoidPtr)
    return true;

  // coerce c_array to c_ptr
  if (actualType->symbol->hasFlag(FLAG_C_ARRAY) &&
      formalType->symbol->hasFlag(FLAG_C_PTR_CLASS)) {
    // check element types match
    Type* actualElt = getDataClassType(actualType->symbol)->typeInfo();
    Type* formalElt = getDataClassType(formalType->symbol)->typeInfo();
    if (actualElt && formalElt && (actualElt == formalElt))
      return true;
  }

  // Check for class subtyping
  // Class subtyping needs coercions in order to generate C code.
  if (isClassLike(actualType) && isClassLike(formalType)) {
    AggregateType* actualC =
      toAggregateType(canonicalDecoratedClassType(actualType));
    ClassTypeDecoratorEnum actualDecorator = classTypeDecorator(actualType);
    AggregateType* formalC =
      toAggregateType(canonicalDecoratedClassType(formalType));
    ClassTypeDecoratorEnum formalDecorator = classTypeDecorator(formalType);

    // Check that the decorators allow coercion
    if (canCoerceDecorators(actualDecorator, formalDecorator,
                            /*allowNonSubtypes*/ true,
                            /*implicitBang*/false)) {
      // are the decorated class types the same?
      if (actualC == formalC)
        return true;

      // are we passing a subclass?
      AggregateType* actualParent = actualC;
      while (actualParent != NULL) {
        if (actualParent == formalC)
          return true;
        if (actualParent == dtObject)
          break;
        actualParent = actualParent->dispatchParents.only();
      }
    }
  }

  return false;
}


//
// returns true iff dispatching the actualType to the formalType
// results in a coercion.
//
// fn is the function being called usually but in resolveReturnType it
// is the function we're finding return types for.
bool canCoerce(Type*     actualType,
               Symbol*   actualSym,
               Type*     formalType,
               ArgSymbol* formalSym,
               FnSymbol* fn,
               bool*     promotes,
               bool*     paramNarrows) {
  bool tmpPromotes = false;
  bool tmpParamNarrows = false;

  // MPF TODO: use the intent instead of whether or not `formalType`
  // has a `ref` type in order to rule out coercions that would not
  // be allowed (e.g. passing an `int(8)` argument to a `ref x: int(64)`).

  if (canParamCoerce(actualType, actualSym, formalType, &tmpParamNarrows)) {
    if (paramNarrows) *paramNarrows = tmpParamNarrows;
    return true;
  }

  tmpParamNarrows = false;
  tmpPromotes = false;
  if (canCoerceAsSubtype(actualType, actualSym, formalType, formalSym, fn,
                         &tmpPromotes, &tmpParamNarrows)) {
    if (promotes) *promotes = tmpPromotes;
    if (paramNarrows) *paramNarrows = tmpParamNarrows;
    return true;
  }

  if (canCoerceTuples(actualType, actualSym, formalType, formalSym, fn)) {
    return true;
  }

  if (actualType->symbol->hasFlag(FLAG_REF))
    // ref can't store a param, so no need to propagate paramNarrows
    return canDispatch(actualType->getValType(), actualSym,
                       // MPF: Should this be formalType->getValType() ?
                       formalType, formalSym,
                       fn,
                       promotes);

  // Check for class subtyping
  // Class subtyping needs coercions in order to generate C code.
  if (isClassLike(actualType) && isClassLike(formalType)) {
    AggregateType* actualC =
      toAggregateType(canonicalDecoratedClassType(actualType));
    ClassTypeDecoratorEnum actualDecorator = classTypeDecorator(actualType);
    AggregateType* formalC =
      toAggregateType(canonicalDecoratedClassType(formalType));
    ClassTypeDecoratorEnum formalDecorator = classTypeDecorator(formalType);

    bool implicitBang = allowImplicitNilabilityRemoval(actualType, actualSym,
                                                       formalType, formalSym);

    // Check that the decorators allow coercion
    if (canCoerceDecorators(actualDecorator, formalDecorator,
                            /*allowNonSubtypes*/ true, implicitBang)) {
      // are the decorated class types the same?
      if (actualC == formalC)
        return true;
      else if (isBuiltinGenericClassType(formalType))
        return true; // only decorators matter, other type is generic
      else if (formalC->symbol->hasFlag(FLAG_GENERIC) &&
               instantiatedFieldsMatch(actualC, formalC))
        return true;

      // are we passing a subclass?
      AggregateType* actualParent = actualC;
      while (actualParent != NULL) {
        if (actualParent == formalC)
          return true;
        if (actualParent == dtObject)
          break;
        actualParent = actualParent->dispatchParents.only();
      }
    }
  }

  if (Type* copyType = canCoerceToCopyType(actualType, actualSym,
                                           formalType, formalSym, fn)) {
    if (copyType != actualType) {
      return canDispatch(copyType, actualSym, formalType, formalSym, fn,
                         promotes, paramNarrows);
    }
  }

  return false;
}

/************************************* | **************************************
*                                                                             *
* Returns true iff the actualType can dispatch to the formalType.             *
*                                                                             *
* The function symbol is used to avoid scalar promotion on =.                 *
* param is set if the actual is a parameter (compile-time constant).          *
* fn is the function being called                                             *
*                                                                             *
************************************** | *************************************/

static bool isGenericInstantiation(Type*     concrete,
                                   Type*     generic);


static
bool doCanDispatch(Type*     actualType,
                   Symbol*   actualSym,
                   Type*     formalType,
                   ArgSymbol* formalSym,
                   FnSymbol* fn,
                   bool*     promotes,
                   bool*     paramNarrows,
                   bool      paramCoerce) {
  if (actualType == formalType)
    return true;

  // MPF 2019-09 - for some reason resolving generic initializers
  // needs this adjustment, which started in PR #5424.
  if (fn && (fn->isInitializer() || fn->isCopyInit()) &&
      formalSym == fn->_this &&
      isGenericInstantiation(formalType, actualType))
    return true;

  if (actualType->refType == formalType &&
      // This is a workaround for type problems with tuples
      // in implement forall intents...
      !(fn &&
        fn->hasFlag(FLAG_BUILD_TUPLE) &&
        fn->hasFlag(FLAG_ALLOW_REF)))
    return true;

  if (formalSym != NULL &&
      formalSym->originalIntent == INTENT_INOUT &&
      formalType->symbol->hasFlag(FLAG_REF)) {
    actualType = actualType->getValType();
    formalType = formalType->getValType();
  }

  if (paramCoerce == false &&
      canCoerce(actualType, actualSym,
                formalType, formalSym,
                fn, promotes, paramNarrows))
    return true;

  if (paramCoerce == true  &&
      canParamCoerce(actualType, actualSym,
                     formalType,
                     paramNarrows))
    return true;

  // check if promotion is possible
  if (fn                              != NULL        &&
      fn->name                        != astrSassign &&
      strcmp(fn->name, "these")       != 0           &&
      fn->retTag                      != RET_TYPE    &&
      fn->retTag                      != RET_PARAM   &&
      actualType->scalarPromotionType != NULL        &&
      doCanDispatch(actualType->scalarPromotionType, NULL,
                    formalType, formalSym,
                    fn,
                    promotes,
                    paramNarrows,
                    false)) {
    *promotes = true;
    return true;
  }

  return false;
}

// if paramCoerce is true, only check for param coercions (and not other
// coercions).
bool canDispatch(Type*     actualType,
                 Symbol*   actualSym,
                 Type*     formalType,
                 ArgSymbol* formalSym,
                 FnSymbol* fn,
                 bool*     promotes,
                 bool*     paramNarrows,
                 bool      paramCoerce) {
  bool tmpPromotes     = false;
  bool tmpParamNarrows = false;
  bool retval          = doCanDispatch(actualType, actualSym,
                                       formalType, formalSym,
                                       fn,
                                       &tmpPromotes,
                                       &tmpParamNarrows,
                                       paramCoerce);

  if (promotes     != NULL) {
    *promotes = tmpPromotes;
  }

  if (paramNarrows != NULL) {
    *paramNarrows = tmpParamNarrows;
  }

  return retval;
}

static bool isGenericInstantiation(Type*     concrete,
                                   Type*     generic) {
  AggregateType* atConcrete = toAggregateType(concrete);
  AggregateType* atGeneric = toAggregateType(generic);
  bool           retval   = false;

  if (atGeneric                                != NULL &&
      atGeneric->symbol->hasFlag(FLAG_GENERIC) == true &&

      atConcrete                                 != NULL &&
      atConcrete->isInstantiatedFrom(atGeneric)  == true) {

    retval = true;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool
moreSpecificCanDispatch(FnSymbol* fn, Type* actualType, Type* formalType) {
  if (canDispatch(actualType, NULL, formalType, NULL, fn))
    return true;
  if (canInstantiate(actualType, formalType)) {
    return true;
  }
  return false;
}

static BlockStmt*
getParentBlock(Expr* expr) {
  for (Expr* tmp = expr->parentExpr; tmp; tmp = tmp->parentExpr) {
    if (BlockStmt* block = toBlockStmt(tmp))
      return block;
  }
  if (expr->parentSymbol) {
    if (expr->parentSymbol->defPoint)
      return getParentBlock(expr->parentSymbol->defPoint);
  }
  return NULL;
}

enum MoreVisibleResult {
  FOUND_F1_FIRST,
  FOUND_F2_FIRST,
  FOUND_BOTH,
  FOUND_NEITHER
};

//
// helper routines for isMoreVisible (below);
//

static bool
isDefinedInBlock(BlockStmt* block, FnSymbol* fn) {
  return (fn->defPoint->parentExpr == block);
}

static bool
isDefinedInUseImport(BlockStmt* block, FnSymbol* fn,
                     bool allowPrivateUseImp, bool forShadowScope,
                     Vec<BlockStmt*>& visited) {
  if (block && block->useList) {
    visited.set_add(block);

    for_actuals(expr, block->useList) {
      bool checkThisInShadowScope = false;
      if (UseStmt* use = toUseStmt(expr)) {
        if (use->isPrivate)
          checkThisInShadowScope = true;
      }

      // Skip for now things that don't match the request
      // to find use/import for the shadow scope (or not).
      // (these will be handled in a different call to this function)
      if (forShadowScope != checkThisInShadowScope)
        continue;

      bool isPrivate = false;
      SymExpr* se = NULL;
      if (UseStmt* use = toUseStmt(expr)) {
        se = toSymExpr(use->src);
        isPrivate = use->isPrivate;
        // TODO: handle renames
        if (use->skipSymbolSearch(fn->name))
          continue;
      } else if (ImportStmt* imp = toImportStmt(expr)) {
        se = toSymExpr(imp->src);
        isPrivate = imp->isPrivate;
        // TODO: handle renames
        if (imp->skipSymbolSearch(fn->name))
          continue;
      }
      INT_ASSERT(se);

      // Skip things that are private when considering a nested use/import
      if (isPrivate && !allowPrivateUseImp)
        continue;

      // ignore uses of enums here
      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        // is it defined in the use/imported module?
        if (isDefinedInBlock(mod->block, fn))
          return true;
        // is it defined in something used/imported in that module?
        if (!visited.set_in(mod->block)) {
          if (isDefinedInUseImport(mod->block, fn,
                                   /* allowPrivateUseImp */ false,
                                   /* forShadowScope */ false,
                                   visited))
            return true;
        }
      }
    }
  }

  return false;
}

// Returns a distance measure used to compare the visibility
// of two functions.
//
// Enclosing scope adds 2 distance
// Shadow scope adds 1 distance
//
// Returns -1 if the function is not found here
// or if the block was already visited.
static int
computeVisibilityDistanceInternal(BlockStmt* block, FnSymbol* fn,
                                  int distance, Vec<BlockStmt*>& visited) {
  // if the block was already visited, we should have
  // already gathered the distance if it was there at all
  if (visited.set_in(block))
    return -1;

  // first, check things in the current block or
  // from use/import that don't use a shadow scope
  bool foundHere = isDefinedInBlock(block, fn) ||
                   isDefinedInUseImport(block, fn,
                                        /* allowPrivateUseImp */ true,
                                        /* forShadowScope */ false,
                                        visited);


  if (foundHere) {
    return distance;
  }

  // next, check anything from a use/import in the
  // current block that uses a shadow scope
  bool foundShadowHere = isDefinedInUseImport(block, fn,
                                              /* allowPrivateUseImp */ true,
                                              /* forShadowScope */ true,
                                              visited);

  if (foundShadowHere) {
    return distance+1;
  }

  // next, check parent scope, recursively
  if (BlockStmt* parentBlock = getParentBlock(block)) {
    return computeVisibilityDistanceInternal(parentBlock, fn,
                                             distance+2, visited);
  }

  return -1;
}

// Returns a distance measure used to compare the visibility
// of two functions.
//
// Returns -1 if the function is not found here
static int computeVisibilityDistance(Expr* expr, FnSymbol* fn) {
  //
  // call helper function with visited set to avoid infinite recursion
  //
  Vec<BlockStmt*> visited;
  BlockStmt* block = toBlockStmt(expr);
  if (!block)
    block = getParentBlock(expr);

  return computeVisibilityDistanceInternal(block, fn, 0, visited);
}



static MoreVisibleResult
isMoreVisibleInternal(BlockStmt* block, FnSymbol* fn1, FnSymbol* fn2,
                      Vec<BlockStmt*>& visited1, Vec<BlockStmt*>& visited2) {

  if (visited1.set_in(block) && visited2.set_in(block))
    return FOUND_NEITHER;

  // first, check things in the current block or
  // from use/import that don't use a shadow scope
  bool foundHere1 = isDefinedInBlock(block, fn1) ||
                    isDefinedInUseImport(block, fn1,
                                         /* allowPrivateUseImp */ true,
                                         /* forShadowScope */ false,
                                         visited1);

  bool foundHere2 = isDefinedInBlock(block, fn2) ||
                    isDefinedInUseImport(block, fn2,
                                         /* allowPrivateUseImp */ true,
                                         /* forShadowScope */ false,
                                         visited2);

  if (foundHere1 && foundHere2)
    return FOUND_BOTH;

  if (foundHere1)
    return FOUND_F1_FIRST;

  if (foundHere2)
    return FOUND_F2_FIRST;

  // next, check anything from a use/import in the
  // current block that uses a shadow scope
  bool foundShadowHere1 = isDefinedInUseImport(block, fn1,
                                               /* allowPrivateUseImp */ true,
                                               /* forShadowScope */ true,
                                               visited1);

  bool foundShadowHere2 = isDefinedInUseImport(block, fn2,
                                               /* allowPrivateUseImp */ true,
                                               /* forShadowScope */ true,
                                               visited2);

  if (foundShadowHere1 && foundShadowHere2)
    return FOUND_BOTH;

  if (foundShadowHere1)
    return FOUND_F1_FIRST;

  if (foundShadowHere2)
    return FOUND_F2_FIRST;

  // next, check parent scope, recursively
  if (BlockStmt* parentBlock = getParentBlock(block)) {
    return isMoreVisibleInternal(parentBlock, fn1, fn2, visited1, visited2);
  }

  return FOUND_NEITHER;
}

//
// return whether fn1 is more visible than fn2 from expr
//
static MoreVisibleResult
isMoreVisible(Expr* expr, FnSymbol* fn1, FnSymbol* fn2) {
  //
  // common-case check to see if functions have equal visibility
  //
  if (fn1->defPoint->parentExpr == fn2->defPoint->parentExpr) {
    return FOUND_BOTH;
  }

  //
  // call helper function with visited set to avoid infinite recursion
  //
  Vec<BlockStmt*> visited1;
  Vec<BlockStmt*> visited2;
  BlockStmt* block = toBlockStmt(expr);
  if (!block)
    block = getParentBlock(expr);
  return isMoreVisibleInternal(block, fn1, fn2, visited1, visited2);
}


static Immediate* getImmediate(Symbol* actual) {
  Immediate* imm = NULL;

  if (VarSymbol* var = toVarSymbol(actual)) {
    imm = var->immediate;
  }
  if (EnumSymbol* enumsym = toEnumSymbol(actual)) {
    ensureEnumTypeResolved(toEnumType(enumsym->type));
    imm = enumsym->getImmediate();
  }

  return imm;
}

// Returns:
//   -1 if 't' is not a numeric type
//   0 if 't' is a default numeric type ('int' 'bool' etc)
//   n a positive integer width if 't' is a non-default numeric type
static int classifyNumericWidth(Type* t)
{
  // The default size counts as 0
  if (t == dtInt[INT_SIZE_DEFAULT] ||
      t == dtUInt[INT_SIZE_DEFAULT] ||
      t == dtReal[FLOAT_SIZE_DEFAULT] ||
      t == dtImag[FLOAT_SIZE_DEFAULT] ||
      t == dtComplex[COMPLEX_SIZE_DEFAULT])
    return 0;

  // Bool size 64 should be considered the same as int 64
  // and just treat all bools the same
  // to prefer the default size (i.e. int)
  if (is_bool_type(t))
    return 0;

  if (is_int_type(t) ||
      is_uint_type(t) ||
      is_real_type(t) ||
      is_imag_type(t) ||
      is_bool_type(t))
    return get_width(t);

  if (is_complex_type(t))
    return get_width(t) / 2;

  return -1;
}

typedef enum {
  NUMERIC_TYPE_NON_NUMERIC,
  NUMERIC_TYPE_BOOL,
  NUMERIC_TYPE_INT_UINT,
  NUMERIC_TYPE_REAL,
  NUMERIC_TYPE_IMAG,
  NUMERIC_TYPE_COMPLEX,
} numeric_type_t;

static numeric_type_t classifyNumericType(Type* t)
{
  if (is_bool_type(t)) return NUMERIC_TYPE_BOOL;
  if (is_int_type(t)) return NUMERIC_TYPE_INT_UINT;
  if (is_uint_type(t)) return NUMERIC_TYPE_INT_UINT;
  if (is_real_type(t)) return NUMERIC_TYPE_REAL;
  if (is_imag_type(t)) return NUMERIC_TYPE_IMAG;
  if (is_complex_type(t)) return NUMERIC_TYPE_COMPLEX;

  return NUMERIC_TYPE_NON_NUMERIC;
}

static bool isNegativeParamToUnsigned(Symbol* actualSym,
                                      Type* actualScalarType,
                                      Type* formalType) {
  if (is_int_type(actualScalarType) && is_uint_type(formalType)) {
    if (VarSymbol* var = toVarSymbol(actualSym)) {
      if (Immediate* imm = var->immediate) {
        if (is_negative(imm)) {
          return true;
        }
      }
    }
  }

  return false;
}

// This method implements rules such as that a bool would prefer to
// coerce to 'int' over 'int(8)'.
// Returns
//  0 if there is no preference
//  1 if f1Type is better
//  2 if f2Type is better
static int prefersNumericCoercion(ResolutionCandidate* candidate1,
                                  ResolutionCandidate* candidate2,
                                  const DisambiguationContext& DC,
                                  Symbol* actualSym,
                                  Type* actualScalarType,
                                  Type* f1Type,
                                  Type* f2Type,
                                  const char*& reason) {


  int acWidth = classifyNumericWidth(actualScalarType);
  int f1Width = classifyNumericWidth(f1Type);
  int f2Width = classifyNumericWidth(f2Type);

  if (acWidth < 0 || f1Width < 0 || f2Width < 0) {
    // something is not a numeric type
    return 0;
  }

  // prefer something with the same numeric kind
  numeric_type_t acKind = classifyNumericType(actualScalarType);
  numeric_type_t f1Kind = classifyNumericType(f1Type);
  numeric_type_t f2Kind = classifyNumericType(f2Type);
  if (acKind == f1Kind && acKind != f2Kind) {
    reason = "same numeric kind";
    return 1;
  }
  if (acKind != f1Kind && acKind == f2Kind) {
    reason = "same numeric kind";
    return 2;
  }

  // Otherwise, prefer the function with the same numeric width
  // as the actual. This rule helps this case:
  //
  //  proc f(arg: real(32))
  //  proc f(arg: real(64))
  //  f(myInt64)
  //
  // here we desire to call f(real(64)) e.g. for sin(1)
  //
  // Additionally, it impacts this case:
  //  proc f(a: real(32), b: real(32))
  //  proc f(a: real(64), b: real(64))
  //  f(myInt64, 1.0)
  // (it arranges for it to call the real(64) version vs the real(32) one)
  if (acWidth == f1Width && acWidth != f2Width) {
    reason = "same numeric width";
    return 1;
  }

  if (acWidth != f1Width && acWidth == f2Width) {
    reason = "same numeric width";
    return 2;
  }

  // note that if in the future we allow more numeric coercions,
  // we might need to make this function complete
  // (where currently it falls back on the "can dispatch" check
  //  in some cases). E.g. it could finish up by comparing
  // the two formal types in terms of their index in this list:
  //
  //  int(8) uint(8) int(16) uint(16) int(32) uint(32) int(64) uint(64)
  //  real(32) real(64) imag(32) real(64) complex(64) complex(128)

  return 0;
}

static bool fits_in_bits_no_sign(int width, int64_t i) {
  // is it between -2**width .. 2**width, inclusive?
  int64_t p = 1;
  p <<= width; // now p is 2**width

  return -p <= i && i <= p;
}

static bool fits_in_twos_complement(int width, int64_t i) {
  // would it fit in a width-bit 2's complement representation?

  INT_ASSERT(width < 64);

  int64_t max_pos = 1;
  max_pos <<= width-1;
  max_pos--;

  int64_t min_neg = 1+max_pos;
  return -min_neg <= i && i <= max_pos;
}

static bool fits_in_mantissa_exponent(int mantissa_width,
                                      int exponent_width,
                                      Immediate* imm,
                                      bool realPart) {
  double v = 0.0;

  if (imm->const_kind == NUM_KIND_REAL ||
      imm->const_kind == NUM_KIND_IMAG) {
    if (imm->num_index == FLOAT_SIZE_32)
      v = imm->v_float32;
    else if(imm->num_index == FLOAT_SIZE_64)
      v = imm->v_float64;
    else
      INT_FATAL("unsupported real/imag size");
  } else if (imm->const_kind == NUM_KIND_COMPLEX) {
    if (imm->num_index == COMPLEX_SIZE_64) {
      if (realPart)
        v = imm->v_complex64.r;
      else
        v = imm->v_complex64.i;
    } else if (imm->num_index == COMPLEX_SIZE_128) {
      if (realPart)
        v = imm->v_complex128.r;
      else
        v = imm->v_complex128.i;
    } else
      INT_FATAL("unsupported complex size");
  } else
    INT_FATAL("unsupported number kind");

  if (!std::isfinite(v)) {
    // it must be infinity or nan
    // assume these fit in any sized real
    return true;
  }

  double frac = 0.0;
  int exp = 0;

  frac = frexp(v, &exp);

  int64_t intpart = 2*frac;

  if (fits_in_bits_no_sign(mantissa_width, intpart) &&
      fits_in_twos_complement(exponent_width, exp))
    return true;

  return false;
}

bool explainCallMatch(CallExpr* call) {
  if (!call->isNamed(fExplainCall))
    return false;
  if (explainCallModule && explainCallModule != call->getModule())
    return false;
  if (explainCallLine != -1 && explainCallLine != call->linenum())
    return false;
  return true;
}

static bool shouldSkip(CallExpr* call) {
  FnSymbol* fn = call->getFunction();
  ModuleSymbol* mod = call->getModule();

  if (mod->modTag == MOD_INTERNAL) {
    return true;
  } else if (fn->hasFlag(FLAG_LINE_NUMBER_OK) == false &&
             fn->hasFlag(FLAG_COMPILER_GENERATED)) {
    return true;
  }

  return false;
}

static CallExpr* userCall(CallExpr* call) {
  if (developer)
    return call;
  // If the called function is compiler-generated or is in one of the internal
  // modules, back up the stack until a call is encountered whose target
  // function is neither.

  if (shouldSkip(call)) {
    for (int i = callStack.n-1; i >= 0; i--) {
      CallExpr* cur = callStack.v[i];
      if (!shouldSkip(cur))
        return cur;
    }
  }
  return call;
}

static CallExpr* reissueCompilerWarning(const char* str, int offset, bool err) {
  //
  // Disable compiler warnings in internal modules that are triggered
  // within a dynamic dispatch context because of potential user
  // confusion.  See note in 'issueCompileError' above.
  //
  if (!err && inDynamicDispatchResolution)
    if (callStack.tail()->getModule()->modTag == MOD_INTERNAL &&
        callStack.head()->getModule()->modTag == MOD_INTERNAL)
      return NULL;

  CallExpr* from = NULL;
  for (int i = callStack.n-offset; i >= 0; i--) {
    from = callStack.v[i];
    // We report calls whose target function is not compiler-generated and is
    // not defined in one of the internal modules.
    if (from->linenum() > 0 &&
        from->getModule()->modTag != MOD_INTERNAL &&
        !from->getFunction()->hasFlag(FLAG_COMPILER_GENERATED))
      break;
  }
  return from;
}

static void reissueMsgHelp(CallExpr* from, const char* str, bool err) {
  if (err) {
    USR_FATAL(from, "%s", str);
  } else {
    gdbShouldBreakHere();
    USR_WARN(from, "%s", str);
  }
}

//
// The following several functions support const-ness checking.
// Which is tailored to how our existing Chapel code is written
// and to the current constructor story. In particular:
//
// * Const-ness of fields is not honored within constructors
// and initialize() functions
//
// * A function invoked directly from a constructor or initialize()
// are treated as if were a constructor.
//
// The implementation also tends to the case where such an invocation
// occurs inside a task function within the constructor or initialize().
//
// THESE RULES ARE INTERIM.
// They will change - and the Chapel code will need to be updated -
// for the upcoming new constructor story.
//
// Implementation note: we need to propagate the constness property
// through temp assignments, dereferences, and calls to methods with
// FLAG_REF_TO_CONST_WHEN_CONST_THIS.
//

static bool leftCallIsOrContainsRightCall(CallExpr* callLeft,
                                          CallExpr* callRight) {
  if (callLeft == callRight)
    return true;

  if (callLeft == callRight->parentExpr)
    return true;

  if (ContextCallExpr* cc = toContextCallExpr(callRight->parentExpr))
    if (callLeft == cc->parentExpr)
      return true;

  return false;
}

static void findNonTaskFnParent(CallExpr* call,
                                FnSymbol*& parent, int& stackIdx) {
  if (callStack.n >= 1 &&
      leftCallIsOrContainsRightCall(callStack.v[callStack.n-1], call)) {

    // We assume that 'call' is at the top of the call stack.

    int ix;
    for (ix = callStack.n-1; ix >= 0; ix--) {
      CallExpr* curr = callStack.v[ix];
      Symbol* parentSym = curr->parentSymbol;
      FnSymbol* parentFn = toFnSymbol(parentSym);
      if (!parentFn)
        break;
      if (!isTaskFun(parentFn)) {
        stackIdx = ix;
        parent = parentFn;
        return;
      }
    }
  }

  // backup plan
  parent = toFnSymbol(call->parentSymbol);
  stackIdx = -1;
}

static bool isConstructorLikeFunction(FnSymbol* fn) {
  return strcmp(fn->name, "init")       ==    0;
}

// Is 'call' in a constructor or in initialize()?
// This includes being in a task function invoked from the above.
static bool isInConstructorLikeFunction(CallExpr* call) {
  FnSymbol* parent;
  int stackIdx;
  findNonTaskFnParent(call, parent, stackIdx); // sets the args
  return parent && isConstructorLikeFunction(parent);
}

// Is the function of interest invoked from a constructor
// or initialize(), with the constructor's or initialize's 'this'
// as the receiver actual.
static bool isInvokedFromConstructorLikeFunction(int stackIdx) {
  if (stackIdx > 0) {
    CallExpr* call2 = callStack.v[stackIdx - 1];
    if (FnSymbol* parent2 = toFnSymbol(call2->parentSymbol))
     if (isConstructorLikeFunction(parent2))
      if (call2->numActuals() >= 2)
        if (SymExpr* thisArg2 = toSymExpr(call2->get(2)))
          if (thisArg2->symbol()->hasFlag(FLAG_ARG_THIS))
            return true;
  }
  return false;
}

// Check whether the actual comes from accessing a const field of 'this'
// and the call is in a function invoked directly from this's constructor.
// In such case, fields of 'this' are not considered 'const',
// so we remove the const-ness flag.
static bool checkAndUpdateIfLegalFieldOfThis(CallExpr* call, Expr* actual,
                                             FnSymbol*& nonTaskFnParent) {
  int stackIdx;
  findNonTaskFnParent(call, nonTaskFnParent, stackIdx); // sets the args

  if (SymExpr* se = toSymExpr(actual))
    if (se->symbol()->hasFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS))
      if (isInvokedFromConstructorLikeFunction(stackIdx)) {
          // Yes, this is the case we are looking for.
          se->symbol()->removeFlag(FLAG_REF_TO_CONST);
          return true;
      }

  return false;
}


// little helper
static Symbol* getBaseSymForConstCheck(CallExpr* call) {
  // ensure this is a method call
  INT_ASSERT(call->get(1)->typeInfo() == dtMethodToken);
  SymExpr* baseExpr = toSymExpr(call->get(2));
  INT_ASSERT(baseExpr); // otherwise, cannot do the checking
  return baseExpr->symbol();
}


// Report an error when storing a sync or single variable into a tuple.
// This is because currently we deallocate memory excessively in this case.
void checkForStoringIntoTuple(CallExpr* call, FnSymbol* resolvedFn)
{
  // Do not perform the checks if:
      // not building a tuple
  if (!resolvedFn->hasFlag(FLAG_BUILD_TUPLE) ||
      // sync/single tuples are used in chpl__autoCopy(x: _tuple), allow them
      resolvedFn->hasFlag(FLAG_ALLOW_REF)    ||
      // sync/single tuple *types* and params seem OK
      resolvedFn->retTag != RET_VALUE)
    return;

  for_formals_actuals(formal, actual, call)
    if (isSyncType(formal->type) || isSingleType(formal->type)) {
      const char* name = "";

      if (SymExpr* aSE = toSymExpr(actual))
        if (!aSE->symbol()->hasFlag(FLAG_TEMP))
          name = aSE->symbol()->name;

      USR_FATAL_CONT(actual,
                     "storing a sync or single variable %s in a tuple "
                     "is not currently implemented - "
                     "apply readFE() or readFF()",
                     name);
    }
}


// If 'fn' is the default assignment for a record type, return
// the name of that record type; otherwise return NULL.
static const char* defaultRecordAssignmentTo(FnSymbol* fn) {
  if (fn->name == astrSassign) {
    if (fn->hasFlag(FLAG_COMPILER_GENERATED)) {
      Type* desttype = fn->getFormal(3)->type->getValType();
      INT_ASSERT(desttype != dtUnknown); // otherwise this test is unreliable
      if (isRecord(desttype) || isUnion(desttype))
        return desttype->symbol->name;
    }
  }
  return NULL;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static FnSymbol* resolveUninsertedCall(BlockStmt* insert, CallExpr* call,
                                       bool errorOnFailure);
static FnSymbol* resolveUninsertedCall(Expr*      insert, CallExpr* call,
                                       bool errorOnFailure);

static Expr*     getInsertPointForTypeFunction(Type* type) {
  AggregateType* at     = toAggregateType(type);
  Expr*          retval = NULL;

  // BHARSH TODO: Why not use at->symbol->instantiationPoint ?
  // Some tests failed at the time:
  //   - library/standard/DateTime/*
  //   - mason/*
  if (at == NULL) {
    // Not an AggregateType
    retval = chpl_gen_main->body;

  } else if (at->symbol->instantiationPoint != NULL) {
    retval = at->symbol->instantiationPoint;

  } else {
    // This case applies to non-generic AggregateTypes and
    // possibly to generic AggregateTypes with default fields.
    retval = at->symbol->defPoint;
  }

  return retval;
}

static FnSymbol* resolveUninsertedCall(Type* type, CallExpr* call,
                                       bool errorOnFailure) {
  FnSymbol*      retval = NULL;

  Expr* where = getInsertPointForTypeFunction(type);
  if (BlockStmt* stmt = toBlockStmt(where))
    retval = resolveUninsertedCall(stmt, call, errorOnFailure);
  else
    retval = resolveUninsertedCall(where, call, errorOnFailure);

  return retval;
}

static FnSymbol* resolveUninsertedCall(BlockStmt* insert, CallExpr* call,
                                       bool errorOnFailure) {
  FnSymbol* ret = NULL;
  BlockStmt* block = new BlockStmt(call, BLOCK_SCOPELESS);

  insert->insertAtHead(block); // Tail?

  if (errorOnFailure) {
    resolveCall(call);
    ret = call->resolvedFunction();
  } else {
    ret = tryResolveCall(call, false);
  }

  call->remove();
  block->remove();

  return ret;
}

static FnSymbol* resolveUninsertedCall(Expr* insert, CallExpr* call,
                                       bool errorOnFailure) {
  FnSymbol* ret = NULL;
  BlockStmt* block = new BlockStmt(call, BLOCK_SCOPELESS);

  insert->insertBefore(block);

  if (errorOnFailure) {
    resolveCall(call);
    ret = call->resolvedFunction();
  } else {
    ret = tryResolveCall(call, false);
  }

  call->remove();
  block->remove();

  return ret;
}

static void checkForInfiniteRecord(AggregateType* at, std::set<AggregateType*>& nestedRecords) {
  for_fields(field, at) {
    if (isRecord(field->type)) {
      AggregateType* ft = toAggregateType(field->type);
      if (nestedRecords.find(ft) != nestedRecords.end()) {
        // Found a cycle
        // Note: error message text agreed upon in #10281
        if (ft == at) {
          // Simple cycle:
          // record B {
          //   var b : B;
          // }
          USR_FATAL(field,
                    "record '%s' cannot contain a recursive field '%s' of type '%s'",
                    at->symbol->name,
                    field->name,
                    at->symbol->name);
        } else {
          // Cycle involving multiple records
          if (at->symbol->hasFlag(FLAG_TUPLE)) {
            USR_FATAL(ft, "tuple '%s' cannot contain recursive record type '%s'", at->symbol->name, ft->symbol->name);
          } else {
            USR_FATAL(field,
                      "record '%s' cannot contain a recursive field '%s' whose type '%s' contains '%s'",
                      at->symbol->name,
                      field->name,
                      ft->symbol->name,
                      at->symbol->name);
          }
        }
      } else {
        nestedRecords.insert(ft);
        checkForInfiniteRecord(ft, nestedRecords);
        nestedRecords.erase(ft);
      }
    }
  }
}

// Convenience wrapper
static void checkForInfiniteRecord(AggregateType* at) {
  std::set<AggregateType*> nestedRecords;
  nestedRecords.insert(at);
  checkForInfiniteRecord(at, nestedRecords);
}

static void markArraysOfBorrows(AggregateType* at);

void resolveTypeWithInitializer(AggregateType* at, FnSymbol* fn) {
  at->initializerResolved = true;
  at->resolveStatus = RESOLVED;

  // TODO: this is a hack to allow for further resolution of fields with very
  // simple type-exprs (e.g. "var x : T"). This allows the compiler to resolve
  // the promotion type for an owned class.
  //
  // Long-term, we should organize the AST such that we can immediately resolve
  // all non-generic fields once the type is fully instantiated.
  //
  // An alternative would be to resolve the promotion type once all fields are
  // initialized (and organizing the AST so that situation can be recognized).
  for_fields(field, at) {
    if (field->type == dtUnknown && field->defPoint->exprType != NULL) {
      field->type = field->defPoint->exprType->typeInfo();
    }
  }

  if (isRecord(at)) {
    checkForInfiniteRecord(at);
  }

  if (at->symbol->instantiationPoint == NULL &&
      fn->instantiationPoint() != NULL) {
    at->symbol->instantiationPoint = fn->instantiationPoint();
  }
  if (at->scalarPromotionType == NULL) {
    resolvePromotionType(at);
  }
  if (developer == false) {
    fixTypeNames(at);
  }

  markArraysOfBorrows(at);
}

static void markArraysOfBorrows(AggregateType* at) {

  AggregateType* nextAt = at;

  if (isRecordWrappedType(at)) {
    Symbol* instanceField = at->getField("_instance", false);

    if (instanceField) {
      Type* implType = canonicalDecoratedClassType(instanceField->type);

      if (implType != dtUnknown)
        if (AggregateType* implAt = toAggregateType(implType))
          nextAt = implAt;
    }
  }

  if (isArrayClass(nextAt) && !nextAt->symbol->hasFlag(FLAG_BASE_ARRAY)) {
    Symbol* eltTypeField = nextAt->getField("eltType", false);

    if (eltTypeField) {
      Type* eltType    = eltTypeField->type;

      if (eltType != dtUnknown) {
        // check for infinite records here to avoid compiler crash
        if (isRecord(eltType))
          checkForInfiniteRecord(toAggregateType(eltType));

        if (isOrContainsBorrowedClass(eltType)) {
          at->symbol->addFlag(FLAG_ARRAY_OF_BORROWS);
          nextAt->symbol->addFlag(FLAG_ARRAY_OF_BORROWS);
        }
      }
    }
  }
}

void resolvePromotionType(AggregateType* at) {
  INT_ASSERT(at->scalarPromotionType == NULL);
  INT_ASSERT(at->symbol->hasFlag(FLAG_GENERIC) == false);

  // don't try to resolve promotion types for sync
  // (for erroneous sync of array it leads to coercion which leads
  //  to confusing error messages)
  if (at->symbol->hasFlag(FLAG_SINGLE) || at->symbol->hasFlag(FLAG_SYNC))
    return;

  VarSymbol* temp     = newTemp(at);
  CallExpr* promoCall = new CallExpr("chpl__promotionType", gMethodToken, temp);

  FnSymbol* promoFn = resolveUninsertedCall(at, promoCall, false);

  if (promoFn != NULL) {
    promoFn->setInstantiationPoint(at->symbol->instantiationPoint);
    resolveFunction(promoFn);

    INT_ASSERT(promoFn->retType != dtUnknown);
    INT_ASSERT(promoFn->retTag == RET_TYPE);

    at->scalarPromotionType = promoFn->retType;
  }
}

void resolveDestructor(AggregateType* at) {
  SET_LINENO(at);

  VarSymbol* tmp   = newTemp(at);
  CallExpr*  call  = new CallExpr("deinit", gMethodToken, tmp);

  FnSymbol* deinitFn = resolveUninsertedCall(at, call, false);

  if (deinitFn != NULL) {
    deinitFn->setInstantiationPoint(at->symbol->instantiationPoint);
    resolveFunction(deinitFn);
    at->setDestructor(deinitFn);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool resolveTypeComparisonCall(CallExpr* call);
static bool resolveBuiltinCastCall(CallExpr* call);
static bool resolveClassBorrowMethod(CallExpr* call);
static bool resolveFunctionPointerCall(CallExpr* call);
static void resolveCoerceCopyMove(CallExpr* call);
static void resolvePrimInit(CallExpr* call);
static void resolveRefDeserialization(CallExpr* call);

void resolveCall(CallExpr* call) {
  if (call->primitive) {
    switch (call->primitive->tag) {

    case PRIM_TUPLE_EXPAND:
      resolveTupleExpand(call);
      break;

    case PRIM_SET_MEMBER:
      resolveSetMember(call);
      break;

    case PRIM_DEFAULT_INIT_VAR:
    case PRIM_NOINIT_INIT_VAR:
    case PRIM_INIT_VAR_SPLIT_DECL:
      resolveGenericActuals(call);
      resolvePrimInit(call);
      break;

    case PRIM_INIT_FIELD:
      resolveInitField(call);
      break;

    case PRIM_INIT_VAR:
    case PRIM_INIT_VAR_SPLIT_INIT:
      resolveGenericActuals(call);
      resolveInitVar(call);
      break;

    case PRIM_MOVE:
    case PRIM_ASSIGN:
      resolveMove(call);
      break;

    case PRIM_COERCE:
      resolveCoerce(call);
      break;

    case PRIM_NEW:
      resolveNew(call);
      break;

    case PRIM_RESOLUTION_POINT:
      resolveForResolutionPoint(call);
      break;

    case PRIM_REF_DESERIALIZE:
      resolveRefDeserialization(call);
      break;

    default:
      break;
    }

  } else {

    // TODO: Should these move to preFold?
    if (resolveTypeComparisonCall(call))
      return;

    if (resolveBuiltinCastCall(call))
      return;

    if (resolveClassBorrowMethod(call))
      return;

    if (resolveFunctionPointerCall(call))
      return;

    if (call->isNamed("chpl__coerceCopy")) {
      resolveCoerceCopyMove(call);
      return;
    }

    resolveNormalCall(call);
  }
}

static void resolveRefDeserialization(CallExpr* call) {
  CallExpr* parentCall = toCallExpr(call->parentExpr);
  INT_ASSERT(parentCall && parentCall->isPrimitive(PRIM_MOVE));

  SymExpr* lhsSE = toSymExpr(parentCall->get(1));
  SymExpr* typeSE = toSymExpr(call->get(1));
  INT_ASSERT(lhsSE && typeSE);

  Type* objType = typeSE->symbol()->type;

  AggregateType* t = toAggregateType(objType);
  if (!t) {
    DecoratedClassType* decType = toDecoratedClassType(typeSE->symbol()->type);
    INT_ASSERT(decType);
    t = toAggregateType(decType->getCanonicalClass());
  }

  INT_ASSERT(t);

  Serializers ser = serializeMap[t];
  INT_ASSERT(ser.deserializer);

  // we need to set the resolved function to the deserializer, so that the move
  // can resolve. Otherwise, the ref deserialization primitive has return type
  // c_void_ptr, which is surely not the type of the LHS here.
  call->setResolvedFunction(ser.deserializer);

  lhsSE->symbol()->type = typeSE->symbol()->getRefType();
}

static FnSymbol* resolveNormalCall(CallExpr* call, check_state_t checkState);

FnSymbol* resolveNormalCall(CallExpr* call) {
  return resolveNormalCall(call, CHECK_NORMAL_CALL);
}

FnSymbol* tryResolveCall(CallExpr* call, bool checkWithin) {
  check_state_t checkState = CHECK_CALLABLE_ONLY;
  if (checkWithin)
      checkState = CHECK_BODY_RESOLVES;

  return resolveNormalCall(call, checkState);
}

static bool resolveTypeComparisonCall(CallExpr* call) {

  if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(call->baseExpr)) {
    if (call->numActuals() == 2) {
      const char* name = urse->unresolved;

      bool eq  = name == astrSeq;
      bool ne  = name == astrSne;
      bool lt  = name == astrSlt;
      bool lte = name == astrSlte;
      bool gt  = name == astrSgt;
      bool gte = name == astrSgte;

      if (eq || ne || lt || lte || gt || gte) {
        SymExpr* lhs = toSymExpr(call->get(1));
        SymExpr* rhs = toSymExpr(call->get(2));

        if (lhs && rhs &&
            lhs->symbol()->hasFlag(FLAG_TYPE_VARIABLE) &&
            rhs->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {

          if (eq || ne) {
            Symbol* value = gFalse;
            bool sameType = lhs->symbol()->type == rhs->symbol()->type;
            if (eq && sameType)
              value = gTrue;
            if (ne && !sameType)
              value = gTrue;
            SymExpr* se = new SymExpr(value);
            call->convertToNoop();
            call->replace(se);
            // Put the call back in to aid traversal
            se->getStmtExpr()->insertBefore(call);
          } else {
            USR_WARN(call, "type comparison operators are deprecated; "
              "use isSubtype/isProperSubtype instead");

            rhs->remove();
            lhs->remove();
            call->baseExpr->remove();

            if (lte || gte)
              call->primitive = primitives[PRIM_IS_SUBTYPE];
            else
              call->primitive = primitives[PRIM_IS_PROPER_SUBTYPE];

            if (lt || lte) {
              call->insertAtTail(rhs);
              call->insertAtTail(lhs);
            } else {
              call->insertAtTail(lhs);
              call->insertAtTail(rhs);
            }
          }

          return true;
        }
      }
    }
  }

  return false;
}

Type* computeDecoratedManagedType(AggregateType* canonicalClassType,
                                  ClassTypeDecoratorEnum useDec,
                                  AggregateType* manager,
                                  Expr* ctx) {
  SET_LINENO(ctx);

  INT_ASSERT(!isManagedPtrType(canonicalClassType));
  INT_ASSERT(isClass(canonicalClassType));

  // Now type-construct it with appropriate nilability
  ClassTypeDecoratorEnum d = combineDecorators(ClassTypeDecorator::BORROWED, useDec);
  Type* borrowType = canonicalClassType->getDecoratedClass(d);

  CallExpr* typeCall = new CallExpr(manager->symbol, borrowType->symbol);

  // Find different insert point if ctx isn't in a list
  if (ctx->list == NULL)
    ctx = ctx->parentSymbol->defPoint;

  ctx->insertAfter(typeCall);
  resolveCall(typeCall);
  Type* ret = typeCall->typeInfo();
  typeCall->remove();

  INT_ASSERT(ret && ret != dtUnknown);
  return ret;
}

static void adjustClassCastCall(CallExpr* call)
{
  SymExpr* valueSe = toSymExpr(call->get(1));
  SymExpr* targetTypeSe = toSymExpr(call->get(2));
  bool valueIsType = isTypeExpr(valueSe);
  Type* targetType = targetTypeSe->symbol()->getValType();
  Type* valueType = valueSe->symbol()->getValType();

  // Handle casting from managed type to
  //  * any-management SomeClass
  //  * unmanaged SomeClass
  //  * borrowed SomeClass
  //  * unmanaged
  //  * owned
  // This section just handles merging the decorators.
  // Casts from owned etc. to owned are still also handled in module code.
  // Down-casting is handled in the module code as well.
  if (isClassLikeOrManaged(valueType) && isClassLikeOrManaged(targetType)) {
    Type* canonicalValue = canonicalClassType(valueType);
    ClassTypeDecoratorEnum valueD = classTypeDecorator(valueType);

    Type* canonicalTarget = canonicalClassType(targetType);
    ClassTypeDecoratorEnum targetD = classTypeDecorator(targetType);

    AggregateType* at = NULL;

    // Compute the class type to work with.
    // If the target isn't specifying the class type, get that from the value.
    // e.g. C: borrowed ; C: owned
    if (isBuiltinGenericClassType(canonicalTarget) ||
        (isManagedPtrType(canonicalTarget) &&
         canonicalTarget == getManagedPtrManagerType(canonicalTarget)))
      at = toAggregateType(canonicalValue);
    else
      at = toAggregateType(canonicalTarget);

    // Compute the decorator combining generic properties
    ClassTypeDecoratorEnum d = combineDecorators(targetD, valueD);

    // Compute the type based upon the decorators
    Type* t = NULL;
    if (isDecoratorManaged(d)) {
      AggregateType* manager = NULL;
      if (isDecoratorUnknownManagement(targetD))
        manager = getManagedPtrManagerType(valueType);
      else
        manager = getManagedPtrManagerType(targetType);
      t = computeDecoratedManagedType(at, d, manager, call);
    } else {
      t = at->getDecoratedClass(d);
    }

    // But don't change the target type in some cases that should be errors.
    // These could raise errors here but presently just leave it for
    // a resolution failure
    if (!valueIsType) {
      if (isDecoratorManaged(d) && !isDecoratorManaged(valueD)) {
        // Don't change it, expecting an error
        t = NULL;
      } else if (isDecoratorManaged(d)) {
        AggregateType* manager = getManagedPtrManagerType(valueType);
        if (isManagedPtrType(targetType) &&
            manager != getManagedPtrManagerType(targetType)) {
          // Don't change it, expecting an error
          t = NULL;
        }
      }
    }

    // Replace the target type with the instantiated one if it differs
    if (t && targetType != t) {
      targetTypeSe->setSymbol(t->symbol);
    }

    // Now, do a borrow if it's a cast from managed to not managed.
    // (Note, this assumes that managedType.borrow() does not throw/halt
    //  for nilable managed types storing nil. If that changed, we'd
    //  need another method to call here to get the possibly nil ptr).
    if (isDecoratorManaged(valueD) &&
        !isDecoratorManaged(d) &&
        !valueIsType) {
      if (isDecoratorUnknownManagement(d))
        INT_FATAL(call, "actual value has unknown type");
      // Convert it to borrow before trying to resolve the cast again
      SET_LINENO(call);
      VarSymbol* tmp = newTempConst("cast_tmp");
      Symbol* valueSym = valueSe->symbol();
      CallExpr* c = new CallExpr("borrow", gMethodToken, valueSym);
      CallExpr* m = new CallExpr(PRIM_MOVE, tmp, c);
      call->getStmtExpr()->insertBefore(new DefExpr(tmp));
      call->getStmtExpr()->insertBefore(m);
      resolveCallAndCallee(c);
      resolveCall(m);

      // Now update the cast call we have to cast
      // the result of the borrow
      valueSe->setSymbol(tmp);
    }
  }
}

static bool resolveBuiltinCastCall(CallExpr* call)
{
  UnresolvedSymExpr* urse = toUnresolvedSymExpr(call->baseExpr);
  if (urse == NULL)
    return false;

  if (urse->unresolved != astrScolon || call->numActuals() != 2)
    return false;

  SymExpr* valueSe = toSymExpr(call->get(1));
  SymExpr* targetTypeSe = toSymExpr(call->get(2));

  if (targetTypeSe && valueSe &&
      targetTypeSe->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {

    // Fix types for e.g. cast to generic-management MyClass.
    // This can change the symbols pointed to by targetTypeSe/valueSe.
    adjustClassCastCall(call);

    Type* targetType = targetTypeSe->symbol()->getValType();
    Type* valueType = valueSe->symbol()->getValType();

    // handle casts from types
    if (isTypeExpr(valueSe)) {
      if (targetType == dtString) {
        // Handle cast of type to string
        call->primitive = primitives[PRIM_NOOP];
        call->baseExpr->remove();
        if (CallExpr* parentCall = toCallExpr(call->parentExpr)) {
          if (parentCall->isPrimitive(PRIM_MOVE) ||
              parentCall->isPrimitive(PRIM_ASSIGN)) {
            SET_LINENO(call);
            const char* typeName = toString(valueType);
            Symbol* typeNameSym = new_StringSymbol(typeName);
            call->replace(new SymExpr(typeNameSym));
            parentCall->getStmtExpr()->insertBefore(call);
          }
        }

        return true;
      } else {
        Type* cSrc = canonicalClassType(valueType);
        Type* cDst= canonicalClassType(targetType);
        if (isSubtypeOrInstantiation(cSrc, cDst, call) ||
            isSubtypeOrInstantiation(cDst, cSrc, call)) {
          // Handle e.g. owned MyClass:borrowed
          call->primitive = primitives[PRIM_NOOP];
          call->baseExpr->remove();
          if (CallExpr* parentCall = toCallExpr(call->parentExpr)) {
            if (parentCall->isPrimitive(PRIM_MOVE) ||
                parentCall->isPrimitive(PRIM_ASSIGN)) {
              SET_LINENO(call);
              call->replace(new SymExpr(targetType->symbol));
              parentCall->getStmtExpr()->insertBefore(call);
            }
          }
          return true;
        } else {
          return false;
        }
      }
    }

    // If it's a trivial cast, replace it with PRIM_CAST.
    // Trivial casts are those casts that:
    //   * are already handled by C/LLVM
    //   * implement coercions in Chapel's type system
    // e.g. casting pointer or numeric types.
    //
    // The following are exceptions:
    //  * casts involving records (e.g. owned to borrowed)
    //  * casts involving complex (e.g. imag to complex)
    //  * promoted casts

    bool promotes = false;
    bool paramNarrows = false;
    bool paramCoerce = false;
    bool dispatches = canDispatch(valueType, valueSe->symbol(),
                                  targetType, NULL, call->getFunction(),
                                  &promotes, &paramNarrows, paramCoerce);

    if (!isRecord(targetType) && !isRecord(valueType) &&
        !is_complex_type(targetType) && !is_complex_type(valueType) &&
        dispatches && !promotes) {

      // Otherwise, convert the _cast call to a primitive cast
      call->baseExpr->remove();
      call->primitive = primitives[PRIM_CAST];

      // swap the order of the two arguments
      //  have: val, type
      //  want: type, val
      valueSe->remove();
      targetTypeSe->insertAfter(valueSe);

      // Add a dereference for references to avoid confusing the compiler.
      if (valueSe->symbol()->isRef()) {
        if (targetTypeSe->symbol()->isRef())
          INT_FATAL("casting from reference to reference not handled");

        SET_LINENO(call);

        // Dereference before casting
        VarSymbol* tmp = newTempConst("cast_tmp");
        CallExpr* c = new CallExpr(PRIM_DEREF, valueSe->symbol());
        CallExpr* m = new CallExpr(PRIM_MOVE, tmp, c);
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        call->getStmtExpr()->insertBefore(m);
        resolveCall(m);

        // Now update the cast call we have to cast the result of deref
        valueSe->setSymbol(tmp);
      }

      return true;
    }
  }

  return false;
}

static bool resolveClassBorrowMethod(CallExpr* call) {
  if (isUnresolvedSymExpr(call->baseExpr)) {
    if (call->numActuals() == 2) { //mt, this
      if (call->isNamedAstr(astrBorrow) && dtMethodToken == call->get(1)->typeInfo()) {
        Type *t = call->get(2)->getValType();
        if (isClassLike(t)) {
          CallExpr *pe = toCallExpr(call->parentExpr);
          INT_ASSERT(call->methodTag && pe && pe->baseExpr == call);

          // if the class is nilable the borrow should be too
          ClassTypeDecoratorEnum d = ClassTypeDecorator::BORROWED_NONNIL;
          if (isDecoratorNilable(classTypeDecorator(t))) {
            d = ClassTypeDecorator::BORROWED_NILABLE;
          }

          // this works around a compiler bug
          AggregateType *at = toAggregateType(canonicalDecoratedClassType(t));
          Type *newType = at->getDecoratedClass(d);

          // make the call a PRIM_CAST
          call->baseExpr->remove();
          call->primitive = primitives[PRIM_CAST];

          call->get(1)->remove();  //remove method token
          Expr *receiver = call->get(1)->remove(); // remove `this`

          // add arguments to PRIM_CAST
          call->insertAtTail(newType->symbol);
          call->insertAtTail(receiver);

          // put the cast before the parent
          pe->insertBefore(call->remove());

          //make parent noop
          pe->convertToNoop();

          return true;
        }
      }
    }
  }
  return false;
}

// TODO: Ideally, we would be able to leverage the existing machinery for
// resolving calls, but we may not be able to do that until dyno is used
// to resolve code.
static bool resolveFunctionPointerCall(CallExpr* call) {
  FunctionType* ft = nullptr;
  if (auto base = call->baseExpr)
    if (auto se = toSymExpr(base))
      if (auto baseFnType = toFunctionType(se->getValType()))
        ft = baseFnType;

  if (!ft) return false;

  auto base = call->baseExpr;

  // TODO: Support default arguments?
  if (call->numActuals() != ft->numFormals()) {
    USR_FATAL(call, "incorrect number of arguments - expected '%d', "
                    "but found '%d'",
                    ft->numFormals(),
                    call->numActuals());
    return true;
  }

  // TODO: Support named actuals. We have to refactor code from things like
  // 'ResolutionCandidate::computeAlignment', and 'wrapAndCleanUpActuals'.
  for_actuals(actual, call) {
    if (isNamedExpr(actual)) {
      auto se = toSymExpr(base);
      const char* name = se ? se->symbol()->name : nullptr;

      if (name) {
        USR_FATAL_CONT(actual, "calls to function values ('%s' in this "
                               "case) do not support named arguments yet",
                               name);
      } else {
        USR_FATAL_CONT(actual, "calls to function values do not support "
                               "named arguments yet");
      }
    }
  }

  bool onceForErrorHeader = true;

  // TODO: Can we rework 'ResolutionCandidate' to operate in terms of
  // function types? That might enable us to use that machinery here.
  for (int i = 0; i < ft->numFormals(); i++) {
    auto actual = call->get(i + 1);

    Type* actualType = actual->qualType().type();
    Symbol* actualSym = isSymExpr(actual) ? toSymExpr(actual)->symbol()
                                          : nullptr;
    Type* formalType = ft->formal(i)->type;
    ArgSymbol* formalSym = nullptr;
    FnSymbol* fn = nullptr;
    bool promotes = false;
    bool paramNarrows = false;
    const bool paramCoerce = false;

    bool ok = canDispatch(actualType, actualSym, formalType, formalSym, fn,
                          &promotes,
                          &paramNarrows,
                          paramCoerce);
    if (!ok) {
      if (onceForErrorHeader) {
        USR_FATAL_CONT(call, "failed to resolve call");
        onceForErrorHeader = false;
      }

      USR_FATAL_CONT(actual, "because actual argument with type '%s' is "
                             "passed to formal '%s'",
                             toString(actualType),
                             toString(formalType));
    }
  }

  return true;
}

// Save chpl__coerceMove with the same arguments as chpl__coerceCopy
// in case copy elision decides to replace a chpl__coerceCopy with a coerceMove.
//
// compilerErrors reported in coerceMove are reported now, but errors in
// coerceCopy will be put off until callDestructors.
static void resolveCoerceCopyMove(CallExpr* call) {
  // Try resolving the coerceCopy call
  tryResolveCall(call, /* checkWithin */ false);

  // Note the chpl__coerceCopy fn
  FnSymbol* copyFn = call->resolvedFunction();

  // Couldn't find a copy function - try again to give unresolved error
  if (copyFn == NULL) {
    resolveNormalCall(call);
    return;
  }

  // Now try to find the corresponding chpl__coerceMove.
  if (coerceMoveFromCopyMap.get(copyFn) != NULL) {
    // already resolved and put into map; nothing more to do here.
    return;
  }

  // Add a block in which we can temporarily resolve chpl__coerceMove
  BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);
  call->getStmtExpr()->insertAfter(block);

  // Add another call next to it to chpl__coerceMove
  CallExpr* coerceMove = call->copy();
  block->insertAtTail(coerceMove);
  coerceMove->baseExpr->replace(new UnresolvedSymExpr(astr_coerceMove));
  // Resolve the chpl__coerceMove call (and its body)
  resolveCallAndCallee(coerceMove, false);
  // Add it to the map
  FnSymbol* moveFn = coerceMove->resolvedFunction();
  coerceMoveFromCopyMap.put(copyFn, moveFn);
  // Remove the chpl__coerceMove call and any temps added
  block->remove();

  // After the chpl__coerceMove body is resolved, resolve
  // the body of chpl__coerceCopy. Doing it in this order allows
  // compilerErrors in chpl__coerceMove to halt compilation.
  // That prevents the compiler from trying to continue past severe
  // errors, like rank mismatch, when compiling chpl__coerceCopy
  // (since chpl__coerceCopy might save an error for later).
  //
  // A more robust solution would be preferred.
  inTryResolve++;
  tryResolveStates.push_back(CHECK_BODY_RESOLVES);
  tryResolveFunctions.push_back(copyFn);

  resolveFunction(copyFn);

  tryResolveFunctions.pop_back();
  tryResolveStates.pop_back();
  inTryResolve--;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool      isGenericRecordInit(CallExpr* call);

static FnSymbol* resolveNormalCall(CallInfo& info, check_state_t checkState);
static FnSymbol* resolveNormalCall(CallExpr* call, check_state_t checkState);

static BlockStmt* findVisibleFunctionsAndCandidates(
                                     CallInfo&                  info,
                                     VisibilityInfo&            visInfo,
                                     Vec<FnSymbol*>&            visibleFns,
                                     Vec<ResolutionCandidate*>& candidates);

static int       disambiguateByMatch(CallInfo&                  info,
                                     BlockStmt*                 searchScope,
                                     Vec<ResolutionCandidate*>& candidates,
                                     ResolutionCandidate*&      bestRef,
                                     ResolutionCandidate*&      bestConstRef,
                                     ResolutionCandidate*&      bestValue);

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   check_state_t        checkState,
                                   ResolutionCandidate* best);

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   check_state_t        checkState,
                                   ResolutionCandidate* bestRef,
                                   ResolutionCandidate* bestConstRef,
                                   ResolutionCandidate* bestValue);

static void      resolveNormalCallConstRef(CallExpr* call);

static void      resolveNormalCallFinalChecks(CallExpr* call);

static FnSymbol* wrapAndCleanUpActuals(ResolutionCandidate* best,
                                       CallInfo&            info,
                                       bool                 followerChecks);

static bool isTypeConstructionCall(CallExpr* call) {
  bool ret = false;

  if (SymExpr* se = toSymExpr(call->baseExpr)) {
    if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
      // Compiler may represent accesses of instantiated tuple components in
      // the same way as a type construction call, so skip that case here.
      //
      // A SymExpr of dtTuple indicates tuple type construction.
      // TODO: Shouldn't we see a call to 'this' as the baseExpr?
      if (se->typeInfo()->symbol->hasFlag(FLAG_TUPLE) &&
          se->typeInfo() != dtTuple) {
        ret = false;
      } else {
        ret = true;
      }
    }
  }

  return ret;
}

// t is the type we resolved call to return
static void warnForPartialInstantiationNoQ(CallExpr* call, Type* t) {
  // is the resulting type generic?
  if (t != nullptr) {
    CallExpr* checkCall = call;
    if (call->numActuals() >= 1) {
      // check for 'owned C' e.g.
      if (SymExpr* se = toSymExpr(call->baseExpr)) {
        if (se->symbol()->hasFlag(FLAG_MANAGED_POINTER)) {
          checkCall = toCallExpr(call->get(1));
        }
      }
    }
    if (checkCall != nullptr && checkCall->numActuals() > 0) {
      bool foundQuestionMarkArg = false;
      for_actuals(actual, checkCall) {
        if (SymExpr* se = toSymExpr(actual)) {
          if (se->symbol() == gUninstantiated) {
            foundQuestionMarkArg = true;
          }
        }
      }

      if (!foundQuestionMarkArg) {
        Type* tt = canonicalClassType(t);
        if (tt && tt->symbol->hasFlag(FLAG_GENERIC)) {
          // print out which field
          USR_WARN(checkCall, "partial instantiation without '?' argument");
          USR_PRINT(checkCall, "opt in to partial instantiation explicitly with a trailing '?' argument");
          USR_PRINT(checkCall, "or, add arguments to instantiate the following fields in generic type '%s':", tt->symbol->name);
          // which field names are generic?
          if (AggregateType* at = toAggregateType(tt)) {
            bool printedAnyFields = false;
            for_fields(field, at) {
              if (field->type == dtUnknown ||
                  field->type->symbol->hasFlag(FLAG_GENERIC)) {
                const char* k = "";
                bool possiblyDependent = false;
                if (field->hasFlag(FLAG_TYPE_VARIABLE)) {
                  k = " type";
                } else if (field->hasFlag(FLAG_PARAM)) {
                  k = " param";
                } else if (field->defPoint->exprType == nullptr) {
                  // field with no type e.g. var x;
                } else {
                  // var x: something could be concrete or generic,
                  // depending on what 'something' refers to,
                  // so only report such a field if it's the first generic one.
                  possiblyDependent = true;
                }

                if (!printedAnyFields || !possiblyDependent) {
                  USR_PRINT(field, "  generic%s field '%s'", k, field->name);
                  printedAnyFields = true;
                }
              }
            }
          }
        }
      }
    }
  }
}


static Type* resolveTypeSpecifier(CallInfo& info) {
  CallExpr* call = info.call;
  if (call->id == breakOnResolveID) gdbShouldBreakHere();

  Type* ret = NULL;

  SymExpr* ts = toSymExpr(call->baseExpr);
  Type* tsType = ts->typeInfo();
  AggregateType* at = toAggregateType(canonicalClassType(tsType));
  ClassTypeDecoratorEnum decorator = ClassTypeDecorator::BORROWED_NONNIL;
  bool decorated = false;
  if (DecoratedClassType* dt = toDecoratedClassType(ts->typeInfo())) {
    decorated = true;
    decorator = dt->getDecorator();
    // Convert 'managed' to 'generic' -
    // type will be wrapped with 'owned' e.g. after borrowed type is computed.
    if (isDecoratorManaged(decorator)) {
      if (isDecoratorNonNilable(decorator))
        decorator = ClassTypeDecorator::GENERIC_NONNIL;
      else if (isDecoratorNilable(decorator))
        decorator = ClassTypeDecorator::GENERIC_NILABLE;
      else
        decorator = ClassTypeDecorator::GENERIC;
    }
  }

  if (at == NULL) {
    USR_FATAL_CONT(info.call, "illegal type index expression '%s'", info.toString());
    const char* typeclass = (isPrimitiveType(tsType) ? "primitive type" :
                             (isEnumType(tsType) ? "enum type" : "type"));
    USR_PRINT(info.call, "%s '%s' cannot be used in an index expression", typeclass, tsType->symbol->name);
    USR_STOP();
  } else if (at->symbol->hasFlag(FLAG_TUPLE)) {
    SymbolMap subs;
    if (FnSymbol* fn = createTupleSignature(NULL, subs, call)) {
      ret = fn->retType;
    }
  } else {
    ret = at->generateType(info.call, info.toString());
    if (ret && decorated) {
      // Include the decorator in the type
      ret = getDecoratedClass(ret, decorator);
      INT_ASSERT(ret);
    }

    if (ret && isManagedPtrType(tsType)) {
      // For owned/shared types, resolve the canonical class type specifier and
      // re-wrap the resulting type in owned or shared.
      AggregateType* manager = getManagedPtrManagerType(tsType);
      if (isManagedPtrType(ret)) {
        // It's already a managed pointer type
        INT_ASSERT(manager == getManagedPtrManagerType(ret));
      } else {
        CallExpr* again = new CallExpr(manager->symbol, ret->symbol);
        info.call->getStmtExpr()->insertBefore(again);
        resolveCall(again);
        ret = again->typeInfo();
        again->remove();
      }
    }
    warnForPartialInstantiationNoQ(call, ret);
  }

  if (ret != NULL) {
    call->baseExpr->replace(new SymExpr(ret->symbol));
  }

  if (isAggregateType(ret) &&
      ret->scalarPromotionType == NULL &&
      ret->symbol->hasFlag(FLAG_REF) == false &&
      ret->symbol->hasFlag(FLAG_GENERIC) == false) {
    resolvePromotionType(toAggregateType(ret));
  }

  return ret;
}

static void warnIfMisleadingNew(CallExpr* call, Type* lhs, Type* rhs) {
  // return 'true' for:
  //  * lhs type is borrowed
  //  * rhs type is owned/shared/unmanaged
  //  * rhs represents a 'new' expression
  if (isBorrowedClass(lhs) &&
      (isUnmanagedClass(rhs) || isManagedPtrType(rhs))) {

    // check for rhs being a 'new' expression using pattern matching
    Symbol* dst = nullptr;
    Symbol* src = nullptr;
    bool foundNew = false;
    if (call->numActuals() >= 2) {
      if (call->isPrimitive(PRIM_INIT_VAR) ||
          call->isPrimitive(PRIM_INIT_VAR_SPLIT_INIT)) {
        if (SymExpr* se = toSymExpr(call->get(1))) {
          dst = se->symbol();
        }
        if (SymExpr* se = toSymExpr(call->get(2))) {
          src = se->symbol();
        }
      } else if (call->isNamedAstr(astrSassign)) {
        int i = 1;
        if (call->get(1)->typeInfo() == dtMethodToken) {
          i += 2; // pass method token and (type) this arg
        }
        if (i+1 <= call->numActuals()) {
          if (SymExpr* se = toSymExpr(call->get(i))) {
            dst = se->symbol();
          }
          if (SymExpr* se = toSymExpr(call->get(i+1))) {
            src = se->symbol();
          }
        }
      }
    }

    // rely on FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW which
    // is introduced for temporaries storing the result of a 'new' expression
    if (src &&
        src->hasFlag(FLAG_TEMP) &&
        src->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW)) {
      foundNew = true;
    }

    if (foundNew && dst && !dst->hasFlag(FLAG_TEMP)) {
      USR_WARN(userCall(call),
               "here, '%s' is set to the result of borrowing from a 'new'",
               dst->name);
      USR_PRINT(userCall(call),
                "if you meant to save the result of new, change "
                "the type of '%s' to not be 'borrowed'",
                dst->name);
      USR_PRINT(userCall(call),
                "if you meant to refer to another variable storing the 'new', "
                "use an explicit variable, e.g. "
                "'var myOwned = new C(); var myBorrowed = myOwned.borrow();'");
    }
  }
}

static
void resolveNormalCallAdjustAssign(CallExpr* call) {
  if (call->isNamedAstr(astrSassign)) {
    int i = 1;
    if (call->get(1)->typeInfo() == dtMethodToken) {
      i += 2; // pass method token and (type) this arg
    }
    if (i <= call->numActuals()) {
      Expr* lhsExpr = call->get(i);
      Expr* rhsExpr = call->get(i+1);
      Type* targetType = lhsExpr->typeInfo();
      Type* srcType = rhsExpr->getValType();

      // Adjust the type for formal_temp_out before trying to resolve '='
      if (SymExpr* lhsSe = toSymExpr(lhsExpr)) {
        if (targetType == dtSplitInitType) {
          targetType = srcType;
        } else if (targetType->symbol->hasFlag(FLAG_GENERIC)) {
          targetType = getInstantiationType(srcType, NULL, targetType, NULL,
                                            call,
                                            /* allowCoercion */ true,
                                            /* implicitBang */ false,
                                            /* inOrOtherValue */ true);
        }
        lhsSe->symbol()->type = targetType;
      }

      // This is a workaround to avoid deprecation warnings for sync/single
      // variables within compiler-generated assignment functions.
      FnSymbol* inFn = call->getFunction();
      if (isUnresolvedSymExpr(call->baseExpr) &&
          inFn->name == astrSassign &&
          inFn->hasFlag(FLAG_COMPILER_GENERATED) &&
          (isSyncType(srcType) || isSingleType(srcType)) &&
          targetType->getValType() == srcType) {

        // if we are in a compiler-generated assign, rewrite sync/single
        // assignment to avoid deprecation warnings.

        // remove this and method token if this was a method invocation of =
        for (int j = 1; j < i; j++) {
          call->get(1)->remove();
        }
        INT_ASSERT(call->numActuals() == 2);

        Expr* newBase =
          new UnresolvedSymExpr("chpl__compilerGeneratedAssignSyncSingle");
        call->baseExpr->replace(newBase);
      }

      // warn for misleading new in assign statement
      warnIfMisleadingNew(call, targetType->getValType(), srcType);
    }
  }
}


static
FnSymbol* resolveNormalCall(CallExpr* call, check_state_t checkState) {
  CallInfo  info;
  FnSymbol* retval = NULL;

  if (checkState != CHECK_NORMAL_CALL) {
    inTryResolve++;
    tryResolveStates.push_back(checkState);
  }

  if (call->id == breakOnResolveID) {
    printf("breaking on resolve call %d:\n", call->id);
    print_view(call);
    gdbShouldBreakHere();
  }

  resolveGenericActuals(call);

  resolveNormalCallAdjustAssign(call);

  if (isGenericRecordInit(call) == true) {
    retval = resolveInitializer(call);
  } else if (info.isWellFormed(call) == true) {
    if (isTypeConstructionCall(call)) {
      resolveTypeSpecifier(info);
    } else {
      retval = resolveNormalCall(info, checkState);
    }

  } else if (checkState != CHECK_NORMAL_CALL) {
    retval = NULL;

  } else {
    info.haltNotWellFormed();
  }

  if (checkState != CHECK_NORMAL_CALL) {
    if (checkState == CHECK_BODY_RESOLVES) {
      if (FnSymbol* fn = call->resolvedFunction()) {
        tryResolveFunctions.push_back(fn);
        // resolve that function as well
        resolveFunction(fn);
        tryResolveFunctions.pop_back();
      }
    }

    check_state_t state = tryResolveStates.back();
    tryResolveStates.pop_back();
    inTryResolve--;

    // Also check for errors in tryResolveErrors
    // in case this is the 2nd time we attempted to resolve it.
    if (FnSymbol* fn = call->resolvedFunction()) {
      try_resolve_map_t::iterator it;
      it = tryResolveErrors.find(fn);
      if (it != tryResolveErrors.end()) {
        state = CHECK_FAILED;
      }
    }

    if (state == CHECK_FAILED)
      retval = NULL;
  }

  return retval;
}

static bool isGenericRecordInit(CallExpr* call) {
  bool retval = false;

  if (UnresolvedSymExpr* ures = toUnresolvedSymExpr(call->baseExpr)) {
    if ((ures->unresolved == astrInit ||
         ures->unresolved == astrInitEquals) &&
        call->numActuals()               >= 2) {
      Type* t1 = call->get(1)->typeInfo();
      Type* t2 = call->get(2)->typeInfo();

      if (t1                                  == dtMethodToken &&
          isGenericRecordWithInitializers(t2) == true) {
        retval = true;
      }
    }
  }

  return retval;
}


// Return the module for RC->fn, if it exists and is not internal.
// Otherwise, return NULL, meaning "nothing to consider here".
static inline ModuleSymbol* overloadSetModule(ResolutionCandidate* RC) {
  if (RC == NULL) return NULL;
  ModuleSymbol* mod = RC->fn->getModule();
  if (mod->modTag == MOD_INTERNAL) return NULL;
  return mod;
}

// As of this writing, this always succeeds on our test suite.
static bool bestModulesAreConsistent(CallExpr* call, check_state_t checkState,
   ModuleSymbol* mod1, ResolutionCandidate* rc1, const char* descr1,
   ModuleSymbol* mod2, ResolutionCandidate* rc2, const char* descr2)
{
  if (mod1 != NULL && mod2 != NULL && mod1 != mod2) {
    if (checkState == CHECK_NORMAL_CALL) {
      USR_FATAL_CONT(call,
                     "the best %s and %s overloads are in different modules",
                     descr1, descr2);
      USR_PRINT(rc1->fn, "the best %s overload is here", descr1);
      USR_PRINT(rc2->fn, "the best %s overload is here", descr2);
      USR_STOP();
    }
    return false;
  }
  return true;
}

static bool isAcceptableArgOverloadSets (CallExpr* call, FnSymbol* bestFn,
                                         int argNum) {
  // Should this be a public utility function ex. canonicalClassTypeOrNull() ?
  Type* actualType = canonicalClassType(call->get(argNum)->getValType());
  AggregateType* actualClass = toAggregateType(actualType);
  if (actualClass == NULL || ! actualClass->isClass())
    return false;  // the arg was not a class, so we shouldn't check

  ModuleSymbol* actualMod = actualClass->getModule();
  if (actualMod == bestFn->getModule())
    return true;  // bestFn and the arg's class are in the same module

  return false;
}

// If 'call' is a method call on a class, accept it if 'bestFn' is
// in the same module as the class of the receiver actual of 'call'.
//
// The purpose of this function is to accept certain method calls
// that method-oblivious checking already rejected.
static bool isAcceptableMethodChoice(CallExpr* call,
                                 FnSymbol* bestFn, FnSymbol* candFn,
                                 Vec<ResolutionCandidate*>& candidates)
{
  if (!bestFn->hasFlag(FLAG_OPERATOR) && !candFn->hasFlag(FLAG_OPERATOR)) {
    if (call->numActuals() < 2 || call->get(1)->getValType() != dtMethodToken)
      return false;  // not a method call

  } else if (call->get(1)->getValType() != dtMethodToken) {
    // At least one of these was an operator function (hopefully both), but the
    // call wasn't set up as a method call originally.  We should still treat it
    // somewhat like a method call just in case.
    bool firstArgRes = isAcceptableArgOverloadSets(call, bestFn, 1);
    if (firstArgRes)
      return firstArgRes;

    if (call->numActuals() < 2)
      // This was a unary operator, don't check for a second argument, just
      // return since the only argument didn't fulfill the conditions
      return false;
  }

  return isAcceptableArgOverloadSets(call, bestFn, 2);
}

static void reportHijackingError(CallExpr* call,
                                 BlockStmt* searchScope,
                                 FnSymbol* bestFn, ModuleSymbol* bestMod,
                                 FnSymbol* candFn, ModuleSymbol* candMod)
{
  USR_FATAL_CONT(call, "multiple overload sets are applicable to this call");

  if (isMoreVisible(searchScope, candFn, bestFn) == FOUND_F1_FIRST)
  {
    USR_PRINT(candFn, "instead of the candidate here");
    USR_PRINT(candMod, "... defined in this closer module");
    USR_PRINT(bestFn, "the best-matching candidate is here");
    USR_PRINT(bestMod, "... in this farther-away module");
  }
  else
  {
    USR_PRINT(bestFn, "the best-matching candidate is here");
    USR_PRINT(bestMod, "... defined in this module");
    USR_PRINT(candFn, "even though the candidate here is also available");
    USR_PRINT(candMod, "... defined in this module");
  }

  USR_PRINT(call, "use --no-overload-sets-checks to disable overload sets errors");
  USR_STOP();
}

static bool overloadSetsOK(CallExpr* call,
                           BlockStmt* searchScope,
                           check_state_t checkState,
                           Vec<ResolutionCandidate*>& candidates,
                           ResolutionCandidate* bestRef,
                           ResolutionCandidate* bestCref,
                           ResolutionCandidate* bestVal)
{
  if (!fOverloadSetsChecks) return true;

  ModuleSymbol* bestRefMod  = overloadSetModule(bestRef);
  ModuleSymbol* bestCrefMod = overloadSetModule(bestCref);
  ModuleSymbol* bestValMod  = overloadSetModule(bestVal);

  // Ensure these "best" modules, if any, are consistent.
  if (! bestModulesAreConsistent(call, checkState,
                                 bestRefMod,  bestRef,  "ref",
                                 bestCrefMod, bestCref, "const ref") ||
      ! bestModulesAreConsistent(call, checkState,
                                 bestRefMod,  bestRef,  "ref",
                                 bestValMod,  bestVal,  "value")     ||
      ! bestModulesAreConsistent(call, checkState,
                                 bestCrefMod, bestCref, "const ref",
                                 bestValMod,  bestVal,  "value")     )
    return false;

  FnSymbol* bestFn = bestRefMod  ? bestRef->fn  :
                      bestCrefMod ? bestCref->fn :
                       bestValMod  ? bestVal->fn  : NULL;
  ModuleSymbol* bestMod = bestRefMod ? bestRefMod :
                           bestCrefMod ? bestCrefMod :
                            bestValMod;
  if (bestMod == NULL)
    // No "best" candidates, so nothing to check. Or, all "best" candidates
    // are from internal modules, so they will always be "more visible".
    return true;

  forv_Vec(ResolutionCandidate, candidate, candidates) {
    if (candidate == bestRef || candidate == bestCref || candidate == bestVal)
      continue; // do not check the "best" candidate against itself

    ModuleSymbol* candMod = overloadSetModule(candidate);
    if (candMod && candMod != bestMod                                       &&
        isMoreVisible(searchScope, bestFn, candidate->fn) != FOUND_F1_FIRST &&
        ! isAcceptableMethodChoice(call, bestFn, candidate->fn, candidates) )
    {
      if (checkState == CHECK_NORMAL_CALL)
        reportHijackingError(call, searchScope,
                             bestFn, bestMod, candidate->fn, candMod);
      return false;
    }
  }

  // All checks passed.
  return true;
}


static FnSymbol* resolveForwardedCall(CallInfo& info, check_state_t checkState);
static bool typeUsesForwarding(Type* t);

static FnSymbol* resolveNormalCall(CallInfo& info, check_state_t checkState) {
  Vec<FnSymbol*>            mostApplicable;
  Vec<ResolutionCandidate*> candidates;

  ResolutionCandidate*      bestRef    = NULL;
  ResolutionCandidate*      bestCref   = NULL;
  ResolutionCandidate*      bestVal    = NULL;

  VisibilityInfo            visInfo(info);
  int                       numMatches = 0;

  FnSymbol*                 retval     = NULL;

  BlockStmt* scopeUsed = nullptr;

  scopeUsed = findVisibleFunctionsAndCandidates(info, visInfo,
                                                mostApplicable, candidates);

  numMatches = disambiguateByMatch(info, scopeUsed, candidates,
                                   bestRef, bestCref, bestVal);

  if (checkState == CHECK_NORMAL_CALL && numMatches > 0 && visInfo.inPOI())
    updateCacheInfosForACall(visInfo,
                             bestRef, bestCref, bestVal);

  // If no candidates were found and it's a method, try forwarding
  if (candidates.n                  == 0 &&
      info.call->numActuals()       >= 1 &&
      info.call->get(1)->typeInfo() == dtMethodToken &&
      isUnresolvedSymExpr(info.call->baseExpr)) {
    Type* receiverType = canonicalDecoratedClassType(info.call->get(2)->getValType());
    if (typeUsesForwarding(receiverType)) {
      FnSymbol* fn = resolveForwardedCall(info, checkState);
      if (fn) {
        return fn;
      }
      // otherwise error is printed below
    }
  }

  if (numMatches > 0) {
    if (! overloadSetsOK(info.call, scopeUsed, checkState, candidates,
                         bestRef, bestCref, bestVal))
      return NULL; // overloadSetsOK() found an error

    recordCGInterimInstantiations(info.call, bestRef, bestCref, bestVal);
  }

  if (numMatches == 0) {
    if (info.call->partialTag == false) {
      if (checkState == CHECK_NORMAL_CALL) {
        if (candidates.n == 0) {
          bool existingErrors = fatalErrorsEncountered();
          printResolutionErrorUnresolved(info, mostApplicable);

          if (!inGenerousResolutionForErrors()) {
            startGenerousResolutionForErrors();
            FnSymbol* retry = resolveNormalCall(info, CHECK_CALLABLE_ONLY);
            stopGenerousResolutionForErrors();

            if (fIgnoreNilabilityErrors && existingErrors == false && retry)
              clearFatalErrors();

            if (retry != NULL)
              return retry;
          }

          // TODO: we could try e.g. removing the bad call
          // and checking other functions instead of giving up here.
          USR_STOP();

        } else {
          printResolutionErrorAmbiguous (info, candidates);
        }
      }
    }

  } else if (numMatches == 1) {
    ResolutionCandidate* best = NULL;

    if        (bestRef  != NULL) {
      best = bestRef;

    } else if (bestVal  != NULL) {
      best = bestVal;

    } else if (bestCref != NULL) {
      best = bestCref;
    }

    retval = resolveNormalCall(info, checkState, best);

  } else {
    retval = resolveNormalCall(info, checkState, bestRef, bestCref, bestVal);
  }

  forv_Vec(ResolutionCandidate*, candidate, candidates) {
    delete candidate;
  }

  if (retval) {
    retval->maybeGenerateDeprecationWarning(info.call);
    retval->maybeGenerateUnstableWarning(info.call);
  }

  return retval;
}

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   check_state_t        checkState,
                                   ResolutionCandidate* best) {
  CallExpr* call   = info.call;
  FnSymbol* retval = NULL;

  instantiateBody(best->fn);

  if (explainCallLine != 0 && explainCallMatch(call) == true) {
    USR_PRINT(best->fn, "best candidate is: %s", toString(best->fn));
  }

  if (call->partialTag                  == false ||
      best->fn->hasFlag(FLAG_NO_PARENS) == true) {
    retval = wrapAndCleanUpActuals(best, info, true);

    if (checkState == CHECK_NORMAL_CALL &&
        retval->name == astrSassign) {
      int lhsNum = 1;
      int rhsNum = 2;

      if (retval->hasFlag(FLAG_METHOD)) {
        // Adjust check for operator methods, when necessary
        lhsNum += 2;
        rhsNum += 2;
      }

      if (isRecord(retval->getFormal(lhsNum)->type) == true &&
          retval->getFormal(rhsNum)->type == dtNil) {
        USR_FATAL(userCall(call),
                  "type mismatch in assignment from nil to %s",
                  toString(retval->getFormal(lhsNum)->type));
      }
    }

    SET_LINENO(call);

    if (call->partialTag == true) {
      call->partialTag = false;
    }

    call->baseExpr->replace(new SymExpr(retval));

    resolveNormalCallConstRef(call);

    if (checkState == CHECK_NORMAL_CALL) {
      resolveNormalCallFinalChecks(call);
    }
  }

  return retval;
}

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   check_state_t        checkState,
                                   ResolutionCandidate* bestRef,
                                   ResolutionCandidate* bestConstRef,
                                   ResolutionCandidate* bestValue) {
  CallExpr*            call         = info.call;
  CallExpr*            refCall      = NULL;
  CallExpr*            valueCall    = NULL;
  CallExpr*            constRefCall = NULL;
  CallExpr*            ccAnchor     = NULL; // the last of the above CallExprs
  ResolutionCandidate* best         = NULL;
  FnSymbol*            retval       = NULL;

  if (bestRef      != NULL) {
    refCall = call;

    instantiateBody(bestRef->fn);
    ccAnchor = refCall;
  }

  if (bestValue    != NULL) {
    if (bestRef == NULL) {
      valueCall = call;

    } else {
      valueCall = call->copy();

      call->insertAfter(valueCall);
    }

    instantiateBody(bestValue->fn);
    ccAnchor = valueCall;
  }

  if (bestConstRef != NULL) {
    constRefCall = call->copy();

    call->insertAfter(constRefCall);

    instantiateBody(bestConstRef->fn);

    if (valueCall == nullptr || valueCall == call) {
      ccAnchor = constRefCall;
    } else {
      ccAnchor = valueCall;
    }
  }

  if        (bestRef      != NULL) {
    best = bestRef;

  } else if (bestValue    != NULL) {
    best = bestValue;

  } else if (bestConstRef != NULL) {
    best = bestConstRef;
  }

  if (explainCallLine != 0 && explainCallMatch(call) == true) {
    USR_PRINT(best->fn, "best candidate is: %s", toString(best->fn));
  }

  if (call->partialTag                  == true &&
      best->fn->hasFlag(FLAG_NO_PARENS) == false) {
    if (valueCall    != NULL) valueCall->remove();
    if (constRefCall != NULL) constRefCall->remove();

  } else {
    wrapAndCleanUpActuals(best, info, true);

    if (valueCall != NULL && valueCall != call) {
      CallInfo tmpInfo;

      if (tmpInfo.isWellFormed(valueCall) == true) {
        wrapAndCleanUpActuals(bestValue, tmpInfo, false);

      } else {
        if (checkState == CHECK_NORMAL_CALL) {
          tmpInfo.haltNotWellFormed();
        }
      }
    }

    if (constRefCall != NULL) {
      CallInfo tmpInfo;

      if (tmpInfo.isWellFormed(constRefCall) == true) {
        wrapAndCleanUpActuals(bestConstRef, tmpInfo, false);

      } else {
        if (checkState == CHECK_NORMAL_CALL) {
          tmpInfo.haltNotWellFormed();
        }
      }
    }

    if (call->partialTag == true) {
      call->partialTag = false;
    }

    if (checkState == CHECK_NORMAL_CALL) {
      if (best->fn->name                         == astrSassign &&
          isRecord(best->fn->getFormal(1)->type) == true        &&
          best->fn->getFormal(2)->type           == dtNil) {
        USR_FATAL(userCall(call),
                  "type mismatch in assignment from nil to %s",
                  toString(best->fn->getFormal(1)->type));
      }
    }

    SET_LINENO(call);

    if (refCall      != NULL) {
      refCall->baseExpr->replace(new SymExpr(bestRef->fn));
    }

    if (valueCall    != NULL) {
      valueCall->baseExpr->replace(new SymExpr(bestValue->fn));
    }

    if (constRefCall != NULL) {
      constRefCall->baseExpr->replace(new SymExpr(bestConstRef->fn));
    }

    // Replace the call with a new ContextCallExpr containing 2 or 3 calls
    ContextCallExpr* contextCall = new ContextCallExpr();

    ccAnchor->insertAfter(contextCall);

    if (refCall      != NULL) refCall->remove();
    if (valueCall    != NULL) valueCall->remove();
    if (constRefCall != NULL) constRefCall->remove();

    contextCall->setRefValueConstRefOptions(refCall,
                                            valueCall,
                                            constRefCall);

    if (checkState == CHECK_NORMAL_CALL) {
      resolveNormalCallFinalChecks(call);
    }

    retval = best->fn;
  }

  return retval;
}

static void resolveNormalCallConstRef(CallExpr* call) {
  FnSymbol* fn = call->resolvedFunction();

  if (fn->retTag == RET_CONST_REF) {
    if (CallExpr* parentCall = toCallExpr(call->parentExpr)) {
      if (parentCall->isPrimitive(PRIM_MOVE) == true) {
        if (SymExpr* lhsSe = toSymExpr(parentCall->get(1))) {
          if (VarSymbol* lhs = toVarSymbol(lhsSe->symbol())) {
            if (lhs->hasFlag(FLAG_EXPR_TEMP) == true) {
              Symbol* parentSymbol = parentCall->parentSymbol;

              if (FnSymbol* inFn = toFnSymbol(parentSymbol)) {
                if (isConstructorLikeFunction(inFn) == false) {
                  lhs->addFlag(FLAG_REF_TO_CONST);
                }
              }
            }
          }
        }
      }
    }
  }
}

// Returns the element type, given an array type.
static Type* arrayElementType(AggregateType* arrayType) {
  Type* eltType = NULL;
  INT_ASSERT(arrayType->symbol->hasFlag(FLAG_ARRAY));
  Type* instType = arrayType->getField("_instance")->type;
  AggregateType* instClass = toAggregateType(canonicalClassType(instType));
  eltType = instClass->getField("eltType")->getValType();
  return eltType;
}

// Returns the element type, given an array type.
// Recurse into it if it is still an array.
static Type* finalArrayElementType(AggregateType* arrayType) {
  Type* eltType = NULL;
  do {
    eltType = arrayElementType(arrayType);
    arrayType = toAggregateType(eltType);
  } while (arrayType != NULL && arrayType->symbol->hasFlag(FLAG_ARRAY));

  return eltType;
}

// Is it OK to default-initialize an array with this element type?
static bool okForDefaultInitializedArray(Type* eltType) {
  return isDefaultInitializable(eltType);
}

// Is 'actualSym' passed to an assignment (a 'proc =') ?
static bool isAssignedTo(Symbol* actualSym, SymExpr* knownRef) {
  for_SymbolUses(use, actualSym)
    if (use != knownRef)
      if (CallExpr* parent = toCallExpr(use->parentExpr))
        if (parent->isNamedAstr(astrSassign))
          if (use == parent->get(1))
            return true;

  return false; // assignment is not found
}

// Does the corresponding formal has a default expression?
static bool corrFormalHasDefaultExpr(SymExpr* actualSE) {
  ArgSymbol* formal = actual_to_formal(actualSE);
  // Determine the presence of a default expression by checking
  // whether defaultExpr is not _typeDefaultT.
  // Similar to defaultedFormalUsesDefaultForType().
  AList& defaultExprBody = formal->defaultExpr->body;
  if (defaultExprBody.length == 1)
    if (SymExpr* bodySE = toSymExpr(defaultExprBody.get(1)))
      if (bodySE->symbol() == gTypeDefaultToken)
        return false;
  return true;
}

static const char* userFieldNameForError(Symbol* actualSym) {
  if (strncmp(actualSym->name, "default_arg_", 12) == 0)
    return actualSym->name + 12;
  else
    return actualSym->name;
}

//
// This checks for an array-typed field with a non-nilable element type
// that is default-initialized in a compiler-generated initializer.
// Ex. C1.A1 in
//   test/classes/nilability/array-with-nonnilable-elttype.chpl
// This case is represented in the AST with a default-arg function
// being passed to the initializer.
//
static void checkDefaultNonnilableArrayArg(CallExpr* call, FnSymbol* fn) {
  if (! (fn->name == astrInit || fn->name == astrInitEquals ||
         fn->hasFlag(FLAG_NEW_WRAPPER)                      ))
    return; // nothing to do

  for_formals_actuals(formal, actualExpr, call)
   if (SymExpr* actualSE = toSymExpr(actualExpr))
    if (Symbol* actualSym = actualSE->symbol())
     if (actualSym->hasFlag(FLAG_DEFAULT_ACTUAL) &&
         ! formal->hasFlag(FLAG_UNSAFE))
      if (AggregateType* actualType = toAggregateType(actualSym->getValType()))
       if (actualType->symbol->hasFlag(FLAG_ARRAY) &&
           ! okForDefaultInitializedArray(finalArrayElementType(actualType)))
        //
        // Acceptable handling of the default actual is this:
        //   def default_arg_xxx: _array(...)
        //   move( default_arg_xxx call( fn _new_default_xxx ) )
        //   call( fn = default_arg_xxx <whatever> )
        //   move( new_temp call( fn _new default_arg_xxx ) )
        //
        // The call( fn = ... ) establishes the non-default value.
        // It is an error if that call is missing.
        //
        if (! isAssignedTo(actualSym, actualSE) &&
            ! corrFormalHasDefaultExpr(actualSE))
         USR_FATAL_CONT(call, "cannot default-initialize the array field"
                        " %s because it has a non-nilable element type '%s'",
                        userFieldNameForError(actualSym),
                        toString(finalArrayElementType(actualType), true));
}

static void resolveNormalCallFinalChecks(CallExpr* call) {
  FnSymbol* fn = call->resolvedFunction();

  if (fn->hasFlag(FLAG_MODIFIES_CONST_FIELDS) == true) {
    // Not allowed if it is not called directly from a constructor.
    if (isInConstructorLikeFunction(call)                     == false ||
        getBaseSymForConstCheck(call)->hasFlag(FLAG_ARG_THIS) == false) {
      USR_FATAL_CONT(call,
                     "illegal call to %s() - it modifies 'const' fields "
                     "of 'this', therefore it can be invoked only directly "
                     "from a constructor on the object being constructed",
                     fn->name);
    }
  }

  lvalueCheck(call);

  checkForStoringIntoTuple(call, fn);

  checkDefaultNonnilableArrayArg(call, fn);

  resolveNormalCallCompilerWarningStuff(call, fn);
}

static FnSymbol* wrapAndCleanUpActuals(ResolutionCandidate* best,
                                       CallInfo&            info,
                                       bool                 followerChecks) {
  if (best->fn->isInitializer()) {
    AggregateType* at = toAggregateType(best->fn->_this->getValType());
    if (isRecord(at)) {
      // If we allow an infinite record's initializer to be processed for
      // default values, we will enter an infinite loop and the compiler will
      // crash.
      checkForInfiniteRecord(at);
    }
  }
  best->fn = wrapAndCleanUpActuals(best->fn,
                                   info,
                                   best->actualIdxToFormal,
                                   followerChecks);

  return best->fn;
}

// Reissue compiler warning or error messages
static void reissueMsgs(FnSymbol* resolvedFn,
                        std::map<FnSymbol*, const char*>& innerMap,
                        std::map<FnSymbol*, const char*>& outerMap,
                        bool err) {
  std::map<FnSymbol*, const char*>::iterator it;
  CallExpr *from1 = NULL, *from2 = NULL;
  const char *str1 = NULL, *str2 = NULL;

  it = innerMap.find(resolvedFn);
  if (it != innerMap.end()) {
    str1 = it->second;
    from1 = reissueCompilerWarning(it->second, 2, err);

    if (callStack.n >= 2) {
      if (FnSymbol* fn = callStack.v[callStack.n - 2]->resolvedFunction()) {
        outerMap[fn] = it->second;
      }
    }
  }

  it = outerMap.find(resolvedFn);
  if (it != outerMap.end()) {
    str2 = it->second;
    from2 = reissueCompilerWarning(it->second, 1, err);
  }

  // avoid duplicates and "This source location is a guess."
  if (str1 || str2) {
    if ((str1 == str2) && !err && (!from1 || !from2)) {
      if (from1) reissueMsgHelp(from1, str1, err);
      else       reissueMsgHelp(from2, str2, err);

    } else {
      if (str1) reissueMsgHelp(from1, str1, err);
      if (str2) reissueMsgHelp(from2, str2, err);
    }
  }
}

void resolveNormalCallCompilerWarningStuff(CallExpr* call,
                                           FnSymbol* resolvedFn) {
  reissueMsgs(resolvedFn, innerCompilerErrorMap, outerCompilerErrorMap, true);
  reissueMsgs(resolvedFn, innerCompilerWarningMap, outerCompilerWarningMap, false);

  try_resolve_map_t::iterator it;
  it = tryResolveErrors.find(resolvedFn);
  if (it != tryResolveErrors.end()) {
    if (inTryResolve > 0 && tryResolveFunctions.size() > 0) {
      FnSymbol* fn = tryResolveFunctions.back();
      tryResolveErrors[fn] = it->second;
    } else if (resolvedFn && resolvedFn->hasFlag(FLAG_ERRONEOUS_COPY)) {
      // error will be reported later (in callDestructors) if
      // copy is not eliminated by then.
    } else {
      BaseAST* from = it->second.first;
      const char* err = it->second.second;
      FnSymbol* inFn = call->getFunction();
      bool inCopyIsh = false;
      if (inFn != NULL) {
        inCopyIsh = inFn->hasFlag(FLAG_INIT_COPY_FN) ||
                    inFn->hasFlag(FLAG_AUTO_COPY_FN) ||
                    inFn->name == astrInitEquals;
      }
      if (inCopyIsh) {
        inFn->addFlag(FLAG_ERRONEOUS_COPY);
        tryResolveErrors[inFn] = std::make_pair(from,err);
      } else {
        USR_FATAL_CONT(from, "%s", err);
        USR_PRINT(call, "in function called here");
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void generateUnresolvedMsg(CallInfo& info, Vec<FnSymbol*>& visibleFns);
static bool maybeIssueSplitInitMissingTypeError(CallInfo& info,
                                                Vec<FnSymbol*>& visibleFns);
static void sortExampleCandidates(CallInfo& info,
                                  Vec<FnSymbol*>& visibleFns);
static bool defaultValueMismatch(CallInfo& info);

void printResolutionErrorUnresolved(CallInfo&       info,
                                    Vec<FnSymbol*>& visibleFns) {
  if (info.call == NULL) {
    INT_FATAL("call is NULL");

  } else {
    CallExpr* call = userCall(info.call);

    if (maybeIssueSplitInitMissingTypeError(info, visibleFns)) {
      // don't show additional unresolved error in addition to missing init
    } else
    if (call->isCast() == true) {
      if (info.actuals.tail()->hasFlag(FLAG_TYPE_VARIABLE) == false) {
        USR_FATAL_CONT(call, "illegal cast to non-type");
      } else {
        Type* srcType = info.actuals.v[0]->type;
        Type* dstType = info.actuals.v[1]->type;
        EnumType* dstEnumType = toEnumType(dstType);
        EnumType* srcEnumType = toEnumType(srcType);
        if (srcEnumType && srcEnumType->isAbstract()) {
          USR_FATAL_CONT(call,
                         "can't cast from an abstract enum ('%s') to %s",
                         toString(srcType),
                         toString(dstType));
        } else if (dstEnumType && dstEnumType->isAbstract()) {
          USR_FATAL_CONT(call,
                         "can't cast from %s to an abstract enum type ('%s')",
                         toString(srcType),
                         toString(dstType));
        } else {
          USR_FATAL_CONT(call,
                         "illegal cast from %s to %s",
                         toString(srcType),
                         toString(dstType));
        }
      }

    } else if (strcmp("these", info.name) == 0) {
      if (info.actuals.n          == 2 &&
          info.actuals.v[0]->type == dtMethodToken) {

        if (info.actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE) == true) {
          USR_FATAL_CONT(call,
                         "unable to iterate over type '%s'",
                         toString(info.actuals.v[1]->type));
        } else {
          USR_FATAL_CONT(call,
                         "cannot iterate over values of type %s",
                         toString(info.actuals.v[1]->type));
        }

      } else {
        generateUnresolvedMsg(info, visibleFns);
      }

    } else if (info.name == astrSassign) {

      if        (info.actuals.v[0]                              !=  NULL  &&
                 info.actuals.v[1]                              !=  NULL  &&
                 info.actuals.v[0]->hasFlag(FLAG_TYPE_VARIABLE) == false  &&
                 info.actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE) ==  true) {
        USR_FATAL_CONT(call,
                       "illegal assignment of type to value");

      } else if (info.actuals.v[0]                              != NULL   &&
                 info.actuals.v[1]                              != NULL   &&
                 info.actuals.v[0]->hasFlag(FLAG_TYPE_VARIABLE) == true) {
        USR_FATAL_CONT(call,
                       "illegal assignment to type");

      } else if (info.actuals.v[1]->type == dtNil) {

        bool handled = defaultValueMismatch(info);

        if (!handled) {
          USR_FATAL_CONT(call,
                         "Cannot assign to %s from nil",
                         toString(info.actuals.v[0]->type));
        }

      } else if (info.actuals.v[0]->hasFlag(FLAG_RVV) ||
                 info.actuals.v[0]->hasFlag(FLAG_YVV)) {
        const char* retKind =
          info.actuals.v[0]->hasFlag(FLAG_RVV) ? "return" : "yield";
        USR_FATAL_CONT(call,
                       "cannot %s '%s' when the declared return type is '%s'",
                       retKind,
                       toString(info.actuals.v[1]->type),
                       toString(info.actuals.v[0]->type));

      } else {
        bool handled = defaultValueMismatch(info);

        if (!handled) {
          USR_FATAL_CONT(call,
                         "Cannot assign to %s from %s",
                         toString(info.actuals.v[0]->type),
                         toString(info.actuals.v[1]->type));
        }
      }

    } else if (info.name == astr_coerceCopy || info.name == astr_coerceMove) {

        USR_FATAL_CONT(call,
                       "cannot initialize %s from %s",
                       toString(info.actuals.v[0]->type),
                       toString(info.actuals.v[1]->type));

    } else if (info.name == astrThis) {
      Type* t = info.actuals.v[1]->getValType();

      if (t->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
        USR_FATAL_CONT(call,
                       "illegal access of iterator or promoted expression");

      } else if (t->symbol->hasFlag(FLAG_FUNCTION_CLASS)) {
        const char* str = "first class function";

        // Create an error sink, one per each category of kind of function
        // type (though in practice, right now only 'PROC' will be used.
        // We have to propagate some sort of type so that resolution can
        // continue as far as possible without repeated errors.
        if (t == fcfs::errorSink(FunctionType::OPERATOR)->type) {
          str = "operator";
        } else if (t == fcfs::errorSink(FunctionType::ITER)->type) {
          str = "iterator";
        } else if (t == fcfs::errorSink(FunctionType::PROC)->type) {
          str = "procedure";
        }

        USR_FATAL_CONT(call, "illegal access of %s", str);

      } else {
        USR_FATAL_CONT(call,
                       "unresolved access of '%s' by '%s'",
                       toString(info.actuals.v[1]->type),
                       info.toString());
      }

    } else {
      generateUnresolvedMsg(info, visibleFns);
    }

    if (developer == true) {
      USR_PRINT(call, "unresolved call had id %i", call->id);
    }
  }
}

struct ExampleCandidateComparator {
  CallInfo& info;

  ExampleCandidateComparator(CallInfo& info) : info(info) { }

  bool operator() (FnSymbol* aFn, FnSymbol* bFn) {
    // Return true if aFn is better than bFn
    bool aObv = obviousMismatch(info.call, aFn);
    bool bObv = obviousMismatch(info.call, bFn);
    if (!aObv && bObv) return true;
    if (aObv && !bObv) return false;
    // if aObv == bObv, continue

    bool ret = false;
    ResolutionCandidate* a = new ResolutionCandidate(aFn);
    ResolutionCandidate* b = new ResolutionCandidate(bFn);

    a->isApplicable(info, NULL);
    b->isApplicable(info, NULL);

    if (failedCandidateIsBetterMatch(a, b))
      ret = true;

    delete a;
    delete b;

    return ret;
  }
};

static void sortExampleCandidates(CallInfo& info,
                                  Vec<FnSymbol*>& visibleFns)
{
  ExampleCandidateComparator cmp(info);
  // Try to sort them so that the more relevant candidates are first.
  std::stable_sort(&visibleFns.v[0], &visibleFns.v[visibleFns.n], cmp);
}

static bool defaultValueMismatch(CallInfo& info) {
  bool handled = false;
  CallExpr* call = info.call;

  if (FnSymbol* inFn = call->getFunction()) {
    if (inFn->hasFlag(FLAG_DEFAULT_ACTUAL_FUNCTION)) {
      // Look in the function for a FLAG_USER_VARIABLE_NAME variable
      // for better error reporting
      VarSymbol* userVariable = NULL;

      for_alist(expr, inFn->body->body) {
        if (DefExpr* def = toDefExpr(expr))
          if (VarSymbol* definedVar = toVarSymbol(def->sym))
            if (definedVar->hasFlag(FLAG_USER_VARIABLE_NAME))
              if (definedVar->getValType() ==
                  info.actuals.v[0]->getValType())
                userVariable = definedVar;
      }

      if (userVariable != NULL) {
        Type* formalType = userVariable->getValType();
        Type* actualType = info.actuals.v[1]->getValType();

        if (isNonNilableClassType(formalType) &&
            actualType == dtNil) {
          USR_FATAL_CONT(call, "cannot initialize %s of non-nilable type '%s' from nil",
                         userVariable->name,
                         toString(formalType));
        } else if (isNonNilableClassType(formalType) &&
            isNilableClassType(actualType)) {
          USR_FATAL_CONT(call, "cannot initialize %s of non-nilable type '%s' from a nilable '%s'",
                         userVariable->name,
                         toString(formalType), toString(actualType));
        } else {
          USR_FATAL_CONT(call, "cannot initialize '%s' of type %s from a '%s'",
                         userVariable->name,
                         toString(formalType), toString(actualType));
        }
        handled = true;
      }
    }
  }
  return handled;
}

void printResolutionErrorAmbiguous(CallInfo&                  info,
                                   Vec<ResolutionCandidate*>& candidates) {
  CallExpr* call       = userCall(info.call);
  bool      printedOne = false;

  if (info.name == astrThis) {
    USR_FATAL_CONT(call,
                   "ambiguous access of '%s' by '%s'",
                   toString(info.actuals.v[1]->type),
                   info.toString());

  } else {
    const char* entity = "call";
    const char* str    = info.toString();

    if (info.scope) {
      ModuleSymbol* mod = toModuleSymbol(info.scope->parentSymbol);

      INT_ASSERT(mod);

      str = astr(mod->name, ".", str);
    }

    USR_FATAL_CONT(call, "ambiguous %s '%s'", entity, str);
  }

  if (developer == true) {
    for (int i = callStack.n - 1; i >= 0; i--) {
      CallExpr* cs = callStack.v[i];
      FnSymbol* f  = cs->getFunction();

      if (f->instantiatedFrom) {
        USR_PRINT(callStack.v[i], "  instantiated from %s", f->name);
      } else {
        break;
      }
    }
  }

  int nFnsWithOutIntentFormals = 0;
  forv_Vec(ResolutionCandidate, cand, candidates) {
    if (printedOne == false) {
      USR_PRINT(cand->fn, "candidates are: %s", toString(cand->fn));
      printedOne = true;

    } else {
      USR_PRINT(cand->fn, "                %s", toString(cand->fn));
    }

    bool hasOutFormal = false;
    for_formals(arg, cand->fn) {
      if (arg->originalIntent == INTENT_OUT)
        hasOutFormal = true;
    }
    if (hasOutFormal)
      nFnsWithOutIntentFormals++;
  }


  // This condition is not exact
  // (the ambiguity could be due to something other than the out formals).
  if (nFnsWithOutIntentFormals == candidates.n) {
    USR_PRINT(call, "out-intent formals do not participate in overload resolution");
  }

  if (developer == true) {
    USR_PRINT(call, "unresolved call had id %i", call->id);
  }

  USR_STOP();
}

static bool isMethodPreResolve(CallExpr* call) {
  bool result = false;

  if (call->numActuals() >= 2)
    if (SymExpr* se = toSymExpr(call->get(1)))
      result = se->symbol() == gMethodToken;

  return result;
}

static bool isInitEqualsPreResolve(CallExpr* call) {
  bool result = false;

  if (isMethodPreResolve(call))
    if (call->isNamedAstr(astrInitEquals))
      result = true;

  return result;
}

// currently, this returns 'true' for cases when a call is a method
// call (e.g., 'r.foo()' and the function is a standalone function
// (e.g., 'foo()') or vice-versa.  With additional work, other
// "obvious" mismatches could be eliminated, such as if the call and
// function are both methods but in disjoint class hierarchies.
//
static bool obviousMismatch(CallExpr* call, FnSymbol* fn) {
  bool isMethodCall = isMethodPreResolve(call);
  bool isMethodFn = fn->_this != NULL;

  return (isMethodCall != isMethodFn);
}

static bool isSymExprTypeVar(Expr* e) {
  if (SymExpr* se = toSymExpr(e)) {
    return se->symbol()->hasFlag(FLAG_TYPE_VARIABLE);
  }

  return false;
}

// returns true if the error was issued
static bool maybeIssueSplitInitMissingTypeError(CallInfo& info,
                                                Vec<FnSymbol*>& visibleFns) {
  // Check for uninitialized values (with type dtSplitInitType)
  bool foundUnknownTypeActual = false;
  for_actuals(actual, info.call) {
    bool isTypeVariable = isSymExprTypeVar(actual);
    Type* t = actual->getValType();
    if (t == dtSplitInitType ||
        (!isTypeVariable && t->symbol->hasFlag(FLAG_GENERIC))) {
      foundUnknownTypeActual = true;
    }
  }

  if (foundUnknownTypeActual) {
    // Are all of the failures due to not having an established type?
    bool anyTypeNotEstablished = false;
    forv_Vec(FnSymbol, fn, visibleFns) {
      ResolutionCandidate* rc = new ResolutionCandidate(fn);
      rc->isApplicable(info, NULL);

      if (rc->reason == RESOLUTION_CANDIDATE_ACTUAL_TYPE_NOT_ESTABLISHED) {
        anyTypeNotEstablished = true;
      }
      delete rc;
    }

    if (anyTypeNotEstablished) {
      bool printedError = false;
      for_actuals(actual, info.call) {
        bool isTypeVariable = isSymExprTypeVar(actual);
        Type* t = actual->getValType();
        if (t == dtSplitInitType ||
            (!isTypeVariable && t->symbol->hasFlag(FLAG_GENERIC))) {
          if (SymExpr* se = toSymExpr(actual)) {
            CallExpr* call = userCall(info.call);
            splitInitMissingTypeError(se->symbol(), call, false);
            printedError = true;
          }
        }
      }
      if (printedError)
        return true; // don't print out candidates - probably irrelevant
    }
  }
  return false;
}

static void generateUnresolvedMsg(CallInfo& info, Vec<FnSymbol*>& visibleFns) {
  CallExpr*   call = userCall(info.call);
  const char* str  = NULL;

  if (info.scope != NULL) {
    ModuleSymbol* mod = toModuleSymbol(info.scope->parentSymbol);

    INT_ASSERT(mod);

    str = astr(mod->name, ".", info.toString());

  } else {
    str = info.toString();
  }

  if (info.actuals.n                              >  1             &&
      info.actuals.v[0]->getValType()             == dtMethodToken &&
      isEnumType(info.actuals.v[1]->getValType()) == true) {
    USR_FATAL_CONT(call,
                   "unresolved enumerated type symbol or call '%s'",
                   str);
  } else if (isInitEqualsPreResolve(call)) {
    INT_ASSERT(info.actuals.v[0]->getValType() == dtMethodToken);
    INT_ASSERT(info.actuals.n == 3);

    Type* receiverType = info.actuals.v[1]->getValType();
    Type* exprType = info.actuals.v[2]->getValType();

    USR_FATAL_CONT(call, "could not find a copy initializer ('%s') "
                         "for type '%s' from type '%s'",
                         astrInitEquals,
                         receiverType->symbol->name,
                         exprType->symbol->name);
  } else {
    USR_FATAL_CONT(call, "unresolved call '%s'", str);
  }

  if (visibleFns.n > 0) {
    if (developer == true) {
      for (int i = callStack.n - 1; i >= 0; i--) {
        CallExpr* cs = callStack.v[i];
        FnSymbol* f  = cs->getFunction();

        if (f->instantiatedFrom != NULL) {
          USR_PRINT(callStack.v[i], "  instantiated from %s", f->name);
        } else {
          break;
        }
      }
    }

    sortExampleCandidates(info, visibleFns);

    int nPrintDetails = 1;
    int nPrint = 3; // unused if fPrintAllCandidates

    Vec<FnSymbol*> filteredFns;
    if (fPrintAllCandidates || visibleFns.n <= nPrint + 1) {
      // Don't do any filtering; we're going to print everything
      filteredFns = visibleFns;
    } else {
      // If there is more than one visible function and we haven't
      // been asked to print all candidates, let's try to eliminate
      // "worse" matches (e.g., method call / standalone function call
      // mismatches), copying over the first nPrint functions that
      // aren't obvious mismatches.
      int i = 0;
      forv_Vec(FnSymbol, fn, visibleFns) {
        if (!obviousMismatch(call, fn)) {
          i++;
          filteredFns.add(fn);
          if (i == nPrint) break;
        }
      }

      // If the filtered functions list is empty (implying everything
      // was an obvious mismatch), then let's copy over the first
      // nPrint visibleFns as our filtered functions, to avoid
      // suggesting that there are no other candidates.
      //
      if (filteredFns.n == 0) {
        for (int i = 0; i < nPrint; i++) {
          filteredFns.add(visibleFns.v[i]);
        }
      }
    }


    // Print our top candidate(s) and why they were rejected
    int nPrinted = 0; // how many candidates have we printed?
    bool printedOne = false;  // have we printed one "other candidate"?
    forv_Vec(FnSymbol, fn, filteredFns) {
      if (nPrinted < nPrintDetails) {
        explainCandidateRejection(info, fn);
      } else {
        if (printedOne == false) {
          USR_PRINT(call, "other candidates are:");
          printedOne = true;
        }
        USR_PRINT(fn, "  %s", toString(fn));
      }
      nPrinted++;
    }

    // Print indication of additional candidates, if any
    int numRemaining = visibleFns.n - nPrinted;
    if (numRemaining > 0) {
      USR_PRINT("%s %i other candidate%s, use --print-all-candidates to see %s",
                (printedOne ? "and" : ((numRemaining == 1) ?
                                       "there is also" :
                                       "there are also")),
                numRemaining,
                ((numRemaining == 1) ? "" : "s"),
                ((numRemaining == 1) ? "it" : "them"));
    }
  } else {
    USR_PRINT(call, "because no functions named %s found in scope", info.name);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

//
// We gather a list of last-resort candidates as we go.
// The last-resort candidates visible from the call are followed by a NULL
// to separate them from those visible from the point of instantiation.
//
typedef std::vector<FnSymbol*> LastResortCandidates;

// add a null separator
static void markEndOfPOI(LastResortCandidates& lrc) {
  lrc.push_back(NULL);
}

// do we have any LRCs to look at?
static bool haveAnyLRCs(LastResortCandidates& lrc, int poiDepth) {
  // discount (visInfo.poiDepth+1) nulls that are separators
  return (int)lrc.size() > (poiDepth + 1);
}

// do we have more LRCs to look at?
static bool haveMoreLRCs(LastResortCandidates& lrc, int numVisited) {
  return numVisited < (int)lrc.size();
}

static void filterCandidate (CallInfo&                  info,
                             VisibilityInfo&            visInfo,
                             FnSymbol*                  fn,
                             Vec<ResolutionCandidate*>& candidates);

static void gatherCandidates(CallInfo&                  info,
                             VisibilityInfo&            visInfo,
                             FnSymbol*                  fn,
                             Vec<ResolutionCandidate*>& candidates);

static void gatherCandidatesAndLastResort(CallInfo&     info,
                             VisibilityInfo&            visInfo,
                             Vec<FnSymbol*>&            visibleFns,
                             int&                       numVisited,
                             LastResortCandidates&      lrc,
                             Vec<ResolutionCandidate*>& candidates);

static void gatherLastResortCandidates(CallInfo&                  info,
                                       VisibilityInfo&            visInfo,
                                       LastResortCandidates&      lrc,
                                       int&                       numVisited,
                                       Vec<ResolutionCandidate*>& candidates);

static
void trimVisibleCandidates(CallInfo&       info,
                           Vec<FnSymbol*>& mostApplicable,
                           int&            numVisitedVis,
                           Vec<FnSymbol*>& visibleFns) {
  CallExpr* call = info.call;

  bool isMethod = isMethodPreResolve(call);

  bool isInit   = isMethod && (call->isNamedAstr(astrInit) || call->isNamedAstr(astrInitEquals));
  bool isNew    = call->numActuals() >= 1 && call->isNamedAstr(astrNew);
  bool isDeinit = isMethod && call->isNamedAstr(astrDeinit);

  // 3 actuals: _mt, 'this', thing-to-be-copied
  bool maybeCopyInit = isInit && call->numActuals() == 3 &&
                       call->get(2)->getValType() == call->get(3)->getValType();

  if (!(isInit || isNew || isDeinit) || info.call->isResolved()) {
   if (numVisitedVis == 0)
    mostApplicable = visibleFns;
   else
    // copy only new fns since last time
    for (int i = numVisitedVis; i < visibleFns.n; i++)
      mostApplicable.add(visibleFns.v[i]);
  } else {
    for (int i = numVisitedVis; i < visibleFns.n; i++) {
      FnSymbol* fn = visibleFns.v[i];
      bool shouldKeep = true;
      BaseAST* actual = NULL;
      BaseAST* formal = NULL;

      if ((fn->isInitializer()) || (isDeinit && fn->isMethod()) || fn->isCopyInit()) {
        actual = call->get(2);
        formal = fn->_this;
      } else if (isNew && (fn->hasFlag(FLAG_NEW_WRAPPER) || fn->isInitializer())) {
        actual = call->get(1);
        formal = toDefExpr(fn->formals.head)->sym;
      }

      if (actual != NULL && formal != NULL) {
        Type* at = canonicalDecoratedClassType(actual->getValType());
        Type* ft = canonicalDecoratedClassType(formal->getValType());

        AggregateType* actualType = toAggregateType(at)->getRootInstantiation();
        AggregateType* formalType = toAggregateType(ft)->getRootInstantiation();

        // Allow deinit to match dtObject's deinit so that it will at least
        // dispatch at some point. Without this check, the compiler would
        // fail to resolve a 'deinit' call for a concrete class declared in
        // another module. See the following for examples:
        //   - modules/diten/returnClassDiffModule*.chpl
        //   - classes/moduleScope/mod-init.chpl
        if (isDeinit && formalType == dtObject) {
          shouldKeep = true;
        } else if (actualType != formalType) {
          shouldKeep = false;
        }
      } else {
        shouldKeep = false;
      }

      // Default-initializers should not be considered for calls that look
      // like a copy-init
      if (shouldKeep && maybeCopyInit && fn->isDefaultInit()) {
        shouldKeep = false;
      }

      if (shouldKeep) {
        mostApplicable.add(fn);
      }
    }
  }

  numVisitedVis = visibleFns.n;
}

void trimVisibleCandidates(CallInfo&       info,
                           Vec<FnSymbol*>& mostApplicable,
                           Vec<FnSymbol*>& visibleFns) {
  int numVisitedVis = 0;
  trimVisibleCandidates(info, mostApplicable, numVisitedVis, visibleFns);
}

void advanceCurrStart(VisibilityInfo& visInfo) {
  INT_ASSERT((int)visInfo.instnPoints.size() == visInfo.poiDepth);
  if (visInfo.nextPOI != NULL)
    visInfo.instnPoints.push_back(visInfo.nextPOI);

  visInfo.currStart = visInfo.nextPOI;
  visInfo.nextPOI = NULL;
}

// Returns the POI scope used to find the candidates
static BlockStmt* findVisibleFunctionsAndCandidates(
                                CallInfo&                  info,
                                VisibilityInfo&            visInfo,
                                Vec<FnSymbol*>&            mostApplicable,
                                Vec<ResolutionCandidate*>& candidates) {
  CallExpr* call = info.call;
  FnSymbol* fn   = call->resolvedFunction();
  Vec<FnSymbol*> visibleFns;

  if (fn != NULL) {
    visibleFns.add(fn);
    mostApplicable.add(fn); // for better error reporting

    handleTaskIntentArgs(info, fn);

    // no need for trimVisibleCandidates() and findVisibleCandidates()
    gatherCandidates(info, visInfo, fn, candidates);

    explainGatherCandidate(info, candidates);

    return getVisibilityScope(call);
  }

  // CG TODO: pull all visible interface functions, if within a CG context

  // Keep *all* discovered functions in 'visibleFns' and 'mostApplicable'
  // so that we can revisit them for error reporting.
  // Keep track in 'numVisited*' of where we left off with the previous POI
  // to avoid revisiting those functions for the next POI.
  int numVisitedVis = 0, numVisitedMA = 0;
  LastResortCandidates lrc;
  PtrSet<BlockStmt*> visited;
  visInfo.currStart = getVisibilityScope(call);
  INT_ASSERT(visInfo.poiDepth == -1); // we have not used it
  BlockStmt* scopeUsed = nullptr;

  do {
    // CG TODO: no POI for CG functions
    visInfo.poiDepth++;

    findVisibleFunctions(info, &visInfo, &visited,
                         &numVisitedVis, visibleFns);

    trimVisibleCandidates(info, mostApplicable,
                          numVisitedVis, visibleFns);

    gatherCandidatesAndLastResort(info, visInfo, mostApplicable, numVisitedMA,
                                  lrc, candidates);

    // save the scope used for disambiguation
    scopeUsed = visInfo.currStart;

    advanceCurrStart(visInfo);
  }
  while
    (candidates.n == 0 && visInfo.currStart != NULL);

  // If we have not found any candidates after traversing all POIs,
  // look at "last resort" candidates, if any.
  if (candidates.n == 0 && haveAnyLRCs(lrc, visInfo.poiDepth)) {
    visInfo.poiDepth = -1;
    int numVisitedLRC = 0;
    do {
      visInfo.poiDepth++;
      gatherLastResortCandidates(info, visInfo, lrc, numVisitedLRC, candidates);
    }
    while
      (candidates.n == 0 && haveMoreLRCs(lrc, numVisitedLRC));
  }

  explainGatherCandidate(info, candidates);

  return scopeUsed;
}

// run filterCandidate() on 'fn' if appropriate
static void gatherCandidates(CallInfo&                  info,
                             VisibilityInfo&            visInfo,
                             FnSymbol*                  fn,
                             Vec<ResolutionCandidate*>& candidates) {
      // Consider
      //
      //   c1.foo(10, 20);
      //
      // where foo() is a simple method on some class/record
      //
      // Normalize currently converts this to
      //
      //   #<Call     #<Call "foo" _mt c1>    10    20>
      //
      // Resolution performs a post-order traversal of this expression
      // and so the inner call is visited before the outer call.
      //
      // In this context, the inner "call" is effectively a field access
      // rather than a true function call.  Normalize sets the methodTag
      // property to true to indicate this, and this form of call can only
      // be matched to parentheses-less methods and type constructors.
      //
      // Later steps will convert the outer call to become
      //
      //   #<Call "foo" _mt c1  10    20>
      //
      // This outer call has methodTag set to false and this call
      // should be filtered against the available visibleFunctions.
      //

      if (info.call->methodTag == false) {
        filterCandidate(info, visInfo, fn, candidates);

      } else {
        if (fn->hasFlag(FLAG_NO_PARENS) == true) {
          filterCandidate(info, visInfo, fn, candidates);
        }
      }
}

// filter non-last-resort fns into 'candidates',
// store last-resort fns into 'lrc'
static void gatherCandidatesAndLastResort(CallInfo&     info,
                             VisibilityInfo&            visInfo,
                             Vec<FnSymbol*>&            visibleFns,
                             int&                       numVisited,
                             LastResortCandidates&      lrc,
                             Vec<ResolutionCandidate*>& candidates) {
  for (int i = numVisited; i < visibleFns.n; i++) {
    FnSymbol* fn = visibleFns.v[i];
    if (fn->hasFlag(FLAG_LAST_RESORT))
      lrc.push_back(fn);
    else
      gatherCandidates(info, visInfo, fn, candidates);
  }
  markEndOfPOI(lrc);
  numVisited = visibleFns.n;
}

// run filterCandidate() on the next batch of last resort fns
static void gatherLastResortCandidates(CallInfo&                  info,
                                       VisibilityInfo&            visInfo,
                                       LastResortCandidates&      lrc,
                                       int&                       numVisited,
                                       Vec<ResolutionCandidate*>& candidates) {
  int idx = numVisited;

  for (FnSymbol* fn = lrc[idx]; fn != NULL; fn = lrc[++idx]) {
    gatherCandidates(info, visInfo, fn, candidates);
  }

  numVisited = ++idx;
}

static void filterCandidate(CallInfo&                  info,
                            VisibilityInfo&            visInfo,
                            FnSymbol*                  fn,
                            Vec<ResolutionCandidate*>& candidates) {
  ResolutionCandidate* candidate = new ResolutionCandidate(fn);

  if (fExplainVerbose &&
      ((explainCallLine && explainCallMatch(info.call)) ||
       info.call->id == explainCallID)) {
    USR_PRINT(fn, "Considering function: %s", toString(fn));

    if (info.call->id == breakOnResolveID) {
      gdbShouldBreakHere();
    }
  }

  if (candidate->isApplicable(info, &visInfo)) {
    candidates.add(candidate);
  } else {
    delete candidate;
  }
}

static bool typeUsesForwarding(Type* t) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(t)) {
    retval = (toForwardingStmt(at->forwardingTo.head) != NULL) ? true : false;
  }

  return retval;
}

static const char* getForwardedMethodName(const char* calledName,
                                          ForwardingStmt* delegate) {

  const char* methodName = calledName;

  if (delegate->renamed.count(calledName) > 0) {
    methodName = delegate->renamed[calledName];
    // Make sure methodName is a blessed string
    // TODO -- remove this assert
    INT_ASSERT(methodName == astr(methodName));

  } else if (delegate->named.count(calledName)) {
    if (delegate->except) {
      // don't handle this symbol
      methodName = NULL;

    } else {
      // OK, calledName is in the only list.
    }

  } else {
    // It's not a specifically mentioned symbol.
    // It's OK if:
    //  - there was no list at all, or
    //  - the list was an 'except' list
    if ((delegate->renamed.size() == 0 && delegate->named.size() == 0) ||
        delegate->except) {
      // OK
    } else {
      methodName = NULL;
    }
  }

  return methodName;
}

static FnSymbol* adjustAndResolveForwardedCall(CallExpr* call, ForwardingStmt* delegate, const char* methodName) {

  FnSymbol* ret = NULL;
  const char* fnGetTgt   = delegate->fnReturningForwarding;
  Expr* receiver = call->get(2);
  Expr* callStmt = call->getStmtExpr();

  // Handle method receiver being a NamedExpr
  //   e.g. test/arrays/deitz/jacobi5-no-local.chpl
  if (NamedExpr* ne = toNamedExpr(receiver)) {
    INT_ASSERT(ne->name == astrThis);
    receiver = ne->actual;
  }

  // Create a tmp to store the forwarded-to method target.
  VarSymbol* tgt = newTemp(astr_chpl_forward_tgt);
  tgt->addFlag(FLAG_MAYBE_REF);
  tgt->addFlag(FLAG_MAYBE_TYPE);
  DefExpr* defTgt = new DefExpr(tgt);

  // note: cycle detection uses the name of tgt
  // as well as the fact that it's 1st in the block
  callStmt->insertBefore(defTgt);

  // Set the target
  CallExpr* getTgt = new CallExpr(fnGetTgt, gMethodToken, receiver->copy());
  CallExpr* setTgt = new CallExpr(PRIM_MOVE, tgt, getTgt);
  callStmt->insertBefore(setTgt);

  // Adjust the call expression
  UnresolvedSymExpr* baseUrse = toUnresolvedSymExpr(call->baseExpr);
  SymExpr* receiverSe = toSymExpr(receiver);
  INT_ASSERT(baseUrse && receiverSe);
  baseUrse->unresolved = methodName;
  receiverSe->setSymbol(tgt);

  // Now try to resolve setTgt and forwardedCall
  if (tryResolveCall(getTgt)) {
    // getTgt might not resolve if we're looking for a type
    // method and target is value, for example.
    if (FnSymbol* callee = getTgt->resolvedFunction()) {
      resolveFnForCall(callee, getTgt);
    }

    resolveCall(setTgt);
    ret = tryResolveCall(call);
  }

  return ret;
}

static void detectForwardingCycle(CallExpr* call) {
  BlockStmt* cur = toBlockStmt(call->getStmtExpr()->parentExpr);
  DefExpr* firstDef = NULL;
  while (cur != NULL) {
    DefExpr* def = toDefExpr(cur->body.head);
    if (def == NULL || def->sym->name != astr_chpl_forward_tgt)
      return; // not a cycle

    if (firstDef == NULL) {
      firstDef = def;
      INT_ASSERT(firstDef->sym->type && firstDef->sym->type != dtUnknown);
    } else {
      // If firstDef has same type as def, cycle is found.
      if (firstDef->sym->type == def->sym->type) {
        Type* t = canonicalDecoratedClassType(firstDef->sym->getValType());
        TypeSymbol* ts = t->symbol;
        USR_FATAL_CONT(def, "forwarding cycle detected");
        USR_PRINT(ts, "for the type %s", ts->name);
        USR_STOP();
      }
    }
    cur = toBlockStmt(cur->parentExpr);
  }
}


// Returns a relevant FnSymbol if it worked
static FnSymbol* resolveForwardedCall(CallInfo& info, check_state_t checkState) {
  CallExpr* call = info.call;
  const char* calledName = info.name;
  const char* inFnName = call->getFunction()->name;
  Expr* receiver = call->get(2);
  Type* t = receiver->getValType();
  AggregateType* at = toAggregateType(canonicalDecoratedClassType(t));

  FnSymbol* bestFn = NULL;
  CallExpr* bestCall = NULL;
  BlockStmt* bestBlock = NULL;
  ForwardingStmt* bestDelegate = NULL;


  // Don't forward forwarding expr (infinite loop ensues)
  if (startsWith(calledName, "chpl_forwarding_expr"))
    return NULL;

  // This is a workaround for resolvePromotionType being called
  // when some fields have unknown type. A better solution
  // is preferred.
  if (0 == strcmp(calledName, "chpl__promotionType")) {
    return NULL;
  }

  // Do not forward de/initializers.
  // Todo: what else to add here? Note: we cannot be here for a non-method.
  if (call->isNamedAstr(astrInit)       ||
      call->isNamedAstr(astrInitEquals) ||
      call->isNamedAstr(astrDeinit)     ) {
    return NULL;
  }

  // Detect cycles
  detectForwardingCycle(call);

  // Try each of the forwarding clauses to see if any get us
  // a match.
  for_alist(expr, at->forwardingTo) {
    ForwardingStmt* delegate = toForwardingStmt(expr);
    INT_ASSERT(delegate);

    // Forwarding method should use line number of forwarding stmt
    SET_LINENO(delegate);

    // Don't try to recursively forward when processing
    // forwarding expression!
    if (inFnName == delegate->fnReturningForwarding)
      continue;

    // Adjust methodName for rename processing.
    const char* methodName = NULL;
    methodName = getForwardedMethodName(calledName, delegate);

    // Stop processing this delegate if we've ruled out this name.
    if (methodName == NULL) {
      continue;
    }

    // Create a block to store temporaries etc.
    // Store it just before the call being resolved.
    CallExpr* forwardedCall = call->copy();
    BlockStmt* block = new BlockStmt(forwardedCall, BLOCK_SCOPELESS);
    call->getStmtExpr()->insertBefore(block);

    FnSymbol* fn = NULL;
    fn = adjustAndResolveForwardedCall(forwardedCall, delegate, methodName);

    if (fn) {
      if (bestFn == NULL) {
        bestFn = fn;
        bestCall = forwardedCall;
        bestBlock = block;
        bestDelegate = delegate;
      } else {
        USR_FATAL_CONT(call, "ambiguous forwarded call");
        USR_PRINT(bestFn, "candidates include: %s", toString(bestFn));
        USR_PRINT(bestDelegate, "from forwarding statement here");
        USR_PRINT(fn, "candidates include: %s", toString(fn));
        USR_PRINT(delegate, "from forwarding statement here");
        USR_STOP();
      }
    } else {
      // Remove the block for any forwards that didn't resolve
      block->remove();
    }
  }

  // Replace call with bestCall and put any statements in bestBlock
  // just before the call, then remove bestBlock.
  if (bestFn != NULL) {
    if (isContextCallExpr(bestCall->parentExpr)) {
      // retry to get the right ContextCall expression
      bestBlock->remove();

      // Adjust methodName for rename processing.
      const char* methodName = getForwardedMethodName(calledName, bestDelegate);
      INT_ASSERT(methodName);

      bestFn = adjustAndResolveForwardedCall(call, bestDelegate, methodName);
    } else {
      // Replace actuals in call with those from bestCall
      // Note that the above path could be used instead, but
      // this branch hopefully makes fewer temporary AST nodes.
      for_actuals(actual, call) {
        actual->remove();
      }
      call->baseExpr->replace(bestCall->baseExpr->remove());
      call->partialTag = bestCall->partialTag;

      for_actuals(actual, bestCall) {
        call->insertAtTail(actual->remove());
      }
      bestCall->remove();
      bestBlock->flattenAndRemove();
    }
  }

  return bestFn;
}


/************************************* | **************************************
*                                                                             *
* Find the best return-intent overloads from a list of candidates.            *
*                                                                             *
* If there was ambiguity, bestRef, bestConstRef, and bestValue will be NULL,  *
* and the vector ambiguous will store any functions that participated in the  *
* ambiguity (i.e. the multiple best matches).                                 *
*                                                                             *
************************************** | *************************************/

// Allow disambiguation tracing to be controlled by the command-line option
// --explain-verbose.
#define ENABLE_TRACING_OF_DISAMBIGUATION 1

#ifdef ENABLE_TRACING_OF_DISAMBIGUATION

#define EXPLAIN(...)                                  \
        if (developer && DC.explain) fprintf(stderr, __VA_ARGS__)

#else

#define EXPLAIN(...)

#endif


static ResolutionCandidate*
disambiguateByMatch(Vec<ResolutionCandidate*>&   candidates,
                    const DisambiguationContext& DC,
                    bool                         ignoreWhere,
                    Vec<ResolutionCandidate*>&   ambiguous);

static int compareSpecificity(ResolutionCandidate*         candidate1,
                              ResolutionCandidate*         candidate2,
                              const DisambiguationContext& DC,
                              int                          i,
                              int                          j,
                              bool                         forGenericInit);

static int testArgMapping(ResolutionCandidate*         candidate1,
                          ArgSymbol*                   formal1,
                          ResolutionCandidate*         candidate2,
                          ArgSymbol*                   formal2,
                          Symbol*                      actual,
                          const DisambiguationContext& DC,
                          int                          i,
                          int                          j,
                          DisambiguationState&         DS,
                          const char*&                 reason);

static int testOpArgMapping(FnSymbol* fn1, ArgSymbol* formal1, FnSymbol* fn2,
                            ArgSymbol* formal2, Symbol* actual,
                            const DisambiguationContext& DC, int i, int j,
                            DisambiguationState& DS);

ResolutionCandidate*
disambiguateForInit(CallInfo& info, Vec<ResolutionCandidate*>& candidates) {
  DisambiguationContext     DC(info, getVisibilityScope(info.call));
  Vec<ResolutionCandidate*> ambiguous;

  return disambiguateByMatch(candidates, DC, false, ambiguous);
}


// searchScope is the scope used to evaluate is-more-visible
static int disambiguateByMatch(CallInfo&                  info,
                               BlockStmt*                 searchScope,
                               Vec<ResolutionCandidate*>& candidates,

                               ResolutionCandidate*&      bestRef,
                               ResolutionCandidate*&      bestConstRef,
                               ResolutionCandidate*&      bestValue) {
  DisambiguationContext     DC(info, searchScope);

  Vec<ResolutionCandidate*> ambiguous;

  ResolutionCandidate*      best   = disambiguateByMatch(candidates,
                                                         DC,
                                                         true,
                                                         ambiguous);

  int                       retval = 0;

  // The common case is that there is no ambiguity because the
  // return intent overload feature is not used.
  if (best != NULL) {
    if (best->fn->retTag == RET_REF) {
      bestRef = best;

    } else if(best->fn->retTag == RET_CONST_REF) {
      bestConstRef = best;

    } else {
      bestValue = best;
    }

    retval = 1;

  } else {
    // Now, if there was ambiguity, find candidates with different
    // return intent in ambiguousCandidates.
    // If there is only one of each, we are good to go.
    int                  nRef              = 0;
    int                  nConstRef         = 0;
    int                  nValue            = 0;
    int                  nOther            = 0;
    int                  total             = 0;

    ResolutionCandidate* refCandidate      = NULL;
    ResolutionCandidate* constRefCandidate = NULL;
    ResolutionCandidate* valueCandidate    = NULL;

    // Count number of candidates in each category.
    forv_Vec(ResolutionCandidate*, candidate, ambiguous) {
      RetTag retTag = candidate->fn->retTag;

      if (retTag == RET_REF) {
        refCandidate = candidate;
        nRef++;

      } else if(retTag == RET_CONST_REF) {
        constRefCandidate = candidate;
        nConstRef++;

      } else if(retTag == RET_VALUE) {
        valueCandidate = candidate;
        nValue++;

      } else {
        nOther++;
      }
    }

    total = nRef + nConstRef + nValue + nOther;

    // 0 or 1 matches -> return now, not a ref pair.
    if (total <= 1) {
      retval = 0;

      // 1 match is possible with best==NULL in cases
      // where the 'more specific' relation is not transitive.

    } else if (nOther > 0) {
      ambiguous.clear();

      // If there are *any* type/param candidates, we need to cause ambiguity
      // if they are not selected... including consideration of where clauses.
      bestValue  = disambiguateByMatch(candidates, DC, false, ambiguous);
      if (bestValue)
        retval = 1;
      else
        retval = 0;

    } else {
      if (nRef > 1 || nConstRef > 1 || nValue > 1) {
        // Split candidates into ref, const ref, and value candidates
        Vec<ResolutionCandidate*> refCandidates;
        Vec<ResolutionCandidate*> constRefCandidates;
        Vec<ResolutionCandidate*> valueCandidates;
        Vec<ResolutionCandidate*> tmpAmbiguous;

        // Move candidates to above Vecs according to return intent
        forv_Vec(ResolutionCandidate*, candidate, candidates) {
          RetTag retTag = candidate->fn->retTag;

          if (retTag == RET_REF) {
            refCandidates.push_back(candidate);

          } else if (retTag == RET_CONST_REF) {
            constRefCandidates.push_back(candidate);

          } else if (retTag == RET_VALUE) {
            valueCandidates.push_back(candidate);
          }
        }

        // Disambiguate each group
        refCandidate      = disambiguateByMatch(refCandidates,
                                                DC,
                                                false,
                                                tmpAmbiguous);

        constRefCandidate = disambiguateByMatch(constRefCandidates,
                                                DC,
                                                false,
                                                tmpAmbiguous);

        valueCandidate    = disambiguateByMatch(valueCandidates,
                                                DC,
                                                false,
                                                tmpAmbiguous);
        // update the counts
        if (refCandidate      != NULL) nRef      = 1;
        if (constRefCandidate != NULL) nConstRef = 1;
        if (valueCandidate    != NULL) nValue    = 1;
      }

      // Now we know there are >= 2 matches.
      // If there are more than 2 matches in any category, fail for ambiguity.
      if (nRef > 1 || nConstRef > 1 || nValue > 1) {
        retval = 0;

      } else {
        bestRef      = refCandidate;
        bestConstRef = constRefCandidate;
        bestValue    = valueCandidate;

        int nBestRef      = bestRef      != NULL ? 1 : 0;
        int nBestValue    = bestValue    != NULL ? 1 : 0;
        int nBestConstRef = bestConstRef != NULL ? 1 : 0;

        retval = nBestRef + nBestValue + nBestConstRef;
      }
    }
  }

  return retval;
}

static bool isMatchingImagComplex(Type* actualVt, Type* formalVt) {
  if (is_imag_type(actualVt) && is_complex_type(formalVt) &&
      2*get_width(actualVt) == get_width(formalVt))
    return true;

  if (is_real_type(actualVt) && is_complex_type(formalVt) &&
      2*get_width(actualVt) == get_width(formalVt))
    return true;

  return false;
}

static void computeConversionInfo(ResolutionCandidate* candidate,
                                  const DisambiguationContext& DC) {

  // no need to recompute it if it is already computed
  if (candidate->nImplicitConversionsComputed) {
    return;
  }

  bool anyNegParamToUnsigned = false;
  int numParamNarrowing = 0;
  int nImplicitConversions = 0;

  bool forGenericInit = candidate->fn->isInitializer() ||
                        candidate->fn->isCopyInit();

  for (int k = 0; k < DC.actuals->n; k++) {
    Symbol*    actual  = DC.actuals->v[k];
    ArgSymbol* formal = candidate->actualIdxToFormal[k];

    if (formal == nullptr) {
      // this can happen with some operators
      continue;
    }
    if (forGenericInit && k < 2) {
      // Initializer work-around: Skip _mt/_this for generic initializers
      continue;
    }
    if (formal->originalIntent == INTENT_OUT) {
      continue; // type comes from call site so ignore it here
    }

    Type* actualVt = actual->type->getValType();
    Type* formalVt = formal->type->getValType();

    bool promotes = false;
    bool paramNarrows = false;
    canDispatch(actualVt, actual, formalVt, formal, candidate->fn,
                &promotes, &paramNarrows);

    if (paramNarrows) {
      numParamNarrowing++;
    }

    if (promotes) {
      actualVt = actualVt->scalarPromotionType->getValType();
    }

    if (isNegativeParamToUnsigned(actual, actualVt, formalVt)) {
      anyNegParamToUnsigned = true;
    }

    if (actualVt == formalVt) {
      // same type, nothing else to worry about here
      continue;
    }

    if (actualVt == dtNil) {
      // don't worry about converting 'nil' to something else
      continue;
    }

    // Not counting real/imag/complex avoids an ambiguity with
    //  proc f(x: complex(64), y: complex(64))
    //  proc f(x: complex(128), y: complex(128))
    //  f(myInt64, myImag32)
    if (isMatchingImagComplex(actualVt, formalVt)) {
      // don't worry about imag vs complex
      continue;
    }

    // Not counting tuple value vs referential tuple changes
    if (actualVt->symbol->hasFlag(FLAG_TUPLE) &&
        formalVt->symbol->hasFlag(FLAG_TUPLE)) {
      Type* actualNormTup = normalizeTupleTypeToValueTuple(actualVt);
      Type* formalNormTup = normalizeTupleTypeToValueTuple(formalVt);
      if (actualNormTup == formalNormTup) {
        // it is only a change in the tuple ref-ness
        continue;
      }
    }

    nImplicitConversions++;
  }

  // save the computed details in the ResolutionCandidate
  candidate->nImplicitConversionsComputed = true;
  candidate->anyNegParamToUnsigned = anyNegParamToUnsigned;
  candidate->nImplicitConversions = nImplicitConversions;
  candidate->nParamNarrowingImplicitConversions = numParamNarrowing;
}

// If any candidate does not require promotion,
// eliminate candidates that do require promotion.
static void discardWorsePromoting(Vec<ResolutionCandidate*>&   candidates,
                                  const DisambiguationContext& DC,
                                  std::vector<bool>&           discarded) {
  int nPromoting = 0;
  int nNotPromoting = 0;
  for (int i = 0; i < candidates.n; ++i) {
    if (discarded[i]) {
      continue;
    }

    ResolutionCandidate* candidate = candidates.v[i];
    if (candidate->anyPromotes) {
      nPromoting++;
    } else {
      nNotPromoting++;
    }
  }

  if (nPromoting > 0 && nNotPromoting > 0) {
    for (int i = 0; i < candidates.n; ++i) {
      if (discarded[i]) {
        continue;
      }

      ResolutionCandidate* candidate = candidates.v[i];
      if (candidate->anyPromotes) {
        EXPLAIN("%s\n\n", toString(candidate->fn));
        EXPLAIN("X: Fn %d promotes but others do not\n", i);
        discarded[i] = true;
      }
    }
  }
}

// Discard any candidate that has a worse argument mapping than another
// candidate.
static void discardWorseArgs(Vec<ResolutionCandidate*>&   candidates,
                             const DisambiguationContext& DC,
                             std::vector<bool>&           discarded) {

  // If index i is set then we can skip testing function F_i because
  // we already know it can not be the best match
  // because it is a less good match than another candidate.
  std::vector<bool> notBest(candidates.n, false);

  for (int i = 0; i < candidates.n; ++i) {
    if (discarded[i]) {
      continue;
    }

    ResolutionCandidate* candidate1 = candidates.v[i];

    bool forGenericInit = candidate1->fn->isInitializer() ||
                          candidate1->fn->isCopyInit();

    EXPLAIN("##########################\n");
    EXPLAIN("# Considering function %d #\n", i);
    EXPLAIN("##########################\n\n");

    EXPLAIN("%s\n\n", toString(candidate1->fn));

    if (notBest[i]) {
      EXPLAIN("Already known to not be best match.  Skipping.\n\n");
      continue;
    }

    for (int j = 0; j < candidates.n; ++j) {
      if (i == j) {
        continue;
      }
      if (discarded[j]) {
        continue;
      }

      EXPLAIN("Comparing to function %d\n", j);
      EXPLAIN("-----------------------\n");

      ResolutionCandidate* candidate2 = candidates.v[j];

      EXPLAIN("%s\n", toString(candidate2->fn));

      // Consider the relationship among the arguments
      // Note that this part is a partial order;
      // in other words, "incomparable" is an option when comparing
      // two candidates.
      int p = compareSpecificity(candidate1, candidate2,
                                 DC,
                                 i, j,
                                 forGenericInit);

      if (p == 1) {
        EXPLAIN("X: Fn %d is a better match than Fn %d\n\n\n", i, j);
        notBest[j] = true;

      } else if (p == 2) {
        EXPLAIN("X: Fn %d is a worse match than Fn %d\n\n\n", i, j);
        notBest[i] = true;
        break;
      } else {
        if (p == -1) {
          EXPLAIN("X: Fn %d is incomparable with Fn %d\n\n\n", i, j);
        } else if (p == 0) {
          EXPLAIN("X: Fn %d is as good a match as Fn %d\n\n\n", i, j);
          if (notBest[j]) {
            notBest[i] = true;
            break;
          }
        }
      }
    }
  }

  // Now, discard any candidates that were worse than another candidate
  for (int i = 0; i < candidates.n; ++i) {
    if (notBest[i]) {
      discarded[i] = true;
    }
  }
}

// Discard any candidate that has more implicit conversions
// than another candidate. Do not count consider conversions between
// real(w), imag(w), and complex(2*w) when computing this.
//
// Not counting real/imag/complex avoids an ambiguity with
//  proc f(x: complex(64), y: complex(64))
//  proc f(x: complex(128), y: complex(128))
//  f(myInt64, myImag32)
//
// After that, discard any candidate that has a negative param
// converting to unsigned, if there are candidates that do not do that.
//
// After that, discard any candidate that has more param narrowing
// conversions than another candidate.
//
// The number of conversions rule resolves an ambiguity with many cases, e.g.:
//   proc f(x: real(32), y: real(32))
//   proc f(x: real(64), y: real(64))
//   f(1, 1.0: real(32))
//
// The check for negative param to unsigned helps with
//   proc f(param a: int(8),   param b: int(8))
//   proc f(param a: uint(64), param b: uint(64))
//   f(-5: int(8), 16: uint)
//
// The number of param narrowing conversions resolves an ambiguity with:
//   proc f(x: int,    y: int)
//   proc f(x: int(8), y: int(8))
//   f(1, 1:int(8))
static void discardWorseConversions(Vec<ResolutionCandidate*>&   candidates,
                                    const DisambiguationContext& DC,
                                    std::vector<bool>&           discarded) {
  int minImpConv = INT_MAX;
  int maxImpConv = INT_MIN;

  for (int i = 0; i < candidates.n; i++) {
    if (discarded[i]) {
      continue;
    }

    ResolutionCandidate* candidate = candidates.v[i];
    computeConversionInfo(candidate, DC);
    int impConv = candidate->nImplicitConversions;
    if (impConv < minImpConv) {
      minImpConv = impConv;
    }
    if (impConv > maxImpConv) {
      maxImpConv = impConv;
    }
  }

  if (minImpConv < maxImpConv) {
    for (int i = 0; i < candidates.n; i++) {
      if (discarded[i]) {
        continue;
      }

      ResolutionCandidate* candidate = candidates.v[i];
      int impConv = candidate->nImplicitConversions;
      if (impConv > minImpConv) {
        EXPLAIN("X: Fn %d has more implicit conversions\n", i);
        discarded[i] = true;
      }
    }
  }

  int numWithNegParamToSigned = 0;
  int numNoNegParamToSigned = 0;
  for (int i = 0; i < candidates.n; i++) {
    if (discarded[i]) {
      continue;
    }

    ResolutionCandidate* candidate = candidates.v[i];
    computeConversionInfo(candidate, DC);
    if (candidate->anyNegParamToUnsigned) {
      numWithNegParamToSigned++;
    } else {
      numNoNegParamToSigned++;
    }
  }

  if (numWithNegParamToSigned > 0 && numNoNegParamToSigned > 0) {
    for (int i = 0; i < candidates.n; i++) {
      if (discarded[i]) {
        continue;
      }

      ResolutionCandidate* candidate = candidates.v[i];
      if (candidate->anyNegParamToUnsigned) {
        EXPLAIN("X: Fn %d has negative param to signed and others do not\n", i);
        discarded[i] = true;
      }
    }
  }

  int minNarrowing = INT_MAX;
  int maxNarrowing = INT_MIN;
  for (int i = 0; i < candidates.n; i++) {
    if (discarded[i]) {
      continue;
    }

    ResolutionCandidate* candidate = candidates.v[i];
    computeConversionInfo(candidate, DC);
    int narrowing = candidate->nParamNarrowingImplicitConversions;
    if (narrowing < minNarrowing) {
      minNarrowing = narrowing;
    }
    if (narrowing > maxNarrowing) {
      maxNarrowing = narrowing;
    }
  }

  if (minNarrowing < maxNarrowing) {
    for (int i = 0; i < candidates.n; i++) {
      if (discarded[i]) {
        continue;
      }

      ResolutionCandidate* candidate = candidates.v[i];
      int narrowing = candidate->nParamNarrowingImplicitConversions;
      if (narrowing > minNarrowing) {
        EXPLAIN("X: Fn %d has more param narrowing conversions\n", i);
        discarded[i] = true;
      }
    }
  }
}

// If some candidates have 'where' clauses and others do not,
// discard those without 'where' clauses
static void discardWorseWhereClauses(Vec<ResolutionCandidate*>&   candidates,
                                     const DisambiguationContext& DC,
                                     std::vector<bool>&           discarded) {
  int nWhere = 0;
  int nNoWhere = 0;
  for (int i = 0; i < candidates.n; ++i) {
    if (discarded[i]) {
      continue;
    }

    ResolutionCandidate* candidate = candidates.v[i];
    bool where = candidate->fn->where != NULL &&
                 !candidate->fn->hasFlag(FLAG_COMPILER_ADDED_WHERE);

    if (where) {
      nWhere++;
    } else {
      nNoWhere++;
    }
  }

  if (nWhere > 0 && nNoWhere > 0) {
    for (int i = 0; i < candidates.n; ++i) {
      if (discarded[i]) {
        continue;
      }

      ResolutionCandidate* candidate = candidates.v[i];
      bool where = candidate->fn->where != NULL &&
                   !candidate->fn->hasFlag(FLAG_COMPILER_ADDED_WHERE);
      if (!where) {
        EXPLAIN("X: Fn %d does not have 'where' but others do\n", i);
        discarded[i] = true;
      }
    }
  }
}

// Discard candidates with further visibility distance
// than other candidates.
static void discardWorseVisibility(Vec<ResolutionCandidate*>&   candidates,
                                   const DisambiguationContext& DC,
                                   std::vector<bool>&           discarded) {
  int minDistance = INT_MAX;
  int maxDistance = INT_MIN;

  for (int i = 0; i < candidates.n; i++) {
    if (discarded[i]) {
      continue;
    }

    ResolutionCandidate* candidate = candidates.v[i];
    int distance = computeVisibilityDistance(DC.scope, candidate->fn);
    candidate->visibilityDistance = distance;

    if (distance < minDistance) {
      minDistance = distance;
    }
    if (distance > maxDistance) {
      maxDistance = distance;
    }
  }

  if (minDistance < maxDistance) {
    for (int i = 0; i < candidates.n; i++) {
      if (discarded[i]) {
        continue;
      }

      ResolutionCandidate* candidate = candidates.v[i];
      int distance = candidate->visibilityDistance;
      if (distance > minDistance) {
        EXPLAIN("X: Fn %d has further visibility distance\n", i);
        discarded[i] = true;
      }
    }
  }
}

struct PartialOrderChecker {
  int n;
  std::vector<bool> results;
  const Vec<ResolutionCandidate*>& candidates;
  DisambiguationContext DC;

  PartialOrderChecker(const Vec<ResolutionCandidate*>& candidates,
                      const DisambiguationContext& DC)
    : n(candidates.n), candidates(candidates), DC(DC) {
    results.resize(n*n);
    this->DC.explain = true;
  }

  int idx(int i, int j) {
    assert(0 <= i && i < n);
    assert(0 <= j && j < n);
    return n*i + j;
  }

  void addResult(int i, int j, int cmp) {
    if (cmp == 1) { // aka i < j
      results[idx(i, j)] = true;
    } else if (cmp == 2) { // aka j < i
      results[idx(j, i)] = true;
    }
  }

  void printActuals() {
    printf("actuals: (");
    bool first = true;
    forv_Vec(Symbol, actual, *DC.actuals) {
      if (first) {
        first = false;
      } else {
        printf(", ");
      }
      Type* t = actual->getValType();
      printf("%s : %s", toString(actual, false), toString(t));
    }
    printf(")\n");
  }

  void explainComparison(int i, int j) {
    ResolutionCandidate* candidate1         = candidates.v[i];
    bool forGenericInit = candidate1->fn->isInitializer() || candidate1->fn->isCopyInit();

    EXPLAIN("##########################\n");
    EXPLAIN("# Considering function %d #\n", i);
    EXPLAIN("##########################\n\n");
    EXPLAIN("%s\n\n", toString(candidate1->fn));

    ResolutionCandidate* candidate2 = candidates.v[j];

    EXPLAIN("Comparing to function %d\n", j);
    EXPLAIN("-----------------------\n");
    EXPLAIN("%s\n", toString(candidate2->fn));

    int cmp = compareSpecificity(candidate1,
                                 candidate2,
                                 DC,
                                 i,
                                 j,
                                 forGenericInit);

    if (cmp == 1) {
      EXPLAIN("X: Fn %d is a better match than Fn %d\n\n\n", i, j);
    } else if (cmp == 2) {
      EXPLAIN("X: Fn %d is a worse match than Fn %d\n\n\n", i, j);
    } else {
      EXPLAIN("X: Fn %d is not better or worse than Fn %d\n\n\n", i, j);
    }
  }

  void checkResults() {
    // check irreflexivity
    for (int i = 0; i < n; i++) {
      if (results[idx(i, i)]) {
        ResolutionCandidate* candidatei = candidates.v[i];

        printf("irreflexivity fail : i < i\n");
        printActuals();
        printf("i: %s\n", toString(candidatei->fn));
        printf("\n");
      }
    }

    // check asymmetry
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (results[idx(i, j)] && results[idx(j, i)]) {
          ResolutionCandidate* candidatei = candidates.v[i];
          ResolutionCandidate* candidatej = candidates.v[j];

          printf("asymmetry fail : i < j and j < i\n");
          printActuals();
          printf("i: %s\n", toString(candidatei->fn));
          printf("j: %s\n", toString(candidatej->fn));
          printf("\n");
          explainComparison(i, j);
          explainComparison(j, i);
          printf("\n\n");
        }
      }
    }

    // check transitivity
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (i != j && results[idx(i, j)]) {
          for (int k = 0; k < n; k++) {
            if (j != k && results[idx(j, k)] && !results[idx(i, k)]) {
              ResolutionCandidate* candidatei = candidates.v[i];
              ResolutionCandidate* candidatej = candidates.v[j];
              ResolutionCandidate* candidatek = candidates.v[k];

              printf("transitivity fail : i < j and j < k but not i < k\n");
              printActuals();
              printf("i: %s\n", toString(candidatei->fn));
              printf("j: %s\n", toString(candidatej->fn));
              printf("k: %s\n", toString(candidatek->fn));
              printf("\n");
              explainComparison(i, j);
              explainComparison(j, k);
              explainComparison(i, k);
              printf("\n\n");
            }
          }
        }
      }
    }
  }
};

static void disambiguateDiscarding(Vec<ResolutionCandidate*>&   candidates,
                                   const DisambiguationContext& DC,
                                   bool                         ignoreWhere,
                                   std::vector<bool>&           discarded) {

  if (!DC.useOldVisibility && !DC.isMethodCall) {
    // If some candidates are less visible than other candidates,
    // discard those with less visibility.
    // This filter should not be applied to method calls.
    discardWorseVisibility(candidates, DC, discarded);
  }

  // If any candidate does not require promotion,
  // eliminate candidates that do require promotion.
  discardWorsePromoting(candidates, DC, discarded);

  // Consider the relationship among the arguments
  // Note that this part is a partial order;
  // in other words, "incomparable" is an option when comparing
  // two candidates. It should be transitive.
  // Discard any candidate that has a worse argument mapping than another
  // candidate.
  discardWorseArgs(candidates, DC, discarded);

  // Apply further filtering to the set of candidates

  // Discard any candidate that has more implicit conversions
  // than another candidate.
  // After that, discard any candidate that has more param narrowing
  // conversions than another candidate.
  discardWorseConversions(candidates, DC, discarded);

  if (!ignoreWhere) {
    // If some candidates have 'where' clauses and others do not,
    // discard those without 'where' clauses
    discardWorseWhereClauses(candidates, DC, discarded);
  }
  if (DC.useOldVisibility && !DC.isMethodCall) {
    // If some candidates are less visible than other candidates,
    // discard those with less visibility.
    // This filter should not be applied to method calls.
    discardWorseVisibility(candidates, DC, discarded);
  }
}

static ResolutionCandidate*
disambiguateByMatchInner(Vec<ResolutionCandidate*>&   candidates,
                         const DisambiguationContext& DC,
                         bool                         ignoreWhere,
                         Vec<ResolutionCandidate*>&   ambiguous) {

  // quick exit if there is nothing to do
  if (candidates.n == 0) {
    return nullptr;
  }
  if (candidates.n == 1) {
    return candidates.v[0];
  }

  // Disable implicit conversion to remove nilability for disambiguation
  int saveGenerousResolutionForErrors = 0;
  if (generousResolutionForErrors > 0) {
    saveGenerousResolutionForErrors = generousResolutionForErrors;
    generousResolutionForErrors = 0;
  }

  // If index i is set we have ruled out that function
  std::vector<bool> discarded(candidates.n, false);

  // use new rules
  disambiguateDiscarding(candidates, DC, ignoreWhere, discarded);

  // If there is just 1 candidate at this point, return it
  {
    int only = -1;
    int currentCandidates = 0;
    for (int i = 0; i < candidates.n; ++i) {
      if (discarded[i]) {
        continue;
      }
      only = i;
      currentCandidates++;
    }

    if (currentCandidates == 1) {
      EXPLAIN("Y: Fn %d is the best match.\n\n\n", only);

      if (saveGenerousResolutionForErrors > 0)
        generousResolutionForErrors = saveGenerousResolutionForErrors;

      return candidates.v[only];
    }
  }

  // There was more than one best candidate.
  // So, add whatever is left to 'ambiguous'
  // and return NULL.
  for (int i = 0; i < candidates.n; i++) {
    if (discarded[i]) {
      continue;
    }

    EXPLAIN("Z: Fn %d is one of the best matches\n", i);
    ambiguous.add(candidates.v[i]);
  }

  if (saveGenerousResolutionForErrors > 0)
    generousResolutionForErrors = saveGenerousResolutionForErrors;

  return nullptr;
}

static ResolutionCandidate*
disambiguateByMatch(Vec<ResolutionCandidate*>&   candidates,
                    const DisambiguationContext& DC,
                    bool                         ignoreWhere,
                    Vec<ResolutionCandidate*>&   ambiguous) {
  auto ret = disambiguateByMatchInner(candidates, DC, ignoreWhere, ambiguous);

  if (fVerify) {
    // check that 'compareSpecificity' creates a partial order
    PartialOrderChecker checker(candidates, DC);

    for (int i = 0; i < candidates.n; ++i) {
      ResolutionCandidate* candidate1         = candidates.v[i];
      bool forGenericInit = candidate1->fn->isInitializer() || candidate1->fn->isCopyInit();

      for (int j = i; j < candidates.n; ++j) {
        ResolutionCandidate* candidate2 = candidates.v[j];

        int cmp = compareSpecificity(candidate1,
                                     candidate2,
                                     DC,
                                     i,
                                     j,
                                     forGenericInit);

        checker.addResult(i, j, cmp);
      }
    }

    checker.checkResults();
  }

  return ret;
}

/** Determines if fn1 is a better match than fn2.
 *
 * This function implements the function comparison component of the
 * disambiguation procedure as detailed in section 13.13 of the Chapel
 * language specification.
 *
 * \param candidate1 The function on the left-hand side of the comparison.
 * \param candidate2 The function on the right-hand side of the comparison.
 * \param DC         The disambiguation context.
 *
 * \return -1 if the two functions are incomparable
 * \return 0 if the two functions are equally specific
 * \return 1 if fn1 is a more specific function than f2
 * \return 2 if fn2 is a more specific function than f1
 */
static int compareSpecificity(ResolutionCandidate*         candidate1,
                              ResolutionCandidate*         candidate2,
                              const DisambiguationContext& DC,
                              int                          i,
                              int                          j,
                              bool                         forGenericInit) {

  DisambiguationState DS;

  // Initializer work-around: Skip _mt/_this for generic initializers
  int                 start   = (forGenericInit == false) ? 0 : 2;

  bool                prefer1 = false;
  bool                prefer2 = false;

  int                 nArgsIncomparable = 0;

  for (int k = start; k < DC.actuals->n; ++k) {
    Symbol*    actual  = DC.actuals->v[k];
    ArgSymbol* formal1 = candidate1->actualIdxToFormal[k];
    ArgSymbol* formal2 = candidate2->actualIdxToFormal[k];

    EXPLAIN("\nLooking at argument %d\n", k);

    if (formal1 == NULL || formal2 == NULL) {
      if (candidate1->fn->hasFlag(FLAG_OPERATOR) &&
          candidate2->fn->hasFlag(FLAG_OPERATOR)) {
        EXPLAIN("\nSkipping argument %d because could be in an operator call\n",
                k);
        continue;
      } else {
        // One of the two candidate functions was not an operator, but one
        // was so we need to do something special here.
        int p = testOpArgMapping(candidate1->fn, formal1,
                                 candidate2->fn, formal2,
                                 actual, DC, i, j, DS);
        const char* reason = "operator method vs function";
        if (p == 1) {
          DS.fn1NonParamArgsPreferred = true;
          EXPLAIN("%s: Fn %d is non-param preferred\n", reason, i);
        } else if (p == 2) {
          DS.fn2NonParamArgsPreferred = true;
          EXPLAIN("%s: Fn %d is non-param preferred\n", reason, j);
        }
        continue;
      }
    }

    bool actualParam = (getImmediate(actual) != NULL);

    const char* reason = "";
    int p = testArgMapping(candidate1, formal1,
                           candidate2, formal2,
                           actual, DC, i, j, DS, reason);

    if (p == -1) {
      nArgsIncomparable++;
    }

    if (actualParam) {
      if (p == 1) {
        DS.fn1ParamArgsPreferred = true;
        EXPLAIN("%s: Fn %d is param preferred\n", reason, i);
      } else if (p == 2) {
        DS.fn2ParamArgsPreferred = true;
        EXPLAIN("%s: Fn %d is param preferred\n", reason, j);
      }
    } else {
      if (p == 1) {
        DS.fn1NonParamArgsPreferred = true;
        EXPLAIN("%s: Fn %d is non-param preferred\n", reason, i);
      } else if (p == 2) {
        DS.fn2NonParamArgsPreferred = true;
        EXPLAIN("%s: Fn %d is non-param preferred\n", reason, j);
      }
    }
  }

  if (DS.fn1NonParamArgsPreferred != DS.fn2NonParamArgsPreferred) {
    EXPLAIN("\nP: only one function has preferred non-param args\n");

    prefer1 = DS.fn1NonParamArgsPreferred;
    prefer2 = DS.fn2NonParamArgsPreferred;

  } else if (DS.fn1ParamArgsPreferred != DS.fn2ParamArgsPreferred) {
    EXPLAIN("\nP1: only one function has preferred param args\n");

    prefer1 = DS.fn1ParamArgsPreferred;
    prefer2 = DS.fn2ParamArgsPreferred;

  }

  if (prefer1) {
    EXPLAIN("\nW: Fn %d is more specific than Fn %d\n", i, j);
    return 1;

  } else if (prefer2) {
    EXPLAIN("\nW: Fn %d is less specific than Fn %d\n", i, j);
    return 2;

  } else {
    if (nArgsIncomparable > 0 ||
        (DS.fn1NonParamArgsPreferred && DS.fn2NonParamArgsPreferred) ||
        (DS.fn1ParamArgsPreferred && DS.fn2ParamArgsPreferred)) {
      EXPLAIN("\nW: Fn %d and Fn %d are incomparable\n", i, j);
      return -1;
    }

    EXPLAIN("\nW: Fn %d and Fn %d are equally specific\n", i, j);
    return 0;
  }
}

// Calls canDispatch and does the initial EXPLAIN calls, which were otherwise
// duplicated
static void testArgMapHelper(FnSymbol* fn, ArgSymbol* formal, Symbol* actual,
                             Type* fType, Type* actualType, bool actualParam,
                             bool* formalPromotes, bool* formalNarrows,
                             const DisambiguationContext& DC,
                             DisambiguationState& DS,
                             int fnNum) {
  canDispatch(actualType, actual, fType, formal, fn, formalPromotes,
              formalNarrows);

  EXPLAIN("Formal %d's type: %s", fnNum, toString(fType));
  if (*formalPromotes)
    EXPLAIN(" (promotes)");
  if (formal->hasFlag(FLAG_INSTANTIATED_PARAM))
    EXPLAIN(" (instantiated param)");
  if (*formalNarrows)
    EXPLAIN(" (narrows param)");
  EXPLAIN("\n");

  if (actualType != fType) {
    if (actualParam) {
      EXPLAIN("Actual requires param coercion to match formal %d\n", fnNum);
    } else {
      EXPLAIN("Actual requires coercion to match formal %d\n", fnNum);
    }
  }
}

/** Compare two argument mappings, given a set of actual arguments, and set the
 *  disambiguation state appropriately.
 *
 * This function implements the argument mapping comparison component of the
 * disambiguation procedure as detailed in section 13.14.3 of the Chapel
 * language specification (page 107).
 *
 * \param candidate1 The first function to be compared.
 * \param formal1 The formal argument that correspond to the actual argument
 *                for the first function.
 * \param candidate2 The second function to be compared.
 * \param formal2 The formal argument that correspond to the actual argument
 *                for the second function.
 * \param actual  The actual argument from the call site.
 * \param DC      The disambiguation context.
 * \param DS      The disambiguation state. This function
 *                sets DS.fnXPromotes and DS.fnXNumParamNarrowing.
 *
 * Returns:
 *  -1 if the two formals are incomparable
 *   0 if the two formals have the same level of preference
 *   1 if fn1 is preferred
 *   2 if fn2 is preferred
 */
static int testArgMapping(ResolutionCandidate*         candidate1,
                          ArgSymbol*                   formal1,
                          ResolutionCandidate*         candidate2,
                          ArgSymbol*                   formal2,
                          Symbol*                      actual,
                          const DisambiguationContext& DC,
                          int                          i,
                          int                          j,
                          DisambiguationState&         DS,
                          const char*&                 reason) {

  // Give up early for out intent arguments
  // (these don't impact candidate selection)
  if (formal1->originalIntent == INTENT_OUT ||
      formal2->originalIntent == INTENT_OUT) {
    return -1;
  }

  FnSymbol* fn1 = candidate1->fn;
  FnSymbol* fn2 = candidate2->fn;

  // We only want to deal with the value types here, avoiding odd overloads
  // working (or not) due to _ref.
  Type* f1Type          = formal1->type->getValType();
  Type* f2Type          = formal2->type->getValType();
  Type* actualType      = actual->type->getValType();

  // Additionally, ignore the difference between referential tuples
  // and value tuples.
  f1Type = normalizeTupleTypeToValueTuple(f1Type);
  f2Type = normalizeTupleTypeToValueTuple(f2Type);
  actualType = normalizeTupleTypeToValueTuple(actualType);

  bool  formal1Promotes = false;
  bool  formal2Promotes = false;
  bool  formal1Narrows = false;
  bool  formal2Narrows = false;

  Type* actualScalarType = actualType;

  bool f1Param = formal1->hasFlag(FLAG_INSTANTIATED_PARAM);
  bool f2Param = formal2->hasFlag(FLAG_INSTANTIATED_PARAM);

  bool actualParam = (getImmediate(actual) != NULL);

  EXPLAIN("Actual's type: %s", toString(actualType));
  if (actualParam)
    EXPLAIN(" (param)");
  EXPLAIN("\n");

  // do some EXPLAIN calls
  testArgMapHelper(fn1, formal1, actual, f1Type, actualType, actualParam,
                   &formal1Promotes, &formal1Narrows, DC, DS, 1);

  testArgMapHelper(fn2, formal2, actual, f2Type, actualType, actualParam,
                   &formal2Promotes, &formal2Narrows, DC, DS, 2);

  // Figure out scalar type for candidate matching
  if ((formal1Promotes || formal2Promotes) &&
      actualType->scalarPromotionType != NULL) {
    actualScalarType = actualType->scalarPromotionType->getValType();
  }

  if (isSyncType(actualScalarType) || isSingleType(actualScalarType)) {
    actualScalarType = actualScalarType->getField("valType")->getValType();
  }

  // consider promotion
  if (!formal1Promotes && formal2Promotes) {
    reason = "no promotion vs promotes";
    return 1;
  }

  if (formal1Promotes && !formal2Promotes) {
    reason = "no promotion vs promotes";
    return 2;
  }

  // consider concrete vs generic functions
  // note: the f1Type == f2Type part here is important
  // and it prevents moving this logic out of the pairwise comparison.
  // It is important e.g. for:
  //   class Parent { }
  //   class GenericChild : Parent { type t; }
  // Here a GenericChild argument should be preferred to a Parent one
  if (f1Type == f2Type) {
    if (!formal1->instantiatedFrom && formal2->instantiatedFrom) {
      reason = "concrete vs generic";
      return 1;
    }

    if (formal1->instantiatedFrom && !formal2->instantiatedFrom) {
      reason = "concrete vs generic";
      return 2;
    }

    if (formal1->instantiatedFrom != dtAny &&
        formal2->instantiatedFrom == dtAny) {
      reason = "generic any vs partially generic/concrete";
      return 1;
    }

    if (formal1->instantiatedFrom == dtAny &&
        formal2->instantiatedFrom != dtAny) {
      reason = "generic any vs partially generic/concrete";
      return 2;
    }

    if (formal1->instantiatedFrom && formal2->instantiatedFrom &&
        formal1->hasFlag(FLAG_NOT_FULLY_GENERIC) &&
        !formal2->hasFlag(FLAG_NOT_FULLY_GENERIC)) {
      reason = "partially generic vs generic";
      return 1;
    }

    if (formal1->instantiatedFrom && formal2->instantiatedFrom &&
        !formal1->hasFlag(FLAG_NOT_FULLY_GENERIC) &&
        formal2->hasFlag(FLAG_NOT_FULLY_GENERIC)) {
      reason = "partially generic vs generic";
      return 2;
    }
  }

  if (f1Param && !f2Param) {
    reason = "param vs not";
    return 1;
  }

  if (!f1Param && f2Param) {
    reason = "param vs not";
    return 2;
  }

  if (f1Type != f2Type) {
    // to help with
    // proc f(x: int(8))
    // proc f(x: int(64))
    // f(myInt32) vs. f(1: int(32)) should behave the same
    if (actualParam) {
      if (!formal1Narrows && formal2Narrows) {
        reason = "param narrows vs not";
        return 1;
      }
      if (formal1Narrows && !formal2Narrows) {
        reason = "param narrows vs not";
        return 2;
      }
    }

    // e.g. to help with
    //   sin(1) calling the real(64) version (vs real(32) version)
    //
    //   proc f(complex(64), complex(64))
    //   proc f(complex(128), complex(128))
    //   f(1.0, 1.0i) calling the complex(128) version
    int p = prefersNumericCoercion(candidate1, candidate2, DC,
                                   actual, actualScalarType,
                                   f1Type, f2Type, reason);
    if (p == 1) {
      return 1;
    }
    if (p == 2) {
      return 2;
    }

    if (actualType == f1Type && actualType != f2Type) {
      reason = "actual type vs not";
      return 1;
    }

    if (actualType == f2Type && actualType != f1Type) {
      reason = "actual type vs not";
      return 2;
    }

    if (actualScalarType == f1Type && actualScalarType != f2Type) {
      reason = "scalar type vs not";
      return 1;
    }

    if (actualScalarType == f2Type && actualScalarType != f1Type) {
      reason = "scalar type vs not";
      return 2;
    }


    bool fn1Dispatches = moreSpecificCanDispatch(fn1, f1Type, f2Type);
    bool fn2Dispatches = moreSpecificCanDispatch(fn2, f2Type, f1Type);
    if (fn1Dispatches && !fn2Dispatches) {
      reason = "can dispatch";
      return 1;
    }
    if (!fn1Dispatches && fn2Dispatches) {
      reason = "can dispatch";
      return 2;
    }
  }

  if (f1Type == f2Type) {
    // the formals are the same in terms of preference
    return 0;
  }

  // the formals are incomparable
  return -1;
}

/*
 * Returns:
 *   0 if there is no preference between them
 *   1 if fn1 is preferred
 *   2 if fn2 is preferred
*/
static int testOpArgMapping(FnSymbol* fn1, ArgSymbol* formal1, FnSymbol* fn2,
                            ArgSymbol* formal2, Symbol* actual,
                            const DisambiguationContext& DC, int i, int j,
                            DisambiguationState& DS) {
  // Validate our assumptions in this function - only operator functions should
  // return a NULL for the formal and they should only do so for method token
  // and "this" actuals.
  INT_ASSERT(fn1->hasFlag(FLAG_OPERATOR) == (formal1 == NULL));
  INT_ASSERT(fn2->hasFlag(FLAG_OPERATOR) == (formal2 == NULL));

  Type* actualType = actual->type->getValType();
  bool actualParam = getImmediate(actual) != NULL;
  if (formal1 == NULL) {
    INT_ASSERT(formal2 != NULL);

    Type* f2Type = formal2->type->getValType();
    bool formal2Promotes = false;
    bool formal2Narrows = false;

    testArgMapHelper(fn2, formal2, actual, f2Type, actualType, actualParam,
                     &formal2Promotes, &formal2Narrows, DC, DS, 2);

    return 2;

  } else {
    INT_ASSERT(formal2 == NULL);

    Type* f1Type = formal1->type->getValType();
    bool formal1Promotes = false;
    bool formal1Narrows = false;

    testArgMapHelper(fn1, formal1, actual, f1Type, actualType, actualParam,
                     &formal1Promotes, &formal1Narrows, DC, DS, 1);

    return 1;
  }

  return 0;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void  captureTaskIntentValues(int        argNum,
                                     ArgSymbol* formal,
                                     Expr*      actual,
                                     Symbol*    varActual,
                                     CallInfo&  info,
                                     FnSymbol*  taskFn);

static void  verifyTaskFnCall(BlockStmt* parent, CallExpr* call);

static Expr* parentToMarker(BlockStmt* parent, CallExpr* call);

//
// Copy the type of the actual into the type of the corresponding formal
// of a task function. Also do captureTaskIntentValues() when needed.
//
static void handleTaskIntentArgs(CallInfo& info, FnSymbol* taskFn) {
  CallExpr* call = info.call;

  INT_ASSERT(taskFn);

  if (needsCapture(taskFn) == false) {
    // A task function should have args only if it needsCapture.
    if (taskFn->hasFlag(FLAG_ON) == true) {
      // Documenting the current state: fn_on gets a chpl_localeID_t arg.
      INT_ASSERT(call->numActuals() == 1);

    } else {
      INT_ASSERT(call->numActuals() == 0 || isTaskFun(taskFn) == false);
    }

  } else {
    int argNum = 0;

    for_formals_actuals(formal, actual, call) {
      SymExpr* symexpActual = toSymExpr(actual);

      if (symexpActual == NULL) {
        // We add NamedExpr args in propagateExtraLeaderArgs().
        NamedExpr* namedexpActual = toNamedExpr(actual);

        INT_ASSERT(namedexpActual);

        symexpActual = toSymExpr(namedexpActual->actual);
      }

      INT_ASSERT(symexpActual); // because of how we invoke a task function

      Symbol* varActual = symexpActual->symbol();

      // If 'call' is in a generic function, it will have been instantiated by
      // now. Otherwise our task function has to remain generic.
      INT_ASSERT(varActual->type->symbol->hasFlag(FLAG_GENERIC) == false);

      if (formal->id == breakOnResolveID)
        gdbShouldBreakHere();

      IntentTag origFormalIntent = formal->intent;

      // Need to copy varActual->type even for type variables.
      // BTW some formals' types may have been set in createTaskFunctions().
      formal->type = varActual->type;

      // If the actual is a ref, still need to capture it => remove ref.
      if (isReferenceType(varActual->type) == true) {
        Type* deref = varActual->type->getValType();

        // todo: replace needsCapture() with always resolveArgIntent(formal)
        // then checking (formal->intent & INTENT_FLAG_IN)
        if (needsCapture(deref) == true) {
          formal->type = deref;

          // If the formal has a ref intent, DO need a ref type => restore it.
          resolveArgIntent(formal);

          if (formal->intent & INTENT_FLAG_REF) {
            formal->type = varActual->type;
          }

          if (varActual->isConstant() == true) {
            int newIntent = formal->intent | INTENT_FLAG_CONST;

            // and clear INTENT_FLAG_MAYBE_CONST flag
            newIntent      &= ~INTENT_FLAG_MAYBE_CONST;
            formal->intent =  (IntentTag) newIntent;
          }
        }
      }

      if (varActual->hasFlag(FLAG_TYPE_VARIABLE) == true) {
        formal->addFlag(FLAG_TYPE_VARIABLE);
      }

      // This does not capture records/strings that are passed
      // by blank or const intent. As of this writing (6'2015)
      // records and strings are (incorrectly) captured at the point
      // when the task function/arg bundle is created.
      bool shouldCapture = false;
      if (taskFn->hasFlag(FLAG_COBEGIN_OR_COFORALL) == true &&
          varActual->isConstValWillNotChange()      == false &&
          (concreteIntent(formal->intent, formal->type->getValType())
           & INTENT_FLAG_IN)) {
        // skip dummy_locale_arg: chpl_localeID_t
        if (argNum != 0 || taskFn->hasFlag(FLAG_ON) == false) {
          shouldCapture = true;
        }
      }

      // Also always capture when getting a borrow from an owned/shared.
      if (isManagedPtrType(varActual->getValType()) &&
          (taskFn->hasFlag(FLAG_COBEGIN_OR_COFORALL) ||
           taskFn->hasFlag(FLAG_BEGIN)) &&
          (//origFormalIntent == INTENT_IN ||
           origFormalIntent == INTENT_CONST ||
           //origFormalIntent == INTENT_CONST_IN ||
           origFormalIntent == INTENT_BLANK)) {
        if (taskFn->hasFlag(FLAG_COBEGIN_OR_COFORALL))
          shouldCapture = true;
        formal->type = getManagedPtrBorrowType(varActual->getValType());
        formal->intent = INTENT_CONST_IN;
      }

      if (shouldCapture) {
        captureTaskIntentValues(argNum,
                                formal,
                                actual,
                                varActual,
                                info,
                                taskFn);
      }

      argNum = argNum + 1;
    }

    // Even if some formals are (now) types, if 'taskFn' remained generic,
    // gatherCandidates() would not instantiate it, for some reason.
    taskFn->setGeneric(false);
  }
}

//
// Generate code to store away the value of 'varActual' before
// the cobegin or the coforall loop starts. Use this value
// instead of 'varActual' as the actual to the task function,
// meaning (later in compilation) in the argument bundle.
//
// This is to ensure that all task functions use the same value
// for their respective formal when that has an 'in'-like intent,
// even if 'varActual' is modified between creations of
// the multiple task functions.
//
static void captureTaskIntentValues(int        argNum,
                                    ArgSymbol* formal,
                                    Expr*      actual,
                                    Symbol*    varActual,
                                    CallInfo&  info,
                                    FnSymbol*  taskFn) {
  CallExpr*  call   = info.call;
  BlockStmt* parent = toBlockStmt(call->parentExpr);

  INT_ASSERT(parent);

  if (taskFn->hasFlag(FLAG_ON) && !parent->isForLoop()) {
    // coforall ... { on ... { .... }} ==> there is an intermediate BlockStmt
    parent = toBlockStmt(parent->parentExpr);

    INT_ASSERT(parent);
  }

  if (fVerify == true) {
    if (argNum == 0 || (argNum == 1 && taskFn->hasFlag(FLAG_ON) == true)) {
      verifyTaskFnCall(parent, call); //assertions only
    }
  }

  Expr* marker = parentToMarker(parent, call);

  if (varActual->hasFlag(FLAG_NO_CAPTURE_FOR_TASKING) == true) {

  } else if (varActual->defPoint->parentExpr == parent) {
    // Index variable of the coforall loop? Do not capture it!
    INT_ASSERT(varActual->hasFlag(FLAG_COFORALL_INDEX_VAR));

  } else {
    SymbolMap*& symap   = capturedValues[parent->id];
    Symbol*     capTemp = NULL;

    if (symap != NULL) {
      capTemp = symap->get(varActual);

    } else {
      symap = new SymbolMap();
    }

    if (capTemp == NULL) {
      capTemp = newTemp(astr(formal->name, "_captemp"), formal->type);

      marker->insertBefore(new DefExpr(capTemp));

      if (hasAutoCopyForType(formal->type) == true) {
        FnSymbol* autoCopy = getAutoCopy(formal->type);

        marker->insertBefore("'move'(%S,%S(%S, %S))",
                             capTemp,
                             autoCopy,
                             varActual,
                             gFalse); // can we do something better here?

      } else if (isReferenceType(varActual->type) ==  true &&
                 isReferenceType(capTemp->type)   == false) {
        marker->insertBefore(new CallExpr(PRIM_ASSIGN, capTemp, varActual));

      } else {
        marker->insertBefore("'move'(%S,%S)", capTemp, varActual);
      }

      symap->put(varActual, capTemp);
    }

    actual->replace(new SymExpr(capTemp));

    INT_ASSERT(info.actuals.v[argNum] == varActual);

    info.actuals.v[argNum] = capTemp;
  }
}

// Ensure 'parent' is the block before which we want to do the capturing.
static void verifyTaskFnCall(BlockStmt* parent, CallExpr* call) {
  if (call->isNamed("coforall_fn") == true ||
      call->isNamed("on_fn")       == true) {
    INT_ASSERT(parent->isForLoop());

  } else if (call->isNamed("cobegin_fn") == true) {
    DefExpr* first = toDefExpr(parent->getFirstExpr());

    // just documenting the current state
    INT_ASSERT(first && !strcmp(first->sym->name, "_cobeginCount"));

  } else {
    INT_ASSERT(call->isNamed("begin_fn"));
  }
}

//
// Returns the expression that we want to capture before.
//
// Why not just 'parent'? In users/shetag/fock/fock-dyn-prog-cntr.chpl,
// we cannot do parent->insertBefore() because parent->list is null.
// That's because we have: if ... then cobegin ..., so 'parent' is
// immediately under CondStmt. This motivated me for cobegins to capture
// inside of the 'parent' block, at the beginning of it.
//
static Expr* parentToMarker(BlockStmt* parent, CallExpr* call) {
  Expr*  retval = parent;

  if (call->isNamed("cobegin_fn") == true) {
    DefExpr*  def  = toDefExpr(parent->body.head);
    CallExpr* move = toCallExpr(def->next);
    SymExpr*  arg1 = toSymExpr(move->get(1));

    INT_ASSERT(strcmp(def->sym->name, "_cobeginCount") == 0);
    INT_ASSERT(arg1->symbol()                          == def->sym);

    retval = move->next;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/


void lvalueCheck(CallExpr* call) {
  // Check to ensure the actual supplied to an OUT, INOUT or REF argument
  // is an lvalue.
  for_formals_actuals(formal, actual, call) {
    lvalueCheckActual(call, actual, formal->intent, formal);
  }
}

static CallExpr* findOutIntentCallFromAssign(CallExpr* call,
                                             Expr** outActual,
                                             ArgSymbol** outFormal) {
  // Call is an assign from a temp
  // Find an out/inout argument call setting the temp
  if (call->isNamed("=")) {
    if (SymExpr* lhs = toSymExpr(call->get(1))) {
      if (SymExpr* rhs = toSymExpr(call->get(2))) {
        if (rhs->symbol()->hasFlag(FLAG_TEMP)) {
          for_SymbolDefs(defSe, rhs->symbol()) {
            CallExpr* parentCall = toCallExpr(defSe->parentExpr);
            if (parentCall->resolvedFunction() != NULL) {
              for_formals_actuals(formal, actual, parentCall) {
                if (actual == defSe) {
                  if (formal->intent == INTENT_OUT ||
                      formal->originalIntent == INTENT_OUT ||
                      formal->intent == INTENT_INOUT ||
                      formal->originalIntent == INTENT_INOUT) {
                    *outActual = lhs;
                    *outFormal = formal;
                    return parentCall;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return NULL;
}

static void lvalueCheckActual(CallExpr* call, Expr* actual, IntentTag intent, ArgSymbol* formal) {
  bool constnessError = false;
  bool exprTmpError = false;
  bool errorMsg = false;

  switch (intent) {
   case INTENT_BLANK:
   case INTENT_CONST:
   case INTENT_PARAM:
   case INTENT_TYPE:
   case INTENT_REF_MAYBE_CONST:
    // not checking them here
    break;

   case INTENT_IN:
   case INTENT_CONST_IN:
    // generally, not checking them here
    // but, FLAG_COPY_MUTATES makes INTENT_IN actually modify actual
    if (formal && formal->getValType()->symbol->hasFlag(FLAG_COPY_MUTATES))
      if (!isLegalLvalueActualArg(formal, actual, constnessError, exprTmpError))
        errorMsg = true;
    break;

   case INTENT_OUT:
   case INTENT_INOUT:
   case INTENT_REF:
    if (!isLegalLvalueActualArg(formal, actual, constnessError, exprTmpError)) {
      errorMsg = true;

      // ignore const errors for out until we sort out split-init
      if (exprTmpError == false && intent == INTENT_OUT)
        errorMsg = false;
    }
    break;

   case INTENT_CONST_REF:
    if (!isLegalConstRefActualArg(formal, actual))
      errorMsg = true;
    break;

   default:
    // all intents should be covered above
    INT_ASSERT(false);
    break;
  }

  FnSymbol* nonTaskFnParent = NULL;

  if (errorMsg &&
      // sets nonTaskFnParent
      checkAndUpdateIfLegalFieldOfThis(call, actual, nonTaskFnParent)) {
    errorMsg = false;

    nonTaskFnParent->addFlag(FLAG_MODIFIES_CONST_FIELDS);
  }

  if (errorMsg == true) {
    if (nonTaskFnParent &&
        nonTaskFnParent->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS)) {
      // we are asked to ignore errors here
      return;
    }

    if (SymExpr* se = toSymExpr(actual)) {
      if (se->symbol()->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS)) {
        // Ignore lvalue errors default argument expressions
        return;
      }
    }

    FnSymbol* calleeFn = call->resolvedFunction();

    if (formal) {
      INT_ASSERT(calleeFn == formal->defPoint->parentSymbol); // sanity
    }

    Symbol* actSym = isSymExpr(actual) ? toSymExpr(actual)->symbol() : NULL;
    bool isInit = calleeFn && (calleeFn->isInitializer() ||
                               calleeFn->isCopyInit());
    bool isInitParam = actSym->hasFlag(FLAG_PARAM) && isInit;

    if (isInit && formal->hasFlag(FLAG_ARG_THIS)) {
      // Ignore lvalue errors for 'this' in initializer calls
      return;
    }

    bool isAssign = false;
    if (calleeFn && calleeFn->hasFlag(FLAG_ASSIGNOP))
      isAssign = true;

    if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(call->baseExpr))
      if (urse->unresolved == astrSassign)
        isAssign = true;

    if (isAssign) {
      // If the RHS is a temp, it might be for the out intent

      {
        Expr* outActual = NULL;
        ArgSymbol* outFormal = NULL;
        CallExpr* outCall =
          findOutIntentCallFromAssign(call, &outActual, &outFormal);

        if (outCall != NULL) {

          IntentTag intent = INTENT_OUT;
          if (outFormal->intent == INTENT_INOUT ||
              outFormal->originalIntent == INTENT_INOUT)
            intent = INTENT_INOUT;

          lvalueCheckActual(outCall, outActual, intent, outFormal);
          return;
        }
      }

      // This assert is FYI. Perhaps can remove it if it fails.
      INT_ASSERT(callStack.n > 0 && callStack.v[callStack.n - 1] == call);

      FnSymbol*   fnParent   = toFnSymbol(call->parentSymbol);
      const char* recordName = defaultRecordAssignmentTo(fnParent);

      if (recordName && callStack.n >= 2) {
        // blame on the caller of the caller, if available
        USR_FATAL_CONT(callStack.v[callStack.n - 2],
                       "cannot assign to a record of the type %s using "
                       "the default assignment operator because it has "
                       "'const' field(s)",
                       recordName);
      } else {
        if (constnessError)
          USR_FATAL_CONT(actual, "cannot assign to const variable");
        else
          USR_FATAL_CONT(actual, "illegal lvalue in assignment");
      }

    } else if (isInitParam == false) {
      ModuleSymbol* mod          = calleeFn->getModule();
      char          cn1          = calleeFn->name[0];
      const char*   calleeParens = (isalpha(cn1) || cn1 == '_') ? "()" : "";

      const char* kind = "non-lvalue actual";
      if (constnessError)
        kind = "const actual";

      // Should this be the same condition as in insertLineNumber() ?
      if (developer || mod->modTag == MOD_USER) {
        USR_FATAL_CONT(actual,
                       "%s is passed to %s formal '%s' of %s%s",
                       kind,
                       formal->intentDescrString(),
                       formal->name,
                       calleeFn->name,
                       calleeParens);

      } else {
        USR_FATAL_CONT(actual,
                       "%s is passed to a %s formal of %s%s",
                       kind,
                       formal->intentDescrString(),
                       calleeFn->name,
                       calleeParens);
      }
    }

    if (SymExpr* aSE = toSymExpr(actual)) {
      Symbol* aVar = aSE->symbol();

      if (aVar->hasFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT)) {
        printTaskOrForallConstErrorNote(aVar);
      }
    }
  }
}

void printTaskOrForallConstErrorNote(Symbol* aVar) {
  const char* varname = aVar->name;

  if (strncmp(varname, "_formal_tmp_in_", 15) == 0)
    varname += 15;
  else if (strncmp(varname, "_formal_tmp_", 12) == 0)
    varname += 12;

  if (isArgSymbol(aVar) || aVar->hasFlag(FLAG_TEMP)) {
    Symbol*     enclTaskFn    = aVar->defPoint->parentSymbol;
    BaseAST*    marker        = NULL;
    const char* constructName = NULL;

    if (enclTaskFn->hasFlag(FLAG_BEGIN)) {
      // enclTaskFn points to a good line number
      marker        = enclTaskFn;
      constructName = "begin";

    } else {
      marker        = enclTaskFn->defPoint->parentExpr;
      constructName = "parallel";
    }

    USR_PRINT(marker,
              "The shadow variable '%s' is constant due to task intents "
              "in this %s statement",
              varname,
              constructName);

  } else {
    Expr* enclLoop = aVar->defPoint->parentExpr;

    USR_PRINT(enclLoop,
              "The shadow variable '%s' is constant due to forall intents "
              "in this loop",
              varname);
  }
}

/************************************* | **************************************
*                                                                             *
* This resolves the `(...myTuple)` style of expression form                   *
*                                                                             *
************************************** | *************************************/

static void resolveTupleExpand(CallExpr* call) {
  SymExpr*  sym    = toSymExpr(call->get(1));
  Type*     type   = sym->symbol()->getValType();

  CallExpr* parent = toCallExpr(call->parentExpr);
  CallExpr* noop   = new CallExpr(PRIM_NOOP);
  Expr*     stmt   = call->getStmtExpr();

  int       size   = 0;

  if (type->symbol->hasFlag(FLAG_TUPLE) == false) {
    USR_FATAL(call, "invalid tuple expand primitive");
  }

  for (int i = 0; i < type->substitutions.n && size == 0; i++) {
    SymbolMapElem& elem = type->substitutions.v[i];

    if (elem.key != NULL && strcmp("size", elem.key->name) == 0) {
      size = toVarSymbol(elem.value)->immediate->int_value();
    }
  }

  if (size == 0)
    INT_FATAL(call, "Invalid tuple expand primitive");

  if (parent != NULL && parent->primitive != NULL) {
    if (!parent->isPrimitive(PRIM_ITERATOR_RECORD_SET_SHAPE) &&
        !parent->isPrimitive(PRIM_NEW)) {
      USR_FATAL(parent, "illegal tuple expansion context");
    }
  }

  if (parent != NULL && parent->isPrimitive(PRIM_ITERATOR_RECORD_SET_SHAPE))
    size = 1; // use the first component of the tuple for the shape

  stmt->insertBefore(noop);

  for (int i = 0; i < size; i++) {
    VarSymbol* tmp = newTemp(astr("_tuple_expand_tmp_", istr(i)));
    CallExpr*  e   = NULL;

    tmp->addFlag(FLAG_MAYBE_TYPE);

    if (sym->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
      tmp->addFlag(FLAG_TYPE_VARIABLE);
    }

    call->insertBefore(new SymExpr(tmp));

    if (call->parentSymbol->hasFlag(FLAG_EXPAND_TUPLES_WITH_VALUES) == false) {
      e = new CallExpr(sym->copy(), new_IntSymbol(i));
    } else {
      e = new CallExpr(PRIM_GET_MEMBER_VALUE,
                       sym->copy(),
                       new_CStringSymbol(astr("x", istr(i))));
    }

    stmt->insertBefore(new DefExpr(tmp));
    stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, e));
  }

  call->remove();

  noop->replace(call); // put call back in ast for function resolution

  call->convertToNoop();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void handleSetMemberTypeMismatch(Type*     t,
                                        Symbol*   fs,
                                        CallExpr* call,
                                        SymExpr*  rhs);

// Returns the field Symbol for 'fieldExpr'.
// It must be a field in 'base' type.
static Symbol* resolveFieldSymbol(Type* base, Expr* fieldExpr) {
  AggregateType* ct = toAggregateType(base);
  INT_ASSERT(ct); // required for PRIM_SET_MEMBER

  // fieldExpr must be either a literal or a field.
  // In any case it is a VarSymbol.
  SymExpr* sym = toSymExpr(fieldExpr);
  VarSymbol* var = toVarSymbol(sym->symbol());

  if (var->immediate == NULL) {
    // Confirm that this is already a correct field Symbol.
    INT_ASSERT(var->defPoint->parentSymbol == ct->symbol);
    return var;
  }

  // Get the field name.
  const char* name = var->immediate->v_string.c_str();

  // Special case: An integer field name is actually a tuple member index.
  {
    int64_t i = 0;

    if (get_int(sym, &i) == true) {
      name = astr("x", istr(i));
    }
  }

  for_fields(field, ct) {
    if (!strcmp(field->name, name)) {
      fieldExpr->replace(new SymExpr(field));
      return field;
    }
  }

  INT_ASSERT(false); // did not find field - bad set member primitive
  return NULL;
}

static void resolveSetMember(CallExpr* call) {

  if (call->id == breakOnResolveID) {
    gdbShouldBreakHere();
  }

  Symbol* fs = resolveFieldSymbol(call->get(1)->typeInfo()->getValType(),
                                  call->get(2));

  Type* t = call->get(3)->typeInfo();

  if (t == dtUnknown) {
    INT_FATAL(call, "Unable to resolve field type");
  }

  if (isGenericInstantiation(t, fs->type)) {
    fs->type = t;
  }
  if (fs->type->symbol->hasFlag(FLAG_GENERIC)) {
    if (canInstantiate(t, fs->type))
      fs->type = t;
    else
      USR_FATAL(call->parentSymbol,
                "unable to determine type of generic field");
  }

  if (fs->type == dtUnknown) {
    if (t != dtNil) {
      fs->type = t;

    } else {
      USR_FATAL(call->parentSymbol,
                "unable to determine type of field from nil");
    }
  }

  INT_ASSERT(isSymExpr(call->get(3)));

  handleSetMemberTypeMismatch(t, fs, call, toSymExpr(call->get(3)));
}

static void handleSetMemberTypeMismatch(Type*     t,
                                        Symbol*   fs,
                                        CallExpr* call,
                                        SymExpr*  rhs) {
  if (t->getValType() != fs->getValType() && t != dtNil && t != dtObject) {
    Symbol*   actual = rhs->symbol();
    FnSymbol* fn     = toFnSymbol(call->parentSymbol);

    if (canCoerceTuples(t, actual, fs->type, NULL, fn) == true) {
      // Add a PRIM_MOVE so that insertCasts will take care of it later.
      VarSymbol* tmp = newTemp("coerce_elt", fs->type);

      call->insertBefore(new DefExpr(tmp));
      rhs->remove();
      call->insertBefore(new CallExpr(PRIM_MOVE, tmp, rhs));
      call->insertAtTail(tmp);

    } else {
      USR_FATAL(userCall(call),
                "cannot assign expression of type %s to field '%s' of type %s",
                toString(t),
                fs->name,
                toString(fs->type));
    }
  }
}


/************************************* | **************************************
*                                                                             *
* Resolves calls inserted into initializers during Phase 1,                   *
* to fully determine the instantiated type.                                   *
*                                                                             *
* This function is a combination of resolveMove and resolveSetMember          *
*                                                                             *
************************************** | *************************************/

static void resolveInitField(CallExpr* call) {
  if (call->id == breakOnResolveID) {
    gdbShouldBreakHere();
  }

  INT_ASSERT(call->argList.length == 3);

  // PRIM_INIT_FIELD contains three args:
  // fn->_this, the name of the field, and the value/type it is to be given

  SymExpr* srcExpr = toSymExpr(call->get(3)); // the value/type to give field

  // Get the field name.
  SymExpr* sym = toSymExpr(call->get(2));
  if (!sym)
    INT_FATAL(call, "bad initializer set field primitive");
  VarSymbol* var = toVarSymbol(sym->symbol());
  if (!var || !var->immediate)
    INT_FATAL(call, "bad initializer set field primitive");
  const char* name = astr(var->immediate->v_string.c_str());

  // Get the type
  AggregateType* ct = toAggregateType(call->get(1)->typeInfo()->getValType());
  if (!ct)
    INT_FATAL(call, "bad initializer set field primitive");

  Symbol* fs = NULL;
  int index = 1;
  for_fields(field, ct) {
    if (field->name == name) {
      fs = field;
      break;
    }
    index++;
  }

  if (!fs)
    INT_FATAL(call, "bad initializer set field primitive");

  Symbol* srcSym = srcExpr->symbol();
  Type* srcType = srcSym->type;
  if (srcType == dtUnknown)
    INT_FATAL(call, "Unable to resolve field type");

  if (fs->hasFlag(FLAG_PARAM)) {
    if (isLegalParamType(srcType) == false) {
      USR_FATAL_CONT(fs, "'%s' is not of a supported param type", fs->name);
    }

    // Check against the symbol's Immediate is required because the symbol is
    // likely a temporary, which generally will suppress an error if
    // initialized from a non-param.
    //
    // Note: This unfortunately relies on not checking the error when
    // initializing the temporary, where it would be more difficult to
    // determine the context of the initialization.
    else if (srcSym->isParameter() == false ||
            (isVarSymbol(srcSym) && getImmediate(srcSym) == NULL)) {
      USR_FATAL_CONT(call, "Initializing parameter '%s' to value not known at compile time", fs->name);
    }
  }

  if (srcType == dtNil && fs->type == dtUnknown) {
    USR_FATAL(call->parentSymbol, "unable to determine type of field from nil");
  }

  bool wasGeneric = ct->symbol->hasFlag(FLAG_GENERIC);

  FnSymbol* parentFn = toFnSymbol(call->parentSymbol);
  INT_ASSERT(parentFn);

  if (fs->type->symbol->hasFlag(FLAG_GENERIC))
    fs->type = dtUnknown;

  if (fs->type == dtUnknown) {
    // Update the type of the field.  If necessary, update to a new
    // instantiation of the overarching type (and replaces references to the
    // fields from the old instantiation

    if (fs->id == breakOnResolveID) {
      gdbShouldBreakHere();
    }

    bool ignoredHasDefault = false;

    if ((fs->hasFlag(FLAG_TYPE_VARIABLE) && isTypeExpr(srcExpr)) ||
        fs->hasFlag(FLAG_PARAM) ||
        (fs->defPoint->exprType == NULL && fs->defPoint->init == NULL) ||
        ct->fieldIsGeneric(fs, ignoredHasDefault)) {
      Expr* insnPt = call->getFunction()->instantiationPoint();
      AggregateType* instantiate = ct->getInstantiation(srcSym, index, insnPt);
      if (instantiate != ct) {
        // TODO: make this set of operations a helper function I can call
        INT_ASSERT(parentFn->_this);
        parentFn->_this->type = instantiate;

        SymbolMap fieldTranslate;
        for (int i = 1; i <= instantiate->fields.length; i++) {
          fieldTranslate.put(ct->getField(i), instantiate->getField(i));
        }
        update_symbols(parentFn, &fieldTranslate);

        ct = instantiate;
        fs = instantiate->getField(index);
      }
    } else {
      // The field is not generic.
      if (fs->defPoint->exprType == NULL) {
        fs->type = srcType;
      } else if (fs->defPoint->exprType) {
        Type* exprType = fs->defPoint->exprType->typeInfo();
        if (exprType == dtUnknown) {
          fs->type = srcType;
        } else if (exprType->symbol->hasFlag(FLAG_GENERIC)) {
          // e.g. the type so far is a partial instantiation
          // but the field declaration is concrete
          // TODO: does this need to check srcType is compatible
          // with any partial instantiation?
          fs->type = srcType;
        } else {
          // Try calling resolveGenericActuals in case the field is e.g. range
          CallExpr* dummyCall = new CallExpr(PRIM_NOOP,
                                             new SymExpr(exprType->symbol));
          call->insertBefore(dummyCall);
          resolveGenericActuals(dummyCall);
          fs->type = dummyCall->get(1)->typeInfo();
          dummyCall->remove();
        }
      }
    }
  }

  if (fs->type->getValType() != srcType->getValType()) {
    USR_FATAL_CONT(call, "Cannot replace an instantiated field "
                         "with another type");
    USR_PRINT(call, "field '%s' has type '%s' but is set to '%s'",
                     fs->name,
                     toString(fs->getValType()),
                     toString(srcType->getValType()));
    USR_STOP();
  }
  if (fs->isParameter() && srcSym->isParameter()) {
    Symbol* dstParam = fs;
    Symbol* srcParam = srcSym;
    if (Symbol* s = paramMap.get(dstParam))
      dstParam = s;
    if (Symbol* s = paramMap.get(srcParam))
      srcParam = s;

    if (Immediate* srcImm = getSymbolImmediate(srcParam)) {
      if (Immediate* dstImm = getSymbolImmediate(dstParam)) {
        if (srcImm != dstImm) {
          USR_FATAL_CONT(call, "Cannot replace an instantiated param field "
                               "with another value");
          VarSymbol* dstVar = toVarSymbol(dstParam);
          VarSymbol* srcVar = toVarSymbol(srcParam);
          if (dstVar != NULL && srcVar != NULL)
            USR_PRINT(call, "field '%s' has value '%s' but is set to '%s'",
                            fs->name,
                            toString(dstVar, false), toString(srcVar, false));
          USR_STOP();
        }
      }
    }
  }

  // Type was just fully instantiated, let's try to find its promotion type.
  if (wasGeneric                        == true &&
      ct->symbol->hasFlag(FLAG_GENERIC) == false) {

    resolveTypeWithInitializer(ct, parentFn);
    // BHARSH INIT TODO: Would like to resolve destructor here, but field
    // types are not fully resolved. E.g., "var foo : t"
  } else if (developer == false &&
             ct->symbol->hasFlag(FLAG_BASE_ARRAY) == false &&
             isArrayClass(ct) &&
             strcmp(fs->name, "dom") == 0) {
    fixTypeNames(ct);
  }

  call->primitive = primitives[PRIM_SET_MEMBER];

  setDefinedConstForPrimSetMemberIfApplicable(call);

  resolveSetMember(call); // Can we remove some of the above with this?
}

static bool managementMismatch(Type* lhs, Type* rhs) {
  if (isManagedPtrType(lhs)) {
    if (isManagedPtrType(rhs)) {
      // Here, only (owned <-- shared) is disallowed.
      return (! strncmp(lhs->symbol->name, "_owned(", 7)  ||
              ! strncmp(lhs->symbol->name, "owned ",  6)   )  &&
             (! strncmp(rhs->symbol->name, "_shared(", 8) ||
              ! strncmp(rhs->symbol->name, "shared ",  7)  );
    } else if (isUnmanagedClass(rhs)    ||
               isBorrowedClass(rhs) ) {
      return true;
    }
  } else if (isUnmanagedClass(lhs)) {
    if (isOwnedOrSharedOrBorrowed(rhs))
      return true;
  }
  // Otherwise it's fine, ex. OK to borrow from anything.
  return false;
}

// Looks for this pattern:
//   init var( temp, initValue )
//   init field( parentAggregate, "fieldName", temp )
// and returns the 'init field' call.
static CallExpr* fieldInitFrom(Symbol* temp) {
  // Todo: have PRIM_INIT_VAR be considered a "def" by isDefAndOrUse(),
  // so we can simply use "temp->getSingleUse()".
  for_SymbolSymExprs(se, temp)
    if (CallExpr* parent = toCallExpr(se->parentExpr))
      if (parent->isPrimitive(PRIM_INIT_FIELD) &&  se == parent->get(3))
        return parent;

  return NULL;
}

static const char* describeFieldInit(const char* parent, const char* field,
                                     const char* nonnilable) {
    return astr("initialize field ", parent, ".", field, " of",
                nonnilable, " type");
}

static const char* describeLHS(CallExpr* call, const char* nonnilable) {
  if (call->isPrimitive(PRIM_INIT_VAR)) {
    Symbol* lhs = toSymExpr(call->get(1))->symbol();
    if (lhs->hasFlag(FLAG_TEMP)) {
      if (CallExpr* fieldInit = fieldInitFrom(lhs))
        return describeFieldInit(fieldInit->get(1)->getValType()->symbol->name,
                                 get_string(fieldInit->get(2)), nonnilable);
    } else {
      return astr("initialize variable '", lhs->name,
                  "' of", nonnilable, " type");
    }
  }

  // Nothing clicked. Assume assignment.
  return astr("assign to a", nonnilable);
}

void checkMoveIntoClass(CallExpr* call, Type* lhs, Type* rhs) {
  if (isNonNilableClassType(lhs) && isNilableClassType(rhs))
    USR_FATAL(userCall(call), "cannot %s '%s' from a nilable '%s'",
              describeLHS(call, " non-nilable"), toString(lhs), toString(rhs));

  if (managementMismatch(lhs, rhs))
    USR_FATAL(userCall(call), "cannot %s '%s' from a '%s'",
              describeLHS(call, ""), toString(lhs), toString(rhs));

  // warn for misleading new in init or split-init
  warnIfMisleadingNew(call, lhs, rhs);
}


void warnForIntUintConversion(BaseAST* context,
                              Type* formalType,
                              Type* actualType,
                              Symbol* actual) {
  if (fWarnIntUint || fWarnUnstable) {
    Type* formalVt = formalType->getValType();
    Type* actualVt = actualType->getValType();
    if (is_uint_type(formalVt) && is_int_type(actualVt)) {
      if (get_width(formalVt) <= get_width(actualVt)) {
        bool isParam = false;
        bool isNegParam = false;

        if (VarSymbol* var = toVarSymbol(actual)) {
          if (Immediate* imm = var->immediate) {
            isParam = true;
            isNegParam = is_negative(imm);
          }
        }

        // If it's a param, warn only if it is negative.
        // Otherwise, warn.
        if (isNegParam || !isParam) {
          if (fWarnIntUint) {
            USR_WARN(context, "potentially surprising int->uint implicit conversion");
          } else {
            USR_WARN(context, "int->uint implicit conversion is unstable");
          }
          USR_PRINT(context, "add a cast :%s to avoid this warning",
                    toString(formalVt));
        }
      }
    }
  }
}


/************************************* | **************************************
*                                                                             *
* This handles expressions of the form                                        *
*      CallExpr(PRIM_INIT_VAR, dst, src, [optional type to coerce to])        *
*                                                                             *
* 2017/03/06: This initial, trivial, implementation converts this to either   *
*                                                                             *
*      CallExpr(PRIM_MOVE, dst, src)                                          *
*                                                                             *
* or                                                                          *
*                                                                             *
*      CallExpr(PRIM_MOVE, dst, CallExpr("init_copy", src))                   *
*                                                                             *
* and then resolves the PRIM_MOVE.                                            *
*                                                                             *
************************************** | *************************************/

void resolveInitVar(CallExpr* call) {
  Symbol*  dst     = toSymExpr(call->get(1))->symbol();

  SymExpr* srcExpr = toSymExpr(call->get(2));
  Symbol*  src     = srcExpr->symbol();
  Type*    srcType = src->type;

  bool isParamString = dst->hasFlag(FLAG_PARAM) && isString(srcType);

  if (call->id == breakOnResolveID) {
    gdbShouldBreakHere();
  }

  if (call->isPrimitive(PRIM_INIT_VAR_SPLIT_INIT)) {
    // Also, check that the types are compatible for split initialization
    // with type inference.
    if (call->numActuals() < 3)
      if (dst->type != dtUnknown)
        if (dst->type->getValType() != srcType->getValType())
          USR_FATAL_CONT(call, "Split initialization uses multiple types; "
                               "another initialization has type %s "
                               "but this initialization has type %s",
                               toString(dst->type->getValType()),
                               toString(srcType->getValType()));

    // Also, if the dst is a param, check that the src values are
    // an acceptable param value, and if there are many, the same param value.
    if (dst->isParameter()) {
      // Check that they are initialized to the same value.
      Symbol* dstParam = dst;
      Symbol* srcParam = src;
      if (Symbol* s = paramMap.get(dst))
        dstParam = s;
      if (Symbol* s = paramMap.get(src))
        srcParam = s;

      if (Immediate* srcImm = getSymbolImmediate(srcParam)) {
        if (Immediate* dstImm = getSymbolImmediate(dstParam)) {
          if (dstImm != srcImm) {
            USR_FATAL_CONT(call, "Split initialization sets parameter to "
                                 "different values");
            call->convertToNoop();
            return;
          }
        }
      }
    }
  }

  Type* targetType = NULL;
  bool addedCoerce = false;

  bool inferType = call->numActuals() == 2;

  if (call->numActuals() > 3) {
    INT_FATAL(call, "unexpected number of actuals in variable init call");
  }

  // Handle noinit
  if (srcExpr && srcExpr->symbol() == gNoInit) {
    if (call->numActuals() < 3) {
      // no init needs a type, cannot infer from gNoInit.
      USR_FATAL(call, "cannot use noinit on the variable '%s' "
                      "declared without a type", dst->name);
    }

    SymExpr* targetTypeExpr = toSymExpr(call->get(3)->remove());
    targetType = targetTypeExpr->typeInfo();

    if (targetType->symbol->hasFlag(FLAG_GENERIC)) {
      // no init needs a concrete type, cannot infer from gNoInit.
      USR_FATAL(call, "cannot use noinit on the variable '%s' "
                      "declared with generic type", dst->name);
    }

    // Since we are not initializing, just set the variable's type
    dst->type = targetType;
    call->primitive = primitives[PRIM_NOOP];
    return;
  }

  if (call->numActuals() == 3) {
    SymExpr* targetTypeExpr = toSymExpr(call->get(3)->remove());
    targetType = targetTypeExpr->typeInfo();

    // PRIM_INIT_VAR has dst, src like PRIM_MOVE so we can reuse some checking
    if (moveIsAcceptable(call) == false)
      moveHaltMoveIsUnacceptable(call);

    bool genericTgt = targetType->symbol->hasFlag(FLAG_GENERIC);
    // If the target type is generic, compute the appropriate instantiation
    // type.
    if (genericTgt) {
      Type* inst = getInstantiationType(srcType, NULL, targetType, NULL, call,
                                        /* allowCoercion */ true,
                                        /* implicitBang */ false,
                                        /* inOrOtherValue */ true);

      // Does not allow initializations of the form:
      //   var x : MyGenericType = <expr>;
      // if the type of <expr> is not an instantiation of 'MyGenericType'.
      // Left as future work until 'init=' design finalized.
      if (inst == NULL && isRecord(targetType) == false) {
        USR_FATAL(call, "Could not coerce '%s' to '%s' in initialization",
                  srcType->symbol->name,
                  targetType->symbol->name);
      } else if (inst != NULL) {
        targetType = inst;
      }
    }

    bool mismatch = targetType->getValType() != srcType->getValType();
    // Insert a coercion if the types are different. Some internal types use a
    // coercion because their initCopy returns a different type.
    if ((targetType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) && !genericTgt) ||
        (targetType->symbol->hasFlag(FLAG_TUPLE) && mismatch) ||
        (isRecord(targetType->getValType()) == false && mismatch)) {

      if (mismatch)
        checkMoveIntoClass(call, targetType->getValType(), src->getValType());

      VarSymbol* tmp = newTemp(astr_init_coerce_tmp, targetType);
      if (dst->hasFlag(FLAG_PARAM))
        tmp->addFlag(FLAG_PARAM);

      // this protects against issues with coercing from sync int to int
      if ((targetType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) && !genericTgt)) {
        if (dst->hasFlag(FLAG_CONST)) {
          tmp->addFlag(FLAG_CONST);
        }
      }

      CallExpr* coerce = new CallExpr(PRIM_COERCE,
                                      srcExpr->copy(),
                                      targetTypeExpr);
      CallExpr* move = new CallExpr(PRIM_MOVE, tmp, coerce);

      call->insertBefore(new DefExpr(tmp));
      call->insertBefore(move);
      resolveCoerce(coerce);
      resolveExpr(move);

      // Use the targetType as the srcType
      srcType = targetType;
      // Set the source expression to the result of coercion
      if (Symbol* p = paramMap.get(tmp))
        srcExpr->setSymbol(p); // Use the right param value if we computed one.
      else
        srcExpr->setSymbol(tmp);

      addedCoerce = true;
    }
  } else {
    targetType = srcType;
  }

  bool srcSyncSingle = isSyncType(srcType->getValType()) ||
                       isSingleType(srcType->getValType());

  // This is a workaround to avoid deprecation warnings for sync/single
  // variables within compiler-generated initializers.
  FnSymbol* inFn = call->getFunction();
  if (srcSyncSingle &&
      inFn->hasFlag(FLAG_COMPILER_GENERATED) &&
      (inFn->name == astrInit || inFn->name == astrInitEquals) &&
      targetType->getValType() == srcType->getValType()) {

    targetType = targetType->getValType();

    // change
    //   PRIM_INIT_VAR lhsSync, rhsSync
    // to
    //   PRIM_MOVE lhsSync, chpl__compilerGeneratedCopySyncSingle(rhsSync)

    call->primitive = primitives[PRIM_MOVE];
    srcExpr->remove();
    CallExpr* clone = new CallExpr("chpl__compilerGeneratedCopySyncSingle",
                                   srcExpr);
    call->insertAtTail(clone);
    resolveExpr(clone);
    resolveMove(call);

    return;
  }



  // 'var x = new _domain(...)' should not bother going through chpl__initCopy
  // logic so that the result of the 'new' is MOVE'd and not copy-initialized,
  // which is handled in the 'init=' branch.
  bool isDomain = targetType->getValType()->symbol->hasFlag(FLAG_DOMAIN);
  bool isDomainWithoutNew = isDomain &&
                            src->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW) == false;
  bool initCopySyncSingle = inferType && srcSyncSingle;
  bool initCopyIter = inferType && srcType->getValType()->symbol->hasFlag(FLAG_ITERATOR_RECORD);

  if (isDomain &&
      srcType && !srcType->getValType()->symbol->hasFlag(FLAG_DOMAIN)) {
    USR_FATAL(call, "Domains cannot be initialized using expressions of non-domain type");
  }

  if (dst->hasFlag(FLAG_NO_COPY) ||
      isPrimitiveScalar(targetType) ||
      isEnumType(targetType) ||
      addedCoerce) {
    dst->type = targetType;

    call->primitive = primitives[PRIM_MOVE];
    if (srcType->isRef() && !dst->hasFlag(FLAG_NO_COPY)) {
      // don't infer the type of the LHS to be a ref type
      srcExpr->remove();
      call->insertAtTail(new CallExpr(PRIM_DEREF, srcExpr));
    }
    resolveMove(call);

  } else if (isRecord(targetType->getValType()) == false ||
             isParamString ||
             targetType->getValType()->symbol->hasFlag(FLAG_ARRAY) ||
             isDomainWithoutNew ||
             initCopySyncSingle ||
             (targetType->getValType()->symbol->hasFlag(FLAG_TUPLE) &&
              srcType->getValType()->symbol->hasFlag(FLAG_TUPLE)) ||
             initCopyIter) {
    // These cases require an initCopy to implement special initialization
    // semantics (e.g. reading a sync for variable initialization).
    //
    // For example, even though domains can leverage 'init=' for basic
    // copy-initialization, the compiler only currently knows about calls to
    // 'chpl__initCopy' and how to turn them into something else when necessary

    // Normally e.g. var y = foo() - where foo returns by value - will not add a
    // copy and so the result of foo() need not be auto-destroyed.  However, if
    // foo() is returning an array slice, then it is copied from and the source
    // of the copy does need to be destroyed.
    if (SymExpr* rhsSe = toSymExpr(srcExpr))
      if (VarSymbol* rhsVar = toVarSymbol(rhsSe->symbol()))
        if (isAliasingArrayType(rhsVar->getValType()))
          if (rhsVar->hasFlag(FLAG_NO_AUTO_DESTROY) == false)
            rhsVar->addFlag(FLAG_INSERT_AUTO_DESTROY);

    Symbol *definedConst = dst->hasFlag(FLAG_CONST)? gTrue : gFalse;
    CallExpr* initCopy = new CallExpr(astr_initCopy, srcExpr->remove(),
                                                     definedConst);
    call->insertAtTail(initCopy);
    call->primitive = primitives[PRIM_MOVE];

    if (initCopyIter == false) {
      // If there is an error in that initCopy call,
      // just mark it for later (rather than raising the error now)
      // since the initCopy might be removed later in compilation.
      inTryResolve++;
      tryResolveStates.push_back(CHECK_CALLABLE_ONLY);

      resolveExpr(initCopy);
      resolveMove(call);

      tryResolveStates.pop_back();
      inTryResolve--;
    } else {
      // give errors for init-copy on iterators now
      resolveExpr(initCopy);
      resolveMove(call);
    }

    if (isAliasingArrayType(srcExpr->getValType()) || initCopyIter)
      if (FnSymbol* initCopyFn = initCopy->resolvedFunction())
        if (initCopyFn->retType == srcExpr->getValType())
          INT_FATAL("Expected different return type for this initCopy");

  } else if (isRecord(targetType->getValType())) {
    AggregateType* at = toAggregateType(targetType->getValType());

    // If the RHS is a temp that is the result of a ContextCallExpr,
    // make the choice now about which ContextCallExpr to use
    // (and prefer a value if available, and const ref if not).
    // Update 'src' and 'srcType' so that the below logic about
    // whether or not to copy applies.
    if (src->hasFlag(FLAG_TEMP)) {
      ContextCallExpr* ccRhs = nullptr;
      int nLhsDefsFound = 0;

      for_SymbolSymExprs(se, src) {
        if (CallExpr* inCall = toCallExpr(se->parentExpr)) {
          if (inCall->isPrimitive(PRIM_MOVE) ||
              inCall->isPrimitive(PRIM_ASSIGN)) {
            if (SymExpr* inCallLhs = toSymExpr(inCall->get(1))) {
              if (inCallLhs->symbol() == src) {
                nLhsDefsFound++;
                if (ContextCallExpr* cc = toContextCallExpr(inCall->get(2))) {
                  ccRhs = cc;
                }
              }
            }
          }
        }
      }

      if (nLhsDefsFound == 1 && ccRhs != nullptr) {
        CallExpr* refCall = nullptr;
        CallExpr* valueCall = nullptr;
        CallExpr* constRefCall = nullptr;
        ccRhs->getCalls(refCall, valueCall, constRefCall);

        if (valueCall) {
          // replace the ContextCallExpr with the valueCall
          // and adjust the type of src
          valueCall->remove();
          ccRhs->replace(valueCall);
          srcType = valueCall->getValType();
          src->type = srcType;
        }
        // Other situations will be handled later in cullOverReferences.
      }
    }


    // Clear FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW
    // since the result of the 'new' will "move" into
    // the variable we are initializing.
    src->removeFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW);

    // Don't need to copy string/bytes literals when initializing
    bool moveStringLiteral = src->hasFlag(FLAG_CHAPEL_STRING_LITERAL) &&
                             targetType->getValType() == dtString;
    bool moveBytesLiteral  = src->hasFlag(FLAG_CHAPEL_BYTES_LITERAL) &&
                             targetType->getValType() == dtBytes;

    // The LHS will "own" the temp without an auto-destroy, provided the
    // types are the same.
    bool canStealTemp = src->hasFlag(FLAG_TEMP) &&
                        src->hasFlag(FLAG_INSERT_AUTO_DESTROY) == false &&
                        srcType->getValType() == targetType->getValType() &&
                        src->isRef() == false;

    bool canStealCallTemp = src->hasFlag(FLAG_EXPR_TEMP) &&
                            srcType->getValType() == targetType->getValType() &&
                            src->isRef() == false;

    if (moveStringLiteral || moveBytesLiteral ||
        canStealTemp || canStealCallTemp) {
      dst->type = src->type;

      call->primitive = primitives[PRIM_MOVE];

      resolveMove(call);

      if (canStealCallTemp)
        src->addFlag(FLAG_NO_AUTO_DESTROY);

    } else if (isRecordOrUnionWithInitializers(at) == false) {
      INT_FATAL("Unable to initialize record/union variable with type '%s'", at->symbol->name);
    } else if (targetType->getValType() == srcType->getValType() &&
               targetType->getValType()->symbol->hasFlag(FLAG_POD)) {
      dst->type = targetType->getValType();
      call->primitive = primitives[PRIM_MOVE];

      // Need to dereference in order to avoid const-ness issues
      if (srcType->isRef()) {
        srcExpr->remove();
        call->insertAtTail(new CallExpr(PRIM_DEREF, srcExpr));
      }

      resolveMove(call);

    } else {

      dst->type = targetType->getValType();
      src->addFlag(FLAG_INSERT_AUTO_DESTROY);

      call->insertAtHead(gMethodToken);

      call->setUnresolvedFunction(astrInitEquals);

      // If there is an error in that initCopy call,
      // just mark it for later (rather than raising the error now)
      // since the initCopy might be removed later in compilation.
      inTryResolve++;
      tryResolveStates.push_back(CHECK_CALLABLE_ONLY);

      resolveExpr(call);

      tryResolveStates.pop_back();
      inTryResolve--;

      dst->type = call->resolvedFunction()->_this->getValType();

      if (at->hasPostInitializer() == true) {
        call->insertAfter(new CallExpr("postinit", gMethodToken, dst));
      }

    }

  } else {
    INT_FATAL("Unhandled case when resolving PRIM_INIT_VAR");
  }
}

/************************************* | **************************************
*                                                                             *
* Detect if there is a copy initializer by attempting to resolve              *
*                                                                             *
*    tmpAt.init(tmpAt, tmpAt);                                                *
*                                                                             *
* where tmpAt is a temp of type at.                                           *
*                                                                             *
************************************** | *************************************/

static FnSymbol* fixInstantiationPointAndTryResolveBody(AggregateType* at,
                                                        CallExpr* call);

FnSymbol* findCopyInitFn(AggregateType* at, const char*& err) {
  VarSymbol* tmpAt = newTemp(at);

  CallExpr* call = NULL;

  if (at->symbol->hasFlag(FLAG_TUPLE)) {
    call = new CallExpr(astr_initCopy, tmpAt,
                        /* definedConst = */gFalse);

  } else {
    call = new CallExpr(astrInitEquals, gMethodToken, tmpAt, tmpAt);
  }

  FnSymbol* foundFn = resolveUninsertedCall(at, call, false);
  // foundFn's instantiationPoint points to the dummy BlockStmt created by
  // resolveUninsertedCall, so it needs to be updated.
  FnSymbol* resolvedFn = fixInstantiationPointAndTryResolveBody(at, call);
  FnSymbol* ret = NULL;

  if (foundFn != NULL && resolvedFn == NULL)
    if (tryResolveErrors.count(foundFn) != 0)
      err = tryResolveErrors[foundFn].second;

  if (foundFn != NULL)
    ret = resolvedFn;

  return ret;
}

FnSymbol* findAssignFn(AggregateType* at) {
  VarSymbol* tmpAt = newTemp(at);

  CallExpr* call = new CallExpr("=", tmpAt, tmpAt);

  FnSymbol* foundFn = resolveUninsertedCall(at, call, false);
  FnSymbol* resolvedFn = fixInstantiationPointAndTryResolveBody(at, call);
  FnSymbol* ret = NULL;

  if (foundFn != NULL)
    ret = resolvedFn;

  return ret;
}

FnSymbol* findZeroArgInitFn(AggregateType* at) {
  SET_LINENO(at->symbol->defPoint);

  VarSymbol* tmpAt = newTemp(at);

  // Do this to avoid a call to 'createGenericRecordVarDefaultInitCall'
  // emitting errors about missing runtime types.
  tmpAt->addFlag(FLAG_UNSAFE);

  CallExpr* call = NULL;

  // non-generic records with initializers
  if (at                                           != NULL &&
      at->instantiatedFrom                         == NULL &&
      isNonGenericRecordWithInitializers(at)       == true) {

    call = new CallExpr(astrInit, gMethodToken, tmpAt);

  // extern types (but not memory_order)
  } else if (at->symbol->hasFlag(FLAG_EXTERN) &&
             !at->symbol->hasFlag(FLAG_C_MEMORY_ORDER_TYPE)) {

    call = NULL; // not handled

  // generic records with initializers
  } else if (at != NULL && at->symbol->hasFlag(FLAG_TUPLE) == false &&
            (at->isRecord() || at->isUnion())) {

    BlockStmt* tmpBlock = new BlockStmt(BLOCK_SCOPELESS);
    Expr* where = getInsertPointForTypeFunction(at);
    if (BlockStmt* stmt = toBlockStmt(where))
      stmt->insertAtHead(tmpBlock);
    else
      where->insertBefore(tmpBlock);

    DefExpr* def = new DefExpr(tmpAt);
    tmpBlock->insertAtTail(def);

    call = createGenericRecordVarDefaultInitCall(tmpAt, at, def);

    tmpBlock->remove();

  // other types (sync, single, ..)
  } else {
    call = new CallExpr("_defaultOf", at->symbol);
  }

  if (call == NULL)
    return NULL;

  FnSymbol* foundFn = resolveUninsertedCall(at, call, false);
  FnSymbol* resolvedFn = fixInstantiationPointAndTryResolveBody(at, call);
  FnSymbol* ret = NULL;

  if (foundFn != NULL)
    ret = resolvedFn;

  return ret;
}

static FnSymbol* fixInstantiationPointAndTryResolveBody(AggregateType* at,
                                                        CallExpr* call) {

  if (FnSymbol* fn = call->resolvedFunction()) {
    if (fn->instantiatedFrom != NULL) {
      // it is a generic function, so make sure to set instantiationPoint
      BlockStmt* point = NULL;
      if (at->symbol->instantiationPoint == NULL) {
        // If the type doesn't have an instantiation point, use its defPoint
        point = getInstantiationPoint(at->symbol->defPoint);
        // Unless it is an iterator record - in that case use the
        // instantiation point for the iterator if there is one.
        if (at->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
          IteratorInfo* ii = at->iteratorInfo;
          if (ii != NULL && ii->iterator != NULL) {
            BlockStmt* iterPt = ii->iterator->instantiationPoint();
            if (iterPt != NULL)
              point = iterPt;
          }
        }
      } else {
        point = at->symbol->instantiationPoint;
      }
      INT_ASSERT(point != NULL);
      fn->setInstantiationPoint(point);
    }

    inTryResolve++;
    tryResolveStates.push_back(CHECK_BODY_RESOLVES);
    tryResolveFunctions.push_back(fn);

    resolveFunction(fn);

    check_state_t state = tryResolveStates.back();

    tryResolveFunctions.pop_back();
    tryResolveStates.pop_back();
    inTryResolve--;

    // return the function if no compilerError was encountered
    if (state != CHECK_FAILED)
      return fn;
  }

  return NULL;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool  moveSupportsUnresolvedFunctionReturn(CallExpr* call);

static bool  isIfExprResult(Expr* LHS);

static Type* moveDetermineRhsTypeErrorIfInvalid(CallExpr* call);

static void  moveHaltForUnacceptableTypes(CallExpr* call);

static void  resolveMoveForRhsSymExpr(CallExpr* call, SymExpr* rhs);

static void  resolveMoveForRhsCallExpr(CallExpr* call, Type* rhsType);

static void  moveSetConstFlagsAndCheck(CallExpr* call, CallExpr* rhs);

static void  moveSetFlagsAndCheckForConstAccess(Symbol*   lhs,
                                                CallExpr* rhs,
                                                FnSymbol* resolvedFn);

static void  moveSetFlagsForConstAccess(Symbol*   lhs,
                                        CallExpr* rhs,
                                        Symbol*   baseSym,
                                        bool      refConstWCT);

static void  moveFinalize(CallExpr* call);

// Helper: is this a move from the result of main()?
static bool isMoveFromMain(CallExpr* call) {
  INT_ASSERT(call->isPrimitive(PRIM_MOVE) ||
             call->isPrimitive(PRIM_ASSIGN)); // caller responsibility
  if (CallExpr* rhs = toCallExpr(call->get(2)))
    if (FnSymbol* target = rhs->resolvedFunction())
      if (target == chplUserMain)
        return true;
  return false;
}

static void resolveMove(CallExpr* call) {
  if (call->id == breakOnResolveID) {
    gdbShouldBreakHere();
  }

  if (moveIsAcceptable(call) == false) {
    // NB: This call will not return
    moveHaltMoveIsUnacceptable(call);


  // Ignore moves to RVV unless this is a constructor
  } else if (moveSupportsUnresolvedFunctionReturn(call) == true) {

  // Ignore moves to if-expr result, the type will be determined later by
  // 'resolveIfExprType'
  } else if (isIfExprResult(call->get(1))) {


  } else {
    // These calls might modify the fields in call
    Type* rhsType = moveDetermineRhsTypeErrorIfInvalid(call);
    Type* lhsType = moveDetermineLhsType(call);
    Expr* rhs     = call->get(2);

    if (moveTypesAreAcceptable(lhsType, rhsType) == false) {
      // NB: This call will not return
      moveHaltForUnacceptableTypes(call);

    } else if (SymExpr* rhsSymExpr = toSymExpr(rhs)) {
      resolveMoveForRhsSymExpr(call, rhsSymExpr);

    } else if (isCallExpr(rhs) == true) {
      resolveMoveForRhsCallExpr(call, rhsType);

    } else {
      INT_ASSERT(false);
    }
  }
}

//
//
//

bool moveIsAcceptable(CallExpr* call) {
  Symbol* lhsSym = toSymExpr(call->get(1))->symbol();
  Expr*   rhs    = call->get(2);
  bool    retval = true;

  // Handle IfExpr results in 'resolveIfExprType' for better error message
  if (isTypeExpr(rhs) == false) {
    if (lhsSym->hasFlag(FLAG_TYPE_VARIABLE) == true &&
        lhsSym->hasFlag(FLAG_IF_EXPR_RESULT) == false) {
      FnSymbol* fn = toFnSymbol(call->parentSymbol);

      if (fn->getReturnSymbol()                  != lhsSym ||
          fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN) == false) {
        retval = false;
      }
    }

  } else {
    if (lhsSym->hasFlag(FLAG_TYPE_VARIABLE) == false &&
        lhsSym->hasFlag(FLAG_MAYBE_TYPE)    == false &&
        lhsSym->hasFlag(FLAG_IF_EXPR_RESULT) == false) {
      retval = false;
    }
  }

  return retval;
}

static void moveHaltMoveIsUnacceptable(CallExpr* call) {
  Symbol* lhsSym = toSymExpr(call->get(1))->symbol();
  Expr*   rhs    = call->get(2);

  if (isTypeExpr(rhs) == false) {
    if (lhsSym->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      FnSymbol* fn = toFnSymbol(call->parentSymbol);

      if (fn->getReturnSymbol() != lhsSym) {
        USR_FATAL(call, "illegal assignment of value to type");

      } else if (fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN) == false) {
        USR_FATAL(call, "illegal return of value where type is expected");
      }
    }

  } else {
    if (lhsSym->hasFlag(FLAG_TYPE_VARIABLE) == false &&
        lhsSym->hasFlag(FLAG_MAYBE_TYPE)    == false) {
      FnSymbol* fn = toFnSymbol(call->parentSymbol);

      if (fn->getReturnSymbol() == lhsSym) {
        USR_FATAL(call, "illegal return of type where value is expected");

      } else if (lhsSym->hasFlag(FLAG_CHPL__ITER) == true) {
        USR_FATAL(call,
                  "unable to iterate over type '%s'",
                  toString(rhs->getValType()));

      } else {
        USR_FATAL(call, "illegal assignment of type to value");
      }
    }
  }
}

//
// Return true if the move supports a return from a function.
// NB this does not include a constructor
// or a function with a known concrete return type.
//
static bool moveSupportsUnresolvedFunctionReturn(CallExpr* call) {
  bool retval = false;

  if (FnSymbol* fn = toFnSymbol(call->parentSymbol)) {
    Symbol* lhsSym = toSymExpr(call->get(1))->symbol();

    if (isUnresolvedOrGenericReturnType(fn->retType) &&
        fn->getReturnSymbol() == lhsSym          && // LHS is the RVV
        fn->_this             != lhsSym          && // Not a constructor
        call->parentExpr      != fn->where       &&
        call->parentExpr      != fn->retExprType) {
      retval = true;
    }
  }

  return retval;
}

static bool isIfExprResult(Expr* LHS) {
  bool ret = false;

  if (SymExpr* se = toSymExpr(LHS)) {
    ret = se->symbol()->hasFlag(FLAG_IF_EXPR_RESULT);
  }

  return ret;
}

//
//
//

static Type* moveDetermineRhsTypeErrorIfInvalid(CallExpr* call) {
  Type* ret = moveDetermineRhsType(call);

  if (ret == dtVoid) {
    Expr* rhs = call->get(2);
    if (CallExpr* rhsCall = toCallExpr(rhs)) {
      if (FnSymbol* rhsFn = rhsCall->resolvedFunction()) {
        // `expandExternArrayCalls` can add void assignments when
        // the extern function has an explicit void return type.
        // Let those through by looking for two flags it adds.
        if (!rhsFn->hasFlag(FLAG_INIT_COPY_FN) &&
            !(rhsFn->hasFlag(FLAG_VOID_NO_RETURN_VALUE) &&
              rhsFn->hasFlag(FLAG_EXTERN_FN_WITH_ARRAY_ARG))) {
          const char* rhsName = rhsFn->name;
          if (rhsFn->hasFlag(FLAG_PROMOTION_WRAPPER))
            rhsName = unwrapFnName(rhsFn);

          // TODO <June 23 Release>: this deprecation-specific error message should be removed
          //                         when the bool-returning-writers deprecation is finalized
          if (
            rhsFn->getModule()->modTag != MOD_USER && (
              // writer methods on channels
              (rhsFn->isMethod() == 1 &&
                (
                    std::strcmp("write", rhsName) == 0 ||
                    std::strcmp("writeln", rhsName) == 0 ||
                    std::strcmp("writeBits", rhsName) == 0 ||
                    std::strcmp("writeBytes", rhsName) == 0 ||
                    std::strcmp("writef", rhsName) == 0
                )
              ) ||
              // or, top-level writef
              std::strcmp("writef", rhsName) == 0
            )
          ) {
            // emit the write* specific error message:
            // (https://github.com/chapel-lang/chapel/pull/20907#pullrequestreview-1155017128)
            USR_FATAL(userCall(call),
                    "illegal use of return value from '%s'. "
                    "Note: if you wish to detect early EOF from a write call, "
                    "check for an EofError using a try-catch block",
                    rhsName);
          } else {
            // otherwise, emit the normal error message:
            USR_FATAL(userCall(call),
                    "illegal use of function that does not "
                    "return a value: '%s'",
                    rhsName);
          }
        }
      }
    }
  }

  return ret;
}

// Determine type of RHS.
// NB: This function may update the RHS
Type* moveDetermineRhsType(CallExpr* call) {
  Symbol* lhsSym = toSymExpr(call->get(1))->symbol();
  Expr*   rhs    = call->get(2);
  Type*   retval = rhs->typeInfo();

  // Workaround for order-of-resolution problems with extern type aliases
  if (retval == dtUnknown) {
    bool rhsIsTypeExpr = isTypeExpr(rhs);

    if (rhsIsTypeExpr == true && isSymExpr(rhs) == true) {
      // Try resolving type aliases now.
      retval = resolveTypeAlias(toSymExpr(rhs));
    }
  }

  // Workaround for problems where the _iterator in buildForLoopExpr would
  // be an _array instead of a ref(_array) in 4-init-array-forexpr.chpl.
  // This could be improved with QualifiedType.
  if (lhsSym->hasFlag(FLAG_MAYBE_REF) ==  true &&
      isReferenceType(retval)         == false) {
    if (SymExpr* se = toSymExpr(rhs)) {
      if (ArgSymbol* arg = toArgSymbol(se->symbol())) {
        if (concreteIntent(arg->intent, arg->type) & INTENT_FLAG_REF) {
          makeRefType(retval);

          retval = retval->refType;

          // Add PRIM_ADDR_OF
          // (this won't be necessary with QualifiedType/PRIM_SET_REFERENCE)
          VarSymbol* addrOfTmp = newTemp("moveAddr", retval);
          CallExpr*  addrOf    = new CallExpr(PRIM_ADDR_OF, rhs->copy());
          SymExpr*   newRhs    = new SymExpr(addrOfTmp);

          call->insertBefore(new DefExpr(addrOfTmp));
          call->insertBefore(new CallExpr(PRIM_MOVE, addrOfTmp, addrOf));

          rhs->replace(newRhs);
        }
      }
    }
  }

  return retval;
}

Type* moveDetermineLhsType(CallExpr* call) {
  Symbol* lhsSym = toSymExpr(call->get(1))->symbol();

  if (lhsSym->type == dtUnknown || lhsSym->type == dtNil) {
    if (lhsSym->id == breakOnResolveID) {
      gdbShouldBreakHere();
    }

    Type* type = call->get(2)->typeInfo();
    if (call->isPrimitive(PRIM_ASSIGN))
      type = type->getValType();

    lhsSym->type = type;
  }

  return lhsSym->type;
}

//
//
//

bool moveTypesAreAcceptable(Type* lhsType, Type* rhsType) {
  bool retval = true;

  if (rhsType == dtUnknown) {
    retval = false;

  } else if (rhsType == dtNil) {
    if (lhsType != dtNil && lhsType != dtStringC && !isClass(lhsType)) {
      retval = false;
    }
  }

  return retval;
}

static void moveHaltForUnacceptableTypes(CallExpr* call) {
  Type* lhsType = call->get(1)->typeInfo();
  Type* rhsType = call->get(2)->typeInfo();

  if (rhsType == dtUnknown) {
    Expr* rhs = call->get(2);

    if (CallExpr* rhsCall = toCallExpr(rhs)) {
      if (FnSymbol* fn = rhsCall->resolvedFunction()) {
        USR_FATAL_CONT(fn,
                       "unable to resolve return type of function '%s'",
                       fn->name);
        USR_FATAL(rhs, "called recursively at this point");
      }
    }

    USR_FATAL(call, "unable to resolve type");

  } else if (rhsType == dtNil) {
    bool lhsIsPointer = isClassLikeOrPtr(lhsType) ||
                        lhsType == dtCVoidPtr ||
                        lhsType == dtCFnPtr;

    if (lhsType != dtNil && !lhsIsPointer) {
      USR_FATAL(userCall(call),
                "type mismatch in assignment from nil to %s",
                toString(lhsType));
    }
  }
}

//
//
//

// Check the 'move' into an YVV from the result of an addrOf of 'argOfAddrOf'
// when the iterator has the 'ref' return/yield intent.
// Disallow args that are not references or are const references.
// Note that the addr-of and YVV's qual are set in normalizeYields().
static void checkMoveAddrOfToRefYVV(CallExpr* move, Expr* argOfAddrOf) {
  Symbol* arg = toSymExpr(argOfAddrOf)->symbol();
  if (( arg->hasFlag(FLAG_TEMP)        ||
        arg->isImmediate()             ||
        arg->hasFlag(FLAG_PARAM)       ||
        arg->hasFlag(FLAG_CONST)       ||
        // this probably cannot happen: addrOf would be preFold-ed away
        arg->hasFlag(FLAG_REF_TO_CONST) )
// need this? && ! move->parentSymbol->hasFlag(FLAG_PROMOTION_WRAPPER)
     )
    USR_FATAL_CONT(move,
      "yielding a const or not a reference from a non-const ref iterator");
}

// Ditto for the 'const ref' return/yield intent.
static void checkMoveAddrOfToCRefYVV(CallExpr* move, Expr* argOfAddrOf) {
  Symbol* arg = toSymExpr(argOfAddrOf)->symbol();
  if (( arg->hasFlag(FLAG_TEMP)        ||
        arg->isImmediate()             ||
        arg->hasFlag(FLAG_PARAM)        )
// need this? && ! move->parentSymbol->hasFlag(FLAG_PROMOTION_WRAPPER)
     )
    USR_FATAL_CONT(move,
      "yielding a non-reference from a const ref iterator");
}

// Check the 'move' of 'arg' into an YVV
// when the iterator has the 'ref' return/yield intent.
// Disallow args that are not references or are const references.
// Note that the addr-of inserted in normalizeYields()
// may have been eliminated in preFoldPrimOp().
static void checkMoveSymToRefYVV(CallExpr* move, Symbol* arg) {
  if (( arg->hasFlag(FLAG_CONST)        ||
        arg->hasFlag(FLAG_REF_TO_CONST) ||
        ! arg->isRef()                   )
      && ! move->parentSymbol->hasFlag(FLAG_PROMOTION_WRAPPER)
     )
    USR_FATAL_CONT(move,
      "yielding a const or not a reference from a non-const ref iterator");
}

// Ditto for the 'const ref' return/yield intent.
static void checkMoveSymToCRefYVV(CallExpr* move, Symbol* arg) {
  if (( ! arg->isRef()                   )
      && ! move->parentSymbol->hasFlag(FLAG_PROMOTION_WRAPPER)
     )
    USR_FATAL_CONT(move,
      "yielding a non-reference from a const ref iterator");
}

static void resolveMoveForRhsSymExpr(CallExpr* call, SymExpr* rhs) {
  Symbol* lhsSym = toSymExpr(call->get(1))->symbol();
  Symbol* rhsSym = rhs->symbol();

  // If this assigns into a loop index variable from a non-var iterator,
  // mark the variable constant.
  // If RHS is this special variable...
  if (rhsSym->hasFlag(FLAG_INDEX_OF_INTEREST) == true) {
    Type*   rhsType = rhs->typeInfo();

    if (lhsSym->hasFlag(FLAG_INDEX_VAR) ||
        // non-zip forall over a standalone iterator
        (lhsSym->hasFlag(FLAG_TEMP) &&
         rhsSym->hasFlag(FLAG_INDEX_VAR))) {

      // ... and not of a reference type
      // ... and not an array (arrays are always yielded by reference)
      // see also resolveIdxVar() / defaultIntentYieldsConst()
      // todo: differentiate based on ref-ness, not _ref type
      // todo: not all const if it is zippered and one of iterators is var
      if (isReferenceType(rhsType)                == false &&
          isTupleContainingAnyReferences(rhsType) == false &&
          rhsType->symbol->hasFlag(FLAG_ARRAY)    == false &&
          rhsType->symbol->hasFlag(FLAG_COPY_MUTATES) == false) {
        // ... then mark LHS constant.
        lhsSym->addFlag(FLAG_CONST);
      }
    }
  } else if (rhsSym->hasFlag(FLAG_DELAY_GENERIC_EXPANSION)) {
    lhsSym->addFlag(FLAG_DELAY_GENERIC_EXPANSION);

  } else if (rhsSym->hasFlag(FLAG_REF_TO_CONST)) {
    lhsSym->addFlag(FLAG_REF_TO_CONST);

  }

  if (lhsSym->hasFlag(FLAG_TYPE_VARIABLE) &&
      lhsSym->type != dtUnknown &&
      lhsSym->getValType() != rhsSym->getValType()) {
    USR_FATAL(call, "type alias split initialization uses different types");
  }

  if (lhsSym->hasFlag(FLAG_YVV)) {
    if (lhsSym->qual == QUAL_REF)       checkMoveSymToRefYVV(call, rhsSym);
    if (lhsSym->qual == QUAL_CONST_REF) checkMoveSymToCRefYVV(call, rhsSym);
  }

  moveFinalize(call);
}

static void resolveMoveForRhsCallExpr(CallExpr* call, Type* rhsType) {
  CallExpr* rhs = toCallExpr(call->get(2));

  moveSetConstFlagsAndCheck(call, rhs);

  if (gChplHereAlloc != NULL && rhs->resolvedFunction() == gChplHereAlloc) {
    Symbol*  lhsType = call->get(1)->typeInfo()->symbol;
    Symbol*  tmp     = newTemp("cast_tmp", rhs->typeInfo());

    call->insertBefore(new DefExpr(tmp));
    call->insertBefore(new CallExpr(PRIM_MOVE, tmp,     rhs->remove()));
    call->insertAtTail(new CallExpr(PRIM_CAST, lhsType, tmp));

  } else if (rhs->isPrimitive(PRIM_CAST_TO_VOID_STAR) == true) {
    if (isReferenceType(rhs->get(1)->typeInfo())) {
      // Add a dereference as needed, as we did not have complete
      // type information earlier
      SymExpr*   castVar   = toSymExpr(rhs->get(1));
      Type*      castType  = castVar->typeInfo()->getValType();

      VarSymbol* derefTmp  = newTemp("castDeref", castType);
      CallExpr*  derefCall = new CallExpr(PRIM_DEREF, castVar->symbol());

      call->insertBefore(new DefExpr(derefTmp));
      call->insertBefore(new CallExpr(PRIM_MOVE, derefTmp, derefCall));

      rhs->replace(new CallExpr(PRIM_CAST_TO_VOID_STAR, derefTmp));
    }

    moveFinalize(call);

  // Fix up PRIM_COERCE : remove it if it has a param RHS.
  } else if (rhs->isPrimitive(PRIM_COERCE) == true) {
    moveFinalize(call);

    if (SymExpr* fromSe = toSymExpr(rhs->get(1))) {
      Symbol* fromSym = fromSe->symbol();

      // This transformation is normally handled in insertCasts
      // but we need to do it earlier for parameters. We can't just
      // call insertCasts here since that would dramatically change the
      // resolution order (and would be apparently harder to get working).
      if (fromSym->isParameter()               == true  ||
          fromSym->hasFlag(FLAG_TYPE_VARIABLE) == true) {
        // Can we coerce from the argument to the function return type?
        // Note that rhsType here is the function return type
        // (since that is what the primitive returns as its type).
        Type* fromType = fromSym->type;
        Type* rhsType    = rhs->typeInfo();
        bool tmpParamNarrows = false;

        if (fromType                                     == rhsType ||
            canParamCoerce(fromType, fromSym, rhsType, &tmpParamNarrows) == true)   {
          SymExpr* lhs = toSymExpr(call->get(1));

          call->get(1)->replace(lhs->copy());
          call->get(2)->replace(new SymExpr(fromSym));

        } else {
          // Any other case (including error cases)
          // will be handled in insertCasts
        }
      }
    }

  } else {
    if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
      Type* baseType = rhs->get(1)->getValType();
      const char* memberName = get_string(rhs->get(2));
      Symbol* sym = baseType->getField(memberName);
      if (sym->hasFlag(FLAG_DELAY_GENERIC_EXPANSION)) {
        if (SymExpr* se = toSymExpr(call->get(1))) {
          se->symbol()->addFlag(FLAG_DELAY_GENERIC_EXPANSION);
        }
      }
    } else if (rhs->isPrimitive(PRIM_ADDR_OF)) {
      // Check that the types match
      SymExpr* lhsSe = toSymExpr(call->get(1));
      Symbol* lhs = lhsSe->symbol();
      INT_ASSERT(lhs->isRef());

      if (lhs->getValType() != rhsType->getValType()) {
        USR_FATAL_CONT(call, "Initializing a reference with another type");
        USR_PRINT(lhs, "Reference has type %s", toString(lhs->getValType()));
        USR_PRINT(call, "Initializing with type %s",
                        toString(rhsType->getValType()));
        USR_STOP();
      }

      if (lhs->hasFlag(FLAG_YVV)) {
        Expr* arg = rhs->get(1);
        if (lhs->qual == QUAL_REF)       checkMoveAddrOfToRefYVV(call, arg);
        if (lhs->qual == QUAL_CONST_REF) checkMoveAddrOfToCRefYVV(call, arg);
      }
    }

    moveFinalize(call);
  }
}


static void moveSetConstFlagsAndCheck(CallExpr* call, CallExpr* rhs) {
  if (rhs->isPrimitive(PRIM_GET_MEMBER) ||
      rhs->isPrimitive(PRIM_ADDR_OF))
  {
    if (SymExpr* rhsBase = toSymExpr(rhs->get(1))) {
      if (rhsBase->symbol()->hasFlag(FLAG_CONST)        == true  ||
          rhsBase->symbol()->hasFlag(FLAG_REF_TO_CONST) == true) {
        toSymExpr(call->get(1))->symbol()->addFlag(FLAG_REF_TO_CONST);
      }

    } else {
      INT_ASSERT(false);
    }

  } else if (FnSymbol* resolvedFn = rhs->resolvedFunction()) {
    Symbol* lhsSym = toSymExpr(call->get(1))->symbol();

    moveSetFlagsAndCheckForConstAccess(lhsSym, rhs, resolvedFn);
  }
}

// If 'call' is an access to a const thing, for example a const field
// or a field of a const record, set const flag(s) on the symbol
// that stores the result of 'call'.
static void moveSetFlagsAndCheckForConstAccess(Symbol*   lhsSym,
                                               CallExpr* rhs,
                                               FnSymbol* resolvedFn) {
  bool refConst    = resolvedFn->hasFlag(FLAG_REF_TO_CONST);
  bool refConstWCT = resolvedFn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS);

  INT_ASSERT(refConst == false || refConstWCT == false);

  if (refConst == true) {
    Symbol* baseSym = NULL;

    if (resolvedFn->hasFlag(FLAG_FIELD_ACCESSOR)    == true &&
        resolvedFn->hasFlag(FLAG_PROMOTION_WRAPPER) == false) {
      baseSym = getBaseSymForConstCheck(rhs);
    }

    moveSetFlagsForConstAccess(lhsSym, rhs, baseSym, false);

  } else if (refConstWCT == true && !resolvedFn->isIterator()) {
    Symbol* baseSym = getBaseSymForConstCheck(rhs);

    if (baseSym->isConstant()               == true ||
        baseSym->hasFlag(FLAG_CONST)        == true ||
        baseSym->hasFlag(FLAG_REF_TO_CONST) == true) {
      moveSetFlagsForConstAccess(lhsSym, rhs, baseSym, true);
    }
  }
}

static void moveSetFlagsForConstAccess(Symbol*   lhsSym,
                                       CallExpr* rhs,
                                       Symbol*   baseSym,
                                       bool      refConstWCT) {
  bool isArgThis = baseSym != NULL && baseSym->hasFlag(FLAG_ARG_THIS) == true;

  // Do not consider it const if it is an access to 'this' in a constructor.
  if (isArgThis == false || isInConstructorLikeFunction(rhs) == false) {

    if (isReferenceType(lhsSym->type) == true) {
      lhsSym->addFlag(FLAG_REF_TO_CONST);
    } else {
      lhsSym->addFlag(FLAG_CONST);
    }

    if (isArgThis == true ||
        (refConstWCT                                        == true &&
         baseSym->hasFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS) == true)) {
      lhsSym->addFlag(FLAG_REF_FOR_CONST_FIELD_OF_THIS);
    }
  }
}

//
//
//

static void moveFinalize(CallExpr* call) {
  Type* lhsType    = call->get(1)->typeInfo();
  Type* rhsType    = call->get(2)->typeInfo();

  Type* lhsValType = lhsType->getValType();
  Type* rhsValType = rhsType->getValType();

  if (isDispatchParent(rhsValType, lhsValType) == true) {
    if (rhsType != lhsType) {
      Expr*   rhs = call->get(2);
      Symbol* tmp = newTemp("cast_tmp", rhsType);

      call->insertBefore(new DefExpr(tmp));
      call->insertBefore(new CallExpr(PRIM_MOVE, tmp, rhs->remove()));

      call->insertAtTail(new CallExpr(PRIM_CAST, lhsValType->symbol, tmp));
    }

  } else {
    if (rhsValType != lhsValType) {
      if (isMoveFromMain(call)) {
        USR_FATAL(chplUserMain, "main() returns a non-integer (%s)",
                  rhsValType->name());
      }
    }
  }
}

//
//
//

bool isDispatchParent(Type* t, Type* pt) {
  bool retval = false;

  // Use the canonical class type
  // If one is managed and the other is not, it's not
  // a dispatch parent.
  if (classesWithSameKind(t, pt)) {
    t = canonicalDecoratedClassType(t);
    pt = canonicalDecoratedClassType(pt);
  }

  if (AggregateType* at = toAggregateType(t)) {
    forv_Vec(AggregateType, p, at->dispatchParents) {
      if (p == pt || isDispatchParent(p, pt) == true) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Resolve a PRIM_NEW expression.                                              *
* The 1st argument is a type or a partial call to get the type and the        *
* remaining arguments are assumed to be arguments for the constructor or      *
* initializer call                                                            *
*                                                                             *
* The syntax supports calling the result of a type function as a constructor  *
* but this is not fully implemented.                                          *
*                                                                             *
************************************** | *************************************/

static void           resolveNewWithInitializer(CallExpr* newExpr,
                                                Type* manager);

static SymExpr*       resolveNewFindTypeExpr(CallExpr* newExpr);

static void resolveNewSetupManaged(CallExpr* newExpr, Type*& manager);

static bool isUndecoratedClassNew(CallExpr* newExpr, Type* newType);

static void resolveNew(CallExpr* newExpr) {

  if (newExpr->id == breakOnResolveID) {
    gdbShouldBreakHere();
  }

  // If it's a managed new, detect the _chpl_manager named arg and remove it
  // The following variables allow the latter half of this function
  // to construct the AST for initializing the owned/shared if necessary.
  // Manager is:
  //  dtBorrowed for 'new borrowed'
  //  dtUnmanaged for 'new unmanaged'
  //  owned record for 'new owned'
  //  shared record for 'new shared'
  //  dtAnyManagementNilable to request nilable variant
  Type* manager = NULL;

  resolveNewSetupManaged(newExpr, manager);

  if (SymExpr* typeExpr = resolveNewFindTypeExpr(newExpr)) {
    if (Type* type = resolveTypeAlias(typeExpr)) {
      if (isAggregateType(type)) {

        resolveNewWithInitializer(newExpr, manager);

      } else if (PrimitiveType* pt = toPrimitiveType(type)) {
        const char* name = pt->symbol->name;

        USR_FATAL(newExpr, "invalid use of 'new' on primitive %s", name);

      } else if (EnumType* et = toEnumType(type)) {
        const char* name = et->symbol->name;

        USR_FATAL(newExpr, "invalid use of 'new' on enum %s", name);

      } else {
        USR_FATAL(newExpr, "new must be applied to a record or class");
      }
    }

  } else {
    const char* name = NULL;

    if (Expr* arg = newExpr->get(1)) {
      if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(arg)) {
        name = urse->unresolved;

      } else if (CallExpr* subCall = toCallExpr(arg)) {
        if (FnSymbol* fn = subCall->resolvedFunction()) {
          name = fn->name;
        }
      }
    }

    if (name == NULL) {
      USR_FATAL(newExpr, "invalid use of 'new'");

    } else {
      USR_FATAL(newExpr, "invalid use of 'new' on %s", name);
    }
  }
}

static void checkManagerType(Type* t) {
  // Verify that the manager matches expectations
  //  - borrowed or borrowedNilable
  //  - unmanaged or unmanagedNilable
  //  - owned or DecoratedClassType(owned,?)  (or other management type)
  if (t == dtBorrowed || t == dtBorrowedNilable ||
      t == dtUnmanaged || t == dtUnmanagedNilable ||
      t == dtAnyManagementNilable) {
    return; // OK
  }

  if (isManagedPtrType(t)) {
    ClassTypeDecoratorEnum d = classTypeDecorator(t);
    INT_ASSERT(d == ClassTypeDecorator::MANAGED || d ==
        ClassTypeDecorator::MANAGED_NILABLE);
    // check that it's not an instantiation
    INT_ASSERT(getDecoratedClass(t, d) == t);
  }
}

static void resolveNewSetupManaged(CallExpr* newExpr, Type*& manager) {

  for_actuals(expr, newExpr) {
    if (NamedExpr* ne = toNamedExpr(expr)) {
      if (ne->name == astr_chpl_manager) {
        Type* t = ne->actual->typeInfo();
        checkManagerType(t);
        manager = t;
        expr->remove();
        break;
      }
    }
  }

  // adjust the type to initialize for managed new cases
  if (SymExpr* typeExpr = resolveNewFindTypeExpr(newExpr)) {
    if (Type* type = resolveTypeAlias(typeExpr)) {

      bool makeNilable = (manager == dtAnyManagementNilable ||
                          isNilableClassType(type));

      // set manager for new t(1,2,3)
      // where t is e.g Owned(MyClass)
      // or for t is unmanaged(MyClass)
      if (manager == NULL || manager == dtAnyManagementNilable) {
        if (isManagedPtrType(type)) {
          manager = getManagedPtrManagerType(type);
        } else if (DecoratedClassType* dt = toDecoratedClassType(type)) {
          if (dt->isUnmanaged()) {
            manager = dtUnmanaged;
          } else if (dt->isBorrowed()) {
            manager = dtBorrowed;
          } else {
            manager = dtOwned;
          }
        } else if (isClass(type)) {
          manager = dtBorrowed;
        } else if (isClass(type) && isUndecoratedClassNew(newExpr, type)) {
          manager = dtOwned;
        }
      } else {
        // Manager is specified, so check for duplicate management decorators
        checkDuplicateDecorators(manager, type, newExpr);
      }

      // if manager is set, and we're not calling the manager's init function,
      // use the canonical class type instead of the managed type, since
      // the rest of the compiler (e.g. initializer resolution) uses
      // only the canonical class types.
      if (manager) {
        // if needed, make the manager nilable
        if (makeNilable) {
          if (isManagedPtrType(manager))
            manager = getDecoratedClass(manager,
                ClassTypeDecorator::MANAGED_NILABLE);
          else if (manager == dtUnmanaged)
            manager = dtUnmanagedNilable;
          else if (manager == dtBorrowed)
            manager = dtBorrowedNilable;
          else
            INT_FATAL("case not handled");
        }

        checkManagerType(manager);

        // fail if it's a record
        if (isRecord(type) && !isManagedPtrType(type))
          USR_FATAL_CONT(newExpr, "Cannot use new %s with record %s",
                                  toString(manager), toString(type));
        else if (!isClassLikeOrManaged(type))
          USR_FATAL_CONT(newExpr, "cannot use management %s on non-class %s",
                                   toString(manager), toString(type));


        Type* initType = type;

        // Use the class type inside a owned/shared/etc
        // unless we are initializing Owned/Shared itself
        if (isManagedPtrType(type)) {
          Type* subtype = getManagedPtrBorrowType(type);
          // rule out dtUnknown
          if (isAggregateType(subtype) || isDecoratedClassType(subtype))
            initType = subtype;
        }

        // Use the canonical class to simplify the rest of initializer
        // resolution
        if (DecoratedClassType* mt = toDecoratedClassType(initType)) {
          initType = mt->getCanonicalClass();
        }

        if (manager) {
          if (initType != type)
            // Set the type to initialize
            typeExpr->setSymbol(initType->symbol);
        }
      }

      if (manager == dtBorrowed && fWarnUnstable) {
        Type* ct = canonicalClassType(type);
        USR_WARN(newExpr, "new borrowed %s is unstable", ct->symbol->name);
        USR_PRINT(newExpr, "use 'new unmanaged %s' "
                           "'new owned %s' or "
                           "'new shared %s'",
                           ct->symbol->name,
                           ct->symbol->name,
                           ct->symbol->name);
      }
    }
  }
}

static bool isUndecoratedClassNew(CallExpr* newExpr, Type* newType) {
  bool isUndecorated = false;

  INT_ASSERT(newExpr->parentSymbol);
  if (isClass(newType) &&
      !isReferenceType(newType) &&
      !newType->symbol->hasFlag(FLAG_DATA_CLASS) &&
      !newType->symbol->hasFlag(FLAG_NO_OBJECT) &&
      !newType->symbol->hasFlag(FLAG_NO_DEFAULT_FUNCTIONS)) {

    // Assume it is undecorated and below we will determine if otherwise
    isUndecorated = true;

    SymExpr* checkSe = NULL;
    if (CallExpr* parentCall = toCallExpr(newExpr->parentExpr))
      if (parentCall->isPrimitive(PRIM_MOVE) ||
          parentCall->isPrimitive(PRIM_ASSIGN))
        if (SymExpr* lhsSe = toSymExpr(parentCall->get(1)))
          checkSe = lhsSe;

    if (checkSe) {
      for_SymbolSymExprs(se, checkSe->symbol()) {
        // Check that 'new' was used either in
        // PRIM_TO_UNMANAGED_CLASS or in an initialization of Owned/Shared/etc.
        if (se == checkSe) {
          // do nothing
        } else if (CallExpr* parentCall = toCallExpr(se->parentExpr)) {
          if (parentCall->isNamed("chpl__tounmanaged") || // TODO -- remove case
              parentCall->isNamed("chpl__delete") || // TODO -- remove case
              parentCall->isNamed("chpl__buildDistValue")) {
            // treat these as decorated
            isUndecorated = false;
          } else if (parentCall->isPrimitive(PRIM_NEW) &&
                     parentCall->get(1)->typeInfo()->symbol->hasFlag(FLAG_MANAGED_POINTER)) {
            // OK e.g. new Owned(new MyClass())
            isUndecorated = false;
          } else if (parentCall->isPrimitive(PRIM_TO_UNMANAGED_CLASS) ||
                     parentCall->isPrimitive(PRIM_TO_UNMANAGED_CLASS_CHECKED)) {
            // OK e.g. new raw MyClass() / new owned MyClass()
            isUndecorated = false;
          } else {
            // do nothing (this use is undecorated)
          }
        }
      }
    }
  }

  return isUndecorated;
}

static void resolveNewWithInitializer(CallExpr* newExpr, Type* manager) {
  SET_LINENO(newExpr);

  //
  // Normalize the allocation for a nested type (constructors only)
  //
  //    primNew(Inner(_mt, this), ...) => primNew(Inner, this, ...)
  //
  if (CallExpr* partial = toCallExpr(newExpr->get(1))) {
    SymExpr* typeExpr = toSymExpr(partial->baseExpr);

    partial->remove();

    newExpr->insertAtHead(typeExpr);
  }

  resolveNewInitializer(newExpr, manager);
}


// Find the SymExpr for the type.
//   1) Common case  :- primNew(Type, arg1, ...);
//   2) Module scope :- primNew(module=, moduleName, Type, arg1, ...);
//   3) Nested call  :- primNew(Inner(_mt, this), arg1, ...);
static SymExpr* resolveNewFindTypeExpr(CallExpr* newExpr) {
  SymExpr* retval = NULL;

  if (SymExpr* se = toSymExpr(newExpr->get(1))) {
    if (se->symbol() != gModuleToken) {
      retval = se;

    } else {
      retval = toSymExpr(newExpr->get(3));
    }

  } else if (CallExpr* partial = toCallExpr(newExpr->get(1))) {
    if (SymExpr* se = toSymExpr(partial->baseExpr)) {
      retval = partial->partialTag ? se : NULL;
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveForResolutionPoint(CallExpr* call) {
  INT_ASSERT(call->numActuals() == 1);
  resolveConstrainedGenericSymbol(toSymExpr(call->get(1))->symbol(), true);
  call->convertToNoop();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveCoerce(CallExpr* call) {
  resolveGenericActuals(call);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static Type* resolveGenericActual(SymExpr* se, bool resolvePartials = false);
static Type* resolveGenericActual(SymExpr* se, Type* type);

Type* resolveDefaultGenericTypeSymExpr(SymExpr* se) {
  return resolveGenericActual(se, /* resolvePartials */ true);
}

void resolveGenericActuals(CallExpr* call) {
  SET_LINENO(call);

  for_actuals(actual, call) {
    Expr* safeActual = actual;

    if (NamedExpr* ne = toNamedExpr(safeActual)) {
      safeActual = ne->actual;
    }

    if (SymExpr*   se = toSymExpr(safeActual))   {
      resolveGenericActual(se);
    }
  }
}

static Type* resolveGenericActual(SymExpr* se, bool resolvePartials) {
  Type* retval = se->typeInfo();

  if (TypeSymbol* ts = toTypeSymbol(se->symbol())) {
    retval = resolveGenericActual(se, ts->type);

  } else if (VarSymbol* vs = toVarSymbol(se->symbol())) {
    if (vs->hasFlag(FLAG_TYPE_VARIABLE) == true) {

      // Fix for complicated extern vars like
      //   extern var x: c_ptr(c_int);
      if ((vs->hasFlag(FLAG_EXTERN) == true || isGlobal(vs)) &&
          vs->defPoint             != NULL &&
          vs->getValType()         == dtUnknown ) {
        vs->type = resolveTypeAlias(se);
      }

      if (resolvePartials) {
        Type* origType = vs->typeInfo();
        retval = resolveGenericActual(se, origType);
      }
    }
  }

  return retval;
}

static Type* resolveGenericActual(SymExpr* se, Type* type) {
  Type* retval = se->typeInfo();

  ClassTypeDecoratorEnum decorator = ClassTypeDecorator::BORROWED_NONNIL;
  bool isDecoratedGeneric = false;
  if (DecoratedClassType* dt = toDecoratedClassType(type)) {
    type = dt->getCanonicalClass();
    decorator = dt->getDecorator();
    if (isDecoratorUnknownManagement(decorator))
      isDecoratedGeneric = true;
  }

  if (AggregateType* at = toAggregateType(type)) {
    if (at->symbol->hasFlag(FLAG_GENERIC) && at->isGenericWithDefaults()) {
      CallExpr*   cc    = new CallExpr(at->symbol);

      se->replace(cc);

      resolveCall(cc);

      Type* retType = cc->typeInfo();

      if (decorator != ClassTypeDecorator::BORROWED &&
          decorator != ClassTypeDecorator::BORROWED_NONNIL) {
        AggregateType* gotAt = toAggregateType(retType);
        INT_ASSERT(gotAt);
        retType = gotAt->getDecoratedClass(decorator);
      }

      cc->replace(new SymExpr(retType->symbol));

      retval = retType;
    } else if (isDecoratedGeneric) {
      Type* retType = at->getDecoratedClass(decorator);
      se->replace(new SymExpr(retType->symbol));
      retval = retType;
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool isInstantiation(Type* sub, Type* super) {
  bool retval = false;

  INT_ASSERT(super != NULL);

  if (AggregateType* at = toAggregateType(sub)) {
    AggregateType* cur = at->instantiatedFrom;

    while (cur != NULL && cur != super) {
      cur = cur->instantiatedFrom;
    }

    retval = cur == super;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static Expr* resolveTypeOrParamExpr(Expr* expr);

void ensureEnumTypeResolved(EnumType* etype) {
  if (etype->integerType == NULL) {
    // Make sure to resolve all enum types.

    // Set it to int so we can handle enum constants
    // referring to previous enum constants.
    // This might be temporarily incorrect, but it's good enough
    // for resolving the enum constant initializers.
    etype->integerType = dtInt[INT_SIZE_DEFAULT];

    int64_t v=0;
    uint64_t uv=0;
    bool foundInit = false;
    bool foundNegs = false;

    for_enums(def, etype) {
      if (def->init != NULL) {
        foundInit = true;
        Expr* enumTypeExpr = resolveTypeOrParamExpr(def->init);

        Type* t = enumTypeExpr->typeInfo();

        if (t == dtUnknown) {
          USR_FATAL(def->init, "unable to resolve the initializer"
                    " for the enumerator constant '%s'"
                    " possibly because of a use before definition",
                    def->sym->name);
        }
        if (!is_int_type(t) && !is_uint_type(t)) {
          USR_FATAL(def,
                    "enumerator constant '%s' has a non-integer initializer",
                    def->sym->name);
        }

        // Replace def->init if it's not the same as enumTypeExpr
        if (enumTypeExpr != def->init) {
          enumTypeExpr->remove();
          def->init->replace(enumTypeExpr);
        }

        // At this point, def->init should be an integer symbol.
        if( get_int( def->init, &v ) ) {
          if( v >= 0 ) uv = v;
          else uv = 1;
          if (v < 0) {
            foundNegs = true;
          }
        } else if( get_uint( def->init, &uv ) ) {
          v = uv;
        }
      } else {
        if (foundInit) {
          // Use the u/v value we had from adding 1 to the previous one
          if( v >= INT32_MIN && v <= INT32_MAX )
            def->init = new SymExpr(new_IntSymbol(v, INT_SIZE_32));
          else if (uv <= UINT32_MAX)
            def->init = new SymExpr(new_IntSymbol(v, INT_SIZE_64));
          else
            def->init = new SymExpr(new_UIntSymbol(uv, INT_SIZE_64));

          parent_insert_help(def, def->init);
        }
      }
      if (uv > INT64_MAX) {
        // Switch to uint(64) as the current enum type.
        etype->integerType = dtUInt[INT_SIZE_DEFAULT];
        if (foundNegs) {
          USR_FATAL(etype,
                    "this enum cannot be represented with a single integer type");
        }
      }
      if (foundInit) {
        v++;
        uv++;
      }
    }
  }

  INT_ASSERT(etype->integerType != NULL);
}

Type* resolveTypeAlias(SymExpr* se) {
  Type* retval = NULL;

  if (se != NULL) {
    Type* valType = se->getValType();

    if (valType != dtUnknown) {
      retval = valType;

    } else if (VarSymbol* var = toVarSymbol(se->symbol())) {
      SET_LINENO(var->defPoint);

      DefExpr* def = var->defPoint;
      SymExpr* tse = NULL;

      if (def->init != NULL) {
        tse = toSymExpr(resolveTypeOrParamExpr(def->init));
      } else if (SymExpr* singleDef = var->getSingleDef()) {
        // Figure out the RHS of the singleDef, assuming it is in a PRIM_MOVE.
        if (CallExpr* call = toCallExpr(singleDef->parentExpr))
          if (call->isPrimitive(PRIM_MOVE))
            if (call->get(1) == singleDef)
              tse = toSymExpr(resolveTypeOrParamExpr(call->get(2)));
      }

      if (tse == NULL) {
        INT_FATAL("unsupported case in resolveTypeAlias");
      }

      retval = resolveTypeAlias(tse);
    }
  }

  return retval;
}

static Expr* resolveTypeOrParamExpr(Expr* expr) {
  Expr* retval = NULL;

  for_exprs_postorder(e, expr) {
    if (CallExpr* call = toCallExpr(e)) {
      Expr* result = preFold(call);

      if (CallExpr* callFolded = toCallExpr(result)) {
        if (callFolded->inTree()) {

          callStack.add(callFolded);

          resolveCall(callFolded);

          if (FnSymbol* fn = callFolded->resolvedFunction()) {

              if (fn->retTag  == RET_PARAM || fn->retTag  == RET_TYPE) {
                resolveSignatureAndFunction(fn);

              } else if (fn->retType == dtUnknown) {
                resolveSignatureAndFunction(fn);

              } else {
                resolveSignature(fn);

              }
          }

          callStack.pop();
        }
      }
      retval = foldTryCond(postFold(result));

      if (expr != e) {
        // Avoids issue where modification of expr's contents caused early
        // termination of the loop.
        e = retval;
      }

    } else {
      retval = foldTryCond(postFold(e));
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// Non-normalizable expressions may nest at an arbitrary depth (because
// they are not normalized), so we have to search upwards in the tree
// to look for them.
static Expr* handleNonNormalizableExpr(Expr* expr) {
  Expr* ret = nullptr;

  // TODO: Ways to limit our search depth?
  if (Expr* nonNormalRoot = partOfNonNormalizableExpr(expr)) {
    if (CallExpr* call = toCallExpr(nonNormalRoot)) {

      // Prefolding will completely replace PRIM_RESOLVES calls, so
      // further action is not needed here.
      if (call->isPrimitive(PRIM_RESOLVES)) {
        ret = preFold(call);
      } else {
        INT_FATAL("Not handled!");
      }
    }
  }

  return ret;
}

static bool terminatesControlFlow(Expr* expr);

// Is it guaranteed that 'block' terminates control flow?
static bool terminatesControlFlowBlock(BlockStmt* block) {
  Expr* last = block->body.tail;
  if (last == nullptr)
    return false;

  if (CallExpr* call = toCallExpr(last)) {
    if (call->isPrimitive(PRIM_END_OF_STATEMENT)) {
      if (Expr* prev = last->prev)
        return terminatesControlFlow(prev);
      else
        return false;
    }
  }

  return terminatesControlFlow(last);
}

static bool terminatesControlFlow(Expr* expr) {
  if (! isBlockStmt(expr->parentExpr))
    return false; // allow only stmt-level terminations

  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIM_THROW))
      return true;
    // We do not check PRIM_RETURN because there is only one in the epilogue
    // and we should not to mess with the epilogue.

    if (FnSymbol* target = call->resolvedFunction())
        if(target->hasFlag(FLAG_FUNCTION_TERMINATES_PROGRAM))
          return true;

  } else if (isGotoStmt(expr)) {
    return true;

  } else if (BlockStmt* block = toBlockStmt(expr)) {
    if (block->isRealBlockStmt())
      return terminatesControlFlowBlock(block);

  } else if (CondStmt* cond = toCondStmt(expr)) {
    // if-stmt terminates CF if both branches do
    if (cond->elseStmt != nullptr)
      return terminatesControlFlowBlock(cond->thenStmt) &&
             terminatesControlFlowBlock(cond->elseStmt);
  }

  return false;
}

// If 'expr' terminates CF, make an exception if 'expr' halts or throws.
// Reason: existing code uses unreachable code to infer the return type, ex.
//   proc bulkAdd_help(...) { halt(...); return -1; } --> returns int
// Ex. test/sparse/CS/multiplication/correctness.chpl
static bool needLaterReturn(Expr* expr) {
  return isCallExpr(expr);
}

// If 'expr' aborts control flow, ex. goto (incl. return), throw, or call
// to a "terminate program" function, remove subsequent unreachable stmts.
static void handleEarlyFinish(Expr* expr) {
  if (! terminatesControlFlow(expr)) return; // nothing to do

  if (needLaterReturn(expr))
    if (isFnSymbol(expr->parentSymbol)) // returns are only in functions
      for (Expr* cur = expr->next; cur; cur = cur->next)
        if (CallExpr* call = toCallExpr(cur))
          if (call->isPrimitive(PRIM_MOVE))
            if (SymExpr* destSE = toSymExpr(call->get(1)))
              if (destSE->symbol()->hasFlag(FLAG_RVV))
                return; // keep the unreachable code

  if (CallExpr* nextCall = toCallExpr(expr->next))
    if (nextCall->isPrimitive(PRIM_END_OF_STATEMENT))
      expr = expr->next; // "end of statement" might be important

  // clean up the unreachable statements to avoid resolving them
  while (expr->next != nullptr) {
    if (DefExpr* def = toDefExpr(expr->next))
      if (isLabelSymbol(def->sym))
        break; // this can be reachable via a goto
    if (CallExpr* call = toCallExpr(expr->next))
      if (call->isPrimitive(PRIM_RETURN))
        break; // need this to keep the function well-formed

    if (isDefExpr(expr->next) &&
        !isVarSymbol(toDefExpr(expr->next)->sym))
      // do not remove defs of useful things
      expr = expr->next;
    else
      expr->next->remove();
  }
}

static bool shouldHandleEarlyFinish(BlockStmt* block) {
  if (FnSymbol* enclosingFn = toFnSymbol(block->parentSymbol))
    if (isTaskFun(enclosingFn))
      return false; // ensure the special handling of endCounts stays
  return true;
}

void resolveBlockStmt(BlockStmt* blockStmt) {
  bool ef = shouldHandleEarlyFinish(blockStmt);

  for_exprs_postorder(expr, blockStmt) {
    if (Expr* changed = handleNonNormalizableExpr(expr)) {
      expr = changed;
    } else {
      expr = resolveExpr(expr);
    }

    INT_ASSERT(expr);
    if (ef) handleEarlyFinish(expr);
  }
}

static Expr* resolveFunctionTypeConstructor(DefExpr* def) {
  auto fn = toFnSymbol(def->sym);

  INT_ASSERT(fn && fn->isAnonymous() && fn->isSignature());
  INT_ASSERT(fn->type == dtUnknown);

  if (def->id == breakOnResolveID || fn->id == breakOnResolveID) {
    gdbShouldBreakHere();
  }

  bool isBodyResolved = fcfs::checkAndResolveSignature(fn, def);

  // Signature only, so no body to resolve.
  INT_ASSERT(!isBodyResolved && !fn->isResolved());

  auto ft = FunctionType::get(fn);
  INT_ASSERT(ft);

  Type* t = ft;

  // If we aren't using pointers, have to convert to a function class type
  // for type-based equality checks to work correctly.
  if (!fcfs::usePointerImplementation()) {
    t = fcfs::functionClassSuperTypeFromFunctionType(ft);
    INT_ASSERT(t);
  }

  auto ret = new SymExpr(t->symbol);
  def->replace(ret);
  return ret;
}

// Intentionally does nothing for now, but is where we'd handle closures.
static Expr* convertFunctionToClosureIfNeeded(FnSymbol* fn, Expr* use) {
  return nullptr;
}

// TODO: We want to support closures, obviously...
static bool errorIfFunctionCapturesAnyOuterVars(FnSymbol* fn, Expr* use) {
  INT_ASSERT(fn && fn->type && isFunctionType(fn->type));

  FunctionType* ft = toFunctionType(fn->type);
  auto& env = fcfs::computeOuterVariables(fn);

  // Check to make sure the function does not refer to any outer variables.
  if (!env.isEmpty()) {
    auto kindStr = FunctionType::kindToString(ft->kind());
    if (fn->hasFlag(FLAG_LEGACY_LAMBDA)) kindStr = "lambda";

    if (fn->hasFlag(FLAG_ANONYMOUS_FN)) {
      USR_FATAL_CONT(use, "cannot capture %s because it refers to "
                          "outer variables",
                          kindStr);
    } else {
      INT_ASSERT(!fn->hasFlag(FLAG_LEGACY_LAMBDA));
      USR_FATAL_CONT(use, "cannot capture %s '%s' because it refers "
                          "to outer variables",
                          kindStr,
                          fn->name);
    }

    const int hi = 3;
    const int num = env.numOuterVariables();
    const int stop = num < hi ? num : hi;

    for (int i = 0; i < stop; i++) {
      auto sym = env.outerVariable(i);
      auto mention = env.firstMention(sym);
      USR_PRINT(mention, "such as '%s', here", sym->name);
    }

    return true;
  }

  return false;
}

static FnSymbol*
resolveNameToSingleFunctionOrError(const char* name, Expr* use) {
  FnSymbol* ret = nullptr;

  auto v = fcfs::lookupFunctions(name, use);

  if (v.empty()) {
    USR_FATAL_CONT(use, "no routine with name '%s' is visible", name);

  } else if (v.size() > 1) {
    USR_FATAL_CONT(use, "cannot capture routine with name '%s' because "
                        "it is overloaded",
                        name);
    const int hi = 3;
    const int size = ((int) v.size());
    const int stop = hi < size ? hi : size;
    const int remaining = size - hi;

    // TODO: Display candidates like function resolution does.
    for (int i = 0; i < stop; i++) USR_PRINT(v[i], "declared here");

    if (remaining > 0) {
      USR_PRINT(use, "and '%d' other candidates", remaining);
    }

    // Have to stop or else confusing output follows.
    USR_STOP();

  } else {
    ret = v[0];
    INT_ASSERT(ret);
  }

  return ret;
}

static Expr* swapInErrorSinkForCapture(FunctionType::Kind kind, Expr* use) {
  auto sink = fcfs::errorSink(kind);
  auto ret = new SymExpr(sink);
  use->replace(ret);
  return ret;
}

static Expr* swapInFunctionForCapture(FnSymbol* fn, Expr* use) {
  auto ret = new SymExpr(fn);
  use->replace(ret);
  return ret;
}

static Expr*
finalLoweringForFunctionCapture(FnSymbol* fn, Expr* use, bool useClass) {
  if (useClass || !fcfs::usePointerImplementation()) {
    return fcfs::createFunctionClassInstance(fn, use);
  } else {
    return swapInFunctionForCapture(fn, use);
  }
}

// Create either a function pointer, a closure, or a 'c_fn_ptr' depending
// on the function to be captured (or an error, if it does not exist or
// could not be disambiguated to a single symbol).
static Expr* resolveFunctionCapture(FnSymbol* fn, Expr* use,
                                    bool discardType,
                                    bool useClass) {
  if (fn->id == breakOnResolveID || use->id == breakOnResolveID) {
    gdbShouldBreakHere();
  }

  // Discarding the type (e.g., for 'c_ptrTo') is well specified.
  if (fWarnUnstable && !discardType) {
    USR_WARN(use, "use of routines as values is unstable");
  }

  fcfs::checkAndResolveSignatureAndBody(fn, use);

  auto ft = fn->computeAndSetType();
  INT_ASSERT(ft);

  if (ft->isGeneric() || ft->returnType() == dtUnknown) {
    auto kindStr = FunctionType::kindToString(ft->kind());
    if (fn->hasFlag(FLAG_LEGACY_LAMBDA)) kindStr = "lambda";

    // TODO: Maybe use 'iterator'/'procedure' instead of 'proc'/'iter'?
    if (fn->hasFlag(FLAG_ANONYMOUS_FN)) {
      USR_FATAL_CONT(use, "anonymous %s is generic and cannot be captured",
                     kindStr);
    } else {
      USR_FATAL_CONT(use, "the %s '%s' is generic and cannot be captured",
                     kindStr,
                     fn->name);
    }

    // Return a "sink" to handle any "illegal access" errors later.
    return swapInErrorSinkForCapture(ft->kind(), use);
  }

  // Surprisingly this seems to work somewhat well if removed, see:
  // 'functions/vass/passing-iterator-as-argument'
  if (ft->kind() == FunctionType::ITER) {
    USR_FATAL_CONT(use, "passing iterators by name is not yet supported");
    return swapInErrorSinkForCapture(ft->kind(), use);
  }

  // There was some other problem resolving the body.
  if (!fn->isResolved() && !fn->hasFlag(FLAG_EXTERN)) {
    return swapInErrorSinkForCapture(ft->kind(), use);
  }

  // For now, error if any function refers to outer variables. At this
  // point we can just swap in the actual function symbol, since its
  // signature is not the problem.
  if (errorIfFunctionCapturesAnyOuterVars(fn, use)) {
    return finalLoweringForFunctionCapture(fn, use, useClass);
  }

  // This path discards the function type by casting to 'dtCFnPtr'. It has
  // a redundant check for outer variables, because even in a world with
  // closure support, we cannot allow a pointer to a closure.
  if (discardType) {
    auto& env = fcfs::computeOuterVariables(fn);
    if (!env.isEmpty()) {
      USR_FATAL_CONT(use, "cannot convert type '%s' to '%s' because it "
                          "refers to one or more outer variables",
                          fn->name,
                          dtCFnPtr->symbol->name);
      // TODO: Sink or function symbol here?
      return swapInErrorSinkForCapture(ft->kind(), use);

    } else {
      auto seFn = new SymExpr(fn);
      auto seType = new SymExpr(dtCFnPtr->symbol);
      auto ret = new CallExpr(PRIM_CAST_TO_TYPE, seFn, seType);
      use->replace(ret);
      return ret;
    }
  }

  // Normally, create a closure only if needed. Function is NOP for now,
  // since we don't actually support closures yet, and the above check
  // rules out the possibility.
  if (auto ret = convertFunctionToClosureIfNeeded(fn, use)) {
    return ret;
  }

  // Otherwise, perform some last minute lowering based on mode.
  auto ret = finalLoweringForFunctionCapture(fn, use, useClass);
  return ret;
}

//
// Handle the family of function capture primitives, as well as the
// primitive implementing the legacy function type constructor
// function 'func()'. This function will return 'nullptr' if it does
// not succeed. If it succeeds, it will have already performed AST
// mutation (e.g., replacing the input call).
//
static Expr* maybeResolveFunctionCapturePrimitive(CallExpr* call) {
  if (!call->isPrimitive(PRIM_CAPTURE_FN) &&
      !call->isPrimitive(PRIM_CAPTURE_FN_TO_CLASS) &&
      !call->isPrimitive(PRIM_CREATE_FN_TYPE)) {
    return nullptr;
  }

  if (call->id == breakOnResolveID) gdbShouldBreakHere();

  switch (call->primitive->tag) {

    // Create a closure, a function pointer, or a 'c_fn_ptr'.
    case PRIM_CAPTURE_FN_TO_CLASS:
    case PRIM_CAPTURE_FN: {
      INT_ASSERT(1 <= call->numActuals() && call->numActuals() <= 2);

      FnSymbol* fn = nullptr;
      auto one = call->get(1);

      // First argument always exists and is the name of the function.
      INT_ASSERT(isUnresolvedSymExpr(one) || isSymExpr(one));

      if (auto usym = toUnresolvedSymExpr(one)) {
        auto name = usym->unresolved;
        fn = resolveNameToSingleFunctionOrError(name, usym);
      } else if (auto se = toSymExpr(one)) {
        if (auto known = toFnSymbol(se->symbol())) {
          fn = known;
        }
      }

      if (!fn) return nullptr;

      bool discardType = (call->numActuals() > 1);
      bool useClass = call->isPrimitive(PRIM_CAPTURE_FN_TO_CLASS);

      // Expression return will already be in tree.
      auto ret = resolveFunctionCapture(fn, call, discardType, useClass);

      return ret;
    } break;

    // TODO: Need to have this compute a function type.
    case PRIM_CREATE_FN_TYPE: {
      if (!fcfs::useLegacyBehavior()) {
        USR_WARN(call, "the 'func(...)' function type constructor has "
                       "been deprecated");
        USR_PRINT(call, "consider the builtin 'proc(...)' syntax "
                        "instead");
      } else if (fWarnUnstable) {
        USR_WARN(call, "the 'func(...)' function type constructor is "
                       "unstable");
      }

      if (fcfs::usePointerImplementation()) {
        INT_FATAL(call, "Not supported!");
      }

      Type* t = fcfs::functionClassSuperTypeForFuncConstructor(call);
      auto ret = new SymExpr(t->symbol);
      call->replace(ret);
      return ret;
    } break;

    default: INT_FATAL("Impossible!");
  }

  return nullptr;
}

/************************************* | **************************************
*                                                                             *
* resolveExpr(expr) resolves 'expr' and manages the callStack.                *
* Returns either 'expr' or its substitute.                                    *
*                                                                             *
* The returned Expr is inTree() and has been resolved.                        *
* Exception: if isParamResolved() succeeds and 'expr' is the SymExpr          *
* of the final PRIM_RETURN, it returns 'expr' and it is !inTree().            *
* In this case for_exprs_postorder() terminates.                              *
*                                                                             *
************************************** | *************************************/

static bool        isParamResolved(FnSymbol* fn, Expr* expr);

static Expr*       resolveExprPhase2(Expr* origExpr, FnSymbol* fn, Expr* expr);

static Expr*       resolveExprResolveEachCall(ContextCallExpr* cc);

static bool        contextTypesMatch(FnSymbol* valueFn,
                                     FnSymbol* constRefFn,
                                     FnSymbol* refFn);

static void        contextTypeInfo(FnSymbol* fn);

static void        resolveExprExpandGenerics(CallExpr* call);

static void        resolveExprTypeConstructor(SymExpr* symExpr);

static bool        isStringLiteral(Symbol* sym);

static void        resolveExprMaybeIssueError(CallExpr* call);

static bool        isMentionOfFnTriggeringCapture(SymExpr* se);

Expr* resolveExpr(Expr* expr) {
  FnSymbol* fn     = toFnSymbol(expr->parentSymbol);
  Expr*     retval = NULL;

  if (expr->id == breakOnResolveID)
    gdbShouldBreakHere();

  SET_LINENO(expr);

  if (isContextCallExpr(expr) == true) {
    retval = expr;

  } else if (isParamResolved(fn, expr) == true) {
    retval = expr;

  // This must be after isParamResolved
  } else if (BlockStmt* block = toBlockStmt(expr)) {
    if (ForLoop* forLoop = toForLoop(block)) {
      retval = replaceForWithForallIfNeeded(forLoop);
    } else {
      retval = expr;
    }

  } else if (DefExpr* def = toDefExpr(expr)) {
    Expr* fold = def;
    if (isConstrainedGenericSymbol(def->sym)) {
      resolveConstrainedGenericSymbol(def->sym, false);
    } else if (auto fn = toFnSymbol(def->sym)) {
      INT_ASSERT(!def->init);
      if (fn->isSignature()) {
        fold = resolveFunctionTypeConstructor(def);
      }
    }
    retval = foldTryCond(postFold(fold));
  } else if (SymExpr* se = toSymExpr(expr)) {
    makeRefType(se->symbol()->type);

    if (ForallStmt* pfs = isForallIterExpr(se)) {
      CallExpr* call = resolveForallHeader(pfs, se);
      retval = resolveExprPhase2(expr, fn, preFold(call));
    } else if (isMentionOfFnTriggeringCapture(se)) {
      auto fn = toFnSymbol(se->symbol());
      INT_ASSERT(fn);

      auto e = resolveFunctionCapture(fn, se, false, false);
      if (auto c = toCallExpr(e)) e = preFold(c);
      retval = resolveExprPhase2(expr, fn, e);

    } else {
      retval = resolveExprPhase2(expr, fn, expr);
    }

  } else if (CallExpr* call = toCallExpr(expr)) {
    if (auto e = maybeResolveFunctionCapturePrimitive(call)) {
      if (auto c = toCallExpr(e)) e = preFold(c);
      retval = resolveExprPhase2(expr, fn, e);

    } else {
      // Most calls to resolveCall() are from here.
      retval = resolveExprPhase2(expr, fn, preFold(call));
    }
  } else if (CondStmt* stmt = toCondStmt(expr)) {
    BlockStmt* then = stmt->thenStmt;
    // TODO: Should we just store a boolean field in CondStmt instead?
    if (CallExpr* call = toCallExpr(then->body.tail)) {
      if (call->isPrimitive(PRIM_MOVE)) {
        Symbol* LHS = toSymExpr(call->get(1))->symbol();
        if (LHS->hasFlag(FLAG_IF_EXPR_RESULT)) {
          resolveIfExprType(stmt);
        }
      }
    }
    retval = foldTryCond(postFold(expr));

  } else if (ImplementsStmt* istm = toImplementsStmt(expr)) {
    resolveImplementsStmt(istm);
    retval = istm;

  } else {
    retval = foldTryCond(postFold(expr));
  }

  return retval;
}

static bool isMentionOfFnTriggeringCapture(SymExpr* se) {
  INT_ASSERT(se != nullptr);

  auto fn = toFnSymbol(se->symbol());
  if (!fn) return false;

  if (auto call = toCallExpr(se->parentExpr)) {
    if (call->isPrimitive(PRIM_RESOLUTION_POINT)) return false;
    if (call->isPrimitive(PRIM_END_OF_STATEMENT)) return false;
    if (call->baseExpr == se) return false;
  }

  return true;
}

static bool isParamResolved(FnSymbol* fn, Expr* expr) {
  bool retval = false;

  if (fn != NULL && fn->retTag == RET_PARAM) {
    if (BlockStmt* block = toBlockStmt(expr)) {
      if (block->isWhileStmt() == true) {
        USR_FATAL(expr,
                  "param function cannot contain a non-param while loop");

      } else if (block->isForLoop() == true) {
        USR_FATAL(expr, "param function cannot contain a non-param for loop");

      } else if (block->isLoopStmt() == true) {
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
      INT_ASSERT(call->isPrimitive(PRIM_RETURN));

      call->get(1)->replace(new SymExpr(paramMap.get(fn->getReturnSymbol())));

      retval = true;
    }
  }

  return retval;
}

static Expr* resolveExprPhase2(Expr* origExpr, FnSymbol* fn, Expr* expr) {
  Expr* retval = NULL;

  if (SymExpr* symExpr = toSymExpr(expr)) {
    resolveExprTypeConstructor(symExpr);

    retval = foldTryCond(postFold(expr));

  } else if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIM_ERROR)   == true  ||
        call->isPrimitive(PRIM_WARNING) == true) {
      resolveExprMaybeIssueError(call);
    }

    callStack.add(call);

    resolveCall(call);

    if (call->isResolved()) {
      if (ContextCallExpr* cc = toContextCallExpr(call->parentExpr)) {
        expr = resolveExprResolveEachCall(cc);

      } else {
        resolveFunction(call->resolvedFunction(), call);
      }

      resolveExprExpandGenerics(call);
    }

    callStack.pop();

    retval = foldTryCond(postFold(expr));

  } else {
    retval = foldTryCond(postFold(expr));
  }

  return retval;
}

// A ContextCallExpr wraps 2 or 3 CallExprs.
// Resolve every call and perform semantic checks
static Expr* resolveExprResolveEachCall(ContextCallExpr* cc) {
  FnSymbol* valueFn    = NULL;
  FnSymbol* constRefFn = NULL;
  FnSymbol* refFn      = NULL;
  int       n          =    0;
  int       nIterator  =    0;

  if (CallExpr* tmpCall = cc->getValueCall()) {
    valueFn    = tmpCall->resolvedFunction();

    resolveFunction(valueFn, tmpCall);

    n         += 1;
    nIterator += (valueFn->isIterator()    == true) ? 1 : 0;
  }

  if (CallExpr* tmpCall = cc->getConstRefCall()) {
    constRefFn = tmpCall->resolvedFunction();

    resolveFunction(constRefFn, tmpCall);

    n         += 1;
    nIterator += (constRefFn->isIterator() == true) ? 1 : 0;
  }

  if (CallExpr* tmpCall = cc->getRefCall()) {
    refFn      = tmpCall->resolvedFunction();

    resolveFunction(refFn, tmpCall);

    n         += 1;
    nIterator += (refFn->isIterator()      == true) ? 1 : 0;
  }

  // If there are no iterators then confirm that the return types match.
  //    (The return type of an iterator is not the yielded type)
  if (nIterator == 0) {
    if (contextTypesMatch(valueFn, constRefFn, refFn) == false) {
      USR_FATAL_CONT(cc,
                     "invalid return intent overload: return types differ");

      contextTypeInfo(valueFn);
      contextTypeInfo(constRefFn);
      contextTypeInfo(refFn);

      USR_STOP();
    }

  // If there are any iterators then they must all be iterators
  } else if (nIterator != n) {
    USR_FATAL_CONT(cc, "invalid ref return pair: mixing proc and iter");

    if (valueFn    != NULL) USR_FATAL_CONT(valueFn,    "here");
    if (constRefFn != NULL) USR_FATAL_CONT(constRefFn, "here");
    if (refFn      != NULL) USR_FATAL_CONT(refFn,      "here");
  }

  // Return the "designated call"
  return getDesignatedCall(cc);
}

static bool contextTypesMatch(FnSymbol* valueFn,
                              FnSymbol* constRefFn,
                              FnSymbol* refFn) {
  Type* type   = NULL;
  bool  retval = true;

  if (valueFn != NULL) {
    type   = valueFn->retType->getValType();
  }

  if (constRefFn != NULL) {
    Type* retType = constRefFn->retType->getValType();

    retval = (type == NULL || type == retType) ? true : false;
    type   = retType;
  }

  if (refFn != NULL) {
    retval = (type == refFn->retType->getValType()) ? true : false;
  }

  return retval;
}

static void contextTypeInfo(FnSymbol* fn) {
  if (fn != NULL) {
    USR_FATAL_CONT(fn, "function returns %s", toString(fn->retType));
  }
}

static void resolveExprExpandGenerics(CallExpr* call) {
  for (int i = 1; i <= call->numActuals(); i++) {
    Expr*   actualExpr = call->get(i);
    Symbol* actualSym  = NULL;

    if (SymExpr* actual = toSymExpr(actualExpr)) {
      actualSym = actual->symbol();

    } else if (NamedExpr* named = toNamedExpr(actualExpr)) {
      SymExpr* namedSe = toSymExpr(named->actual);

      INT_ASSERT(namedSe);

      actualSym = namedSe->symbol();

    } else {
      INT_FATAL(actualExpr, "wasn't expecting this type of Expr");
    }

    if (actualSym->hasFlag(FLAG_DELAY_GENERIC_EXPANSION) == true &&
        actualSym->type->symbol->hasFlag(FLAG_GENERIC)   == true) {
      Symbol*        formal     = call->resolvedFunction()->getFormal(i);
      AggregateType* formalType = toAggregateType(formal->type);

      INT_ASSERT(formalType);
      INT_ASSERT(formalType->symbol->hasFlag(FLAG_GENERIC) == false);

      // The type has been determined to no longer be generic.
      // Update the delayed instance to have the right type.
      actualSym->type = formalType;

      actualSym->removeFlag(FLAG_DELAY_GENERIC_EXPANSION);

      formalType->initializerResolved = true;

      if (actualSym->hasFlag(FLAG_SUPER_TEMP)) {
        if (FnSymbol* fn = toFnSymbol(actualExpr->parentSymbol)) {
          if (fn->_this != NULL && isClass(fn->_this->type) == true) {
            AggregateType* ct         = toAggregateType(fn->_this->type);
            Symbol*        superField = ct->getField(1);

            if (superField->hasFlag(FLAG_DELAY_GENERIC_EXPANSION)) {
              bool wasGeneric = ct->symbol->hasFlag(FLAG_GENERIC);
              ct              = ct->getInstantiationParent(formalType);
              fn->_this->type = ct;

              superField      = ct->getField(1);
              superField->removeFlag(FLAG_DELAY_GENERIC_EXPANSION);

              if (wasGeneric == true && ct->symbol->hasFlag(FLAG_GENERIC) == false) {
                resolveTypeWithInitializer(ct, fn);
              }
            }
          }
        }
      }
    }
  }
}

static
void resolveTypeConstructor(AggregateType* at) {
  forv_Vec(AggregateType, pt, at->dispatchParents) {
    resolveTypeConstructor(pt);
  }
  at->resolveConcreteType();
  if (at->scalarPromotionType == NULL &&
      at->symbol->hasFlag(FLAG_REF) == false) {
    resolvePromotionType(at);
  }

  if (at->hasDestructor() == false) {
    resolveDestructor(at);
  }
}

// TODO: Why shouldn't this handle generics too?
static void resolveExprTypeConstructor(SymExpr* symExpr) {
  if (symExpr->typeInfo() == NULL) {
    return;
  }

  Type* t = symExpr->getValType();
  AggregateType* at = toAggregateType(t);

  if (DecoratedClassType * dt = toDecoratedClassType(t))
    at = dt->getCanonicalClass();

  // Do not run on instantiated generics
  if (at != NULL && at->instantiatedFrom == NULL) {
    if (at->symbol->hasFlag(FLAG_GENERIC)         == false  &&
        at->symbol->hasFlag(FLAG_ITERATOR_CLASS)  == false  &&
        at->symbol->hasFlag(FLAG_ITERATOR_RECORD) == false &&
        at->resolveStatus == UNRESOLVED) {
      CallExpr* parent = toCallExpr(symExpr->parentExpr);
      Symbol*   sym    = symExpr->symbol();

      if (parent                               == NULL  ||
          parent->isPrimitive(PRIM_IS_SUBTYPE) == false ||
          sym->hasFlag(FLAG_TYPE_VARIABLE)     == false) {
        if (isStringLiteral(sym) == false) {

          // Resolve type constructors for this type
          // as well as any parent types it has.
          resolveTypeConstructor(at);
        }
      }
    }
  }
}

static bool isStringLiteral(Symbol* sym) {
  bool retval = false;

  if (sym->type == dtString) {
    if (sym->isParameter()                    == true ||
        sym->hasFlag(FLAG_INSTANTIATED_PARAM) == true) {
      retval = true;
    }
  }

  return retval;
}

static void resolveExprMaybeIssueError(CallExpr* call) {
  //
  // Disable compiler warnings in internal modules that are triggered within
  // a dynamic dispatch context to reduce potential user confusion.
  //
  if (call->isPrimitive(PRIM_ERROR)         == true          ||
      inDynamicDispatchResolution           == false         ||
      callStack.head()->getModule()->modTag != MOD_INTERNAL) {

    //
    // If an errorDepth was specified, report a diagnostic about the call
    // that deep into the callStack. The default depth is 1.
    //
    FnSymbol*   fn            = toFnSymbol(call->parentSymbol);
    DefExpr*    lastFormal    = toDefExpr(fn->formals.tail);
    VarSymbol*  depthParam    = toVarSymbol(paramMap.get(lastFormal->sym));
    int         depth         = 0;
    int         head          = 0;
    bool        foundDepthVal = false;
    CallExpr*   from          = NULL;
    const char* str           = "";
    VarSymbol*  var           = NULL;

    if (depthParam                        != NULL &&
        depthParam->immediate             != NULL &&
        depthParam->immediate->const_kind == NUM_KIND_INT) {
      depth         = (int) depthParam->immediate->int_value();
      foundDepthVal = true;

      if (depth > callStack.n - 1) {
        USR_WARN(call,
                 "compiler diagnostic depth value exceeds call stack depth");

        depth = callStack.n - 1;

      } else if (depth < 0) {
        USR_WARN(call, "compiler diagnostic depth value can not be negative");

        depth = 0;
      }

    } else {
      depth         = (callStack.n == 1) ? 0 : 1;
      foundDepthVal = false;
    }

    head = callStack.n - 1 - depth;

    FnSymbol* tryResolveFunction = NULL;
    if (tryResolveFunctions.size() > 0)
      tryResolveFunction = tryResolveFunctions.back();

    for (int i = callStack.n-1; i >= 0; i--) {
      CallExpr*     frame  = callStack.v[i];
      ModuleSymbol* module = frame->getModule();
      FnSymbol*     fn     = frame->getFunction();

      from = frame;

      // If we are in a tryResolve call, record the error at the
      // function being attempted, instead of at a further up call site,
      // so that the message makes sense when reissued.
      if (tryResolveFunction != NULL && fn == tryResolveFunction)
        break;

      if (i <= head &&
          frame->linenum() > 0                                  &&
          fn->hasFlag(FLAG_COMPILER_GENERATED) == false         &&
          (developer || module->modTag != MOD_INTERNAL)         ) {
        break;
      }
    }

    for_formals(arg, fn) {
      if (foundDepthVal == false || arg->defPoint != fn->formals.tail) {
        var = toVarSymbol(paramMap.get(arg));

       if         (var                        != NULL &&
                   var->immediate             != NULL &&
                   var->immediate->const_kind != CONST_KIND_STRING)
        USR_FATAL(from, "arguments to compilerWarning() and compilerError(),"
          " except for the optional depth argument, must be cast to string");
       else
        str = astr(str, var->immediate->v_string.c_str());
      }
    }

    // collapse newlines and other escape sequences before printing
    str = astr(chpl::unescapeStringC(str).c_str());

    if (call->isPrimitive(PRIM_ERROR) == true) {
      if (inTryResolve == 0) {
        USR_FATAL(from, "%s", str);
        if (FnSymbol* fn = callStack.tail()->resolvedFunction())
          innerCompilerErrorMap[fn] = str;
        if (FnSymbol* fn = callStack.v[head]->resolvedFunction())
          outerCompilerErrorMap[fn] = str;
      } else {
        tryResolveStates.back() = CHECK_FAILED;

        if (tryResolveFunctions.size() > 0) {
          FnSymbol* fn = tryResolveFunctions.back();

          tryResolveErrors[fn] = std::make_pair(from,str);
        }

        // Make any errors in initCopy / autoCopy mark the function
        // with the flag for later errors (during callDestructors)
        for (int i = callStack.n-1; i >= 0; i--) {
          CallExpr*     frame  = callStack.v[i];
          FnSymbol*     fn     = frame->getFunction();
          bool inCopyIsh = fn->hasFlag(FLAG_INIT_COPY_FN) ||
                           fn->hasFlag(FLAG_AUTO_COPY_FN) ||
                           fn->hasFlag(FLAG_COERCE_FN) ||
                           fn->name == astrInitEquals;
          if (inCopyIsh) {
            fn->addFlag(FLAG_ERRONEOUS_COPY);
            tryResolveErrors[fn] = std::make_pair(from,str);
          }
        }
      }

    } else {
      USR_WARN (from, "%s", str);
      if (FnSymbol* fn = callStack.tail()->resolvedFunction())
        innerCompilerWarningMap[fn] = str;
      if (FnSymbol* fn = callStack.v[head]->resolvedFunction())
        outerCompilerWarningMap[fn] = str;
    }
  }
}

static Expr* foldTryCond(Expr* expr) {
  Expr* retval = expr;

  if (CondStmt* cond = toCondStmt(expr->parentExpr))
    if (cond->condExpr == expr)
      if (CallExpr* noop = cond->foldConstantCondition(true))
        retval = noop;

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

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


static void resolveExterns()
{
  // This code resolves extern variable and type definitions
  // early in resolution.
  forv_Vec(ModuleSymbol, mod, allModules) {
    for_alist(expr, mod->initFn->body->body) {
      if (BlockStmt* block = toBlockStmt(expr)) {
        if ((block->blockTag & BLOCK_EXTERN)) {
          resolveBlockStmt(block);
        }
      }
    }
  }
}


static void adjustInternalSymbols() {
  SET_LINENO(rootModule);

  // call _nilType nil so as to not confuse the user
  dtNil->symbol->name = gNil->name;

  // we want gDummyRef to be passable to 'ref' formals
  makeRefType(dtDummyRef);
  gDummyRef->type = dtDummyRef->getRefType();
  gDummyRef->qual = QUAL_REF;
  gDummyRef->addFlag(FLAG_REF);
  gDummyRef->removeFlag(FLAG_CONST);

  startInterfaceChecking();
}


static bool isObviousType(Type* type) {
  return isPrimitiveType(type) && ! type->isInternalType;
}

static bool isObviousValue(Symbol* val) {
  return val->isImmediate() || isEnumSymbol(val) || val == gUninstantiated || paramMap.get(val);
}

//
// Set the type and/or update paramMap for global variables that have
// a simple type or init expression, ex. dtBool or gFalse.
// This allows us to place such globals freely in the internal modules.
// Restrict this to internal modules to avoid surprises for users.
//
static void resolveObviousGlobals() {
  forv_Vec(ModuleSymbol, mod, allModules)
   if (mod->modTag == MOD_INTERNAL)
    for_alist(expr, mod->initFn->body->body)
     if (CallExpr* call = toCallExpr(expr))
      if (call->isPrimitive(PRIM_INIT_VAR) ||
          call->isPrimitive(PRIM_MOVE)     )
       if (SymExpr* lhsSE = toSymExpr(call->get(1)))
        if (SymExpr* rhsSE = toSymExpr(call->get(2)))
         {
           Symbol* lhs = lhsSE->symbol();
           Symbol* rhs = rhsSE->symbol();

           if (lhs->hasEitherFlag(FLAG_TEMP, FLAG_EXPR_TEMP))
             continue; // handle only user variables

           if (lhs->hasFlag(FLAG_PARAM)) {
             // If there is an obvious rhs value, use it.
             // Do a full resolve for uniformity, to ensure paramMap, etc.
             if (isObviousValue(rhs))
               if (Expr* result = resolveExpr(call))
                 // We do not want 'call' to be resolved a second time.
                 INT_ASSERT(isCallExpr(result) &&
                            toCallExpr(result)->isPrimitive(PRIM_NOOP));

           } else if (lhs->hasFlag(FLAG_TYPE_VARIABLE)) {
             // If there is an obvious rhs type, use it.
             if (isObviousType(rhs->type))
               if (Expr* result = resolveExpr(call))
                 // As of this writing, 'call' remains in the AST unchanged.
                 INT_ASSERT(result == call);
           }
         }
}


/************************************* | **************************************
*                                                                             *
*  Check for void fields as errors                                            *
*                                                                             *
************************************** | *************************************/

static void checkNoVoidFields()
{
  for_alive_in_Vec(AggregateType, at, gAggregateTypes) {
    // We exclude "no object" aggregates, to allow the Chapel internal '_ref'
    // class to be instantiated with a 'void' member
    if(!at->symbol->hasFlag(FLAG_NO_OBJECT)) {
      for_fields(field, at) {
        if(field->type == dtVoid)
          USR_FATAL(field,
                    "Field '%s' cannot be declared 'void'."
                    " Consider using 'nothing' instead.",
                    field->name);
      }
    }
  }
}


void resolve() {
  parseExplainFlag(fExplainCall, &explainCallLine, &explainCallModule);

  unmarkDefaultedGenerics();

  adjustInternalSymbols();

  resolveExterns();

  resolveObviousGlobals();

  resolveUsesAndModule(ModuleSymbol::mainModule(), "");

  // Also resolve modules mentioned on command line
  // Could rely on init calls in chpl_gen_main for this.
  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    if (mod->hasFlag(FLAG_MODULE_FROM_COMMAND_LINE_FILE)) {
      resolveUsesAndModule(mod, "");
    }
  }

  if (printModuleInitModule)
    resolveUsesAndModule(printModuleInitModule, "");

  if (chpl_gen_main)
    resolveFunction(chpl_gen_main);

  resolveSupportForModuleDeinits();

  USR_STOP();

  finishInterfaceChecking();  // should happen before resolveAutoCopies

  resolveExports();

  resolveEnumTypes();

  populateRuntimeTypeMap();

  resolveAutoCopies();

  resolveOther();

  resolveDynamicDispatches();

  // MPF - this 2nd resolveAutoCopies call is a workaround
  // for the case when resolving a dynamic dispatch created a
  // new type. Resolution should instead have some sort of work-queue
  // and iterate until everything is resolved. (Or at least
  // resolveDynamicDispatches and resolveAutoCopies should be called
  // in a loop).
  resolveAutoCopies();

  resolveSerializers();

  resolveDestructors();

  insertReturnTemps();

  insertDynamicDispatchCalls();

  handleRuntimeTypes();

  if (fPrintCallGraph) {
    // This needs to go after resolution is complete, but before
    // pruneResolvedTree() removes unused functions (like the uninstantiated
    // versions of generic functions).
    printCallGraph();
  }

  if (fPrintUnusedFns || fPrintUnusedInternalFns)
    printUnusedFunctions();

  saveGenericSubstitutions();

  checkNoVoidFields();

  pruneResolvedTree();

  resolveForallStmts2();

  finalizeForallOptimizationsResolution();

  freeCache(genericsCache);
  freeCache(promotionsCache);

  visibleFunctionsClear();

  std::map<int, SymbolMap*>::iterator it;

  for (it = capturedValues.begin(); it != capturedValues.end(); ++it) {
    delete it->second;
  }

  clearPartialCopyDataFnMap();

  clearCoercibleCache();

  forv_Vec(BlockStmt, stmt, gBlockStmts) {
    stmt->useListClear();
    stmt->modRefsClear();
  }

  resolved = true;
}

/************************************* | **************************************
*                                                                             *
* Make it so that arguments with types that have default values               *
* for all generic arguments used with those defaults                          *
*                                                                             *
* FLAG_MARKED_GENERIC is used to identify places where the user               *
* inserted '?' (queries) to mark such a type as generic.                      *
*                                                                             *
************************************** | *************************************/

static void unmarkDefaultedGenerics() {
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
      for_formals(formal, fn) {
        AggregateType* formalAt   = toAggregateType(formal->type);
        bool typeHasGenericDefaults = false;
        if (formalAt && formalAt->isGenericWithDefaults())
          typeHasGenericDefaults = true;

        if (formal                               != fn->_this &&
            typeHasGenericDefaults                            &&
            formal->hasFlag(FLAG_MARKED_GENERIC) == false) {
          SET_LINENO(formal);

          formal->type     = dtUnknown;
          formal->typeExpr = new BlockStmt(new CallExpr(formalAt->symbol));

          insert_help(formal->typeExpr, NULL, formal);
        }
      }
  }
}

/************************************* | **************************************
*                                                                             *
* Resolve uses in postorder (removing back-links).                            *
*                                                                             *
* We have to resolve modules in dependency order so that                      *
* the types of globals are ready when we need them.                           *
*                                                                             *
************************************** | *************************************/

static std::set<ModuleSymbol*> moduleInitResolved;

static void resolveUsesAndModule(ModuleSymbol* mod, const char* path) {
  if (moduleInitResolved.count(mod) == 0) {
    moduleInitResolved.insert(mod);

    if (fPrintModuleResolution == true) {
      // update path variable
      if (path == NULL || path[0] == '\0')
        path = mod->name;
      else
        path = astr(path, ".", mod->name);
    }

    if (ModuleSymbol* parent = mod->defPoint->getModule()) {
      if (parent != theProgram && parent != rootModule) {
        resolveUsesAndModule(parent, path);
      }
    }

    for_vector(ModuleSymbol, usedMod, mod->modUseList) {
      resolveUsesAndModule(usedMod, path);
    }

    if (fPrintModuleResolution == true) {
      fprintf(stderr, "%s\n  from %s\n", mod->name, path);
    }

    resolveSignatureAndFunction(mod->initFn);

    if (FnSymbol* defn = mod->deinitFn) {
      resolveSignatureAndFunction(defn);
    }

    if (fPrintModuleResolution == true) {
      AstCount visitor = AstCount();

      mod->accept(&visitor);

      if (developer)
        fprintf(stderr, "%s contains %6d asts\n", mod->name, visitor.total());
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveSupportForModuleDeinits() {
  SET_LINENO(chpl_gen_main);

  Expr*      modNameDum = buildCStringLiteral("");
  VarSymbol* fnPtrDum   = newTemp("fnPtr", dtCFnPtr);
  CallExpr*  addModule  = new CallExpr("chpl_addModule", modNameDum, fnPtrDum);

  resolveUninsertedCall(chpl_gen_main->body, addModule, true);

  gAddModuleFn = addModule->resolvedFunction();

  resolveFunction(gAddModuleFn);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveExports() {
  std::vector<FnSymbol*> exps;

  // We need to resolve any additional functions that will be exported.
  forv_expanding_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn == initStringLiterals) {
      // initStringLiterals is exported but is explicitly resolved last since
      // code may be added to it in postFold calls while resolving other
      // functions
      continue;
    }

    if (fn->hasFlag(FLAG_EXPORT) || fn->hasFlag(FLAG_ALWAYS_RESOLVE)) {
      SET_LINENO(fn);

      resolveSignatureAndFunction(fn);

      if (fn->hasFlag(FLAG_EXPORT))
        exps.push_back(fn);
    }
  }

  fixupExportedFunctions(exps);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveEnumTypes() {
  // need to handle enumerated types better
  for_alive_in_Vec(TypeSymbol, type, gTypeSymbols) {
    if (EnumType* et = toEnumType(type->type)) {
      // skip irrelevant code
      if (FnSymbol* parentFn = et->symbol->defPoint->getFunction())
        if (! parentFn->isResolved())
          continue;

      SET_LINENO(et);
      ensureEnumTypeResolved(et);
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void populateRuntimeTypeMap() {
  for_alive_in_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->hasFlag(FLAG_HAS_RUNTIME_TYPE) &&
        !ts->hasFlag(FLAG_GENERIC)) {
      AggregateType* at = toAggregateType(ts->type);
      INT_ASSERT(at);

      if (valueToRuntimeTypeMap.get(at))
        continue;

      SET_LINENO(ts);
      VarSymbol* tmp = newTemp("_runtime_type_tmp_", at);
      at->symbol->defPoint->insertBefore(new DefExpr(tmp));
      CallExpr* call = new CallExpr("chpl__convertValueToRuntimeType", tmp);
      FnSymbol* fn = resolveUninsertedCall(at, call, true);
      resolveFunction(fn);
      valueToRuntimeTypeMap.put(at, fn);
      tmp->defPoint->remove();
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static FnSymbol* resolveNormalSerializer(CallExpr* call) {
  BlockStmt* block = new BlockStmt(call, BLOCK_SCOPELESS);

  chpl_gen_main->body->insertAtHead(block);

  tryResolveCall(call);

  block->remove();

  FnSymbol* ret = call->resolvedFunction();

  return ret;
}

static bool resolveSerializeDeserialize(AggregateType* at) {
  SET_LINENO(at->symbol);
  VarSymbol* tmp          = newTemp(at);
  FnSymbol* serializeFn   = NULL;
  FnSymbol* deserializeFn = NULL;
  bool retval             = false;

  chpl_gen_main->insertAtHead(new DefExpr(tmp));

  CallExpr* serializeCall = new CallExpr("chpl__serialize", gMethodToken, tmp);
  serializeFn = resolveNormalSerializer(serializeCall);
  if (serializeFn != NULL && serializeFn->hasFlag(FLAG_PROMOTION_WRAPPER)) {
    // Without this check we would resolve a serializer for arrays despite a
    // serializer only being implemented for domains.
    serializeFn = NULL;
  }

  if (serializeFn != NULL) {
    resolveFunction(serializeFn);
    Type* retType = serializeFn->retType->getValType();

    if (retType == dtVoid) {
      USR_FATAL(serializeFn, "chpl__serialize cannot return void");
    }

    // Make sure we have resolved autocopy / autodestroy etc
    if (AggregateType* at = toAggregateType(retType)) {
      resolveAutoCopyEtc(at);
      propagateNotPOD(at);
    }

    if (isPrimitiveType(retType) == false &&
        (autoDestroyMap.get(retType) == NULL ||
         isClass(retType))) {
      USR_FATAL_CONT(serializeFn, "chpl__serialize must return a type that can be automatically memory managed (e.g. a record)");
      serializeFn = NULL;
    } else {
      VarSymbol* data = newTemp(serializeFn->retType);
      chpl_gen_main->insertAtHead(new DefExpr(data));

      CallExpr* deserializeCall = new CallExpr("chpl__deserialize", gMethodToken, at->symbol, data);
      deserializeFn = resolveNormalSerializer(deserializeCall);

      if (deserializeFn != NULL) {
        resolveFunction(deserializeFn);

        Type* retType = deserializeFn->retType->getValType();

        // Class decorators shouldn't matter for RVF, right? Or should they
        // always be unmanaged?
        if (DecoratedClassType* decType = toDecoratedClassType(retType)) {
          retType = decType->getCanonicalClass();
        }
        if (retType == dtVoid) {
          USR_FATAL(deserializeFn, "chpl__deserialize cannot return void");
        } else if (retType != at) {
          const char* rt = (developer == false) ? retType->symbol->name
                                                : retType->symbol->cname;
          const char* att =  (developer == false) ? at->symbol->name
                                                  : at->symbol->cname;
          USR_FATAL(deserializeFn, "chpl__deserialize returning '%s' when it must return '%s'", rt, att);
        }
      }

      data->defPoint->remove();
    }
  }

  if (serializeFn != NULL && deserializeFn == NULL) {
    USR_WARN("Found chpl__serialize for type '%s', but did not find matching chpl__deserialize", at->symbol->cname);
  } else if (serializeFn != NULL && deserializeFn != NULL) {
    Serializers ser;
    ser.serializer = serializeFn;
    ser.deserializer = deserializeFn;
    serializeMap[at] = ser;

    retval = true;
  }

  // remove the temporary used in resolving
  tmp->defPoint->remove();

  return retval;
}

static void resolveBroadcasters(AggregateType* at) {
  SET_LINENO(at->symbol);
  Serializers& ser = serializeMap[at];
  INT_ASSERT(ser.serializer != NULL);

  if (ser.serializer->hasFlag(FLAG_COMPILER_GENERATED) &&
     (ser.deserializer->hasFlag(FLAG_COMPILER_GENERATED))) {
    // this was generated for RVF only. TODO should we be able to broadcast
    // promotion iterator records?
    return;
  }

  VarSymbol* tmp = newTemp("global_temp", at);
  chpl_gen_main->insertAtHead(new DefExpr(tmp));

  FnSymbol* broadcastFn;
  FnSymbol* destroyFn;
  {
    SET_LINENO(tmp);
    CallExpr* call = new CallExpr("chpl__broadcastGlobal", tmp, new_IntSymbol(0, INT_SIZE_64));
    broadcastFn = resolveNormalSerializer(call);
    broadcastFn->addFlag(FLAG_BROADCAST_FN);
  }
  {
    SET_LINENO(tmp);
    CallExpr* call = new CallExpr("chpl__destroyBroadcastedGlobal", tmp, new_IntSymbol(0, INT_SIZE_64));
    destroyFn = resolveNormalSerializer(call);
  }
  if (broadcastFn == NULL || destroyFn == NULL) {
    INT_FATAL("Unable to resolve serialized broadcasting for type %s", at->symbol->cname);
  }

  resolveFunction(broadcastFn);
  resolveFunction(destroyFn);

  ser.broadcaster = broadcastFn;
  ser.destroyer   = destroyFn;

  // remove the temporary used to resolve
  tmp->defPoint->remove();
}

static FnSymbol* createMethodStub(AggregateType* at, const char* methodName,
                                  bool isType) {
  // taken from build_accessor
  FnSymbol* fn = new FnSymbol(methodName);
  fn->addFlag(FLAG_COMPILER_GENERATED);

  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));

  fn->setMethod(true);

  Type* thisType = at->getValType();
  if (isClass(at) && isType)
    thisType = at->getDecoratedClass(ClassTypeDecorator::GENERIC);
  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", thisType);

  _this->addFlag(FLAG_ARG_THIS);
  fn->insertFormalAtTail(_this);
  if (isType) {
    _this->addFlag(FLAG_TYPE_VARIABLE);
  }

  DefExpr* def = new DefExpr(fn);
  at->symbol->defPoint->insertBefore(def);
  at->methods.add(fn);
  fn->addFlag(FLAG_METHOD_PRIMARY);
  fn->_this = _this;

  return fn;
}

static bool resolveSerializeDeserialize(AggregateType* at);
static bool createSerializeDeserialize(AggregateType* at);

static bool ensureSerializersExist(AggregateType* at) {
  Symbol* ts = at->symbol;

  if (serializeMap.find(at) != serializeMap.end()) {
    return true;
  }

  if (ts->hasFlag(FLAG_PROMOTION_ITERATOR_RECORD)) {
    return createSerializeDeserialize(at);
  }
  else if (! ts->hasFlag(FLAG_GENERIC)                &&
           ! isSingleType(ts->type)                   &&
           ! isSyncType(ts->type)                     &&
           ! ts->hasFlag(FLAG_ITERATOR_RECORD)        &&
           ! ts->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION)) {
    if (at != NULL) {
      if (isRecord(at) == true ||
          (isClass(at) == true && !at->symbol->hasFlag(FLAG_REF))) {
        return resolveSerializeDeserialize(at);
      }
    }
  }

  return false;
}

static AggregateType* getSerializableFieldType(Symbol* field) {
  AggregateType* fieldAggType = NULL;
  if (DecoratedClassType* decClassType = toDecoratedClassType(field->type)) {
    fieldAggType = toAggregateType(decClassType->getCanonicalClass());
  }
  else {
    fieldAggType = toAggregateType(field->type);
  }

  if (fieldAggType) {
    fieldAggType = toAggregateType(fieldAggType->getValType());
  }

  return fieldAggType;
}

static bool createSerializeDeserialize(AggregateType* at) {

  if (at->numFields() == 0) {
    return false;
  }

  SET_LINENO(at);
  bool retval = true;

  FnSymbol* serializer = createMethodStub(at, "chpl__serialize",
                                          /*isType=*/false);

  ArgSymbol* _this = toArgSymbol(serializer->_this);
  INT_ASSERT(_this);

  CallExpr* buildTuple = new CallExpr("_build_tuple");

  std::vector<FnSymbol*> deserializers;

  for_fields (field, at) {
    if (AggregateType* fieldValType = getSerializableFieldType(field)) {
      if (ensureSerializersExist(fieldValType)) {
        Serializers& ser = serializeMap[fieldValType];
        FnSymbol* fieldSerializer = ser.serializer;
        deserializers.push_back(ser.deserializer);

        VarSymbol* fieldTmp = newTemp("ser_field_tmp");
        fieldTmp->addFlag(FLAG_REF);
        serializer->insertAtTail(new DefExpr(fieldTmp));

        CallExpr* getField;
        if (field->type->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
          getField =  new CallExpr(PRIM_ITERATOR_RECORD_FIELD_VALUE_BY_FORMAL, _this,
                                          field);
        }
        else {
          getField =  new CallExpr(PRIM_GET_MEMBER, _this,
                                          new_CStringSymbol(field->name));
        }
        serializer->insertAtTail(new CallExpr(PRIM_ASSIGN, fieldTmp, getField));
        // update the serializer body
        buildTuple->insertAtTail(new CallExpr(fieldSerializer, gMethodToken, fieldTmp));
      }
      else {
        retval = false;
        break;
      }
    }
    else if (isPOD(field->type)) {
      buildTuple->insertAtTail(new CallExpr(PRIM_GET_MEMBER_VALUE, _this,
                                            new_CStringSymbol(field->name)));
      deserializers.push_back(NULL);
    }
    else {
      INT_FATAL("Unhandled case");
    }
  }

  if (!retval) {
    return false;
  }

  serializer->insertAtTail(new CallExpr(PRIM_RETURN, buildTuple));
  normalize(serializer);
  resolveFunction(serializer);
  Type* serialDataType = serializer->retType;

  FnSymbol* deserializer = createMethodStub(at, "chpl__deserialize",
                                            /*isType=*/true);

  if (at->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    deserializer->addFlag(FLAG_FN_RETURNS_ITERATOR);
  }

  ArgSymbol* deserializerFormal = new ArgSymbol(INTENT_CONST_IN, "data", serialDataType);
  deserializer->insertFormalAtTail(deserializerFormal);
  VarSymbol* deserializerRet = new VarSymbol("deserializer_return", at);

  deserializer->insertAtTail(new DefExpr(deserializerRet));

  int fieldNum = 0;
  for_fields (field, at) {

    CallExpr* dataGetField = new CallExpr(PRIM_GET_MEMBER_VALUE, deserializerFormal,
                                          new_CStringSymbol(astr("x", istr(fieldNum))));

    if (FnSymbol* fieldDeserializer = deserializers[fieldNum]) {

      VarSymbol* deserMarker = newTemp("deser_marker", dtBool);
      deserMarker->addFlag(FLAG_DESERIALIZATION_BLOCK_MARKER);
      deserializer->insertAtTail(new DefExpr(deserMarker));

      BlockStmt* varDeser = new BlockStmt();
      BlockStmt* refDeser = new BlockStmt();

      CondStmt* deserVersions = new CondStmt(new SymExpr(deserMarker),
                                             varDeser,
                                             refDeser);

      // create type temp
      VarSymbol* typeTemp = newTemp("field_type", field->getValType());
      typeTemp->addFlag(FLAG_TYPE_VARIABLE);
      varDeser->insertAtTail(new DefExpr(typeTemp));

      // create deserialization call
      CallExpr* fieldDeserialize = new CallExpr(fieldDeserializer, dataGetField);
      fieldDeserialize->insertAtHead(typeTemp);
      fieldDeserialize->insertAtHead(gMethodToken);

      varDeser->insertAtTail(new CallExpr(PRIM_SET_MEMBER, deserializerRet,
                                          field,
                                          fieldDeserialize));

      refDeser->insertAtTail(new CallExpr(PRIM_SET_MEMBER, deserializerRet,
                                          field,
                                          new CallExpr(PRIM_REF_DESERIALIZE,
                                                       typeTemp,
                                                       deserializerFormal,
                                                       new_IntSymbol(fieldNum))));

      deserializer->insertAtTail(deserVersions);
    }
    else {
      deserializer->insertAtTail(new CallExpr(PRIM_SET_MEMBER, deserializerRet,
                                         field,
                                         dataGetField));
    }

    fieldNum++;
  }

  deserializer->insertAtTail(new CallExpr(PRIM_RETURN, deserializerRet));
  normalize(deserializer);
  resolveFunction(deserializer);
  INT_ASSERT(deserializer->retType == toDefExpr(deserializer->formals.get(2))->sym->type);
  retval = resolveSerializeDeserialize(at); // now this should work

  return retval;
}

static void resolveSerializers() {
  if (fNoRemoteSerialization == true) {
    return;
  }

  for_alive_in_expanding_Vec(TypeSymbol, ts, gTypeSymbols) {

    if (AggregateType* at = toAggregateType(ts->type)) {
      bool hasSerializers = ensureSerializersExist(at);

      if (hasSerializers) {
        resolveBroadcasters(at);
      }
    }
  }

  resolveAutoCopies();
}

static void resolveDestructors() {
  std::set<Type*> wellknown = getWellKnownTypesSet();

  for_alive_in_expanding_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (! ts->hasFlag(FLAG_REF)                     &&
        ! ts->hasFlag(FLAG_GENERIC)                 &&
        ! ts->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION)) {
      if (AggregateType* at = toAggregateType(ts->type)) {
        if (at->hasDestructor()          == false &&
            at->hasInitializers()        == true  &&
            isUnusedClass(at, wellknown) == false) {
          resolveDestructor(at);
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static const char* autoCopyFnForType(AggregateType* at);

static void resolveAutoCopies() {
  for_alive_in_expanding_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (! ts->hasFlag(FLAG_GENERIC)                 &&
        ! ts->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION)) {
      if (AggregateType* at = toAggregateType(ts->type)) {
        if (isRecord(at) || isUnion(at)) {
          // If we attempt to resolve auto-copy and co. for an infinite record
          // we may enter an infinite loop and the compiler will crash.
          checkForInfiniteRecord(at);
          resolveAutoCopyEtc(at);
        }

        propagateNotPOD(at);
      }
    }
  }
}

static void resolveAutoCopyEtc(AggregateType* at) {
  SET_LINENO(at->symbol);

  if (typeNeedsCopyInitDeinit(at) == false)
    return;

  // resolve autoCopy
  if (hasAutoCopyForType(at) == false) {
    FnSymbol* fn = autoMemoryFunction(at, autoCopyFnForType(at));

    autoCopyMap[at] = fn;
  }

  // resolve destructor
  if (at->hasDestructor() == false) {
    if (at->symbol->hasFlag(FLAG_REF)             == false &&
        isTupleContainingOnlyReferences(at)       == false &&
        // autoDestroy for iterator record filled in callDestructors
        at->symbol->hasFlag(FLAG_ITERATOR_RECORD) == false) {

      // Resolve a call to deinit
      VarSymbol* tmp   = newTemp(at);
      CallExpr*  call  = new CallExpr("deinit", gMethodToken, tmp);

      FnSymbol* fn = resolveUninsertedCall(at->symbol->defPoint, call, true);
      INT_ASSERT(fn);

      if (at->hasInitializers()) {
        fn->setInstantiationPoint(at->symbol->instantiationPoint);
      }

      resolveFunction(fn);

      at->setDestructor(fn);

    }
  }

  // resolve autoDestroy
  if (autoDestroyMap.get(at) == NULL) {
    FnSymbol* fn = autoMemoryFunction(at, astr_autoDestroy);
    // If --minimal-modules is used, `chpl_autoDestroy` won't be defined
    if (fn)
      fn->addFlag(FLAG_AUTO_DESTROY_FN);
    autoDestroyMap.put(at, fn);
  }
}

// Just use 'chpl__initCopy' instead of 'chpl__autoCopy'
// for user-defined records. This way, if the type does not
// support copying, the autoCopyMap will store a function
// marked with FLAG_ERRONEOUS_INITCOPY. Additionally, user-defined
// records shouldn't be defining chpl__initCopy or chpl__autoCopy
// and certainly shouldn't rely on the differences between the two.
static const char* autoCopyFnForType(AggregateType* at) {
  const char* retval = astr_autoCopy;

  if (typeNeedsCopyInitDeinit(at)            == true  &&
      at->symbol->hasFlag(FLAG_TUPLE)        == false &&
      at->symbol->hasFlag(FLAG_ITERATOR_RECORD) == false &&
      isRecordWrappedType(at)                == false &&
      isSyncType(at)                         == false &&
      isSingleType(at)                       == false &&
      at->symbol->hasFlag(FLAG_COPY_MUTATES) == false) {
    retval = astr_initCopy;
  }

  return retval;
}

static FnSymbol* autoMemoryFunction(AggregateType* at, const char* fnName) {
  VarSymbol* tmp    = newTemp(at);
  CallExpr*  call   = NULL;

  if (fnName == astr_initCopy || fnName == astr_autoCopy) {
    call = new CallExpr(fnName, tmp, new SymExpr(gFalse));
  }
  else {
    call = new CallExpr(fnName, tmp);
  }




  chpl_gen_main->insertAtHead(new DefExpr(tmp));

  FnSymbol* foundFn = resolveUninsertedCall(at, call, false);
  FnSymbol* resolvedFn = fixInstantiationPointAndTryResolveBody(at, call);
  FnSymbol* retval = NULL;

  if (foundFn != NULL)
    retval = resolvedFn;

  if (retval == NULL) {
    if (FnSymbol* fn = call->resolvedFunction()) {
      // if it's an initCopy e.g. we should have already marked it as erroneous
      if (fn->hasFlag(FLAG_INIT_COPY_FN) ||
          fn->hasFlag(FLAG_AUTO_COPY_FN))
        INT_ASSERT(fn->hasFlag(FLAG_ERRONEOUS_COPY));

      // Return the resolved function, for storing in the map
      retval = fn;
    }
  }

  if (retval != NULL)
    INT_ASSERT(retval->hasFlag(FLAG_PROMOTION_WRAPPER) == false);

  tmp->defPoint->remove();

  return retval;
}

/************************************* | **************************************
*                                                                             *
* In order to correctly initialize records or tuples in which a component     *
* has FLAG_IGNORE_NOINIT, we need to propagate that flag to the parent types  *
* as well.                                                                    *
*                                                                             *
* While doing so, we also propagate whether or not that type is not POD       *
* (Plain-Old-Data which would mean it does not need auto copies or auto       *
* destroys - bit copies will do).                                             *
*                                                                             *
* After this function is called on an aggregate type, that type will be       *
* marked FLAG_POD and FLAG_NOT_POD, and this function will be called          *
* recursively to also mark any aggregate type fields with FLAG_POD or         *
* FLAG_NOT_POD.                                                               *
*                                                                             *
* After setting either FLAG_POD or FLAG_NOT_POD if necessary, returns true    *
* if FLAG_NOT_POD is set, false otherwise.                                    *
*                                                                             *
* This function should only be called during resolution.                      *
* Call isPOD (or check FLAG_POD/FLAG_NOT_POD) after resolution.               *
*                                                                             *
************************************** | *************************************/

static bool isCompilerGenerated(FnSymbol* fn);

bool propagateNotPOD(Type* t) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(t)) {
    if        (at->symbol->hasFlag(FLAG_POD)     == true) {
      retval = false;

    } else if (at->symbol->hasFlag(FLAG_NOT_POD) == true) {
      retval =  true;

    } else {
      // Some special rules for special things.
      if (isSyncType(at)                        ||
          isSingleType(at)                      ||
          at->symbol->hasFlag(FLAG_DOMAIN)      || // may as well check these
          at->symbol->hasFlag(FLAG_ARRAY)       ||
          at->symbol->hasFlag(FLAG_ATOMIC_TYPE) ) {
        retval = true;

      } else if (isClass(at) == true) {
        // Most class types are POD (user classes, _ddata, c_ptr)
        // Also, there is no need to check the fields of a class type
        // since a variable of that type is a pointer to the instance.
        // So, don't enumerate sub-fields or check for autoCopy etc.
        retval = false;

      } else if (typeNeedsCopyInitDeinit(at) == false) {
        // some types aren't subject to copy init / deinit
        retval = false;

      } else {
        // If any field in a record/tuple is not POD, the aggregate is not POD.
        for_fields(field, at) {
          retval = retval | propagateNotPOD(field->typeInfo());
        }

        if (retval == false) {
          // Make sure we have resolved auto copy/auto destroy.
          // Except not for runtime types, because that causes
          // some sort of fatal resolution error. This is a workaround.

          if (at->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE) == false) {
            resolveAutoCopyEtc(at);
          }

          if (at->symbol->hasFlag(FLAG_IGNORE_NOINIT)      == true  ||
              isCompilerGenerated(autoCopyMap[at])         == false ||
              isCompilerGenerated(autoDestroyMap.get(at))  == false ||
              isCompilerGenerated(at->getDestructor())     == false) {
            retval = true;
          }
        }

        // Since hasUserAssign tries to resolve =, we only
        // check it if we think we have a POD type.
        if (retval == false && hasUserAssign(at) == true) {
          retval = true;
        }
      }

      at->symbol->addFlag((retval == true) ? FLAG_NOT_POD : FLAG_POD);
    }
  }

  return retval;
}

static bool isCompilerGenerated(FnSymbol* fn) {
  return fn != NULL && fn->hasFlag(FLAG_COMPILER_GENERATED) == true;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveOther() {
  //
  // When compiling with --minimal-modules, gPrintModuleInitFn is not
  // defined.
  //
  if (gPrintModuleInitFn) {
    // Resolve the function that will print module init order
    resolveFunction(gPrintModuleInitFn);
  }

  std::vector<FnSymbol*> fns = getWellKnownFunctions();

  for_vector(FnSymbol, fn, fns) {
    // resolve the argument types
    resolveSignature(fn);
    fn->tagIfGeneric();
    // resolve the return type if specified
    if (fn->retExprType)
      resolveSpecifiedReturnType(fn);
    // resolve the function body if it is not generic
    if (!fn->isGeneric())
      resolveFunction(fn);
  }
}


// Copied from iterator.cpp.
static bool isIteratorOrForwarder(FnSymbol* it) {
  // The test 'it->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD)'
  // gives a false negative when it->retType is "unknown"
  // or a false positive for chpl__autoCopy(_iteratorRecord).
  // FLAG_FN_RETURNS_ITERATOR is not a great test either because
  // iteratorIndex() has it whereas it usually doesn't.

  return it->hasFlag(FLAG_ITERATOR_FN) ||
         it->hasFlag(FLAG_FN_RETURNS_ITERATOR);
}

// Handle the case where 'call' is a statement-level iterator invocation.
// Adds a forall over 'tmp'.
static void handleStatementLevelIteratorCall(DefExpr* def, VarSymbol* tmp)
{
  BlockStmt* block = ForallStmt::build(new UnresolvedSymExpr("SLIidx"),
                                       new SymExpr(tmp), NULL,
                                       new BlockStmt(), false, true);

  // We do not actually need the BlockStmt.
  INT_ASSERT(block->length() == 1);
  ForallStmt* fs = toForallStmt(block->body.head->remove());

  def->next->insertAfter(fs);
  resolveForallHeader(fs, toSymExpr(fs->firstIteratedExpr()));
  resolveBlockStmt(fs->loopBody());
}


static AggregateType* buildRuntimeTypeInfo(FnSymbol* fn) {
  SET_LINENO(fn);
  AggregateType* ct = new AggregateType(AGGREGATE_RECORD);
  TypeSymbol* ts = new TypeSymbol(astr("_RuntimeTypeInfo"), ct);
  for_formals(formal, fn) {
    VarSymbol* field = new VarSymbol(formal->name, formal->type);
    ct->fields.insertAtTail(new DefExpr(field));

    if (formal->hasFlag(FLAG_TYPE_VARIABLE)) {
      field->addFlag(FLAG_TYPE_VARIABLE);
      Symbol* sub = formal->type->symbol;
      ct->substitutions.put(field, sub);
    }
    if (formal->hasFlag(FLAG_INSTANTIATED_PARAM)) {
      field->addFlag(FLAG_PARAM);
      Symbol* sub = paramMap.get(formal);
      ct->substitutions.put(field, sub);
    }
  }
  theProgram->block->insertAtTail(new DefExpr(ts));
  ct->symbol->addFlag(FLAG_RUNTIME_TYPE_VALUE);
  makeRefType(ts->type); // make sure the new type has a ref type.
  propagateNotPOD(ts->type); // mark the runtime type as POD/NOT_POD
  return ct;
}


static void insertReturnTemps() {
  //
  // Insert return temps for functions that return values if no
  // variable captures the result. If the value is a sync/single var or a
  // reference to a sync/single var, pass it through a chpl_statementLevelSymbol
  // function to get the semantics of reading a sync/single var. If the value
  // is an iterator, iterate over it in a ForallStmt for side effects.
  // Note that we do not do this for --minimal-modules compilation
  // because we do not support sync/singles for minimal modules.
  //
  for_alive_in_expanding_Vec(CallExpr, call, gCallExprs) {
      if (call->list == NULL && isForallRecIterHelper(call))
        continue;
      if (FnSymbol* fn = call->resolvedOrVirtualFunction()) {
        if (fn->retType != dtVoid && fn->retTag != RET_TYPE) {
          ContextCallExpr* contextCall = toContextCallExpr(call->parentExpr);
          Expr*            contextCallOrCall; // insert before, remove it

          if (contextCall) {
            contextCallOrCall = contextCall;
            // Only consider the designated call; the other
            // call need not be considered by this transformation.
            if (call != getDesignatedCall(contextCall))
              continue;
          } else {
            contextCallOrCall = call;
          }

          Expr* parent = contextCallOrCall->parentExpr;

          if (isForallIterExpr(contextCallOrCall))
            continue; // not really a top-level expression

          if (!isCallExpr(parent) && !isDefExpr(parent)) { // no use
            SET_LINENO(call);
            VarSymbol* tmp = newTemp("_return_tmp_", fn->retType);
            DefExpr*   def = new DefExpr(tmp);

            if (typeNeedsCopyInitDeinit(fn->retType) == true)
              tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);

            contextCallOrCall->insertBefore(def);
            def->insertAfter(new CallExpr(PRIM_MOVE,
                                          tmp,
                                          contextCallOrCall->remove()));

            if (fMinimalModules == false) {
              if (isIteratorOrForwarder(fn)) {
                handleStatementLevelIteratorCall(def, tmp);

              } else
              if ((fn->retType->getValType() &&
                   (isSyncType(fn->retType->getValType()) ||
                    isSingleType(fn->retType->getValType())))         ||

                  isSyncType(fn->retType)                             ||
                  isSingleType(fn->retType)                           )
              {
                CallExpr* sls = new CallExpr(
                    astr_chpl_statementLevelSymbol, tmp);

                def->next->insertAfter(sls);
                resolveCallAndCallee(sls);
              }
            }

            if (isTypeExpr(contextCallOrCall)) {
              tmp->addFlag(FLAG_MAYBE_TYPE);
              tmp->addFlag(FLAG_MAYBE_PARAM);
            }

            // Mark the variable as last-mention or end-of-block
            // (because this runs after fixPrimInitsAndAddCasts).
            markTempDeadLastMention(tmp);
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
  AggregateType* ct = toAggregateType(sym->type);
  INT_ASSERT(ct);
  for_fields(field, ct) {
    if (!field->hasFlag(FLAG_SUPER_CLASS)) {
      SET_LINENO(field);
      if (field->type->defaultValue) {
        Type* type = field->type;
        Expr* deflt = NULL;
        if (type->defaultValue->type == type) {
          deflt = new SymExpr(type->defaultValue);
        } else {
          VarSymbol* defaultTmp = newTemp("_init_class_def_", type);
          Expr* mv = new CallExpr(PRIM_MOVE, defaultTmp,
                                  new CallExpr(PRIM_CAST, type->symbol, deflt));
          stmt->insertBefore(new DefExpr(defaultTmp));
          stmt->insertBefore(mv);
          deflt = new SymExpr(defaultTmp);
        }
        stmt->insertBefore(new CallExpr(PRIM_SET_MEMBER, sym, field, deflt));
      } else if (isRecord(field->type)) {
        VarSymbol* tmp = newTemp("_init_class_tmp_", field->type);
        stmt->insertBefore(new DefExpr(tmp));
        initializeClass(stmt, tmp);
        stmt->insertBefore(new CallExpr(PRIM_SET_MEMBER, sym, field, tmp));
      }
    }
  }
}


static void handleRuntimeTypes()
{
  // populateRuntimeTypeMap is also called earlier in resolve().  That call
  // can insert variables that need autoCopies and inserting autoCopies can
  // insert things that need runtime type temps.  These need to be fixed up
  // by populateRuntimeTypeMap before buildRuntimeTypeInitFns is called to
  // update the type -> runtimeType mapping.  Without this, there is an
  // actual/formal type mismatch (with --verify) for the code:
  // record R { var A: [1..1][1..1] real; }
  populateRuntimeTypeMap();
  buildRuntimeTypeInitFns();
  replaceTypeFormalsWithRuntimeTypes();
  replaceReturnedTypesWithRuntimeTypes();
  replaceRuntimeTypeVariableTypes();
}


static void printUnusedFunctions() {
/* Defining the macro PRINT_UNUSED_FNS_TO_FILE will cause
   --print-unused-internal-functions to print the unused functions to a file
   named 'execFilename.unused'. Then paratest can be run to print all unused
   functions across the test suite into files to determine what functions
   are not used by any tests.

   The macro EXIT_AFTER_PRINTING_UNUSED_FNS can also be defined to cause the
   compiler to immediately exit after printing unused functions for faster
   checking.
*/
#ifdef PRINT_UNUSED_FNS_TO_FILE
  char fname[FILENAME_MAX+1];
  snprintf(fname, FILENAME_MAX, "%s.%s", executableFilename, "unused");
  FILE* outFile = fopen(fname, "w");
#else
  FILE* outFile = stdout;
#endif
  // map from generic functions to instantiated versions
  // a generic function is 'used' if it is instantiated.
  std::map<FnSymbol*, std::vector<FnSymbol*> > instantiations;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (FnSymbol* instantiatedFrom = fn->instantiatedFrom) {
      while (instantiatedFrom->instantiatedFrom != NULL) {
        instantiatedFrom = instantiatedFrom->instantiatedFrom;
      }
      instantiations[instantiatedFrom].push_back(fn);
    }
  }

  bool first = true;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_PRINT_MODULE_INIT_FN)) continue;
    if (fn->defPoint && fn->defPoint->parentSymbol) {
      if (fn->defPoint->parentSymbol == stringLiteralModule) continue;
      if (!fn->isResolved() || fn->retTag == RET_PARAM) {
        if (!fn->instantiatedFrom) {
          if (instantiations.count(fn) == 0 || instantiations[fn].size() == 0) {
            if (fPrintUnusedInternalFns ||
                fn->defPoint->getModule()->modTag == MOD_USER) {
              if (first) {
                first = false;
                fprintf(outFile, "The following functions are unused:\n");
              }
              fprintf(outFile, "  %s:%d: %s\n",
                      fn->defPoint->fname(), fn->defPoint->linenum(), fn->name);
            }
          }
        }
      }
    }
  }

#ifdef PRINT_UNUSED_FNS_TO_FILE
  fclose(outFile);
#ifdef EXIT_AFTER_PRINTING_UNUSED_FNS
  clean_exit(0);
#endif
#endif
}

//
// Print a representation of the call graph of the program.
// This needs to be done after function resolution so we can follow calls
// into the called function.  However, it needs to be done before
// removeUnusedFunctions so that we can consider multiple instantiations
// of a function to be the same by tracking them using the function they
// were instantiated from before they are removed as unused.
//
static void printCallGraph(FnSymbol* startPoint, int indent, std::set<FnSymbol*>* alreadyCalled) {

  std::vector<BaseAST*> asts;
  std::set<FnSymbol*> alreadySeenLocally;
  bool freeAlreadyCalledSet = false;
  const bool printLocalMultiples = false;

  if (!startPoint) {
    startPoint = findGenMainFn();
  }

  if (alreadyCalled == NULL) {
    alreadyCalled = new std::set<FnSymbol*>();
    freeAlreadyCalledSet = true;
  }

  collect_asts_postorder(startPoint, asts);
  if (startPoint->hasFlag(FLAG_COMPILER_GENERATED) ||
      startPoint->hasFlag(FLAG_MODULE_INIT)) {
    // Don't print chpl_gen_main and chpl__init_moduleName
    // Set the indent to -2 so recursing down to the next level
    // will set it back to 0.
    indent = -2;
  } else {
    fprintf(stdout, "%*s%s\n", indent, "", startPoint->name);
  }

  for_vector(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (FnSymbol* fn = call->resolvedFunction()) {
        if ((fn->getModule()->modTag == MOD_USER ||
             call->getModule()->modTag == MOD_USER) &&
            fn->getModule()->modTag != MOD_INTERNAL &&
            !fn->hasFlag(FLAG_COMPILER_GENERATED) &&
            !fn->hasFlag(FLAG_COMPILER_NESTED_FUNCTION)) {

          if (strncmp("chpl_", fn->name, 5) == 0 &&
              !fn->hasFlag(FLAG_MODULE_INIT)) {
            // skip any functions that are internal (start with "chpl_")
            // except for the init function for the module, which needs
            // to be traversed to find top-level calls in the module
            continue;
          }

          FnSymbol* instFn = fn;
          if (FnSymbol* gfn = fn->instantiatedFrom) {
            instFn = gfn;
          }
          if (printLocalMultiples || 0 == alreadySeenLocally.count(instFn)) {
            alreadySeenLocally.insert(instFn);
            if (0 == alreadyCalled->count(fn)) {
              alreadyCalled->insert(fn);
              printCallGraph(fn, indent+2, alreadyCalled);
              alreadyCalled->erase(fn);
            } else {
              fprintf(stdout, "%*s%s (Recursive)\n", indent+2, "", fn->name);
            }
          }
        }
      }
    }
  }
  if (freeAlreadyCalledSet) {
    delete alreadyCalled;
  }
}

//
// Return the compiler generated main function.
//
static FnSymbol* findGenMainFn() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->isResolved()) {
      if (!strcmp("chpl_gen_main", fn->name)) {
        return fn;
      }
    }
  }
  INT_FATAL("couldn't find compiler generated main function");
  return NULL;
}

void removeCopyFns(Type* t) {
  // If they exist, remove the autoCopy and autoDestroy functions
  // for type t, which is about to be removed itself.
  if (FnSymbol* autoDestroy = autoDestroyMap.get(t)) {
    autoDestroyMap.put(t, NULL);
    autoDestroy->defPoint->remove();
  }

  std::map<Type*,FnSymbol*>::iterator it = autoCopyMap.find(t);
  if (it != autoCopyMap.end()) {
    FnSymbol* autoCopy = it->second;
    autoCopyMap.erase(it);
    autoCopy->defPoint->remove();
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// Functions flagged with the "runtime type init fn" pragma
// (FLAG_RUNTIME_TYPE_INIT_FN during compilation) are designed to
// specify to the compiler how to create a new value of a given type
// from the arguments to the function.  These arguments effectively
// supply whatever static and/or runtime information is required to
// build such a value (and therefore effectively represent the
// "type").  Any non-static arguments are bundled into a runtime type
// (record) by the compiler and passed around to represent the type at
// execution time.
//
// The actual type specified is fully-resolved during function resolution.
//
// In practice, we currently use these to create
// runtime types for domains and arrays (via procedures named
// 'chpl__buildDomainRuntimeType' and 'chpl__buildArrayRuntimeType',
// respectively).
//
// Notice also that the original functions had been marked as type
// functions during parsing even though they were not written as such
// (see addPragmaFlags() in build.cpp for more info).  Now they are
// actually type functions.

void adjustRuntimeTypeInitFn(FnSymbol* fn) {
  // Look only at functions flagged as "runtime type init fn".
  INT_ASSERT(fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN));
  // Look only at resolved instances.
  INT_ASSERT(fn->isResolved());

  INT_ASSERT(fn->retType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE));
  SET_LINENO(fn);

  // Build a new runtime type for this function
  Type* runtimeType = buildRuntimeTypeInfo(fn);
  runtimeTypeMap.put(fn->retType, runtimeType);
}

Symbol* getPrimGetRuntimeTypeField_Field(CallExpr* call) {
  INT_ASSERT(call->numActuals()==2);

  SymExpr* rt = toSymExpr(call->get(1));
  SymExpr* f = toSymExpr(call->get(2));
  INT_ASSERT(rt && f);

  AggregateType* rtt = toAggregateType(rt->typeInfo());
  if (rtt->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE))
    rtt = toAggregateType(runtimeTypeMap.get(rtt));

  if (rtt == NULL)
    return NULL;

  INT_ASSERT(rtt->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE));

  VarSymbol* fieldName = toVarSymbol(f->symbol());
  Symbol* field = NULL;
  if (Immediate* imm = fieldName->immediate) {
    INT_ASSERT(imm->const_kind == CONST_KIND_STRING);
    field = rtt->getField(imm->v_string.c_str());
  } else {
    field = fieldName;
  }

  return field;
}

Type* getPrimGetRuntimeTypeFieldReturnType(CallExpr* call, bool& isType)
{
  Symbol* field = getPrimGetRuntimeTypeField_Field(call);
  if (field) {
    isType = field->hasFlag(FLAG_TYPE_VARIABLE);
    return field->type;
  } else {
    isType = false;
    return dtUnknown;
  }
}

//
// For each such flagged function:
//
//   - Replace the body of the original function with a new function
//     that returns the dynamic runtime type info
static void buildRuntimeTypeInitFns() {
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
    // Look only at functions flagged as "runtime type init fn".
    if (fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN)) {

      // Look only at resolved instances.
      if (! fn->isResolved())
        continue;

      INT_ASSERT(fn->retType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE));
      SET_LINENO(fn);

      Type* runtimeType = runtimeTypeMap.get(fn->retType);
      INT_ASSERT(runtimeType);
      buildRuntimeTypeInitFn(fn, runtimeType);
    }
  }
}

// Build a function to return the runtime type by modifying
// the original function.
static void buildRuntimeTypeInitFn(FnSymbol* fn, Type* runtimeType)
{
  // Change the return type of the original function.
  fn->retType = runtimeType;
  fn->getReturnSymbol()->type = runtimeType;

  // Build a new body for the original function.
  BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);
  VarSymbol* var = newTemp("_return_tmp_", fn->retType);
  block->insertAtTail(new DefExpr(var));

  // Bundle all non-static arguments into the runtime type record.
  // Remove static arguments from this specialized buildRuntimeType function.
  for_formals(formal, fn)
  {
    if (formal->hasFlag(FLAG_INSTANTIATED_PARAM))
      continue;

    Symbol* field = runtimeType->getField(formal->name);

    if (formal->hasFlag(FLAG_TYPE_VARIABLE) &&
        ! field->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE))
      continue;

    block->insertAtTail(new CallExpr(PRIM_SET_MEMBER, var, field, formal));
  }

  block->insertAtTail(new CallExpr(PRIM_RETURN, var));

  // Replace the body of the original chpl__buildRuntime...Type() function.
  fn->replaceBodyStmtsWithStmts(block);
}

static void replaceTypeFormalsWithRuntimeTypes()
{
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
      for_formals(formal, fn) {
        if (formal->hasFlag(FLAG_TYPE_VARIABLE) &&
            formal->getValType()->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          if (FnSymbol* fn = valueToRuntimeTypeMap.get(formal->getValType())) {
            Type* rt = (fn->retType->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) ?
                        fn->retType : runtimeTypeMap.get(fn->retType);
            INT_ASSERT(rt);
            formal->type =  rt;
            formal->removeFlag(FLAG_TYPE_VARIABLE);
          }
        }
      }
  }
}

static void replaceReturnedTypesWithRuntimeTypes()
{
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->retTag == RET_TYPE) {
        VarSymbol* ret = toVarSymbol(fn->getReturnSymbol());
        if (ret && ret->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          if (FnSymbol* rtfn = valueToRuntimeTypeMap.get(ret->type)) {
            Type* rt = NULL;
            if (rtfn->retType->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE))
              rt = rtfn->retType;
            else
              rt = runtimeTypeMap.get(rtfn->retType);
            INT_ASSERT(rt);
            ret->type = rt;
            fn->retType = ret->type;
            fn->retTag = RET_VALUE;
          }
        }
      }
  }
}

static void lowerRuntimeTypeInit(CallExpr* call,
                                 Symbol* var,
                                 AggregateType* at,
                                 bool noinit)
{
  INT_ASSERT(at->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE));

  if (var->hasFlag(FLAG_NO_INIT)) {
    call->convertToNoop();
    return;
  }

  if (noinit) {
    if (!at->symbol->hasFlag(FLAG_ARRAY)) {
      USR_FATAL(call, "noinit is only supported for arrays");
    } else if (fAllowNoinitArrayNotPod == false) {
      // noinit of an array
      Type* eltType = arrayElementType(at);
      bool notPOD = propagateNotPOD(eltType);
      if (notPOD) {
        USR_FATAL_CONT(call, "noinit is only supported for arrays of trivially copyable types");
      }
    }
  }

  SymExpr* typeSe = toSymExpr(call->get(2));
  INT_ASSERT(typeSe);
  Symbol* typeSym = typeSe->symbol();

  if (isTypeSymbol(typeSym)) {
    // This is a work-around for tuple _defaultOf when applied
    // to a tuple type with an element that is an array type.

    // This is resolved in some tests and not used, so
    // the error here is runtime, for now.

    Expr* stmt = call->getStmtExpr();

    const char* msg = NULL;

    bool tupleDefaultOf = false;
    if (FnSymbol* fn = call->getFunction())
      if (fn->name == astr_defaultOf)
        if (fn->retType->symbol->hasFlag(FLAG_TUPLE))
          tupleDefaultOf = true;

    if (tupleDefaultOf) {
      msg = "default initialization of tuple containing "
            "array or domain type not yet implemented";
    } else {
      msg = "default initialization of type containing "
            "array or domain type not yet implemented";
    }
    stmt->insertBefore(new CallExpr(PRIM_RT_ERROR, new_CStringSymbol(msg)));

    // Create a local type variable that will be uninitialized
    // so compilation can continue.
    VarSymbol* tmp = newTemp(typeSym->type);
    tmp->addFlag(FLAG_TYPE_VARIABLE);
    stmt->insertBefore(new DefExpr(tmp));

    typeSym = tmp;
  }

  // Get the runtime type
  AggregateType* runtimeType = toAggregateType(runtimeTypeMap.get(at));
  INT_ASSERT(runtimeType);

  // ('init' x foo), where typeof(foo) has flag "runtime type value"
  //
  // ==>
  //
  // (var _rtt_1)
  // ('move' _rtt_1 ('.v' foo "field1"))
  // (var _rtt_2)lowerRuntimeTypeInit
  // ('move' _rtt_2 ('.v' foo "field2"))
  // ('move' x chpl__convertRuntimeTypeToValue _rtt_1 _rtt_2 ... )
  SET_LINENO(call);
  CallExpr* runtimeTypeToValueCall =
    new CallExpr("chpl__convertRuntimeTypeToValue");
  for_fields(field, runtimeType) {
    Symbol* sub = runtimeType->substitutions.get(field);
    if (sub == NULL || sub->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
      VarSymbol* tmp = newTemp("_runtime_type_tmp_", field->type);
      call->getStmtExpr()->insertBefore(new DefExpr(tmp));
      call->getStmtExpr()->insertBefore(
          new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_RUNTIME_TYPE_FIELD,
                                                    typeSym,
                                                    field)));
      if (field->hasFlag(FLAG_TYPE_VARIABLE))
        tmp->addFlag(FLAG_TYPE_VARIABLE);
      runtimeTypeToValueCall->insertAtTail(tmp);
    } else {
      runtimeTypeToValueCall->insertAtTail(sub);
    }
  }

  // Add the argument indicating if this is a noinit
  Symbol* isNoInit = noinit ? gTrue : gFalse;
  runtimeTypeToValueCall->insertAtTail(isNoInit);

  // Add the argument indicating if this is defined as const
  Symbol* definedConst = var->hasFlag(FLAG_CONST) ? gTrue : gFalse;
  runtimeTypeToValueCall->insertAtTail(definedConst);

  call->replace(new CallExpr(PRIM_MOVE, var, runtimeTypeToValueCall));

  resolveCallAndCallee(runtimeTypeToValueCall);
}

/************************************* | **************************************
*                                                                             *
* Handles PRIM_DEFAULT_INIT_VAR
*                                                                             *
* Returns NULL if no substitution was made.                                   *
* Otherwise remove the primitive and return the new expression.               *
*                                                                             *
* ('init'    foo) --> A default value or the result of an initializer call.   *
* ('no_init' foo) --> Ditto, only in some cases a simpler default value.      *
*                                                                             *
************************************** | *************************************/

static Symbol* resolvePrimInitGetField(CallExpr* call);

static void resolvePrimInit(CallExpr* call, Symbol* val, Type* type);

static void lowerPrimInit(CallExpr* call, Symbol* val, Type* type,
                          Expr* preventingSplitInit);

static void lowerPrimInitNonGenericRecordVar(CallExpr* call,
                                               Symbol* val,
                                               AggregateType* at);

static void lowerPrimInitGenericRecordVar(CallExpr* call,
                                            Symbol* val,
                                            AggregateType* at);


static bool    primInitIsUnacceptableGeneric(CallExpr* call, Type* type);

static void    primInitHaltForUnacceptableGeneric(CallExpr* call, Type* type, Symbol* val);

static void errorInvalidParamInit(CallExpr* call, Symbol* val, Type* type);

static bool isInDefaultActualFunction(Expr* ref) {
  if (FnSymbol* parent = toFnSymbol(ref->parentSymbol))
    if (parent->hasFlag(FLAG_DEFAULT_ACTUAL_FUNCTION))
      return true;
  return false;
}

// Returns NULL if 'val' is not a temp, is a temp with FLAG_USER_VARIABLE_NAME,
// or does not feed into a PRIM_SET_MEMBER.
static Symbol* tempFeedsIntoSetMember(CallExpr* call, SymExpr* valSe,
                                      Symbol* val) {
  if (val->hasFlag(FLAG_TEMP) && ! val->hasFlag(FLAG_USER_VARIABLE_NAME)) {
    // See if it is the temp used to set a field.
    for_SymbolSymExprs(se, val) {
      if (se != valSe)
        if (CallExpr* parent = toCallExpr(se->parentExpr))
          if (parent->isPrimitive(PRIM_SET_MEMBER))
            if (SymExpr* fieldSE = toSymExpr(parent->get(2)))
              return fieldSE->symbol();
    }
  }

  return NULL;
}

// For a PRIM_DEFAULT_INIT_VAR or a PRIM_INIT_VAR_SPLIT_DECL,
// establish the type of the initialized variable so that resolution
// can continue.
//
// Additionally lower PRIM_DEFAULT_INIT for params since these
// need to be established for further resolution.
//
// lowerPrimInit will be called later to generate the code to properly
// initialize this variable.
static void resolvePrimInit(CallExpr* call) {
  Expr* valExpr = NULL;
  Expr* typeExpr = NULL;

  INT_ASSERT(call->isPrimitive(PRIM_DEFAULT_INIT_VAR) ||
             call->isPrimitive(PRIM_NOINIT_INIT_VAR) ||
             call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL));

  valExpr = call->get(1);

  if (call->numActuals() >= 2)
    typeExpr = call->get(2);

  if (call->isPrimitive(PRIM_DEFAULT_INIT_VAR) ||
      call->isPrimitive(PRIM_NOINIT_INIT_VAR))
    INT_ASSERT(valExpr && typeExpr);

  if (call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL) && typeExpr == NULL)
    return; // do nothing because we cannot establish the type

  SymExpr* valSe = toSymExpr(valExpr);
  INT_ASSERT(valSe);
  Symbol* val = valSe->symbol();

  if (SymExpr* se = toSymExpr(typeExpr)) {
    if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      resolvePrimInit(call, val, resolveTypeAlias(se));
    } else {
      USR_FATAL(call, "invalid type specification");
    }
  } else if (CallExpr* ce = toCallExpr(typeExpr)) {
    if (Symbol* field = resolvePrimInitGetField(ce)) {
      if (field->hasFlag(FLAG_TYPE_VARIABLE) == true) {
        resolvePrimInit(call, val, field->typeInfo());
      } else {
        USR_FATAL(call, "invalid type specification");
      }
    }
  } else {
    INT_FATAL(call, "Unsupported primInit");
  }
}

static void resolvePrimInit(CallExpr* call, Symbol* val, Type* type) {

  if (call->id == breakOnResolveID) gdbShouldBreakHere();

  AggregateType* at = toAggregateType(type);
  val->type = type;

  // Shouldn't be called with ref types
  INT_ASSERT(at == NULL || at->getValType() == at);

  SET_LINENO(call);

  // When --ignore-errors-for-pass is used, earlier errors, such as
  // compilerError errors, are marked as pending until the end of the
  // resolution pass, which can cause secondary errors about void
  // variables here. If the function resolution pass has any pending
  // fatal errors, we suppress any errors about void variables here,
  // which might be secondary to the original fatal error.
  if(type == dtVoid && !fatalErrorsEncountered())
    USR_FATAL(call,
              "Variable '%s' cannot be declared 'void'."
              " Consider using 'nothing' instead.",
              val->name);

  if (type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) == true) {
    if (call->isPrimitive(PRIM_DEFAULT_INIT_VAR) ||
        call->isPrimitive(PRIM_NOINIT_INIT_VAR)) {
      // handled in lowerPrimInit
      errorInvalidParamInit(call, val, at);
    }

  // Shouldn't be default-initializing iterator records here
  } else if (type->symbol->hasFlag(FLAG_ITERATOR_RECORD)  == true) {
    INT_ASSERT(false);
  // Generate a more specific USR_FATAL if resolution would fail
  } else if (primInitIsUnacceptableGeneric(call, type)    == true) {
    // ignore this case for now

  // These types default to nil
  } else if (isClassLikeOrPtr(type) || type == dtNil) {
    // Nothing to do here besides set the type
    errorInvalidParamInit(call, val, at);

    // These will be handled in lowerPrimInit.
    if (call->numActuals() >= 2)
      call->get(2)->replace(new SymExpr(type->symbol));

  // any type with a defaultValue is easy enough
  // (expect this to handle numeric types and classes)
  } else if (type->defaultValue != NULL) {
    // note: it is important that param cases are handled here rather
    // than in lowerPrimInit.

    // note: error for bad param initialization checked for in resolving move
    if (val->hasFlag(FLAG_MAYBE_PARAM) || val->isParameter()) {
      if (!call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL)) {
        Expr* defaultExpr = NULL;
        SymExpr* typeSe = NULL;
        if (type->defaultValue->type == type) {
          defaultExpr = new SymExpr(type->defaultValue);
        } else {
          typeSe = new SymExpr(type->symbol);
          defaultExpr = new CallExpr(PRIM_CAST, type->symbol, type->defaultValue);
        }

        CallExpr* moveDefault = new CallExpr(PRIM_MOVE, val, defaultExpr);
        call->insertBefore(moveDefault);
        if (typeSe) resolveExprTypeConstructor(typeSe);
        resolveExpr(moveDefault);
        call->convertToNoop();
      } else {
        call->convertToNoop(); // initialize it in PRIM_INIT_VAR_SPLIT_INIT
                               // (important for params)
      }
    } else {
      // non-param integers e.g. will be handled in lowerPrimInit.
      if (call->numActuals() >= 2)
        call->get(2)->replace(new SymExpr(type->symbol));
    }

  // non-generic records with initializers
  // extern types
  // generic records with initializers
  // other types (sync, single, ..)
  } else {
    // These will be handled in lowerPrimInit.
    errorInvalidParamInit(call, val, at);

    if ((type == dtStringC || type == dtString || type == dtBytes) &&
        (val->hasFlag(FLAG_MAYBE_PARAM) || val->isParameter())) {
      // Initialize a param string
      if (!call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL)) {
        VarSymbol* empty = nullptr;
        if (type == dtStringC) {
          empty = new_CStringSymbol("");
        } else if (type == dtString) {
          empty = new_StringSymbol("");
        } else if (type == dtBytes) {
          empty = new_BytesSymbol("");
        } else {
          INT_FATAL("case not handled");
        }
        CallExpr* moveDefault = new CallExpr(PRIM_MOVE, val, empty);
        call->insertBefore(moveDefault);
        resolveExpr(moveDefault);
        call->convertToNoop();

      } else {
        call->convertToNoop(); // initialize it in PRIM_INIT_VAR_SPLIT_INIT
      }
    }
  }
}

void lowerPrimInit(CallExpr* call, Expr* preventingSplitInit) {
  Expr* valExpr = NULL;
  Expr* typeExpr = NULL;

  valExpr = call->get(1);
  SymExpr* valSe = toSymExpr(valExpr);
  INT_ASSERT(valSe);
  Symbol* val = valSe->symbol();
  INT_ASSERT(val);

  if (call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL)) {
    val->addFlag(FLAG_SPLIT_INITED);
    // PRIM_INIT_SPLIT_DECL does nothing else at this point
    call->convertToNoop();
    return;
  }

  typeExpr = call->get(2);
  INT_ASSERT(valExpr && typeExpr);

  if (SymExpr* se = toSymExpr(typeExpr)) {
    if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      lowerPrimInit(call, val, resolveTypeAlias(se), preventingSplitInit);

    } else {
      USR_FATAL(call, "invalid type specification");
    }

  } else {
    INT_FATAL(call, "Unsupported primInit");
  }

  //
  // Check for a default-initialized array with a non-nilable element type
  // (a) in a variable declaration, or (b) in a user-defined initializer
  // when the field is not initialized explicitly ex. (a) x1 and (b) C2.A2 in
  //   test/classes/nilability/array-with-nonnilable-elttype.chpl
  //
  if (call->isPrimitive(PRIM_DEFAULT_INIT_VAR) &&
      val->type->symbol->hasFlag(FLAG_ARRAY)   &&
      ! val->hasFlag(FLAG_UNSAFE)              &&
      ! isInDefaultActualFunction(call)        ) {
    const char* name = NULL;
    if (Symbol* field = tempFeedsIntoSetMember(call, valSe, val)) {
      if (! field->hasFlag(FLAG_UNSAFE))
        name = astr("field ", field->name);
    } else {
      name = val->name;
    }

    INT_ASSERT(val->type != dtUnknown && val->type != dtAny);
    if (name != NULL) {
      Type* eltType = finalArrayElementType(toAggregateType(val->type));
      if (! okForDefaultInitializedArray(eltType))
        USR_FATAL_CONT(call, "cannot default-initialize the array %s"
                       " because it has a non-nilable element type '%s'",
                       name, toString(eltType));
    }
  }
}

static Symbol* resolvePrimInitGetField(CallExpr* call) {
  Symbol* retval = NULL;

  if (call->isPrimitive(PRIM_GET_MEMBER_VALUE) == true) {
    AggregateType* ct  = toAggregateType(call->get(1)->getValType());
    SymExpr*       se  = toSymExpr(call->get(2));
    VarSymbol*     var = toVarSymbol(se->symbol());

    if (Immediate* imm = var->immediate) {
      if (imm->const_kind == CONST_KIND_STRING) {
        retval = ct->getField(var->immediate->v_string.c_str());

      } else if (imm->const_kind == NUM_KIND_INT) {
        int64_t i = imm->int_value();

        if (ct->symbol->hasFlag(FLAG_ITERATOR_CLASS) == true) {
          retval = getTheIteratorFn(ct)->getFormal(i);

        } else {
          retval = ct->getField(i);
        }
      }

    } else {
      INT_FATAL(call, "Bad actual to \".v\"");
    }

  } else {
    INT_FATAL(call, "PrimInit only support \".v\"");
  }

  return retval;
}


// Does 'val' feed into a tuple? Ex.:
//   default init var( elt_x1 type owned Foo )
//   .=( tup "x0" elt_x1 )
static bool isTupleComponent(Symbol* val, CallExpr* call) {
  CallExpr* otherUse = NULL;
  for_SymbolSymExprs(se, val)
    if (se->parentExpr != call)
      if (CallExpr* parentCall = toCallExpr(se->parentExpr))
        if (parentCall->isPrimitive(PRIM_SET_MEMBER))
          {
            if (otherUse == NULL) otherUse = parentCall;
            else return false; // too many uses
          }

  if (otherUse != NULL)
    if (SymExpr* defSE = toSymExpr(otherUse->get(1)))
      // Does 'otherUse' define a tuple?
      if (defSE->symbol()->type->symbol->hasFlag(FLAG_TUPLE))
        return true;

  return false;
}

// Create an error when default initializing a non-nilable class type.
static void errorIfNonNilableType(CallExpr* call, Symbol* val,
                                  Type* typeToCheck, Type* type,
                                  Expr* preventingSplitInit)
{
  if (isDefaultInitializable(typeToCheck) || useLegacyNilability(call))
    return;

  // Work around current problems in array / assoc array types
  bool unsafe = call->getFunction()->hasFlag(FLAG_UNSAFE) ||
                call->getModule()->hasFlag(FLAG_UNSAFE)   ||
                val->hasFlag(FLAG_UNSAFE);
  if (unsafe)
    return;

  // If the variable is explicitly not initialized, don't worry
  if (val->hasFlag(FLAG_NO_INIT))
    return;

  // Skip this error for a param - it will get "not of a supported param type"
  if (val->hasFlag(FLAG_PARAM))
    return;

  const char* descr = val->name;
  if (VarSymbol* v = toVarSymbol(val))
    descr = toString(v, true);

  CallExpr* uCall = call;
  if (isTupleComponent(val, call)) {
    descr = astr("a tuple component of the type ", toString(type));
    uCall = userCall(call);
  }

  USR_FATAL_CONT(uCall, "cannot default-initialize %s", descr);
  if (preventingSplitInit != NULL && !val->hasFlag(FLAG_TEMP))
    USR_FATAL_CONT(preventingSplitInit, "use here prevents split-init");

  if (isNonNilableClassType(typeToCheck)) {
    USR_PRINT("non-nilable class type '%s' "
              "does not support default initialization",
              toString(typeToCheck));

    AggregateType* at = toAggregateType(canonicalDecoratedClassType(type));
    ClassTypeDecoratorEnum d = classTypeDecorator(type);
    Type* suggestedType = at->getDecoratedClass(addNilableToDecorator(d));
    USR_PRINT("Consider using the type %s instead", toString(suggestedType));
  }
}

static void errorInvalidParamInit(CallExpr* call, Symbol* val, Type* type) {
  if (val->hasFlag(FLAG_PARAM) &&
      val->hasFlag(FLAG_TEMP) == false &&
      isLegalParamType(type) == false) {
    USR_FATAL_CONT(call, "'%s' is not of a supported param type", val->name);
  }
}

static void lowerPrimInit(CallExpr* call, Symbol* val, Type* type,
                          Expr* preventingSplitInit) {

  if (call->id == breakOnResolveID) gdbShouldBreakHere();

  AggregateType* at     = toAggregateType(type);

  val->type = type;

  // Shouldn't be called with ref types
  INT_ASSERT(at == NULL || at->getValType() == at);

  SET_LINENO(call);

  if (call->isPrimitive(PRIM_NOINIT_INIT_VAR) &&
      type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) == false) {
    USR_FATAL_CONT(call, "noinit is only supported for arrays");
  }

  if (type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) == true) {
    if (call->isPrimitive(PRIM_DEFAULT_INIT_VAR) ||
        call->isPrimitive(PRIM_NOINIT_INIT_VAR)) {
      errorInvalidParamInit(call, val, at);
      lowerRuntimeTypeInit(call, val, at,
                           call->isPrimitive(PRIM_NOINIT_INIT_VAR));
    }

  // Shouldn't be default-initializing iterator records here
  } else if (type->symbol->hasFlag(FLAG_ITERATOR_RECORD)  == true) {

    INT_ASSERT(false);

  // Generate a more specific USR_FATAL if resolution would fail
  } else if (primInitIsUnacceptableGeneric(call, type)    == true) {
    primInitHaltForUnacceptableGeneric(call, type, val);

  // These types default to nil
  } else if (isClassLikeOrPtr(type) || type == dtNil) {

    if (!call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL)) {
      Expr* nilExpr = NULL;
      SymExpr* typeSe = NULL;
      if (gNil->type == type) {
        nilExpr = new SymExpr(gNil);
      } else {
        typeSe = new SymExpr(type->symbol);
        nilExpr = new CallExpr(PRIM_CAST, typeSe, gNil);
      }

      CallExpr* moveDefault = new CallExpr(PRIM_MOVE, val, nilExpr);
      call->insertBefore(moveDefault);
      if (typeSe) resolveExprTypeConstructor(typeSe);
      resolveExpr(moveDefault);
      call->convertToNoop();

      errorIfNonNilableType(call, val, type, type, preventingSplitInit);
    } else {
      call->convertToNoop(); // initialize it in PRIM_INIT_VAR_SPLIT_INIT
    }

  // any type with a defaultValue is easy enough
  // (expect this to handle numeric types and classes)
  } else if (type->defaultValue != NULL) {
    // note: error for bad param initialization checked for in resolving move

    if (!call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL)) {
      Expr* defaultExpr = NULL;
      SymExpr* typeSe = NULL;
      if (type->defaultValue->type == type) {
        defaultExpr = new SymExpr(type->defaultValue);
      } else {
        typeSe = new SymExpr(type->symbol);
        defaultExpr = new CallExpr(PRIM_CAST, type->symbol, type->defaultValue);
      }

      CallExpr* moveDefault = new CallExpr(PRIM_MOVE, val, defaultExpr);
      call->insertBefore(moveDefault);
      if (typeSe) resolveExprTypeConstructor(typeSe);
      resolveExpr(moveDefault);
      call->convertToNoop();
    } else {
      call->convertToNoop(); // initialize it in PRIM_INIT_VAR_SPLIT_INIT
                             // (important for params)
    }

  // non-generic records with initializers
  } else if (at                                           != NULL &&
             at->instantiatedFrom                         == NULL &&
             isNonGenericRecordWithInitializers(at)       == true) {

    errorInvalidParamInit(call, val, at);
    if (!val->hasFlag(FLAG_NO_INIT) &&
        !call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL))
      lowerPrimInitNonGenericRecordVar(call, val, at);
    else
      call->convertToNoop(); // let the memory be uninitialized

  // extern types (but not memory_order)
  } else if (type->symbol->hasFlag(FLAG_EXTERN) &&
             !type->symbol->hasFlag(FLAG_C_MEMORY_ORDER_TYPE)) {

    errorInvalidParamInit(call, val, at);

    if (!val->hasFlag(FLAG_NO_INIT) &&
        !call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL)) {
      // Zero-initialize the memory
      call->insertBefore(new CallExpr(PRIM_ZERO_VARIABLE, val));
      call->convertToNoop();
    } else {
      // Just let the memory be uninitialized
      call->convertToNoop();
    }

  // generic records with initializers
  } else if (at != NULL && at->symbol->hasFlag(FLAG_TUPLE) == false &&
            (at->isRecord() || at->isUnion())) {

    errorInvalidParamInit(call, val, at);
    if (at->symbol->hasFlag(FLAG_MANAGED_POINTER))
      errorIfNonNilableType(call, val, getManagedPtrBorrowType(at), at,
                            preventingSplitInit);

    if (!val->hasFlag(FLAG_NO_INIT) &&
        !call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL))
      lowerPrimInitGenericRecordVar(call, val, at);
    else
      call->convertToNoop(); // let the memory be uninitialized

  // tuples
  } else if (at != NULL && at->symbol->hasFlag(FLAG_TUPLE)) {
    errorInvalidParamInit(call, val, at);

    // Handle tuple variables marked "no init". Convert to NOP and leave.
    if (val->hasFlag(FLAG_NO_INIT) &&
        call->isPrimitive(PRIM_DEFAULT_INIT_VAR)) {
      call->convertToNoop();
      return;
    }

    // Zero index and skip the first tuple field. TODO: Future-proof this?
    int idx = -1;
    bool hasErrored = false;

    // Emit errors for any non-default-initializable tuple fields.
    for_fields(field, at) {
      if (!isDefaultInitializable(field->type)) {

        if (!hasErrored) {
          hasErrored = true;
          USR_FATAL_CONT(call, "cannot default initialize the tuple %s",
                         val->name);
        }

        USR_PRINT("element %d of type %s has no default value",
                  idx, toString(field->type));

        if (isNonNilableClassType(field->type)) {

          Type* cdc = canonicalDecoratedClassType(field->type);
          AggregateType* atc = toAggregateType(cdc);
          ClassTypeDecoratorEnum d = classTypeDecorator(field->type);
          Type* rec = atc->getDecoratedClass(addNilableToDecorator(d));

          USR_PRINT("because it is a non-nilable class - consider the "
                    "type %s instead", toString(rec));
        }
      }

      idx++;
    }

    //
    // Only insert a call to `_defaultOf` here if our tuple can be default
    // initialized. This way avoid emitting confusing errors from within
    // the `_defaultOf` and give other code a chance to emit errors as well.
    //
    if (!hasErrored) {
      CallExpr* defaultCall = new CallExpr("_defaultOf", type->symbol);
      CallExpr* move = new CallExpr(PRIM_MOVE, val, defaultCall);

      call->insertBefore(move);
      call->convertToNoop();

      resolveCallAndCallee(defaultCall);
      resolveExpr(move);

    //
    // Go ahead and convert the call to a NOP to avoid getting errors during
    // post-resolution checks (these run even if errors have been emitted).
    //
    } else {
      call->convertToNoop();
    }

  // other types (sync, single, ...)
  } else {
    errorInvalidParamInit(call, val, at);

    // enum types should have a defaultValue
    INT_ASSERT(!isEnumType(type));

    CallExpr* defaultCall = new CallExpr("_defaultOf", type->symbol);
    CallExpr* move = new CallExpr(PRIM_MOVE, val, defaultCall);

    call->insertBefore(move);
    call->convertToNoop();

    resolveCallAndCallee(defaultCall);
    resolveExpr(move);
  }
}

static void lowerPrimInitNonGenericRecordVar(CallExpr* call,
                                               Symbol* val,
                                               AggregateType* at) {
  // This code not intended to handle _array etc (but these are generic, right?)
  INT_ASSERT(isRecordWrappedType(at->getValType()) == false);

  CallExpr* callInit = new CallExpr("init", gMethodToken, val);

  // This juggling is required by use of
  // for_exprs_postorder() in resolveBlockStmt
  call->insertBefore(callInit);
  call->convertToNoop();

  val->type = at;

  resolveCallAndCallee(callInit);

  if (isRecord(at) && at->hasPostInitializer()) {
    CallExpr* postinit = new CallExpr("postinit", gMethodToken, val);
    call->insertBefore(postinit);
    resolveCallAndCallee(postinit);
  }
}

// Given the type expression of the PRIM_INIT_VAR call, search for a type
// construction call and try to grab the actual corresponding to a
// substitution. This is needed if the actual in the type construction call
// is a runtime type. This will not work in all cases, in particular if
// we have to chase a type more than once to find its constructor.
// See: test/types/records/bugs/RuntimeTypeEscapes.chpl
static Expr*
chaseTypeConstructorForActual(CallExpr* init, const char* subName,
                              int subIdx,
                              AggregateType* at) {
  if (!init || !init->isPrimitive(PRIM_DEFAULT_INIT_VAR)) return nullptr;

  auto seTypeVar = toSymExpr(init->get(2));
  auto sym = seTypeVar->symbol();

  INT_ASSERT(sym->hasFlag(FLAG_TYPE_VARIABLE));

  // This function is not currently capable of chasing interprocedurally,
  // so if the init type is a formal, just bail out now.
  if (!isVarSymbol(sym)) return nullptr;

  // Given (PRIM_INIT_VAR obj temp) where 'temp' is a type variable, look
  // for (PRIM_MOVE temp call), where 'call' is a type construction call,
  // e.g. for the type 'foo(domain(3))'. Normally type construction calls
  // are replaced with a SymExpr referring to the TypeSymbol, however they
  // will not be if the call contains one or more runtime types.
  CallExpr* move = nullptr;

  // Check if the other use of 'temp' is a move.
  int numUses = 0;
  for_SymbolSymExprs(se, sym) {

    // Be conservative and limit to two uses.
    if (++numUses > 2) break;
    if (!se->inTree()) continue;

    if (auto innerCall = toCallExpr(se->parentExpr)) {
      if (innerCall->isPrimitive(PRIM_MOVE)) {
        move = innerCall;
        break;
      }
    }
  }

  // Didn't find a move, so return early.
  if (!move) return nullptr;

  // The type variable from the PRIM_INIT_VAR should be on the LHS.
  auto seLhs = toSymExpr(move->get(1));
  INT_ASSERT(seLhs);
  INT_ASSERT(seLhs->symbol() == sym);

  CallExpr* typeConstructorCall = nullptr;

  // Check if the RHS of 'move' is a type construction call. If so, see
  // if the base expression of that call refers to 'at'.
  if (auto innerCall = toCallExpr(move->get(2)))
    if (auto seBase = toSymExpr(innerCall->baseExpr))
      if (seBase->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
        if (auto ts = toTypeSymbol(seBase->symbol()))
          if (ts->type == at) typeConstructorCall = innerCall;

  // No luck, so return early.
  if (!typeConstructorCall) return nullptr;

  bool isAnyActualNamed = false;
  Expr* ret = nullptr;
  int idx = 1;

  // Look for a matching actual using the substitution position.
  for_actuals(actual, typeConstructorCall) {
    if (isNamedExpr(actual)) {
      isAnyActualNamed = true;
      break;
    } else if (subIdx == idx++) {
      ret = actual;
    }
  }

  // TODO: Handle named actuals in type construction calls.
  if (isAnyActualNamed) return nullptr;

  return ret;
}

// call is the context or PRIM_DEFAULT_INIT_VAR - in some cases
// code will be added before this.
static CallExpr* createGenericRecordVarDefaultInitCall(Symbol* val,
                                                       AggregateType* at,
                                                       Expr* call) {
  AggregateType* root = at->getRootInstantiation();

  val->type = root;

  CallExpr* initCall = new CallExpr("init", gMethodToken,
                                    new NamedExpr("this",
                                                  new SymExpr(val)));

  int idx = 1;
  for (auto elem: sortedSymbolMapElts(at->substitutions)) {
    const char* keyName = elem.key->name;
    Symbol*     value   = elem.value;

    Symbol* field = root->getField(keyName);
    bool hasDefault = false;
    bool isGenericField = root->fieldIsGeneric(field, hasDefault);

    Expr* appendExpr = NULL;
    if (field->isParameter()) {
      appendExpr = new SymExpr(value);
    } else if (field->hasFlag(FLAG_TYPE_VARIABLE)) {

      // For non-runtime-types, just use the type.
      if (!value->getValType()->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
        appendExpr = new SymExpr(value);

      // Else, try to search for the type constructor used to create the
      // type. If we find it, try to grab the runtime type used in the
      // type constructor and pass that to the initializer.
      } else {

        auto init = toCallExpr(call);
        auto chase = chaseTypeConstructorForActual(init, keyName, idx, at);
        bool recoveredRuntimeType = false;

        // If we found a type constructor with an actual matching this
        // substitution, then reuse the actual if it is a runtime type.
        if (chase) {
          if (auto chaseSymExpr = toSymExpr(chase)) {
            if (auto ts = chaseSymExpr->symbol()->type->symbol) {
              if (ts->hasFlag(FLAG_HAS_RUNTIME_TYPE) &&
                  ts->type == value->type) {
                auto tmp = newTemp("runtime_temp");
                tmp->addFlag(FLAG_MAYBE_TYPE);
                auto copy = chase->copy();
                auto move = new CallExpr(PRIM_MOVE, tmp, copy);
                call->insertBefore(new DefExpr(tmp));
                call->insertBefore(move);
                resolveExpr(move);
                appendExpr = new SymExpr(tmp);
                recoveredRuntimeType = true;
              }
            }
          }
        }

        // No luck above, so create a temporary runtime type instead...
        // BHARSH 2018-11-02: This technically generates code that would
        // crash at runtime because aggregate types don't contain the
        // runtime type information for their fields, so this temporary
        // will go uninitialized.
        if (!recoveredRuntimeType) {
          auto fn = toFnSymbol(val->defPoint->parentSymbol);
          bool doEmitError = fn && isUserRoutine(fn) &&
                             !val->hasFlag(FLAG_UNSAFE) &&
                             !at->symbol->hasFlag(FLAG_SYNC) &&
                             !at->symbol->hasFlag(FLAG_SINGLE);
          if (doEmitError) {
            USR_FATAL_CONT(call, "cannot default initialize '%s' because "
                                 "field '%s' of type '%s' does not have "
                                 "enough information to be default "
                                 "initialized",
                                 val->name,
                                 keyName,
                                 toString(value->getValType()));
          }

          // Keep the old lowering even though we'll terminate the pass.
          VarSymbol* tmp = newTemp("default_runtime_temp");
          tmp->addFlag(FLAG_TYPE_VARIABLE);
          CallExpr* query = new CallExpr(PRIM_QUERY_TYPE_FIELD,
                                         at->symbol,
                                         new_CStringSymbol(keyName));
          CallExpr* move = new CallExpr(PRIM_MOVE, tmp, query);
          call->insertBefore(new DefExpr(tmp));
          call->insertBefore(move);
          resolveExpr(query);
          resolveExpr(move);
          appendExpr = new SymExpr(tmp);
        }
      }
    } else if (isGenericField) {

      bool hasCompilerGeneratedInitializer = root->wantsDefaultInitializer();

      if (hasCompilerGeneratedInitializer && hasDefault == false) {
        // Create a temporary to pass for typeless generic fields
        // e.g. for
        //   record R { var x; }
        //   var myR: R(int);
        // convert the  default initialization into
        //   var default_field_tmp: int;
        //   var myR = new R(x=default_field_tmp)
        VarSymbol* temp = newTemp("default_field_temp", value->typeInfo());
        CallExpr* tempCall = new CallExpr(PRIM_DEFAULT_INIT_VAR, temp, value);

        call->insertBefore(new DefExpr(temp));
        call->insertBefore(tempCall);
        resolveExpr(tempCall->get(2));
        resolveExpr(tempCall);
        appendExpr = new SymExpr(temp);

      } else {
        USR_FATAL_CONT(call, "default initialization with type '%s' "
                             "is not yet supported", toString(at));
        USR_PRINT(field, "field '%s' is a generic value",
                         field->name);
        USR_PRINT(field, "consider separately declaring a type field for it "
                         "or using a 'new' call");
        USR_STOP();
      }

    } else {
      INT_FATAL("Unhandled case for default-init");
    }

    appendExpr = new NamedExpr(keyName, appendExpr);

    initCall->insertAtTail(appendExpr);
    idx++;
  }

  return initCall;
}

static void lowerPrimInitGenericRecordVar(CallExpr* call,
                                          Symbol* val,
                                          AggregateType* at) {
  AggregateType* root = at->getRootInstantiation();

  Type* saveType = val->type;

  val->type = root;

  CallExpr* initCall = createGenericRecordVarDefaultInitCall(val, at, call);

  call->insertBefore(initCall);
  resolveCallAndCallee(initCall);
  call->convertToNoop();

  if (saveType != val->type) {
    USR_FATAL_CONT(call, "initializer produces a different type");
    USR_PRINT(call, "when default-initializing '%s' of type '%s'", val->name,
                    toString(saveType));
    USR_PRINT(call, "init resulted in type '%s'", toString(val->type));
  }

  if (at && at->isRecord() && at->hasPostInitializer()) {
    CallExpr* postinit = new CallExpr("postinit", gMethodToken, val);
    call->insertBefore(postinit);
    resolveCallAndCallee(postinit);
  }
}

// Return true if this type is generic *and* resolution will fail
static bool primInitIsUnacceptableGeneric(CallExpr* call, Type* type) {

  // Not generic? OK.
  if (!type->symbol->hasFlag(FLAG_GENERIC))
    return false;

  // Allow generic types in PRIM_INIT_VAR_SPLIT_DECL.
  // The initialization at PRIM_INIT_VAR_SPLIT_INIT will establish a
  // concrete type.
  if (call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL))
    return false;

  bool retval = true;

  // If it is generic then try to resolve the default type constructor
  // for better error reporting.
  if (AggregateType* at = toAggregateType(canonicalDecoratedClassType(type))) {
    SET_LINENO(call);
    CallExpr* typeCall = new CallExpr(at->symbol);
    call->replace(typeCall);

    retval = (tryResolveCall(typeCall) == NULL);

    typeCall->replace(call);
  }

  return retval;
}

// Generate a useful USR_FATAL for an unacceptable Generic
static void primInitHaltForUnacceptableGeneric(CallExpr* call, Type* type, Symbol* val) {
  USR_FATAL_CONT(call,
                 "Cannot default-initialize a variable with generic type");
  USR_PRINT(call, "'%s' has generic type '%s'", val->name, toString(type));
  printUndecoratedClassTypeNote(call, type);
  USR_STOP();
}

void printUndecoratedClassTypeNote(Expr* ctx, Type* type) {
  if (DecoratedClassType* dt = toDecoratedClassType(type)) {
    if (AggregateType* at = dt->getCanonicalClass()) {
      if (!at->symbol->hasFlag(FLAG_GENERIC)) {
        if (isDecoratorUnknownManagement(dt->getDecorator())) {
          if (isDecoratorNilable(dt->getDecorator())) {
            USR_PRINT(ctx, "'%s?' "
                           "indicates a nilable class with any management",
                      at->symbol->name);
          } else {
            USR_PRINT(ctx, "'%s' "
                            "indicates a non-nilable class with any management",
                      at->symbol->name);
          }
          USR_PRINT(ctx, "consider adding a management decorator such as "
                         "'owned', 'shared', 'borrowed', or 'unmanaged'");
        }
      }
    }
  }

}

void checkDuplicateDecorators(Type* decorator, Type* decorated, Expr* ctx) {
  if (isClassLikeOrManaged(decorator) && isClassLikeOrManaged(decorated)) {
    ClassTypeDecoratorEnum d = classTypeDecorator(decorated);

    if (!isDecoratorUnknownManagement(d))
      USR_FATAL_CONT(ctx, "duplicate decorators - %s %s",
                           toString(decorator), toString(decorated));
  }
}

void startGenerousResolutionForErrors() {
  generousResolutionForErrors++;
}

bool inGenerousResolutionForErrors() {
  return generousResolutionForErrors > 0;
}

void stopGenerousResolutionForErrors() {
  generousResolutionForErrors--;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void replaceRuntimeTypeVariableTypes() {
  // Visit asts which are definitions of VarSymbols that are type variables
  // and are flagged as runtime types.
  for_alive_in_Vec(DefExpr, def, gDefExprs) {
    if (isVarSymbol(def->sym) &&
        def->sym->hasFlag(FLAG_TYPE_VARIABLE) &&
        def->sym->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {

      // Collapse these through the runtimeTypeMap ...
      Type* rt = runtimeTypeMap.get(def->sym->type);
      // TODO the following is a workaround for arrayviews. They don't get a
      // RTT in the map above because we use the array's RTT for them. That
      // mapping is stored in `valueToRuntimeTypeMap`. We probably want RTT for
      // arrayviews as well.
      if (!rt) {
        rt = valueToRuntimeTypeMap.get(def->sym->type)->retType;
      }
      // This assert might fail for code that is no longer traversed
      // after it is resolved, ex. in where-clauses.
      INT_ASSERT(rt);
      def->sym->type = rt;

      // ... and remove the type variable flag
      // (Make these declarations look like normal vars.)
      def->sym->removeFlag(FLAG_TYPE_VARIABLE);
    }
  }
}

void expandInitFieldPrims()
{
  for_alive_in_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_INIT_FIELDS))
    {
      initializeClass(call, toSymExpr(call->get(1))->symbol());
      call->remove();
    }
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

DisambiguationContext::DisambiguationContext(CallInfo& info,
                                             BlockStmt* searchScope) {
  actuals = &info.actuals;
  scope   = searchScope;

  explain = false;
  useOldVisibility = false;
  isMethodCall = false;

  if (info.call->numActuals() >= 2) {
    if (SymExpr* se = toSymExpr(info.call->get(1))) {
      if (se->symbol() == gMethodToken) {
        isMethodCall = true;
      }
    }
  }

  // this is a workaround -- a better solution would be preferred
  if (info.call->getModule()->modTag == MOD_INTERNAL) {
    useOldVisibility = true;
  }

  // this is a workaround -- a better solution would be preferred.
  // this function seems to be created in a way that has problems with
  // the visibility logic in disambiguation.
  if (info.call->isNamed("_getIterator")) {
    useOldVisibility = true;
  }

  if (fExplainVerbose == true) {
    if (explainCallLine != 0 && explainCallMatch(info.call) == true) {
      explain = true;
    }

    if (info.call->id == explainCallID) {
      explain = true;
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

DisambiguationState::DisambiguationState() {
  fn1NonParamArgsPreferred = false;
  fn2NonParamArgsPreferred = false;

  fn1ParamArgsPreferred     = false;
  fn2ParamArgsPreferred     = false;
}
