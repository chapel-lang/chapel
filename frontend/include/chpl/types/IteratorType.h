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

  IteratorType(typetags::TypeTag tag, QualifiedType yieldType)
    : Type(tag), yieldType_(std::move(yieldType)) {}

  Genericity genericity() const override {
    return CONCRETE;
  }

 public:
  const QualifiedType& yieldType() const {
    return yieldType_;
  }
};

} // end namespace types
} // end namespace chpl

#endif
