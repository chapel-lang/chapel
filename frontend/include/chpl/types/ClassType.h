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

#ifndef CHPL_TYPES_CLASS_TYPE_H
#define CHPL_TYPES_CLASS_TYPE_H

#include "chpl/types/BasicClassType.h"
#include "chpl/types/AnyClassType.h"
#include "chpl/types/ClassTypeDecorator.h"
#include "chpl/types/Type.h"

namespace chpl {
namespace types {


/**
  This class represents an class type including a memory management decorator.
  E.g. if we have `class C`, then `borrowed C?` or `shared C` are ClassTypes.
 */
class ClassType final : public Type {
 private:
  const ManageableType* manageableType_;
  const Type* manager_;
  ClassTypeDecorator decorator_;

  ClassType(const ManageableType* manageableType,
            const Type* manager,
            ClassTypeDecorator decorator)
    : Type(typetags::ClassType),
      manageableType_(manageableType),
      manager_(manager),
      decorator_(decorator)
  { }

  bool contentsMatchInner(const Type* other) const override {
    const ClassType* lhs = this;
    const ClassType* rhs = (const ClassType*) other;
    return lhs->manageableType_ == rhs->manageableType_ &&
           lhs->manager_ == rhs->manager_ &&
           lhs->decorator_ == rhs->decorator_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }


  static const owned<ClassType>&
  getClassType(Context* context,
               const ManageableType* manageableType,
               const Type* manager,
               ClassTypeDecorator decorator);

  Genericity genericity() const override {
    if (decorator_.isUnknownManagement() || decorator_.isUnknownNilability())
      return GENERIC;
    else
      return MAYBE_GENERIC; // depends on the class
  }

 public:
  ~ClassType() = default;
  void stringify(std::ostream& ss,
                 chpl::StringifyKind stringKind) const override;

  static const ClassType* get(Context* context,
                              const ManageableType* manageableType,
                              const Type* manager,
                              ClassTypeDecorator decorator);

  const Type* substitute(Context* context,
                         const PlaceholderMap& subs) const override {
    return get(context,
               Type::substitute(context, manageableType_, subs),
               Type::substitute(context, manager_, subs), decorator_);
  }

  /** Returns the ClassTypeDecorator for this ClassType.
      This decorator indicates the memory management strategy. */
  ClassTypeDecorator decorator() const { return decorator_; }

  /** Returns the manager for this ClassType, or nullptr
      if the decorator does not indicate a managed type.
      This can return a record type or AnyOwnedType / AnySharedType. */
  const Type* manager() const {
    if (!decorator_.isManaged())
      return nullptr;
    return manager_;
  }

  /** Returns the type being managed */
  const ManageableType* manageableType() const {
    return manageableType_;
  }

  /** Returns the basic class type */
  const BasicClassType* basicClassType() const {
    if (manageableType_) return manageableType_->toBasicClassType();
    return nullptr;
  }

  /** Returns the version of this ClassType with the passed decorator */
  const ClassType* withDecorator(Context* context,
                                 ClassTypeDecorator decorator) const;

  /** Returns the recordType for the manager. This will be instantiated,
      e.g. as '_owned(C)' for some class C. */
  const RecordType* managerRecordType(Context* context) const;

};


} // end namespace uast
} // end namespace chpl

#endif
