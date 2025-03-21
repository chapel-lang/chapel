/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_TYPES_PTR_TYPE_H
#define CHPL_TYPES_PTR_TYPE_H

#include "chpl/types/Type.h"
#include "chpl/types/QualifiedType.h"

namespace chpl {
namespace types {

class PtrType : public Type {
 protected:
  const PtrType* instantiatedFrom_;
  const Type* eltType_;

  PtrType(typetags::TypeTag tag, const PtrType* instantiatedFrom,
          const Type* eltType)
      : Type(tag), instantiatedFrom_(instantiatedFrom), eltType_(eltType) {
    // not an instantiation -> eltType_ should be empty
    CHPL_ASSERT(instantiatedFrom_ != nullptr || eltType_ == nullptr);
    // is an instantiation -> eltType should not be empty
    CHPL_ASSERT(instantiatedFrom_ == nullptr || eltType_ != nullptr);

    // check instantiated only from same type of object
    CHPL_ASSERT(instantiatedFrom_ == nullptr ||
                instantiatedFrom_->tag() == tag);
  }

  Genericity genericity() const override {
    if (!eltType_) return GENERIC;
    return eltType_->genericity();
  }

  bool isEltTypeInstantiationOf(Context* context, const PtrType* other) const;

  const PtrType* instantiatedFromPtrType() const {
    // at present, only expecting a single level of instantiated-from.
    CHPL_ASSERT(instantiatedFrom_ == nullptr ||
                instantiatedFrom_->instantiatedFrom_ == nullptr);
    return instantiatedFrom_;
  }

 public:
  virtual const ID& id(Context* context) const = 0;

  const Type* eltType() const {
    return eltType_;
  }

  bool isVoidPtr() const {
    if (eltType_ == nullptr) return false;
    return eltType_->isVoidType();
  }

  bool isInstantiationOf(Context* context,
                         const PtrType* genericType) const;
};

} // end namespace types
} // end namespace chpl

#endif
