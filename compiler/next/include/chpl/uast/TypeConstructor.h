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

#ifndef CHPL_UAST_TYPE_CONSTRUCTOR_H
#define CHPL_UAST_TYPE_CONSTRUCTOR_H

#include "chpl/uast/Call.h"
#include "chpl/queries/Location.h"

namespace chpl {
namespace uast {


//
// TODO: Better name for me besides TypeConstructor?
//

/**
  This class represents a type constructor. For example:

  \rst
  .. code-block:: chapel

      // Both 'atomic int' and 'sync int' are type constructors.
      var v1: atomic int;
      var v2: sync int;

  \endrst
*/
class TypeConstructor final : public Call {
 private:
  TypeConstructor(ASTList children)
    : Call(asttags::TypeConstructor, std::move(children),
           /*hasCalledExpression*/ false) {
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    return callContentsMatchInner(other->toCall());
  }

  void markUniqueStringsInner(Context* context) const override {
    callMarkUniqueStringsInner(context);
  }

 public:
  ~TypeConstructor() override = default;

  /**
    Create and return a type constructor.
  */
  static owned<TypeConstructor> build(Builder* builder, Location loc,
                                      ASTList actuals);

};


} // end namespace uast
} // end namespace chpl

#endif
