/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

/*
  A type representing an interface constraint, either generic (as in
  'hashable') or instantiated with a set of types as actuals (e.g.,
  'hashable(int)').

  This type does not mean that the actuals ('int' in the above example)
  satisfy the interface; rather, it can be thought of as a claim that
  the interface is satisfied, which may be true or false.

  The following cases both create instances of InterfaceType:

    record R : I {} // the 'I' has type InterfaceType(name='I', ...)
    implements I(R); // the 'I(R)' has type InterfaceType(name='I', subs = {... => R })

*/
class InterfaceType final : public Type {
 public:
  using SubstitutionsMap = CompositeType::SubstitutionsMap;

 private:
  // The ID of the interface's declaration
  ID id_;
  // The name of the interface, for convenience
  UniqueString name_;
  // The substitutions for the interface's formals. If the interface is
  // generic, this will be empty; otherwise, this will have exactly one
  // substitution for each of the interface's formals.
  SubstitutionsMap subs_;

  // check that the substitutions are valid for the given interface.
  // executed from an assertion, so does not haev an impact in release mode.
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

  // computed from substitutions, c.f. CompositeType
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

  static const InterfaceType* getContextManagerType(Context* context);

  /* Get an interface type, assigning the types in the list to the interface's
     formals in order. If the number of types does not match the interface's
     number of formals, returns nullptr.
    */
  static const InterfaceType* withTypes(Context* context,
                                        const InterfaceType* ift,
                                        std::vector<QualifiedType> types);

  const ID& idForAssociatedType(Context* context, UniqueString name) const;

  /** Returns true if 'this' is an instantiation of genericType */
  bool isInstantiationOf(Context* context,
                         const InterfaceType* genericType) const;

  const Type* substitute(Context* context,
                         const PlaceholderMap& subs) const override {
    return get(context, id_, name_, resolution::substituteInMap(context, subs_, subs));
  }

  /* Get the ID of the interface. */
  const ID& id() const { return id_; }

  /* Get the name of the interface. */
  UniqueString name() const { return name_; }

  /* Get the substitutions for the interface (mapping formals to thir types). */
  const SubstitutionsMap& substitutions() const { return subs_; }

  void stringify(std::ostream& ss, StringifyKind stringKind) const override;
};

} // end namespace types
} // end namespace chpl

#endif
