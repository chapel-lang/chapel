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

#ifndef CHPL_TYPES_BOOLTYPE_H
#define CHPL_TYPES_BOOLTYPE_H

#include "chpl/types/PrimitiveType.h"

namespace chpl {
namespace types {


/**
  This class represents a bool type, e.g. `bool` or `bool(32)`.

  Note that `bool` normally has width `8` but `bool(8)` is a separate type.
 */
class BoolType final : public PrimitiveType {
 private:
  BoolType()
    : PrimitiveType(typetags::BoolType, 0)
  { }

  bool contentsMatchInner(const Type* other) const override {
    return primitiveTypeContentsMatchInner((PrimitiveType*) other);
  }

  void markUniqueStringsInner(Context* context) const override {
    primitiveTypeMarkUniqueStringsInner(context);
  }

  static const owned<BoolType>& getBoolType(Context* context);

  /** what is stored in bitwidth_ for the default 'bool' ? */
  static int defaultBitwidth() {
    return 0;
  }

 public:
  ~BoolType() = default;

  static const BoolType* get(Context* context);

  int bitwidth() const override {
    return 8; // Chapel doesn't guarantee this bitwidth, so this seems fragile...
  }

  bool isDefaultWidth() const override {
    return true;
  }

  const char* c_str() const override {
    return "bool";
  }
};


} // end namespace uast
} // end namespace chpl

#endif
