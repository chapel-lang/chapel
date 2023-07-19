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

#include "chpl/types/ArrayType.h"

#include "chpl/framework/query-impl.h"
#include "chpl/resolution/intents.h"
#include "chpl/types/Param.h"

namespace chpl {
namespace types {

const ID ArrayType::domainId = ID(UniqueString(), 0, 0);
const ID ArrayType::eltTypeId = ID(UniqueString(), 1, 0);

void ArrayType::stringify(std::ostream& ss,
                           chpl::StringifyKind stringKind) const {
  if (domainType().isUnknown() && eltType().isUnknown()) {
    ss << "[]";
  } else if (domainType().isUnknown()) {
    ss << "[] ";
    eltType().type()->stringify(ss, stringKind);
  } else {
    ss << "[";
    domainType().type()->stringify(ss, stringKind);
    ss << "] ";
    eltType().type()->stringify(ss, stringKind);
  }
}

static ID getArrayID(Context* context) {
  auto symbolPath = UniqueString::get(context, "ChapelArray._array");
  return ID(symbolPath, -1, 0);
}

const owned<ArrayType>&
ArrayType::getArrayTypeQuery(Context* context, ID id, UniqueString name,
                          const ArrayType* instantiatedFrom,
                          SubstitutionsMap subs) {
  QUERY_BEGIN(getArrayTypeQuery, context, id, name, instantiatedFrom, subs);
  auto result = toOwned(new ArrayType(id, name, instantiatedFrom,
                                       std::move(subs)));
  return QUERY_END(result);
}

const ArrayType*
ArrayType::getGenericArrayType(Context* context) {
  auto name = UniqueString::get(context, "_array");
  auto id = getArrayID(context);
  SubstitutionsMap subs;
  const ArrayType* instantiatedFrom = nullptr;
  return getArrayTypeQuery(context, id, name, instantiatedFrom, subs).get();
}

const ArrayType*
ArrayType::getArrayType(Context* context,
                        const QualifiedType& domainType,
                        const QualifiedType& eltType) {
  SubstitutionsMap subs;
  subs.emplace(ArrayType::domainId, domainType);
  subs.emplace(ArrayType::eltTypeId, eltType);
  auto name = UniqueString::get(context, "_array");
  auto id = getArrayID(context);
  auto instantiatedFrom = getGenericArrayType(context);
  return getArrayTypeQuery(context, id, name, instantiatedFrom, subs).get();
}


} // end namespace types
} // end namespace chpl
