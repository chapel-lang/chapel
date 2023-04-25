/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/DomainType.h"

#include "chpl/framework/query-impl.h"
#include "chpl/resolution/intents.h"
#include "chpl/types/Param.h"
#include "chpl/types/TupleType.h"

namespace chpl {
namespace types {


void DomainType::stringify(std::ostream& ss,
                           chpl::StringifyKind stringKind) const {
  if (kind_ == Kind::Rectangular) {
    ss << "domain(";
    rank().param()->stringify(ss, stringKind);
    ss << ",";
    idxType().type()->stringify(ss, stringKind);
    ss << ",";
    stridable().param()->stringify(ss, stringKind);
    ss << ")";
  } else if (kind_ == Kind::Associative) {
    ss << "domain(";
    idxType().type()->stringify(ss, stringKind);
    ss << ",";
    parSafe().param()->stringify(ss, stringKind);
    ss << ")";
  } else if (kind_ == Kind::Unknown) {
    ss << "domain(?)";
  } else {
    CHPL_ASSERT(false && "all domain kinds should be handled.");
  }
}

static ID getDomainID(Context* context) {
  auto symbolPath = UniqueString::get(context, "ChapelDomain._domain");
  return ID(symbolPath, -1, 0);
}

const owned<DomainType>&
DomainType::getDomainType(Context* context, ID id, UniqueString name,
                          const DomainType* instantiatedFrom,
                          SubstitutionsMap subs,
                          DomainType::Kind kind) {
  QUERY_BEGIN(getDomainType, context, id, name, instantiatedFrom, subs, kind);
  auto result = toOwned(new DomainType(id, name, instantiatedFrom,
                                       std::move(subs), kind));
  return QUERY_END(result);
}

const DomainType*
DomainType::getGenericDomainType(Context* context) {
  auto name = UniqueString::get(context, "_domain");
  auto id = getDomainID(context);
  SubstitutionsMap subs;
  const DomainType* instantiatedFrom = nullptr;
  return getDomainType(context, id, name, instantiatedFrom, subs).get();
}

const DomainType*
DomainType::getRectangularType(Context* context,
                               const QualifiedType& rank,
                               const QualifiedType& idxType,
                               const QualifiedType& stridable) {
  SubstitutionsMap subs;
  subs.emplace(ID(UniqueString(), 0, 0), rank);
  subs.emplace(ID(UniqueString(), 1, 0), idxType);
  subs.emplace(ID(UniqueString(), 2, 0), stridable);
  auto name = UniqueString::get(context, "_domain");
  auto id = getDomainID(context);
  auto instantiatedFrom = getGenericDomainType(context);
  return getDomainType(context, id, name, instantiatedFrom, subs,
                       DomainType::Kind::Rectangular).get();
}

const DomainType*
DomainType::getAssociativeType(Context* context,
                               const QualifiedType& idxType,
                               const QualifiedType& parSafe) {
  SubstitutionsMap subs;
  subs.emplace(ID(UniqueString(), 0, 0), idxType);
  subs.emplace(ID(UniqueString(), 1, 0), parSafe);
  auto name = UniqueString::get(context, "_domain");
  auto id = getDomainID(context);
  auto instantiatedFrom = getGenericDomainType(context);
  return getDomainType(context, id, name, instantiatedFrom, subs,
                       DomainType::Kind::Associative).get();
}


} // end namespace types
} // end namespace chpl
