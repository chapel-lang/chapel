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

#ifndef CHPL_TYPES_BASIC_CLASS_TYPE_H
#define CHPL_TYPES_BASIC_CLASS_TYPE_H

#include "chpl/types/CompositeType.h"

namespace chpl {
namespace types {


/**
  This class represents an class type (e.g. `class C`)
  without considering decorators.
 */
class BasicClassType final : public CompositeType {
 private:
  const Type* parentType_ = nullptr;

  BasicClassType(ID id, UniqueString name,
                 const Type* parentType,
                 std::vector<CompositeType::FieldDetail> fields)
    : CompositeType(typetags::BasicClassType, id, name, std::move(fields)),
      parentType_(parentType)
  {
    assert(parentType_);
    assert(parentType_->isObjectType() ||
           parentType_->isErroneousType() ||
           parentType_->isBasicClassType());

    // compute the summary information including the parent type
    computeSummaryInformation();
  }

  bool contentsMatchInner(const Type* other) const override {
    const BasicClassType* rhs = (const BasicClassType*) other;
    return compositeTypeContentsMatchInner(rhs) &&
           parentType_ == rhs->parentType_;
  }

  void markUniqueStringsInner(Context* context) const override {
    compositeTypeMarkUniqueStringsInner(context);
  }

  static const owned<BasicClassType>&
  getBasicClassType(Context* context, ID id, UniqueString name,
                    const Type* parentType,
                    std::vector<CompositeType::FieldDetail> fields);
 public:
  ~BasicClassType() = default;

  static const BasicClassType*
  get(Context* context, ID id, UniqueString name,
      const Type* parentType,
      std::vector<CompositeType::FieldDetail> fields);

  /** Return the parent class type, which will be one of:
        * a BasicClassType
        * ObjectType
        * ErroneousType
   */
  const Type* parentClassType() const {
    return parentType_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
