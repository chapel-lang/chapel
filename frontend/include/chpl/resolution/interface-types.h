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

#ifndef CHPL_RESOLUTION_INTERFACE_TYPES_H
#define CHPL_RESOLUTION_INTERFACE_TYPES_H

#include "chpl/framework/ID.h"
#include "chpl/framework/update-functions.h"
#include "chpl/types/InterfaceType.h"
#include "chpl/util/hash.h"

namespace chpl {
namespace resolution {

/*
  Represent a resolved form of an 'implements'-like statement, which asserts
  that a (possibly-generic) list of types satisfies an interface. All
  of the following are transformed into implementation points:

    record R : I {}
    implements R(I);
    R implements I;

  The types of the "actuals" (R in the above examples) are stored as substitutions
  for the InterfaceType. This means that in the case of a concrete implementation
  point, its interface() will be equal to constraint being searched for.
 */
class ImplementationPoint {
 private:
  // The interface being implemented (instantiated with the types of the arguments)
  const types::InterfaceType* interface_;
  // The ID of the implementation statement
  ID id_;

  ImplementationPoint(const types::InterfaceType* interface,
                      ID id)
    : interface_(interface), id_(id) {}

  static owned<ImplementationPoint> const&
  getImplementationPoint(Context* context,
                         const types::InterfaceType* interface,
                         ID id);

 public:
  static const ImplementationPoint*
  get(Context* context, const types::InterfaceType* interface, ID id);

  static bool update(owned<ImplementationPoint>& lhs,
                     owned<ImplementationPoint>& rhs) {
    return defaultUpdateOwned(lhs, rhs);
  }
  bool operator==(const ImplementationPoint& other) const {
    return interface_ == other.interface_ &&
           id_ == other.id_;
  }
  bool operator!=(const ImplementationPoint& other) const {
    return !(*this == other);
  }
  void mark(Context* context) const;
  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /* get the (possibly-generic) interface being implemented, which includes
     substitutions from the actuals. */
  const types::InterfaceType* interface() const { return interface_; }

  /* get the ID of the implementation point (statement or inheritance expression) */
  const ID& id() const { return id_; }
};

/*
   Represents evidence that a particular type or list of types implements
   a given interface. This includes:

   * associated constraints (e.g., a 'totalOrder' interface requires a
     'partialOrder' interface to be satisfied).
   * associated types (e.g., a 'collection' interface requires a 'Element'
     type to be defined; in the standard library, the 'contextManager' interface
     specifies the type of the underlying resource).
   * required functions (e.g., a 'hashable' interface requires a 'hash' function
     to be defined).
 */
class ImplementationWitness {
 public:
  using ConstraintMap = std::unordered_map<ID, const ImplementationWitness*>;
  using AssociatedTypeMap = types::PlaceholderMap;
  using FunctionMap = std::unordered_map<ID, ID>;

 private:

  ConstraintMap associatedConstraints_;
  AssociatedTypeMap associatedTypes_;
  FunctionMap requiredFns_;

  ImplementationWitness(ConstraintMap associatedConstraints,
                        AssociatedTypeMap associatedTypes,
                        FunctionMap requiredFns)
    : associatedConstraints_(std::move(associatedConstraints)),
      associatedTypes_(std::move(associatedTypes)),
      requiredFns_(std::move(requiredFns)) {}

  static const owned<ImplementationWitness>&
  getImplementationWitness(Context* context,
                           ConstraintMap associatedConstraints,
                           AssociatedTypeMap associatedTypes,
                           FunctionMap requiredFns);

 public:
  static ImplementationWitness*
  get(Context* context,
      ConstraintMap associatedConstraints,
      AssociatedTypeMap associatedTypes,
      FunctionMap requiredFns);

  static bool update(owned<ImplementationWitness>& lhs,
                     owned<ImplementationWitness>& rhs) {
    return defaultUpdateOwned(lhs, rhs);
  }
  bool operator==(const ImplementationWitness& other) const {
    return associatedConstraints_ == other.associatedConstraints_ &&
           associatedTypes_ == other.associatedTypes_ &&
           requiredFns_ == other.requiredFns_;
  }
  bool operator!=(const ImplementationWitness& other) const {
    return !(*this == other);
  }
  void mark(Context* context) const {
    chpl::mark<decltype(associatedTypes_)>{}(context, associatedTypes_);
    chpl::mark<decltype(requiredFns_)>{}(context, requiredFns_);
    chpl::mark<decltype(associatedConstraints_)>{}(context, associatedConstraints_);
  }
  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /** Get the associated constraints. */
  const ConstraintMap& associatedConstraints() const {
    return associatedConstraints_;
  }

  /** Get the associated types. */
  const AssociatedTypeMap& associatedTypes() const {
    return associatedTypes_;
  }

  /** Get the required functions. */
  const FunctionMap& requiredFns() const {
    return requiredFns_;
  }
};

} // end namespace resolution
} // end namespace chpl


namespace std {

template<> struct hash<chpl::resolution::ImplementationWitness::ConstraintMap> {
  size_t operator()(const chpl::resolution::ImplementationWitness::ConstraintMap& key) const {
    return chpl::hashUnorderedMap(key);
  }
};

template<> struct hash<chpl::resolution::ImplementationWitness::FunctionMap> {
  size_t operator()(const chpl::resolution::ImplementationWitness::FunctionMap& key) const {
    return chpl::hashUnorderedMap(key);
  }
};

}
#endif
