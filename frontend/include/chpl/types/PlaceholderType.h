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

#ifndef CHPL_TYPES_PLACEHOLDER_TYPE_H
#define CHPL_TYPES_PLACEHOLDER_TYPE_H

#include "chpl/types/Type.h"
#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace types {

/* An opaque placeholder type that was introduced for some AST node.
   Currently, these are used for the interface resolution process, where
   they serve both as temporary types for interface formals like 'Self',
   and as the types of 'type queries'. In the following code:

     interface I {
       proc Self.foo(x: ?t1, y: ?t2);
     }

   All of 'Self', 't1', and 't2' will have type 'PlaceholderType(..)', with IDs given
   by the (implicit) formal declaration and the type queries, respectively.

   Because placeholder types are unique (created from a particular position
   in the AST), they cannot be passed to any concrete function argument. This
   means that when searching for witnesses for 'foo', functions with concrete 'x'
   will always be rejected (as desired). Moreover, as part of checking applicability,
   placeholder types will be substituted for type queries in the candidate
   (non-interface) function, and thus enforce a matching pattern of genericity.

   As an example, the following function will match the constraint above:

     proc R.foo(x, y) {}

   because 'x' will be instantiated with 'PlaceholderType(t1)', and
   'y' with 'PlaceholderType(t2)'. However, the following function will not match:

     proc R.foo(x, y: x.type) {}

   Because the type of 'y' will be 'PlaceholderType(t1)', which is not
   equal to 'PlaceholderType(t2)'. This is desired because the interface
   constraint asks for a function that can be called with any two
   (possibly different) types. A function that supports generic types
   but constrains the two formals to have equal types does not fulfill that
   ask.

   Placeholder types can be eliminated from types using the 'substitute' method,
   which will replace them with the corresponding "real" type.
 */
class PlaceholderType final : public Type {
 private:
  // the syntactic ID for which this placeholder type was created
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

  const Type* substitute(Context* context,
                         const PlaceholderMap& subs) const override {
    auto it = subs.find(id_);
    if (it != subs.end()) {
      return it->second;
    }
    return this;
  }

  /* Get the ID for this placeholder type. */
  const ID& id() const { return id_; }

  void stringify(std::ostream& ss,
                 chpl::StringifyKind stringKind) const override;
};

} // end namespace types
} // end namespace chpl

#endif
