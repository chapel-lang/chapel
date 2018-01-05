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

#include "initializerResolution.h"

#include "AggregateType.h"
#include "caches.h"
#include "callInfo.h"
#include "driver.h"
#include "expandVarArgs.h"
#include "expr.h"
#include "initializerRules.h"
#include "passes.h"
#include "resolution.h"
#include "ResolutionCandidate.h"
#include "resolveFunction.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "view.h"
#include "visibleFunctions.h"
#include "wrappers.h"

static void resolveInitCall(CallExpr* call);

static void gatherInitCandidates(CallInfo&                  info,
                                 Vec<FnSymbol*>&            visibleFns,
                                 Vec<ResolutionCandidate*>& candidates);

static void resolveInitializerMatch(FnSymbol* fn);

static void makeRecordInitWrappers(CallExpr* call);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

FnSymbol* resolveInitializer(CallExpr* call) {
  callStack.add(call);

  resolveInitCall(call);

  INT_ASSERT(call->isResolved());

  resolveInitializerMatch(call->resolvedFunction());

  if (isGenericRecord(call->get(2)->typeInfo())) {
    NamedExpr* named   = toNamedExpr(call->get(2));
    SymExpr*   namedSe = toSymExpr(named->actual);
    Symbol*    sym     = namedSe->symbol();

    sym->type = call->resolvedFunction()->_this->type;

    if (sym->hasFlag(FLAG_DELAY_GENERIC_EXPANSION) == true) {
      sym->removeFlag(FLAG_DELAY_GENERIC_EXPANSION);
    }

    makeRecordInitWrappers(call);
  }

  callStack.pop();

  return call->resolvedFunction();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveInitCall(CallExpr* call) {
  CallInfo info;

  if (call->id == breakOnResolveID) {
    printf("breaking on resolve call:\n");
    print_view(call);
    gdbShouldBreakHere();
  }

  if (info.isWellFormed(call) == true) {
    Vec<FnSymbol*>            visibleFns;
    Vec<ResolutionCandidate*> candidates;
    ResolutionCandidate*      best        = NULL;

    findVisibleFunctions(info, visibleFns);

    gatherInitCandidates(info, visibleFns, candidates);

    explainGatherCandidate(info, candidates);

    best = disambiguateForInit(info, candidates);

    if (best == NULL) {
      if (call->partialTag == false) {
        if (candidates.n == 0) {
          printResolutionErrorUnresolved(info, visibleFns);
        } else {
          printResolutionErrorAmbiguous (info, candidates);
        }
      }

    } else {
      instantiateBody(best->fn);

      if (explainCallLine != 0 && explainCallMatch(call) == true) {
        USR_PRINT(best->fn, "best candidate is: %s", toString(best->fn));
      }

      if (call->partialTag == false) {
        SET_LINENO(call);

        call->baseExpr->replace(new SymExpr(best->fn));

        checkForStoringIntoTuple(call, best->fn);

        resolveNormalCallCompilerWarningStuff(best->fn);
      }
    }

    forv_Vec(ResolutionCandidate*, candidate, candidates) {
      delete candidate;
    }

  } else {
    info.haltNotWellFormed();
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void doGatherInitCandidates(CallInfo&                  info,
                                   Vec<FnSymbol*>&            visibleFns,
                                   bool                       generated,
                                   Vec<ResolutionCandidate*>& candidates);

static void filterInitCandidate(CallInfo&                  info,
                                FnSymbol*                  fn,
                                Vec<ResolutionCandidate*>& candidates);

static void gatherInitCandidates(CallInfo&                  info,
                                 Vec<FnSymbol*>&            visibleFns,
                                 Vec<ResolutionCandidate*>& candidates) {
  // Search user-defined (i.e. non-compiler-generated) functions first.
  doGatherInitCandidates(info, visibleFns, false, candidates);

  // If no results, try again with any compiler-generated candidates.
  if (candidates.n == 0) {
    doGatherInitCandidates(info, visibleFns, true, candidates);
  }
}

static void doGatherInitCandidates(CallInfo&                  info,
                                   Vec<FnSymbol*>&            visibleFns,
                                   bool                       lastResort,
                                   Vec<ResolutionCandidate*>& candidates) {
  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    // Only consider functions marked with/without FLAG_LAST_RESORT
    if (visibleFn->hasFlag(FLAG_LAST_RESORT) == lastResort) {

      // Some expressions might resolve to methods without parenthesis.
      // If the call is marked with methodTag, it indicates the called
      // function should be a no-parens function or a type constructor.
      // (a type constructor call without parens uses default arguments)
      if (info.call->methodTag) {
        if (visibleFn->hasEitherFlag(FLAG_NO_PARENS, FLAG_TYPE_CONSTRUCTOR)) {
          // OK

        } else {
          // Skip this candidate
          continue;
        }
      }

      if (fExplainVerbose &&
          ((explainCallLine && explainCallMatch(info.call)) ||
           info.call->id == explainCallID)) {
        USR_PRINT(visibleFn, "Considering function: %s", toString(visibleFn));

        if (info.call->id == breakOnResolveID) {
          gdbShouldBreakHere();
        }
      }

      filterInitCandidate(info, visibleFn, candidates);
    }
  }
}

/** Tests to see if a function is a candidate for resolving a specific call.
 *  If it is a candidate, we add it to the candidate lists.
 *
 * This version of filterInitCandidate is called by code outside the
 * filterInitCandidate family of functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void filterInitCandidate(CallInfo&                  info,
                                FnSymbol*                  fn,
                                Vec<ResolutionCandidate*>& candidates) {
  ResolutionCandidate* candidate = new ResolutionCandidate(fn);

  if (candidate->isApplicableForInit(info) == true) {
    candidates.add(candidate);

  } else {
    delete candidate;
  }
}

/************************************* | **************************************
*                                                                             *
* Copied from resolveFunction(FnSymbol* fn) in functionResolution.            *
*                                                                             *
* Removed code for extern functions (since I don't think it will apply),      *
* iterators, type constructors, and FLAG_PRIVATIZED_CLASS.                    *
*                                                                             *
************************************** | *************************************/

static bool resolveInitializerBody(FnSymbol* fn);

static void resolveInitializerMatch(FnSymbol* fn) {
  if (fn->isResolved() == false) {
    AggregateType* at = toAggregateType(fn->_this->type);

    if (fn->id == breakOnResolveID) {
      printf("breaking on resolve fn:\n");
      print_view(fn);
      gdbShouldBreakHere();
    }

    insertFormalTemps(fn);

    if (at->isRecord() == true) {
      at->setFirstGenericField();

      resolveInitializerBody(fn);

    } else if (at->isClass() == true) {
      AggregateType* parent = at->dispatchParents.v[0];

      if (parent->isGeneric() == false) {
        if (at->setFirstGenericField() == false) {
          INT_ASSERT(false);
        }

      } else {
        at->setFirstGenericField();
      }

      resolveInitializerBody(fn);

      buildClassAllocator(fn);

    } else {
      INT_ASSERT(false);
    }
  }
}

static bool resolveInitializerBody(FnSymbol* fn) {
  bool retval = false;

  fn->addFlag(FLAG_RESOLVED);

  resolveBlockStmt(fn->body);

  if (tryFailure == false) {
    resolveReturnType(fn);

    toAggregateType(fn->_this->type)->initializerResolved = true;

    insertAndResolveCasts(fn);

    ensureInMethodList(fn);

    retval = true;

  } else {
    fn->removeFlag(FLAG_RESOLVED);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* This creates wrapper functions for calls to record initializers with        *
* default values, out of order named arguments, etc.  That effort was skipped *
* during the call match stage because the "this" argument to the initializer  *
* was still generic until the body had been resolved.  After we have          *
* determined the concrete type for the "this" argument, then we are capable   *
* of creating valid wrappers.                                                 *
*                                                                             *
* Note that this action is not necessary for class initializers, because      *
* such calls are wrapped by the "_new" function, and appropriate wrappers     *
* will be created for it, so we don't need to wrap the initializer itself.    *
*                                                                             *
************************************** | *************************************/

static void makeActualsVector(const CallInfo&          info,
                              std::vector<ArgSymbol*>& actualIdxToFormal);

static void makeRecordInitWrappers(CallExpr* call) {
  CallInfo info;

  if (info.isWellFormed(call) == true) {
    std::vector<ArgSymbol*> actualIdxToFormal;
    FnSymbol*               wrap = NULL;

    makeActualsVector(info, actualIdxToFormal);

    wrap = wrapAndCleanUpActuals(call->resolvedFunction(),
                                 info,
                                 actualIdxToFormal,
                                 true);

    call->baseExpr->replace(new SymExpr(wrap));

    resolveFunction(wrap);

  } else {
    info.haltNotWellFormed();
  }
}

// Modified version of computeActualFormalAlignment to only populate the
// actualIdxToFormal Vec.  Substitutes the formalIdxToActual Vec with one
// that stores booleans, because I do still need that information in order to
// correctly populate the actuals
//
// This work was already performed when we found the right resolution candidate
// so the "failure" modes should never get triggered.  The information we need
// was cleaned up, though, so we are just going to recreate the parts we need.
static void makeActualsVector(const CallInfo&          info,
                              std::vector<ArgSymbol*>& actualIdxToFormal) {
  const CallExpr*   call = info.call;
  FnSymbol*         fn   = call->resolvedFunction();
  std::vector<bool> formalIdxToActual;

  for (int i = 0; i < fn->numFormals(); i++) {
    formalIdxToActual.push_back(false);
  }
  for (int i = 0; i < info.actuals.n; i++) {
    actualIdxToFormal.push_back(NULL);
  }

  for (int i = 0; i < info.actuals.n; i++) {
    if (info.actualNames.v[i]) {
      bool match = false;
      int  j     = 0;

      for_formals(formal, fn) {
        if (strcmp(info.actualNames.v[i], formal->name) == 0) {
          match                = true;
          actualIdxToFormal[i] = formal;
          formalIdxToActual[j] = true;
          break;
        }

        j++;
      }

      // Fail if no matching formal is found.
      if (!match) {
        INT_FATAL(call,
                  "Compilation should have already ensured this action ",
                  "would be valid");
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

      while (formal) {
        if (formal->variableExpr) {
          return;
        }

        if (formalIdxToActual[j] == false) {
          match                = true;
          actualIdxToFormal[i] = formal;
          formalIdxToActual[j] = true;
          formal               = next_formal(formal);
          j++;
          break;
        }

        formal = next_formal(formal);
        j++;
      }

      // Fail if there are too many unnamed actuals.
      if (!match && !(fn->hasFlag(FLAG_GENERIC) && fn->hasFlag(FLAG_TUPLE))) {
        INT_FATAL(call,
                  "Compilation should have verified this action was valid");
      }
    }
  }

  // Make sure that any remaining formals are matched by name
  // or have a default value.
  while (formal) {
    if (formalIdxToActual[j] == false && !formal->defaultExpr) {
      // Fail if not.
      INT_FATAL(call,
                "Compilation should have verified this action was valid");
    }

    formal = next_formal(formal);

    j++;
  }
}

