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

#ifndef CHPL_TYPES_ITERATOR_TYPE_H
#define CHPL_TYPES_ITERATOR_TYPE_H

#include "chpl/types/Type.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace types {

class IteratorType : public Type {
 protected:
  /*
    The type produced by this iterator. E.g., in a loop such
    as the right hand side in the below assignment:

       var A = foreach i in 1..10 do (i,i);

    the yield type is (int, int).
   */
  QualifiedType yieldType_;
  /*
    The instantiation scope that was used for the function that generated this
    iterator. This is needed to properly find leader/follower etc. overloads
    of generic function.
   */
  const resolution::PoiScope* poiScope_;

  bool iteratorTypeContentsMatchInner(const IteratorType* other) const {
    return yieldType_ == other->yieldType_ &&
           poiScope_ == other->poiScope_;
  }

  IteratorType(typetags::TypeTag tag, QualifiedType yieldType,
               const resolution::PoiScope* poiScope)
    : Type(tag), yieldType_(std::move(yieldType)), poiScope_(poiScope) {}

  Genericity genericity() const override {
    return CONCRETE;
  }

 public:
  const QualifiedType& yieldType() const {
    return yieldType_;
  }

  const resolution::PoiScope* poiScope() const {
    return poiScope_;
  }
};

} // end namespace types
} // end namespace chpl

#endif
