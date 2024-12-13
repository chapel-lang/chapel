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

#ifndef CHPL_TYPES_INTERFACE_TYPE_H
#define CHPL_TYPES_INTERFACE_TYPE_H

#include "chpl/resolution/resolution-types.h"
#include "chpl/types/Type.h"
#include "chpl/types/CompositeType.h"
#include "chpl/types/QualifiedType.h"

namespace chpl {
namespace types {

class InterfaceType final : public Type {
 public:
  using SubstitutionsMap = CompositeType::SubstitutionsMap;

 private:
  ID id_;
  UniqueString name_;
  SubstitutionsMap subs_;

  static bool validateSubstitutions(Context* context,
                                    const ID& id,
                                    SubstitutionsMap& subs);

  InterfaceType(ID id, UniqueString name, SubstitutionsMap subs)
    : Type(typetags::InterfaceType), id_(std::move(id)), name_(std::move(name)),
      subs_(std::move(subs)) {}

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = (const InterfaceType*) other;
    return id_ == rhs->id_ &&
           name_ == rhs->name_ &&
           subs_ == rhs->subs_;
  }

  void markUniqueStringsInner(Context* context) const override {
    id_.mark(context);
    name_.mark(context);
    ::chpl::mark<decltype(subs_)>{}(context, subs_);
  }

  Genericity genericity() const override {
    return MAYBE_GENERIC;
  }

  static owned<InterfaceType> const&
  getInterfaceType(Context* context,
                   ID id,
                   UniqueString name,
                   SubstitutionsMap subs);

 public:
  static const InterfaceType* get(Context* context,
                                  ID id,
                                  UniqueString name,
                                  SubstitutionsMap subs);

  static const InterfaceType* withTypes(Context* context,
                                        const InterfaceType* ift,
                                        std::vector<QualifiedType> types);

  bool isInstantiationOf(Context* context,
                         const InterfaceType* genericType) const;

  const Type* substitute(Context* context,
                         const PlaceholderMap& subs) const override {
    return get(context, id_, name_, resolution::substituteInMap(context, subs_, subs));
  }

  const ID& id() const { return id_; }

  UniqueString name() const { return name_; }

  const SubstitutionsMap& substitutions() const { return subs_; }

  void stringify(std::ostream& ss, StringifyKind stringKind) const override;
};

} // end namespace types
} // end namespace chpl

#endif
