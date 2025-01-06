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

#ifndef CHPL_TYPES_ANY_CLASS_TYPE_H
#define CHPL_TYPES_ANY_CLASS_TYPE_H

#include "chpl/types/ManageableType.h"
#include "chpl/framework/global-strings.h"

namespace chpl {
namespace types {


/**
  This class represents the generic `class` typeclass. It inherits from
  ManageableType so that it can be used with decorators and managers.
 */
class AnyClassType final : public ManageableType {
 private:
  AnyClassType()
    : ManageableType(typetags::AnyClassType,
                     /* ID */ ID(),
                     /* name */ USTR("class"),
                     /* instantiatedFrom */ nullptr,
                     /* subs */ SubstitutionsMap()) {
  }

  bool contentsMatchInner(const Type* other) const override {
    const AnyClassType* rhs = (const AnyClassType*) other;
    return compositeTypeContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    compositeTypeMarkUniqueStringsInner(context);
  }

  static const owned<AnyClassType>& getAnyClassType(Context* context);
 public:
  ~AnyClassType() = default;

  static const AnyClassType* get(Context* context);

  Type::Genericity genericity() const override {
    return GENERIC;
  }
};

} // end namespace types


} // end namespace chpl

#endif
