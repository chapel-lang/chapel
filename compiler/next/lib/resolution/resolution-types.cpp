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

#include "chpl/resolution/resolution-types.h"

#include "chpl/queries/query-impl.h"
#include "chpl/queries/update-functions.h"
#include "chpl/uast/Builder.h"
#include "chpl/uast/Formal.h"

namespace chpl {
namespace resolution {

using namespace uast;

const owned<UntypedFnSignature>&
UntypedFnSignature::getUntypedFnSignature(Context* context, ID id,
                                          UniqueString name,
                                          bool isMethod,
                                          bool idIsFunction,
                                          bool isTypeConstructor,
                                          uast::Function::Kind kind,
                                          std::vector<FormalDetail> formals,
                                          const Expression* whereClause) {
  QUERY_BEGIN(getUntypedFnSignature, context,
              id, name, isMethod, idIsFunction, isTypeConstructor,
              kind, formals, whereClause);

  owned<UntypedFnSignature> result =
    toOwned(new UntypedFnSignature(id, name, isMethod,
                                   idIsFunction, isTypeConstructor,
                                   kind, std::move(formals), whereClause));

  return QUERY_END(result);
}

const UntypedFnSignature*
UntypedFnSignature::get(Context* context, ID id,
                        UniqueString name,
                        bool isMethod,
                        bool idIsFunction,
                        bool isTypeConstructor,
                        uast::Function::Kind kind,
                        std::vector<FormalDetail> formals,
                        const uast::Expression* whereClause) {
  return getUntypedFnSignature(context, id, name, isMethod,
                               idIsFunction, isTypeConstructor, kind,
                               std::move(formals), whereClause).get();
}

const UntypedFnSignature*
UntypedFnSignature::get(Context* context, const uast::Function* fn) {
  const UntypedFnSignature* result = nullptr;

  if (fn != nullptr) {
    // compute the FormalDetails
    std::vector<FormalDetail> formals;
    for (auto decl : fn->formals()) {
      UniqueString name;
      bool hasDefault = false;
      if (auto formal = decl->toFormal()) {
        name = formal->name();
        hasDefault = formal->initExpression() != nullptr;
      }

      formals.push_back(FormalDetail(name, hasDefault, decl));
    }

    // find the unique-ified untyped signature
    result = get(context, fn->id(),
                 fn->name(), fn->isMethod(),
                 /* idIsFunction */ true,
                 /* isTypeConstructor */ false,
                 fn->kind(),
                 std::move(formals), fn->whereClause());
  }

  return result;
}


void ResolutionResultByPostorderID::setupForSymbol(const ASTNode* ast) {
  assert(Builder::astTagIndicatesNewIdScope(ast->tag()));
  vec.resize(ast->id().numContainedChildren());

  symbolId = ast->id();
}
void ResolutionResultByPostorderID::setupForSignature(const Function* func) {
  int bodyPostorder = 0;
  if (func && func->body())
    bodyPostorder = func->body()->id().postOrderId();
  assert(0 <= bodyPostorder);
  vec.resize(bodyPostorder);

  symbolId = func->id();
}
void ResolutionResultByPostorderID::setupForFunction(const Function* func) {
  setupForSymbol(func);
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
  byFormalIdx.clear();
  actualIdxToFormalIdx.clear();
  mappingIsValid = false;
  failingActualIdx = -1;

  // allocate space in the arrays
  byFormalIdx.resize(untyped->numFormals());
  actualIdxToFormalIdx.resize(call.numActuals());

  // initialize the FormalActual parts from the Formals
  int formalIdx = 0;
  int numFormals = untyped->numFormals();
  for (int i = 0; i < numFormals; i++) {
    FormalActual& entry = byFormalIdx[formalIdx];
    if (const Decl* decl = untyped->formalDecl(i)) {
      entry.formal = decl;
    }
    if (typed) {
      entry.formalType = typed->formalType(formalIdx);
    }
    entry.hasActual = false;
    entry.actualIdx = -1;

    formalIdx++;
  }

  // Match named actuals against formal names in the function signature.
  // Record successful matches in actualIdxToFormalIdx.

  int actualIdx = 0;
  for (const CallInfoActual& actual : call.actuals()) {
    if (!actual.byName().isEmpty()) {
      bool match = false;
      for (int i = 0; i < numFormals; i++) {
        if (actual.byName() == untyped->formalName(i)) {
          match = true;
          FormalActual& entry = byFormalIdx[i];
          entry.hasActual = true;
          entry.actualIdx = actualIdx;
          entry.actualType = actual.type();
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
  for (const CallInfoActual& actual : call.actuals()) {
    if (formalIdx >= (int) byFormalIdx.size()) {
      // too many actuals
      mappingIsValid = false;
      failingActualIdx = actualIdx;
      return false;
    }

    if (actual.byName().isEmpty()) {
      // Skip any formals already matched to named arguments
      while (byFormalIdx[formalIdx].actualIdx >= 0) {
        if (formalIdx + 1 >= (int) byFormalIdx.size()) {
          // too many actuals
          mappingIsValid = false;
          failingActualIdx = actualIdx;
          return false;
        }
        formalIdx++;
      }
      assert(formalIdx < (int) byFormalIdx.size());

      // TODO: special handling for operators

      FormalActual& entry = byFormalIdx[formalIdx];
      entry.hasActual = true;
      entry.actualIdx = actualIdx;
      entry.actualType = actual.type();
      actualIdxToFormalIdx[actualIdx] = formalIdx;
    }
    actualIdx++;
  }

  if (!untyped->isTypeConstructor()) {
    // Make sure that any remaining formals are matched by name
    // or have a default value.
    // This is left out for type constructors because presently
    // a partial instantiation is provided by simply leaving out arguments.
    while (formalIdx < (int) byFormalIdx.size()) {
      if (byFormalIdx[formalIdx].actualIdx < 0) {
        if (!untyped->formalHasDefault(formalIdx)) {
          // formal was not provided and there is no default value
          mappingIsValid = false;
          failingFormalIdx = formalIdx;
          return false;
        }
      }
      formalIdx++;
    }
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
  bool ok = ret.computeAlignment(sig->untyped(), sig, call);
  if (!ok) {
    ret.mappingIsValid = false;
  }
  return ret;
}

std::string TypedFnSignature::toString() const {
  std::string ret = id().toString();
  ret += "(";
  int nFormals = numFormals();
  for (int i = 0; i < nFormals; i++) {
    if (i != 0) {
      ret += ", ";
    }
    ret += formalName(i).toString();
    ret += " : ";
    ret += formalType(i).toString();
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
