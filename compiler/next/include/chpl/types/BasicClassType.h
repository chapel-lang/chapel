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
  const BasicClassType* parentType_ = nullptr;

  BasicClassType(ID id, UniqueString name,
                 const BasicClassType* parentType,
                 std::vector<CompositeType::FieldDetail> fields)
    : CompositeType(typetags::BasicClassType, id, name, std::move(fields)),
      parentType_(parentType)
  {
    // all classes should have a parent type, except for object
    // which doesn't.
    assert(parentType_ || name == "object");

    // compute the summary information, including the parent type
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
                    const BasicClassType* parentType,
                    std::vector<CompositeType::FieldDetail> fields);
 public:
  ~BasicClassType() = default;

  static const BasicClassType*
  get(Context* context, ID id, UniqueString name,
      const BasicClassType* parentType,
      std::vector<CompositeType::FieldDetail> fields);

  static const BasicClassType* getObjectType(Context* context);

  /** Return the parent class type, or nullptr if this is the 'object' type. */
  const BasicClassType* parentClassType() const {
    return parentType_;
  }

  /** Return true if this type is the 'object' type. */
  bool isObjectType() const {
    return parentType_ == nullptr;
  }

  /** Returns true if this class type is a subclass of the passed
      parent class type. That is, some chain of
         this->parentClassType()->parentClassType()->... = parentType
   */
 bool isTransitiveChildOf(const BasicClassType* parentType) const;

};


} // end namespace types
template<> struct stringify<chpl::types::BasicClassType> {
  std::string operator()(StringifyKind stringKind, const chpl::types::BasicClassType& stringMe) const {
    return defaultStringify(stringKind, stringMe);
  }
};
} // end namespace chpl

#endif
