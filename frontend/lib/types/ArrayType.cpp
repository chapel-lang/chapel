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

#include "chpl/types/ArrayType.h"

#include "chpl/types/RuntimeType.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/intents.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/Param.h"

namespace chpl {
namespace types {

const ID ArrayType::domainId = ID(UniqueString(), 0, 0);
const ID ArrayType::eltTypeId = ID(UniqueString(), 1, 0);
const ID ArrayType::runtimeTypeId = ID(UniqueString(), 2, 0);

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
  return parsing::getSymbolIdFromTopLevelModule(context, "ChapelArray",
                                                "_array");
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
  auto id = getArrayID(context);
  auto name = id.symbolName(context);
  SubstitutionsMap subs;
  const ArrayType* instantiatedFrom = nullptr;
  return getArrayTypeQuery(context, id, name, instantiatedFrom, subs).get();
}

const ArrayType*
ArrayType::getArrayType(Context* context,
                        const QualifiedType& instance,
                        const QualifiedType& domainType,
                        const QualifiedType& eltType) {
  auto genericArray = getGenericArrayType(context);

  SubstitutionsMap subs;
  subs.emplace(ArrayType::domainId, domainType);
  subs.emplace(ArrayType::eltTypeId, eltType);

  // Add substitution for _instance field
  auto& rf = fieldsForTypeDecl(context, genericArray,
                               resolution::DefaultsPolicy::IGNORE_DEFAULTS,
                               /* syntaxOnly */ true);
  ID instanceFieldId;
  for (int i = 0; i < rf.numFields(); i++) {
    if (rf.fieldName(i) == USTR("_instance")) {
      instanceFieldId = rf.fieldDeclId(i);
      break;
    }
  }

  // TODO: resolver currently doesn't bother to set up the array instance type.
  //       as a result, this is sometimes unknown, which leads to problems
  //       when doing canInstantiate (which uses canPass). So, don't include
  //       the unknown sub to avoid those issues. Remove this when
  //       resolver handles _instance.
  if (!instance.isUnknown()) {
    subs.emplace(instanceFieldId, instance);
  }
  auto id = getArrayID(context);
  auto name = id.symbolName(context);
  auto instantiatedFrom = getGenericArrayType(context);
  return getArrayTypeQuery(context, id, name, instantiatedFrom, subs).get();
}

const ArrayType* ArrayType::getWithRuntimeType(Context* context,
                                               const ArrayType* arrayType,
                                               const RuntimeType* runtimeType) {
  CHPL_ASSERT(runtimeType != nullptr);
  auto rttQt = QualifiedType(QualifiedType::TYPE, runtimeType);
  auto subs = arrayType->substitutions();
  subs.emplace(runtimeTypeId, rttQt);

  const ArrayType* instantiatedFrom = nullptr;
  if (auto sourceInstFrom = arrayType->instantiatedFromCompositeType()) {
    CHPL_ASSERT(sourceInstFrom->isArrayType());
    instantiatedFrom = sourceInstFrom->toArrayType();
  }

  return getArrayTypeQuery(context, arrayType->id(), arrayType->name(),
                           instantiatedFrom, subs).get();
}


} // end namespace types
} // end namespace chpl
