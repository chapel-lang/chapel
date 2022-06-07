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

#ifndef CHPL_TYPES_RECORD_TYPE_H
#define CHPL_TYPES_RECORD_TYPE_H

#include "chpl/types/CompositeType.h"

namespace chpl {
namespace types {


/**
  This class represents a record type, e.g., if we have `record R`,
  then `R` refers to a RecordType.
 */
class RecordType final : public CompositeType {
 private:
  RecordType(ID id, UniqueString name,
             const RecordType* instantiatedFrom,
             SubstitutionsMap subs)
    : CompositeType(typetags::RecordType, id, name,
                    instantiatedFrom, std::move(subs))
  { }

  bool contentsMatchInner(const Type* other) const override {
    return compositeTypeContentsMatchInner((const CompositeType*) other);
  }

  void markUniqueStringsInner(Context* context) const override {
    compositeTypeMarkUniqueStringsInner(context);
  }

  static const owned<RecordType>&
  getRecordType(Context* context, ID id, UniqueString name,
                const RecordType* instantiatedFrom,
                SubstitutionsMap subs);

 public:

  /** Get a record type for an ID, possibly instantiated with substitutions */
  static const RecordType* get(Context* context, ID id, UniqueString name,
                               const RecordType* instantiatedFrom,
                               CompositeType::SubstitutionsMap subs);

  /** Get the string type */
  static const RecordType* getStringType(Context* context);

  /** Get the bytes type */
  static const RecordType* getBytesType(Context* context);

  ~RecordType() = default;

  /** If this type represents an instantiated type,
      returns the type it was instantiated from.

      This is just instantiatedFromCompositeType() with the
      result cast to RecordType.
   */
  const RecordType* instantiatedFrom() const {
    const CompositeType* ret = instantiatedFromCompositeType();
    assert(ret == nullptr || ret->tag() == typetags::RecordType);
    return (const RecordType*) ret;
  }

};


} // end namespace uast


} // end namespace chpl

#endif
