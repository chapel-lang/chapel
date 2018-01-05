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

#include "caches.h"
#include "callInfo.h"
#include "driver.h"
#include "expandVarArgs.h"
#include "expr.h"
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

      retval = checkResolveFormalsWhereClauses();
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
          if (formal->instantiatedFrom) {
            typeConstructorCall->insertAtTail(formal->type->symbol);
          } else if (formal->hasFlag(FLAG_INSTANTIATED_PARAM)) {
            typeConstructorCall->insertAtTail(paramMap.get(formal));
          }

        } else {
          if (strcmp(formal->name, "outer") == 0 ||
              formal->type                  == dtMethodToken) {
            typeConstructorCall->insertAtTail(formal);

          } else if (formal->instantiatedFrom) {
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
    computeSubstitutions();

    if (substitutions.n > 0) {
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

bool ResolutionCandidate::isApplicableForInit(CallInfo& info) {
  bool retval = false;

  if (fn->hasFlag(FLAG_GENERIC) == false) {
    retval = isApplicableForInitConcrete(info);

  } else {
    retval = isApplicableForInitGeneric (info);
  }

  return retval;
}

bool ResolutionCandidate::isApplicableForInitConcrete(CallInfo& info) {
  bool retval = false;

  fn = expandIfVarArgs(fn, info);

  if (fn == NULL) {
    retval = false;

  } else {
    resolveTypedefedArgTypes();

    if (computeAlignment(info) == false) {
      retval = false;

    } else {
      retval = checkResolveFormalsWhereClauses();
    }
  }

  return retval;
}

bool ResolutionCandidate::isApplicableForInitGeneric(CallInfo& info) {
  bool retval = false;

  fn = expandIfVarArgs(fn, info);

  if (fn == NULL) {
    retval = false;

  } else if (computeAlignment(info) == false) {
    retval = false;

  } else if (checkGenericFormals()  == false) {
    retval = false;

  } else {
    computeSubstitutions();

    if (substitutions.n > 0) {
      /*
       * Instantiate just enough of the generic to get through the
       * the rest of the filtering and disambiguation processes.
       */
      fn = instantiateInitSig(info);

      if (fn != NULL) {
        retval = isApplicableForInit(info);
      }
    }
  }

  return retval;
}

FnSymbol* ResolutionCandidate::instantiateInitSig(CallInfo& info) {
  SymbolMap& subs   = substitutions;
  FnSymbol*  retval = NULL;

  if (FnSymbol* tupleFn = createTupleSignature(fn, subs, info.call)) {
    retval = tupleFn;

  } else {
    form_Map(SymbolMapElem, e, subs) {
      if (TypeSymbol* ts = toTypeSymbol(e->value)) {
        // This line is modified from instantiateSignature to allow the "this"
        // arg to remain generic until we have finished resolving the generic
        // portions of the initializer body's Phase 1.
        if (ts->type->symbol->hasFlag(FLAG_GENERIC) ==  true &&
            e->key->hasFlag(FLAG_ARG_THIS)          == false) {
          INT_FATAL(fn, "illegal instantiation with a generic type");

        } else {
          TypeSymbol* nts = getNewSubType(fn, e->key, ts);

          if (ts != nts) {
            e->value = nts;
          }
        }
      }
    }

    //
    // determine root function in the case of partial instantiation
    //
    FnSymbol* root = determineRootFunc(fn);

    //
    // determine all substitutions (past substitutions in a partial
    // instantiation plus the current substitutions) and change the
    // substitutions to refer to the root function's formal arguments
    //
    SymbolMap allSubs;

    determineAllSubs(fn, root, subs, allSubs);

    //
    // use cached instantiation if possible
    //
    if (FnSymbol* cached = checkCache(genericsCache, root, &allSubs)) {
      if (cached != (FnSymbol*)gVoid) {
        checkInfiniteWhereInstantiation(cached);

        retval = cached;

      } else {
        retval = NULL;
      }

    } else {

      SET_LINENO(fn);

      //
      // instantiate function
      //

      SymbolMap map;

      FnSymbol* newFn = instantiateFunction(fn,
                                            root,
                                            allSubs,
                                            info.call,
                                            subs,
                                            map);

      fixupTupleFunctions(fn, newFn, info.call);

      if (newFn->numFormals()       >  1 &&
          newFn->getFormal(1)->type == dtMethodToken) {
        newFn->getFormal(2)->type->methods.add(newFn);
      }

      newFn->tagIfGeneric();

      if (newFn->hasFlag(FLAG_GENERIC) == false &&
          evaluateWhereClause(newFn)   == false) {

        // where clause evaluates to false so cache gVoid as a function
        replaceCache(genericsCache, root, (FnSymbol*) gVoid, &allSubs);

        retval = NULL;

      } else {
        explainAndCheckInstantiation(newFn, fn);

        retval = newFn;
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
      if (match == false &&
          (fn->hasFlag(FLAG_GENERIC) == false ||
           fn->hasFlag(FLAG_TUPLE)   == false)) {
        return false;
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

void ResolutionCandidate::computeSubstitutions() {
  SymbolMap& subs = substitutions;
  int        i    = 0;

  substitutions.clear();

  for_formals(formal, fn) {
    if (formal->intent == INTENT_PARAM) {
      if (formalIdxToActual[i]                != NULL &&
          formalIdxToActual[i]->isParameter() == true) {
        if (!formal->type->symbol->hasFlag(FLAG_GENERIC) ||
            canInstantiate(formalIdxToActual[i]->type, formal->type))
          subs.put(formal, formalIdxToActual[i]);

      } else if (formalIdxToActual[i] == NULL && formal->defaultExpr) {
        // break because default expression may reference generic
        // arguments earlier in formal list; make those substitutions
        // first (test/classes/bradc/paramInClass/weirdParamInit4)
        if (subs.n > 0) {
          break;
        }

        resolveBlockStmt(formal->defaultExpr);

        SymExpr* se = toSymExpr(formal->defaultExpr->body.tail);

        if (se                          != NULL                              &&
            se->symbol()->isParameter() == true                              &&
            (formal->type->symbol->hasFlag(FLAG_GENERIC)      == false ||
             canInstantiate(se->symbol()->type, formal->type) ==  true)) {
          subs.put(formal, se->symbol());
        } else {
          INT_FATAL(fn, "unable to handle default parameter");
        }
      }

    } else if (formal->type->symbol->hasFlag(FLAG_GENERIC) == true) {
      //
      // check for field with specified generic type
      //
      if (formal->hasFlag(FLAG_TYPE_VARIABLE) == false &&
          formal->type                        != dtAny) {
        if (strcmp(formal->name, "outer")       != 0     &&
            formal->hasFlag(FLAG_IS_MEME)       == false &&
            (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) == true ||
             fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)    == true)) {
          USR_FATAL(formal,
                    "invalid generic type specification on class field");

        } else if (fn->isMethod()                       == true  &&
                   strcmp(fn->name, "init")             == 0     &&
                   fn->hasFlag(FLAG_COMPILER_GENERATED) == true  &&
                   fn->hasFlag(FLAG_DEFAULT_COPY_INIT)  == false &&
                   !(formal->hasFlag(FLAG_ARG_THIS)                == true  &&
                     formal->hasFlag(FLAG_DELAY_GENERIC_EXPANSION) == true)) {
          // This is a compiler generated initializer, so the argument with
          // a generic type corresponds with a class field.
          USR_FATAL(formal,
                    "invalid generic type specification on class field");

        }
      }

      if (formalIdxToActual[i] != NULL) {
        Type* actualType = formalIdxToActual[i]->type;

        if (formal->hasFlag(FLAG_ARG_THIS)                == true  &&
            formal->hasFlag(FLAG_DELAY_GENERIC_EXPANSION) == true &&
            actualType->symbol->hasFlag(FLAG_GENERIC)     == true) {

          // If the "this" arg is generic, we're resolving an initializer, and
          // the actual being passed is also still generic, don't count this as
          // a substitution.  Otherwise, we'll end up in an infinite loop if
          // one of the later generic args has a defaultExpr, as we will always
          // count the this arg as a substitution and so always approach the
          // generic arg with a defaultExpr as though a substitution was going
          // to take place.

        } else if (Type* type = getInstantiationType(actualType,
                                                     formal->type)) {

          // String literal actuals aligned with non-param generic formals of
          // type dtAny will result in an instantiation of dtStringC when the
          // function is extern. In other words, let us write:
          //   extern proc foo(str);
          //   foo("bar");
          // and pass "bar" as a c_string instead of a string
          if (fn->hasFlag(FLAG_EXTERN)            == true      &&
              formal->type                        == dtAny     &&
              formal->hasFlag(FLAG_PARAM)         == false     &&

              type                                == dtString  &&

              actualType                          == dtString  &&
              formalIdxToActual[i]->isImmediate() == true) {
            subs.put(formal, dtStringC->symbol);
          } else {
            subs.put(formal, type->symbol);
          }
        }
      } else if (formal->defaultExpr) {

        // break because default expression may reference generic
        // arguments earlier in formal list; make those substitutions
        // first (test/classes/bradc/genericTypes)
        if (subs.n > 0) {
          break;
        } else {

          resolveBlockStmt(formal->defaultExpr);

          Type* defaultType = formal->defaultExpr->body.tail->typeInfo();

          if (defaultType == dtTypeDefaultToken) {
            subs.put(formal, dtTypeDefaultToken->symbol);

          } else if (Type* type = getInstantiationType(defaultType,
                                                       formal->type)) {
            subs.put(formal, type->symbol);
          }
        }
      }
    }

    i++;
  }
}

static Type* getInstantiationType(Type* actualType, Type* formalType) {
  Type* ret = getBasicInstantiationType(actualType, formalType);

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

  if (Type* vt = actualType->getValType()) {
    if (canInstantiate(vt, formalType))
      return vt;
    else if (Type* st = vt->scalarPromotionType)
      if (canInstantiate(st, formalType))
        return st;
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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool ResolutionCandidate::checkResolveFormalsWhereClauses() {
  int coindex = -1;

  /*
   * A derived generic type will use the type of its parent,
   * and expects this to be instantiated before it is.
   */
  resolveSignature(fn);

  for_formals(formal, fn) {
    if (Symbol* actual = formalIdxToActual[++coindex]) {
      bool actualIsTypeAlias = actual->hasFlag(FLAG_TYPE_VARIABLE);
      bool formalIsTypeAlias = formal->hasFlag(FLAG_TYPE_VARIABLE);

      bool formalIsParam     = formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
                               formal->intent == INTENT_PARAM;

      if (actualIsTypeAlias != formalIsTypeAlias) {
        return false;

      } else if (canDispatch(actual->type,
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
          Type* vt  = actual->getValType();
          Type* st  = actual->type->scalarPromotionType;
          Type* svt = (vt) ? vt->scalarPromotionType : NULL;

          if (canInstantiate(actual->type, formal->type) == false &&

              (vt  == NULL || canInstantiate(vt,  formal->type) == false)  &&
              (st  == NULL || canInstantiate(st,  formal->type) == false)  &&
              (svt == NULL || canInstantiate(svt, formal->type) == false)) {

            return false;
          }

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

