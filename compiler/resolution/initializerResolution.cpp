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
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "view.h"
#include "visibleCandidates.h"
#include "visibleFunctions.h"
#include "wrappers.h"

static void resolveInitCall(CallExpr* call);

static void gatherInitCandidates(CallInfo&                  info,
                                 Vec<FnSymbol*>&            visibleFns,
                                 Vec<ResolutionCandidate*>& candidates);

static void resolveMatch(FnSymbol* fn);

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

  resolveMatch(call->resolvedFunction());

  if (isGenericRecord(call->get(2)->typeInfo())) {
    NamedExpr* named   = toNamedExpr(call->get(2));
    INT_ASSERT(named);

    SymExpr*   namedSe = toSymExpr(named->actual);
    INT_ASSERT(namedSe);

    namedSe->symbol()->type = call->resolvedFunction()->_this->type;

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

  if (info.isNotWellFormed(call, true) == true) {
    info.haltNotWellFormed(true);

  } else {
    DisambiguationContext     DC(info);
    Vec<FnSymbol*>            visibleFns;
    Vec<ResolutionCandidate*> candidates;
    Vec<ResolutionCandidate*> ambiguous;
    ResolutionCandidate*      best        = NULL;

    findVisibleFunctions(info, visibleFns);

    gatherInitCandidates(info, visibleFns, candidates);

    explainGatherCandidate(candidates, info);

    best = disambiguateByMatch(candidates, DC, false, ambiguous);

    if (best == NULL) {
      if (call->partialTag == false) {
        if (candidates.n > 0) {
          printResolutionErrorAmbiguous(info, candidates);

        } else {
          printResolutionErrorUnresolved(visibleFns, &info);
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
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void      doGatherInitCandidates(CallInfo&                  info,
                                        Vec<FnSymbol*>&            visibleFns,
                                        bool                       generated,
                                        Vec<ResolutionCandidate*>& candidates);

static void      filterInitCandidate(CallInfo&                  info,
                                     FnSymbol*                  fn,
                                     Vec<ResolutionCandidate*>& candidates);

static void      filterInitCandidate(CallInfo&                  info,
                                     ResolutionCandidate*       currCandidate,
                                     Vec<ResolutionCandidate*>& candidates);

static void      filterInitGenericCandidate(
                                     CallInfo&                  info,
                                     ResolutionCandidate*       currCandidate,
                                     Vec<ResolutionCandidate*>& candidates);


static FnSymbol* instantiateInitSig(FnSymbol*  fn,
                                    SymbolMap& subs,
                                    CallExpr*  call);

static void      filterInitConcreteCandidate(
                                     CallInfo&                  info,
                                     ResolutionCandidate*       currCandidate,
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
                                   bool                       generated,
                                   Vec<ResolutionCandidate*>& candidates) {
  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    // Only consider user functions or compiler-generated functions
    if (visibleFn->hasFlag(FLAG_COMPILER_GENERATED) == generated) {

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
  ResolutionCandidate* currCandidate = new ResolutionCandidate(fn);

  filterInitCandidate(info, currCandidate, candidates);

  if (candidates.tail() != currCandidate) {
    delete currCandidate;
  }
}

/** Tests to see if a function is a candidate for resolving a specific call.
 *  If it is a candidate, we add it to the candidate lists.
 *
 * This version of filterInitCandidate is called by other versions of
 * filterInitCandidate, and shouldn't be called outside this family of
 * functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void filterInitCandidate(CallInfo&                  info,
                                ResolutionCandidate*       currCandidate,
                                Vec<ResolutionCandidate*>& candidates) {
  if (currCandidate->fn->hasFlag(FLAG_GENERIC)) {
    filterInitGenericCandidate (info, currCandidate, candidates);

  } else {
    filterInitConcreteCandidate(info, currCandidate, candidates);
  }
}


/** Candidate filtering logic specific to generic functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void filterInitGenericCandidate(
                                  CallInfo&                  info,
                                  ResolutionCandidate*       currCandidate,
                                  Vec<ResolutionCandidate*>& candidates) {
  currCandidate->fn = expandIfVarArgs(currCandidate->fn, info);

  if (!currCandidate->fn) {
    return;
  }

  if (!currCandidate->computeAlignment(info)) {
    return;
  }

  if (checkGenericFormals(currCandidate) == false) {
    return;
  }

  // Compute the param/type substitutions for generic arguments.
  currCandidate->computeSubstitutions(true);

  /*
   * If no substitutions were made we can't instantiate this generic, and must
   * reject it.
   */
  if (currCandidate->substitutions.n > 0) {
    /*
     * Instantiate just enough of the generic to get through the rest of the
     * filtering and disambiguation processes.
     */
    currCandidate->fn = instantiateInitSig(currCandidate->fn,
                                           currCandidate->substitutions,
                                           info.call);

    if (currCandidate->fn != NULL) {
      filterInitCandidate(info, currCandidate, candidates);
    }
  }
}

// Rewrite of instantiateSignature, for initializers.  Removed some bits,
// modified others.
/** Instantiate enough of the function for it to make it through the candidate
 *  filtering and disambiguation process.
 *
 * \param fn   Generic function to instantiate
 * \param subs Type substitutions to be made during instantiation
 * \param call Call that is being resolved
 */
static FnSymbol* instantiateInitSig(FnSymbol*  fn,
                                    SymbolMap& subs,
                                    CallExpr*  call) {
  //
  // Handle tuples explicitly
  // (_build_tuple, tuple type constructor, tuple default constructor)
  //
  FnSymbol* tupleFn = createTupleSignature(fn, subs, call);

  if (tupleFn) return tupleFn;

  form_Map(SymbolMapElem, e, subs) {
    if (TypeSymbol* ts = toTypeSymbol(e->value)) {
      // This line is modified from instantiateSignature to allow the "this"
      // arg to remain generic until we have finished resolving the generic
      // portions of the initializer body's Phase 1.
      if (ts->type->symbol->hasFlag(FLAG_GENERIC) &&
          !e->key->hasFlag(FLAG_ARG_THIS)) {
        INT_FATAL(fn, "illegal instantiation with a generic type");
      }

      TypeSymbol* nts = getNewSubType(fn, e->key, ts);

      if (ts != nts) {
        e->value = nts;
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
  SymbolMap all_subs;

  determineAllSubs(fn, root, subs, all_subs);

  //
  // use cached instantiation if possible
  //
  if (FnSymbol* cached = checkCache(genericsCache, root, &all_subs)) {
    if (cached != (FnSymbol*)gVoid) {
      checkInfiniteWhereInstantiation(cached);
      return cached;

    } else {
      return NULL;
    }
  }

  SET_LINENO(fn);

  //
  // instantiate function
  //

  SymbolMap map;

  FnSymbol* newFn = instantiateFunction(fn, root, all_subs, call, subs, map);

  fixupTupleFunctions(fn, newFn, call);

  if (newFn->numFormals() > 1 && newFn->getFormal(1)->type == dtMethodToken) {
    newFn->getFormal(2)->type->methods.add(newFn);
  }

  newFn->tagIfGeneric();

  if (newFn->hasFlag(FLAG_GENERIC) == false &&
      evaluateWhereClause(newFn)   == false) {
    //
    // where clause evaluates to false so cache gVoid as a function
    //
    replaceCache(genericsCache, root, (FnSymbol*)gVoid, &all_subs);

    return NULL;

  } else {
    explainAndCheckInstantiation(newFn, fn);

    return newFn;
  }
}

/** Candidate filtering logic specific to concrete functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void filterInitConcreteCandidate(
                              CallInfo&                  info,
                              ResolutionCandidate*       currCandidate,
                              Vec<ResolutionCandidate*>& candidates) {
  currCandidate->fn = expandIfVarArgs(currCandidate->fn, info);

  if (!currCandidate->fn) {
    return;
  }

  resolveTypedefedArgTypes(currCandidate->fn);

  if (!currCandidate->computeAlignment(info)) {
    return;
  }

  // We should reject this candidate if any of the situations handled by this
  // function are violated.
  if (checkResolveFormalsWhereClauses(currCandidate) == false) {
    return;
  }

  candidates.add(currCandidate);
}

/************************************* | **************************************
*                                                                             *
* Copied from resolveFns(FnSymbol* fn) in functionResolution.                 *
*                                                                             *
* Removed code for extern functions (since I don't think it will apply),      *
* iterators, type constructors, and FLAG_PRIVATIZED_CLASS.                    *
*                                                                             *
************************************** | *************************************/

static void resolveMatch(FnSymbol* fn) {
  if (fn->isResolved() == true) {
    return;
  }

  if (fn->id == breakOnResolveID) {
    printf("breaking on resolve fn:\n");
    print_view(fn);
    gdbShouldBreakHere();
  }

  fn->addFlag(FLAG_RESOLVED);

  insertFormalTemps(fn);

  bool wasGeneric = fn->_this->type->symbol->hasFlag(FLAG_GENERIC);

  if (wasGeneric) {
    AggregateType* at  = toAggregateType(fn->_this->type);
    INT_ASSERT(at);

    bool           res = at->setNextGenericField();
    INT_ASSERT(res);
  }

  resolveBlockStmt(fn->body);

  if (wasGeneric == true && isClass(fn->_this->type) == true) {
    FnSymbol* classAlloc = buildClassAllocator(fn);

    normalize(classAlloc);
  }

  if (tryFailure) {
    fn->removeFlag(FLAG_RESOLVED);
    return;
  }

  resolveReturnType(fn);

  toAggregateType(fn->_this->type)->initializerResolved = true;

  //
  // insert casts as necessary
  //
  insertAndResolveCasts(fn);

  //
  // make sure methods are in the methods list
  //
  ensureInMethodList(fn);
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
  CallInfo                info;
  std::vector<ArgSymbol*> actualIdxToFormal;
  FnSymbol*               wrap = NULL;

  if (info.isNotWellFormed(call) == true) {
    info.haltNotWellFormed();
  }

  makeActualsVector(info, actualIdxToFormal);

  wrap = wrapAndCleanUpActuals(call->resolvedFunction(),
                               info,
                               &actualIdxToFormal,
                               true);

  call->baseExpr->replace(new SymExpr(wrap));

  resolveFns(wrap);
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

