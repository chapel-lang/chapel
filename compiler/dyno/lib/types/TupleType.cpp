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
#include "chpl/resolution/intents.h"

namespace chpl {
namespace types {


static ID idForTupElt(int i) {
  // Use an ID with only a postOrderId but empty name for the tuple elts
  return ID(UniqueString(), i, 0);
}

void TupleType::computeIsStarTuple() {
  int n = numElements();
  QualifiedType eltT;
  bool allEltTypesSame = true;
  for (int i = 0; i < n; i++) {
    if (i == 0) {
      eltT = elementType(i);
    } else {
      if (eltT != elementType(i)) {
        allEltTypesSame = false;
        break;
      }
    }
  }

  isStarTuple_ = allEltTypesSame;
}

const owned<TupleType>&
TupleType::getTupleType(Context* context, ID id, UniqueString name,
                        const TupleType* instantiatedFrom,
                        SubstitutionsMap subs) {
  QUERY_BEGIN(getTupleType, context, id, name, instantiatedFrom, subs);

  auto result = toOwned(new TupleType(id, name,
                                      instantiatedFrom, std::move(subs)));

  return QUERY_END(result);
}

const TupleType*
TupleType::getValueTuple(Context* context, std::vector<const Type*> eltTypes) {
  auto kind = QualifiedType::VAR;
  SubstitutionsMap subs;
  int i = 0;
  for (auto t : eltTypes) {
    subs.emplace(idForTupElt(i), QualifiedType(kind, t));
    i++;
  }

  auto name = UniqueString::get(context, "_tuple");
  auto id = ID();
  const TupleType* instantiatedFrom = getGenericTupleType(context);
  return getTupleType(context, id, name, instantiatedFrom,
                      std::move(subs)).get();
}

const TupleType*
TupleType::getReferentialTuple(Context* context,
                               std::vector<const Type*> eltTypes) {
  SubstitutionsMap subs;
  int i = 0;
  for (auto t : eltTypes) {
    // is the default intent for this type a variation of ref?
    QualifiedType testFormal = QualifiedType(QualifiedType::DEFAULT_INTENT, t);
    auto kind = resolution::resolveIntent(testFormal, false);
    bool defaultIntentRef = (kind == QualifiedType::CONST_REF ||
                             kind == QualifiedType::REF);
    if (defaultIntentRef) {
      kind = QualifiedType::REF;
    } else {
      kind = QualifiedType::VAR;
    }

    subs.emplace(idForTupElt(i), QualifiedType(kind, t));
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

const TupleType* TupleType::toValueTuple(Context* context) const {
  // Is it already a value tuple? If so, return that
  bool allValue = true;
  int n = numElements();
  for (int i = 0; i < n; i++) {
    auto kind = elementType(i).kind();
    if (kind == QualifiedType::CONST_REF ||
        kind == QualifiedType::REF)
      allValue = false;
  }

  if (numElements() == 0 || allValue)
    return this;

  // Otherwise, compute a new value tuple
  std::vector<const Type*> eltTypes;
  for (int i = 0; i < n; i++) {
    eltTypes.push_back(elementType(i).type());
  }

  return getValueTuple(context, std::move(eltTypes));
}


const TupleType* TupleType::toReferentialTuple(Context* context) const {
  // Is it already a referential tuple? If so, return that
  bool allRef = true;
  int n = numElements();
  for (int i = 0; i < n; i++) {
    auto kind = elementType(i).kind();
    if (kind != QualifiedType::CONST_REF &&
        kind != QualifiedType::REF)
      allRef = false;
  }

  if (numElements() == 0 || allRef)
    return this;

  // Otherwise, compute a new referential tuple
  std::vector<const Type*> eltTypes;
  for (int i = 0; i < n; i++) {
    eltTypes.push_back(elementType(i).type());
  }

  return getReferentialTuple(context, std::move(eltTypes));
}


} // end namespace types
} // end namespace chpl
