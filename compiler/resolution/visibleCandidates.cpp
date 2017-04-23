/*
 * Copyright 2004-2017 Cray Inc.
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

#include "visibleCandidates.h"

#include "callInfo.h"
#include "expandVarArgs.h"
#include "expr.h"
#include "resolution.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

static void filterConcrete(CallInfo&                  info,
                           ResolutionCandidate*       currCandidate,
                           Vec<ResolutionCandidate*>& candidates);

static void filterGeneric (CallInfo&                  info,
                           ResolutionCandidate*       currCandidate,
                           Vec<ResolutionCandidate*>& candidates);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void gatherCandidates(CallInfo&                  info,
                             Vec<FnSymbol*>&            visibleFns,
                             bool                       generated,
                             Vec<ResolutionCandidate*>& candidates);

static void filterCandidate (CallInfo&                  info,
                             FnSymbol*                  fn,
                             Vec<ResolutionCandidate*>& candidates);

static void filterCandidate (CallInfo&                  info,
                             ResolutionCandidate*       currCandidate,
                             Vec<ResolutionCandidate*>& candidates);


void findVisibleCandidates(CallInfo&                  info,
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
                             bool                       compilerGenerated,
                             Vec<ResolutionCandidate*>& candidates) {
  forv_Vec(FnSymbol, fn, visibleFns) {
    // Consider either the user-defined functions or the compiler-generated
    // functions based on the input 'compilerGenerated'.
    if (fn->hasFlag(FLAG_COMPILER_GENERATED) == compilerGenerated) {

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
        if (fn->hasFlag(FLAG_NO_PARENS)        == true ||
            fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) == true) {
          filterCandidate(info, fn, candidates);
        }
      }
    }
  }
}

static void filterCandidate(CallInfo&                  info,
                            FnSymbol*                  fn,
                            Vec<ResolutionCandidate*>& candidates) {
  ResolutionCandidate* currCandidate = new ResolutionCandidate(fn);

  if (fExplainVerbose &&
      ((explainCallLine && explainCallMatch(info.call)) ||
       info.call->id == explainCallID)) {
    USR_PRINT(fn, "Considering function: %s", toString(fn));

    if (info.call->id == breakOnResolveID) {
      gdbShouldBreakHere();
    }
  }

  filterCandidate(info, currCandidate, candidates);

  if (candidates.tail() != currCandidate) {
    delete currCandidate;
  }
}

static void filterCandidate(CallInfo&                  info,
                            ResolutionCandidate*       currCandidate,
                            Vec<ResolutionCandidate*>& candidates) {
  if (currCandidate->fn->hasFlag(FLAG_GENERIC) == false) {
    filterConcrete(info, currCandidate, candidates);
  } else {
    filterGeneric (info, currCandidate, candidates);
  }
}

/************************************* | **************************************
*                                                                             *
* Candidate filtering logic specific to concrete functions.                   *
*                                                                             *
************************************** | *************************************/

static void resolveTypeConstructor(CallInfo& info, FnSymbol* fn);

static void filterConcrete(CallInfo&                  info,
                           ResolutionCandidate*       currCandidate,
                           Vec<ResolutionCandidate*>& candidates) {
  currCandidate->fn = expandIfVarArgs(currCandidate->fn, info);

  if (currCandidate->fn != NULL) {
    resolveTypedefedArgTypes(currCandidate->fn);

    if (currCandidate->computeAlignment(info) == true) {
      // Ensure that type constructor is resolved before other constructors.
      if (currCandidate->fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) == true) {
        resolveTypeConstructor(info, currCandidate->fn);
      }

      if (checkResolveFormalsWhereClauses(currCandidate) == true) {
        candidates.add(currCandidate);
      }
    }
  }
}

void resolveTypedefedArgTypes(FnSymbol* fn) {
  for_formals(formal, fn) {
    INT_ASSERT(formal->type); // Should be *something*.

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

static void resolveTypeConstructor(CallInfo& info, FnSymbol* fn) {
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

    resolveFns(typeConstructorCall->resolvedFunction());

    fn->_this->type = typeConstructorCall->resolvedFunction()->retType;

    typeConstructorCall->remove();
  }
}

// Verifies the usage of this candidate function
bool checkResolveFormalsWhereClauses(ResolutionCandidate* currCandidate) {
  int coindex = -1;

  /*
   * A derived generic type will use the type of its parent,
   * and expects this to be instantiated before it is.
   */
  resolveFormals(currCandidate->fn);

  for_formals(formal, currCandidate->fn) {
    if (Symbol* actual = currCandidate->formalIdxToActual.v[++coindex]) {
      bool actualIsTypeAlias = actual->hasFlag(FLAG_TYPE_VARIABLE);
      bool formalIsTypeAlias = formal->hasFlag(FLAG_TYPE_VARIABLE);

      bool formalIsParam     = formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
                               formal->intent == INTENT_PARAM;

      if (actualIsTypeAlias != formalIsTypeAlias) {
        return false;

      } else if (canDispatch(actual->type,
                             actual,
                             formal->type,
                             currCandidate->fn,
                             NULL,
                             formalIsParam) == false) {
        return false;
      }
    }
  }

  return evaluateWhereClause(currCandidate->fn);
}

/************************************* | **************************************
*                                                                             *
* Handle Generic functions                                                    *
*                                                                             *
************************************** | *************************************/

static void filterGeneric(CallInfo&                  info,
                          ResolutionCandidate*       currCandidate,
                          Vec<ResolutionCandidate*>& candidates) {
  currCandidate->fn = expandIfVarArgs(currCandidate->fn, info);

  if (currCandidate->fn                     != NULL &&
      currCandidate->computeAlignment(info) == true &&
      checkGenericFormals(currCandidate)    == true) {
    // Compute the param/type substitutions for generic arguments.
    currCandidate->computeSubstitutions();

    if (currCandidate->substitutions.n > 0) {
      /*
       * Instantiate enough of the generic to get through the rest of the
       * filtering and disambiguation processes.
       */
      currCandidate->fn = instantiateSignature(currCandidate->fn,
                                               currCandidate->substitutions,
                                               info.call);

      if (currCandidate->fn != NULL) {
        filterCandidate(info, currCandidate, candidates);
      }
    }
  }
}

// Verify that the generic formals are matched correctly
bool checkGenericFormals(ResolutionCandidate* currCandidate) {
  int coindex = 0;

  for_formals(formal, currCandidate->fn) {
    if (formal->type != dtUnknown) {
      if (Symbol* actual = currCandidate->formalIdxToActual.v[coindex]) {
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
                           currCandidate->fn,
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
