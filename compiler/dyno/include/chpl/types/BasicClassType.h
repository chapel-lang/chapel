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

  BasicClassType(ID id, UniqueString name,
                 const BasicClassType* parentType,
                 const BasicClassType* instantiatedFrom,
                 SubstitutionsMap subs)
    : CompositeType(typetags::BasicClassType, id, name,
                    instantiatedFrom, std::move(subs)),
      parentType_(parentType)
  {
    // all classes should have a parent type, except for object
    // which doesn't.
    assert(parentType_ || name == USTR("object"));
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
                    const BasicClassType* instantiatedFrom,
                    SubstitutionsMap subs);

 public:

  ~BasicClassType() = default;

  static const BasicClassType*
  get(Context* context, ID id, UniqueString name,
      const BasicClassType* parentType,
      const BasicClassType* instantiatedFrom,
      CompositeType::SubstitutionsMap subs);

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
      parent class type or an instantiation of a passed generic
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
