/*
 * Copyright 2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_TYPES_PLACEHOLDER_TYPE_H
#define CHPL_TYPES_PLACEHOLDER_TYPE_H

#include "chpl/types/Type.h"

namespace chpl {
namespace types {

class PlaceholderType final : public Type {
 private:
  ID id_;

  PlaceholderType(ID id)
    : Type(typetags::PlaceholderType), id_(std::move(id)) {}

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = (const PlaceholderType*) other;
    return id_ == rhs->id_;
  }

  void markUniqueStringsInner(Context* context) const override {
    id_.mark(context);
  }

  Genericity genericity() const override {
    return CONCRETE;
  }

  static owned<PlaceholderType> const&
  getPlaceholderType(Context* context,
                   ID id);

 public:
  static const PlaceholderType* get(Context* context,
                                    ID id);

  const ID& id() const { return id_; }
};

} // end namespace types
} // end namespace chpl

#endif
