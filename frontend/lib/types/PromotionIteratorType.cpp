/*
 * Copyright 2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/PromotionIteratorType.h"
#include "chpl/framework/query-impl.h"
#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace types {

void PromotionIteratorType::markUniqueStringsInner(Context* context) const {
  scalarFn_->mark(context);
  for (const auto& pair : promotedFormals_) {
    pair.second.mark(context);
  }
}

const owned<PromotionIteratorType>&
PromotionIteratorType::getPromotionIteratorType(Context* context,
                                                const resolution::TypedFnSignature* scalarFn,
                                                resolution::SubstitutionsMap promotedFormals) {
  QUERY_BEGIN(getPromotionIteratorType, context, scalarFn, promotedFormals);
  auto result = toOwned(new PromotionIteratorType(scalarFn, std::move(promotedFormals)));
  return QUERY_END(result);
}

const PromotionIteratorType*
PromotionIteratorType::get(Context* context,
                           const resolution::TypedFnSignature* scalarFn,
                           resolution::SubstitutionsMap promotedFormals) {
  return getPromotionIteratorType(context, scalarFn, std::move(promotedFormals)).get();
}

}  // end namespace types
}  // end namespace chpl
