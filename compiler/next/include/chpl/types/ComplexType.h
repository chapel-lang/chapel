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

#ifndef CHPL_TYPES_COMPLEXTYPE_H
#define CHPL_TYPES_COMPLEXTYPE_H

#include "chpl/types/PrimitiveType.h"

namespace chpl {
namespace types {


/**
  This class represents a complex type, e.g. `complex` or `complex(64)`.
 */
class ComplexType final : public PrimitiveType {
 private:
  ComplexType(int bitwidth)
    : PrimitiveType(typetags::ComplexType, bitwidth)
  { }

  bool contentsMatchInner(const Type* other) const override {
    return primitiveTypeContentsMatchInner((PrimitiveType*) other);
  }

  void markUniqueStringsInner(Context* context) const override {
    primitiveTypeMarkUniqueStringsInner(context);
  }

  static const owned<ComplexType>& getComplexType(Context* context, int bitwidth);

 public:
  ~ComplexType() = default;

  static const ComplexType* get(Context* context, int bitwidth);

  int bitwidth() const override {
    return bitwidth_;
  }

  const char* c_str() const override {
    switch (bitwidth_) {
      case 64:
        return "complex(64)";
      case 128:
        return "complex(128)";
      default:
        assert(false && "complex bit width case not handled");
        return "complex(<unknown>)";
    }
  }
};


} // end namespace uast
} // end namespace chpl

#endif
