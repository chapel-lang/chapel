/*
 * Copyright 2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/RuntimeType.h"
#include "chpl/framework/query-impl.h"

namespace chpl {
namespace types {

owned<RuntimeType> const&
RuntimeType::getRuntimeType(Context* context,
                            const resolution::TypedFnSignature* initializer) {
  QUERY_BEGIN(getRuntimeType, context, initializer);
  auto result = toOwned(new RuntimeType(initializer));
  return QUERY_END(result);
}

const RuntimeType* RuntimeType::get(Context* context,
                                    const resolution::TypedFnSignature* initializer) {
  return getRuntimeType(context, initializer).get();
}

} // end namespace types
} // end namespace chpl
