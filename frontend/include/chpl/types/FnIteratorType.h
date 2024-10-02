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

#ifndef CHPL_TYPES_FN_ITERATOR_TYPE_H
#define CHPL_TYPES_FN_ITERATOR_TYPE_H

#include "chpl/types/Type.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/types/IteratorType.h"

namespace chpl {
namespace types {

class FnIteratorType final : public IteratorType {
 private:
  /*
    The iterator procedure that was invoked to construct an iterator of this
    type. E.g,, the type of `foo()` will contain `iter foo()` as the iterator
    function.
   */
  const resolution::TypedFnSignature* iteratorFn_;
  /*
    The instantiation scope that was used for the function that generated this
    iterator. This is needed to properly find leader/follower etc. overloads
    of generic function.
   */
  const resolution::PoiScope* poiScope_;

  FnIteratorType(QualifiedType yieldType,
                 const resolution::TypedFnSignature* iteratorFn,
                 const resolution::PoiScope* poiScope)
    : IteratorType(typetags::FnIteratorType, std::move(yieldType)),
      iteratorFn_(iteratorFn), poiScope_(poiScope) {}

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = (FnIteratorType*) other;
    return this->yieldType_ == rhs->yieldType_ &&
           this->iteratorFn_ == rhs->iteratorFn_ &&
           this->poiScope_ == rhs->poiScope_;
  }

  void markUniqueStringsInner(Context* context) const override;

  static const owned <FnIteratorType>&
  getFnIteratorType(Context* context,
                    QualifiedType yieldType,
                    const resolution::TypedFnSignature* iteratorFn,
                    const resolution::PoiScope* poiScope);

 public:
  static const FnIteratorType* get(Context* context,
                                   QualifiedType yieldType,
                                   const resolution::TypedFnSignature* iteratorFn,
                                   const resolution::PoiScope* poiScope);

  const resolution::TypedFnSignature* iteratorFn() const {
    return iteratorFn_;
  }

  const resolution::PoiScope* poiScope() const {
    return poiScope_;
  }
};

} // end namespace types
} // end namespace chpl

#endif
