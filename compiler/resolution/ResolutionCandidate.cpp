/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#include "ResolutionCandidate.h"

#include "astutil.h"
#include "caches.h"
#include "callInfo.h"
#include "DecoratedClassType.h"
#include "driver.h"
#include "expandVarArgs.h"
#include "expr.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "splitInit.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

static ResolutionCandidateFailureReason
classifyTypeMismatch(Type* actualType, Type* formalType);
static Type* getInstantiationType(Symbol* actual, ArgSymbol* formal, Expr* ctx);
static bool shouldAllowCoercions(Symbol* actual, ArgSymbol* formal);
static bool shouldAllowCoercionsType(Type* actualType, Type* formalType);

std::map<Type*,std::map<Type*,bool> > actualFormalCoercible;

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

ResolutionCandidate::ResolutionCandidate(FnSymbol* function) {
  fn = function;
  isInterimInstantiation = false;
  failingArgument = NULL;
  reason = RESOLUTION_CANDIDATE_MATCH;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool ResolutionCandidate::isApplicable(CallInfo& info,
                                       VisibilityInfo* visInfo) {
  bool retval = false;

  TagGenericResult tagResult = fn->tagIfGeneric(NULL, true);
  if (tagResult == TGR_TAGGING_ABORTED)
    return false;

  resolveConstrainedGenericFun(fn);

  if (! fn->isGeneric()) {
    retval = isApplicableConcrete(info, visInfo);
  } else {
    retval = isApplicableGeneric(info, visInfo);
  }

  // Note: for generic instantiations, this code will be executed twice.
  // This is because by the time the generic branch returns, its function will
  // have been replaced by the instantiation, which will have already had this
  // function called on it.  However, reducing the scope for this operation does
  // not seem to have a noticeable impact.
  if (retval && fn->retExprType != NULL && fn->retType == dtUnknown) {
    resolveSpecifiedReturnType(fn);
  }

  return retval;
}

bool ResolutionCandidate::isApplicableConcrete(CallInfo& info,
                                               VisibilityInfo* visInfo) {

  fn = expandIfVarArgs(fn, info);
  if (fn == NULL) {
    reason = RESOLUTION_CANDIDATE_OTHER;
    return false;
  }

  resolveTypedefedArgTypes();

  if (computeAlignment(info) == false)
    return false;

  return checkResolveFormalsWhereClauses(info, visInfo);
}

bool ResolutionCandidate::isApplicableGeneric(CallInfo& info,
                                              VisibilityInfo* visInfo) {

  FnSymbol* oldFn = fn;

  fn = expandIfVarArgs(fn, info);
  if (fn == NULL) {
    reason = RESOLUTION_CANDIDATE_OTHER;
    return false;
  }

  if (computeAlignment(info) == false)
    return false;

  if (checkGenericFormals(info.call) == false)
    return false;

  // Compute the param/type substitutions for generic arguments.
  if (computeSubstitutions(info.call) == false) {
    reason = RESOLUTION_CANDIDATE_OTHER;
    return false;
  }

  if (fn->isConstrainedGeneric() && ! isApplicableCG(info, visInfo)) {
    reason = RESOLUTION_CANDIDATE_INTERFACE_CONSTRAINTS_NOT_SATISFIED;
    return false;
  }

  /*
   * Instantiate enough of the generic to get through the rest of the
   * filtering and disambiguation processes.
   */
  fn = instantiateSignature(fn, substitutions, visInfo);
  adjustForCGinstantiation(fn, substitutions, isInterimInstantiation);

  if (fn == NULL) {
    reason = RESOLUTION_CANDIDATE_OTHER;
    return false;
  }

  // Return early if instantiating the function resulted in the same function.
  // This avoids infinite recursion.
  if (fn == oldFn)
    return true;

  return isApplicable(info, visInfo);
}

// Computes whether fn's interface constraints are satisfied at the call site.
// Stores witnesses in this->witnesses, if yes.
// Should this be entirely in interfaceResolution.cpp ?
bool ResolutionCandidate::isApplicableCG(CallInfo& info,
                                         VisibilityInfo* visInfo) {
  int indx = 0;
  for_alist(iconExpr, fn->interfaceInfo->interfaceConstraints) {
    ConstraintSat csat = constraintIsSatisfiedAtCallSite(info.call,
                             toIfcConstraint(iconExpr), substitutions);
    if (csat.istm != nullptr) {
      // satisfied with an implements statement
      witnesses.push_back(csat.istm);
      copyIfcRepsToSubstitutions(fn, info.call, indx++,
                                 csat.istm, substitutions);

    } else if (csat.icon != nullptr) {
      // satisfied with a constraint of the enclosing GC function
      isInterimInstantiation = true;

    } else {
      // not satisfied, making this CG fn not applicable
      return false;
    }
  }

  return true; // all constraints are satisfied
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool ResolutionCandidate::computeAlignment(CallInfo& info) {
  formalIdxToActual.clear();
  actualIdxToFormal.clear();

  for (int i = 0; i < fn->numFormals(); i++) {
    formalIdxToActual.push_back(NULL);
  }

  for (int i = 0; i < info.actuals.n; i++) {
    actualIdxToFormal.push_back(NULL);
  }

  // Match named actuals against formal names in the function signature.
  // Record successful matches.
  for (int i = 0; i < info.actuals.n; i++) {
    if (info.actualNames.v[i] != NULL) {
      bool match = false;
      int  j    = 0;
      for_formals(formal, fn) {
        if (info.actualNames.v[i] == formal->name) {

          if (formalIdxToActual[j] != NULL)
            USR_FATAL(info.call, "named argument '%s' provided twice",
                      formal->name);

          match                = true;
          actualIdxToFormal[i] = formal;
          formalIdxToActual[j] = info.actuals.v[i];
          break;
        } else {
          j++;
        }
      }

      // Fail if no matching formal is found.
      if (match == false) {
        failingArgument = info.actuals.v[i];
        reason = RESOLUTION_CANDIDATE_NO_NAMED_ARGUMENT;
        return false;
      }
    }
  }

  // Fill in unmatched formals in sequence with the remaining actuals.
  // Record successful substitutions.
  int        j      = 0;
  ArgSymbol* formal = (fn->numFormals()) ? fn->getFormal(1) : NULL;
  bool skipNextActual = false;
  bool skipNextFormal = false;

  for (int i = 0; i < info.actuals.n; i++) {
    if (info.actualNames.v[i] == NULL) {
      bool match = false;
      bool skippedThisActual = false;

      while (formal != NULL) {
        if (formal->variableExpr) {
          return fn->isGeneric();
        }

        if (fn->hasFlag(FLAG_OPERATOR)) {
          if (formal->typeInfo() == dtMethodToken) {
            // Don't care about method token arguments to operator functions,
            // or the next argument (which should be "this")
            formal = next_formal(formal);
            j++;
            skipNextFormal = true;
          }

          if (skipNextFormal) {
            INT_ASSERT(formal->hasFlag(FLAG_ARG_THIS));
            formal = next_formal(formal);
            j++;
            skipNextFormal = false; // clear
            continue;

          }

          if (info.actuals.v[i]->typeInfo() == dtMethodToken) {
            // Don't care about method token actuals to operator calls, or
            // the next actual (which should correspond to the "this" argument)
            skippedThisActual = true;
            skipNextActual = true;
            break;
          }

          if (skipNextActual) {
            skippedThisActual = true;
            skipNextActual = false; // clear
            break;
          }
        }

        if (formalIdxToActual[j] == NULL) {
          match                = true;
          actualIdxToFormal[i] = formal;
          formalIdxToActual[j] = info.actuals.v[i];
          formal               = next_formal(formal);
          j++;
          break;

        } else {
          formal = next_formal(formal);
          j++;
        }
      }

      // Fail if there are too many unnamed actuals.
      if (match == false) {
        // If this isn't an operator call, or it was an operator call but this
        // actual wasn't intended for a skippable method token or "this"
        // argument, then we should fail at this actual.
        if (!fn->hasFlag(FLAG_OPERATOR) || !skippedThisActual) {
          if (! fn->isGeneric()) {
            failingArgument = info.actuals.v[i];
            reason = RESOLUTION_CANDIDATE_TOO_MANY_ARGUMENTS;
            return false;
          } else if (fn->hasFlag(FLAG_INIT_TUPLE) == false) {
            failingArgument = info.actuals.v[i];
            reason = RESOLUTION_CANDIDATE_TOO_MANY_ARGUMENTS;
            return false;
          }
        }
      }
    }
  }

  // Make sure that any remaining formals are matched by name
  // or have a default value.
  while (formal) {
    if (formalIdxToActual[j] == NULL && formal->defaultExpr == NULL) {
      if (fn->hasFlag(FLAG_OPERATOR) && (formal->typeInfo() == dtMethodToken ||
                                         formal->hasFlag(FLAG_ARG_THIS))) {
      // Operator calls are allowed to skip matching the method token and "this"
      // arguments
      } else {
        failingArgument = formal;
        reason = RESOLUTION_CANDIDATE_TOO_FEW_ARGUMENTS;
        return false;
      }
    }

    formal = next_formal(formal);
    j++;
  }

  return true;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static Type* getBasicInstantiationType(Type* actualType, Symbol* actualSym,
                                       Type* formalType, Symbol* formalSym,
                                       Expr* ctx,
                                       bool allowCoercion,
                                       bool implicitBang);

bool ResolutionCandidate::computeSubstitutions(Expr* ctx) {
  substitutions.clear();

  int nDefault = 0;
  int nIgnored = 0;
  int i = 1;
  for_formals(formal, fn) {
    // Don't compute substitutions for out intent arguments
    // since the type comes from the function body rather than the call site.
    // Do compute substitutions for param or generic typed formals.
    if (formal->originalIntent != INTENT_OUT &&
        (formal->intent == INTENT_PARAM ||
         formal->type->symbol->hasFlag(FLAG_GENERIC))) {

      if (Symbol* actual = formalIdxToActual[i - 1]) {
        computeSubstitution(formal, actual, ctx);

      } else if (formal->defaultExpr != NULL) {
        computeSubstitutionForDefaultExpr(formal, ctx);
        nDefault++;
      } else if (fn->hasFlag(FLAG_OPERATOR) &&
                 (formal->typeInfo() == dtMethodToken ||
                  formal->hasFlag(FLAG_ARG_THIS))) {
        nIgnored++;
      }

      if (isConstrainedType(formal->type)) {
        substitutions.put(formal->type->symbol, substitutions.get(formal));
      }
    }
    i++;
  }

  return substitutions.n + nDefault + nIgnored > 0;
}

bool ResolutionCandidate::verifyGenericFormal(ArgSymbol* formal) const {
  bool retval = true;

  if (formal->intent                      != INTENT_PARAM &&
      formal->hasFlag(FLAG_TYPE_VARIABLE) == false        &&
      formal->type                        != dtAny) {
    if (fn->isDefaultInit() &&
        (formal->hasFlag(FLAG_ARG_THIS)                == false ||
         formal->hasFlag(FLAG_DELAY_GENERIC_EXPANSION) == false)) {
      // This is a compiler generated initializer, so the argument with
      // a generic type corresponds with a class field.
      retval = false;
    }
  }

  return retval;
}

void ResolutionCandidate::computeSubstitution(ArgSymbol* formal,
                                              Symbol*    actual,
                                              Expr*      ctx) {
  if (formal->intent == INTENT_PARAM) {
    if (actual->isParameter() == true) {
      if (formal->type->symbol->hasFlag(FLAG_GENERIC) == false ||
          canInstantiate(actual->type, formal->type)  == true) {
        substitutions.put(formal, actual);
      }
    }

  } else if (formal->type->symbol->hasFlag(FLAG_GENERIC) == true) {
    if (actual->type->symbol->hasFlag(FLAG_GENERIC)   == true &&
        formal->hasFlag(FLAG_ARG_THIS)                == true &&
        formal->hasFlag(FLAG_DELAY_GENERIC_EXPANSION) == true &&
        actual->getValType() == formal->getValType()) {
      // If the "this" arg is generic, we're resolving an initializer, and
      // the actual being passed is also still generic, don't count this as
      // a substitution.  Otherwise, we'll end up in an infinite loop if
      // one of the later generic args has a defaultExpr, as we will always
      // count the this arg as a substitution and so always approach the
      // generic arg with a defaultExpr as though a substitution was going
      // to take place.

    } else if (Type* type = getInstantiationType(actual, formal, ctx)) {
      // String literal actuals aligned with non-param generic formals of
      // type dtAny will result in an instantiation of dtStringC when the
      // function is extern. In other words, let us write:
      //   extern proc foo(str);
      //   foo("bar");
      // and pass "bar" as a c_string instead of a string
      if (fn->hasFlag(FLAG_EXTERN)    == true     &&
          formal->type                == dtAny    &&
          formal->hasFlag(FLAG_PARAM) == false    &&

          type                        == dtString &&

          actual->type                == dtString &&
          actual->isImmediate()       == true) {
        substitutions.put(formal, dtStringC->symbol);

      } else {
        substitutions.put(formal, type->symbol);

      }
    }
  }
}

void ResolutionCandidate::computeSubstitutionForDefaultExpr(ArgSymbol* formal,
                                                            Expr* ctx) {
  Expr* tail = NULL;
  Expr* origTail = formal->defaultExpr->body.tail;

  // Handle the common case of a literal of some sort (e.g. 1)
  if (SymExpr* se = toSymExpr(origTail)) {
    Symbol* sym = se->symbol();
    if (isEnumSymbol(sym)) {
      tail = origTail;
    } else if (VarSymbol* v = toVarSymbol(sym)) {
      if (v->isImmediate())
        tail = origTail;
    }
  }

  if (tail != NULL) {
    if (formal->intent == INTENT_PARAM) {
      if (SymExpr* se = toSymExpr(tail)) {
        if (se->symbol()->isParameter() == false) {
          USR_FATAL(formal, "default value for param is not a param");

        } else if (formal->type->symbol->hasFlag(FLAG_GENERIC)      == true &&
                   canInstantiate(se->symbol()->type, formal->type) == false) {
          USR_FATAL(formal,
                    "type mismatch between declared formal type "
                    "and default value type");

        } else {
          substitutions.put(formal, se->symbol());
        }

      } else {
        USR_FATAL(formal, "default value for param is not a param");
      }

    } else if (formal->type->symbol->hasFlag(FLAG_GENERIC) == true) {
      Type* defaultType = tail->typeInfo();

      bool inOutCopy = inOrOutFormalNeedingCopyType(formal);
      if (defaultType == dtTypeDefaultToken) {
        substitutions.put(formal, dtTypeDefaultToken->symbol);

      } else if (Type* type = getInstantiationType(defaultType, NULL,
                                                   formal->type, NULL, ctx,
                                                   true, false,
                                                   inOutCopy)) {
        substitutions.put(formal, type->symbol);
      }
    }
  }
}

static bool shouldAllowCoercions(Symbol* actual, ArgSymbol* formal) {
  bool allowCoercions = true;

  if (formal->hasFlag(FLAG_TYPE_VARIABLE)) {
    // Generally, do not allow coercions
    allowCoercions = false;

    // ... however, make an exception for class subtyping.
    Type* actualType = actual->getValType();
    Type* formalType = formal->getValType();
    std::map<Type*,bool>& formalCoercible = actualFormalCoercible[actualType];
    if (formalCoercible.count(formalType) > 0) {
      allowCoercions = formalCoercible[formalType];
    } else {
      allowCoercions = shouldAllowCoercionsType(actualType, formalType);
      formalCoercible[formalType] = allowCoercions;
    }
  }

  return allowCoercions;
}

static bool shouldAllowCoercionsType(Type* actualType, Type* formalType) {
  if (isClassLikeOrManaged(actualType) && isClassLikeOrManaged(formalType)) {
    Type* canonicalActual = canonicalClassType(actualType);
    ClassTypeDecorator actualD = classTypeDecorator(actualType);

    Type* canonicalFormal = canonicalClassType(formalType);
    ClassTypeDecorator formalD = classTypeDecorator(formalType);

    AggregateType* at = toAggregateType(canonicalActual);

    if (canInstantiateOrCoerceDecorators(actualD, formalD, false, false)) {
      if (canonicalActual == canonicalFormal ||
          isDispatchParent(canonicalActual, canonicalFormal) ||
          (at && at->instantiatedFrom &&
           canonicalFormal->symbol->hasFlag(FLAG_GENERIC) &&
           getConcreteParentForGenericFormal(at, canonicalFormal) != NULL)) {
        return true;
      }
    }
  }
  return false;
}

void clearCoercibleCache() {
  actualFormalCoercible.clear();
}

// Uses formalSym and actualSym to compute allowCoercion, implicitBang, and
// inOutCopy in a way that is appropriate for uses when resolving arguments
static
Type* getInstantiationType(Symbol* actual, ArgSymbol* formal, Expr* ctx) {
  bool allowCoercions = shouldAllowCoercions(actual, formal);

  bool implicitBang = allowImplicitNilabilityRemoval(actual->type, actual,
                                                     formal->type, formal);

  bool inOutCopy = inOrOutFormalNeedingCopyType(formal);

  return getInstantiationType(actual->type, actual, formal->type, formal, ctx,
                              allowCoercions, implicitBang, inOutCopy);
}


Type* getInstantiationType(Type* actualType, Symbol* actualSym,
                           Type* formalType, Symbol* formalSym,
                           Expr* ctx,
                           bool allowCoercion, bool implicitBang,
                           bool inOrOtherValue) {

  // memoize unaliasing for in/inout/out/value return
  if (inOrOtherValue) {
    Type* valType = actualType->getValType();
    if (Type* copyType = getCopyTypeDuringResolution(valType)) {
      if (isReferenceType(actualType)) {
        // make the new actual type also a reference type
        INT_ASSERT(copyType->refType);
        actualType = copyType->refType;
      } else {
        actualType = copyType;
      }
    }
  }

  Type* ret = getBasicInstantiationType(actualType, actualSym,
                                        formalType, formalSym, ctx,
                                        allowCoercion, implicitBang);

  // If that didn't work, try it again with the value type.
  if (ret == NULL) {
    if (Type* vt = actualType->getValType()) {
      ret = getBasicInstantiationType(vt, actualSym,
                                      formalType, formalSym, ctx,
                                      allowCoercion, implicitBang);
    }
  }

  // If we still don't have an instantiation type, try it again
  // with the promotion type.
  if (ret == NULL) {
    if (Type* st = actualType->getValType()->scalarPromotionType) {
      ret = getBasicInstantiationType(st->getValType(), actualSym,
                                      formalType, formalSym, ctx,
                                      allowCoercion, implicitBang);
    }
  }

  return ret;
}

bool inOrOutFormalNeedingCopyType(ArgSymbol* formal) {
  // Rule out type/param variables
  if (formal->hasFlag(FLAG_TYPE_VARIABLE) ||
      formal->hasFlag(FLAG_PARAM) ||
      formal->intent == INTENT_TYPE ||
      formal->originalIntent == INTENT_TYPE ||
      formal->intent == INTENT_PARAM ||
      formal->originalIntent == INTENT_PARAM)
    return false;

  FnSymbol* inFn = formal->defPoint->getFunction();
  INT_ASSERT(inFn);

  // Don't consider 'in' in chpl__coerceMove for this purpose.
  if (inFn->name == astr_coerceMove || inFn->name == astr_coerceCopy ||
      inFn->name == astr_initCopy || inFn->name == astr_autoCopy)
    return false;

  return (formal->originalIntent == INTENT_IN ||
          formal->originalIntent == INTENT_CONST_IN ||
          formal->originalIntent == INTENT_INOUT);
}

static Type* getBasicInstantiationType(Type* actualType, Symbol* actualSym,
                                       Type* formalType, Symbol* formalSym,
                                       Expr* ctx,
                                       bool allowCoercion,
                                       bool implicitBang) {
  if (canInstantiate(actualType, formalType)) {
    return actualType;
  }

  // The formal is generic but the actual might require a coercion
  // on the way to it. In that event, instantiate the formal type
  // using the type that the actual will coerce to.

  // E.g. a MyClass actual passed to an x:borrowed? formal
  // should instantiate with MyClass?

  if (isClassLikeOrManaged(actualType) && isClassLikeOrManaged(formalType)) {
    Type* canonicalActual = canonicalClassType(actualType);
    ClassTypeDecorator actualDec = classTypeDecorator(actualType);
    AggregateType* actualManager = NULL;
    if (isManagedPtrType(actualType))
      actualManager = getManagedPtrManagerType(actualType);

    Type* canonicalFormal = canonicalClassType(formalType);
    ClassTypeDecorator formalDec = classTypeDecorator(formalType);
    AggregateType* formalManager = NULL;
    if (isManagedPtrType(formalType))
      formalManager = getManagedPtrManagerType(formalType);

    if (canInstantiateDecorators(actualDec, formalDec) ||
        (allowCoercion && canInstantiateOrCoerceDecorators(actualDec,
                                                           formalDec,
                                                           true,
                                                           implicitBang))) {
      // Can the canonical formal type instantiate with the canonical actual?

      // Adjust the formalDec to use when instantiating
      // according to the actual decorator, when formalDec is generic.
      ClassTypeDecorator useDec = combineDecorators(formalDec, actualDec);
      Type* useType = NULL;
      AggregateType* useManager = actualManager ? actualManager : formalManager;

      // handle e.g. unmanaged MyClass actual -> borrowed MyClass? formal
      if (canInstantiate(canonicalActual, canonicalFormal) ||
          isBuiltinGenericClassType(canonicalFormal)) {
        useType = canonicalActual;
      }
      // Handle owned MyClass actual for owned! formal (say)
      // but not owned MyClass -> shared!
      if (canonicalFormal == actualManager && formalManager == actualManager) {
        useType = canonicalActual;
      }

      // Now, if formalType is a generic parent type to actualType,
      // we should instantiate the parent actual type
      if (allowCoercion && useType == NULL) {
        if (AggregateType* at = toAggregateType(canonicalActual)) {
          if (at->instantiatedFrom                           != NULL  &&
              canonicalFormal->symbol->hasFlag(FLAG_GENERIC) == true) {
            if (Type* c = getConcreteParentForGenericFormal(at, canonicalFormal)) {
              useType = c;
            }
          }
        }
      }
      // If the formal was a parent class with generic management e.g.
      if (allowCoercion && useType == NULL) {
        if (isDecoratorUnknownNilability(formalDec) ||
            isDecoratorUnknownManagement(formalDec)) {
          bool promotes = false;
          if (canCoerce(canonicalActual, actualSym,
                        canonicalFormal, toArgSymbol(formalSym),
                        NULL, &promotes, NULL) && !promotes) {
            useType = canonicalFormal;
            useManager = formalManager ? formalManager : actualManager;
          }
        }
      }

      if (useType != NULL) {
        // If the formal is e.g. _owned
        // any-management formal -> return actual
        if (isDecoratorManaged(useDec)) {
          INT_ASSERT(ctx != NULL);
          INT_ASSERT(useManager != NULL);

          AggregateType* at = toAggregateType(useType);
          INT_ASSERT(at);
          return computeDecoratedManagedType(at, useDec, useManager, ctx);
        }

        // Then compute the instantiation type as the actual
        // with the formal's decorator.
        return getDecoratedClass(useType, useDec);
      }
    }
  }

  if (actualType == dtNil) {
    if (isBuiltinGenericClassType(formalType) &&
        !isNonNilableClassType(formalType))
      return actualType;
  }

  if (isSyncType(actualType) || isSingleType(actualType)) {
    Type* baseType = actualType->getField("valType")->type;
    if (canInstantiate(baseType, formalType))
      return baseType;
  }

  return NULL;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void ResolutionCandidate::resolveTypedefedArgTypes() {
  for_formals(formal, fn) {
    INT_ASSERT(formal->type);

    if (formal->type == dtUnknown) {
      if (BlockStmt* block = formal->typeExpr) {
        if (SymExpr* se = toSymExpr(block->body.first())) {
          if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE) == true) {
            Type* type = resolveTypeAlias(se);

            INT_ASSERT(type);

            formal->type = type;
          }
        }
      }
    }
  }
}

static AggregateType* getActualType(ResolutionCandidate* rc, int idx) {
  Symbol* sym = rc->formalIdxToActual[idx];

  return (sym != NULL) ? toAggregateType(sym->getValType()) : NULL;
}

//
// Something looks like a copy initializer if it has three actuals (a
// gMethodToken and two others), and if the last two actuals' types match in
// one of the following ways:
// 1) They are identical
// 2) They are instantiated from the same root type
//
// This case is checked to help avoid confusing users. If the user writes:
//   var a = new MyClass(b);
// where ``b`` is also an instantiation of ``MyClass``, most users would expect
// a copy initializer to be called. If there is not a copy-initializer for b's
// type, but there is an initializer for b's promotion type, then without this
// check the compiler would promote the expression. This would result in an
// array of ``MyClass`` elements, which could be very confusing.
//
// See GitHub Issue #6019.
//
static bool looksLikeCopyInit(ResolutionCandidate* rc) {
  bool retval = false;

  if ((rc->fn->isCopyInit() || rc->fn->isInitializer()) &&
      rc->formalIdxToActual.size() == 3) {
    // First formal/actual is gMethodToken
    AggregateType* base  = getActualType(rc, 1);
    AggregateType* other = getActualType(rc, 2);

    if (base != NULL && other != NULL) {
      if (base == other) {
        retval = true;

      } else {
        AggregateType* baseRoot  = base->getRootInstantiation();
        AggregateType* otherRoot = other->getRootInstantiation();

        retval = (baseRoot == otherRoot) ? true : false;
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

bool ResolutionCandidate::checkResolveFormalsWhereClauses(CallInfo& info,
                                                    VisibilityInfo* visInfo) {
  int coindex = -1;

  /*
   * A derived generic type will use the type of its parent,
   * and expects this to be instantiated before it is.
   */
  resolveSignature(fn);

  bool isCopyInit = looksLikeCopyInit(this);
  bool isInitCopy = fn->hasFlag(FLAG_INIT_COPY_FN);

  for_formals(formal, fn) {
    if (Symbol* actual = formalIdxToActual[++coindex]) {
      bool actualIsTypeAlias = actual->hasFlag(FLAG_TYPE_VARIABLE);
      bool formalIsTypeAlias = formal->hasFlag(FLAG_TYPE_VARIABLE);

      bool formalIsParam     = formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
                               formal->intent == INTENT_PARAM;
      bool isInitThis        = (fn->isInitializer() || fn->isCopyInit()) &&
                               formal->hasFlag(FLAG_ARG_THIS);
      bool isNewTypeArg      = strcmp(fn->name,"_new") == 0 &&
                               coindex == 0; // first formal/actual

      bool promotes          = false;

      if (isInitCopy && isString(actual) && formal->getValType() == dtStringC) {
        // Do not allow an initCopy of a string to find the c_string initCopy,
        // which is considered first because it is not compiler generated.
        failingArgument = actual;
        reason = RESOLUTION_CANDIDATE_OTHER;
        return false;

      } else if (actualIsTypeAlias != formalIsTypeAlias) {
        failingArgument = actual;
        reason = RESOLUTION_CANDIDATE_NOT_TYPE;
        return false;

      } else if (formalIsTypeAlias &&
                 !shouldAllowCoercions(actual, formal) &&
                 actual->getValType() != formal->getValType()) {
        // coercions should not generally be allowed for type variables
        failingArgument = actual;
        reason = classifyTypeMismatch(actual->getValType(),
                                      formal->getValType());
        return false;


      } else if (formal->originalIntent != INTENT_OUT &&
                 (actual->getValType() == dtSplitInitType ||
                  (formalIsTypeAlias == false && isInitThis == false &&
                   actual->getValType()->symbol->hasFlag(FLAG_GENERIC)))) {
        failingArgument = actual;
        reason = RESOLUTION_CANDIDATE_ACTUAL_TYPE_NOT_ESTABLISHED;
        return false;

      // MPF TODO: one day, this should use actual/formal getValType,
      // and canCoerce should be adjusted to consider intents,
      // rather than depending on ref types at this stage in compilation.
      } else if (canDispatch(actual->type,
                             actual,
                             formal->type,
                             formal,
                             fn,
                             &promotes,
                             NULL,
                             formalIsParam) == false &&
                 formal->originalIntent != INTENT_OUT) {
        failingArgument = actual;
        reason = classifyTypeMismatch(actual->type, formal->type);
        return false;

      } else if (isInitThis || isNewTypeArg) {
        AggregateType* ft = toAggregateType(formal->getValType());
        AggregateType* at = toAggregateType(actual->getValType());

        // Do not allow dispatch for 'this' formal with initializers
        //
        // Types may only mismatch if the formal's type is an instantiation of
        // the actual's type.
        if (ft != at && ft->isInstantiatedFrom(at) == false) {
          failingArgument = actual;
          reason = RESOLUTION_CANDIDATE_OTHER;
          return false;
        }
      } else if (promotes && isCopyInit) {
        failingArgument = actual;
        reason = RESOLUTION_CANDIDATE_OTHER;
        return false;
      }
    }
  }

  if (evaluateWhereClause(fn) == false) {
    failingArgument = NULL;
    if (fn->hasFlag(FLAG_COMPILER_ADDED_WHERE))
      // RESOLUTION_CANDIDATE_WHERE_FAILED is not helpful to the user
      // if they did not write the where clause.
      reason = RESOLUTION_CANDIDATE_IMPLICIT_WHERE_FAILED;
    else
      reason = RESOLUTION_CANDIDATE_WHERE_FAILED;
    return false;
  }

  return true;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool ResolutionCandidate::checkGenericFormals(Expr* ctx) {
  int coindex = 0;

  for_formals(formal, fn) {
    if (Symbol* actual = formalIdxToActual[coindex]) {
      bool actualIsTypeAlias = actual->hasFlag(FLAG_TYPE_VARIABLE);
      bool formalIsTypeAlias = formal->hasFlag(FLAG_TYPE_VARIABLE);
      bool formalIsParam     = formal->intent == INTENT_PARAM;
      bool isInitThis        = (fn->isInitializer() || fn->isCopyInit()) &&
                               formal->hasFlag(FLAG_ARG_THIS);

      // type independent checks
      if (actualIsTypeAlias != formalIsTypeAlias) {
        failingArgument = actual;
        reason = RESOLUTION_CANDIDATE_NOT_TYPE;
        return false;

      }
      if (formalIsParam && !actual->isParameter()) {
        failingArgument = actual;
        reason = RESOLUTION_CANDIDATE_NOT_PARAM;
        return false;
      }

      if (ConstrainedType* actCT = toConstrainedType(actual->getValType())) {
        if (actCT == formal->type) {
          ; // ok: a CG actual matches against the same type
        } else if (cgActualCanMatch(fn, formal->getValType(), actCT)) {
          ; // other matching cases
        } else {
          // cannot pass a CG actual to an unconstrained-generic formal
          failingArgument = actual;
          reason = RESOLUTION_CANDIDATE_INTERFACE_FORMAL_AS_ACTUAL;
          return false;
        }
      }

      if (formalIsTypeAlias == false &&
          isInitThis == false &&
          formal->originalIntent != INTENT_OUT &&
          (actual->type == dtSplitInitType ||
           actual->type->symbol->hasFlag(FLAG_GENERIC))) {
        failingArgument = actual;
        reason = RESOLUTION_CANDIDATE_ACTUAL_TYPE_NOT_ESTABLISHED;
        return false;
      }

      // type dependent checks
      if (formal->type != dtUnknown && formal->originalIntent != INTENT_OUT) {
        if (formal->type->symbol->hasFlag(FLAG_GENERIC)) {
          Type* t = getInstantiationType(actual, formal, ctx);
          if (t == NULL) {
            failingArgument = actual;
            reason = classifyTypeMismatch(actual->type, formal->type);
            return false;
          }

        } else if (cgFormalCanMatch(fn, formal->type)) {
          // acceptable

        } else {
          bool formalIsParam = formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
                               formal->intent == INTENT_PARAM;

          // MPF TODO: one day, this should use actual/formal getValType,
          // and canCoerce should be adjusted to consider intents,
          // rather than depending on ref types at this stage in compilation.
          if (canDispatch(actual->type,
                          actual,
                          formal->type,
                          formal,
                          fn,
                          NULL,
                          NULL,
                          formalIsParam) == false) {
            failingArgument = actual;
            reason = classifyTypeMismatch(actual->type, formal->type);
            return false;
          }
        }
      }
    }

    coindex++;
  }

  return true;
}

static bool isNumericType(Type* t) {
  return is_bool_type(t) ||
         is_int_type(t) ||
         is_uint_type(t) ||
         is_real_type(t) ||
         is_imag_type(t) ||
         is_complex_type(t);
}

static bool isClassLikeOrPtrOrManaged(Type* t) {
  return isClassLikeOrPtr(t) || isClassLikeOrManaged(t);
}

static ResolutionCandidateFailureReason
classifyTypeMismatch(Type* actualType, Type* formalType) {
  if (actualType == formalType)
    return RESOLUTION_CANDIDATE_MATCH;

  // From this point, don't consider references
  actualType = actualType->getValType();
  formalType = formalType->getValType();
  if (actualType == formalType)
    return RESOLUTION_CANDIDATE_TYPE_RELATED;

  if (canonicalClassType(actualType) == canonicalClassType(formalType))
    return RESOLUTION_CANDIDATE_TYPE_RELATED;

  if ((is_bool_type   (actualType) && is_bool_type   (formalType)) ||
      (is_int_type    (actualType) && is_int_type    (formalType)) ||
      (is_uint_type   (actualType) && is_uint_type   (formalType)) ||
      (is_real_type   (actualType) && is_real_type   (formalType)) ||
      (is_imag_type   (actualType) && is_imag_type   (formalType)) ||
      (is_complex_type(actualType) && is_complex_type(formalType)))
    return RESOLUTION_CANDIDATE_TYPE_RELATED;

  if (isNumericType(actualType) && isNumericType(formalType))
    return RESOLUTION_CANDIDATE_TYPE_SAME_CATEGORY;

  if (isClassLikeOrPtrOrManaged(actualType) &&
      isClassLikeOrPtrOrManaged(formalType))
    return RESOLUTION_CANDIDATE_TYPE_SAME_CATEGORY;

  return RESOLUTION_CANDIDATE_UNRELATED_TYPE;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void explainCandidateRejection(CallInfo& info, FnSymbol* fn) {
  ResolutionCandidate c(fn);

  c.isApplicable(info, NULL);

  USR_PRINT(fn, "this candidate did not match: %s", toString(fn));

  Symbol* failingActual = NULL;
  bool failingActualIsReceiver = false;
  int failingActualUserIndex = 0; // user index, e.g. skips method token
  int failingActualRealIndex = 0; // works for CallInfo.actuals
  ArgSymbol* failingFormal = NULL;
  bool callIsMethod = false;
  bool fnIsMethod = false;

  int userIndex = 1;
  for (int i = 0; i < info.actuals.n; i++) {
    Symbol* actual = info.actuals.v[i];
    Symbol* prevActual = (i>0)?(info.actuals.v[i-1]):(NULL);

    if (actual == c.failingArgument) {
      failingActual = c.failingArgument;
      if (prevActual && prevActual->type == dtMethodToken)
        failingActualIsReceiver = true;
      else
        failingActualUserIndex = userIndex;
      failingActualRealIndex = i;
      failingFormal = c.actualIdxToFormal[i];
    }

    if (actual->type == dtMethodToken)
      callIsMethod = true;

    // Don't count method token or this when reporting user errors
    if (!(actual->type == dtMethodToken ||
          (prevActual && prevActual->type == dtMethodToken)))
      userIndex++;
  }
  int userActualsCount = userIndex-1;

  userIndex = 1;
  ArgSymbol* prevFormal = NULL;
  for_formals(formal, fn) {
    if (formal == c.failingArgument) {
      failingFormal = formal;
      // This only happens when no actual exists for this formal,
      // so no point in trying to find one.
    }

    if (formal->type == dtMethodToken)
      fnIsMethod = true;

    if (!(formal->type == dtMethodToken ||
          (prevFormal != NULL && prevFormal->type == dtMethodToken)))
      userIndex++;

    prevFormal = formal;
  }
  int userFormalsCount = userIndex-1;


  CallExpr* call = info.call;

  const char* failingActualDesc = NULL;

  if (failingActualIsReceiver)
    failingActualDesc = astr("method call receiver");
  else
    failingActualDesc = astr("actual argument #",
                             istr(failingActualUserIndex));

  if (fnIsMethod && !callIsMethod) {
    USR_PRINT(call, "because call is not written as a method call");
    USR_PRINT(fn, "but candidate function is a method");
    return;
  }
  if (callIsMethod && !fnIsMethod) {
    USR_PRINT(call, "because call is written as a method call");
    USR_PRINT(fn, "but candidate function is not a method");
    return;
  }

  switch (c.reason) {
    case RESOLUTION_CANDIDATE_TYPE_RELATED:
    case RESOLUTION_CANDIDATE_TYPE_SAME_CATEGORY:
    case RESOLUTION_CANDIDATE_UNRELATED_TYPE:
      USR_PRINT(call, "because %s with type '%s'",
                    failingActualDesc,
                    toString(failingActual->getValType()));
      USR_PRINT(failingFormal, "is passed to formal '%s'",
                               toString(failingFormal, true));
      if (isNilableClassType(failingActual->getValType()) &&
          isNonNilableClassType(failingFormal->getValType()))
        USR_PRINT(call, "try to apply the postfix ! operator to %s",
                  failingActualDesc);
      break;
    case RESOLUTION_CANDIDATE_WHERE_FAILED:
      USR_PRINT(fn, "because where clause evaluated to false");
      break;
    case RESOLUTION_CANDIDATE_IMPLICIT_WHERE_FAILED:
      USR_PRINT(fn, "because an argument was incompatible");
      break;
    case RESOLUTION_CANDIDATE_INTERFACE_CONSTRAINTS_NOT_SATISFIED:
      // CG TODO: show more information about the constrains
      USR_PRINT(fn, "because interface constraint(s) were not satisfied");
      break;
    case RESOLUTION_CANDIDATE_INTERFACE_FORMAL_AS_ACTUAL:
      USR_PRINT(call, "because %s of an interface type %s",
                    failingActualDesc,
                    toString(failingActual->getValType()));
      USR_PRINT(failingFormal, "is passed to a generic formal");
      USR_PRINT(failingFormal, "passing a constrained-generic formal"
                " to a non-constrained-generic function is not allowed");
      break;
    case RESOLUTION_CANDIDATE_NOT_PARAM:
      USR_PRINT(call, "because non-param %s", failingActualDesc);
      USR_PRINT(failingFormal, "is passed to param formal '%s'",
                               toString(failingFormal, true));
      break;
    case RESOLUTION_CANDIDATE_NOT_TYPE:
      if (failingFormal->hasFlag(FLAG_TYPE_VARIABLE)) {
        USR_PRINT(call, "because non-type %s", failingActualDesc);
        USR_PRINT(failingFormal, "is passed to formal '%s'",
                                 toString(failingFormal, true));
      } else {
        USR_PRINT(call, "because %s is a type", failingActualDesc);
        if (failingFormal->hasFlag(FLAG_EXPANDED_VARARGS)) {
          USR_PRINT(fn, "but is passed to non-type varargs formal '%s'",
                    failingFormal->demungeVarArgName().c_str());
        } else {
          USR_PRINT(fn, "but is passed to non-type formal '%s'",
                    toString(failingFormal, true));
        }
      }
      break;
    case RESOLUTION_CANDIDATE_ACTUAL_TYPE_NOT_ESTABLISHED:
      if (failingActual->getValType() == dtSplitInitType) {
        splitInitMissingTypeError(failingActual, call, /*unresolved*/ true);
      } else {
        USR_PRINT(call,
                  "actual argument '%s' has generic type '%s'",
                  toString(failingActual, false),
                  toString(failingActual->getValType()));
        printUndecoratedClassTypeNote(call, failingActual->getValType());
      }
      break;
    case RESOLUTION_CANDIDATE_TOO_MANY_ARGUMENTS:
      USR_PRINT(call, "because call includes %i argument%s",
                userActualsCount, ((userActualsCount == 1) ? "" : "s"));
      USR_PRINT(fn, "but function can only accept %i argument%s",
                userFormalsCount, ((userFormalsCount == 1) ? "" : "s"));
      break;
    case RESOLUTION_CANDIDATE_TOO_FEW_ARGUMENTS:
      USR_PRINT(call, "because call does not supply enough arguments");
      USR_PRINT(failingFormal, "it is missing a value for formal '%s'",
                               toString(failingFormal, true));
      break;
    case RESOLUTION_CANDIDATE_NO_NAMED_ARGUMENT:
      {
        const char* name = info.actualNames.v[failingActualRealIndex];

        if (name != NULL) {
          USR_PRINT(call, "because call uses named argument %s", name);
          USR_PRINT(fn, "but function contains no formal named %s", name);
        }
      }
      break;
    case RESOLUTION_CANDIDATE_OTHER:
    case RESOLUTION_CANDIDATE_MATCH:
      if (c.reason == RESOLUTION_CANDIDATE_MATCH &&
          call->methodTag == true                &&
          ! fn->hasFlag(FLAG_NO_PARENS)          ) {
        USR_PRINT(call, "because call is written without parentheses");
        USR_PRINT(fn, "but candidate function has parentheses");
      }
      // Print nothing else
      break;
    // No default -> compiler warning
  }
}

void explainGatherCandidate(const CallInfo&            info,
                            Vec<ResolutionCandidate*>& candidates) {
  CallExpr* call = info.call;

  if ((explainCallLine != 1 && explainCallMatch(info.call) == true) ||
      call->id == explainCallID) {
    if (candidates.n == 0) {
      USR_PRINT(info.call, "no candidates found");

    } else {
      bool first = true;

      forv_Vec(ResolutionCandidate*, candidate, candidates) {
        USR_PRINT(candidate->fn,
                  "%s %s",
                  first ? "candidates are:" : "               ",
                  toString(candidate->fn));

        first = false;
      }
    }
  }
}

bool failedCandidateIsBetterMatch(ResolutionCandidate* a,
                                  ResolutionCandidate* b) {
  return a->reason < b->reason;
}

