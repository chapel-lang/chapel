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

#include "chpl/types/CompositeType.h"

#include "chpl/types/BasicClassType.h"
#include "chpl/uast/NamedDecl.h"
#include "chpl/uast/Decl.h"

namespace chpl {
namespace types {


bool CompositeType::compositeTypeContentsMatchInner(
    const CompositeType* other, MatchAssumptions& assumptions) const {
  // check basic properties
  if (id_ != other->id_ || name_ != other->name_)
    return false;

  // one is instantiated but the other is not
  if ((instantiatedFrom_ != nullptr) !=
      (other->instantiatedFrom_ != nullptr))
    return false;

  // add instantiatedFrom to the assumptions
  if (!assumptions.assume(instantiatedFrom_, other->instantiatedFrom_))
    return false;

  // consider the substitutions
  auto sorted = sortedSubstitutions();
  auto otherSorted = other->sortedSubstitutions();

  if (sorted.size() != otherSorted.size())
    return false;

  // TODO: is the loop here necessary? Could this just
  // use == on the SubstitutionMaps?

  size_t nSubs = sorted.size();
  for (size_t i = 0; i < nSubs; i++) {
    ID declId = sorted[i].first;
    ID otherDeclId = otherSorted[i].first;
    QualifiedType qt = sorted[i].second;
    QualifiedType otherQt = otherSorted[i].second;

    if (declId != otherDeclId)
      return false;

    // Check the elements of the QualifiedType individually,
    // because we want to handle the Type* in a special way.
    if (qt.kind() != otherQt.kind() ||
        qt.param() != otherQt.param())
      return false;

    const Type* fieldType = qt.type();
    const Type* otherFieldType = otherQt.type();

    // type should be established by the time this runs
    assert(fieldType != nullptr);

    // add an assumption about the type ptrs, if they differ
    if (!assumptions.assume(fieldType, otherFieldType))
      return false;
  }

  return true;
}

CompositeType::~CompositeType() {
}

void CompositeType::stringify(std::ostream& ss,
                              chpl::StringifyKind stringKind) const {
  // compute the parent class type for BasicClassType
  const Type* superType = nullptr;
  if (auto bct = this->toBasicClassType()) {
    superType = bct->parentClassType();
  }

  //std::string ret = typetags::tagToString(tag());
  name().stringify(ss, stringKind);

  auto sorted = sortedSubstitutions();

  if (superType || !sorted.empty()) {
    bool emittedField = false;
    ss << "(";

    if (superType != nullptr) {
      ss << "super:";
      superType->stringify(ss, stringKind);
      emittedField = true;
    }

    for (auto sub : sorted) {
      if (emittedField) ss << ", ";
      sub.first.stringify(ss, stringKind);
      ss << ":";
      sub.second.stringify(ss, stringKind);
      emittedField = true;
    }
    ss << ")";
  }
}

using SubstitutionPair = CompositeType::SubstitutionPair;

struct SubstitutionsMapCmp {
  bool operator()(const SubstitutionPair& x, const SubstitutionPair& y) {
    return x.first < y.first;
  }
};

static
std::vector<SubstitutionPair>
sortedSubstitutionsMap(const CompositeType::SubstitutionsMap& subs) {
  // since it's an unordered map, iteration will occur in a
  // nondeterministic order.
  // it's important to sort the keys / iterate in a deterministic order here,
  // so we create a vector of pair<K,V> and sort that instead
  std::vector<SubstitutionPair> v(subs.begin(), subs.end());
  SubstitutionsMapCmp cmp;
  std::sort(v.begin(), v.end(), cmp);
  return v;
}

std::vector<SubstitutionPair> CompositeType::sortedSubstitutions(void) const {
  return sortedSubstitutionsMap(subs_);
}

size_t hashSubstitutionsMap(const CompositeType::SubstitutionsMap& subs) {
  auto sorted = sortedSubstitutionsMap(subs);
  return hashVector(sorted);
}

void stringifySubstitutionsMap(std::ostream& streamOut,
                               StringifyKind stringKind,
                               const CompositeType::SubstitutionsMap& subs) {
  auto sorted = sortedSubstitutionsMap(subs);
  bool first = true;
  for (auto const& x : sorted)
  {
    ID id = x.first;
    QualifiedType qt = x.second;

    if (first) {
      first = false;
    } else {
      streamOut << ", ";
    }

    id.stringify(streamOut, stringKind);

    streamOut << "= ";

    qt.stringify(streamOut, stringKind);
  }
}


} // end namespace types
} // end namespace chpl
