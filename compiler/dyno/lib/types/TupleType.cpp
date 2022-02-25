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

#include "chpl/types/TupleType.h"

#include "chpl/queries/query-impl.h"

namespace chpl {
namespace types {


const owned<TupleType>&
TupleType::getTupleType(Context* context, ID id, UniqueString name,
                        const TupleType* instantiatedFrom,
                        SubstitutionsMap subs) {
  QUERY_BEGIN(getTupleType, context, id, name, instantiatedFrom, subs);

  auto result = toOwned(new TupleType(id, name,
                                      instantiatedFrom, std::move(subs)));

  return QUERY_END(result);
}

static ID idForTupElt(int i) {
  // Use an ID with only a postOrderId but empty name for the tuple elts
  return ID(UniqueString(), i, 0);
}

const TupleType*
TupleType::get(Context* context, std::vector<QualifiedType> eltTypes) {
  SubstitutionsMap subs;
  int i = 0;
  for (auto elt : eltTypes) {
    subs.emplace(idForTupElt(i), elt);
    i++;
  }

  auto name = UniqueString::get(context, "_tuple");
  auto id = ID();
  const TupleType* instantiatedFrom = getGenericTupleType(context);
  return getTupleType(context, id, name, instantiatedFrom,
                      std::move(subs)).get();
}

const TupleType*
TupleType::getGenericTupleType(Context* context) {
  auto name = UniqueString::get(context, "_tuple");
  auto id = ID();
  SubstitutionsMap subs;
  const TupleType* instantiatedFrom = nullptr;
  return getTupleType(context, id, name, instantiatedFrom, subs).get();
}

QualifiedType TupleType::elementType(int i) const {
  assert(0 <= i && (size_t) i < subs_.size());
  // find subs[id]
  auto search = subs_.find(idForTupElt(i));
  if (search != subs_.end()) {
    return search->second;
  } else {
    assert(false && "ID mismatch in tuple elements");
    return QualifiedType();
  }
}


} // end namespace types
} // end namespace chpl
