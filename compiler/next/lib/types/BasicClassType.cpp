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

#include "chpl/types/BasicClassType.h"

#include "chpl/queries/query-impl.h"

namespace chpl {
namespace types {


const owned<BasicClassType>&
BasicClassType::getBasicClassType(
    Context* context, ID id, UniqueString name,
    const BasicClassType* parentType,
    std::vector<CompositeType::FieldDetail> fields) {
  QUERY_BEGIN(getBasicClassType, context, id, name, parentType, fields);

  auto result = toOwned(new BasicClassType(id, name,
                                           parentType, std::move(fields)));

  return QUERY_END(result);
}

const BasicClassType*
BasicClassType::get(Context* context, ID id, UniqueString name,
                    const BasicClassType* parentType,
                    std::vector<CompositeType::FieldDetail> fields) {
  // getObjectType should be used to construct object
  // everything else should have a parent type.
  assert(parentType != nullptr);
  return getBasicClassType(context, id, name,
                           parentType, std::move(fields)).get();
}

const BasicClassType*
BasicClassType::getObjectType(Context* context) {
  ID emptyId;
  auto name = UniqueString::build(context, "object");
  std::vector<CompositeType::FieldDetail> emptyFields;

  return getBasicClassType(context, emptyId, name,
                           nullptr, std::move(emptyFields)).get();
}


} // end namespace types
} // end namespace chpl
