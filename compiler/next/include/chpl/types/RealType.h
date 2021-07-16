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

#ifndef CHPL_TYPES_REALTYPE_H
#define CHPL_TYPES_REALTYPE_H

#include "chpl/types/PrimitiveType.h"

namespace chpl {
namespace types {


/**
  This class represents a real type, e.g. `real` or `real(32)`.
 */
class RealType : public PrimitiveType {
 private:
  RealType(int bitwidth)
    : PrimitiveType(typetags::RealType, bitwidth)
  { }

  void canonicalizeBitWidth() override {
    if (bitwidth_ == 0) bitwidth_ = 64;
    assert(bitwidth_ == 8 || bitwidth_ == 16 ||
           bitwidth_ == 32 || bitwidth_ == 64);
  }

  bool contentsMatchInner(const Type* other) const override {
    return primitiveTypeContentsMatchInner((PrimitiveType*) other);
  }

  void markUniqueStringsInner(Context* context) const override {
    primitiveTypeMarkUniqueStringsInner(context);
  }

 public:
  ~RealType() = default;

  int bitwidth() const override {
    return bitwidth_;
  }

  const char* c_str() const override {
    switch (bitwidth_) {
      case 32:
        return "real(32)";
      case 64:
          return "real(64)";
      default:
        assert(false && "real bit width case not handled");
        return "real(<unknown>)";
    }
  }

  static const owned<RealType>& build(Context* context, int bitwidth);
};


} // end namespace uast
} // end namespace chpl

#endif
