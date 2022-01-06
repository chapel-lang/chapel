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

#include "chpl/types/IntType.h"
#include "chpl/queries/query-impl.h"

namespace chpl {
namespace types {


const owned<IntType>& IntType::getIntType(Context* context, int bitwidth) {
  QUERY_BEGIN(getIntType, context, bitwidth);

  auto result = toOwned(new IntType(bitwidth));

  return QUERY_END(result);
}

const IntType* IntType::get(Context* context, int bitwidth) {
  assert(bitwidth == 0 || bitwidth == 8 || bitwidth == 16 ||
         bitwidth == 32 || bitwidth == 64);
  if (bitwidth == 0) bitwidth = 64; // canonicalize default width
  return getIntType(context, bitwidth).get();
}


} // end namespace types
} // end namespace chpl
