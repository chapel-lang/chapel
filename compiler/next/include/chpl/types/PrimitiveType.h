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

#ifndef CHPL_TYPES_PRIMITIVETYPE_H
#define CHPL_TYPES_PRIMITIVETYPE_H

#include "chpl/types/Type.h"

namespace chpl {
namespace types {


/**
  This class represents a primitive numeric type (e.g. `bool`, `real(32)`)
  that can have a width.

 */
class PrimitiveType : public Type {
 protected:
  int bitwidth_;

  PrimitiveType(typetags::TypeTag tag, int bitwidth)
    : Type(tag), bitwidth_(bitwidth) {
  }

  bool primitiveTypeContentsMatchInner(const PrimitiveType* other) const {
    const PrimitiveType* lhs = this;
    const PrimitiveType* rhs = (const PrimitiveType*) other;
    return lhs->bitwidth_ == rhs->bitwidth_;
  }

  void primitiveTypeMarkUniqueStringsInner(Context* context) const {
  }

  Genericity genericity() const override {
    return CONCRETE;
  }

 public:
  ~PrimitiveType() = default;

  /**
   Returns the bit width selected for numeric types.
   Returns 8 for the default sized bool.
   */
  virtual int bitwidth() const = 0;

  /**
   Returns true if this PrimitiveType has the default width.
   */
  virtual bool isDefaultWidth() const = 0;

  /**
    Returns a C string for the name of this PrimitiveType.
   */
  virtual const char* c_str() const = 0;

  /**
   Compute a string representing this type.
   */
  void stringify(std::ostream& ss,
                 chpl::StringifyKind stringKind) const override;

  /**
   Get the PrimitiveType according to name and bitwidth,
   or nullptr if the name or bitwidth are invalid.
   */
  static const PrimitiveType*
  getWithNameAndWidth(Context* context, UniqueString name, int bitwidth);
};


} // end namespace uast
} // end namespace chpl

#endif
