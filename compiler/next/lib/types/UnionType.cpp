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

#include "chpl/types/UnionType.h"

#include "chpl/queries/query-impl.h"

namespace chpl {
namespace types {


const owned<UnionType>&
UnionType::getUnionType(Context* context, ID id, UniqueString name,
                        std::vector<CompositeType::FieldDetail> fields,
                        const UnionType* instantiatedFrom) {
  QUERY_BEGIN(getUnionType, context, id, name, fields, instantiatedFrom);

  auto result = toOwned(new UnionType(id, name, std::move(fields),
                                      instantiatedFrom));

  return QUERY_END(result);
}

const UnionType*
UnionType::get(Context* context, ID id, UniqueString name,
               std::vector<CompositeType::FieldDetail> fields,
               const UnionType* instantiatedFrom) {
  return getUnionType(context, id, name, std::move(fields),
                      instantiatedFrom).get();
}


} // end namespace types
} // end namespace chpl
