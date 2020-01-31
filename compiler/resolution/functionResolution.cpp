/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
#include "DecoratedClassType.h"
#include "DeferStmt.h"
#include "driver.h"
#include "fixupExports.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "initializerResolution.h"
#include "initializerRules.h"
#include "iterator.h"
#include "lifetime.h"
#include "ModuleSymbol.h"
#include "ParamForLoop.h"
#include "PartialCopyData.h"
#include "passes.h"
#include "postFold.h"
#include "preFold.h"
#include "ResolutionCandidate.h"
#include "resolveFunction.h"
#include "resolveIntents.h"
#include "scopeResolve.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "TryStmt.h"
#include "typeSpecifier.h"
#include "view.h"
#include "virtualDispatch.h"
#include "visibleFunctions.h"
#include "wellknown.h"
#include "WhileStmt.h"
#include "wrappers.h"

#include "../ifa/prim_data.h"

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

  bool  fn1MoreSpecific;
  bool  fn2MoreSpecific;

  bool  fn1Promotes;
  bool  fn2Promotes;

  bool fn1WeakPreferred;
  bool fn2WeakPreferred;

  bool fn1WeakerPreferred;
  bool fn2WeakerPreferred;

  bool fn1WeakestPreferred;
  bool fn2WeakestPreferred;
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
std::map<Type*,     Serializers>   serializeMap;

Map<Type*,          FnSymbol*>     autoDestroyMap;
Map<Type*,          FnSymbol*>     unaliasMap;
Map<Type*,          FnSymbol*>     valueToRuntimeTypeMap;
Map<FnSymbol*,      FnSymbol*>     iteratorLeaderMap;
Map<FnSymbol*,      FnSymbol*>     iteratorFollowerMap;

//#
//# Static Variables
//#
static ModuleSymbol*               explainCallModule;

static Map<Type*,     Type*>       runtimeTypeMap;

static Map<Type*,     FnSymbol*>   runtimeTypeToValueMap;

static std::map<FnSymbol*, const char*> innerCompilerWarningMap;
static std::map<FnSymbol*, const char*> outerCompilerWarningMap;

static std::map<FnSymbol*, const char*> innerCompilerErrorMap;
static std::map<FnSymbol*, const char*> outerCompilerErrorMap;

static CapturedValueMap            capturedValues;

// Used to ensure we only issue the warning once per instantiation
static std::set<FnSymbol*> oldStyleInitCopyFns;

// Enable coercions from nilable -> non-nilable to have easier errors
static int generousResolutionForErrors;

//#
//# Static Function Declarations
//#
static FnSymbol* resolveUninsertedCall(Type* type, CallExpr* call, bool errorOnFailure=true);
static bool hasUserAssign(Type* type);
static void resolveOther();
static bool fits_in_int(int width, Immediate* imm);
static bool fits_in_uint(int width, Immediate* imm);
static bool fits_in_mantissa(int width, Immediate* imm);
static bool fits_in_mantissa_exponent(int mantissa_width, int exponent_width, Immediate* imm, bool realPart=true);
static bool canParamCoerce(Type* actualType, Symbol* actualSym, Type* formalType, bool* paramNarrows);
static bool
moreSpecific(FnSymbol* fn, Type* actualType, Type* formalType);
static BlockStmt* getParentBlock(Expr* expr);

static void resolveTupleExpand(CallExpr* call);
static void resolveSetMember(CallExpr* call);
static void resolveInitField(CallExpr* call);
static void resolveInitVar(CallExpr* call);
static void resolveMove(CallExpr* call);
static void resolveNew(CallExpr* call);
static void resolveCoerce(CallExpr* call);
static void resolveAutoCopyEtc(AggregateType* at);

static Expr* foldTryCond(Expr* expr);

static void unmarkDefaultedGenerics();
static void resolveUses(ModuleSymbol* mod);
static void resolveSupportForModuleDeinits();
static void resolveExports();
static void resolveEnumTypes();
static void insertRuntimeTypeTemps();
static void resolveAutoCopies();
static void resolveSerializers();
static void resolveDestructors();
static Type* buildRuntimeTypeInfo(FnSymbol* fn);
static void insertReturnTemps();
static void initializeClass(Expr* stmt, Symbol* sym);
static void ensureAndResolveInitStringLiterals();
static void handleRuntimeTypes();
static void buildRuntimeTypeInitFns();
static void buildRuntimeTypeInitFn(FnSymbol* fn, Type* runtimeType);
static void replaceValuesWithRuntimeTypes();
static void replaceReturnedValuesWithRuntimeTypes();
static void insertRuntimeInitTemps();
static FnSymbol* findGenMainFn();
static void printCallGraph(FnSymbol* startPoint = NULL,
                           int indent = 0,
                           std::set<FnSymbol*>* alreadyCalled = NULL);
static void printUnusedFunctions();

static void handleTaskIntentArgs(CallInfo& info, FnSymbol* taskFn);
static void lvalueCheckActual(CallExpr* call, Expr* actual, IntentTag intent, ArgSymbol* formal);

static bool  moveIsAcceptable(CallExpr* call);
static void  moveHaltMoveIsUnacceptable(CallExpr* call);


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

/************************************* | **************************************
*                                                                             *
* Invoke resolveFunction(fn) with 'call' on top of 'callStack'.               *
*                                                                             *
************************************** | *************************************/

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
      type->symbol->hasFlag(FLAG_REF) ||
      type->symbol->hasFlag(FLAG_GENERIC)) {

    return;
  }

  CallExpr* call = new CallExpr(dtRef->symbol, type->symbol);
  resolveUninsertedCall(type, call);
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

FnSymbol* getUnalias(Type* t) {
  return unaliasMap.get(t);
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
static bool
isLegalLvalueActualArg(ArgSymbol* formal, Expr* actual) {
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

    bool actualExprTmp = sym->hasFlag(FLAG_EXPR_TEMP);
    TypeSymbol* formalTS = NULL;
    bool formalCopyMutates = false;
    if (formal) {
      formalTS = formal->getValType()->symbol;
      formalCopyMutates = formalTS->hasFlag(FLAG_COPY_MUTATES);
    }
    bool isInitCoerceTmp = sym->name == astr_init_coerce_tmp;

    if ((actualExprTmp && !formalCopyMutates && !isInitCoerceTmp) ||
        (actualConst && !(formal && formal->hasFlag(FLAG_ARG_THIS))) ||
        se->symbol()->isParameter()) {
      // But ignore for now errors with this argument
      // to functions marked with FLAG_REF_TO_CONST_WHEN_CONST_THIS.
      // These will be checked for later, along with ref-pairs.
      if (! (formal && formal->hasFlag(FLAG_ARG_THIS) &&
             calledFn && calledFn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS)))

        return false;
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

  if (formalType == dtString && actualType == dtStringC) {
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
    ClassTypeDecorator actualDec = classTypeDecorator(actualType);

    Type* formalC = canonicalClassType(formalType);
    ClassTypeDecorator formalDec = classTypeDecorator(formalType);

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
    // For smaller integer types, if the argument is a param, does it
    // store a value that's small enough that it could dispatch to
    // this argument?
    //
    if (get_width(formalType) < 64) {
      if (VarSymbol* var = toVarSymbol(actualSym)) {
        if (var->immediate) {
          if (fits_in_int(get_width(formalType), var->immediate)) {
            *paramNarrows = true;
            return true;
          }
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

    if (VarSymbol* var = toVarSymbol(actualSym)) {
      if (var->immediate) {
        if (fits_in_uint(get_width(formalType), var->immediate)) {
          *paramNarrows = true;
          return true;
        }
      }
    }

    //
    // For smaller integer types, if the argument is a param, does it
    // store a value that's small enough that it could dispatch to
    // this argument?
    //
    if (get_width(formalType) < 64) {
      if (VarSymbol* var = toVarSymbol(actualSym)) {
        if (var->immediate) {
          if (fits_in_uint(get_width(formalType), var->immediate)) {
            *paramNarrows = true;
            return true;
          }
        }
      }
    }
  }

  // param strings can coerce between string and c_string
  if ((formalType == dtString || formalType == dtStringC) &&
      (actualType == dtString || actualType == dtStringC)) {
    if (actualSym && actualSym->isImmediate()) {
      return true;
    }
  }

  // coerce fully representable integers into real / real part of complex
  if (is_real_type(formalType)) {
    int mantissa_width = get_mantissa_width(formalType);

    // don't coerce bools to reals (per spec: "unintended by programmer")

    // coerce any integer type to maximum width real
    if ((is_int_type(actualType) || is_uint_type(actualType))
        && get_width(formalType) >= 64)
      return true;

    // coerce integer types that are exactly representable
    if (is_int_type(actualType) &&
        get_width(actualType) < mantissa_width)
      return true;
    if (is_uint_type(actualType) &&
        get_width(actualType) < mantissa_width)
      return true;

    // coerce real from smaller size
    if (is_real_type(actualType) &&
        get_width(actualType) < get_width(formalType))
      return true;

    // coerce literal/param ints that are exactly representable
    if (VarSymbol* var = toVarSymbol(actualSym)) {
      if (var->immediate) {
        if (is_int_type(actualType) || is_uint_type(actualType)) {
          if (fits_in_mantissa(mantissa_width, var->immediate)) {
              *paramNarrows = true;
              return true;
          }
        }
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

    // coerce any integer type to maximum width complex
    if ((is_int_type(actualType) || is_uint_type(actualType)) &&
        get_width(formalType) >= 128)
      return true;

    // coerce integer types that are exactly representable
    if (is_int_type(actualType) &&
        get_width(actualType) < mantissa_width)
      return true;
    if (is_uint_type(actualType) &&
        get_width(actualType) < mantissa_width)
      return true;

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
        if (is_int_type(actualType) || is_uint_type(actualType)) {
          if (fits_in_mantissa(mantissa_width, var->immediate)) {
            *paramNarrows = true;
            return true;
          }
        }
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


static
ClassTypeDecorator removeGenericNilability(ClassTypeDecorator actual) {
  // Normalize actuals to remove generic-ness
  if (actual == CLASS_TYPE_BORROWED)
    actual = CLASS_TYPE_BORROWED_NONNIL;
  if (actual == CLASS_TYPE_UNMANAGED)
    actual = CLASS_TYPE_UNMANAGED_NONNIL;
  if (actual == CLASS_TYPE_MANAGED)
    actual = CLASS_TYPE_MANAGED_NONNIL;

  return actual;
}

/* CLASS_TYPE_BORROWED e.g. can represent any nilability,
   but this function assumes that an actual with type CLASS_TYPE_BORROWED
   is actually the same as CLASS_TYPE_BORROWED_NONNIL.
 */
bool canCoerceDecorators(ClassTypeDecorator actual,
                         ClassTypeDecorator formal,
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
    case CLASS_TYPE_BORROWED:
      // borrowed but generic nilability
      // This would be instantiation
      return false;
    case CLASS_TYPE_BORROWED_NONNIL:
      // Can't coerce away nilable
      return isDecoratorNonNilable(actual) || implicitBang;
    case CLASS_TYPE_BORROWED_NILABLE:
      // Everything can coerce to a nilable borrowed
      // but only subtypes if the actual is already nilable.
      return allowNonSubtypes || isDecoratorNilable(actual);
    case CLASS_TYPE_UNMANAGED:
      // unmanaged but generic nilability
      // This would be instantiation
      return false;
    case CLASS_TYPE_UNMANAGED_NONNIL:
      // Can't coerce away nilable
      // Can't coerce borrowed to unmanaged
      return (implicitBang && actual == CLASS_TYPE_UNMANAGED_NILABLE);
    case CLASS_TYPE_UNMANAGED_NILABLE:
      // Can't coerce borrowed to unmanaged
      return (allowNonSubtypes && actual == CLASS_TYPE_UNMANAGED_NONNIL);

    case CLASS_TYPE_MANAGED:
      // managed but generic nilability
      // this would be instantiation
      return false;
    case CLASS_TYPE_MANAGED_NONNIL:
      // Can't coerce away nilable
      // Can't coerce borrowed to managed
      return (implicitBang && actual == CLASS_TYPE_MANAGED_NILABLE);
    case CLASS_TYPE_MANAGED_NILABLE:
      // Can't coerce borrowed to managed
      return (allowNonSubtypes && actual == CLASS_TYPE_MANAGED_NONNIL);

    case CLASS_TYPE_GENERIC:
      return false; // instantiation not coercion
    case CLASS_TYPE_GENERIC_NONNIL:
      // generally instantiation
      return implicitBang && actual == CLASS_TYPE_GENERIC_NILABLE;
    case CLASS_TYPE_GENERIC_NILABLE:
      // generally instantiation
      return allowNonSubtypes && actual == CLASS_TYPE_GENERIC_NONNIL;

    // no default for compiler warnings to know when to update it
  }

  return false;
}

// Returns true if actual has the same meaning as formal or
// if passing actual to formal should result in instantiation.
bool canInstantiateDecorators(ClassTypeDecorator actual,
                              ClassTypeDecorator formal) {

  if (actual == formal)
    return true;

  // Normalize actuals to remove generic-ness
  actual = removeGenericNilability(actual);

  if (actual == formal)
    return true;

  switch (formal) {
    case CLASS_TYPE_BORROWED:
      return actual == CLASS_TYPE_BORROWED_NONNIL ||
             actual == CLASS_TYPE_BORROWED_NILABLE;
    case CLASS_TYPE_BORROWED_NONNIL:
    case CLASS_TYPE_BORROWED_NILABLE:
      return false;

    case CLASS_TYPE_UNMANAGED:
      return actual == CLASS_TYPE_UNMANAGED_NONNIL ||
             actual == CLASS_TYPE_UNMANAGED_NILABLE;
    case CLASS_TYPE_UNMANAGED_NONNIL:
    case CLASS_TYPE_UNMANAGED_NILABLE:
      return false;

    case CLASS_TYPE_MANAGED:
      return actual == CLASS_TYPE_MANAGED_NONNIL ||
             actual == CLASS_TYPE_MANAGED_NILABLE;
    case CLASS_TYPE_MANAGED_NONNIL:
    case CLASS_TYPE_MANAGED_NILABLE:
      return false;

    case CLASS_TYPE_GENERIC:
      return true;
    case CLASS_TYPE_GENERIC_NONNIL:
      return actual == CLASS_TYPE_GENERIC_NONNIL ||
             actual == CLASS_TYPE_BORROWED_NONNIL ||
             actual == CLASS_TYPE_UNMANAGED_NONNIL ||
             actual == CLASS_TYPE_MANAGED_NONNIL;
    case CLASS_TYPE_GENERIC_NILABLE:
      return actual == CLASS_TYPE_GENERIC_NILABLE ||
             actual == CLASS_TYPE_BORROWED_NILABLE||
             actual == CLASS_TYPE_UNMANAGED_NILABLE||
             actual == CLASS_TYPE_MANAGED_NILABLE;

    // no default for compiler warnings to know when to update it
  }

  return false;
}

// Can we instantiate or coerce or both?
bool canInstantiateOrCoerceDecorators(ClassTypeDecorator actual,
                                      ClassTypeDecorator formal,
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
    case CLASS_TYPE_BORROWED:
      // can borrow from anything, could instantiate as borrowed?
      return true;
    case CLASS_TYPE_BORROWED_NONNIL:
      // can borrow from anything, but can't coerce away nilability
      return isDecoratorNonNilable(actual) || implicitBang;
    case CLASS_TYPE_BORROWED_NILABLE:
      // can borrow from anything, can always coerce to nilable
      return allowNonSubtypes || isDecoratorNilable(actual);;

    case CLASS_TYPE_UNMANAGED:
      // no coercions to unmanaged
      return actual == CLASS_TYPE_UNMANAGED_NONNIL ||
             actual == CLASS_TYPE_UNMANAGED_NILABLE;
    case CLASS_TYPE_UNMANAGED_NONNIL:
      return (implicitBang && actual == CLASS_TYPE_UNMANAGED_NILABLE);
    case CLASS_TYPE_UNMANAGED_NILABLE:
      return (allowNonSubtypes && actual == CLASS_TYPE_UNMANAGED_NONNIL);

    case CLASS_TYPE_MANAGED:
      return actual == CLASS_TYPE_MANAGED_NONNIL ||
             actual == CLASS_TYPE_MANAGED_NILABLE;
    case CLASS_TYPE_MANAGED_NONNIL:
      return (implicitBang && actual == CLASS_TYPE_MANAGED_NILABLE);
    case CLASS_TYPE_MANAGED_NILABLE:
      return (allowNonSubtypes && actual == CLASS_TYPE_MANAGED_NONNIL);

    case CLASS_TYPE_GENERIC:
      // accepts anything
      return true;
    case CLASS_TYPE_GENERIC_NONNIL:
      // accepts anything nonnil
      return isDecoratorNonNilable(actual) || implicitBang;
    case CLASS_TYPE_GENERIC_NILABLE:
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

  if (actualType->symbol->hasFlag(FLAG_C_PTR_CLASS) && formalType == dtCVoidPtr)
    return true;

  if (actualType->symbol->hasFlag(FLAG_C_ARRAY) && formalType == dtCVoidPtr)
    return true;

  if (actualType->symbol->hasFlag(FLAG_C_ARRAY) &&
      formalType->symbol->hasFlag(FLAG_C_PTR_CLASS)) {
    // check element types match
    Type* actualElt = getDataClassType(actualType->symbol)->typeInfo();
    Type* formalElt = getDataClassType(formalType->symbol)->typeInfo();
    if (actualElt && formalElt && actualElt == formalElt)
      return true;
  }

  // Check for class subtyping
  // Class subtyping needs coercions in order to generate C code.
  if (isClassLike(actualType) && isClassLike(formalType)) {
    AggregateType* actualC =
      toAggregateType(canonicalDecoratedClassType(actualType));
    ClassTypeDecorator actualDecorator = classTypeDecorator(actualType);
    AggregateType* formalC =
      toAggregateType(canonicalDecoratedClassType(formalType));
    ClassTypeDecorator formalDecorator = classTypeDecorator(formalType);

    // Check that the decorators allow coercion
    if (canCoerceDecorators(actualDecorator, formalDecorator,
                            /*allowNonSubtypes*/ false,
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

  if (isSyncType(actualType) || isSingleType(actualType)) {
    Type* baseType = actualType->getField("valType")->type;

    // sync can't store an array or a param, so no need to
    // propagate promotes / paramNarrows
    return canDispatch(baseType, NULL, formalType, formalSym, fn);
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
    ClassTypeDecorator actualDecorator = classTypeDecorator(actualType);
    AggregateType* formalC =
      toAggregateType(canonicalDecoratedClassType(formalType));
    ClassTypeDecorator formalDecorator = classTypeDecorator(formalType);

    bool implicitBang = allowImplicitNilabilityRemoval(actualType, actualSym,
                                                       formalType, formalSym);

    // Check that the decorators allow coercion
    if (canCoerceDecorators(actualDecorator, formalDecorator,
                            /*allowNonSubtypes*/ true, implicitBang)) {
      // are the decorated class types the same?
      if (actualC == formalC)
        return true;
      else if (formalC->symbol->hasFlag(FLAG_GENERIC) &&
               instantiatedFieldsMatch(actualC, formalC)) {
        return true;
      }

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
moreSpecific(FnSymbol* fn, Type* actualType, Type* formalType) {
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


//
// helper routine for isMoreVisible (below);
// returns true if fn1 is more visible than fn2
//
static bool
isMoreVisibleInternal(BlockStmt* block, FnSymbol* fn1, FnSymbol* fn2,
                      Vec<BlockStmt*>& visited) {
  //
  // fn1 is more visible
  //
  if (fn1->defPoint->parentExpr == block)
    return true;

  //
  // fn2 is more visible
  //
  if (fn2->defPoint->parentExpr == block)
    return false;

  visited.set_add(block);

  //
  // return true unless fn2 is more visible,
  // including the case where neither are visible
  //

  //
  // ensure f2 is not more visible via parent block, and recurse
  //
  if (BlockStmt* parentBlock = getParentBlock(block))
    if (!visited.set_in(parentBlock))
      if (! isMoreVisibleInternal(parentBlock, fn1, fn2, visited))
        return false;

  //
  // ensure f2 is not more visible via module uses, and recurse
  //
  if (block && block->useList) {
    for_actuals(expr, block->useList) {
      UseStmt* use = toUseStmt(expr);
      INT_ASSERT(use);
      SymExpr* se = toSymExpr(use->src);
      INT_ASSERT(se);
      // We only care about uses of modules during function resolution, not
      // uses of enums.
      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        if (!visited.set_in(mod->block))
          if (! isMoreVisibleInternal(mod->block, fn1, fn2, visited))
            return false;
      }
    }
  }

  return true;
}


//
// return true if fn1 is more visible than fn2 from expr
//
// assumption: fn1 and fn2 are visible from expr; if this assumption
//             is violated, this function will return true
//
static bool
isMoreVisible(Expr* expr, FnSymbol* fn1, FnSymbol* fn2) {
  //
  // common-case check to see if functions have equal visibility
  //
  if (fn1->defPoint->parentExpr == fn2->defPoint->parentExpr) {
    return false;
  }

  //
  // call helper function with visited set to avoid infinite recursion
  //
  Vec<BlockStmt*> visited;
  BlockStmt* block = toBlockStmt(expr);
  if (!block)
    block = getParentBlock(expr);
  return isMoreVisibleInternal(block, fn1, fn2, visited);
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

typedef enum {
  NUMERIC_TYPE_NON_NUMERIC,
  NUMERIC_TYPE_BOOL,
  NUMERIC_TYPE_ENUM,
  NUMERIC_TYPE_INT_UINT,
  NUMERIC_TYPE_REAL,
  NUMERIC_TYPE_IMAG,
  NUMERIC_TYPE_COMPLEX
} numeric_type_t;

static numeric_type_t classifyNumericType(Type* t)
{
  if (is_bool_type(t)) return NUMERIC_TYPE_BOOL;
  if (is_enum_type(t)) return NUMERIC_TYPE_ENUM;
  if (is_int_type(t)) return NUMERIC_TYPE_INT_UINT;
  if (is_uint_type(t)) return NUMERIC_TYPE_INT_UINT;
  if (is_real_type(t)) return NUMERIC_TYPE_REAL;
  if (is_imag_type(t)) return NUMERIC_TYPE_IMAG;
  if (is_complex_type(t)) return NUMERIC_TYPE_COMPLEX;

  return NUMERIC_TYPE_NON_NUMERIC;
}


// Returns 'true' if we should prefer passing actual to f1Type
// over f2Type.
// This method implements rules such as that a bool would prefer to
// coerce to 'int' over 'int(8)'.
static bool prefersCoercionToOtherNumericType(Type* actualType,
                                              Type* f1Type,
                                              Type* f2Type) {

  INT_ASSERT(!actualType->symbol->hasFlag(FLAG_REF));

  if (actualType != f1Type && actualType != f2Type) {
    // Is there any preference among coercions of the built-in type?
    // E.g., would we rather convert 'false' to :int or to :uint(8) ?

    numeric_type_t aT = classifyNumericType(actualType);
    numeric_type_t f1T = classifyNumericType(f1Type);
    numeric_type_t f2T = classifyNumericType(f2Type);

    bool aBoolEnum = (aT == NUMERIC_TYPE_BOOL || aT == NUMERIC_TYPE_ENUM);

    // Prefer e.g. bool(w1) passed to bool(w2) over passing to int (say)
    // Prefer uint(8) passed to uint(16) over passing to a real
    if (aT == f1T && aT != f2T)
      return true;
    // Prefer bool/enum cast to int over uint
    if (aBoolEnum && is_int_type(f1Type) && is_uint_type(f2Type))
      return true;
    // Prefer bool/enum cast to default-sized int/uint over another
    // size of int/uint
    if (aBoolEnum &&
        (f1Type == dtInt[INT_SIZE_DEFAULT] ||
         f1Type == dtUInt[INT_SIZE_DEFAULT]) &&
        f2T == NUMERIC_TYPE_INT_UINT &&
        !(f2Type == dtInt[INT_SIZE_DEFAULT] ||
          f2Type == dtUInt[INT_SIZE_DEFAULT]))
      return true;
    // Prefer bool/enum/int/uint cast to a default-sized real over another
    // size of real or complex.
    if ((aBoolEnum || aT == NUMERIC_TYPE_INT_UINT) &&
        f1Type == dtReal[FLOAT_SIZE_DEFAULT] &&
        (f2T == NUMERIC_TYPE_REAL || f2T == NUMERIC_TYPE_COMPLEX) &&
        f2Type != dtReal[FLOAT_SIZE_DEFAULT])
      return true;
    // Prefer bool/enum/int/uint cast to a default-sized complex over another
    // size of complex.
    if ((aBoolEnum || aT == NUMERIC_TYPE_INT_UINT) &&
        f1Type == dtComplex[COMPLEX_SIZE_DEFAULT] &&
        f2T == NUMERIC_TYPE_COMPLEX &&
        f2Type != dtComplex[COMPLEX_SIZE_DEFAULT])
      return true;
    // Prefer real/imag cast to a same-sized complex over another size of
    // complex.
    if ((aT == NUMERIC_TYPE_REAL || aT == NUMERIC_TYPE_IMAG) &&
        f1T == NUMERIC_TYPE_COMPLEX &&
        f2T == NUMERIC_TYPE_COMPLEX &&
        get_width(actualType)*2 == get_width(f1Type) &&
        get_width(actualType)*2 != get_width(f2Type))
      return true;
  }

  return false;
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

// Does the integer in imm fit in a floating point format with 'width'
// bits of mantissa?
static bool fits_in_mantissa(int width, Immediate* imm) {
  // is it between -2**width .. 2**width, inclusive?

  if (imm->const_kind == NUM_KIND_INT) {
    int64_t i = imm->int_value();
    return fits_in_bits_no_sign(width, i);
  } else if (imm->const_kind == NUM_KIND_UINT) {
    uint64_t u = imm->uint_value();
    if (u > INT64_MAX)
      return false;
    return fits_in_bits_no_sign(width, (int64_t)u);
  }

  return false;
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
      Type* desttype = fn->getFormal(1)->type->getValType();
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

/************************************* | **************************************
*                                                                             *
* Checks that types match.                                                    *
* Note - does not currently check that instantiated params match.             *
*                                                                             *
************************************** | *************************************/

bool signatureMatch(FnSymbol* fn, FnSymbol* gn) {
  bool retval = true;

  if (fn->name != gn->name) {
    retval = false;

  } else if (fn->numFormals() != gn->numFormals()) {
    retval = false;

  } else {
    for (int i = 3; i <= fn->numFormals() && retval == true; i++) {
      ArgSymbol* fa = fn->getFormal(i);
      ArgSymbol* ga = gn->getFormal(i);

      if (fa->type != ga->type || strcmp(fa->name, ga->name) != 0) {
        retval = false;
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static FnSymbol* resolveUninsertedCall(BlockStmt* insert, CallExpr* call, bool errorOnFailure);
static FnSymbol* resolveUninsertedCall(Expr*      insert, CallExpr* call, bool errorOnFailure);

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

static FnSymbol* resolveUninsertedCall(Type* type, CallExpr* call, bool errorOnFailure) {
  FnSymbol*      retval = NULL;

  Expr* where = getInsertPointForTypeFunction(type);
  if (BlockStmt* stmt = toBlockStmt(where))
    retval = resolveUninsertedCall(stmt, call, errorOnFailure);
  else
    retval = resolveUninsertedCall(where, call, errorOnFailure);

  return retval;
}

static FnSymbol* resolveUninsertedCall(BlockStmt* insert, CallExpr* call, bool
    errorOnFailure) {
  BlockStmt* block = new BlockStmt(call, BLOCK_SCOPELESS);

  insert->insertAtHead(block); // Tail?

  if (errorOnFailure)
    resolveCall(call);
  else
    tryResolveCall(call);

  call->remove();
  block->remove();

  return call->resolvedFunction();
}

static FnSymbol* resolveUninsertedCall(Expr* insert, CallExpr* call, bool errorOnFailure) {
  BlockStmt* block = new BlockStmt(call, BLOCK_SCOPELESS);

  insert->insertBefore(block);

  if (errorOnFailure)
    resolveCall(call);
  else
    tryResolveCall(call);

  call->remove();
  block->remove();

  return call->resolvedFunction();
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
    case PRIM_INIT_VAR_SPLIT_DECL:
      resolveGenericActuals(call);
      resolvePrimInit(call);
      break;

    case PRIM_INIT_FIELD:
      resolveInitField(call);
      break;

    case PRIM_INIT_VAR:
    case PRIM_INIT_VAR_SPLIT_INIT:
      resolveInitVar(call);
      break;

    case PRIM_MOVE:
      resolveMove(call);
      break;

    case PRIM_COERCE:
      resolveCoerce(call);
      break;

    case PRIM_NEW:
      resolveNew(call);
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

    resolveNormalCall(call);
  }
}


FnSymbol* tryResolveCall(CallExpr* call) {
  return resolveNormalCall(call, true);
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
                                  ClassTypeDecorator useDec,
                                  AggregateType* manager,
                                  Expr* ctx) {
  SET_LINENO(ctx);

  INT_ASSERT(!isManagedPtrType(canonicalClassType));
  INT_ASSERT(isClass(canonicalClassType));

  // Now type-construct it with appropriate nilability
  ClassTypeDecorator d = combineDecorators(CLASS_TYPE_BORROWED, useDec);
  Type* borrowType = canonicalClassType->getDecoratedClass(d);

  CallExpr* typeCall = new CallExpr(manager->symbol, borrowType->symbol);
  ctx->insertAfter(typeCall);
  resolveCall(typeCall);
  Type* ret = typeCall->typeInfo();
  typeCall->remove();

  INT_ASSERT(ret && ret != dtUnknown);
  return ret;
}

static void adjustClassCastCall(CallExpr* call)
{
  SymExpr* targetTypeSe = toSymExpr(call->get(1));
  SymExpr* valueSe = toSymExpr(call->get(2));
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
    ClassTypeDecorator valueD = classTypeDecorator(valueType);

    Type* canonicalTarget = canonicalClassType(targetType);
    ClassTypeDecorator targetD = classTypeDecorator(targetType);

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
    ClassTypeDecorator d = combineDecorators(targetD, valueD);

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

  if (urse->unresolved != astr_cast || call->numActuals() != 2)
    return false;

  SymExpr* targetTypeSe = toSymExpr(call->get(1));
  SymExpr* valueSe = toSymExpr(call->get(2));

  if (targetTypeSe && valueSe &&
      targetTypeSe->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {

    Type* initialTargetType = targetTypeSe->symbol()->getValType();

    // Fix types for e.g. cast to generic-management MyClass.
    // This can change the symbols pointed to by targetTypeSe/valueSe.
    adjustClassCastCall(call);

    Type* targetType = targetTypeSe->symbol()->getValType();
    Type* valueType = valueSe->symbol()->getValType();

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
      } else if (isBuiltinGenericClassType(initialTargetType) ||
                 isManagedPtrType(initialTargetType)) {
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

    if (!isRecord(targetType) && !isRecord(valueType) &&
        !is_complex_type(targetType) && !is_complex_type(valueType) &&
        dispatches && !promotes) {

      // Otherwise, convert the _cast call to a primitive cast
      call->baseExpr->remove();
      call->primitive = primitives[PRIM_CAST];

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
          ClassTypeDecorator d = CLASS_TYPE_BORROWED_NONNIL;
          if (isDecoratorNilable(classTypeDecorator(t))) {
            d = CLASS_TYPE_BORROWED_NILABLE;
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


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool      isGenericRecordInit(CallExpr* call);

static FnSymbol* resolveNormalCall(CallInfo& info, bool checkOnly);

static void      findVisibleFunctionsAndCandidates(
                                     CallInfo&                  info,
                                     Vec<FnSymbol*>&            visibleFns,
                                     Vec<ResolutionCandidate*>& candidates);

static int       disambiguateByMatch(CallInfo&                  info,
                                     Vec<ResolutionCandidate*>& candidates,
                                     ResolutionCandidate*&      bestRef,
                                     ResolutionCandidate*&      bestConstRef,
                                     ResolutionCandidate*&      bestValue);

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   bool                 checkOnly,
                                   ResolutionCandidate* best);

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   bool                 checkOnly,
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

static Type* resolveTypeSpecifier(CallInfo& info) {
  CallExpr* call = info.call;
  if (call->id == breakOnResolveID) gdbShouldBreakHere();

  Type* ret = NULL;

  SymExpr* ts = toSymExpr(call->baseExpr);
  Type* tsType = ts->typeInfo();
  AggregateType* at = toAggregateType(canonicalClassType(tsType));
  ClassTypeDecorator decorator = CLASS_TYPE_BORROWED_NONNIL;
  bool decorated = false;
  if (DecoratedClassType* dt = toDecoratedClassType(ts->typeInfo())) {
    decorated = true;
    decorator = dt->getDecorator();
    // Convert 'managed' to 'generic' -
    // type will be wrapped with 'owned' e.g. after borrowed type is computed.
    if (isDecoratorManaged(decorator)) {
      if (isDecoratorNonNilable(decorator))
        decorator = CLASS_TYPE_GENERIC_NONNIL;
      else if (isDecoratorNilable(decorator))
        decorator = CLASS_TYPE_GENERIC_NILABLE;
      else
        decorator = CLASS_TYPE_GENERIC;
    }
  }

  if (isPrimitiveType(tsType)) {
    USR_FATAL_CONT(info.call, "illegal type index expression '%s'", info.toString());
    USR_PRINT(info.call, "primitive type '%s' cannot be used in an index expression", tsType->symbol->name);
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

FnSymbol* resolveNormalCall(CallExpr* call, bool checkOnly) {
  CallInfo  info;
  FnSymbol* retval = NULL;

  if (call->id == breakOnResolveID) {
    printf("breaking on resolve call %d:\n", call->id);
    print_view(call);
    gdbShouldBreakHere();
  }

  resolveGenericActuals(call);

  if (isGenericRecordInit(call) == true) {
    retval = resolveInitializer(call);
  } else if (info.isWellFormed(call) == true) {
    if (isTypeConstructionCall(call)) {
      resolveTypeSpecifier(info);
    } else {
      retval = resolveNormalCall(info, checkOnly);
    }

  } else if (checkOnly == true) {
    retval = NULL;

  } else {
    info.haltNotWellFormed();
  }

  return retval;
}

static bool isGenericRecordInit(CallExpr* call) {
  bool retval = false;

  if (UnresolvedSymExpr* ures = toUnresolvedSymExpr(call->baseExpr)) {
    if ((ures->unresolved == astrInit || ures->unresolved == astrInitEquals) &&
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
static bool bestModulesAreConsistent(CallExpr* call, bool checkOnly,
   ModuleSymbol* mod1, ResolutionCandidate* rc1, const char* descr1,
   ModuleSymbol* mod2, ResolutionCandidate* rc2, const char* descr2)
{
  if (mod1 != NULL && mod2 != NULL && mod1 != mod2) {
    if (! checkOnly) {
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

// If 'call' is a method call on a class, accept it if 'bestFn' is
// in the same module as the class of the receiver actual of 'call'.
//
// The purpose of this function is to accept certain method calls
// that method-oblivious checking already rejected.
static bool isAcceptableMethodChoice(CallExpr* call,
                                 FnSymbol* bestFn, FnSymbol* candFn,
                                 Vec<ResolutionCandidate*>& candidates)
{
  if (call->numActuals() < 2 || call->get(1)->getValType() != dtMethodToken)
    return false;  // not a method call

  // Should this be a public utility function ex. canonicalClassTypeOrNull() ?
  Type* actualType = canonicalClassType(call->get(2)->getValType());
  AggregateType* actualClass = toAggregateType(actualType);
  if (actualClass == NULL || ! actualClass->isClass())
    return false;  // a method not on a class

  ModuleSymbol* actualMod = actualClass->getModule();
  if (actualMod == bestFn->getModule())
    return true;  // bestFn and the receiver's class are in the same module

  return false;
}

static void reportHijackingError(CallExpr* call,
                                 FnSymbol* bestFn, ModuleSymbol* bestMod,
                                 FnSymbol* candFn, ModuleSymbol* candMod)
{
  USR_FATAL_CONT(call, "multiple overload sets are applicable to this call");

  if (isMoreVisible(call, candFn, bestFn))
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

static bool overloadSetsOK(CallExpr* call, bool checkOnly,
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
  if (! bestModulesAreConsistent(call, checkOnly,
                                 bestRefMod,  bestRef,  "ref",
                                 bestCrefMod, bestCref, "const ref") ||
      ! bestModulesAreConsistent(call, checkOnly,
                                 bestRefMod,  bestRef,  "ref",
                                 bestValMod,  bestVal,  "value")     ||
      ! bestModulesAreConsistent(call, checkOnly,
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
        ! isMoreVisible(call, bestFn, candidate->fn)                        &&
        ! isAcceptableMethodChoice(call, bestFn, candidate->fn, candidates) )
    {
      if (! checkOnly)
        reportHijackingError(call, bestFn, bestMod, candidate->fn, candMod);
      return false;
    }
  }

  // All checks passed.
  return true;
}


static FnSymbol* resolveForwardedCall(CallInfo& info, bool checkOnly);
static bool typeUsesForwarding(Type* t);

static FnSymbol* resolveNormalCall(CallInfo& info, bool checkOnly) {
  Vec<FnSymbol*>            mostApplicable;
  Vec<ResolutionCandidate*> candidates;

  ResolutionCandidate*      bestRef    = NULL;
  ResolutionCandidate*      bestCref   = NULL;
  ResolutionCandidate*      bestVal    = NULL;

  int                       numMatches = 0;

  FnSymbol*                 retval     = NULL;

  findVisibleFunctionsAndCandidates(info, mostApplicable, candidates);

  numMatches = disambiguateByMatch(info,
                                   candidates,

                                   bestRef,
                                   bestCref,
                                   bestVal);

  // If no candidates were found and it's a method, try forwarding
  if (candidates.n                  == 0 &&
      info.call->numActuals()       >= 1 &&
      info.call->get(1)->typeInfo() == dtMethodToken) {
    Type* receiverType = canonicalDecoratedClassType(info.call->get(2)->getValType());
    if (typeUsesForwarding(receiverType)) {
      FnSymbol* fn = resolveForwardedCall(info, checkOnly);
      if (fn) {
        return fn;
      }
      // otherwise error is printed below
    }
  }

  if (! overloadSetsOK(info.call, checkOnly, candidates,
                       bestRef, bestCref, bestVal)) {
    return NULL; // overloadSetsOK() found an error
  }

  if (numMatches == 0) {
    if (info.call->partialTag == false) {
      if (checkOnly == false) {
        if (candidates.n == 0) {
          bool existingErrors = fatalErrorsEncountered();
          printResolutionErrorUnresolved(info, mostApplicable);

          if (!inGenerousResolutionForErrors()) {
            startGenerousResolutionForErrors();
            FnSymbol* retry = resolveNormalCall(info, /*checkOnly*/ true);
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

    retval = resolveNormalCall(info, checkOnly, best);

  } else {
    retval = resolveNormalCall(info, checkOnly, bestRef, bestCref, bestVal);
  }

  forv_Vec(ResolutionCandidate*, candidate, candidates) {
    delete candidate;
  }

  return retval;
}

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   bool                 checkOnly,
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

    if (checkOnly == false &&
        retval->name                         == astrSassign &&
        isRecord(retval->getFormal(1)->type) == true        &&
        retval->getFormal(2)->type           == dtNil) {
      USR_FATAL(userCall(call),
                "type mismatch in assignment from nil to %s",
                toString(retval->getFormal(1)->type));
    } else {
      SET_LINENO(call);

      if (call->partialTag == true) {
        call->partialTag = false;
      }

      call->baseExpr->replace(new SymExpr(retval));

      resolveNormalCallConstRef(call);

      if (checkOnly == false) {
        resolveNormalCallFinalChecks(call);
      }
    }
  }

  return retval;
}

static FnSymbol* resolveNormalCall(CallInfo&            info,
                                   bool                 checkOnly,
                                   ResolutionCandidate* bestRef,
                                   ResolutionCandidate* bestConstRef,
                                   ResolutionCandidate* bestValue) {
  CallExpr*            call         = info.call;
  CallExpr*            refCall      = NULL;
  CallExpr*            valueCall    = NULL;
  CallExpr*            constRefCall = NULL;
  ResolutionCandidate* best         = NULL;
  FnSymbol*            retval       = NULL;

  if (bestRef      != NULL) {
    refCall = call;

    instantiateBody(bestRef->fn);
  }

  if (bestValue    != NULL) {
    if (bestRef == NULL) {
      valueCall = call;

    } else {
      valueCall = call->copy();

      call->insertAfter(valueCall);
    }

    instantiateBody(bestValue->fn);
  }

  if (bestConstRef != NULL) {
    constRefCall = call->copy();

    call->insertAfter(constRefCall);

    instantiateBody(bestConstRef->fn);
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
        if (checkOnly == false) {
          tmpInfo.haltNotWellFormed();
        }
      }
    }

    if (constRefCall != NULL) {
      CallInfo tmpInfo;

      if (tmpInfo.isWellFormed(constRefCall) == true) {
        wrapAndCleanUpActuals(bestConstRef, tmpInfo, false);

      } else {
        if (checkOnly == false) {
          tmpInfo.haltNotWellFormed();
        }
      }
    }

    if (call->partialTag == true) {
      call->partialTag = false;
    }

    if (checkOnly == false) {
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

    call->insertAfter(contextCall);

    if (refCall      != NULL) refCall->remove();
    if (valueCall    != NULL) valueCall->remove();
    if (constRefCall != NULL) constRefCall->remove();

    contextCall->setRefValueConstRefOptions(refCall,
                                            valueCall,
                                            constRefCall);

    if (checkOnly == false) {
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

  resolveNormalCallCompilerWarningStuff(fn);
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

void resolveNormalCallCompilerWarningStuff(FnSymbol* resolvedFn) {
  reissueMsgs(resolvedFn, innerCompilerErrorMap, outerCompilerErrorMap, true);
  reissueMsgs(resolvedFn, innerCompilerWarningMap, outerCompilerWarningMap, false);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void generateUnresolvedMsg(CallInfo& info, Vec<FnSymbol*>& visibleFns);
static void sortExampleCandidates(CallInfo& info,
                                  Vec<FnSymbol*>& visibleFns);
static void generateCopyInitErrorMsg();

void printResolutionErrorUnresolved(CallInfo&       info,
                                    Vec<FnSymbol*>& visibleFns) {
  if (info.call == NULL) {
    INT_FATAL("call is NULL");

  } else {
    CallExpr* call = userCall(info.call);

    if (call->isCast() == true) {
      if (info.actuals.head()->hasFlag(FLAG_TYPE_VARIABLE) == false) {
        USR_FATAL_CONT(call, "illegal cast to non-type");
      } else {
        Type* dstType = info.actuals.v[0]->type;
        Type* srcType = info.actuals.v[1]->type;
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
        USR_FATAL_CONT(call,
                       "type mismatch in assignment from nil to %s",
                       toString(info.actuals.v[0]->type));

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
        USR_FATAL_CONT(call,
                       "Cannot assign to %s from %s",
                       toString(info.actuals.v[0]->type),
                       toString(info.actuals.v[1]->type));
      }

    } else if (info.name == astrThis) {
      Type* type = info.actuals.v[1]->getValType();

      if (type->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
        USR_FATAL_CONT(call,
                       "illegal access of iterator or promoted expression");

      } else if (type->symbol->hasFlag(FLAG_FUNCTION_CLASS)) {
        USR_FATAL_CONT(call,
                       "illegal access of first class function");

      } else {
        USR_FATAL_CONT(call,
                       "unresolved access of '%s' by '%s'",
                       toString(info.actuals.v[1]->type),
                       info.toString());
      }

    } else {
      generateUnresolvedMsg(info, visibleFns);
    }

    generateCopyInitErrorMsg();

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
    bool ret = false;
    ResolutionCandidate* a = new ResolutionCandidate(aFn);
    ResolutionCandidate* b = new ResolutionCandidate(bFn);

    a->isApplicable(info);
    b->isApplicable(info);

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

static void generateCopyInitErrorMsg() {
  for (int i = callStack.n-1; i >= 0; i--) {
    FnSymbol* currFn = callStack.v[i]->getFunction();
    if (currFn->hasFlag(FLAG_AUTO_COPY_FN) ||
        currFn->hasFlag(FLAG_INIT_COPY_FN)) {
      Type* copied = currFn->getFormal(1)->type;
      if (isNonGenericRecordWithInitializers(copied)) {
        USR_PRINT(copied,
                  "Function 'init' is being treated as a copy initializer for "
                  "type '%s', was that intended?",
                  copied->symbol->name);
        USR_PRINT(copied,
                  "If not, try explicitly declaring the type of the generic "
                  "argument,");
        USR_PRINT(copied,
                  "or excluding '%s' as its type via a where clause",
                  copied->symbol->name);
        return;
      }
    }
  }
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

  forv_Vec(ResolutionCandidate, cand, candidates) {
    if (printedOne == false) {
      USR_PRINT(cand->fn, "candidates are: %s", toString(cand->fn));
      printedOne = true;

    } else {
      USR_PRINT(cand->fn, "                %s", toString(cand->fn));
    }
  }

  if (developer == true) {
    USR_PRINT(call, "unresolved call had id %i", call->id);
  }

  USR_STOP();
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

  } else {
    USR_FATAL_CONT(call, "unresolved call '%s'", str);
  }

  if (visibleFns.n > 0) {
    bool printedOne = false;

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
    int nPrint = 3;

    int i = 0;
    forv_Vec(FnSymbol, fn, visibleFns) {
      i++;

      if (i > nPrintDetails)
        break;

      explainCandidateRejection(info, fn);
    }

    i = 0;
    forv_Vec(FnSymbol, fn, visibleFns) {
      i++;

      if (i <= nPrintDetails)
        continue; // already printed it in detail

      if (fPrintAllCandidates == false && i > nPrint) {
        USR_PRINT("and %i other candidates, use --print-all-candidates to see them",
                  visibleFns.n - (i-1));
        break;
      }

      if (printedOne == false) {
        USR_PRINT(fn, "candidates are: %s", toString(fn));
        printedOne = true;

      } else {
        USR_PRINT(fn, "                %s", toString(fn));
      }
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

static void findVisibleCandidates(CallInfo&                  info,
                                  Vec<FnSymbol*>&            visibleFns,
                                  Vec<ResolutionCandidate*>& candidates);

static void gatherCandidates(CallInfo&                  info,
                             Vec<FnSymbol*>&            visibleFns,
                             bool                       lastResort,
                             Vec<ResolutionCandidate*>& candidates);

static void filterCandidate (CallInfo&                  info,
                             FnSymbol*                  fn,
                             Vec<ResolutionCandidate*>& candidates);


void trimVisibleCandidates(CallInfo&       info,
                           Vec<FnSymbol*>& mostApplicable,
                           Vec<FnSymbol*>& visibleFns) {
  CallExpr* call = info.call;

  bool isMethod = false;
  if (call->numActuals() >= 2) {
    if (SymExpr* se = toSymExpr(call->get(1))) {
      isMethod = se->symbol() == gMethodToken;
    }
  }

  bool isInit   = isMethod && (call->isNamedAstr(astrInit) || call->isNamedAstr(astrInitEquals));
  bool isNew    = call->numActuals() >= 1 && call->isNamedAstr(astrNew);
  bool isDeinit = isMethod && call->isNamedAstr(astrDeinit);

  // 3 actuals: _mt, 'this', thing-to-be-copied
  bool maybeCopyInit = isInit && call->numActuals() == 3 &&
                       call->get(2)->getValType() == call->get(3)->getValType();

  if (!(isInit || isNew || isDeinit) || info.call->isResolved()) {
    mostApplicable = visibleFns;
  } else {
    forv_Vec(FnSymbol, fn, visibleFns) {
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
}

static void findVisibleFunctionsAndCandidates(
                                CallInfo&                  info,
                                Vec<FnSymbol*>&            mostApplicable,
                                Vec<ResolutionCandidate*>& candidates) {
  CallExpr* call = info.call;
  FnSymbol* fn   = call->resolvedFunction();
  Vec<FnSymbol*> visibleFns;

  if (fn != NULL) {
    visibleFns.add(fn);

    handleTaskIntentArgs(info, fn);

  } else {
    findVisibleFunctions(info, visibleFns);
  }

  trimVisibleCandidates(info, mostApplicable, visibleFns);

  findVisibleCandidates(info, mostApplicable, candidates);

  explainGatherCandidate(info, candidates);
}

static void findVisibleCandidates(CallInfo&                  info,
                                  Vec<FnSymbol*>&            visibleFns,
                                  Vec<ResolutionCandidate*>& candidates) {
  // Search user-defined (i.e. non-compiler-generated) functions first.
  gatherCandidates(info, visibleFns, false, candidates);

  // If no results, try again with any compiler-generated candidates.
  if (candidates.n == 0) {
    gatherCandidates(info, visibleFns, true, candidates);
  }
}

static void gatherCandidates(CallInfo&                  info,
                             Vec<FnSymbol*>&            visibleFns,
                             bool                       lastResort,
                             Vec<ResolutionCandidate*>& candidates) {
  forv_Vec(FnSymbol, fn, visibleFns) {
    // Only consider functions marked with/without FLAG_LAST_RESORT
    // (where existence of the flag matches the lastResort argument)
    if (fn->hasFlag(FLAG_LAST_RESORT) == lastResort) {

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
        filterCandidate(info, fn, candidates);

      } else {
        if (fn->hasFlag(FLAG_NO_PARENS) == true) {
          filterCandidate(info, fn, candidates);
        }
      }
    }
  }
}

static void filterCandidate(CallInfo&                  info,
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

  if (candidate->isApplicable(info) == true) {
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
static FnSymbol* resolveForwardedCall(CallInfo& info, bool checkOnly) {
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


static ResolutionCandidate*
disambiguateByMatch(Vec<ResolutionCandidate*>&   candidates,
                    const DisambiguationContext& DC,
                    bool                         ignoreWhere,
                    Vec<ResolutionCandidate*>&   ambiguous);

static int  compareSpecificity(ResolutionCandidate*         candidate1,
                               ResolutionCandidate*         candidate2,
                               const DisambiguationContext& DC,
                               int                          i,
                               int                          j,
                               bool                         ignoreWhere,
                               bool                         forGenericInit);

static void testArgMapping(FnSymbol*                    fn1,
                           ArgSymbol*                   formal1,
                           FnSymbol*                    fn2,
                           ArgSymbol*                   formal2,
                           Symbol*                      actual,
                           const DisambiguationContext& DC,
                           int                          i,
                           int                          j,
                           DisambiguationState&         DS);

ResolutionCandidate*
disambiguateForInit(CallInfo& info, Vec<ResolutionCandidate*>& candidates) {
  DisambiguationContext     DC(info);
  Vec<ResolutionCandidate*> ambiguous;

  return disambiguateByMatch(candidates, DC, false, ambiguous);
}

static int disambiguateByMatch(CallInfo&                  info,
                               Vec<ResolutionCandidate*>& candidates,

                               ResolutionCandidate*&      bestRef,
                               ResolutionCandidate*&      bestConstRef,
                               ResolutionCandidate*&      bestValue) {
  DisambiguationContext     DC(info);

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


static ResolutionCandidate*
disambiguateByMatch(Vec<ResolutionCandidate*>&   candidates,
                    const DisambiguationContext& DC,
                    bool                         ignoreWhere,
                    Vec<ResolutionCandidate*>&   ambiguous) {
  // MPF note: A more straightforwardly O(n) version of this
  // function did not appear to be faster. See history of this comment.

  // If index i is set then we can skip testing function F_i because
  // we already know it can not be the best match.
  std::vector<bool> notBest(candidates.n, false);

  for (int i = 0; i < candidates.n; ++i) {
    EXPLAIN("##########################\n");
    EXPLAIN("# Considering function %d #\n", i);
    EXPLAIN("##########################\n\n");

    ResolutionCandidate* candidate1         = candidates.v[i];
    bool                 singleMostSpecific = true;

    bool forGenericInit = candidate1->fn->isInitializer() || candidate1->fn->isCopyInit();

    EXPLAIN("%s\n\n", toString(candidate1->fn));

    if (notBest[i]) {
      EXPLAIN("Already known to not be best match.  Skipping.\n\n");
      continue;
    }

    for (int j = 0; j < candidates.n; ++j) {
      if (i == j) {
        continue;
      }

      EXPLAIN("Comparing to function %d\n", j);
      EXPLAIN("-----------------------\n");

      ResolutionCandidate* candidate2 = candidates.v[j];

      EXPLAIN("%s\n", toString(candidate2->fn));

      int cmp = compareSpecificity(candidate1,
                                   candidate2,
                                   DC,
                                   i,
                                   j,
                                   ignoreWhere,
                                   forGenericInit);

      if (cmp < 0) {
        EXPLAIN("X: Fn %d is a better match than Fn %d\n\n\n", i, j);
        notBest[j] = true;

      } else if (cmp > 0) {
        EXPLAIN("X: Fn %d is a worse match than Fn %d\n\n\n", i, j);
        notBest[i] = true;
        singleMostSpecific = false;
        break;
      } else {
        EXPLAIN("X: Fn %d is a as good a match as Fn %d\n\n\n", i, j);
        singleMostSpecific = false;
        if (notBest[j]) {
          // Inherit the notBest status of what we are comparing against
          //
          // If this candidate is equally as good as something that wasn't
          // the best, then it is also not the best (or else there is something
          // terribly wrong with our compareSpecificity function).
          notBest[i] = true;
        }
        break;
      }
    }

    if (singleMostSpecific) {
      EXPLAIN("Y: Fn %d is the best match.\n\n\n", i);
      return candidate1;

    } else {
      EXPLAIN("Y: Fn %d is NOT the best match.\n\n\n", i);
    }
  }

  EXPLAIN("Z: No non-ambiguous best match.\n\n");

  for (int i = 0; i < candidates.n; ++i) {
    if (notBest[i] == false) {
      ambiguous.add(candidates.v[i]);
    }
  }

  return NULL;
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
 * \param ignoreWhere Set to `true` to ignore `where` clauses when
 *                    deciding if one match is better than another.
 *                    This is important for resolving return intent
 *                    overloads.
 *
 * \return -1 if fn1 is a more specific function than f2
 * \return 0 if fn1 and fn2 are equally specific
 * \return 1 if fn2 is a more specific function than f1
 */
static int compareSpecificity(ResolutionCandidate*         candidate1,
                              ResolutionCandidate*         candidate2,
                              const DisambiguationContext& DC,
                              int                          i,
                              int                          j,
                              bool                         ignoreWhere,
                              bool                         forGenericInit) {

  DisambiguationState DS;

  // Initializer work-around: Skip _mt/_this for generic initializers
  int                 start   = (forGenericInit == false) ? 0 : 2;

  bool                prefer1 = false;
  bool                prefer2 = false;

  for (int k = start; k < DC.actuals->n; ++k) {
    Symbol*    actual  = DC.actuals->v[k];
    ArgSymbol* formal1 = candidate1->actualIdxToFormal[k];
    ArgSymbol* formal2 = candidate2->actualIdxToFormal[k];

    EXPLAIN("\nLooking at argument %d\n", k);

    testArgMapping(candidate1->fn,
                   formal1,
                   candidate2->fn,
                   formal2,
                   actual,
                   DC,
                   i,
                   j,
                   DS);
  }

  if (DS.fn1Promotes != DS.fn2Promotes) {
    EXPLAIN("\nP: only one of the functions requires argument promotion\n");

    // Prefer the version that did not promote
    prefer1 = !DS.fn1Promotes;
    prefer2 = !DS.fn2Promotes;

  } else if (DS.fn1MoreSpecific != DS.fn2MoreSpecific) {
    EXPLAIN("\nP1: only one more specific argument mapping\n");

    prefer1 = DS.fn1MoreSpecific;
    prefer2 = DS.fn2MoreSpecific;

  } else {
    // If the decision hasn't been made based on the argument mappings...
    if (isMoreVisible(DC.scope, candidate1->fn, candidate2->fn)) {
      EXPLAIN("\nQ: preferring more visible function\n");
      prefer1 = true;

    } else if (isMoreVisible(DC.scope, candidate2->fn, candidate1->fn)) {
      EXPLAIN("\nR: preferring more visible function\n");
      prefer2 = true;

    } else if (DS.fn1WeakPreferred != DS.fn2WeakPreferred) {
      EXPLAIN("\nS: preferring based on weak preference\n");
      prefer1 = DS.fn1WeakPreferred;
      prefer2 = DS.fn2WeakPreferred;

    } else if (DS.fn1WeakerPreferred != DS.fn2WeakerPreferred) {
      EXPLAIN("\nS: preferring based on weaker preference\n");
      prefer1 = DS.fn1WeakerPreferred;
      prefer2 = DS.fn2WeakerPreferred;

    } else if (DS.fn1WeakestPreferred != DS.fn2WeakestPreferred) {
      EXPLAIN("\nS: preferring based on weakest preference\n");
      prefer1 = DS.fn1WeakestPreferred;
      prefer2 = DS.fn2WeakestPreferred;

      /* A note about weak-prefers. Why are there 3 levels?

         Something like 'param x:int(16) = 5' should be able to coerce to any
         integral type. Meanwhile, 'param y = 5' should also be able to coerce
         to any integral type. Now imagine we are resolving 'x+y'.  We
         want it to resolve to the 'int(16)' version because 'x' has a type
         specified, but 'y' is a default type. Before the 3 weak levels, this
         version was chosen simply because non-default-sized ints didn't allow
         param conversion.

       */
    } else if (!ignoreWhere) {
      bool fn1where = candidate1->fn->where != NULL &&
                      !candidate1->fn->hasFlag(FLAG_COMPILER_ADDED_WHERE);
      bool fn2where = candidate2->fn->where != NULL &&
                      !candidate2->fn->hasFlag(FLAG_COMPILER_ADDED_WHERE);

      if (fn1where != fn2where) {
        EXPLAIN("\nU: preferring function with where clause\n");

        prefer1 = fn1where;
        prefer2 = fn2where;
      }
    }
  }

  INT_ASSERT(!(prefer1 && prefer2));

  if (prefer1) {
    EXPLAIN("\nW: Fn %d is more specific than Fn %d\n",
                                i, j);
    return -1;

  } else if (prefer2) {
    EXPLAIN("\nW: Fn %d is less specific than Fn %d\n",
                                i, j);
    return 1;

  } else {
    // Neither is more specific
    EXPLAIN("\nW: Fn %d and Fn %d are equally specific\n",
                                i, j);
    return 0;
  }
}

/** Compare two argument mappings, given a set of actual arguments, and set the
 *  disambiguation state appropriately.
 *
 * This function implements the argument mapping comparison component of the
 * disambiguation procedure as detailed in section 13.14.3 of the Chapel
 * language specification (page 107).
 *
 * \param fn1     The first function to be compared.
 * \param formal1 The formal argument that correspond to the actual argument
 *                for the first function.
 * \param fn2     The second function to be compared.
 * \param formal2 The formal argument that correspond to the actual argument
 *                for the second function.
 * \param actual  The actual argument from the call site.
 * \param DC      The disambiguation context.
 * \param DS      The disambiguation state.
 */
static void testArgMapping(FnSymbol*                    fn1,
                           ArgSymbol*                   formal1,
                           FnSymbol*                    fn2,
                           ArgSymbol*                   formal2,
                           Symbol*                      actual,
                           const DisambiguationContext& DC,
                           int                          i,
                           int                          j,
                           DisambiguationState&         DS) {
  // We only want to deal with the value types here, avoiding odd overloads
  // working (or not) due to _ref.
  Type* f1Type          = formal1->type->getValType();
  Type* f2Type          = formal2->type->getValType();
  Type* actualType      = actual->type->getValType();

  bool  formal1Promotes = false;
  bool  formal2Promotes = false;
  bool  formal1Narrows = false;
  bool  formal2Narrows = false;

  Type* actualScalarType = actualType;

  bool f1Param = formal1->hasFlag(FLAG_INSTANTIATED_PARAM);
  bool f2Param = formal2->hasFlag(FLAG_INSTANTIATED_PARAM);

  bool actualParam = false;
  bool paramWithDefaultSize = false;

  // Don't enable param/ weak preferences for non-default sized param values.
  // If somebody bothered to type the param, they probably want it to stay that
  // way. This is a strategy to resolve ambiguity with e.g.
  //  +(param x:int(32), param y:int(32)
  //  +(param x:int(64), param y:int(64)
  // called with
  //  param x:int(32), param y:int(64)
  if (getImmediate(actual) != NULL) {
    actualParam = true;
    paramWithDefaultSize = isNumericParamDefaultType(actualType);
  }

  EXPLAIN("Actual's type: %s", toString(actualType));
  if (actualParam)
    EXPLAIN(" (param)");
  if (paramWithDefaultSize)
    EXPLAIN(" (default)");
  EXPLAIN("\n");

  canDispatch(actualType, actual,
             f1Type, formal1, fn1,
             &formal1Promotes, &formal1Narrows);

  DS.fn1Promotes |= formal1Promotes;

  EXPLAIN("Formal 1's type: %s", toString(f1Type));
  if (formal1Promotes)
    EXPLAIN(" (promotes)");
  if (formal1->hasFlag(FLAG_INSTANTIATED_PARAM))
    EXPLAIN(" (instantiated param)");
  if (formal1Narrows)
    EXPLAIN(" (narrows param)");
  EXPLAIN("\n");

  if (actualType != f1Type) {
    if (actualParam) {
      EXPLAIN("Actual requires param coercion to match formal 1\n");
    } else {
      EXPLAIN("Actual requires coercion to match formal 1\n");
    }
  }

  canDispatch(actualType, actual,
              f2Type, formal1, fn1,
              &formal2Promotes, &formal2Narrows);

  DS.fn2Promotes |= formal2Promotes;

  EXPLAIN("Formal 2's type: %s", toString(f2Type));
  if (formal2Promotes)
    EXPLAIN(" (promotes)");
  if (formal2->hasFlag(FLAG_INSTANTIATED_PARAM))
    EXPLAIN(" (instantiated param)");
  if (formal2Narrows)
    EXPLAIN(" (narrows param)");
  EXPLAIN("\n");

  // Adjust number of coercions for f2
  if (actualType != f2Type) {
    if (actualParam) {
      EXPLAIN("Actual requires param coercion to match formal 2\n");
    } else {
      EXPLAIN("Actual requires coercion to match formal 2\n");
    }
  }

  // Figure out scalar type for candidate matching
  if ((formal1Promotes || formal2Promotes) &&
      actualType->scalarPromotionType != NULL) {
    actualScalarType = actualType->scalarPromotionType->getValType();
  }

  if (isSyncType(actualScalarType) || isSingleType(actualScalarType)) {
    actualScalarType = actualScalarType->getField("valType")->getValType();
  }

  const char* reason = "";
  typedef enum {
    NONE,
    WEAKEST,
    WEAKER,
    WEAK,
    STRONG
  } arg_preference_t;

  arg_preference_t prefer1 = NONE;
  arg_preference_t prefer2 = NONE;

  if (f1Type == f2Type && f1Param && !f2Param) {
    prefer1 = STRONG; reason = "same type, param vs not";

  } else if (f1Type == f2Type && !f1Param && f2Param) {
    prefer2 = STRONG; reason = "same type, param vs not";

  } else if (!formal1Promotes && formal2Promotes) {
    prefer1 = STRONG; reason = "no promotion vs promotes";

  } else if (formal1Promotes && !formal2Promotes) {
    prefer2 = STRONG; reason = "no promotion vs promotes";

  } else if (f1Type == f2Type           &&
             !formal1->instantiatedFrom &&
             formal2->instantiatedFrom) {
    prefer1 = STRONG; reason = "concrete vs generic";

  } else if (f1Type == f2Type &&
             formal1->instantiatedFrom &&
             !formal2->instantiatedFrom) {
    prefer2 = STRONG; reason = "concrete vs generic";

  } else if (formal1->instantiatedFrom != dtAny &&
             formal2->instantiatedFrom == dtAny) {
    prefer1 = STRONG; reason = "generic any vs partially generic/concrete";

  } else if (formal1->instantiatedFrom == dtAny &&
             formal2->instantiatedFrom != dtAny) {
    prefer2 = STRONG; reason = "generic any vs partially generic/concrete";

  } else if (formal1->instantiatedFrom &&
             formal2->instantiatedFrom &&
             formal1->hasFlag(FLAG_NOT_FULLY_GENERIC) &&
             !formal2->hasFlag(FLAG_NOT_FULLY_GENERIC)) {
    prefer1 = STRONG; reason = "partially generic vs generic";

  } else if (formal1->instantiatedFrom &&
             formal2->instantiatedFrom &&
             !formal1->hasFlag(FLAG_NOT_FULLY_GENERIC) &&
             formal2->hasFlag(FLAG_NOT_FULLY_GENERIC)) {
    prefer2 = STRONG; reason = "partially generic vs generic";

  } else if (f1Param != f2Param && f1Param) {
    prefer1 = WEAK; reason = "param vs not";

  } else if (f1Param != f2Param && f2Param) {
    prefer2 = WEAK; reason = "param vs not";

  } else if (!paramWithDefaultSize && formal2Narrows && !formal1Narrows) {
    prefer1 = WEAK; reason = "no narrows vs narrows";

  } else if (!paramWithDefaultSize && formal1Narrows && !formal2Narrows) {
    prefer2 = WEAK; reason = "no narrows vs narrows";

  } else if (!actualParam && actualType == f1Type && actualType != f2Type) {
    prefer1 = STRONG; reason = "actual type vs not";

  } else if (!actualParam && actualType == f2Type && actualType != f1Type) {
    prefer2 = STRONG; reason = "actual type vs not";

  } else if (actualScalarType == f1Type && actualScalarType != f2Type) {
    if (paramWithDefaultSize)
      prefer1 = WEAKEST;
    else if (actualParam)
      prefer1 = WEAKER;
    else
      prefer1 = STRONG;

    reason = "scalar type vs not";

  } else if (actualScalarType == f2Type && actualScalarType != f1Type) {
    if (paramWithDefaultSize)
      prefer2 = WEAKEST;
    else if (actualParam)
      prefer2 = WEAKER;
    else
      prefer2 = STRONG;

    reason = "scalar type vs not";

  } else if (prefersCoercionToOtherNumericType(actualScalarType, f1Type, f2Type)) {
    if (paramWithDefaultSize)
      prefer1 = WEAKEST;
    else
      prefer1 = WEAKER;

    reason = "preferred coercion to other";

  } else if (prefersCoercionToOtherNumericType(actualScalarType, f2Type, f1Type)) {
    if (paramWithDefaultSize)
      prefer2 = WEAKEST;
    else
      prefer2 = WEAKER;

    reason = "preferred coercion to other";

  } else if (moreSpecific(fn1, f1Type, f2Type) && f2Type != f1Type) {
    prefer1 = actualParam ? WEAKEST : STRONG;
    reason = "can dispatch";

  } else if (moreSpecific(fn1, f2Type, f1Type) && f2Type != f1Type) {
    prefer2 = actualParam ? WEAKEST : STRONG;
    reason = "can dispatch";

  } else if (is_int_type(f1Type) && is_uint_type(f2Type)) {
    // This int/uint rule supports choosing between an 'int' and 'uint'
    // overload when passed say a uint(32).
    prefer1 = actualParam ? WEAKEST : STRONG;
    reason = "int vs uint";

  } else if (is_int_type(f2Type) && is_uint_type(f1Type)) {
    prefer2 = actualParam ? WEAKEST : STRONG;
    reason = "int vs uint";

  }

  if (prefer1 != NONE) {
    const char* level = "";
    if (prefer1 == STRONG)  { DS.fn1MoreSpecific = true;     level = "strong"; }
    if (prefer1 == WEAK)    { DS.fn1WeakPreferred = true;    level = "weak"; }
    if (prefer1 == WEAKER)  { DS.fn1WeakerPreferred = true;  level = "weaker"; }
    if (prefer1 == WEAKEST) { DS.fn1WeakestPreferred = true; level = "weakest"; }
    EXPLAIN("%s: Fn %d is %s preferred\n", reason, i, level);
  } else if (prefer2 != NONE) {
    const char* level = "";
    if (prefer2 == STRONG)  { DS.fn2MoreSpecific = true;     level = "strong"; }
    if (prefer2 == WEAK)    { DS.fn2WeakPreferred = true;    level = "weak"; }
    if (prefer2 == WEAKER)  { DS.fn2WeakerPreferred = true;  level = "weaker"; }
    if (prefer2 == WEAKEST) { DS.fn2WeakestPreferred = true; level = "weakest"; }
    EXPLAIN("%s: Fn %d is %s preferred\n", reason, j, level);
  }
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

        marker->insertBefore("'move'(%S,%S(%S))",
                             capTemp,
                             autoCopy,
                             varActual);

      } else if (isReferenceType(varActual->type) ==  true &&
                 isReferenceType(capTemp->type)   == false) {
        marker->insertBefore("'move'(%S,'deref'(%S))", capTemp, varActual);

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

static void lvalueCheckActual(CallExpr* call, Expr* actual, IntentTag intent, ArgSymbol* formal) {
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
      if (!isLegalLvalueActualArg(formal, actual))
        errorMsg = true;
    break;

   case INTENT_INOUT:
   case INTENT_OUT:
   case INTENT_REF:
    if (!isLegalLvalueActualArg(formal, actual))
      errorMsg = true;
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

    bool isAssign = false;
    if (calleeFn && calleeFn->hasFlag(FLAG_ASSIGNOP))
      isAssign = true;

    if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(call->baseExpr))
      if (urse->unresolved == astrSassign)
        isAssign = true;

    if (isAssign) {
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
        USR_FATAL_CONT(actual, "illegal lvalue in assignment");
      }

    } else if (isInitParam == false) {
      ModuleSymbol* mod          = calleeFn->getModule();
      char          cn1          = calleeFn->name[0];
      const char*   calleeParens = (isalpha(cn1) || cn1 == '_') ? "()" : "";

      // Should this be the same condition as in insertLineNumber() ?
      if (developer || mod->modTag == MOD_USER) {
        USR_FATAL_CONT(actual,
                       "non-lvalue actual is passed to %s formal '%s' "
                       "of %s%s",
                       formal->intentDescrString(),
                       formal->name,
                       calleeFn->name,
                       calleeParens);

      } else {
        USR_FATAL_CONT(actual,
                       "non-lvalue actual is passed to a %s formal of "
                       "%s%s",
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

  if (strncmp(varname, "_formal_tmp_", 12) == 0) {
    varname += 12;
  }

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
*                                                                             *
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

  for (int i = 1; i <= size; i++) {
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
  const char* name = var->immediate->v_string;

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
  const char* name = var->immediate->v_string;

  // Get the type
  AggregateType* ct = toAggregateType(call->get(1)->typeInfo()->getValType());
  if (!ct)
    INT_FATAL(call, "bad initializer set field primitive");

  Symbol* fs = NULL;
  int index = 1;
  for_fields(field, ct) {
    if (!strcmp(field->name, name)) {
      fs = field; break;
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
        (fs->defPoint->init == NULL && fs->defPoint->exprType != NULL &&
         ct->fieldIsGeneric(fs, ignoredHasDefault))) {
      AggregateType* instantiate = ct->getInstantiation(srcSym, index);
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
        if (exprType == dtUnknown)
          fs->type = srcType;
        else
          fs->type = exprType;
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

  if (call->isPrimitive(PRIM_DEFAULT_INIT_FIELD)) {
    return describeFieldInit(get_string(call->get(1)),
                             get_string(call->get(2)), nonnilable);
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

static void resolveInitVar(CallExpr* call) {
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
        if (dst->type != srcType)
          USR_FATAL_CONT(call, "Split initialization uses multiple types; "
                               "another initialization has type %s "
                               "but this initialization has type %s",
                               toString(dst->type),
                               toString(srcType));

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
      INT_FATAL(call, "bad no init call");
    }

    SymExpr* targetTypeExpr = toSymExpr(call->get(3)->remove());
    targetType = targetTypeExpr->typeInfo();

    if (targetType->symbol->hasFlag(FLAG_GENERIC)) {
      // no init needs a concrete type, cannot infer from gNoInit.
      INT_FATAL(call, "bad no init call");
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

    // If the target type is generic, compute the appropriate instantiation
    // type.
    if (targetType->symbol->hasFlag(FLAG_GENERIC)) {
      Type* inst = getInstantiationType(srcType, NULL, targetType, NULL, call);

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
    if (targetType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) ||
        (targetType->symbol->hasFlag(FLAG_TUPLE) && mismatch) ||
        (isRecord(targetType->getValType()) == false && mismatch)) {

      if (mismatch)
        checkMoveIntoClass(call, targetType->getValType(), src->getValType());

      VarSymbol* tmp = newTemp(astr_init_coerce_tmp, targetType);
      tmp->addFlag(FLAG_EXPR_TEMP);
      if (dst->hasFlag(FLAG_PARAM))
        tmp->addFlag(FLAG_PARAM);

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

  // 'var x = new _domain(...)' should not bother going through chpl__initCopy
  // logic so that the result of the 'new' is MOVE'd and not copy-initialized,
  // which is handled in the 'init=' branch.
  bool isDomainWithoutNew = targetType->getValType()->symbol->hasFlag(FLAG_DOMAIN) &&
                            src->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW) == false;
  bool initCopySyncSingle = inferType && (isSyncType(srcType->getValType()) || isSingleType(srcType->getValType()));
  bool initCopyIter = inferType && srcType->getValType()->symbol->hasFlag(FLAG_ITERATOR_RECORD);

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
             srcType->getValType()->symbol->hasFlag(FLAG_TUPLE) ||
             initCopyIter) {
    // These cases require an initCopy to implement special initialization
    // semantics (e.g. reading a sync for variable initialization).
    //
    // For example, even though domains can leverage 'init=' for basic
    // copy-initialization, the compiler only currently knows about calls to
    // "chpl__initCopy" and how to turn them into something else when necessary
    // (e.g. chpl__unalias).

    CallExpr* initCopy = new CallExpr("chpl__initCopy", srcExpr->remove());
    call->insertAtTail(initCopy);
    call->primitive = primitives[PRIM_MOVE];

    resolveExpr(initCopy);
    resolveMove(call);

  } else if (isRecord(targetType->getValType())) {
    AggregateType* at = toAggregateType(targetType->getValType());

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

    if (moveStringLiteral || moveBytesLiteral || canStealTemp) {
      dst->type = src->type;

      call->primitive = primitives[PRIM_MOVE];

      resolveMove(call);
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

      bool foundOldStyleInit = false;
      // Try resolving the old-style copy initializer first
      if (at->hasUserDefinedInitEquals() == false &&
          srcType->getValType() == targetType->getValType()) {
        call->setUnresolvedFunction("init");

        foundOldStyleInit = tryResolveCall(call) != NULL;

        if (foundOldStyleInit) {
          resolveNormalCallFinalChecks(call);
        }
      }

      if (foundOldStyleInit == false) {
        call->setUnresolvedFunction(astrInitEquals);

        resolveExpr(call);

        dst->type = call->resolvedFunction()->_this->getValType();
      } else {
        FnSymbol* fn = call->resolvedFunction();
        if (oldStyleInitCopyFns.find(fn) == oldStyleInitCopyFns.end()) {
          oldStyleInitCopyFns.insert(fn);
          USR_WARN(fn, "'init' has been deprecated as the copy-initializer, use 'init=' instead.");
        }
      }

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

FnSymbol* findCopyInit(AggregateType* at) {
  VarSymbol* tmpAt = newTemp(at);

  FnSymbol* ret = NULL;

  if (at->hasUserDefinedInitEquals() == false) {
    CallExpr* call = new CallExpr("init", gMethodToken, tmpAt, tmpAt);
    ret = resolveUninsertedCall(at, call, false);
  }

  if (ret == NULL) {
    CallExpr* call = NULL;

    call = new CallExpr(astrInitEquals, gMethodToken, tmpAt, tmpAt);

    ret = resolveUninsertedCall(at, call, false);
  } else {
    FnSymbol* fn = ret;
    if (oldStyleInitCopyFns.find(fn) == oldStyleInitCopyFns.end()) {
      oldStyleInitCopyFns.insert(fn);
      USR_WARN(fn, "'init' has been deprecated as the copy-initializer, use 'init=' instead.");
    }
  }

  // ret's instantiationPoint points to the dummy BlockStmt created by
  // resolveUninsertedCall, so it needs to be updated.
  if (ret != NULL) {
    Expr* point = NULL;
    if (BlockStmt* stmt = at->symbol->instantiationPoint) {
      point = stmt;
    }
    ret->setInstantiationPoint(point);
  }

  return ret;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool  moveSupportsUnresolvedFunctionReturn(CallExpr* call);

static bool  isIfExprResult(Expr* LHS);

static Type* moveDetermineRhsType(CallExpr* call);

static Type* moveDetermineLhsType(CallExpr* call);

static bool  moveTypesAreAcceptable(Type* lhsType, Type* rhsType);

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
  INT_ASSERT(call->isPrimitive(PRIM_MOVE)); // caller responsibility
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
    Type* rhsType = moveDetermineRhsType(call);
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

static bool moveIsAcceptable(CallExpr* call) {
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

// Determine type of RHS.
// NB: This function may update the RHS
static Type* moveDetermineRhsType(CallExpr* call) {
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

  if (retval == dtVoid) {
    if (CallExpr* rhsCall = toCallExpr(rhs)) {
      if (FnSymbol* rhsFn = rhsCall->resolvedFunction()) {
        // `expandExternArrayCalls` can add void assignments when
        // the extern function has an explicit void return type.
        // Let those through by looking for two flags it adds.
        if (!(rhsFn->hasFlag(FLAG_VOID_NO_RETURN_VALUE) &&
              rhsFn->hasFlag(FLAG_EXTERN_FN_WITH_ARRAY_ARG))) {
          const char* rhsName = rhsFn->name;
          if (rhsFn->hasFlag(FLAG_PROMOTION_WRAPPER))
            rhsName = unwrapFnName(rhsFn);
          USR_FATAL(userCall(call),
                    "illegal use of function that does not "
                    "return a value: '%s'",
                    rhsName);
        }
      }
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

static Type* moveDetermineLhsType(CallExpr* call) {
  Symbol* lhsSym = toSymExpr(call->get(1))->symbol();

  if (lhsSym->type == dtUnknown || lhsSym->type == dtNil) {
    if (lhsSym->id == breakOnResolveID) {
      gdbShouldBreakHere();
    }

    lhsSym->type = call->get(2)->typeInfo();
  }

  return lhsSym->type;
}

//
//
//

static bool moveTypesAreAcceptable(Type* lhsType, Type* rhsType) {
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
                        lhsType == dtCFnPtr ||
                        lhsType == dtFile;

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
      lhsSym->type != rhsSym->type) {
    USR_FATAL(call, "type alias split initialization uses different types");
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

static bool isManagedPointerInit(SymExpr* typeExpr);

static void resolveNewSetupManaged(CallExpr* newExpr, Type*& manager);

static void handleUnstableNewError(CallExpr* newExpr, Type* newType);

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
    ClassTypeDecorator d = classTypeDecorator(t);
    INT_ASSERT(d == CLASS_TYPE_MANAGED || d == CLASS_TYPE_MANAGED_NILABLE);
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
        AggregateType* at = toAggregateType(type);

        // if needed, make the manager nilable
        if (makeNilable) {
          if (isManagedPtrType(manager))
            manager = getDecoratedClass(manager, CLASS_TYPE_MANAGED_NILABLE);
          else if (manager == dtUnmanaged)
            manager = dtUnmanagedNilable;
          else if (manager == dtBorrowed)
            manager = dtBorrowedNilable;
          else
            INT_FATAL("case not handled");
        }

        checkManagerType(manager);

        // fail if it's a record
        if (isRecord(at) && !isManagedPtrType(at))
          USR_FATAL_CONT(newExpr, "Cannot use new %s with record %s",
                                  toString(manager), toString(type));
        else if (!isClassLikeOrManaged(type))
          USR_FATAL_CONT(newExpr, "cannot use management %s on non-class %s",
                                   toString(manager), toString(type));


        // Use the class type inside a owned/shared/etc
        // unless we are initializing Owned/Shared itself
        if (isManagedPtrType(at) && !isManagedPointerInit(typeExpr)) {
          Type* subtype = getManagedPtrBorrowType(at);
          if (isAggregateType(subtype)) // in particular, not dtUnknown
            at = toAggregateType(subtype);
        }

        // Use the canonical class to simplify the rest of initializer
        // resolution
        if (DecoratedClassType* mt = toDecoratedClassType(type)) {
          at = mt->getCanonicalClass();
        // For records, just ignore the manager
        // e.g. to support 'new owned MyRecord'
        } else if (isRecord(at)) {
          manager = NULL;
        }

        if (manager) {
          if (at != type)
            // Set the type to initialize
            typeExpr->setSymbol(at->symbol);
        }
      }
      if (manager == NULL && fWarnUnstable)
        // Generate an error on 'new MyClass' with fWarnUnstable
        handleUnstableNewError(newExpr, type);
    }
  }
}

static void handleUnstableNewError(CallExpr* newExpr, Type* newType) {
  if (isUndecoratedClassNew(newExpr, newType)) {
    USR_WARN(newExpr, "new %s is unstable", newType->symbol->name);
    USR_PRINT(newExpr, "use 'new unmanaged %s' "
                       "'new owned %s' "
                       "'new shared %s' or "
                       "'new borrowed %s'",
                       newType->symbol->name,
                       newType->symbol->name,
                       newType->symbol->name,
                       newType->symbol->name);
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

static bool isManagedPointerInit(SymExpr* typeExpr) {

  // Managed pointer init methods are:
  //  - accepting a single unmanaged class pointer
  //  - accepting a single managed class pointer

  // everything else is forwarded

  if (typeExpr->next == NULL)
    return false;

  if (typeExpr->next->next != NULL)
    return false;

  Type* singleArgumentType = typeExpr->next->getValType();

  if (isManagedPtrType(singleArgumentType))
    return true;

  if (DecoratedClassType* dt = toDecoratedClassType(singleArgumentType))
    if (dt->isUnmanaged())
      return true;

  return false;
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

static void resolveCoerce(CallExpr* call) {
  resolveGenericActuals(call);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static Type* resolveGenericActual(SymExpr* se, bool decayToBorrow, bool resolvePartials = false);
static Type* resolveGenericActual(SymExpr* se, Type* type, bool decayToBorrow);

Type* resolveDefaultGenericTypeSymExpr(SymExpr* se) {
  return resolveGenericActual(se, false, true);
}

void resolveGenericActuals(CallExpr* call) {
  SET_LINENO(call);

  bool decayToBorrow = false;
  if (SymExpr* baseSe = toSymExpr(call->baseExpr))
    if (TypeSymbol* ts = toTypeSymbol(baseSe->symbol()))
      if (isManagedPtrType(ts->type))
        decayToBorrow = true;

  for_actuals(actual, call) {
    Expr* safeActual = actual;

    if (NamedExpr* ne = toNamedExpr(safeActual)) {
      safeActual = ne->actual;
    }

    if (SymExpr*   se = toSymExpr(safeActual))   {
      resolveGenericActual(se, decayToBorrow);
    }
  }
}

static Type* resolveGenericActual(SymExpr* se, bool decayToBorrow, bool resolvePartials) {
  Type* retval = se->typeInfo();

  if (TypeSymbol* ts = toTypeSymbol(se->symbol())) {
    retval = resolveGenericActual(se, ts->type, decayToBorrow);

  } else if (VarSymbol* vs = toVarSymbol(se->symbol())) {
    if (vs->hasFlag(FLAG_TYPE_VARIABLE) == true) {

      // Fix for complicated extern vars like
      //   extern var x: c_ptr(c_int);
      if ((vs->hasFlag(FLAG_EXTERN) == true || isGlobal(vs)) &&
          vs->defPoint             != NULL &&
          vs->defPoint->init       != NULL &&
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

static Type* resolveGenericActual(SymExpr* se, Type* type, bool decayToBorrow) {
  Type* retval = se->typeInfo();

  ClassTypeDecorator decorator = CLASS_TYPE_BORROWED_NONNIL;
  bool isDecoratedGeneric = false;
  if (DecoratedClassType* dt = toDecoratedClassType(type)) {
    type = dt->getCanonicalClass();
    decorator = dt->getDecorator();
    if (isDecoratorUnknownManagement(decorator))
      isDecoratedGeneric = true;
    if (decayToBorrow) {
      if (isDecoratorNilable(decorator))
        decorator = CLASS_TYPE_BORROWED_NILABLE;
      else
        decorator = CLASS_TYPE_BORROWED_NONNIL;
    }
  }

  if (AggregateType* at = toAggregateType(type)) {
    if (at->symbol->hasFlag(FLAG_GENERIC) && at->isGenericWithDefaults()) {
      CallExpr*   cc    = new CallExpr(at->symbol);

      se->replace(cc);

      resolveCall(cc);

      Type* retType = cc->typeInfo();

      if (decorator != CLASS_TYPE_BORROWED &&
          decorator != CLASS_TYPE_BORROWED_NONNIL) {
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
          INT_FATAL(def->init, "Unable to resolve enumerator type expression");
        }
        if (!is_int_type(t) && !is_uint_type(t)) {
          USR_FATAL(def,
                    "enumerator '%s' is not an integer param value",
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

// Recursively resolve typedefs
Type* resolveTypeAlias(SymExpr* se) {
  Type* retval = NULL;

  if (se != NULL) {
    Type* valType = se->getValType();

    if (valType != dtUnknown) {
      retval = valType;

    } else if (VarSymbol* var = toVarSymbol(se->symbol())) {
      SET_LINENO(var->defPoint);

      DefExpr* def      = var->defPoint;
      Expr*    typeExpr = resolveTypeOrParamExpr(def->init);
      SymExpr* tse      = toSymExpr(typeExpr);

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

void resolveBlockStmt(BlockStmt* blockStmt) {
  for_exprs_postorder(expr, blockStmt) {
    expr = resolveExpr(expr);
    INT_ASSERT(expr != NULL);
  }
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
    retval = foldTryCond(postFold(def));

  } else if (SymExpr* se = toSymExpr(expr)) {
    makeRefType(se->symbol()->type);

    if (ForallStmt* pfs = isForallIterExpr(se)) {
      CallExpr* call = resolveForallHeader(pfs, se);

      retval = resolveExprPhase2(expr, fn, preFold(call));

    } else {
      retval = resolveExprPhase2(expr, fn, expr);
    }

  } else if (CallExpr* call = toCallExpr(expr)) {
    retval = resolveExprPhase2(expr, fn, preFold(call));
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

  } else {
    retval = foldTryCond(postFold(expr));
  }

  return retval;
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
      call->getModule()->modTag             != MOD_INTERNAL  ||
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

    for (int i = head; i >= 0; i--) {
      CallExpr*     frame  = callStack.v[i];
      ModuleSymbol* module = frame->getModule();
      FnSymbol*     fn     = frame->getFunction();

      from = frame;

      if (frame->linenum()                     >  0             &&
          fn->hasFlag(FLAG_COMPILER_GENERATED) == false         &&
          module->modTag                       != MOD_INTERNAL) {
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
        str = astr(str, var->immediate->v_string);
      }
    }

    // collapse newlines and other escape sequences before printing
    str = astr(unescapeString(str, var).c_str());

    if (call->isPrimitive(PRIM_ERROR) == true) {
      USR_FATAL(from, "%s", str);
      if (FnSymbol* fn = callStack.tail()->resolvedFunction())
        innerCompilerErrorMap[fn] = str;
      if (FnSymbol* fn = callStack.v[head]->resolvedFunction())
        outerCompilerErrorMap[fn] = str;
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


void resolve() {
  parseExplainFlag(fExplainCall, &explainCallLine, &explainCallModule);

  unmarkDefaultedGenerics();

  adjustInternalSymbols();

  resolveExterns();

  resolveObviousGlobals();

  resolveUses(ModuleSymbol::mainModule());

  if (printModuleInitModule)
    resolveUses(printModuleInitModule);

  if (chpl_gen_main)
    resolveFunction(chpl_gen_main);

  resolveSupportForModuleDeinits();

  USR_STOP();

  resolveExports();

  resolveEnumTypes();

  insertRuntimeTypeTemps();

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

  // Resolve the string literal constructors after everything else since new
  // ones may be created during postFold
  ensureAndResolveInitStringLiterals();

  handleRuntimeTypes();

  if (fPrintCallGraph) {
    // This needs to go after resolution is complete, but before
    // pruneResolvedTree() removes unused functions (like the uninstantiated
    // versions of generic functions).
    printCallGraph();
  }

  if (fPrintUnusedFns || fPrintUnusedInternalFns)
    printUnusedFunctions();

  pruneResolvedTree();

  resolveForallStmts2();

  freeCache(defaultsCache);

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

static void resolveUses(ModuleSymbol* mod) {
  static Vec<ModuleSymbol*> initMods;
  static int                moduleResolutionDepth = 0;

  if (initMods.set_in(mod) == NULL) {
    initMods.set_add(mod);

    ++moduleResolutionDepth;

    if (ModuleSymbol* parent = mod->defPoint->getModule()) {
      if (parent != theProgram && parent != rootModule) {
        resolveUses(parent);
      }
    }

    for_vector(ModuleSymbol, usedMod, mod->modUseList) {
      resolveUses(usedMod);
    }

    if (fPrintModuleResolution == true) {
      fprintf(stderr,
              "%2d Resolving module %30s ...",
              moduleResolutionDepth,
              mod->name);
    }

    resolveSignatureAndFunction(mod->initFn);

    if (FnSymbol* defn = mod->deinitFn) {
      resolveSignatureAndFunction(defn);
    }

    if (fPrintModuleResolution == true) {
      AstCount visitor = AstCount();

      mod->accept(&visitor);

      fprintf(stderr, " %6d asts\n", visitor.total());
    }

    --moduleResolutionDepth;
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

  resolveUninsertedCall(chpl_gen_main->body, addModule, /*err on fail*/ true);

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
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn == initStringLiterals) {
      // initStringLiterals is exported but is explicitly resolved last since
      // code may be added to it in postFold calls while resolving other
      // functions
      continue;
    }

    if (fn->hasFlag(FLAG_EXPORT) == true) {
      SET_LINENO(fn);

      resolveSignatureAndFunction(fn);

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

static void insertRuntimeTypeTemps() {
  for_alive_in_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->hasFlag(FLAG_HAS_RUNTIME_TYPE) &&
        !ts->hasFlag(FLAG_GENERIC)) {
      SET_LINENO(ts);
      AggregateType* at = toAggregateType(ts->type);
      INT_ASSERT(at);

      VarSymbol* tmp = newTemp("_runtime_type_tmp_", at);
      at->symbol->defPoint->insertBefore(new DefExpr(tmp));
      CallExpr* call = new CallExpr("chpl__convertValueToRuntimeType", tmp);
      FnSymbol* fn = resolveUninsertedCall(at, call);
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

  tmp->defPoint->remove();

  return retval;
}

static void resolveBroadcasters(AggregateType* at) {
  SET_LINENO(at->symbol);
  Serializers& ser = serializeMap[at];
  INT_ASSERT(ser.serializer != NULL);

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
}

static void resolveSerializers() {
  if (fNoRemoteSerialization == true) {
    return;
  }

  for_alive_in_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (! ts->hasFlag(FLAG_GENERIC)                &&
        ! ts->hasFlag(FLAG_ITERATOR_RECORD)        &&
        ! isSingleType(ts->type)                   &&
        ! isSyncType(ts->type)                     &&
        ! ts->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION)) {
      if (AggregateType* at = toAggregateType(ts->type)) {
        if (isRecord(at) == true) {
          bool success = resolveSerializeDeserialize(at);
          if (success) {
            resolveBroadcasters(at);
          }
        }
      }
    }
  }

  resolveAutoCopies();
}

static void resolveDestructors() {
  for_alive_in_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (! ts->hasFlag(FLAG_REF)                     &&
        ! ts->hasFlag(FLAG_GENERIC)                 &&
        ! ts->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION)) {
      if (AggregateType* at = toAggregateType(ts->type)) {
        if (at->hasDestructor()   == false &&
            at->hasInitializers() == true  &&
            isUnusedClass(at)     == false) {
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
static FnSymbol*   autoMemoryFunction(AggregateType* at, const char* fnName);

static void resolveAutoCopies() {
  for_alive_in_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (! ts->hasFlag(FLAG_GENERIC)                 &&
        ! ts->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION)) {
      if (AggregateType* at = toAggregateType(ts->type)) {
        if (isRecord(at) == true) {
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
    FnSymbol* fn = autoMemoryFunction(at, "chpl__autoDestroy");

    fn->addFlag(FLAG_AUTO_DESTROY_FN);

    autoDestroyMap.put(at, fn);
  }

  // resolve unalias
  // We make the 'unalias' hook available to all user records,
  // but for now it only applies to array/domain/distribution
  // in order to minimize the changes.
  if (unaliasMap.get(at) == NULL && isRecordWrappedType(at) == true) {
    FnSymbol* fn = autoMemoryFunction(at, "chpl__unalias");

    unaliasMap.put(at, fn);
  }
}

// Just use 'chpl__initCopy' instead of 'chpl__autoCopy'
// for user-defined records. This way, if the type does not
// support copying, the autoCopyMap will store a function
// marked with FLAG_ERRONEOUS_INITCOPY. Additionally, user-defined
// records shouldn't be defining chpl__initCopy or chpl__autoCopy
// and certainly shouldn't rely on the differences between the two.
static const char* autoCopyFnForType(AggregateType* at) {
  const char* retval = "chpl__autoCopy";

  if (typeNeedsCopyInitDeinit(at)            == true  &&
      at->symbol->hasFlag(FLAG_TUPLE)        == false &&
      at->symbol->hasFlag(FLAG_ITERATOR_RECORD) == false &&
      isRecordWrappedType(at)                == false &&
      isSyncType(at)                         == false &&
      isSingleType(at)                       == false &&
      at->symbol->hasFlag(FLAG_COPY_MUTATES) == false) {
    retval = "chpl__initCopy";
  }

  return retval;
}

static FnSymbol* autoMemoryFunction(AggregateType* at, const char* fnName) {
  VarSymbol* tmp    = newTemp(at);
  CallExpr*  call   = new CallExpr(fnName, tmp);
  FnSymbol*  retval = NULL;

  chpl_gen_main->insertAtHead(new DefExpr(tmp));

  retval = resolveUninsertedCall(at, call);

  resolveFunction(retval);

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
      if (isSyncType(at)                        == true ||
          isSingleType(at)                      == true ||
          at->symbol->hasFlag(FLAG_ATOMIC_TYPE) == true) {
        retval = true;
      }

      // Most class types are POD (user classes, _ddata, c_ptr)
      // Also, there is no need to check the fields of a class type
      // since a variable of that type is a pointer to the instance.
      if (isClass(at) == true) {
        // don't enumerate sub-fields or check for autoCopy etc

      } else {
        // If any field in a record/tuple is not POD, the aggregate is not POD.
        for_fields(field, at) {
          retval = retval | propagateNotPOD(field->typeInfo());
        }

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
    resolveSignature(fn);
    fn->tagIfGeneric();
    if (! fn->isGeneric())
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


static Type*
buildRuntimeTypeInfo(FnSymbol* fn) {
  SET_LINENO(fn);
  AggregateType* ct = new AggregateType(AGGREGATE_RECORD);
  TypeSymbol* ts = new TypeSymbol(astr("_RuntimeTypeInfo"), ct);
  for_formals(formal, fn) {
    if (formal->hasFlag(FLAG_INSTANTIATED_PARAM))
      continue;

    VarSymbol* field = new VarSymbol(formal->name, formal->type);
    ct->fields.insertAtTail(new DefExpr(field));

    if (formal->hasFlag(FLAG_TYPE_VARIABLE))
      field->addFlag(FLAG_TYPE_VARIABLE);
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
  for_alive_in_Vec(CallExpr, call, gCallExprs) {
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


static void ensureAndResolveInitStringLiterals() {
  if (!initStringLiterals) {
    INT_ASSERT(fMinimalModules);
    createInitStringLiterals();
  }
  resolveFunction(initStringLiterals);
}


static void handleRuntimeTypes()
{
  // insertRuntimeTypeTemps is also called earlier in resolve().  That call
  // can insert variables that need autoCopies and inserting autoCopies can
  // insert things that need runtime type temps.  These need to be fixed up
  // by insertRuntimeTypeTemps before buildRuntimeTypeInitFns is called to
  // update the type -> runtimeType mapping.  Without this, there is an
  // actual/formal type mismatch (with --verify) for the code:
  // record R { var A: [1..1][1..1] real; }
  insertRuntimeTypeTemps();
  buildRuntimeTypeInitFns();
  replaceValuesWithRuntimeTypes();
  replaceReturnedValuesWithRuntimeTypes();
  insertRuntimeInitTemps();
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
          if (fn->instantiatedFrom) {
            instFn = fn->instantiatedFrom;
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

//
// buildRuntimeTypeInitFns: Build a 'chpl__convertRuntimeTypeToValue'
// (value) function for all functions tagged as runtime type
// initialization functions.  Also, build a function to return the
// runtime type for all runtime type initialization functions.
//
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
// The actual type specified is fully-resolved during function resolution.  So
// the "runtime type" mechanism is a way to create a parameterized type, but up
// to a point handle it uniformly in the compiler.
// Perhaps a more complete implementation of generic types with inheritance
// would get rid of the need for this specialized machinery.
//
// In practice, we currently use these to create
// runtime types for domains and arrays (via procedures named
// 'chpl__buildDomainRuntimeType' and 'chpl__buildArrayRuntimeType',
// respectively).
//
// For each such flagged function:
//
//   - Clone the function, naming it 'chpl__convertRuntimeTypeToValue'
//     and change it to a value function
//
//   - Replace the body of the original function with a new function
//     that returns the dynamic runtime type info
//
// Subsequently, the functions as written in the modules are now
// called 'chpl__convertRuntimeTypeToValue' and used to initialize
// variables with runtime types later in insertRuntimeInitTemps().
//
// Notice also that the original functions had been marked as type
// functions during parsing even though they were not written as such
// (see addPragmaFlags() in build.cpp for more info).  Now they are
// actually type functions.
//
static void buildRuntimeTypeInitFns() {
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
      // Look only at functions flagged as "runtime type init fn".
      if (fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN)) {

        // Look only at resolved instances.
        if (! fn->isResolved())
          continue;

        INT_ASSERT(fn->retType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE));
        SET_LINENO(fn);

        // Build a new runtime type for this function
        Type* runtimeType = buildRuntimeTypeInfo(fn);
        runtimeTypeMap.put(fn->retType, runtimeType);

        // Build chpl__convertRuntimeTypeToValue() instance.
        buildRuntimeTypeInitFn(fn, runtimeType);
      }
  }
}

// Build a function to return the runtime type by modifying
// the original function.
static void buildRuntimeTypeInitFn(FnSymbol* fn, Type* runtimeType)
{
  // Clone the original function and call the clone chpl__convertRuntimeTypeToValue.
  FnSymbol* runtimeTypeToValueFn = fn->copy();
  INT_ASSERT(runtimeTypeToValueFn->hasFlag(FLAG_RESOLVED));
  runtimeTypeToValueFn->name = astr("chpl__convertRuntimeTypeToValue");
  runtimeTypeToValueFn->cname = runtimeTypeToValueFn->name;

  // Remove this flag from the clone.
  runtimeTypeToValueFn->removeFlag(FLAG_RUNTIME_TYPE_INIT_FN);

  // Make the clone a value function.
  runtimeTypeToValueFn->getReturnSymbol()->removeFlag(FLAG_TYPE_VARIABLE);
  runtimeTypeToValueFn->retTag = RET_VALUE;
  fn->defPoint->insertBefore(new DefExpr(runtimeTypeToValueFn));

  // Remove static arguments from the RTTV function.
  for_formals(formal, runtimeTypeToValueFn)
  {
    if (formal->hasFlag(FLAG_INSTANTIATED_PARAM))
      formal->defPoint->remove();

    if (formal->hasFlag(FLAG_TYPE_VARIABLE))
    {
      Symbol* field = runtimeType->getField(formal->name);
      if (! field->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE))
        formal->defPoint->remove();
    }
  }

  // Insert the clone (convertRuntimeTypeToValue) into the runtimeTypeToValueMap.
  runtimeTypeToValueMap.put(runtimeType, runtimeTypeToValueFn);

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

static void replaceValuesWithRuntimeTypes()
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

static void replaceReturnedValuesWithRuntimeTypes()
{
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->retTag == RET_TYPE) {
        VarSymbol* ret = toVarSymbol(fn->getReturnSymbol());
        if (ret && ret->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          if (FnSymbol* rtfn = valueToRuntimeTypeMap.get(ret->type)) {
            Type* rt = (rtfn->retType->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) ?
                        rtfn->retType : runtimeTypeMap.get(rtfn->retType);
            INT_ASSERT(rt);
            ret->type = rt;
            fn->retType = ret->type;
            fn->retTag = RET_VALUE;
          }
        }
      }
  }
}

static void replaceRuntimeTypeDefaultInit(CallExpr* call) {
  SymExpr* varSe = toSymExpr(call->get(1));
  Symbol* var = varSe->symbol();
  SymExpr* typeSe = toSymExpr(call->get(2));
  Type*    rt = typeSe->symbol()->type;

  if (rt->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
    if (var->hasFlag(FLAG_NO_INIT)) {
      call->convertToNoop();
      return;
    }

    // ('init' x foo), where typeof(foo) has flag "runtime type value"
    //
    // ==>
    //
    // (var _rtt_1)
    // ('move' _rtt_1 ('.v' foo "field1"))
    // (var _rtt_2)
    // ('move' _rtt_2 ('.v' foo "field2"))
    // ('move' x chpl__convertRuntimeTypeToValue _rtt_1 _rtt_2 ... )
    SET_LINENO(call);
    FnSymbol* runtimeTypeToValueFn = runtimeTypeToValueMap.get(rt);
    INT_ASSERT(runtimeTypeToValueFn);
    CallExpr* runtimeTypeToValueCall = new CallExpr(runtimeTypeToValueFn);
    for_formals(formal, runtimeTypeToValueFn) {
      Symbol* field = rt->getField(formal->name);
      INT_ASSERT(field);
      VarSymbol* tmp = newTemp("_runtime_type_tmp_", field->type);
      call->getStmtExpr()->insertBefore(new DefExpr(tmp));
      call->getStmtExpr()->insertBefore(
          new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE,
                                                    typeSe->symbol(), field)));
      if (formal->hasFlag(FLAG_TYPE_VARIABLE))
        tmp->addFlag(FLAG_TYPE_VARIABLE);
      runtimeTypeToValueCall->insertAtTail(tmp);
    }

    call->replace(new CallExpr(PRIM_MOVE, var, runtimeTypeToValueCall));

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

  }
}

static void replaceRuntimeTypeGetField(CallExpr* call) {
  SymExpr* rt = toSymExpr(call->get(2));
  if (rt->typeInfo()->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
    SET_LINENO(call);
    VarSymbol* fieldName = toVarSymbol(toSymExpr(call->get(3))->symbol());
    Immediate* imm = fieldName->immediate;
    INT_ASSERT(imm->const_kind == CONST_KIND_STRING);
    const char* name = imm->v_string;

    Symbol* field = toAggregateType(rt->typeInfo())->getField(name);
    call->replace(new CallExpr(PRIM_GET_MEMBER_VALUE, rt->remove(), field));
  }
}

static void replaceRuntimeTypePrims(std::vector<BaseAST*>& asts) {
  for_vector(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      FnSymbol* parent = isAlive(call) ? call->getFunction() : NULL;

      // Call must be in the tree and lie in a resolved function.
      if (! parent || ! parent->isResolved()) {
        continue;
      }

      if (call->isPrimitive(PRIM_DEFAULT_INIT_VAR)) {
        replaceRuntimeTypeDefaultInit(call);
      } else if (call->isPrimitive(PRIM_GET_RUNTIME_TYPE_FIELD)) {
        replaceRuntimeTypeGetField(call);
      }
    }
  }
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

static void    resolvePrimInit(CallExpr* call, Symbol* val, Expr* fieldNameExpr, Type* type);
static void    resolvePrimInit(CallExpr* call, Symbol* val, Type* type);

static void resolvePrimInitNonGenericRecordVar(CallExpr* call,
                                               Symbol* val,
                                               AggregateType* at);

static void resolvePrimInitGenericRecordVar(CallExpr* call,
                                            Symbol* val,
                                            AggregateType* at);


static bool    primInitIsUnacceptableGeneric(CallExpr* call, Type* type);

static void    primInitHaltForUnacceptableGeneric(CallExpr* call, Type* type, Symbol* val);

void resolvePrimInit(CallExpr* call) {
  Expr* valExpr = NULL;
  Expr* fieldNameExpr = NULL;
  Expr* typeExpr = NULL;

  INT_ASSERT(call->isPrimitive(PRIM_DEFAULT_INIT_VAR) ||
             call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL));

  if (call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL)) {
    // PRIM_INIT_SPLIT_DECL does nothing at this point
    call->convertToNoop();
    return;
  }

  valExpr = call->get(1);
  fieldNameExpr = NULL;
  typeExpr = call->get(2);
  INT_ASSERT(valExpr && typeExpr);

  SymExpr* valSe = toSymExpr(valExpr);
  INT_ASSERT(valSe);
  Symbol* val = valSe->symbol();

  if (SymExpr* se = toSymExpr(typeExpr)) {
    if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      resolvePrimInit(call, val, fieldNameExpr, resolveTypeAlias(se));

    } else {
      USR_FATAL(call, "invalid type specification");
    }

  } else if (CallExpr* ce = toCallExpr(typeExpr)) {
    if (Symbol* field = resolvePrimInitGetField(ce)) {
      if (field->hasFlag(FLAG_TYPE_VARIABLE) == true) {
        resolvePrimInit(call, val, fieldNameExpr, field->typeInfo());

      } else {
        USR_FATAL(call, "invalid type specification");
      }
    }

  } else {
    INT_FATAL(call, "Unsupported primInit");
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
        retval = ct->getField(var->immediate->v_string);

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


static void resolvePrimInit(CallExpr* call,
                            Symbol* val,
                            Expr* fieldNameExpr,
                            Type* type) {

  if (fieldNameExpr == NULL) {
    resolvePrimInit(call, val, type);
  } else {
    // TODO: this is dead code
    INT_FATAL("dead code");

    // Reduce it to variable initialization
    //   - create temporary variable
    //   - default-initialize it
    //   - move it to the field
    VarSymbol* temp = newTemp("default_tmp", type);
    Expr* stmt = call->getStmtExpr();
    stmt->insertBefore(new DefExpr(temp));
    resolvePrimInit(call, temp, type);

    CallExpr* setit = new CallExpr(PRIM_SET_MEMBER, val,
                                   fieldNameExpr->copy(), temp);

    stmt->insertBefore(setit);
    resolveExpr(setit);
  }
}

// Does 'val' feed into a tuple? Ex.:
//   default init var( elt_x1 type owned Foo )
//   .=( tup "x1" elt_x1 )
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
                                  Type* typeToCheck, Type* type)
{
  if (!isNonNilableClassType(typeToCheck) || useLegacyNilability(call))
    return;

  // Work around current problems in array / assoc array types
  bool unsafe = call->getFunction()->hasFlag(FLAG_UNSAFE) ||
                call->getModule()->hasFlag(FLAG_UNSAFE);
  if (unsafe)
    return;

  // Allow default-init assign to work around current compiler oddities.
  // In a future where init= is always used, we can remove this case.
  // Skip this error for a param - it will get "not of a supported param type"
  if (val->hasFlag(FLAG_INITIALIZED_LATER) ||
      val->hasFlag(FLAG_PARAM))
    return;

  const char* descr = val->name;
  if (VarSymbol* v = toVarSymbol(val))
    descr = toString(v);

  CallExpr* uCall = call;
  if (isTupleComponent(val, call)) {
    descr = astr("a tuple component of the type ", toString(type));
    uCall = userCall(call);
  }

  USR_FATAL_CONT(uCall, "Cannot default-initialize %s", descr);
  USR_PRINT("non-nil class types do not support default initialization");

  AggregateType* at = toAggregateType(canonicalDecoratedClassType(type));
  ClassTypeDecorator d = classTypeDecorator(type);
  Type* suggestedType = at->getDecoratedClass(addNilableToDecorator(d));
  USR_PRINT("Consider using the type %s instead", toString(suggestedType));
}

static void errorInvalidParamInit(CallExpr* call, Symbol* val, Type* type) {
  if (val->hasFlag(FLAG_PARAM) &&
      val->hasFlag(FLAG_TEMP) == false &&
      isLegalParamType(type) == false) {
    USR_FATAL_CONT(call, "'%s' is not of a supported param type", val->name);
  }
}

static void resolvePrimInit(CallExpr* call, Symbol* val, Type* type) {

  if (call->id == breakOnResolveID) gdbShouldBreakHere();

  AggregateType* at     = toAggregateType(type);

  val->type = type;

  // Shouldn't be called with ref types
  INT_ASSERT(at == NULL || at->getValType() == at);

  // These are handled in replaceRuntimeTypePrims().
  if (type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) == true) {
    if (call->isPrimitive(PRIM_DEFAULT_INIT_VAR))
      errorInvalidParamInit(call, val, at);

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

      errorIfNonNilableType(call, val, type, type);
    } else {
      call->convertToNoop(); // initialize it PRIM_INIT_VAR_SPLIT_INIT
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
      call->convertToNoop(); // initialize it PRIM_INIT_VAR_SPLIT_INIT
                             // (important for params)
    }

  // non-generic records with initializers
  } else if (at                                           != NULL &&
             at->instantiatedFrom                         == NULL &&
             isNonGenericRecordWithInitializers(at)       == true) {


    errorInvalidParamInit(call, val, at);
    if (!val->hasFlag(FLAG_NO_INIT) &&
        !call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL))
      resolvePrimInitNonGenericRecordVar(call, val, at);
    else
      call->convertToNoop(); // let the memory be uninitialized

  // extern types (but not memory_order)
  } else if (type->symbol->hasFlag(FLAG_EXTERN) &&
             !type->symbol->hasFlag(FLAG_C_MEMORY_ORDER_TYPE)) {

    // Just let the memory be uninitialized
    errorInvalidParamInit(call, val, at);
    call->convertToNoop();

  // generic records with initializers
  } else if (at != NULL && at->symbol->hasFlag(FLAG_TUPLE) == false &&
            (at->isRecord() || at->isUnion())) {

    errorInvalidParamInit(call, val, at);
    if (at->symbol->hasFlag(FLAG_MANAGED_POINTER))
      errorIfNonNilableType(call, val, getManagedPtrBorrowType(at), at);

    if (!val->hasFlag(FLAG_NO_INIT) &&
        !call->isPrimitive(PRIM_INIT_VAR_SPLIT_DECL))
      resolvePrimInitGenericRecordVar(call, val, at);
    else
      call->convertToNoop(); // let the memory be uninitialized

  // other types (sync, single, ..)
  } else {
    errorInvalidParamInit(call, val, at);

    SET_LINENO(call);

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

static void resolvePrimInitNonGenericRecordVar(CallExpr* call,
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

static void resolvePrimInitGenericRecordVar(CallExpr* call,
                                            Symbol* val,
                                            AggregateType* at) {
  AggregateType* root = at->getRootInstantiation();

  val->type = root;

  CallExpr* initCall = new CallExpr("init", gMethodToken, new NamedExpr("this", new SymExpr(val)));
  form_Map(SymbolMapElem, e, at->substitutions) {
    Symbol* field = root->getField(e->key->name);
    bool hasDefault = false;
    bool isGenericField = root->fieldIsGeneric(field, hasDefault);

    Expr* appendExpr = NULL;
    if (field->isParameter()) {
      appendExpr = new SymExpr(e->value);
    } else if (field->hasFlag(FLAG_TYPE_VARIABLE)) {
      if (e->value->getValType()->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
        // BHARSH 2018-11-02: This technically generates code that would
        // crash at runtime because aggregate types don't contain the runtime
        // type information for their fields, so this temporary will go
        // uninitialized. At the moment we fortunately do not access such
        // fields for default-initialized records, and avoid crashing.
        VarSymbol* tmp = newTemp("default_runtime_temp");
        tmp->addFlag(FLAG_TYPE_VARIABLE);
        CallExpr* query = new CallExpr(PRIM_QUERY_TYPE_FIELD, at->symbol, new_CStringSymbol(e->key->name));
        CallExpr* move = new CallExpr(PRIM_MOVE, tmp, query);

        call->insertBefore(new DefExpr(tmp));
        call->insertBefore(move);

        resolveExpr(query);
        resolveExpr(move);

        appendExpr = new SymExpr(tmp);
      } else {
        appendExpr = new SymExpr(e->value);
      }
    } else if (isGenericField && hasDefault == false) {
      // Create a temporary to pass for the fully-generic field (e.g. "var x;")
      VarSymbol* temp = newTemp("default_field_temp", e->value->typeInfo());
      CallExpr* tempCall = new CallExpr(PRIM_DEFAULT_INIT_VAR, temp, e->value);

      call->insertBefore(new DefExpr(temp));
      call->insertBefore(tempCall);
      resolveExpr(tempCall->get(2));
      resolveExpr(tempCall);
      appendExpr = new SymExpr(temp);

    } else {
      INT_FATAL("Unhandled case for default-init");
    }

    appendExpr = new NamedExpr(e->key->name, appendExpr);

    initCall->insertAtTail(appendExpr);
  }

  call->insertBefore(initCall);
  resolveCallAndCallee(initCall);
  call->convertToNoop();

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

  if (isRecord(type) &&
      type->symbol->hasFlag(FLAG_SYNC) == false &&
      type->symbol->hasFlag(FLAG_SINGLE) == false &&
      type->symbol->hasFlag(FLAG_TUPLE) == false &&
      type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) == false) {
    return false;
  }

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
  USR_PRINT(call, "'%s' has generic type '%s'", val->name, type->symbol->name);
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
                           "now means nilable class with any management",
                      at->symbol->name);
            USR_PRINT(ctx, "to migrate old code, change it to 'borrowed %s?'",
                      at->symbol->name);
          } else {
            USR_PRINT(ctx, "'%s' "
                            "now means non-nilable class with any management",
                      at->symbol->name);
            USR_PRINT(ctx, "to migrate old code, change it to 'borrowed %s'",
                      at->symbol->name);
          }
        }
      }
    }
  }

}

void checkDuplicateDecorators(Type* decorator, Type* decorated, Expr* ctx) {
  if (isClassLikeOrManaged(decorator) && isClassLikeOrManaged(decorated)) {
    ClassTypeDecorator d = classTypeDecorator(decorated);

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

static void insertRuntimeInitTemps() {
  std::vector<BaseAST*> asts;
  collect_asts_postorder(rootModule, asts);

  // Collect asts which are definitions of VarSymbols that are type variables
  // and are flagged as runtime types.
  for_vector(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (isVarSymbol(def->sym) &&
          def->sym->hasFlag(FLAG_TYPE_VARIABLE) &&
          def->sym->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {

        // Collapse these through the runtimeTypeMap ...
        Type* rt = runtimeTypeMap.get(def->sym->type);
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

  replaceRuntimeTypePrims(asts);

  for_vector(BaseAST, ast1, asts) {
    if (SymExpr* se = toSymExpr(ast1)) {

      // remove dead type expressions
      if (se->getStmtExpr() == se)
        if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
          se->remove();

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

DisambiguationContext::DisambiguationContext(CallInfo& info) {
  actuals = &info.actuals;
  scope   = (info.scope) ? info.scope : getVisibilityScope(info.call);
  explain = false;

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
  fn1MoreSpecific = false;
  fn2MoreSpecific = false;

  fn1Promotes     = false;
  fn2Promotes     = false;

  fn1WeakPreferred= false;
  fn2WeakPreferred= false;
  fn1WeakerPreferred= false;
  fn2WeakerPreferred= false;
  fn1WeakestPreferred= false;
  fn2WeakestPreferred= false;
}
