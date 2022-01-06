/*
 * Copyright 2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_TYPES_NOTHINGTYPE_H
#define CHPL_TYPES_NOTHINGTYPE_H

#include "chpl/types/Type.h"

namespace chpl {
namespace types {


/**
  This class represents the nothing type.
 */
class NothingType final : public Type {
 private:
  NothingType() : Type(typetags::NothingType) { }

  bool contentsMatchInner(const Type* other) const override {
    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  bool isGeneric() const override {
    return false;
  }

  static const owned<NothingType>& getNothingType(Context* context);

 public:
  ~NothingType() = default;

  static const NothingType* get(Context* context);
};


} // end namespace uast
} // end namespace chpl

#endif
