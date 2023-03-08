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
#include "chpl/types/EnumType.h"

#include "chpl/framework/query-impl.h"

namespace chpl {
namespace types {

const owned<EnumType>&
EnumType::getEnumType(Context* context, ID id, UniqueString name) {
  QUERY_BEGIN(getEnumType, context, id, name);

  auto result = toOwned(new EnumType(id, name));

  return QUERY_END(result);
}

const EnumType* EnumType::get(Context* context, ID id, UniqueString name) {
  return EnumType::getEnumType(context, id, name).get();
}

const EnumType* EnumType::getBoundedRangeTypeType(Context* context) {
  auto symbolPath = UniqueString::get(context, "ChapelRange.BoundedRangeType");
  auto name = UniqueString::get(context, "BoundedRangeType");
  auto id = ID(symbolPath, -1, 0);
  return EnumType::get(context, id, name);
}

void EnumType::stringify(std::ostream& ss, StringifyKind stringKind) const {
  name().stringify(ss, stringKind);
}

} // end namespace types
} // end namespace chpl
