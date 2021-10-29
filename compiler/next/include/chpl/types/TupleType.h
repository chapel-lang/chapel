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

#ifndef CHPL_TYPES_TUPLE_TYPE_H
#define CHPL_TYPES_TUPLE_TYPE_H

#include "chpl/types/CompositeType.h"

namespace chpl {
namespace types {


/**
  This class represents a tuple type e.g. `(int, real)`.
 */
class TupleType final : public CompositeType {
 private:
  TupleType(/* args todo */)
    : CompositeType(typetags::TupleType)
  { }

  bool contentsMatchInner(const Type* other) const override {
    return compositeTypeContentsMatchInner((const CompositeType*) other);
  }

  void markUniqueStringsInner(Context* context) const override {
    compositeTypeMarkUniqueStringsInner(context);
  }

 public:
  ~TupleType() = default;

  static const TupleType* get(Context* context /* args todo */);
};


} // end namespace uast
} // end namespace chpl

#endif
