/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RESOLUTION_RESOLUTION_TYPES_H
#define CHPL_RESOLUTION_RESOLUTION_TYPES_H

#include "chpl/resolution/scope-types.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/types/Type.h"
#include "chpl/uast/ASTNode.h"
#include "chpl/uast/Function.h"
#include "chpl/util/memory.h"

#include <unordered_map>
#include <utility>

namespace chpl {
namespace resolution {

// TODO: Should some/all of these structs be classes
// with getters etc? That would be appropriate for
// use as part of the library API.

/**
  An untyped function signature. This is really just the part of a function
  including the formals. It exists so that the process of identifying
  candidates does not need to depend on the bodies of the function
  (in terms of incremental recomputation).
 */
struct UntypedFnSignature {
  ID functionId;
  UniqueString name;
  bool isMethod; // in that case, formals[0] is the receiver
  uast::Function::Kind kind;
  std::vector<const uast::Formal*> formals;
  const uast::Expression* whereClause;

  UntypedFnSignature(const uast::Function* fn)
    : functionId(fn->id()),
      name(fn->name()),
      isMethod(fn->isMethod()),
      kind(fn->kind()),
      whereClause(fn->whereClause()) {
    for (auto formal : fn->formals()) {
      formals.push_back(formal);
    }
  }

  bool operator==(const UntypedFnSignature& other) const {
    return functionId != other.functionId &&
           name == other.name &&
           isMethod == other.isMethod &&
           kind == other.kind &&
           formals == other.formals &&
           whereClause == other.whereClause;
  }
  bool operator!=(const UntypedFnSignature& other) const {
    return !(*this == other);
  }
};

using SubstitutionsMap = std::unordered_map<const uast::Formal*, types::QualifiedType>;

/*
struct InstantiationInfo {
  SubstitutionsMap substitutions;
  TypedFnSignature* instantiationPointFn = nullptr;
  ID instantiationPointId;

  bool operator==(const InstantiationInfo& other) const {
    return substitutions == other.substitutions &&
           instantiationPointFn == other.instantiationPointFn &&
           instantiationPointId == other.instantiationPointId;
  }
  bool operator!=(const InstantiationInfo& other) const {
    return !(*this == other);
  }
};
*/

struct CallInfoActual {
  types::QualifiedType type;
  UniqueString byName;

  bool operator==(const CallInfoActual& other) const {
    return type == other.type &&
           byName == other.byName;
  }
  bool operator!=(const CallInfoActual& other) const {
    return !(*this == other);
  }
  size_t hash() const {
    size_t h1 = chpl::hash(type);
    size_t h2 = chpl::hash(byName);
    size_t ret = 0;
    ret = hash_combine(ret, h1);
    ret = hash_combine(ret, h2);
    return ret;
  }
};

struct CallInfo {
  UniqueString name;                   // the name of the called thing
  bool isMethod = false;               // in that case, actuals[0] is receiver
  std::vector<CallInfoActual> actuals; // types/params/names of actuals 

  bool operator==(const CallInfo& other) const {
    return name == other.name &&
           isMethod == other.isMethod &&
           actuals == other.actuals;
  }
  bool operator!=(const CallInfo& other) const {
    return !(*this == other);
  }
  size_t hash() const {
    size_t h1 = chpl::hash(name);
    size_t h2 = isMethod;
    size_t h3 = chpl::hash(actuals);
    size_t ret = 0;
    ret = hash_combine(ret, h1);
    ret = hash_combine(ret, h2);
    ret = hash_combine(ret, h3);
    return ret;
  }
};

// When resolving a traditional generic, we also need to consider
// the point-of-instantiation scope as a place to find visible functions.
// This type tracks such a scope.
//
// PoiScopes do not need to consider scopes that are visible from
// the function declaration. These can be collapsed away.
//
// Performance: could have better reuse of PoiScope if it used the Scope ID
// rather than changing if the contents do. But, the downside is that
// further queries would be required to compute which functions are
// visible. Which is better?
// If we want to make PoiScope not depend on the contents it might be nice
// to make Scope itself not depend on the contents, too.
struct PoiScope {
  const Scope* inScope = nullptr;         // parent Scope for the Call
  const PoiScope* inFnPoi = nullptr;      // what is the POI of this POI?

  bool operator==(const PoiScope& other) const {
    return inScope == other.inScope &&
           inFnPoi == other.inFnPoi;
  }
  bool operator!=(const PoiScope& other) const {
    return !(*this == other);
  }
};


struct PoiInfo {
  // For a not-yet-resolved instantiation
  const PoiScope* poiScope;

  // TODO: add VisibilityInfo etc

  // For a resolved instantiation,
  // what are the point-of-instantiation scopes
  // that were needed for resolving the signature?
  std::set<const PoiScope*> poiScopesUsed;

  // TODO: operator == for the hashtable comparison but
  // a different .equals for use in the update function
  // for TypedFnSignature / ResolvedFunction
};

// TODO: should this actually be types::FunctionType?
struct TypedFnSignature {
  typedef enum {
    WHERE_NONE,  // no where clause
    WHERE_TBD,   // where clause not resolved yet
    WHERE_TRUE,  // where resulted in true
    WHERE_FALSE, // where resulted in false
  } WhereClauseResult;

  // What is the untyped function signature?
  const UntypedFnSignature* untypedSignature;
  // What is the type of each of the formals?
  std::vector<types::QualifiedType> formalTypes;
  // If there was a where clause, what was the result of evaluating it?
  WhereClauseResult whereClauseResult = WHERE_TBD;

  // Are any of the formals generic or unknown at this point?
  bool needsInstantiation = true;

  // Is this TypedFnSignature representing an instantiation?
  // If so, what is the generic TypedFnSignature that was instantiated?
  const TypedFnSignature* instantiatedFrom = nullptr;

  // Is this for an inner Function? If so, what is the parent
  // function signature?
  const TypedFnSignature* parentFn = nullptr;

  // If it's an instantiation, what are the point-of-instantiation scopes
  // that were needed for resolving the signature?
  std::set<const PoiScope*> poiScopesUsed;

  // TODO: This could include a substitutions map, if we need it.
  // The formalTypes above might be enough, though.

  bool operator==(const TypedFnSignature& other) const {
    return untypedSignature == other.untypedSignature &&
           formalTypes == other.formalTypes &&
           whereClauseResult == other.whereClauseResult &&
           needsInstantiation == other.needsInstantiation &&
           instantiatedFrom == other.instantiatedFrom &&
           parentFn == other.parentFn &&
           poiScopesUsed == other.poiScopesUsed;
  }
  bool operator!=(const TypedFnSignature& other) const {
    return !(*this == other);
  }

  std::string toString() const;
};

struct MostSpecificCandidates {
  const TypedFnSignature* bestRef = nullptr;
  const TypedFnSignature* bestConstRef = nullptr;
  const TypedFnSignature* bestValue = nullptr;

  const TypedFnSignature* only() {
    const TypedFnSignature* ret = nullptr;
    int i = 0;
    if (bestRef != nullptr) {
      ret = bestRef;
      i++;
    }
    if (bestConstRef != nullptr) {
      ret = bestConstRef;
      i++;
    }
    if (bestValue != nullptr) {
      ret = bestValue;
      i++;
    }
    if (i != 1) {
      return nullptr;
    }
    return ret;
  }

  bool operator==(const MostSpecificCandidates& other) const {
    return bestRef == other.bestRef &&
           bestConstRef == other.bestConstRef &&
           bestValue == other.bestValue;
  }
  bool operator!=(const MostSpecificCandidates& other) const {
    return !(*this == other);
  }
  void swap(MostSpecificCandidates& other) {
    std::swap(bestRef, other.bestRef);
    std::swap(bestConstRef, other.bestConstRef);
    std::swap(bestValue, other.bestValue);
  }
};

struct CallResolutionResult {
  MostSpecificCandidates mostSpecific;
  // if any of the candidates were instantiated, what point-of-instantiation
  // scopes were used when resolving their signature or body?
  std::set<const PoiScope*> poiScopesUsed;

  CallResolutionResult(MostSpecificCandidates mostSpecific,
                       std::set<const PoiScope*> poiScopesUsed)
    : mostSpecific(std::move(mostSpecific)),
      poiScopesUsed(std::move(poiScopesUsed)) {
  }

  bool operator==(const CallResolutionResult& other) const {
    return mostSpecific == other.mostSpecific &&
           poiScopesUsed == other.poiScopesUsed;
  }
  bool operator!=(const CallResolutionResult& other) const {
    return !(*this == other);
  }
  void swap(CallResolutionResult& other) {
    mostSpecific.swap(other.mostSpecific);
    poiScopesUsed.swap(other.poiScopesUsed);
  }
};

struct ResolvedExpression {
  // the ID that is resolved
  ID id;
  // What is its type and param value?
  types::QualifiedType type;
  // For simple (non-function Identifier) cases,
  // the ID of a NamedDecl it refers to
  ID toId;

  // For a function call, what is the most specific candidate,
  // or when using return intent overloading, what are the most specific
  // candidates.
  // The choice between these needs to happen
  // later than the main function resolution.
  MostSpecificCandidates mostSpecific;

  ResolvedExpression() { }

  bool operator==(const ResolvedExpression& other) const {
    return id == other.id &&
           type == other.type &&
           toId == other.toId &&
           mostSpecific == other.mostSpecific;
  }
  bool operator!=(const ResolvedExpression& other) const {
    return !(*this == other);
  }
  void swap(ResolvedExpression& other) {
    id.swap(other.id);
    type.swap(other.type);
    toId.swap(other.toId);
    mostSpecific.swap(other.mostSpecific);
  }
};

// postorder ID (int) -> ResolvedExpression *within* a Function etc
// an inner Function would not be covered here since it would get
// a different ResolvedSymbol entry.
using ResolutionResultByPostorderID = std::vector<ResolvedExpression>;
// TODO: make the above type a struct to have helper methods
// (mainly get existing result by ID)

// A resolution result for a Function
struct ResolvedFunction {
  const TypedFnSignature* signature = nullptr;

  // this is the output of the resolution process
  ResolutionResultByPostorderID resolutionById;

  // the set of point-of-instantiation scopes used by the instantiation
  std::set<const PoiScope*> poiScopesUsed;

  bool operator==(const ResolvedFunction& other) const {
    return signature == other.signature &&
           resolutionById == other.resolutionById &&
           poiScopesUsed == other.poiScopesUsed;
  }
  bool operator!=(const ResolvedFunction& other) const {
    return !(*this == other);
  }
  void swap(ResolvedFunction& other) {
    std::swap(signature, other.signature);
    resolutionById.swap(other.resolutionById);
    poiScopesUsed.swap(other.poiScopesUsed);
  }
};

struct FormalActual {
  const uast::Formal* formal = nullptr;
  types::QualifiedType formalType;
  bool hasActual = false; // == false means uses formal default value
  int actualIdx = -1;
  types::QualifiedType actualType;
};

struct FormalActualMap {
  std::vector<FormalActual> byFormalIdx;
  std::vector<int> actualIdxToFormalIdx;
  bool mappingIsValid = false;
  int failingActualIdx = -1;
  int failingFormalIdx = -1;

  bool computeAlignment(const UntypedFnSignature* untyped,
                        const TypedFnSignature* typed,
                        const CallInfo& call);

  static FormalActualMap build(const UntypedFnSignature* untyped,
                               const CallInfo& call);
  static FormalActualMap build(const TypedFnSignature* typed,
                               const CallInfo& call);
};



} // end namespace resolution


template<> struct update<chpl::resolution::ResolvedExpression> {
  bool operator()(chpl::resolution::ResolvedExpression& keep,
                  chpl::resolution::ResolvedExpression& addin) const {
    return defaultUpdate(keep, addin);
  }
};

template<> struct update<chpl::resolution::MostSpecificCandidates> {
  bool operator()(chpl::resolution::MostSpecificCandidates& keep,
                  chpl::resolution::MostSpecificCandidates& addin) const {
    return defaultUpdate(keep, addin);
  }
};

template<> struct update<chpl::resolution::ResolvedFunction> {
  bool operator()(chpl::resolution::ResolvedFunction& keep,
                  chpl::resolution::ResolvedFunction& addin) const {
    return defaultUpdate(keep, addin);
  }
};



} // end namespace chpl


namespace std {

template<> struct hash<chpl::resolution::CallInfoActual>
{
  size_t operator()(const chpl::resolution::CallInfoActual& key) const {
    return key.hash();
  }
};

template<> struct hash<chpl::resolution::CallInfo>
{
  size_t operator()(const chpl::resolution::CallInfo& key) const {
    return key.hash();
  }
};

} // end namespace std


#endif
