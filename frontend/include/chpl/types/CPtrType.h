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

#ifndef CHPL_TYPES_CPTR_TYPE_H
#define CHPL_TYPES_CPTR_TYPE_H

#include "chpl/types/Type.h"
#include "chpl/types/QualifiedType.h"

namespace chpl {
namespace types {

class CPtrType final : public Type {
 private:
  const CPtrType* instantiatedFrom_;
  const Type* eltType_;

  CPtrType(const CPtrType* instantiatedFrom, const Type* eltType)
    : Type(typetags::CPtrType), instantiatedFrom_(instantiatedFrom),
      eltType_(eltType) {
    // not an instantiation -> eltType_ should be empty
    CHPL_ASSERT(instantiatedFrom_ != nullptr || eltType_ == nullptr);
    // is an instantiation -> eltType should not be empty
    CHPL_ASSERT(instantiatedFrom_ == nullptr || eltType_ != nullptr);
  }

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = (CPtrType*) other;
    return instantiatedFrom_ == rhs->instantiatedFrom_ &&
           eltType_ == rhs->eltType_;
  }

  void markUniqueStringsInner(Context* context) const override {}

  Genericity genericity() const override {
    if (!eltType_) return GENERIC;
    return eltType_->genericity();
  }

  static const owned<CPtrType>& getCPtrType(Context* context,
                                            const CPtrType* instantiatedFrom,
                                            const Type* eltType);

  const CPtrType* instantiatedFromCPtrType() const {
    // at present, only expecting a single level of instantiated-from.
    CHPL_ASSERT(instantiatedFrom_ == nullptr ||
           instantiatedFrom_->instantiatedFrom_ == nullptr);
    return instantiatedFrom_;
  }

  bool isEltTypeInstantiationOf(Context* context, const CPtrType* other) const;

 public:

  static const CPtrType* get(Context* context);
  static const CPtrType* get(Context* context, const Type* eltType);

  static const ID& getId(Context* context);

  const Type* eltType() const {
    return eltType_;
  }

  bool isVoidPtr() const {
    if (eltType_ == nullptr) return false;
    return eltType_->isVoidType();
  }

  bool isInstantiationOf(Context* context,
                         const CPtrType* genericType) const {
    auto thisFrom = instantiatedFromCPtrType();
    auto argFrom = genericType->instantiatedFromCPtrType();
    if (argFrom == nullptr) {
      // if genericType is not a partial instantiation
      return (thisFrom != nullptr && thisFrom == genericType);
    }

    if (thisFrom == argFrom) {
      // handle the case of genericType being partly instantiated
      // (or instantiated with a generic type)
      return isEltTypeInstantiationOf(context, genericType);
    }

    return false;
  }

  virtual void stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const override;
};

} // end namespace types
} // end namespace chpl

#endif
