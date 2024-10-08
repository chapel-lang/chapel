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

#include "chpl/types/PtrType.h"
#include "chpl/types/Type.h"
#include "chpl/types/QualifiedType.h"

namespace chpl {
namespace types {

class CPtrType final : public PtrType {
 private:
  const bool isConst_ = false;

  CPtrType(const CPtrType* instantiatedFrom, const Type* eltType,
           bool isConst = false)
      : PtrType(typetags::CPtrType, instantiatedFrom, eltType),
        isConst_(isConst) {}

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = (CPtrType*)other;
    return instantiatedFrom_ == rhs->instantiatedFrom_ &&
           eltType_ == rhs->eltType_ && isConst_ == rhs->isConst_;
  }

  void markUniqueStringsInner(Context* context) const override {}

  static const owned<CPtrType>& getCPtrType(Context* context,
                                            const CPtrType* instantiatedFrom,
                                            const Type* eltType, bool isConst);

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

  const CPtrType* withoutConst(Context* context) const;

  bool isConst() const { return isConst_; }

  virtual void stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const override;
};

} // end namespace types
} // end namespace chpl

#endif
