/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/disambiguation.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/queries/query-impl.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Function.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

struct DisambiguationCandidate {
  const TypedFnSignature* fn = nullptr;
  FormalActualMap formalActualMap;
  int idx = 0;

  DisambiguationCandidate(const TypedFnSignature* fn,
                          const CallInfo& call,
                          int idx)
    : fn(fn), formalActualMap(fn, call), idx(idx)
  {
  }
};

struct DisambiguationContext {
  Context* context = nullptr;
  const CallInfo* call = nullptr;
  const Scope* callInScope = nullptr;
  bool explain = false;
  DisambiguationContext(Context* context,
                        const CallInfo* call,
                        const Scope* callInScope,
                        bool explain)
    : context(context), call(call), callInScope(callInScope), explain(explain)
  {
  }
};

struct DisambiguationState {
  bool  fn1MoreSpecific = false;
  bool  fn2MoreSpecific = false;

  bool  fn1Promotes = false;
  bool  fn2Promotes = false;

  bool fn1WeakPreferred = false;
  bool fn2WeakPreferred = false;

  bool fn1WeakerPreferred = false;
  bool fn2WeakerPreferred = false;

  bool fn1WeakestPreferred = false;
  bool fn2WeakestPreferred = false;
};

using CandidatesVec = std::vector<const DisambiguationCandidate*>;

enum MoreVisibleResult {
  FOUND_F1_FIRST,
  FOUND_F2_FIRST,
  FOUND_BOTH,
  FOUND_NEITHER
};

#define ENABLE_TRACING_OF_DISAMBIGUATION 1

#ifdef ENABLE_TRACING_OF_DISAMBIGUATION

#define EXPLAIN(...) \
        if (dctx.explain) fprintf(stderr, __VA_ARGS__)

#define EXPLAIN_DUMP(fn) \
        if (dctx.explain) fn->dump(StringifyKind::CHPL_SYNTAX);

#else

#define EXPLAIN(...)
#define EXPLAIN_DUMP(fn)

#endif

static const TypedFnSignature*
findMostSpecificIgnoringReturn(const DisambiguationContext& dctx,
                               const CandidatesVec& candidates,
                               bool ignoreWhere,
                               CandidatesVec& ambiguousBest);

static int compareSpecificity(const DisambiguationContext& dctx,
                              const DisambiguationCandidate& candidate1,
                              const DisambiguationCandidate& candidate2,
                              bool ignoreWhere);

static MoreVisibleResult moreVisible(const DisambiguationContext& dctx,
                                     const DisambiguationCandidate& candidate1,
                                     const DisambiguationCandidate& candidate2);

static void testArgMapping(const DisambiguationContext& dctx,
                           const DisambiguationCandidate& candidate1,
                           const DisambiguationCandidate& candidate2,
                           int actualIdx,
                           DisambiguationState& ds);


static MostSpecificCandidates
computeMostSpecificCandidates(const DisambiguationContext& dctx,
                              const CandidatesVec& candidates) {

  CandidatesVec ambiguousBest;

  // The common case is that there is no ambiguity because the
  // return intent overload feature is not used.
  auto best = findMostSpecificIgnoringReturn(dctx, candidates,
                                             /* ignoreWhere */ true,
                                             ambiguousBest);

  if (best != nullptr) {
    return MostSpecificCandidates::getOnly(best);
  }

  if (ambiguousBest.size() == 0) {
    // nothing to do, return no candidates
    return MostSpecificCandidates::getEmpty();
  }

  // Now, if there was ambiguity, try again while considering
  // separately each category of return intent.
  //
  // If there is only one most specific function in each category,
  // that is what we need to return.
  int                  nRef              = 0;
  int                  nConstRef         = 0;
  int                  nValue            = 0;
  int                  nOther            = 0;

  // Count number of candidates in each category.
  for (auto c : ambiguousBest) {
    auto fn = c->fn;
    auto returnIntent = parsing::idToFnReturnIntent(dctx.context, fn->id());

    switch (returnIntent) {
      case Function::DEFAULT_RETURN_INTENT:
      case Function::CONST:
        nValue++;
        break;
      case Function::CONST_REF:
        nConstRef++;
        break;
      case Function::REF:
        nRef++;
        break;
      case Function::PARAM:
      case Function::TYPE:
        nOther++;
        break;
    }
  }

  if (nOther > 0) {
    // If there are *any* type/param candidates, we need to cause ambiguity
    // if they are not selected... including consideration of where clauses.
    ambiguousBest.clear();
    auto best = findMostSpecificIgnoringReturn(dctx, candidates,
                                               /* ignoreWhere */ false,
                                               ambiguousBest);

    if (ambiguousBest.size() > 1)
      return MostSpecificCandidates::getAmbiguous();

    return MostSpecificCandidates::getOnly(best);
  }

  if (nRef <= 1 && nConstRef <= 1 && nValue <= 1) {
    MostSpecificCandidates ret;

    for (auto c : ambiguousBest) {
      auto fn = c->fn;
      auto returnIntent = parsing::idToFnReturnIntent(dctx.context, fn->id());

      switch (returnIntent) {
        case Function::DEFAULT_RETURN_INTENT:
        case Function::CONST:
          ret.setBestValue(fn);
          break;
        case Function::CONST_REF:
          ret.setBestConstRef(fn);
          break;
        case Function::REF:
          ret.setBestRef(fn);
          break;
        case Function::PARAM:
        case Function::TYPE:
          assert(false && "should not be reachable");
          break;
      }
    }
    return ret;
  }

  // Otherwise, nRef > 1 || nConstRef > 1 || nValue > 1.

  // Split candidates into ref, const ref, and value candidates
  std::vector<const DisambiguationCandidate*> refCandidates;
  std::vector<const DisambiguationCandidate*> constRefCandidates;
  std::vector<const DisambiguationCandidate*> valueCandidates;

  // Gather the candidates in each category
  for (auto c : ambiguousBest) {
    auto fn = c->fn;
    auto returnIntent = parsing::idToFnReturnIntent(dctx.context, fn->id());

    switch (returnIntent) {
      case Function::DEFAULT_RETURN_INTENT:
      case Function::CONST:
        valueCandidates.push_back(c);
        break;
      case Function::CONST_REF:
        constRefCandidates.push_back(c);
        break;
      case Function::REF:
        refCandidates.push_back(c);
        break;
      case Function::PARAM:
      case Function::TYPE:
        break;
    }
  }

  // Disambiguate each group and update the counts
  bool ignoreWhere = false;

  ambiguousBest.clear();
  auto bestRef = findMostSpecificIgnoringReturn(dctx, refCandidates,
                                                ignoreWhere, ambiguousBest);
  if (bestRef != nullptr) {
    nRef = 1;
  } else {
    nRef = ambiguousBest.size();
  }

  ambiguousBest.clear();
  auto bestCRef = findMostSpecificIgnoringReturn(dctx, constRefCandidates,
                                                     ignoreWhere, ambiguousBest);
  if (bestCRef != nullptr) {
    nConstRef = 1;
  } else {
    nConstRef = ambiguousBest.size();
  }

  ambiguousBest.clear();
  auto bestValue = findMostSpecificIgnoringReturn(dctx, valueCandidates,
                                                  ignoreWhere, ambiguousBest);
  if (bestValue != nullptr) {
    nValue = 1;
  } else {
    nValue = ambiguousBest.size();
  }

  // if there is > 1 match in any category, fail to match due to ambiguity
  if (nRef > 1 || nConstRef > 1 || nValue > 1) {
    return MostSpecificCandidates::getAmbiguous();
  }

  // otherwise, there is 1 or fewer match in each category,
  // so there is no ambiguity.
  MostSpecificCandidates ret;
  if (bestRef != nullptr)
    ret.setBestRef(bestRef);
  if (bestCRef != nullptr)
    ret.setBestConstRef(bestCRef);
  if (bestValue != nullptr)
    ret.setBestValue(bestValue);

  return ret;
}

static const MostSpecificCandidates&
findMostSpecificCandidatesQuery(Context* context,
                                std::vector<const TypedFnSignature*> lst,
                                CallInfo call,
                                const Scope* callInScope) {
  QUERY_BEGIN(findMostSpecificCandidatesQuery, context, lst, call, callInScope);

  // Construct the DisambuguationContext
  bool explain = false;
  DisambiguationContext dctx(context, &call, callInScope, explain);

  // Compute all of the FormalActualMaps now
  std::vector<const DisambiguationCandidate*> candidates;
  {
    int n = lst.size();
    for (int i = 0; i < n; i++) {
      candidates.push_back(new DisambiguationCandidate(lst[i], call, i));
    }
  }

  MostSpecificCandidates result =
    computeMostSpecificCandidates(dctx, candidates);

  // Delete all of the FormalActualMaps
  for (auto elt : candidates) {
    delete elt;
  }

  return QUERY_END(result);
}


MostSpecificCandidates
findMostSpecificCandidates(Context* context,
                           const std::vector<const TypedFnSignature*>& lst,
                           const CallInfo& call,
                           const Scope* callInScope) {
  if (lst.size() == 0) {
    // nothing to do, return no candidates
    return MostSpecificCandidates::getEmpty();
  }

  if (lst.size() == 1) {
    // If there is just one candidate, return it
    return MostSpecificCandidates::getOnly(lst[0]);
  }

  // if we get here, > 1 candidates
  // run the query to handle the more complex case
  // TODO: is it worth storing this in a query? Or should
  // we recompute it each time?
  return findMostSpecificCandidatesQuery(context, lst, call, callInScope);
}

/*
  Find the most specific candidate and returns it, ignoring
  return intentss.

  If there is not a single most specific candidate, and ambiguousBest is not
  nullptr, appends the possibly-best candidates to ambiguousBest.

  Does not consider return intent overloading.
  */
static const TypedFnSignature*
findMostSpecificIgnoringReturn(const DisambiguationContext& dctx,
                               const CandidatesVec& candidates,
                               bool ignoreWhere,
                               CandidatesVec& ambiguousBest) {
  int n = candidates.size();

  if (n == 0) {
    // nothing to do
    return nullptr;
  }

  if (n == 1) {
    // the only match is the best match
    return candidates[0]->fn;
  }

  // If index i is set then we can skip testing function F_i because
  // we already know it can not be the best match.
  std::vector<bool> notBest(n, false);

  for (int i = 0; i < n; i++) {
    EXPLAIN("##########################\n");
    EXPLAIN("# Considering function %d #\n", i);
    EXPLAIN("##########################\n\n");

    const DisambiguationCandidate* candidate1 = candidates[i];
    bool singleMostSpecific = true;

    EXPLAIN_DUMP(candidate1->fn);

    if (notBest[i]) {
      EXPLAIN("Already known to not be best match.  Skipping.\n\n");
      continue;
    }

    for (int j = 0; j < n; j++) {
      if (i == j) {
        continue;
      }

      EXPLAIN("Comparing to function %d\n", j);
      EXPLAIN("-----------------------\n");

      const DisambiguationCandidate* candidate2 = candidates[j];

      EXPLAIN_DUMP(candidate2->fn);

      int cmp = compareSpecificity(dctx, *candidate1, *candidate2, ignoreWhere);

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
      return candidates[i]->fn;

    } else {
      EXPLAIN("Y: Fn %d is NOT the best match.\n\n\n", i);
    }
  }

  EXPLAIN("Z: No non-ambiguous best match.\n\n");

  for (int i = 0; i < n; i++) {
    if (notBest[i] == false) {
      ambiguousBest.push_back(candidates[i]);
    }
  }

  return nullptr;
}

/**

  Determines if fn1 is a better match than fn2.

  This function implements the function comparison component of the
  disambiguation procedure as detailed in section 13.13 of the Chapel
  language specification.

  \param candidate1 The function on the left-hand side of the comparison.
  \param candidate2 The function on the right-hand side of the comparison.
  \param ignoreWhere Set to `true` to ignore `where` clauses when
                     deciding if one match is better than another.
                     This is important for resolving return intent
                     overloads.

  \return -1 if fn1 is a more specific function than f2
  \return 0 if fn1 and fn2 are equally specific
  \return 1 if fn2 is a more specific function than f1
 */
static int compareSpecificity(const DisambiguationContext& dctx,
                              const DisambiguationCandidate& candidate1,
                              const DisambiguationCandidate& candidate2,
                              bool ignoreWhere) {

  bool prefer1 = false;
  bool prefer2 = false;
  int n = dctx.call->numActuals();
  DisambiguationState ds;

  for (int k = 0; k < n; k++) {
    testArgMapping(dctx, candidate1, candidate2, k, ds);
  }

  if (ds.fn1Promotes != ds.fn2Promotes) {
    EXPLAIN("\nP: only one of the functions requires argument promotion\n");

    // Prefer the version that did not promote
    prefer1 = !ds.fn1Promotes;
    prefer2 = !ds.fn2Promotes;

  } else if (ds.fn1MoreSpecific != ds.fn2MoreSpecific) {
    EXPLAIN("\nP1: only one more specific argument mapping\n");

    prefer1 = ds.fn1MoreSpecific;
    prefer2 = ds.fn2MoreSpecific;

  } else {
    // If the decision hasn't been made based on the argument mappings...
    auto moreVis = moreVisible(dctx, candidate1, candidate2);
    if (moreVis == MoreVisibleResult::FOUND_F1_FIRST) {
      EXPLAIN("\nQ: preferring more visible function\n");
      prefer1 = true;

    } else if (moreVis == MoreVisibleResult::FOUND_F2_FIRST) {
      EXPLAIN("\nR: preferring more visible function\n");
      prefer2 = true;

    } else if (ds.fn1WeakPreferred != ds.fn2WeakPreferred) {
      EXPLAIN("\nS: preferring based on weak preference\n");
      prefer1 = ds.fn1WeakPreferred;
      prefer2 = ds.fn2WeakPreferred;

    } else if (ds.fn1WeakerPreferred != ds.fn2WeakerPreferred) {
      EXPLAIN("\nS: preferring based on weaker preference\n");
      prefer1 = ds.fn1WeakerPreferred;
      prefer2 = ds.fn2WeakerPreferred;

    } else if (ds.fn1WeakestPreferred != ds.fn2WeakestPreferred) {
      EXPLAIN("\nS: preferring based on weakest preference\n");
      prefer1 = ds.fn1WeakestPreferred;
      prefer2 = ds.fn2WeakestPreferred;

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
      ID id1 = candidate1.fn->id();
      ID id2 = candidate2.fn->id();
      bool fn1where = parsing::functionWithIdHasWhere(dctx.context, id1);
      bool fn2where = parsing::functionWithIdHasWhere(dctx.context, id2);

      if (fn1where != fn2where) {
        EXPLAIN("\nU: preferring function with where clause\n");

        prefer1 = fn1where;
        prefer2 = fn2where;
      }
    }
  }

  assert(!(prefer1 && prefer2));

  if (prefer1) {
    EXPLAIN("\nW: Fn %d is more specific than Fn %d\n",
            candidate1.idx, candidate2.idx);
    return -1;

  } else if (prefer2) {
    EXPLAIN("\nW: Fn %d is less specific than Fn %d\n",
            candidate1.idx, candidate2.idx);
    return 1;

  } else {
    // Neither is more specific
    EXPLAIN("\nW: Fn %d and Fn %d are equally specific\n",
            candidate1.idx, candidate2.idx);
    return 0;
  }
}

static MoreVisibleResult
computeIsMoreVisible(Context* context,
                     UniqueString callName,
                     const Scope* callInScope,
                     ID fn1Id,
                     ID fn2Id) {

  // TODO: This might be over-simplified -- see issue #19167

  LookupConfig config = LOOKUP_DECLS |
                        LOOKUP_IMPORT_AND_USE;

  // Go up scopes tracking which symbol has been found
  // and then use that.
  for (auto curScope = callInScope;
       curScope != nullptr;
       curScope = curScope->parentScope()) {
    auto vec = lookupNameInScope(context, curScope, callName, config);

    bool found1 = false;
    bool found2 = false;
    for (const auto& borrowedIds : vec) {
      for (const auto& id : borrowedIds) {
        if (id == fn1Id) found1 = true;
        if (id == fn2Id) found2 = true;
      }
    }

    if (found1 || found2) {
      if (found1 && found2)
        return MoreVisibleResult::FOUND_BOTH;
      if (found1)
        return MoreVisibleResult::FOUND_F1_FIRST;
      if (found2)
        return MoreVisibleResult::FOUND_F2_FIRST;
    }
  }

  return MoreVisibleResult::FOUND_NEITHER;
}

static const MoreVisibleResult&
moreVisibleQuery(Context* context,
                 UniqueString callName,
                 const Scope* callInScope,
                 ID fn1Id,
                 ID fn2Id) {
  QUERY_BEGIN(moreVisibleQuery, context,
              callName, callInScope, fn1Id, fn2Id);

  MoreVisibleResult result =
    computeIsMoreVisible(context, callName, callInScope, fn1Id, fn2Id);

  return QUERY_END(result);
}

static MoreVisibleResult
moreVisible(const DisambiguationContext& dctx,
            const DisambiguationCandidate& candidate1,
            const DisambiguationCandidate& candidate2) {
  // Get the scopes for call, fn1, fn2
  UniqueString callName = dctx.call->name();
  const Scope* callInScope = dctx.callInScope;
  ID fn1Id = candidate1.fn->id();
  ID fn2Id = candidate2.fn->id();

  return moreVisibleQuery(dctx.context, callName, callInScope, fn1Id, fn2Id);
}

static void testArgMapping(const DisambiguationContext& dctx,
                           const DisambiguationCandidate& candidate1,
                           const DisambiguationCandidate& candidate2,
                           int actualIdx,
                           DisambiguationState& ds) {
  // TODO: fill this in
}


#if 0
// Calls canDispatch and does the initial EXPLAIN calls, which were otherwise
// duplicated
static void testArgMapHelper(FnSymbol* fn, ArgSymbol* formal, Symbol* actual,
                             Type* fType, Type* actualType, bool actualParam,
                             bool* formalPromotes, bool* formalNarrows,
                             const DisambiguationContext& DC,
                             DisambiguationState& ds,
                             int fnNum) {
  canDispatch(actualType, actual, fType, formal, fn, formalPromotes,
              formalNarrows);

  if (fnNum == 1) {
    DS.fn1Promotes |= *formalPromotes;
  } else if (fnNum == 2) {
    DS.fn2Promotes |= *formalPromotes;
  } else {
    INT_FATAL("fnNum should be either 1 or 2");
  }

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

static QualifiedType computeActualScalarType(Context* context,
                                             QualifiedType actualType) {
  // TODO: fill this in
  assert(false && "not implemented yet");
  return actualType;
}

static bool isNumericParamDefaultType(QualifiedType type) {
  if (auto typePtr = type.type()) {
    if (auto primType = typePtr->toPrimitiveType()) {
      return primType->isDefaultWidth();
    }
  }

  return false;
}

/**
  Compare two argument mappings, given a set of actual arguments, and set the
  disambiguation state appropriately.

  This function implements the argument mapping comparison component of the
  disambiguation procedure as detailed in section 13.14.3 of the Chapel
  language specification (page 107).

  actualIdx is the index within the call of the argument to be compared.
 */
static void testArgMapping(DisambiguationContext& dctx,
                           const DisambiguationCandidate& candidate1,
                           const DisambiguationCandidate& candidate2,
                           int actualIdx,
                           DisambiguationState& ds) {

  EXPLAIN("\nLooking at argument %d\n", actualIdx);

  const FormalActual* fa1 = candidate1.formalActualMap.byActualIdx(k);
  const FormalActual* fa2 = candidate2.formalActualMap.byActualIdx(k);

  if (formal1 == nullptr || formal2 == nullptr) {
    // TODO: call testOpArgMapping if one was an operator but the
    // other is not
    assert(false && "TODO -- handle operator calls");
  }

  QualifiedType f1Type = fa1->formalType();
  QualifiedType f2Type = fa2->formalType();
  QualifiedType actualType = fa1->actualType();
  assert(actualType == fa2->actualType());

  // Give up early for out intent arguments
  // (these don't impact candidate selection)
  if (f1Type->kind() == QualifiedType::OUT ||
      f2Type->kind() == QualifiedType::OUT) {
    return;
  }

  // Initializer work-around: Skip 'this' for generic initializers
  if (call->name() == USTR("init") || call->name == USTR("init=")) {
    if (fa1->formal()->name() == USTR("this") &&
        fa2->formal()->name() == USTR("this")) {
      if (f1Type.genericityWithFields(context) != Type::CONCRETE &&
          f2Type.genericityWithFields(context) != Type::CONCRETE) {
        return;
      }
    }
  }

  bool formal1Promotes = false;
  bool formal2Promotes = false;
  bool formal1Narrows = false;
  bool formal2Narrows = false;

  QualifiedType actualScalarType = actualType;

  bool f1Param = formal1->hasParamPtr();
  bool f2Param = formal2->hasParamPtr();

  bool actualParam = false;
  bool paramWithDefaultSize = false;

  // Don't enable param/ weak preferences for non-default sized param values.
  // If somebody bothered to type the param, they probably want it to stay that
  // way. This is a strategy to resolve ambiguity with e.g.
  //  +(param x:int(32), param y:int(32)
  //  +(param x:int(64), param y:int(64)
  // called with
  //  param x:int(32), param y:int(64)
  if (actualType->hasParamPtr()) {
    actualParam = true;
    paramWithDefaultSize = isNumericParamDefaultType(actualType);
  }

  EXPLAIN("Actual's type: %s", toString(actualType));
  if (actualParam)
    EXPLAIN(" (param)");
  if (paramWithDefaultSize)
    EXPLAIN(" (default)");
  EXPLAIN("\n");

  testArgMapHelper(fn1, formal1, actual, f1Type, actualType, actualParam,
                   &formal1Promotes, &formal1Narrows, DC, DS, 1);

  testArgMapHelper(fn2, formal2, actual, f2Type, actualType, actualParam,
                   &formal2Promotes, &formal2Narrows, DC, DS, 2);

  // Figure out scalar type for candidate matching
  if (formal1Promotes || formal2Promotes) {
    actualScalarType = computeActualScalarType(dctx.context, actualType);
  }

  // TODO: for sync/single use the valType

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

static void testOpArgMapping(FnSymbol* fn1, ArgSymbol* formal1, FnSymbol* fn2,
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
  const char* reason = "potentially optional argument present vs not";
  const char* level = "weak";

  if (formal1 == NULL) {
    EXPLAIN("Function 1 did not have a corresponding formal");
    EXPLAIN("Function 1 is an operator standalone function");
    INT_ASSERT(formal2 != NULL);

    Type* f2Type = formal2->type->getValType();
    bool formal2Promotes = false;
    bool formal2Narrows = false;

    testArgMapHelper(fn2, formal2, actual, f2Type, actualType, actualParam,
                     &formal2Promotes, &formal2Narrows, DC, DS, 2);

    DS.fn2WeakPreferred = true;
    EXPLAIN("%s: Fn % is %s preferred\n", reason, j, level);

  } else {
    INT_ASSERT(formal2 == NULL);
    EXPLAIN("Function 2 did not have a corresponding formal");
    EXPLAIN("Function 2 is an operator standalone function");

    Type* f1Type = formal1->type->getValType();
    bool formal1Promotes = false;
    bool formal1Narrows = false;

    testArgMapHelper(fn1, formal1, actual, f1Type, actualType, actualParam,
                     &formal1Promotes, &formal1Narrows, DC, DS, 1);

    DS.fn1WeakPreferred = true;
    EXPLAIN("%s: Fn % is %s preferred\n", reason, i, level);
  }
}
#endif


} // end namespace resolution


template<> struct update<resolution::MoreVisibleResult> {
  bool operator()(resolution::MoreVisibleResult& keep,
                  resolution::MoreVisibleResult& addin) const {
    return defaultUpdateBasic(keep, addin);
  }
};

template<> struct mark<resolution::MoreVisibleResult> {
  void operator()(Context* context,
                  const resolution::MoreVisibleResult& keep) const {
    // nothing to do for enum
  }
};


} // end namespace chpl
