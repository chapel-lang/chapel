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

#ifndef CHPL_TYPES_ENUM_TYPE_H
#define CHPL_TYPES_ENUM_TYPE_H

#include "chpl/types/Type.h"

namespace chpl{
namespace types {

class EnumType final : public Type {
 private:
  ID id_;
  UniqueString name_;

  EnumType(ID id, UniqueString name)
    : Type(typetags::EnumType), id_(id), name_(name) {}

  static const owned<EnumType>& getEnumType(Context* context, ID id, UniqueString name);

 protected:
  bool contentsMatchInner(const Type* other) const override {
    auto otherEnum = (const EnumType*) other;
    return id_ == otherEnum->id_ && name_ == otherEnum->name_;
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

  /** Get the type for a range's BoundedRangeType */
  static const EnumType* getBoundedRangeTypeType(Context* context);

  ~EnumType() = default;

  virtual void stringify(std::ostream& ss,
                         StringifyKind stringKind) const override;

  /** Return the uAST ID associated with this EnumType */
  const ID& id() const { return id_; }

  /** Return the name of the uAST associated with this EnumType */
  const UniqueString& name() const { return name_; }
};

} // end namespace types
} // end namespace chpl

#endif
