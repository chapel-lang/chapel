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

#include "chpl/queries/update-functions.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/uast/Builder.h"
#include "chpl/uast/Formal.h"

namespace chpl {
namespace resolution {

using namespace uast;

void ResolutionResultByPostorderID::resizeForSymbol(const ASTNode* ast) {
  assert(Builder::astTagIndicatesNewIdScope(ast->tag()));
  vec.resize(ast->id().numContainedChildren());
}
void ResolutionResultByPostorderID::resizeForSignature(const Function* func) {
  int bodyPostorder = 0;
  if (func && func->body())
    bodyPostorder = func->body()->id().postOrderId();
  assert(0 <= bodyPostorder);
  vec.resize(bodyPostorder);
}

bool ResolutionResultByPostorderID::update(ResolutionResultByPostorderID& keep,
                                           ResolutionResultByPostorderID& addin)
{
  return defaultUpdateVec(keep.vec, addin.vec);
}

bool FormalActualMap::computeAlignment(const UntypedFnSignature* untyped,
                                       const TypedFnSignature* typed,
                                       const CallInfo& call) {

  // untyped must be provided but typed can be nullptr.
  assert(untyped);

  // create the mapping handling named and default arguments

  // clear the current mapping
  byFormalIdx.empty();
  actualIdxToFormalIdx.empty();
  mappingIsValid = false;
  failingActualIdx = -1;

  // allocate space in the arrays
  byFormalIdx.resize(untyped->formals.size());
  actualIdxToFormalIdx.resize(call.actuals.size());

  // initialize the FormalActual parts from the Formals
  size_t formalIdx = 0;
  for (const Formal* formal : untyped->formals) {
    FormalActual& entry = byFormalIdx[formalIdx];
    entry.formal = formal;
    if (typed) {
      entry.formalType = typed->formalTypes[formalIdx];
    }
    entry.hasActual = false;
    entry.actualIdx = -1;

    formalIdx++;
  }

  // Match named actuals against formal names in the function signature.
  // Record successful matches in actualIdxToFormalIdx.

  size_t actualIdx = 0;
  for (const CallInfoActual& actual : call.actuals) {
    if (!actual.byName.isEmpty()) {
      bool match = false;
      int formalIdx = 0;
      for (const Formal* formal : untyped->formals) {
        if (actual.byName == formal->name()) {
          match = true;
          FormalActual& entry = byFormalIdx[formalIdx];
          entry.hasActual = true;
          entry.actualIdx = actualIdx;
          entry.actualType = actual.type;
          actualIdxToFormalIdx[actualIdx] = formalIdx;
          break;
        }
        formalIdx++;
      }

      // Fail if no matching formal is found.
      if (match == false) {
        mappingIsValid = false;
        failingActualIdx = actualIdx;
        // TODO: track failure type for error messages
        return false;
      }
    }
    actualIdx++;
  }

  // Fill in unmatched formals in sequence with the remaining actuals.
  // Record successful substitutions
  formalIdx = 0;
  actualIdx = 0;
  for (const CallInfoActual& actual : call.actuals) {
    if (actual.byName.isEmpty()) {
      // Skip any formals already matched to named arguments
      while (byFormalIdx[formalIdx].actualIdx >= 0) {
        if (formalIdx + 1 >= byFormalIdx.size()) {
          // too many actuals
          mappingIsValid = false;
          failingActualIdx = actualIdx;
          return false;
        }
        formalIdx++;
      }
      assert(formalIdx < byFormalIdx.size());

      // TODO: special handling for operators

      FormalActual& entry = byFormalIdx[formalIdx];
      entry.hasActual = true;
      entry.actualIdx = actualIdx;
      entry.actualType = actual.type;
      actualIdxToFormalIdx[actualIdx] = formalIdx;
    }
    actualIdx++;
  }

  // Make sure that any remaining formals are matched by name
  // or have a default value.
  while (formalIdx < byFormalIdx.size()) {
    if (byFormalIdx[formalIdx].actualIdx < 0 &&
        untyped->formals[formalIdx]->initExpression() == nullptr) {
      // formal was not provided and there is no default value
      mappingIsValid = false;
      failingFormalIdx = formalIdx;
      return false;
    }
    formalIdx++;
  }

  mappingIsValid = true;
  return true;
}

FormalActualMap FormalActualMap::build(const UntypedFnSignature* sig,
                                       const CallInfo& call) {

  FormalActualMap ret;
  bool ok = ret.computeAlignment(sig, nullptr, call);
  if (!ok) {
    ret.mappingIsValid = false;
  }
  return ret;
}

FormalActualMap FormalActualMap::build(const TypedFnSignature* sig,
                                       const CallInfo& call) {
  FormalActualMap ret;
  bool ok = ret.computeAlignment(sig->untypedSignature, sig, call);
  if (!ok) {
    ret.mappingIsValid = false;
  }
  return ret;
}

std::string TypedFnSignature::toString() const {
  std::string ret = untypedSignature->functionId.toString();
  ret += "(";
  for (size_t i = 0; i < untypedSignature->formals.size(); i++) {
    if (i != 0) {
      ret += ", ";
    }
    ret += untypedSignature->formals[i]->name().c_str();
    ret += " : ";
    ret += formalTypes[i].toString();
  }
  ret += ")";

  return ret;
}


void PoiInfo::accumulate(const PoiInfo& addPoiInfo) {
  poiFnIdsUsed.insert(addPoiInfo.poiFnIdsUsed.begin(),
                      addPoiInfo.poiFnIdsUsed.end());
}

// this is a resolved function
// check is a not-yet-resolved function
bool PoiInfo::canReuse(const PoiInfo& check) const {
  assert(resolved && !check.resolved);

  return false; // TODO -- consider function names etc -- see PR #16261
}

std::string ResolvedExpression::toString() const {
  std::string ret;
  ret += " : ";
  ret += type.toString();
  ret += " ; ";
  if (!toId.isEmpty()) {
    ret += " refers to ";
    ret += toId.toString();
  } else {
    auto onlyFn = mostSpecific.only();
    if (onlyFn) {
      ret += " calls ";
      ret += onlyFn->toString();
    } else {
      if (auto sig = mostSpecific.bestRef()) {
        ret += " calls ref ";
        ret += sig->toString();
      }
      if (auto sig = mostSpecific.bestConstRef()) {
        ret += " calls const ref ";
        ret += sig->toString();
      }
      if (auto sig = mostSpecific.bestValue()) {
        ret += " calls value ";
        ret += sig->toString();
      }
    }
  }

  return ret;
}


} // end namespace resolution
} // end namespace chpl
