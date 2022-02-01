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
  TupleType(ID id, UniqueString name,
            const TupleType* instantiatedFrom,
            SubstitutionsMap subs)
    : CompositeType(typetags::TupleType, id, name,
                    instantiatedFrom, std::move(subs))
  { }


  bool contentsMatchInner(const Type* other) const override {
    return compositeTypeContentsMatchInner((const CompositeType*) other);
  }

  void markUniqueStringsInner(Context* context) const override {
    compositeTypeMarkUniqueStringsInner(context);
  }

  static const owned<TupleType>&
  getTupleType(Context* context, ID id, UniqueString name,
               const TupleType* instantiatedFrom,
               SubstitutionsMap subs);

 public:
  static const TupleType*
  get(Context* context, ID id, UniqueString name,
      const TupleType* instantiatedFrom,
      CompositeType::SubstitutionsMap subs);

  ~TupleType() = default;

  /** If this type represents an instantiated type,
      returns the type it was instantiated from.

      This is just instantiatedFromCompositeType() with the
      result cast to TupleType.
   */
  const TupleType* instantiatedFrom() const {
    const CompositeType* ret = instantiatedFromCompositeType();
    assert(ret == nullptr || ret->tag() == typetags::TupleType);
    return (const TupleType*) ret;
  }
};


} // end namespace uast



} // end namespace chpl

#endif
