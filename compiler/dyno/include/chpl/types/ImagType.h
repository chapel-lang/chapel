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

#ifndef CHPL_TYPES_IMAGTYPE_H
#define CHPL_TYPES_IMAGTYPE_H

#include "chpl/types/PrimitiveType.h"

namespace chpl {
namespace types {


/**
  This class represents a imag type, e.g. `imag` or `imag(32)`.
 */
class ImagType final : public PrimitiveType {
 private:
  ImagType(int bitwidth)
    : PrimitiveType(typetags::ImagType, bitwidth)
  { }

  bool contentsMatchInner(const Type* other) const override {
    return primitiveTypeContentsMatchInner((PrimitiveType*) other);
  }

  void markUniqueStringsInner(Context* context) const override {
    primitiveTypeMarkUniqueStringsInner(context);
  }

  static const owned<ImagType>& getImagType(Context* context, int bitwidth);

  /** what is stored in bitwidth_ for the default 'imag' ? */
  static int defaultBitwidth() {
    return 64;
  }

 public:
  ~ImagType() = default;

  static const ImagType* get(Context* context, int bitwidth);

  int bitwidth() const override {
    return bitwidth_;
  }

  bool isDefaultWidth() const override {
    return bitwidth_ == defaultBitwidth();
  }

  const char* c_str() const override {
    switch (bitwidth_) {
      case 32:
        return "imag(32)";
      case 64:
          return "imag(64)";
      default:
        assert(false && "imag bit width case not handled");
        return "real(<unknown>)";
    }
  }
};


} // end namespace uast
} // end namespace chpl

#endif
