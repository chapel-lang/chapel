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

#include "chpl/types/CPtrType.h"

#include "chpl/framework/query-impl.h"
#include "chpl/resolution/intents.h"
#include "chpl/types/Param.h"
#include "chpl/resolution/can-pass.h"

namespace chpl {
namespace types {

const owned<CPtrType>& CPtrType::getCPtrType(Context* context,
                                             const CPtrType* instantiatedFrom,
                                             QualifiedType eltType) {
  QUERY_BEGIN(getCPtrType, context, instantiatedFrom, eltType);
  auto result = toOwned(new CPtrType(instantiatedFrom, eltType));
  return QUERY_END(result);
}

bool CPtrType::isEltTypeInstantiationOf(Context* context, const CPtrType* other) const {
  auto r = resolution::canPass(context, eltType_, other->eltType_);
  // instantiation and same-type passing are allowed here
  return r.passes() && !r.promotes() && !r.converts();
}

const CPtrType* CPtrType::get(Context* context) {
  return CPtrType::getCPtrType(context,
                               /* instantiatedFrom */ nullptr,
                               /* eltType */ QualifiedType()).get();
}

const CPtrType* CPtrType::get(Context* context, const QualifiedType& eltType) {
  return CPtrType::getCPtrType(context,
                               /* instantiatedFrom */ CPtrType::get(context),
                               eltType).get();
}

} // end namespace types
} // end namespace chpl
