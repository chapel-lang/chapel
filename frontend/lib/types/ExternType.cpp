/*
 * Copyright 2023-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/ExternType.h"
#include "chpl/framework/query-impl.h"

namespace chpl {
namespace types {

const owned<ExternType>& ExternType::getExternType(Context* context,
                                                   UniqueString name) {
  QUERY_BEGIN(getExternType, context, name);
  auto result = toOwned(new ExternType(name));
  return QUERY_END(result);
}

void ExternType::stringify(std::ostream& ss,
                           chpl::StringifyKind stringKind) const {
  ss << "extern type ";
  linkageName().stringify(ss, stringKind);
}

const ExternType* ExternType::get(Context* context, UniqueString linkageName) {
  return getExternType(context, linkageName).get();
}

} // end namespace types
} // end namespace chpl
