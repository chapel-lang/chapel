/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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
#include "chpl/framework/query-impl.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Function.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

struct DisambiguationContext;

struct DisambiguationCandidate {
  const TypedFnSignature* fn = nullptr;
  QualifiedType forwardingTo; // actual passed to receiver when forwarding
  FormalActualMap formalActualMap;
  int idx = 0;
  SubstitutionsMap promotedFormals;
  bool nImplicitConversionsComputed = false;
  bool anyNegParamToUnsigned = false;
  int nImplicitConversions = 0;
  int nParamNarrowingImplicitConversions = 0;
  // What is the visibility distance? This is -1 if it has not been computed.
  int visibilityDistance = -1;

  DisambiguationCandidate(const TypedFnSignature* fn,
                          QualifiedType forwardingTo,
                          const CallInfo& call,
                          int idx)
    : fn(fn), forwardingTo(forwardingTo), formalActualMap(fn, call), idx(idx),
      promotedFormals(), nImplicitConversionsComputed(false),
      anyNegParamToUnsigned(false), nImplicitConversions(0),
      nParamNarrowingImplicitConversions(0), visibilityDistance(-1)
  {
  }

  void computeConversionInfo(Context* context, int numActuals);
  void computeConversionInfo(const DisambiguationContext& dctx);

  MostSpecificCandidate toMostSpecificCandidate(ResolutionContext* rc, const Scope* callInScope, const PoiScope* callInPoiScope) const {
    return MostSpecificCandidate::fromTypedFnSignature(rc, fn, formalActualMap, callInScope, callInPoiScope, promotedFormals);
  }
};

struct DisambiguationContext {
  ResolutionContext* rc = nullptr;
  const CallInfo* call = nullptr;
  const Scope* callInScope = nullptr;
  const PoiScope* callInPoiScope = nullptr;
  bool explain = false;
  bool isMethodCall = false;
  bool useOldVisibility = false;
  DisambiguationContext(ResolutionContext* rc,
                        const CallInfo* call,
                        const Scope* callInScope,
                        const PoiScope* callInPoiScope,
                        bool explain)
    : rc(rc), call(call),
      callInScope(callInScope), callInPoiScope(callInPoiScope),
      explain(explain)
  {
    isMethodCall = call->isMethodCall();

    // this is a workaround -- a better solution would be preferred
    if (parsing::idIsInInternalModule(rc->context(), callInScope->id())) {
      useOldVisibility = true;
    }

    // this is a workaround -- a better solution would be preferred.
    // this function seems to be created in a way that has problems with
    // the visibility logic in disambiguation.
    if (call->name() == "_getIterator") {
      useOldVisibility = true;
    }
  }
};

struct DisambiguationState {
  bool fn1NonParamArgsPreferred = false;
  bool fn2NonParamArgsPreferred = false;

  bool fn1ParamArgsPreferred = false;
  bool fn2ParamArgsPreferred = false;
};

using CandidatesVec = std::vector<const DisambiguationCandidate*>;

#ifdef ENABLE_TRACING_OF_DISAMBIGUATION

#define EXPLAIN(...) \
        if (dctx.explain) fprintf(stderr, __VA_ARGS__)

#define EXPLAIN_DUMP(thing) \
        if (dctx.explain) { \
          (thing)->dump(StringifyKind::CHPL_SYNTAX); \
          fprintf(stderr, "\n"); \
        }

#else

#define EXPLAIN(...)
#define EXPLAIN_DUMP(thing)

#endif

static const DisambiguationCandidate*
disambiguateByMatch(const DisambiguationContext& dctx,
                    const CandidatesVec& candidates,
                    bool ignoreWhere,
                    CandidatesVec& ambiguousBest);

static MostSpecificCandidates
computeMostSpecificCandidates(ResolutionContext* rc,
                              const DisambiguationContext& dctx,
                              const CandidatesVec& candidates) ;

// handle the more complex case where there is > 1 candidate
// with a particular return intent by disambiguating each group
// individually.
static MostSpecificCandidates
computeMostSpecificCandidatesWithVecs(ResolutionContext* rc,
                                      const DisambiguationContext& dctx,
                                      const CandidatesVec& vec);

static int compareSpecificity(const DisambiguationContext& dctx,
                              const DisambiguationCandidate& candidate1,
                              const DisambiguationCandidate& candidate2,
                              int i,
                              int j,
                              bool forGenericInit);

static int testArgMapping(const DisambiguationContext& dctx,
                          const DisambiguationCandidate& candidate1,
                          const DisambiguationCandidate& candidate2,
                          int actualIdx,
                          DisambiguationState& ds,
                          std::string& reason);

static void testArgMapHelper(const DisambiguationContext& dctx,
                             const FormalActual& fa,
                             const QualifiedType& forwardingTo,
                             bool* formalPromotes, bool* formalNarrows,
                             DisambiguationState& ds,
                             int fnNum);

static int testOpArgMapping(const DisambiguationContext& dctx,
                            const DisambiguationCandidate& candidate1,
                            const DisambiguationCandidate& candidate2,
                            int actualIdx,
                            DisambiguationState& ds,
                            std::string& reason);

static bool isFormalInstantiatedAny(const DisambiguationCandidate& candidate,
                                    const FormalActual* fa);

static bool isFormalPartiallyGeneric(const DisambiguationCandidate& candidate,
                                     const FormalActual* fa);

static QualifiedType computeActualScalarType(Context* context,
                                             QualifiedType actualType);

static bool moreSpecificCanDispatch(const DisambiguationContext& dctx,
                                    QualifiedType actualType,
                                    QualifiedType formalType);

static void discardWorseVisibility(const DisambiguationContext& dctx,
                                   const CandidatesVec& candidates,
                                   std::vector<bool>& discarded);

static void discardWorseArgs(const DisambiguationContext& dctx,
                             const CandidatesVec& candidates,
                             std::vector<bool>& discarded);

static void discardWorsePromoting(const DisambiguationContext& dctx,
                                  const CandidatesVec& candidates,
                                  std::vector<bool>&  discarded);

static void discardWorseWhereClauses(const DisambiguationContext& dctx,
                                     const CandidatesVec& candidates,
                                     std::vector<bool>& discarded);

static void discardWorseConversions(const DisambiguationContext& dctx,
                                    const CandidatesVec& candidates,
                                    std::vector<bool>& discarded);

static void disambiguateDiscarding(const DisambiguationContext&   dctx,
                                   const CandidatesVec& candidates,
                                   bool ignoreWhere,
                                   std::vector<bool>&   discarded);

static int prefersNumericCoercion(const DisambiguationContext& dctx,
                                  QualifiedType f1Qt,
                                  QualifiedType f2Qt,
                                  QualifiedType actualQt,
                                  std::string &reason);

// count the number of candidates with each return intent
static void countByReturnIntent(const DisambiguationContext& dctx,
                                const CandidatesVec& vec,
                                int& nRef,
                                int& nConstRef,
                                int& nValue,
                                int& nOther) {
  for (auto c : vec) {
    auto fn = c->fn;
    auto returnIntent = parsing::idToFnReturnIntent(dctx.rc->context(), fn->id());

    switch (returnIntent) {
      case Function::DEFAULT_RETURN_INTENT:
      case Function::OUT:
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
}

// if there is <= 1 most specific candidate with each intent,
// return it as a MostSpecificCandidates
static MostSpecificCandidates
gatherByReturnIntent(ResolutionContext* rc,
                     const DisambiguationContext& dctx,
                     const CandidatesVec& vec) {
  MostSpecificCandidates ret;

  for (auto c : vec) {
    auto fn = c->fn;
    auto returnIntent = parsing::idToFnReturnIntent(dctx.rc->context(), fn->id());

    switch (returnIntent) {
      case Function::DEFAULT_RETURN_INTENT:
      case Function::OUT:
      case Function::CONST:
        CHPL_ASSERT(!ret.bestValue());
        ret.setBestValue(c->toMostSpecificCandidate(rc, dctx.callInScope, dctx.callInPoiScope));
        break;
      case Function::CONST_REF:
        CHPL_ASSERT(!ret.bestConstRef());
        ret.setBestConstRef(c->toMostSpecificCandidate(rc, dctx.callInScope, dctx.callInPoiScope));
        break;
      case Function::REF:
        CHPL_ASSERT(!ret.bestRef());
        ret.setBestRef(c->toMostSpecificCandidate(rc, dctx.callInScope, dctx.callInPoiScope));
        break;
      case Function::PARAM:
      case Function::TYPE:
        CHPL_ASSERT(false && "should not be reachable");
        break;
    }
  }

  return ret;
}

// Gather the most specific candidates with each return intent into vectors
// by return intent
static void gatherVecsByReturnIntent(const DisambiguationContext& dctx,
                                     const CandidatesVec& vec,
                                     CandidatesVec& refCandidates,
                                     CandidatesVec& constRefCandidates,
                                     CandidatesVec& valueCandidates) {
  for (auto c : vec) {
    auto fn = c->fn;
    auto returnIntent = parsing::idToFnReturnIntent(dctx.rc->context(), fn->id());

    switch (returnIntent) {
      case Function::DEFAULT_RETURN_INTENT:
      case Function::OUT:
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
}


static const MostSpecificCandidates&
findMostSpecificCandidatesQuery(ResolutionContext* rc,
                                CandidatesAndForwardingInfo lst,
                                CallInfo call,
                                const Scope* callInScope,
                                const PoiScope* callInPoiScope) {
  CHPL_RESOLUTION_QUERY_BEGIN(findMostSpecificCandidatesQuery, rc,
              lst, call, callInScope, callInPoiScope);

  if (call.name() == "this") {
    if (call.actual(0).type().type() && call.actual(0).type().type()->isArrayType() &&
        call.actual(0)
            .type().type()
            ->toArrayType()
            ->eltType()
            .type()
            ->isStringType()) {
      gdbShouldBreakHere();
    }
  }

  // Construct the DisambiguationContext
  bool explain = true;
  DisambiguationContext dctx(rc, &call,
                             callInScope, callInPoiScope,
                             explain);

  // Compute all of the FormalActualMaps now
  std::vector<const DisambiguationCandidate*> candidates;
  {
    int n = lst.size();
    for (int i = 0; i < n; i++) {
      QualifiedType forwardingTo;
      if (lst.hasForwardingInfo()) {
        forwardingTo = lst.getForwardingInfo(i);
      }
      candidates.push_back(
          new DisambiguationCandidate(lst.get(i), forwardingTo, call, i));
    }
  }

  MostSpecificCandidates result =
    // disambiguateByMatch(dctx, candidates);
    computeMostSpecificCandidates(rc, dctx, candidates);

  if (result.numBest() == 1) {
    MostSpecificCandidate only;
    if (result.bestRef()) only = result.bestRef();
    else if (result.bestConstRef()) only = result.bestConstRef();
    else if (result.bestValue()) only = result.bestValue();

    // Ensure that the only result is in the 'ONLY' slot.
    result = MostSpecificCandidates::getOnly(only);
  }

  // Delete all of the FormalActualMaps
  for (auto elt : candidates) {
    delete elt;
  }

  return CHPL_RESOLUTION_QUERY_END(result);
}

// entry point for disambiguation
MostSpecificCandidates
findMostSpecificCandidates(ResolutionContext* rc,
                           const CandidatesAndForwardingInfo& lst,
                           const CallInfo& call,
                           const Scope* callInScope,
                           const PoiScope* callInPoiScope) {
  if (lst.size() == 0) {
    // nothing to do, return no candidates
    return MostSpecificCandidates::getEmpty();
  }

  if (lst.size() == 1) {
    // If there is just one candidate, return it.
    //
    // Create a DisambiguationCandidate anyway, to re-use the promotion
    // counting logic.
    DisambiguationCandidate candidate(lst.get(0), QualifiedType(), call, 0);
    candidate.computeConversionInfo(rc->context(), call.numActuals());
    auto msc = candidate.toMostSpecificCandidate(rc, callInScope, callInPoiScope);
    return MostSpecificCandidates::getOnly(msc);
  }

  // if we get here, > 1 candidates
  // run the query to handle the more complex case
  // TODO: is it worth storing this in a query? Or should
  // we recompute it each time?
  return findMostSpecificCandidatesQuery(rc, lst, call,
                                         callInScope, callInPoiScope);
}


static MostSpecificCandidates
computeMostSpecificCandidates(ResolutionContext* rc,
                              const DisambiguationContext& dctx,
                              const CandidatesVec& candidates) {
  using Dcp = DisambiguationCandidate*;
  CandidatesVec ambiguousBest;

  // The common case is that there is no ambiguity because the
  // return intent overload feature is not used.
  auto best = (Dcp)disambiguateByMatch(dctx, candidates,
                                       /* ignoreWhere */ true,
                                       ambiguousBest);

  if (best != nullptr) {
    return MostSpecificCandidates::getOnly(best->toMostSpecificCandidate(rc, dctx.callInScope, dctx.callInPoiScope));
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
  int nRef = 0;
  int nConstRef = 0;
  int nValue = 0;
  int nOther = 0;

  // Count number of candidates in each category.
  countByReturnIntent(dctx, ambiguousBest, nRef, nConstRef, nValue, nOther);

  if (nOther > 0) {
    // If there are *any* type/param candidates, we need to cause ambiguity
    // if they are not selected... including consideration of where clauses.
    ambiguousBest.clear();
    auto best = (Dcp)disambiguateByMatch(dctx, candidates,
                                         /* ignoreWhere */ false,
                                         ambiguousBest);

    if (ambiguousBest.size() > 1)
      return MostSpecificCandidates::getAmbiguous();

    return MostSpecificCandidates::getOnly(best->toMostSpecificCandidate(rc, dctx.callInScope, dctx.callInPoiScope));
  }

  if (nRef <= 1 && nConstRef <= 1 && nValue <= 1) {
    return gatherByReturnIntent(rc, dctx, ambiguousBest);
  }

  // Otherwise, nRef > 1 || nConstRef > 1 || nValue > 1.

  // handle the more complex case where there is > 1 candidate
  // with a particular return intent by disambiguating each group
  // individually.
  return computeMostSpecificCandidatesWithVecs(rc, dctx, ambiguousBest);
}

static MostSpecificCandidates
computeMostSpecificCandidatesWithVecs(ResolutionContext* rc,
                                      const DisambiguationContext& dctx,
                                      const CandidatesVec& vec) {
  CandidatesVec refCandidates;
  CandidatesVec constRefCandidates;
  CandidatesVec valueCandidates;
  CandidatesVec ambiguousBest;

  // Split candidates into ref, const ref, and value candidates
  gatherVecsByReturnIntent(dctx, vec,
                           refCandidates,
                           constRefCandidates,
                           valueCandidates);

  // Disambiguate each group and update the counts
  int nRef = 0;
  int nConstRef = 0;
  int nValue = 0;

  bool ignoreWhere = false;
  using Dcp = DisambiguationCandidate*;
  ambiguousBest.clear();
  auto bestRef = (Dcp)disambiguateByMatch(dctx, refCandidates, ignoreWhere,
                                          ambiguousBest);
  if (bestRef != nullptr) {
    nRef = 1;
  } else {
    nRef = ambiguousBest.size();
  }

  ambiguousBest.clear();
  auto bestCRef = (Dcp)disambiguateByMatch(dctx, constRefCandidates, ignoreWhere,
                                           ambiguousBest);
  if (bestCRef != nullptr) {
    nConstRef = 1;
  } else {
    nConstRef = ambiguousBest.size();
  }

  ambiguousBest.clear();
  auto bestValue = (Dcp)disambiguateByMatch(dctx, valueCandidates, ignoreWhere,
                                            ambiguousBest);
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
    ret.setBestRef(bestRef->toMostSpecificCandidate(rc, dctx.callInScope, dctx.callInPoiScope));
  if (bestCRef != nullptr)
    ret.setBestConstRef(bestCRef->toMostSpecificCandidate(rc, dctx.callInScope, dctx.callInPoiScope));
  if (bestValue != nullptr)
    ret.setBestValue(bestValue->toMostSpecificCandidate(rc, dctx.callInScope, dctx.callInPoiScope));

  return ret;
}


/**

  Determines if fn1 is a better match than fn2.

  This function implements the function comparison component of the
  disambiguation procedure as detailed in section 13.13 of the Chapel
  language specification.

  \param candidate1 The function on the left-hand side of the comparison.
  \param candidate2 The function on the right-hand side of the comparison.


  \return -1 if the two functions are incomparable
  \return 0 if the two functions are equally specific
  \return 1 if fn1 is a more specific function than f2
  \return 2 if fn2 is a more specific function than f1
 */
static int compareSpecificity(const DisambiguationContext& dctx,
                              const DisambiguationCandidate& candidate1,
                              const DisambiguationCandidate& candidate2,
                              int i,
                              int j,
                              bool forGenericInit) {

  bool prefer1 = false;
  bool prefer2 = false;
  int n = dctx.call->numActuals();
  int nArgsIncomparable = 0;
  std::string reason;
  DisambiguationState ds;

  // Initializer work-around: Skip 'this' for generic initializers
  int start = (forGenericInit == false) ? 0 : 1;

  for (int k = start; k < n; k++) {

    EXPLAIN("\nLooking at argument %d\n", k);
    const FormalActual* fa1 = candidate1.formalActualMap.byActualIdx(k);
    const FormalActual* fa2 = candidate2.formalActualMap.byActualIdx(k);

    if (fa1 == nullptr || fa2 == nullptr) {
      if (candidate1.fn->untyped()->kind()==Function::Kind::OPERATOR &&
          candidate2.fn->untyped()->kind()==Function::Kind::OPERATOR) {
        EXPLAIN("\nSkipping argument %d because could be in an operator call\n", k);
        continue;
      } else {
        // One of the two candidate functions was not an operator, but one
        // was so we need to do something special here.
        int p = testOpArgMapping(dctx, candidate1, candidate2, k, ds, reason);
        std::string reason = "operator method vs function";
        if (p == 1) {
          ds.fn1NonParamArgsPreferred = true;
          EXPLAIN("%s: Fn %d is non-param preferred\n", reason.c_str(), i);
        } else if (p == 2) {
          ds.fn2NonParamArgsPreferred = true;
            EXPLAIN("%s: Fn %d is non-param preferred\n", reason.c_str(), j);
        }
        continue;
      }
    }

    bool actualParam = fa1->actualType().isParam();


    int p = testArgMapping(dctx, candidate1, candidate2, k, ds, reason);
    if (p == -1) {
      nArgsIncomparable++;
    }

    if (actualParam) {
      if (p == 1) {
        ds.fn1ParamArgsPreferred = true;
        EXPLAIN("%s: Fn %d is param preferred\n", reason.c_str(), i);
      } else if (p == 2) {
        ds.fn2ParamArgsPreferred = true;
        EXPLAIN("%s: Fn %d is param preferred\n", reason.c_str(), j);
      }
    } else {
      if (p == 1) {
        ds.fn1NonParamArgsPreferred = true;
        EXPLAIN("%s: Fn %d is non-param preferred\n", reason.c_str(), i);
      } else if (p == 2) {
        ds.fn2NonParamArgsPreferred = true;
        EXPLAIN("%s: Fn %d is non-param preferred\n", reason.c_str(), j);
      }
    }
  }
  if (ds.fn1NonParamArgsPreferred != ds.fn2NonParamArgsPreferred) {
    EXPLAIN("\nP: only one function has preferred non-param args\n");

    prefer1 = ds.fn1NonParamArgsPreferred;
    prefer2 = ds.fn2NonParamArgsPreferred;

  } else if (ds.fn1ParamArgsPreferred != ds.fn2ParamArgsPreferred) {
    EXPLAIN("\nP1: only one function has preferred param args\n");

    prefer1 = ds.fn1ParamArgsPreferred;
    prefer2 = ds.fn2ParamArgsPreferred;

  }

  if (prefer1) {
    EXPLAIN("\nW: Fn %d is more specific than Fn %d\n", i, j);
    return 1;

  } else if (prefer2) {
    EXPLAIN("\nW: Fn %d is less specific than Fn %d\n", i, j);
    return 2;

  } else {
    if (nArgsIncomparable > 0 ||
        (ds.fn1NonParamArgsPreferred && ds.fn2NonParamArgsPreferred) ||
        (ds.fn1ParamArgsPreferred && ds.fn2ParamArgsPreferred)) {
      EXPLAIN("\nW: Fn %d and Fn %d are incomparable\n", i, j);
      return -1;
    }

    EXPLAIN("\nW: Fn %d and Fn %d are equally specific\n", i, j);
    return 0;
  }
}


/*
 * Returns:
 *   0 if there is no preference between them
 *   1 if fn1 is preferred
 *   2 if fn2 is preferred
*/
static int testOpArgMapping(const DisambiguationContext& dctx,
                           const DisambiguationCandidate& candidate1,
                           const DisambiguationCandidate& candidate2,
                           int actualIdx,
                           DisambiguationState& ds,
                           std::string& reason) {
  // Validate our assumptions in this function - only operator functions should
  // return a NULL for the formal and they should only do so for method token
  // and "this" actuals.

  const FormalActual* fa1 = candidate1.formalActualMap.byActualIdx(actualIdx);
  const FormalActual* fa2 = candidate2.formalActualMap.byActualIdx(actualIdx);

  bool cand1IsOp = candidate1.fn->untyped()->kind() == Function::Kind::OPERATOR;
  bool cand2IsOp = candidate2.fn->untyped()->kind() == Function::Kind::OPERATOR;

  CHPL_ASSERT(cand1IsOp == (fa1 == nullptr));
  CHPL_ASSERT(cand2IsOp == (fa2 == nullptr));
  CHPL_ASSERT(fa1->actualType() == fa2->actualType());

  if (fa1 == nullptr) {
    CHPL_ASSERT(fa2 != nullptr);

    bool formal2Promotes = false;
    bool formal2Narrows = false;

    testArgMapHelper(dctx, *fa2, candidate2.forwardingTo,
                     &formal2Promotes, &formal2Narrows, ds, 2);
    return 2;

  } else {
    CHPL_ASSERT(fa2 == nullptr);

    bool formal1Promotes = false;
    bool formal1Narrows = false;

    testArgMapHelper(dctx, *fa1, candidate1.forwardingTo,
                     &formal1Promotes, &formal1Narrows, ds, 1);

    return 1;
  }

  return 0;

}

//
// helper routines for isMoreVisible (below);
//
static bool
isDefinedInBlock(Context* context, const Scope* scope,
                 const TypedFnSignature* fn) {
  LookupConfig onlyDecls = LOOKUP_DECLS | LOOKUP_METHODS;
  auto ids = lookupNameInScope(context, scope,
                               /* methodLookupHelper */ nullptr,
                               /* receiverScopeHelper */ nullptr,
                               fn->untyped()->name(), onlyDecls);
  for (const auto& id : ids) {
    if (id == fn->id()) return true;
  }
  return false;
}

static bool
isDefinedInUseImport(Context* context, const Scope* scope,
                     const TypedFnSignature* fn,
                     bool allowPrivateUseImp, bool forShadowScope) {
  LookupConfig importAndUse = LOOKUP_IMPORT_AND_USE | LOOKUP_METHODS;

  if (!forShadowScope) {
    importAndUse |= LOOKUP_SKIP_SHADOW_SCOPES;
  }

  if (!allowPrivateUseImp) {
    importAndUse |= LOOKUP_SKIP_PRIVATE_USE_IMPORT;
  }

  auto ids = lookupNameInScope(context, scope,
                               /* methodLookupHelper */ nullptr,
                               /* receiverScopeHelper */ nullptr,
                               fn->untyped()->name(), importAndUse);
  for (const auto& id : ids) {
    if (id == fn->id()) return true;
  }
  return false;
}

// Returns a distance measure used to compare the visibility
// of two functions.
//
// Enclosing scope adds 2 distance
// Shadow scope adds 1 distance
//
// Returns -1 if the function is not found here
// or if the scope was already visited.
static int
computeVisibilityDistanceInternal(Context* context, const Scope* scope,
                                  const TypedFnSignature* fn, int distance) {
  if (!scope) return -1;

  // first, check things in the current block or
  // from use/import that don't use a shadow scope
  bool foundHere = isDefinedInBlock(context, scope, fn) ||
                   isDefinedInUseImport(context, scope, fn,
                                        /* allowPrivateUseImp */ true,
                                        /* forShadowScope */ false);
  if (foundHere) {
    return distance;
  }
  // next, check anything from a use/import in the
  // current block that uses a shadow scope
  bool foundShadowHere = isDefinedInUseImport(context, scope, fn,
                                              /* allowPrivateUseImp */ true,
                                              /* forShadowScope */ true);
  if (foundShadowHere) {
    return distance+1;
  }

  // next, check parent scope, recursively
  if (const Scope* parentScope = scope->parentScope()) {
    return computeVisibilityDistanceInternal(context, parentScope, fn,
                                             distance+2);
  }

  return -1;
}

// Returns a distance measure used to compare the visibility
// of two functions.
//
// Returns -1 if the function is a method or if the function is not found
static int computeVisibilityDistance(Context* context, const Scope* scope,
                                     const TypedFnSignature* fn) {
  // is this a method?
  if (fn->untyped()->isMethod()) {
    return -1;
  }
  return computeVisibilityDistanceInternal(context, scope, fn, 0);
}

// Discard candidates with further visibility distance
// than other candidates.
// This check does not consider methods or operator methods.
static void discardWorseVisibility(const DisambiguationContext& dctx,
                                   const CandidatesVec& candidates,
                                   std::vector<bool>& discarded) {
  int minDistance = INT_MAX;
  int maxDistance = INT_MIN;

  for (size_t i = 0; i < candidates.size(); i++) {
    if (discarded[i]) {
      continue;
    }

    DisambiguationCandidate* candidate = (DisambiguationCandidate*)candidates[i];
    auto scope = dctx.callInPoiScope == nullptr ?
                                        dctx.callInScope :
                                        dctx.callInPoiScope->inScope();
    int distance = computeVisibilityDistance(dctx.rc->context(), scope, candidate->fn);
    candidate->visibilityDistance = distance;

    if (distance >= 0) {
      if (distance < minDistance) {
        minDistance = distance;
      }
      if (distance > maxDistance) {
        maxDistance = distance;
      }
    }
  }

  if (minDistance < maxDistance) {
    for (size_t i = 0; i < candidates.size(); i++) {
      if (discarded[i]) {
        continue;
      }

      const DisambiguationCandidate* candidate = candidates[i];
      int distance = candidate->visibilityDistance;
      if (distance > 0 && distance > minDistance) {
        EXPLAIN("X: Fn %lu has further visibility distance\n", i);
        discarded[i] = true;
      }
    }
  }
}


static void disambiguateDiscarding(const DisambiguationContext&   dctx,
                                   const CandidatesVec& candidates,
                                   bool                         ignoreWhere,
                                   std::vector<bool>&           discarded) {
  // TODO: Implement commented code
  // if (mixesNonOpMethodsAndFunctions(candidates, DC, discarded)) {
  //   return;
  // }

  if (!dctx.useOldVisibility && !dctx.isMethodCall) {
    // If some candidates are less visible than other candidates,
    // discard those with less visibility.
    // This filter should not be applied to method calls.
    discardWorseVisibility(dctx, candidates, discarded);
  }

  // If any candidate does not require promotion,
  // eliminate candidates that do require promotion.
  discardWorsePromoting(dctx, candidates, discarded);

  // Consider the relationship among the arguments
  // Note that this part is a partial order;
  // in other words, "incomparable" is an option when comparing
  // two candidates. It should be transitive.
  // Discard any candidate that has a worse argument mapping than another
  // candidate.
  discardWorseArgs(dctx, candidates, discarded);

  // Apply further filtering to the set of candidates

  // Discard any candidate that has more implicit conversions
  // than another candidate.
  // After that, discard any candidate that has more param narrowing
  // conversions than another candidate.
  discardWorseConversions(dctx, candidates, discarded);

  if (!ignoreWhere) {
    // If some candidates have 'where' clauses and others do not,
    // discard those without 'where' clauses
    discardWorseWhereClauses(dctx, candidates, discarded);
  }
  if (dctx.useOldVisibility && !dctx.isMethodCall) {
    // If some candidates are less visible than other candidates,
    // discard those with less visibility.
    // This filter should not be applied to method calls.
    discardWorseVisibility(dctx, candidates, discarded);
  }
}

static const DisambiguationCandidate*
disambiguateByMatchInner(const DisambiguationContext& dctx,
                         const CandidatesVec& candidates,
                         bool ignoreWhere,
                         CandidatesVec& ambiguous) {

  // quick exit if there is nothing to do
  if (candidates.size() == 0) {
    return nullptr;
  }
  if (candidates.size() == 1) {
    return candidates[0];
  }

  // Disable implicit conversion to remove nilability for disambiguation
  // int saveGenerousResolutionForErrors = 0;
  // if (generousResolutionForErrors > 0) {
  //   saveGenerousResolutionForErrors = generousResolutionForErrors;
  //   generousResolutionForErrors = 0;
  // }

  // If index i is set we have ruled out that function
  std::vector<bool> discarded(candidates.size(), false);

  // use new rules
  disambiguateDiscarding(dctx, candidates, ignoreWhere, discarded);

  // If there is just 1 candidate at this point, return it
  {
    int only = -1;
    int currentCandidates = 0;
    for (size_t i = 0; i < candidates.size(); ++i) {
      if (discarded[i]) {
        continue;
      }
      only = i;
      currentCandidates++;
    }

    if (currentCandidates == 1) {
      EXPLAIN("Y: Fn %d is the best match.\n\n\n", only);

      // if (saveGenerousResolutionForErrors > 0)
      //   generousResolutionForErrors = saveGenerousResolutionForErrors;

      return candidates[only];
    }
  }

  // There was more than one best candidate.
  // So, add whatever is left to 'ambiguous'
  // and return NULL.
  for (size_t i = 0; i < candidates.size(); i++) {
    if (discarded[i]) {
      continue;
    }

    EXPLAIN("Z: Fn %lu is one of the best matches\n", i);
    ambiguous.push_back(candidates[i]);
  }

  // if (saveGenerousResolutionForErrors > 0)
  //   generousResolutionForErrors = saveGenerousResolutionForErrors;

  return nullptr;
}

static const DisambiguationCandidate*
disambiguateByMatch(const DisambiguationContext& dctx,
                    const CandidatesVec& candidates,
                    bool ignoreWhere,
                    CandidatesVec& ambiguousBest) {

  return disambiguateByMatchInner(dctx, candidates, ignoreWhere, ambiguousBest);

  // TODO: maybe implement the verify procedure in dyno
  // if (fVerify) {
  //   // check that 'compareSpecificity' creates a partial order
  //   PartialOrderChecker checker(candidates, DC);

  //   for (int i = 0; i < candidates.n; ++i) {
  //     ResolutionCandidate* candidate1         = candidates.v[i];
  //     bool forGenericInit = candidate1->fn->isInitializer() ||
  //                           candidate1->fn->isCopyInit();

  //     for (int j = i; j < candidates.n; ++j) {
  //       ResolutionCandidate* candidate2 = candidates.v[j];

  //       int cmp = compareSpecificity(candidate1,
  //                                    candidate2,
  //                                    DC,
  //                                    i,
  //                                    j,
  //                                    forGenericInit);

  //       checker.addResult(i, j, cmp);
  //     }
  //   }

  //   checker.checkResults();
  // }

}


// Discard any candidate that has a worse argument mapping than another
// candidate.
static void discardWorseArgs(const DisambiguationContext& dctx,
                             const CandidatesVec& candidates,
                             std::vector<bool>& discarded) {
  int n = candidates.size();

  // If index i is set then we can skip testing function F_i because
  // we already know it can not be the best match.
  std::vector<bool> notBest(n, false);

  for (int i = 0; i < n; i++) {
    if (discarded[i]) {
      continue;
    }

    EXPLAIN("##########################\n");
    EXPLAIN("# Considering function %d #\n", i);
    EXPLAIN("##########################\n\n");

    const DisambiguationCandidate* candidate1 = candidates[i];

    bool forGenericInit = candidate1->fn->untyped()->isMethod() &&
                          (candidate1->fn->untyped()->name() == USTR("init") ||
                           candidate1->fn->untyped()->name() == USTR("init="));

    EXPLAIN_DUMP(candidate1->fn);

    if (notBest[i]) {
      EXPLAIN("Already known to not be best match.  Skipping.\n\n");
      continue;
    }

    for (int j = 0; j < n; j++) {
      if (i == j) {
        continue;
      }
      if (discarded[j]) {
        continue;
      }

      EXPLAIN("Comparing to function %d\n", j);
      EXPLAIN("-----------------------\n");

      const DisambiguationCandidate* candidate2 = candidates[j];

      EXPLAIN_DUMP(candidate2->fn);

      // Consider the relationship among the arguments
      // Note that this part is a partial order;
      // in other words, "incomparable" is an option when comparing
      // two candidates.
      int cmp = compareSpecificity(dctx, *candidate1, *candidate2, i, j,
                                   forGenericInit);

      if (cmp == 1) {
        EXPLAIN("X: Fn %d is a better match than Fn %d\n\n\n", i, j);
        notBest[j] = true;

      } else if (cmp == 2) {
        EXPLAIN("X: Fn %d is a worse match than Fn %d\n\n\n", i, j);
        notBest[i] = true;
        break;
      } else {
        if (cmp == -1) {
          EXPLAIN("X: Fn %d is incomparable with Fn %d\n\n\n", i, j);
        } else if (cmp == 0) {
          EXPLAIN("X: Fn %d is as good a match as Fn %d\n\n\n", i, j);
          if (notBest[j]) {
            notBest[i] = true;
            break;
          }
        }
      }
    }
  }

  // Now, discard any candidates that were worse than another candidate
  for (size_t i = 0; i < candidates.size(); ++i) {
    if (notBest[i]) {
      discarded[i] = true;
    }
  }
}

static void discardWorseWhereClauses(const DisambiguationContext& dctx,
                                     const CandidatesVec& candidates,
                                     std::vector<bool>& discarded) {
  int nWhere = 0;
  int nNoWhere = 0;
  for (size_t i = 0; i < candidates.size(); ++i) {
    if (discarded[i]) {
      continue;
    }

    const DisambiguationCandidate* candidate = candidates[i];
    auto whereClause = candidate->fn->whereClauseResult();
    bool where = whereClause != TypedFnSignature::WHERE_NONE;

    if (where) {
      nWhere++;
    } else {
      nNoWhere++;
    }
  }

  if (nWhere > 0 && nNoWhere > 0) {
    for (size_t i = 0; i < candidates.size(); ++i) {
      if (discarded[i]) {
        continue;
      }

      const DisambiguationCandidate* candidate = candidates[i];
      auto whereClause = candidate->fn->whereClauseResult();
      bool where = whereClause != TypedFnSignature::WHERE_NONE;

      if (!where) {
        EXPLAIN("X: Fn %lu does not have 'where' but others do\n", i);
        discarded[i] = true;
      }
    }
  }
}

static bool isNegative(const Param* p) {
  if (auto ip = p->toIntParam()) {
    int64_t val = ip->value();
    return val < 0;
  }
  return false;
}

static bool isNegativeParamToUnsigned(const Param* actualSym,
                                      const Type* actualScalarType,
                                      const Type* formalType) {
  if (actualSym && actualScalarType->isIntType() && formalType->isUintType()) {
   return isNegative(actualSym);
  }
  return false;
}

static bool isMatchingImagComplex(const Type* actualType, const Type* formalType) {
  if (auto fct = formalType->toComplexType()) {
    if (auto ait = actualType->toImagType()) {
      return ait->bitwidth()*2 == fct->bitwidth();
    } else if (auto art = actualType->toRealType()) {
      return art->bitwidth()*2 == fct->bitwidth();
    }
  }
  return false;
}

static const Type*
normalizeTupleTypeToValueTuple(Context* context, const Type* t) {
  if (auto tt = t->toTupleType()) {
    return (const Type*)tt->toValueTuple(context);
  }
  return t;
}

void DisambiguationCandidate::computeConversionInfo(Context* context, int numActuals) {
  // no need to recompute it if it is already computed
  if (this->nImplicitConversionsComputed) {
    return;
  }

  bool anyNegParamToUnsigned = false;
  int numParamNarrowing = 0;
  int nImplicitConversions = 0;

  bool forGenericInit = this->fn->untyped()->name()==USTR("init") ||
                        this->fn->untyped()->name()==USTR("init=");
  size_t n = (size_t) numActuals;
  for (size_t k = 0; k < n; k++) {

    const FormalActual* fa1 = this->formalActualMap.byActualIdx(k);

    if (fa1 == nullptr) {
      // TODO: is this possible with dyno?
      // for now, assume it cannot happen and assert as such
      CHPL_ASSERT(false && "Unexpected null FormalActual");
      // this can happen with some operators in production
      // continue;
    }
    if (forGenericInit && k < 1) {
      // Initializer work-around: Skip 'this' for generic initializers
      continue;
    }
    if (!fa1->actualType().hasTypePtr()) continue;

    // if (fa1->formalType().kind() == uast::Qualifier::OUT) {
      // continue; // type comes from call site so ignore it here
      // think this is embedded in query
    // }

    const Type* actualType = fa1->actualType().type();
    const Type* formalType = fa1->formalType().type();

    auto canPass = CanPassResult::canPass(context,
                                          fa1->actualType(),
                                          fa1->formalType());

    if (canPass.passes() &&
        canPass.conversionKind() == CanPassResult::ConversionKind::PARAM_NARROWING) {
      numParamNarrowing++;
    }

    if (isNegativeParamToUnsigned(fa1->actualType().param(), actualType, formalType)) {
      anyNegParamToUnsigned = true;
    }

    if (actualType == formalType) {
      // same type, nothing else to worry about here
      continue;
    }

    if (canPass.passes() &&
        canPass.conversionKind() == CanPassResult::ConversionKind::NONE &&
        !canPass.promotes()) {
      continue;
    }

    if (actualType->isNilType()) {
      // don't worry about converting 'nil' to something else
      continue;
    }

    // Not counting real/imag/complex avoids an ambiguity with
    //  proc f(x: complex(64), y: complex(64))
    //  proc f(x: complex(128), y: complex(128))
    //  f(myInt64, myImag32)
    if (isMatchingImagComplex(actualType, formalType)) {
      // don't worry about imag vs complex
      continue;
    }

    // Not counting tuple value vs referential tuple changes
    if (actualType->isTupleType() && formalType->isTupleType()) {
      auto actualNormTup = normalizeTupleTypeToValueTuple(context,
                                                          fa1->actualType().type());
      auto formalNormTup = normalizeTupleTypeToValueTuple(context,
                                                          fa1->formalType().type());
      if (actualNormTup == formalNormTup) {
        // it is only a change in the tuple ref-ness
        continue;
      }
    }

    if (canPass.passes() && canPass.promotes()) {
      actualType = getPromotionType(context, fa1->actualType()).type();
      gdbShouldBreakHere();
      this->promotedFormals[fa1->formal()->id()] = fa1->actualType();
    }

    nImplicitConversions++;
  }

  // save the computed details in the ResolutionCandidate
  this->nImplicitConversionsComputed = true;
  this->anyNegParamToUnsigned = anyNegParamToUnsigned;
  this->nImplicitConversions = nImplicitConversions;
  this->nParamNarrowingImplicitConversions = numParamNarrowing;
}

void DisambiguationCandidate::computeConversionInfo(const DisambiguationContext& dctx) {
  computeConversionInfo(dctx.rc->context(), dctx.call->numActuals());
}

static void discardWorseConversions(const DisambiguationContext& dctx,
                                    const CandidatesVec& candidates,
                                    std::vector<bool>& discarded) {
  int minImpConv = INT_MAX;
  int maxImpConv = INT_MIN;

  for (size_t i = 0; i < candidates.size(); i++) {
    if (discarded[i]) {
      continue;
    }

    DisambiguationCandidate* candidate = (DisambiguationCandidate*)candidates[i];
    candidate->computeConversionInfo(dctx);
    int impConv = candidate->nImplicitConversions;
    if (impConv < minImpConv) {
      minImpConv = impConv;
    }
    if (impConv > maxImpConv) {
      maxImpConv = impConv;
    }
  }

  if (minImpConv < maxImpConv) {
    for (size_t i = 0; i < candidates.size(); i++) {
      if (discarded[i]) {
        continue;
      }

      const DisambiguationCandidate* candidate = candidates[i];
      int impConv = candidate->nImplicitConversions;
      if (impConv > minImpConv) {
        EXPLAIN("X: Fn %lu has more implicit conversions\n", i);
        discarded[i] = true;
      }
    }
  }

  int numWithNegParamToSigned = 0;
  int numNoNegParamToSigned = 0;
  for (size_t i = 0; i < candidates.size(); i++) {
    if (discarded[i]) {
      continue;
    }

    DisambiguationCandidate* candidate = (DisambiguationCandidate*)candidates[i];
    candidate->computeConversionInfo(dctx);
    if (candidate->anyNegParamToUnsigned) {
      numWithNegParamToSigned++;
    } else {
      numNoNegParamToSigned++;
    }
  }

  if (numWithNegParamToSigned > 0 && numNoNegParamToSigned > 0) {
    for (size_t i = 0; i < candidates.size(); i++) {
      if (discarded[i]) {
        continue;
      }

      const DisambiguationCandidate* candidate = candidates[i];
      if (candidate->anyNegParamToUnsigned) {
        EXPLAIN("X: Fn %lu has negative param to signed and others do not\n", i);
        discarded[i] = true;
      }
    }
  }

  int minNarrowing = INT_MAX;
  int maxNarrowing = INT_MIN;
  for (size_t i = 0; i < candidates.size(); i++) {
    if (discarded[i]) {
      continue;
    }

    DisambiguationCandidate* candidate = (DisambiguationCandidate*)candidates[i];
    candidate->computeConversionInfo(dctx);
    int narrowing = candidate->nParamNarrowingImplicitConversions;
    if (narrowing < minNarrowing) {
      minNarrowing = narrowing;
    }
    if (narrowing > maxNarrowing) {
      maxNarrowing = narrowing;
    }
  }

  if (minNarrowing < maxNarrowing) {
    for (size_t i = 0; i < candidates.size(); i++) {
      if (discarded[i]) {
        continue;
      }

      const DisambiguationCandidate* candidate = candidates[i];
      int narrowing = candidate->nParamNarrowingImplicitConversions;
      if (narrowing > minNarrowing) {
        EXPLAIN("X: Fn %lu has more param narrowing conversions\n", i);
        discarded[i] = true;
      }
    }
  }
}

// If any candidate does not require promotion,
// eliminate candidates that do require promotion.
static void discardWorsePromoting(const DisambiguationContext& dctx,
                                   const CandidatesVec& candidates,
                                   std::vector<bool>&  discarded) {
  int nPromoting = 0;
  int nNotPromoting = 0;
  for (size_t i = 0; i < candidates.size(); ++i) {
    if (discarded[i]) {
      continue;
    }

    const DisambiguationCandidate* candidate = candidates[i];
    if (!candidate->promotedFormals.empty()) {
      nPromoting++;
    } else {
      nNotPromoting++;
    }
  }

  if (nPromoting > 0 && nNotPromoting > 0) {
    for (size_t i = 0; i < candidates.size(); ++i) {
      if (discarded[i]) {
        continue;
      }

      const DisambiguationCandidate* candidate = candidates[i];
      if (!candidate->promotedFormals.empty()) {
        EXPLAIN_DUMP(candidate->fn);
        EXPLAIN("\n\n");
        EXPLAIN("X: Fn %lu promotes but others do not\n", i);
        discarded[i] = true;
      }
    }
  }
}


/**
  Compare two argument mappings, given a set of actual arguments, and set the
  disambiguation state appropriately.

  This function implements the argument mapping comparison component of the
  disambiguation procedure as detailed in the language specification.

  actualIdx is the index within the call of the argument to be compared.

  Sets bits in DisambiguationState ds according to whether argument actualIdx
  in candidate1 vs candidate2 is a better match.

  Returns:
   -1 if the two formals are incomparable
    0 if the two formals have the same level of preference
    1 if fn1 is preferred
    2 if fn2 is preferred
 */
static int testArgMapping(const DisambiguationContext& dctx,
                          const DisambiguationCandidate& candidate1,
                          const DisambiguationCandidate& candidate2,
                          int actualIdx,
                          DisambiguationState& ds,
                          std::string& reason) {

  const FormalActual* fa1 = candidate1.formalActualMap.byActualIdx(actualIdx);
  const FormalActual* fa2 = candidate2.formalActualMap.byActualIdx(actualIdx);

  if (fa1 == nullptr || fa2 == nullptr) {

    return testOpArgMapping(dctx, candidate1, candidate2, actualIdx, ds, reason);
  }

  QualifiedType f1QualType = fa1->formalType();
  QualifiedType f2QualType = fa2->formalType();
  QualifiedType actualQualType = fa1->actualType();
  auto f1Type = f1QualType.type();
  auto f2Type = f2QualType.type();
  auto actualType = actualQualType.type();
  CHPL_ASSERT(actualQualType == fa2->actualType());

  if (!actualQualType.hasTypePtr()) return -1;

  // Give up early for out intent arguments
  // (these don't impact candidate selection)
  if (f1QualType.kind() == QualifiedType::OUT ||
      f2QualType.kind() == QualifiedType::OUT) {
    return -1;
  }

  // Additionally, ignore the difference between referential tuples
  // and value tuples.
  // TODO: not sure how to reproduce this code in Dyno
  if (actualType->isTupleType()) {
    f1QualType = QualifiedType(QualifiedType::Kind::IN, normalizeTupleTypeToValueTuple(dctx.rc->context(), f1Type));
    f2QualType = QualifiedType(QualifiedType::Kind::IN, normalizeTupleTypeToValueTuple(dctx.rc->context(), f2Type));
    actualQualType = QualifiedType(QualifiedType::Kind::IN, normalizeTupleTypeToValueTuple(dctx.rc->context(), actualType));
  }
  // TODO: Not sure we still need this here, based on production
  // Initializer work-around: Skip 'this' for generic initializers
  // if (dctx.call->name() == USTR("init") || dctx.call->name() == USTR("init=")) {
  //   auto nd1 = fa1->formal()->toNamedDecl();
  //   auto nd2 = fa2->formal()->toNamedDecl();
  //   if (nd1 != nullptr && nd2 != nullptr &&
  //       nd1->name() == USTR("this") && nd2->name() == USTR("this")) {
  //     if (getTypeGenericity(dctx.context, f1QualType) != Type::CONCRETE &&
  //         getTypeGenericity(dctx.context, f2QualType) != Type::CONCRETE) {
  //       return;
  //     }
  //   }
  // }

  bool formal1Promotes = false;
  bool formal2Promotes = false;
  bool formal1Narrows = false;
  bool formal2Narrows = false;

  QualifiedType actualScalarT = actualQualType;

  bool f1Param = f1QualType.hasParamPtr();
  bool f2Param = f2QualType.hasParamPtr();

  bool f1Instantiated = fa1->formalInstantiated();
  bool f2Instantiated = fa2->formalInstantiated();

  bool f1InstantiatedFromAny = false;
  bool f2InstantiatedFromAny = false;

  bool f1PartiallyGeneric = false;
  bool f2PartiallyGeneric = false;

  if (f1Instantiated) {
    f1InstantiatedFromAny = isFormalInstantiatedAny(candidate1, fa1);
    f1PartiallyGeneric = isFormalPartiallyGeneric(candidate1, fa1);
  }
  if (f2Instantiated) {
    f2InstantiatedFromAny = isFormalInstantiatedAny(candidate2, fa2);
    f2PartiallyGeneric = isFormalPartiallyGeneric(candidate2, fa2);
  }

  bool actualParam = actualQualType.isParam();
  EXPLAIN("Actual's type: ");
  EXPLAIN_DUMP(&actualQualType);
  if (actualParam)
    EXPLAIN(" (param)");
  EXPLAIN("\n");

  // do some EXPLAIN calls
  testArgMapHelper(dctx, *fa1, candidate1.forwardingTo,
                   &formal1Promotes, &formal1Narrows, ds, 1);

  testArgMapHelper(dctx, *fa2, candidate2.forwardingTo,
                   &formal2Promotes, &formal2Narrows, ds, 2);

  // Figure out scalar type for candidate matching
  if (formal1Promotes || formal2Promotes) {
    actualScalarT = computeActualScalarType(dctx.rc->context(), actualQualType);
  }

  // consider promotion
  if (!formal1Promotes && formal2Promotes) {
    reason = "no promotion vs promotes";
    return 1;
  }

  if (formal1Promotes && !formal2Promotes) {
    reason = "no promotion vs promotes";
    return 2;
  }

  // consider concrete vs generic functions
  // note: the f1Type == f2Type part here is important
  // and it prevents moving this logic out of the pairwise comparison.
  // It is important e.g. for:
  //   class Parent { }
  //   class GenericChild : Parent { type t; }
  // Here a GenericChild argument should be preferred to a Parent one
  if (f1Type == f2Type) {

    if (f1Param && !f2Param) {
      reason = "param vs not";
      return 1;
    }

    if (!f1Param && f2Param) {
      reason = "param vs not";
      return 2;
    }

    if (!f1Instantiated && f2Instantiated) {
      reason = "concrete vs generic";
      return 1;
    }

    if (f1Instantiated && !f2Instantiated) {
      reason = "concrete vs generic";
      return 2;
    }

    if (!f1InstantiatedFromAny && f2InstantiatedFromAny) {
      reason = "generic any vs partially generic/concrete";
      return 1;
    }

    if (f1InstantiatedFromAny && !f2InstantiatedFromAny) {
      reason = "generic any vs partially generic/concrete";
      return 2;
    }

    if (f1PartiallyGeneric && f2Instantiated && !f2PartiallyGeneric) {
      reason = "partially generic vs generic";
      return 1;
    }

    if (f1Instantiated && !f1PartiallyGeneric && f2PartiallyGeneric) {
      reason = "partially generic vs generic";
      return 2;
    }
  }

  if (f1Param && !f2Param) {
    reason = "param vs not";
    return 1;
  }

  if (!f1Param && f2Param) {
    reason = "param vs not";
    return 2;
  }

  if (f1Type != f2Type) {
    // to help with
    // proc f(x: int(8))
    // proc f(x: int(64))
    // f(myInt32) vs. f(1: int(32)) should behave the same
    if (actualParam) {
      if (!formal1Narrows && formal2Narrows) {
        reason = "param narrows vs not";
        return 1;
      }
      if (formal1Narrows && !formal2Narrows) {
        reason = "param narrows vs not";
        return 2;
      }
    }
    // e.g. to help with
    //   sin(1) calling the real(64) version (vs real(32) version)
    //
    //   proc f(complex(64), complex(64))
    //   proc f(complex(128), complex(128))
    //   f(1.0, 1.0i) calling the complex(128) version

    int p = prefersNumericCoercion(dctx, f1QualType, f2QualType, actualScalarT, reason);

    if (p == 1) {
      return 1;
    }
    if (p == 2) {
      return 2;
    }

    if (actualType == f1Type && actualType != f2Type) {
      reason = "actual type vs not";
      return 1;
    }

    if (actualType == f2Type && actualType != f1Type) {
      reason = "actual type vs not";
      return 2;
    }
    auto actualScalarType = actualScalarT.type();
    if (actualScalarType == f1Type && actualScalarType != f2Type) {
      reason = "scalar type vs not";
      return 1;
    }

    if (actualScalarType == f2Type && actualScalarType != f1Type) {
      reason = "scalar type vs not";
      return 2;
    }


    bool fn1Dispatches = moreSpecificCanDispatch(dctx, f1QualType, f2QualType);
    bool fn2Dispatches = moreSpecificCanDispatch(dctx, f2QualType, f1QualType);
    if (fn1Dispatches && !fn2Dispatches) {
      reason = "can dispatch";
      return 1;
    }
    if (!fn1Dispatches && fn2Dispatches) {
      reason = "can dispatch";
      return 2;
    }
  }

  if (f1Type == f2Type) {
    // the formals are the same in terms of preference
    return 0;
  }

  // the formals are incomparable
  return -1;
}


static void testArgMapHelper(const DisambiguationContext& dctx,
                             const FormalActual& fa,
                             const QualifiedType& forwardingTo,
                             bool* formalPromotes, bool* formalNarrows,
                             DisambiguationState& ds,
                             int fnNum) {

  QualifiedType actualType = fa.actualType();
  QualifiedType formalType = fa.formalType();

  // If we got to this point, actual type should be passable to the
  // formal type. (If not, it should have been filtered out when
  // filtering candidates).
  // But, here we want to check if it narrows or promotes
  // since that affects the disambiguation.

  if (forwardingTo.type() != nullptr) {
    actualType = forwardingTo;
  }
  CanPassResult result = canPass(dctx.rc->context(), actualType, formalType);
  CHPL_ASSERT(result.passes());
  *formalPromotes = result.promotes();
  *formalNarrows = result.convertsWithParamNarrowing();

  EXPLAIN("Formal %d's type: ", fnNum);
  EXPLAIN_DUMP(&formalType);
  if (*formalPromotes)
    EXPLAIN(" (promotes)");
  if (formalType.hasParamPtr())
    EXPLAIN(" (instantiated param)");
  if (*formalNarrows)
    EXPLAIN(" (narrows param)");
  EXPLAIN("\n");

  if (actualType.type() != formalType.type()) {
    if (actualType.hasParamPtr()) {
      EXPLAIN("Actual requires param coercion to match formal %d\n", fnNum);
    } else {
      EXPLAIN("Actual requires coercion to match formal %d\n", fnNum);
    }
  }
}


/** Is the formal an instantiation of the any-type, e.g.

    proc f(arg)

    or

    proc g(arg: ?t = 3)
 */
static bool isFormalInstantiatedAny(const DisambiguationCandidate& candidate,
                                    const FormalActual* fa) {

  if (candidate.fn->instantiatedFrom() != nullptr) {
    auto initial = candidate.fn->instantiatedFrom();
    CHPL_ASSERT(initial->instantiatedFrom() == nullptr);

    int formalIdx = fa->formalIdx();
    const QualifiedType& qt = initial->formalType(formalIdx);

    if (qt.type() && qt.type()->isAnyType())
      return true;

    if (initial->untyped()->formalIsVarArgs(formalIdx) &&
        qt.type() && qt.type()->isTupleType()) {
      auto tt = qt.type()->toTupleType();
      CHPL_ASSERT(tt && tt->isStarTuple());
      return tt->starType().type() && tt->starType().type()->isAnyType();
    }
  }

  return false;
}

/**
 Is the formal partially generic, syntactically.

 Some examples:

    proc f(arg: [] int)
    proc f(arg: GenericRecord(int, integral))
    proc f(arg: (int, ?t))
 */
static bool isFormalPartiallyGeneric(const DisambiguationCandidate& candidate,
                                     const FormalActual* fa) {
  // TODO
  return false;
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

static numeric_type_t classifyNumericType(const Type* t)
{
  if (t->isBoolType()) return NUMERIC_TYPE_BOOL;
  if (t->isEnumType()) return NUMERIC_TYPE_ENUM;
  if (t->isIntType()) return NUMERIC_TYPE_INT_UINT;
  if (t->isUintType()) return NUMERIC_TYPE_INT_UINT;
  if (t->isRealType()) return NUMERIC_TYPE_REAL;
  if (t->isImagType()) return NUMERIC_TYPE_IMAG;
  if (t->isComplexType()) return NUMERIC_TYPE_COMPLEX;

  return NUMERIC_TYPE_NON_NUMERIC;
}

static bool isDefaultInt(const Type* t) {
  if (auto tt = t->toIntType())
    return tt->isDefaultWidth();

  return false;
}

static bool isDefaultUint(const Type* t) {
  if (auto tt = t->toUintType())
    return tt->isDefaultWidth();

  return false;
}

static bool isDefaultImag(const Type* t) {
  if (auto tt = t->toImagType())
    return tt->isDefaultWidth();

  return false;
}

static bool isDefaultReal(const Type* t) {
  if (auto tt = t->toRealType())
    return tt->isDefaultWidth();

  return false;
}

static bool isDefaultComplex(const Type* t) {
  if (auto tt = t->toComplexType())
    return tt->isDefaultWidth();

  return false;
}

static int bitwidth(const Type* t) {
  if (auto tt = t->toPrimitiveType())
    return tt->bitwidth();

  return 0;
}

// Returns:
//   -1 if 't' is not a numeric type
//   0 if 't' is a default numeric type ('int' 'bool' etc)
//   n a positive integer width if 't' is a non-default numeric type
static int classifyNumericWidth(QualifiedType* qt)
{
  const Type* t = qt->type();
  // The default size counts as 0
  if (isDefaultInt(t)     ||
      isDefaultUint(t)    ||
      isDefaultReal(t)    ||
      isDefaultImag(t)    ||
      isDefaultComplex(t))
    return 0;

  // Bool size 64 should be considered the same as int 64
  // and just treat all bools the same
  // to prefer the default size (i.e. int)
  if (t->isBoolType())
    return 0;

  if (t->isIntType()  ||
      t->isUintType() ||
      t->isRealType() ||
      t->isImagType() )
    return bitwidth(t);

  if (t->isComplexType())
    return bitwidth(t) / 2;

  return -1;
}

// This method implements rules such as that a bool would prefer to
// coerce to 'int' over 'int(8)'.
// Returns
//  0 if there is no preference
//  1 if f1Type is better
//  2 if f2Type is better
static int prefersNumericCoercion(const DisambiguationContext& dctx,
                                  QualifiedType f1Qt,
                                  QualifiedType f2Qt,
                                  QualifiedType actualQt,
                                  std::string &reason) {

  const Type* actualType = actualQt.type();
  const Type* f1Type = f1Qt.type();
  const Type* f2Type = f2Qt.type();

  int acWidth = classifyNumericWidth(&actualQt);
  int f1Width = classifyNumericWidth(&f1Qt);
  int f2Width = classifyNumericWidth(&f2Qt);

  if (acWidth < 0 || f1Width < 0 || f2Width < 0) {
    // something is not a numeric type
    return 0;
  }

  numeric_type_t acKind = classifyNumericType(actualType);
  numeric_type_t f1Kind = classifyNumericType(f1Type);
  numeric_type_t f2Kind = classifyNumericType(f2Type);

  if (acKind == f1Kind && acKind != f2Kind) {
    reason = "same numeric kind";
    return 1;
  }
  if (acKind != f1Kind && acKind == f2Kind) {
    reason = "same numeric kind";
    return 2;
  }

  // Otherwise, prefer the function with the same numeric width
  // as the actual. This rule helps this case:
  //
  //  proc f(arg: real(32))
  //  proc f(arg: real(64))
  //  f(myInt64)
  //
  // here we desire to call f(real(64)) e.g. for sin(1)
  //
  // Additionally, it impacts this case:
  //  proc f(a: real(32), b: real(32))
  //  proc f(a: real(64), b: real(64))
  //  f(myInt64, 1.0)
  // (it arranges for it to call the real(64) version vs the real(32) one)
  if (acWidth == f1Width && acWidth != f2Width) {
    reason = "same numeric width";
    return 1;
  }

  if (acWidth != f1Width && acWidth == f2Width) {
    reason = "same numeric width";
    return 2;
  }

  // note that if in the future we allow more numeric coercions,
  // we might need to make this function complete
  // (where currently it falls back on the "can dispatch" check
  //  in some cases). E.g. it could finish up by comparing
  // the two formal types in terms of their index in this list:
  //
  //  int(8) uint(8) int(16) uint(16) int(32) uint(32) int(64) uint(64)
  //  real(32) real(64) imag(32) real(64) complex(64) complex(128)

  return 0;
}

static QualifiedType computeActualScalarType(Context* context,
                                             QualifiedType actualType) {
  return getPromotionType(context, actualType);
}


static bool moreSpecificCanDispatch(const DisambiguationContext& dctx,
                         QualifiedType actualType, QualifiedType formalType) {
  CanPassResult result = canPass(dctx.rc->context(), actualType, formalType);

  return result.passes();
}


} // end namespace resolution


} // end namespace chpl
