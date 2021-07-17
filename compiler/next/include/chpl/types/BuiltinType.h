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

#ifndef CHPL_TYPES_BUILTINTYPE_H
#define CHPL_TYPES_BUILTINTYPE_H

#include "chpl/types/Type.h"

namespace chpl {
namespace types {


/**
  This class represents a builtin type that only needs a name to
  be used by resolution.

  For example, `numeric`, `borrowed`, `chpl_anyreal`.

  Types that are used very commonly (e.g. AnyType, UnknownType) should get
  their own clases.

  Types that have need for different fields (e.g. PrimitiveType) should get
  their own class.

 */
class BuiltinType : public Type {
 public:
  enum Kind {
    // concrete builtin types

    // generic builtin types
    // (note: isGeneric relies on NUMERIC being the first)
    NUMERIC,

    ANY_BOOL,
    ANY_REAL,
    BORROWED,
  };

 private:
  Kind kind_;

  BuiltinType(Kind kind)
    : Type(typetags::BuiltinType), kind_(kind) {
  }

  bool contentsMatchInner(const Type* other) const override {
    const BuiltinType* lhs = this;
    const BuiltinType* rhs = (const BuiltinType*) other;
    return lhs->kind_ == rhs->kind_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  bool isGeneric() override {
    return kind_ >= NUMERIC;
  }

  static const owned<BuiltinType>& getBuiltinType(Context* context, Kind kind);

 public:
  ~BuiltinType() = default;

  static const BuiltinType* get(Context* context, Kind kind);

  /**
    Returns the kind indicating which BuiltinType it is.
   */
  Kind kind() const {
    return kind_;
  }

  /**
    Returns a C string for the name of this BuiltinType.
   */
  const char* c_str() const;

  /*bool operator==(const BuiltinType& other) const {
    return completeMatch(&other);
  }
  bool operator!=(const BoolType& other) const {
    return !(*this == other);
  }*/
};


} // end namespace uast
} // end namespace chpl

#endif
