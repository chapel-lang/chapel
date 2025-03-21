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

#ifndef CHPL_TYPES_ENUM_TYPE_H
#define CHPL_TYPES_ENUM_TYPE_H

#include "chpl/types/Type.h"
#include "chpl/types/QualifiedType.h"

namespace chpl{
namespace types {

class EnumType final : public Type {
 private:
  ID id_;
  UniqueString name_;
  bool isAbstract_ = false;
  bool isConcrete_ = false;

  EnumType(ID id, UniqueString name, bool isAbstract, bool isConcrete)
    : Type(typetags::EnumType), id_(id), name_(name),
      isAbstract_(isAbstract),
      isConcrete_(isConcrete) {}

  static const owned<EnumType>&
  getEnumType(Context* context, ID id, UniqueString name);

 protected:
  bool contentsMatchInner(const Type* other) const override {
    auto otherEnum = (const EnumType*) other;
    return id_ == otherEnum->id_ && name_ == otherEnum->name_ &&
           isAbstract_ == otherEnum->isAbstract_ &&
           isConcrete_ == otherEnum->isConcrete_;
  }

  void markUniqueStringsInner(Context* context) const override {
    id_.mark(context);
    name_.mark(context);
  }

  Genericity genericity() const override {
    return Genericity::CONCRETE;
  }

 public:
  /** Get an enum type for an ID. */
  static const EnumType* get(Context* context, ID id, UniqueString name);

  /** Get the type for a range's boundKind */
  static const EnumType* getBoundKindType(Context* context);

  /** Get the type representing an iterator's "iteration kind". */
  static const EnumType* getIterKindType(Context* context);

  /** Given an enum type 'et', get a map from the name of each constant
      in 'et' to each constant represented as a param value.
      If there are multiple enum constants with the same name (which
      means the AST is semantically incorrect), then only the first
      constant is added to the map. Returns 'nullptr' if 'et' is
      'nullptr' or has an empty ID, or if it does not have any AST
      representing it. */
  static const std::map<UniqueString, QualifiedType>*
  getParamConstantsMapOrNull(Context* context, const EnumType* et);

  ~EnumType() = default;

  virtual void stringify(std::ostream& ss,
                         StringifyKind stringKind) const override;

  /** Return the uAST ID associated with this EnumType */
  const ID& id() const { return id_; }

  /** Return the name of the uAST associated with this EnumType */
  UniqueString name() const { return name_; }

  /** An enum type is abstract if none of its constants have an initializer
      part defined. */
  inline bool isAbstract() const { return isAbstract_; }

  /** An enum type is concrete if its first constant has an initializer
      part defined. */
  inline bool isConcrete() const { return isConcrete_; }

  /** An enum type is semi-concrete if some of its constants, but not the
      first constant, have an initializer part defined. */
  inline bool isSemiConcrete() const {
    return !isAbstract() && !isConcrete();
  }
};

} // end namespace types
} // end namespace chpl

#endif
