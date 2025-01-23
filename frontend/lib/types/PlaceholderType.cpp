/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/PlaceholderType.h"
#include "chpl/framework/query-impl.h"

namespace chpl {
namespace types {

owned<PlaceholderType> const&
PlaceholderType::getPlaceholderType(Context* context, ID id) {
  QUERY_BEGIN(getPlaceholderType, context, id);
  auto result = toOwned(new PlaceholderType(id));
  return QUERY_END(result);
}

const PlaceholderType* PlaceholderType::get(Context* context, ID id) {
  return getPlaceholderType(context, id).get();
}

void PlaceholderType::stringify(std::ostream& ss,
                                chpl::StringifyKind stringKind) const {
  ss << "PlaceholderType(";
  id_.stringify(ss, stringKind);
  ss << ")";
}

} // end namespace types
} // end namespace chpl
