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
  This class represents a builtin type, e.g. `int`.

 */
class BuiltinType : public Type {
 public:
  enum Kind {
    // concrete builtin types
    BOOL,
    COMPLEX,
    IMAG,
    INT,
    REAL,
    UINT,

    // generic builtin types
    // (note: isGeneric relies on NUMERIC being the first)
    NUMERIC,
  };

 private:
  Kind kind_;
  int bitwidth_;

  BuiltinType(Kind kind, int bitwidth)
    : Type(typetags::BuiltinType), kind_(kind), bitwidth_(bitwidth) {
    canonicalizeBitWidth();
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

  void canonicalizeBitWidth();

 public:
  ~BuiltinType() = default;

  static owned<BuiltinType> build(Kind kind, int bitwidth);

  /**
    Returns the kind indicating which BuiltinType it is.
   */
  Kind kind() const {
    return kind_;
  }

  /**
   Returns the bit width selected for numeric types.
   Returns 8 for the default sized bool.
   */
  int bitwidth() const {
    if (isDefaultBool()) return 8;
    return bitwidth_;
  }

  /**
   Returns `true` if this is the type `bool` which is distinct from
   `bool(8)`.
   */
  bool isDefaultBool() const {
    return kind_ == BOOL && bitwidth_ == 0;
  }

  /**
    Returns a C string for the name of this BuiltinType.
   */
  const char* c_str() const;
};


} // end namespace uast
} // end namespace chpl

#endif
