/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
#include "chpl/queries/global-strings.h"

namespace chpl {
namespace types {


/**
  This class represents an class type (e.g. `class C`)
  without considering decorators.
 */
class BasicClassType final : public CompositeType {
 private:
  const BasicClassType* parentType_ = nullptr;

  bool contentsMatchInner(const Type* other,
                          MatchAssumptions& assumptions) const override {
    const BasicClassType* rhs = (const BasicClassType*) other;

    // by the time this runs, parentType_ should be set
    assert(parentType_ != nullptr);

    // add an assumption about the parent types, if they differ
    if (!assumptions.assume(parentType_, rhs->parentType_))
      return false;

    return compositeTypeContentsMatchInner(rhs, assumptions);
  }

  void markUniqueStringsInner(Context* context) const override {
    compositeTypeMarkUniqueStringsInner(context);
  }

  static const owned<BasicClassType>& getObjectTypeQuery(Context* context);

 public:
  /** Construct a BasicClassType.
      Note: we expect the field types to be nullptr when this is called
   */
  BasicClassType(ID id, UniqueString name,
                 std::vector<CompositeType::FieldDetail> fields,
                 const BasicClassType* instantiatedFrom,
                 SubstitutionsMap subs)
    : CompositeType(typetags::BasicClassType, id, name, std::move(fields),
                    instantiatedFrom, std::move(subs)),
      parentType_(nullptr)
  {
  }

  ~BasicClassType() = default;

  /** Set the parent type. For use only during resolution while the type is
      still being constructed. Be sure to call
      finalizeFieldTypes after changing this and any other fields. */
  void setParentType(const BasicClassType* parentType) {
    parentType_ = parentType;
  }

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
      parent class type or an instantiaton of a passed generic
      class type or a combination of the two.

      The argument 'convert' is set to true if passing required
      using a parent type and 'instantiates' is set to true if
      it requires instantiation.
   */
  bool isSubtypeOf(const BasicClassType* parentType,
                   bool& converts,
                   bool& instantiates) const;

  /** If this type represents an instantiated type,
      returns the type it was instantiated from.

      This is just instantiatedFromCompositeType() with the
      result cast to BasicClassType.
   */
  const BasicClassType* instantiatedFrom() const {
    const CompositeType* ret = instantiatedFromCompositeType();
    assert(ret == nullptr || ret->tag() == typetags::BasicClassType);
    return (const BasicClassType*) ret;
  }

};


} // end namespace types


} // end namespace chpl

#endif
