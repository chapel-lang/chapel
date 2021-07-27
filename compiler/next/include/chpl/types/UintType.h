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

#ifndef CHPL_TYPES_UINTTYPE_H
#define CHPL_TYPES_UINTTYPE_H

#include "chpl/types/PrimitiveType.h"

namespace chpl {
namespace types {


/**
  This class represents a uint type, e.g. `uint` or `uint(32)`.
 */
class UintType : public PrimitiveType {
 private:
  UintType(int bitwidth)
    : PrimitiveType(typetags::UintType, bitwidth)
  { }

  bool contentsMatchInner(const Type* other) const override {
    return primitiveTypeContentsMatchInner((PrimitiveType*) other);
  }

  void markUniqueStringsInner(Context* context) const override {
    primitiveTypeMarkUniqueStringsInner(context);
  }

  static const owned<UintType>& getUintType(Context* context, int bitwidth);

 public:
  ~UintType() = default;

  static const UintType* get(Context* context, int bitwidth);

  int bitwidth() const override {
    return bitwidth_;
  }

  const char* c_str() const override {
    switch (bitwidth_) {
      case 8:
        return "uint(8)";
      case 16:
        return "uint(16)";
      case 32:
        return "uint(32)";
      case 64:
        return "uint(64)";
      default:
        assert(false && "int bit width case not handled");
        return "uint(<unknown>)";
    }
  }
};


} // end namespace uast
} // end namespace chpl

#endif
