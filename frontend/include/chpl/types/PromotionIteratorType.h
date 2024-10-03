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

#ifndef CHPL_TYPES_PROMOTION_ITERATOR_TYPE_H
#define CHPL_TYPES_PROMOTION_ITERATOR_TYPE_H

#include "chpl/types/Type.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/types/IteratorType.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/uast/Decl.h"
#include <unordered_map>

namespace chpl {

namespace types {

class PromotionIteratorType final : public IteratorType {
 private:
  /* The scalar (non-array) function that was invoked as part of the promotion. */
  const resolution::TypedFnSignature* scalarFn_;
  /*
     Mapping of promoted formals to their array (non-scalar) types, for the
     purposes of determining the parallel iteration strategy.
   */
  const resolution::SubstitutionsMap promotedFormals_;

  PromotionIteratorType(QualifiedType yieldType,
                        const resolution::PoiScope* poiScope,
                        const resolution::TypedFnSignature* scalarFn,
                        resolution::SubstitutionsMap promotedFormals)
    : IteratorType(typetags::PromotionIteratorType, std::move(yieldType), poiScope),
      scalarFn_(scalarFn),
      promotedFormals_(std::move(promotedFormals)) {}

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = (PromotionIteratorType*) other;
    return this->yieldType_ == rhs->yieldType_ &&
           this->poiScope_ == rhs->poiScope_ &&
           this->scalarFn_ == rhs->scalarFn_ &&
           this->promotedFormals_ == rhs->promotedFormals_;
  }

  void markUniqueStringsInner(Context* context) const override;

  static const owned<PromotionIteratorType>&
  getPromotionIteratorType(Context* context,
                           QualifiedType yieldType,
                           const resolution::PoiScope* poiScope,
                           const resolution::TypedFnSignature* scalarFn,
                           resolution::SubstitutionsMap promotedFormals);

 public:
  static const PromotionIteratorType* get(Context* context,
                                          QualifiedType yieldType,
                                          const resolution::PoiScope* poiScope,
                                          const resolution::TypedFnSignature* scalarFn,
                                          resolution::SubstitutionsMap promotedFormals);

  const resolution::TypedFnSignature* scalarFn() const {
    return scalarFn_;
  }

  const resolution::SubstitutionsMap& promotedFormals() const {
    return promotedFormals_;
  }
};

} // end namespace types
} // end namespace chpl

#endif
