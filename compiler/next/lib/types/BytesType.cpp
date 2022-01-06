/*
 * Copyright 2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/BytesType.h"
#include "chpl/queries/query-impl.h"

namespace chpl {
namespace types {


const owned<BytesType>& BytesType::getBytesType(Context* context) {
  QUERY_BEGIN(getBytesType, context);

  auto result = toOwned(new BytesType());

  return QUERY_END(result);
}

const BytesType* BytesType::get(Context* context) {
  return getBytesType(context).get();
}


} // end namespace types
} // end namespace chpl
