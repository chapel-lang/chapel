/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_TYPES_ERRONEOUSTYPE_H
#define CHPL_TYPES_ERRONEOUSTYPE_H

#include "chpl/types/Type.h"

namespace chpl {
namespace types {


/**
  This class represents an erroneous type - that is, a type used as an error
  sentinel type. Errors with this type should be ignored.
 */
class ErroneousType : public Type {
 private:
  ErroneousType() : Type(typetags::ErroneousType) { }

  bool contentsMatchInner(const Type* other) const override {
    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  bool isGeneric() override {
    return false;
  }

 public:
  ~ErroneousType() = default;

  static owned<ErroneousType> build();
};


} // end namespace uast
} // end namespace chpl

#endif
