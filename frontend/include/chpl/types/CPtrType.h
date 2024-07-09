/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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
  const bool isConst_ = false;

  CPtrType(const CPtrType* instantiatedFrom,
           const Type* eltType,
           bool isConst = false)
    : Type(typetags::CPtrType), instantiatedFrom_(instantiatedFrom),
      eltType_(eltType), isConst_(isConst) {
    // not an instantiation -> eltType_ should be empty
    CHPL_ASSERT(instantiatedFrom_ != nullptr || eltType_ == nullptr);
    // is an instantiation -> eltType should not be empty
    CHPL_ASSERT(instantiatedFrom_ == nullptr || eltType_ != nullptr);
  }

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = (CPtrType*) other;
    return instantiatedFrom_ == rhs->instantiatedFrom_ &&
           eltType_ == rhs->eltType_ &&
           isConst_ == rhs->isConst_;
  }

  void markUniqueStringsInner(Context* context) const override {}

  Genericity genericity() const override {
    if (!eltType_) return GENERIC;
    return eltType_->genericity();
  }

  static const owned<CPtrType>& getCPtrType(Context* context,
                                            const CPtrType* instantiatedFrom,
                                            const Type* eltType,
                                            bool isConst);

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
  static const CPtrType* getCVoidPtrType(Context* context);
  static const CPtrType* getConst(Context* context);
  static const CPtrType* getConst(Context* context, const Type* eltType);

  static const ID& getId(Context* context);
  static const ID& getConstId(Context* context);

  const ID& id(Context* context) const {
    return isConst() ? getConstId(context) : getId(context);
  }

  const Type* eltType() const {
    return eltType_;
  }

  const CPtrType* withoutConst(Context* context) const;

  bool isConst() const {
    return isConst_;
  }

  bool isVoidPtr() const {
    if (eltType_ == nullptr) return false;
    return eltType_->isVoidType();
  }

  bool isInstantiationOf(Context* context,
                         const CPtrType* genericType) const;

  virtual void stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const override;
};

} // end namespace types
} // end namespace chpl

#endif
