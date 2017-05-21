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

static CandidateDisposition filterConcrete(CallInfo&                  info,
                           ResolutionCandidate*       currCandidate);

static CandidateDisposition filterGeneric(CallInfo&            info,
                                          ResolutionCandidate* currCandidate);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void gatherCandidates(CallInfo&                  info,
                             Vec<FnSymbol*>&            visibleFns,
                             Vec<int>&                  distances,
                             bool                       generated,
                             Vec<ResolutionCandidate*>& candidates,
                             Vec<ResolutionCandidate*>& rejects);

static void gatherCandidate (CallInfo&                  info,
                             FnSymbol*                  fn,
                             int                        distance,
                             Vec<ResolutionCandidate*>& candidates,
                             Vec<ResolutionCandidate*>& rejects);

static CandidateDisposition filterCandidate (CallInfo&                  info,
                             ResolutionCandidate*       currCandidate);


void findVisibleCandidates(CallInfo&                  info,
                           Vec<FnSymbol*>&            visibleFns,
                           Vec<int>&                  distances,
                           Vec<ResolutionCandidate*>& candidates,
                           Vec<ResolutionCandidate*>& rejects) {
  // Search user-defined (i.e. non-compiler-generated) functions first.
  gatherCandidates(info, visibleFns, distances, false, candidates, rejects);

  // If no results, try again with any compiler-generated candidates.
  if (candidates.n == 0) {
    gatherCandidates(info, visibleFns, distances, true, candidates, rejects);
  }
}

static void gatherCandidates(CallInfo&                  info,
                             Vec<FnSymbol*>&            visibleFns,
                             Vec<int>&                  distances,
                             bool                       compilerGenerated,
                             Vec<ResolutionCandidate*>& candidates,
                             Vec<ResolutionCandidate*>& rejects) {
  int i = 0;
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
        gatherCandidate(info, fn, distances.v[i], candidates, rejects);

      } else {
        if (fn->hasFlag(FLAG_NO_PARENS)        == true ||
            fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) == true) {
          gatherCandidate(info, fn, distances.v[i], candidates, rejects);
        }
      }
    }
    i++;
  }
}

static void gatherCandidate(CallInfo&                  info,
                            FnSymbol*                  fn,
                            int                        distance,
                            Vec<ResolutionCandidate*>& candidates,
                            Vec<ResolutionCandidate*>& rejects) {
  ResolutionCandidate* currCandidate = new ResolutionCandidate(fn, distance);

  if (fExplainVerbose &&
      ((explainCallLine && explainCallMatch(info.call)) ||
       info.call->id == explainCallID)) {
    USR_PRINT(fn, "Considering function: %s", toString(fn));

    if (info.call->id == breakOnResolveID) {
      gdbShouldBreakHere();
    }
  }

  CandidateDisposition disp = filterCandidate(info, currCandidate);
  currCandidate->disposition = disp;
  if (disp == AcceptCandidate)
    candidates.add(currCandidate);
  else
    rejects.add(currCandidate);
}

static CandidateDisposition filterCandidate(CallInfo&            info,
                                            ResolutionCandidate* currCandidate){
  if (currCandidate->fn->hasFlag(FLAG_GENERIC) == false) {
    return filterConcrete(info, currCandidate);
  } else {
    return filterGeneric(info, currCandidate);
  }
}

/************************************* | **************************************
*                                                                             *
* Candidate filtering logic specific to concrete functions.                   *
*                                                                             *
************************************** | *************************************/

static void resolveTypeConstructor(CallInfo& info, FnSymbol* fn);

static CandidateDisposition filterConcrete(CallInfo&            info,
                                           ResolutionCandidate* currCandidate) {
  currCandidate->fn = expandIfVarArgs(currCandidate->fn, info);

  if (currCandidate->fn == NULL)
    return RejectCandidateAlignment;

  resolveTypedefedArgTypes(currCandidate->fn);

  if (currCandidate->computeAlignment(info) == false)
    return RejectCandidateAlignment;
  
  // Ensure that type constructor is resolved before other constructors.
  if (currCandidate->fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) == true) {
    resolveTypeConstructor(info, currCandidate->fn);
  }

  return checkResolveFormalsWhereClauses(currCandidate);
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

  // Create reference types more explicitly as a workaround
  /*if (fn->hasFlag(FLAG_REF) == true) {
    DefExpr* def = toDefExpr(fn->formals.first());
    INT_ASSERT(def);
    Type* actualType = def->sym->typeInfo();
    makeRefType(actualType);
    fn->_this->type = actualType->refType;
  } else
  */

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
CandidateDisposition checkResolveFormalsWhereClauses(ResolutionCandidate* currCandidate) {
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

      bool actualIsParam     = actual->isParameter();

      bool formalIsParam     = formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
                               formal->intent == INTENT_PARAM;

      if (actualIsTypeAlias != formalIsTypeAlias) {
        return RejectCandidateTypeFormalNotActual;

      } else if(formalIsParam && !actualIsParam) {
        return RejectCandidateParamFormalNotActual;

      } else if (canDispatch(actual->type,
                             actual,
                             formal->type,
                             currCandidate->fn,
                             NULL,
                             formalIsParam) == false) {
        return RejectCandidateArgTypeError;
      }
    }
  }

  if (evaluateWhereClause(currCandidate->fn) == false)
    return RejectCandidateWhere;

  return AcceptCandidate;
}

/************************************* | **************************************
*                                                                             *
* Handle Generic functions                                                    *
*                                                                             *
************************************** | *************************************/

static CandidateDisposition filterGeneric(CallInfo&            info,
                                          ResolutionCandidate* currCandidate) {
  currCandidate->fn = expandIfVarArgs(currCandidate->fn, info);

  if (currCandidate->fn == NULL)
    return RejectCandidateAlignment;

  if (currCandidate->computeAlignment(info) == false) {
    return RejectCandidateAlignment;
  }

  {
    CandidateDisposition d = checkGenericFormals(currCandidate);
    if (d != AcceptCandidate)
      return d;
  }

  // Compute the param/type substitutions for generic arguments.
  currCandidate->computeSubstitutions();

  if (currCandidate->substitutions.n <= 0) {
    return RejectCandidateArgTypeError;
  }

  /*
   * Instantiate enough of the generic to get through the rest of the
   * filtering and disambiguation processes.
   */
  currCandidate->fn = instantiateSignature(currCandidate->fn,
                                           currCandidate->substitutions,
                                           info.call);

  if (currCandidate->fn == NULL) {
    return RejectCandidateArgTypeError;
  }

  return filterCandidate(info, currCandidate);
}

// Verify that the generic formals are matched correctly
// Returns a disposition
CandidateDisposition checkGenericFormals(ResolutionCandidate* currCandidate) {
  int coindex = 0;

  for_formals(formal, currCandidate->fn) {
    if (formal->type != dtUnknown) {
      if (Symbol* actual = currCandidate->formalIdxToActual.v[coindex]) {
        bool actualIsTypeAlias = actual->hasFlag(FLAG_TYPE_VARIABLE);
        bool formalIsTypeAlias = formal->hasFlag(FLAG_TYPE_VARIABLE);
        bool actualIsParam     = actual->isParameter();

        bool formalIsParam     = formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
                                 formal->intent == INTENT_PARAM;

        if (actualIsTypeAlias != formalIsTypeAlias) {
          return RejectCandidateTypeFormalNotActual;

        } else if(formalIsParam && !actualIsParam) {
          return RejectCandidateParamFormalNotActual;

        } else if (formal->type->symbol->hasFlag(FLAG_GENERIC)) {
          Type* vt  = actual->getValType();
          Type* st  = actual->type->scalarPromotionType;
          Type* svt = (vt) ? vt->scalarPromotionType : NULL;

          if (canInstantiate(actual->type, formal->type) == false &&

              (vt  == NULL || canInstantiate(vt,  formal->type) == false)  &&
              (st  == NULL || canInstantiate(st,  formal->type) == false)  &&
              (svt == NULL || canInstantiate(svt, formal->type) == false)) {


            if (currCandidate->fn->_this == formal)
              return RejectCandidateThisTypeError;
            else
              return RejectCandidateArgTypeError;
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

            return RejectCandidateArgTypeError;
          }
        }
      }
    }

    coindex++;
  }

  return AcceptCandidate;
}
