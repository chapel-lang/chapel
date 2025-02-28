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

#include "chpl/types/TupleType.h"

#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/intents.h"
#include "chpl/types/Param.h"
#include "chpl/resolution/can-pass.h"

namespace chpl {
namespace types {


static ID idForTupElt(int i) {
  // Use an ID with only a postOrderId but empty name for the tuple elts
  return ID(UniqueString(), i, 0);
}

void TupleType::stringify(std::ostream& ss,
                              chpl::StringifyKind stringKind) const {
  if (stringKind != StringifyKind::CHPL_SYNTAX) {
    CompositeType::stringify(ss, stringKind);
    return;
  }

  auto sorted = sortedSubstitutions();
  bool emittedField = false;
  ss << "(";
  for (const auto& sub : sorted) {
    if (emittedField) ss << ", ";
    if (sub.second.type()) {
      sub.second.type()->stringify(ss, stringKind);
    }
    emittedField = true;
  }
  ss << ")";
}


void TupleType::computeIsStarTuple() {
  if (isKnownSize_ == false) {
    isStarTuple_ = true;
  } else {
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
}

void TupleType::computeIsParamKnown() {
  if (isKnownSize_ == false) {
    isParamKnown_ = false;
  } else {
    isParamKnown_ = true;
    int n = numElements();
    for (int i = 0; i < n; i++) {
      const auto& eltT = elementType(i);
      if (!eltT.isParam() || eltT.isUnknown()) {
        isParamKnown_ = false;
        break;
      }
    }
  }
}

const owned<TupleType>&
TupleType::getTupleType(Context* context, const TupleType* instantiatedFrom,
                        SubstitutionsMap subs,
                        bool isVarArgTuple) {
  QUERY_BEGIN(getTupleType, context, instantiatedFrom, subs,
                            isVarArgTuple);

  auto [id, name] =
      parsing::getSymbolFromTopLevelModule(context, "ChapelTuple", "_tuple");
  auto result = toOwned(new TupleType(id, name, instantiatedFrom,
                                      std::move(subs), isVarArgTuple));

  return QUERY_END(result);
}

const TupleType*
TupleType::getValueTuple(Context* context, std::vector<const Type*> eltTypes, bool makeConst) {
  auto kind = makeConst ? QualifiedType::CONST_VAR : QualifiedType::VAR;
  SubstitutionsMap subs;
  int i = 0;
  for (auto t : eltTypes) {
    subs.emplace(idForTupElt(i), QualifiedType(kind, t));
    i++;
  }

  const TupleType* instantiatedFrom = getGenericTupleType(context);
  return getTupleType(context, instantiatedFrom,
                      std::move(subs)).get();
}

const TupleType*
TupleType::getReferentialTuple(Context* context,
                               std::vector<const Type*> eltTypes, bool makeConst) {
  SubstitutionsMap subs;
  int i = 0;
  for (auto t : eltTypes) {
    // is the default intent for this type a variation of ref?
    QualifiedType testFormal = QualifiedType(QualifiedType::DEFAULT_INTENT, t);
    auto kind = resolution::resolveIntent(testFormal, false, false);
    bool defaultIntentRef = (kind == QualifiedType::CONST_REF ||
                             kind == QualifiedType::REF);
    if (defaultIntentRef) {
      kind = QualifiedType::REF;
    } else {
      kind = QualifiedType::VAR;
    }
    if (makeConst) {
      kind = resolution::KindProperties::makeConst(kind);
    }
    subs.emplace(idForTupElt(i), QualifiedType(kind, t));
    i++;
  }

  const TupleType* instantiatedFrom = getGenericTupleType(context);
  return getTupleType(context, instantiatedFrom,
                      std::move(subs)).get();
}

const TupleType*
TupleType::getGenericTupleType(Context* context) {
  SubstitutionsMap subs;
  const TupleType* instantiatedFrom = nullptr;
  return getTupleType(context, instantiatedFrom, subs).get();
}

const TupleType*
TupleType::getQualifiedTuple(Context* context,
                             std::vector<QualifiedType> eltTypes) {
  SubstitutionsMap subs;
  int i = 0;
  for (const auto& t : eltTypes) {
    subs.emplace(idForTupElt(i), t);
    i++;
  }

  const TupleType* instantiatedFrom = getGenericTupleType(context);
  return getTupleType(context, instantiatedFrom,
                      std::move(subs), true).get();
}

const TupleType*
TupleType::getStarTuple(Context* context,
                        QualifiedType paramSize,
                        QualifiedType varArgEltType) {
  if (!paramSize.isUnknown()) {
    // Fixed size, we can at least create a star tuple of AnyType
    int64_t numElements = paramSize.param()->toIntParam()->value();
    std::vector<QualifiedType> eltTypes(numElements, varArgEltType);
    return getQualifiedTuple(context, eltTypes);
  } else {
    // Size unknown, store the expected element type
    const TupleType* instantiatedFrom = getGenericTupleType(context);
    SubstitutionsMap subs;
    subs.emplace(idForTupElt(-1), varArgEltType);
    return getTupleType(context, instantiatedFrom, subs, true).get();
  }
}

const QualifiedType& TupleType::elementType(int i) const {
  CHPL_ASSERT(isKnownSize_);
  CHPL_ASSERT(0 <= i && (size_t) i < subs_.size());
  // find subs[id]
  auto search = subs_.find(idForTupElt(i));
  if (search != subs_.end()) {
    return search->second;
  } else {
    CHPL_ASSERT(false && "ID mismatch in tuple elements");
    static QualifiedType empty;
    return empty;
  }
}

QualifiedType TupleType::starType() const {
  CHPL_ASSERT(isKnownSize_ == false || isStarTuple_);
  return subs_.begin()->second;
}

const TupleType* TupleType::toValueTuple(Context* context, bool makeConst) const {
  // Is it already a value tuple? If so, return that
  bool allValue = true;
  bool allConst = true;
  int n = numElements();
  for (int i = 0; i < n; i++) {
    const auto& eltType = elementType(i);
    auto kind = eltType.kind();
    if (kind != QualifiedType::VAR)
      allValue = false;
    allConst &= elementType(i).isConst();
    if (eltType.type() && eltType.type()->isTupleType()) {
      // Conservatively throw off 'allValue' because the nested tuple might
      // have a reference inside it.
      allValue = false;
      allConst = false;
    }
  }

  if (numElements() == 0 || (allValue && (!makeConst || allConst)))
    return this;

  // Otherwise, compute a new value tuple
  std::vector<const Type*> eltTypes;
  for (int i = 0; i < n; i++) {
    auto eltType = elementType(i).type();
    if (auto eltTup = eltType->toTupleType()) {
      eltType = eltTup->toValueTuple(context, makeConst);
    }
    eltTypes.push_back(eltType);
  }

  return getValueTuple(context, std::move(eltTypes), makeConst);
}


const TupleType* TupleType::toReferentialTuple(Context* context, bool makeConst) const {
  // Is it already a referential tuple? If so, return that
  bool allRef = true;
  bool allConst = true;
  int n = numElements();
  for (int i = 0; i < n; i++) {
    const auto& eltType = elementType(i);
    auto kind = eltType.kind();
    if (kind != QualifiedType::CONST_REF &&
        kind != QualifiedType::REF)
      allRef = false;
    allConst &= elementType(i).isConst();

    if (eltType.type() && eltType.type()->isTupleType()) {
      // Conservatively throw off 'allRef' because the nested tuple might
      // have a reference inside it.
      allRef = false;
      allConst = false;
    }
  }

  if (numElements() == 0 || (allRef && (!makeConst || allConst)))
    return this;

  // Otherwise, compute a new referential tuple
  std::vector<const Type*> eltTypes;
  for (int i = 0; i < n; i++) {
    auto eltType = elementType(i).type();
    if (auto eltTup = eltType->toTupleType()) {
      eltType = eltTup->toReferentialTuple(context, makeConst);
    }
    eltTypes.push_back(eltType);
  }

  return getReferentialTuple(context, std::move(eltTypes), makeConst);
}


} // end namespace types
} // end namespace chpl
