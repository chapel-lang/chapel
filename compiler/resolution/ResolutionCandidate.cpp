/*
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
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

static ResolutionCandidateFailureReason
classifyTypeMismatch(Type* actualType, Type* formalType);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

ResolutionCandidate::ResolutionCandidate(FnSymbol* function) {
  fn = function;
  failingArgument = NULL;
  reason = RESOLUTION_CANDIDATE_MATCH;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool ResolutionCandidate::isApplicable(CallInfo& info) {
  bool retval = false;

  if (fn->hasFlag(FLAG_GENERIC) == false) {
    retval = isApplicableConcrete(info);
  } else {
    retval = isApplicableGeneric (info);
  }

  if (retval && fn->retExprType != NULL && fn->retType == dtUnknown) {
    resolveSpecifiedReturnType(fn);
  }

  return retval;
}

bool ResolutionCandidate::isApplicableConcrete(CallInfo& info) {

  fn = expandIfVarArgs(fn, info);
  if (fn == NULL) {
    reason = RESOLUTION_CANDIDATE_OTHER;
    return false;
  }

  resolveTypedefedArgTypes();

  if (computeAlignment(info) == false)
    return false;

  return checkResolveFormalsWhereClauses(info);
}

void ResolutionCandidate::resolveTypeConstructor(CallInfo& info) {
  SET_LINENO(fn);

  // Ignore tuple constructors; they were generated
  // with their type constructors.
  if (fn->hasFlag(FLAG_PARTIAL_TUPLE) == false) {
    AggregateType* at = toAggregateType(fn->_this->type);
    INT_ASSERT(at->typeConstructor != NULL);

    if (at->typeConstructor->isResolved()) return;

    CallExpr* typeConstructorCall = new CallExpr(at->typeConstructor);

    for_formals(formal, fn) {
      if (fn->_this->type->symbol->hasFlag(FLAG_TUPLE)) {
        if (formal->instantiatedFrom != NULL) {
          typeConstructorCall->insertAtTail(formal->type->symbol);

        } else if (formal->hasFlag(FLAG_INSTANTIATED_PARAM)) {
          typeConstructorCall->insertAtTail(paramMap.get(formal));
        }

      } else {
        if (formal->type == dtMethodToken) {
          typeConstructorCall->insertAtTail(formal);

        } else if (formal->instantiatedFrom != NULL) {
          SymExpr*   se = new SymExpr(formal->type->symbol);
          NamedExpr* ne = new NamedExpr(formal->name, se);

          typeConstructorCall->insertAtTail(ne);

        } else if (formal->hasFlag(FLAG_INSTANTIATED_PARAM)) {
          SymExpr*   se = new SymExpr(paramMap.get(formal));
          NamedExpr* ne = new NamedExpr(formal->name, se);

          typeConstructorCall->insertAtTail(ne);
        }
      }
    }

    info.call->insertBefore(typeConstructorCall);

    // If instead we call resolveCallAndCallee(typeConstructorCall)
    // then the line number reported in an error would change
    // e.g.: domains/deitz/test_generic_class_of_sparse_domain
    // or:   classes/diten/multipledestructor
    resolveCall(typeConstructorCall);

    INT_ASSERT(typeConstructorCall->isResolved());

    resolveFunction(typeConstructorCall->resolvedFunction());

    fn->_this->type = typeConstructorCall->resolvedFunction()->retType;

    typeConstructorCall->remove();
  }
}

bool ResolutionCandidate::isApplicableGeneric(CallInfo& info) {

  fn = expandIfVarArgs(fn, info);
  if (fn == NULL) {
    reason = RESOLUTION_CANDIDATE_OTHER;
    return false;
  }

  if (computeAlignment(info) == false)
    return false;

  if (checkGenericFormals() == false)
    return false;

  // Compute the param/type substitutions for generic arguments.
  if (computeSubstitutions() <= 0) {
    reason = RESOLUTION_CANDIDATE_OTHER;
    return false;
  }

  /*
   * Instantiate enough of the generic to get through the rest of the
   * filtering and disambiguation processes.
   */
  fn = instantiateSignature(fn, substitutions, info.call);

  if (fn == NULL) {
    reason = RESOLUTION_CANDIDATE_OTHER;
    return false;
  }

  return isApplicable(info);
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
        if (strcmp(info.actualNames.v[i], formal->name) == 0) {
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

  for (int i = 0; i < info.actuals.n; i++) {
    if (info.actualNames.v[i] == NULL) {
      bool match = false;

      while (formal != NULL) {
        if (formal->variableExpr) {
          return (fn->hasFlag(FLAG_GENERIC)) ? true : false;
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
        if (fn->hasFlag(FLAG_GENERIC) == false) {
          failingArgument = info.actuals.v[i];
          reason = RESOLUTION_CANDIDATE_TOO_MANY_ARGUMENTS;
          return false;
        } else if (fn->hasFlag(FLAG_INIT_TUPLE) == false &&
                   isTupleTypeConstructor(fn)   == false) {
          failingArgument = info.actuals.v[i];
          reason = RESOLUTION_CANDIDATE_TOO_MANY_ARGUMENTS;
          return false;
        }
      }
    }
  }

  // Make sure that any remaining formals are matched by name
  // or have a default value.
  while (formal) {
    if (formalIdxToActual[j] == NULL && formal->defaultExpr == NULL) {
      failingArgument = formal;
      reason = RESOLUTION_CANDIDATE_TOO_FEW_ARGUMENTS;
      return false;
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

static Type* getBasicInstantiationType(Type* actualType, Type* formalType);

int ResolutionCandidate::computeSubstitutions() {
  bool exitForDefault = false;

  substitutions.clear();

  for (int i = 1; i <= fn->numFormals() && exitForDefault == false; i++) {
    ArgSymbol* formal = fn->getFormal(i);

    if (formal->intent                              == INTENT_PARAM ||
        formal->type->symbol->hasFlag(FLAG_GENERIC) == true) {

      if (Symbol* actual = formalIdxToActual[i - 1]) {
        computeSubstitution(formal, actual);

      } else if (formal->defaultExpr != NULL) {
        if (substitutions.n == 0) {
          computeSubstitution(formal);

        } else {
          // break because default expression may reference generic
          // arguments earlier in formal list; make those substitutions
          // first (test/classes/bradc/paramInClass/weirdParamInit4)
          exitForDefault = true;
        }
      }
    }
  }

  return substitutions.n;
}

bool ResolutionCandidate::verifyGenericFormal(ArgSymbol* formal) const {
  bool retval = true;

  if (formal->intent                      != INTENT_PARAM &&
      formal->hasFlag(FLAG_TYPE_VARIABLE) == false        &&
      formal->type                        != dtAny) {
    if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
      retval = false;

    } else if (fn->isDefaultInit() &&
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
                                              Symbol*    actual) {
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
        formal->hasFlag(FLAG_DELAY_GENERIC_EXPANSION) == true) {
      // If the "this" arg is generic, we're resolving an initializer, and
      // the actual being passed is also still generic, don't count this as
      // a substitution.  Otherwise, we'll end up in an infinite loop if
      // one of the later generic args has a defaultExpr, as we will always
      // count the this arg as a substitution and so always approach the
      // generic arg with a defaultExpr as though a substitution was going
      // to take place.

    } else if (Type* type = getInstantiationType(actual->type, formal->type)) {
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

void ResolutionCandidate::computeSubstitution(ArgSymbol* formal) {
  resolveBlockStmt(formal->defaultExpr);

  if (formal->intent == INTENT_PARAM) {
    if (SymExpr* se = toSymExpr(formal->defaultExpr->body.tail)) {
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
    Type* defaultType = formal->defaultExpr->body.tail->typeInfo();

    if (defaultType == dtTypeDefaultToken) {
      substitutions.put(formal, dtTypeDefaultToken->symbol);

    } else if (Type* type = getInstantiationType(defaultType, formal->type)) {
      substitutions.put(formal, type->symbol);
    }
  }
}

Type* getInstantiationType(Type* actualType, Type* formalType) {
  Type* ret = getBasicInstantiationType(actualType, formalType);

  // If that didn't work, try it again with the value type.
  if (ret == NULL) {
    if (Type* vt = actualType->getValType()) {
      ret = getBasicInstantiationType(vt, formalType);
    }
  }

  // If we still don't have an instantiation type, try it again
  // with the promotion type.
  if (ret == NULL) {
    if (Type* st = actualType->getValType()->scalarPromotionType) {
      ret = getBasicInstantiationType(st->getValType(), formalType);
    }
  }


  // Now, if formalType is a generic parent type to actualType,
  // we should instantiate the parent actual type
  if (AggregateType* at = toAggregateType(ret)) {
    if (at->instantiatedFrom                      != NULL  &&
        formalType->symbol->hasFlag(FLAG_GENERIC) == true) {
      if (Type* concrete = getConcreteParentForGenericFormal(at, formalType)) {
        ret = concrete;
      }
    }
  }

  return ret;
}

static Type* getBasicInstantiationType(Type* actualType, Type* formalType) {
  if (canInstantiate(actualType, formalType)) {
    return actualType;
  }

  if (AggregateType* at = toAggregateType(actualType)) {
    if (at->isClass() && isClassLike(at)) {
      // non-nilable borrowed can coerce to nilable borrowed
      Type* actualNilableBorrowed =
        at->getDecoratedClass(CLASS_TYPE_BORROWED_NILABLE);
      if (canInstantiate(actualNilableBorrowed, formalType))
        return actualNilableBorrowed;
    }
  }

  if (DecoratedClassType* actualDt = toDecoratedClassType(actualType)) {
    ClassTypeDecorator actualDecorator = actualDt->getDecorator();
    AggregateType* actualC = actualDt->getCanonicalClass();

    // No coercions for nilable borrowed

    // nilable unmanaged can coerce to nilable borrowed
    if (actualDecorator == CLASS_TYPE_UNMANAGED_NILABLE) {
      Type* actualNilableBorrowed =
        actualC->getDecoratedClass(CLASS_TYPE_BORROWED_NILABLE);
      if (canInstantiate(actualNilableBorrowed, formalType))
        return actualNilableBorrowed;
    }

    // non-nilable unmanaged can coerce to borrowed,borrowed?,unmanaged?
    if (actualDecorator == CLASS_TYPE_UNMANAGED) {
      Type* actualNilableBorrowed =
        actualC->getDecoratedClass(CLASS_TYPE_BORROWED_NILABLE);
      if (canInstantiate(actualNilableBorrowed, formalType))
        return actualNilableBorrowed;
      Type* actualBorrowed =
        actualC->getDecoratedClass(CLASS_TYPE_BORROWED);
      if (canInstantiate(actualBorrowed, formalType))
        return actualBorrowed;
      Type* actualNilableUnmanaged =
        actualC->getDecoratedClass(CLASS_TYPE_UNMANAGED_NILABLE);
      if (canInstantiate(actualNilableUnmanaged, formalType))
        return actualNilableUnmanaged;
    }
  }

  if (isManagedPtrType(actualType)) {
    Type* actualBaseType = getManagedPtrBorrowType(actualType);
    // non-nilable owned can coerce to non-nilable borrowed
    if (canInstantiate(actualBaseType, formalType))
      return actualBaseType;
    // non-nilable owned can coerce to nilable borrowed
    if (AggregateType* at = toAggregateType(actualBaseType)) {
      Type* actualNilableBorrowed =
        at->getDecoratedClass(CLASS_TYPE_BORROWED_NILABLE);
      if (canInstantiate(actualNilableBorrowed, formalType))
        return actualBaseType;
    }

    // TODO: handle owned -> owned? and shared -> shared?
  }

  if (actualType == dtNil) {
    if (formalType == dtBorrowedNilable || formalType == dtUnmanagedNilable)
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

bool ResolutionCandidate::checkResolveFormalsWhereClauses(CallInfo& info) {
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
                 !isDispatchParent(actual->getValType(),
                                   formal->getValType()) &&
                 actual->getValType() != formal->getValType()) {
        // coercions should not be allowed for type variables
        failingArgument = actual;
        reason = classifyTypeMismatch(actual->getValType(),
                                      formal->getValType());
        return false;

      } else if (canDispatch(actual->type,
                             actual,
                             formal->type,
                             formal,
                             fn,
                             &promotes,
                             NULL,
                             formalIsParam) == false) {
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

bool ResolutionCandidate::checkGenericFormals() {
  int coindex = 0;

  for_formals(formal, fn) {
    if (formal->type != dtUnknown) {
      if (Symbol* actual = formalIdxToActual[coindex]) {
        bool actualIsTypeAlias = actual->hasFlag(FLAG_TYPE_VARIABLE);
        bool formalIsTypeAlias = formal->hasFlag(FLAG_TYPE_VARIABLE);

        bool formalIsParam = formal->intent == INTENT_PARAM;

        if (actualIsTypeAlias != formalIsTypeAlias) {
          failingArgument = actual;
          reason = RESOLUTION_CANDIDATE_NOT_TYPE;
          return false;

        } else if (formalIsParam && !actual->isParameter()) {
          failingArgument = actual;
          reason = RESOLUTION_CANDIDATE_NOT_PARAM;
          return false;

        } else if (formal->type->symbol->hasFlag(FLAG_GENERIC)) {
          Type* t = getInstantiationType(actual->type, formal->type);
          if (t == NULL) {
            failingArgument = actual;
            reason = classifyTypeMismatch(actual->type, formal->type);
            return false;
          }

        } else {
          bool formalIsParam = formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
                               formal->intent == INTENT_PARAM;

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

static bool isClassLikeOrManaged(Type* t) {
  return isClassLikeOrPtr(t) || isManagedPtrType(t) ||
         t == dtBorrowed || t == dtBorrowedNilable ||
         t == dtUnmanaged || t == dtUnmanagedNilable;
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

  if (isClassLikeOrManaged(actualType) && isClassLikeOrManaged(formalType))
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

  c.isApplicable(info);

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
    failingActualDesc = astr("call actual argument #",
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
      USR_PRINT(call, "because %s with type %s",
                    failingActualDesc,
                    toString(failingActual->getValType()));
      USR_PRINT(failingFormal, "is passed to formal '%s'",
                               toString(failingFormal));
      break;
    case RESOLUTION_CANDIDATE_WHERE_FAILED:
      USR_PRINT(fn, "because where clause evaluated to false");
      break;
    case RESOLUTION_CANDIDATE_NOT_PARAM:
      USR_PRINT(call, "because non-param %s", failingActualDesc);
      USR_PRINT(failingFormal, "is passed to param formal '%s'",
                               toString(failingFormal));
      break;
    case RESOLUTION_CANDIDATE_NOT_TYPE:
      if (failingFormal->hasFlag(FLAG_TYPE_VARIABLE)) {
        USR_PRINT(call, "because non-type %s", failingActualDesc);
        USR_PRINT(failingFormal, "is passed to formal '%s'",
                                 toString(failingFormal));
      } else {
        USR_PRINT(call, "because type %s", failingActualDesc);
        USR_PRINT(fn, "is passed to non-type formal '%s'",
                      toString(failingFormal));
      }
      break;
    case RESOLUTION_CANDIDATE_TOO_MANY_ARGUMENTS:
      USR_PRINT(call, "because call includes %i arguments",
                      userActualsCount);
      USR_PRINT(fn, "but function can only accept %i arguments",
                    userFormalsCount);
      break;
    case RESOLUTION_CANDIDATE_TOO_FEW_ARGUMENTS:
      USR_PRINT(call, "because call does not supply enough arguments");
      USR_PRINT(failingFormal, "it is missing a value for formal '%s'",
                               toString(failingFormal));
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

