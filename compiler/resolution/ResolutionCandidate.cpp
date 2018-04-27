/*
 * Copyright 2004-2018 Cray Inc.
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
#include "driver.h"
#include "expandVarArgs.h"
#include "expr.h"
#include "UnmanagedClassType.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

ResolutionCandidate::ResolutionCandidate(FnSymbol* function) {
  fn = function;
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
  bool retval = false;

  fn = expandIfVarArgs(fn, info);

  if (fn != NULL) {
    resolveTypedefedArgTypes();

    if (computeAlignment(info) == true) {
      // Ensure that type constructor is resolved before other constructors.
      if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) == true) {
        resolveTypeConstructor(info);
      }

      retval = checkResolveFormalsWhereClauses(info);
    }
  }

  return retval;
}

void ResolutionCandidate::resolveTypeConstructor(CallInfo& info) {
  SET_LINENO(fn);

  // Ignore tuple constructors; they were generated
  // with their type constructors.
  if (fn->hasFlag(FLAG_PARTIAL_TUPLE) == false) {
    CallExpr* typeConstructorCall = new CallExpr(astr("_type", fn->name));

    for_formals(formal, fn) {
      if (formal->hasFlag(FLAG_IS_MEME) == false) {
        if (fn->_this->type->symbol->hasFlag(FLAG_TUPLE)) {
          if (formal->instantiatedFrom != NULL) {
            typeConstructorCall->insertAtTail(formal->type->symbol);

          } else if (formal->hasFlag(FLAG_INSTANTIATED_PARAM)) {
            typeConstructorCall->insertAtTail(paramMap.get(formal));
          }

        } else {
          if (strcmp(formal->name, "outer") == 0 ||
              formal->type                  == dtMethodToken) {
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
  bool retval = false;

  fn = expandIfVarArgs(fn, info);

  if (fn                     != NULL &&
      computeAlignment(info) == true &&
     checkGenericFormals()   == true) {
    // Compute the param/type substitutions for generic arguments.
    if (computeSubstitutions() > 0) {
      /*
       * Instantiate enough of the generic to get through the rest of the
       * filtering and disambiguation processes.
       */
      fn = instantiateSignature(fn, substitutions, info.call);

      if (fn != NULL) {
        retval = isApplicable(info);
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
          return false;
        } else if (fn->hasFlag(FLAG_INIT_TUPLE) == false &&
                   isTupleTypeConstructor(fn)   == false) {
          return false;
        }
      }
    }
  }

  // Make sure that any remaining formals are matched by name
  // or have a default value.
  while (formal) {
    if (formalIdxToActual[j] == NULL && formal->defaultExpr == NULL) {
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

static Type* getInstantiationType(Type* actualType, Type* formalType);

static Type* getBasicInstantiationType(Type* actualType, Type* formalType);

int ResolutionCandidate::computeSubstitutions() {
  bool exitForDefault = false;

  substitutions.clear();

  for (int i = 1; i <= fn->numFormals() && exitForDefault == false; i++) {
    ArgSymbol* formal = fn->getFormal(i);

    if (formal->intent                              == INTENT_PARAM ||
        formal->type->symbol->hasFlag(FLAG_GENERIC) == true) {
      if (verifyGenericFormal(formal) == false) {
        USR_FATAL(formal, "invalid generic type specification on class field");

      } else if (Symbol* actual = formalIdxToActual[i - 1]) {
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
    if (strcmp(formal->name, "outer") != 0     &&
        formal->hasFlag(FLAG_IS_MEME) == false &&
        (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) == true ||
         fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)    == true)) {
      retval = false;

    } else if (fn->isMethod()                       == true  &&
               strcmp(fn->name, "init")             == 0     &&
               fn->hasFlag(FLAG_COMPILER_GENERATED) == true  &&
               fn->hasFlag(FLAG_DEFAULT_COPY_INIT)  == false &&
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

static Type* getInstantiationType(Type* actualType, Type* formalType) {
  Type* ret = getBasicInstantiationType(actualType, formalType);

  // If that didn't work, try it again with the value type.
  if (ret == NULL) {
    if (Type* vt = actualType->getValType()) {
      ret = getBasicInstantiationType(vt, formalType);
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

  if (Type* st = actualType->scalarPromotionType) {
    if (canInstantiate(st, formalType))
      return st;
  }

  if (UnmanagedClassType* actualMt = toUnmanagedClassType(actualType)) {
    AggregateType* actualC = actualMt->getCanonicalClass();
    if (canInstantiate(actualC, formalType))
      return actualC;
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

  if (rc->fn->isInitializer() && rc->formalIdxToActual.size() == 3) {
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

static bool isCandidateInit(ResolutionCandidate* res, CallInfo& info) {
  bool retval = false;

  AggregateType* ft = toAggregateType(res->fn->_this->getValType());
  AggregateType* at = toAggregateType(info.call->get(2)->getValType());

  if (ft == at) {
    retval = true;
  } else if (ft->getRootInstantiation() == at->getRootInstantiation()) {
    retval = true;
  }

  return retval;
}

static bool isCandidateNew(ResolutionCandidate* res, CallInfo& info) {
  bool retval = false;

  AggregateType* ft = toAggregateType(res->fn->getFormal(1)->getValType());
  AggregateType* at = toAggregateType(info.call->get(1)->getValType());

  if (ft == at) {
    retval = true;
  } else if (ft->getRootInstantiation() == at->getRootInstantiation()) {
    retval = true;
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

  // Exclude initializers on other types before we attempt to resolve the
  // signature.
  //
  // TODO: Expand this check for all methods
  if (fn->isInitializer() && isCandidateInit(this, info) == false) {
    return false;
  } else if (strcmp(fn->name, "_new") == 0 &&
             isCandidateNew(this, info) == false) {
    return false;
  }

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
      bool isInitThis        = fn->isInitializer() &&
                               formal->hasFlag(FLAG_ARG_THIS);
      bool isNewTypeArg      = strcmp(fn->name,"_new") == 0 &&
                               coindex == 0; // first formal/actual

      bool promotes          = false;

      if (isInitCopy && isString(actual) && formal->getValType() == dtStringC) {
        // Do not allow an initCopy of a string to find the c_string initCopy,
        // which is considered first because it is not compiler generated.
        return false;
      } else if (actualIsTypeAlias != formalIsTypeAlias) {
        return false;

      } else if (formalIsTypeAlias &&
                 !isDispatchParent(actual->getValType(),
                                   formal->getValType()) &&
                 actual->getValType() != formal->getValType()) {
        // coercions should not be allowed for type variables
        return false;

      } else if (canDispatch(actual->type,
                             actual,
                             formal->type,
                             fn,
                             &promotes,
                             NULL,
                             formalIsParam) == false) {
        return false;
      } else if (isInitThis || isNewTypeArg) {
        AggregateType* ft = toAggregateType(formal->getValType());
        AggregateType* at = toAggregateType(actual->getValType());

        // Do not allow dispatch for 'this' formal with initializers
        //
        // Types may only mismatch if the formal's type is an instantiation of
        // the actual's type.
        if (ft != at && ft->isInstantiatedFrom(at) == false) {
          return false;
        }
      } else if (promotes && isCopyInit) {
        return false;
      }
    }
  }

  return evaluateWhereClause(fn);
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

        if (actualIsTypeAlias != formalIsTypeAlias) {
          return false;

        } else if (formal->type->symbol->hasFlag(FLAG_GENERIC)) {
          Type* t = getInstantiationType(actual->type, formal->type);
          if (t == NULL)
            return false;

        } else {
          bool formalIsParam = formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
                               formal->intent == INTENT_PARAM;

          if (canDispatch(actual->type,
                           actual,
                           formal->type,
                           fn,
                           NULL,
                           NULL,
                           formalIsParam) == false) {
            return false;
          }
        }
      }
    }

    coindex++;
  }

  return true;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

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

