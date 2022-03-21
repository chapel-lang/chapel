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

#ifndef CHPL_UAST_TUPLE_H
#define CHPL_UAST_TUPLE_H

#include "chpl/queries/Location.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/Call.h"

namespace chpl {
namespace uast {


/**
  This class represents a tuple literal. For example:

  \rst
  .. code-block:: chapel

    // Here '(1, 2, 3)' is a tuple literal.
    var x = (1, 2, 3);

  \endrst
*/
class Tuple final : public Call {
 private:
  // TODO: Record trailing comma?
  Tuple(AstList children)
    : Call(asttags::Tuple, std::move(children),
           /*hasCalledExpression*/ false) {
    assert(numChildren() >= 1);
  }

  bool contentsMatchInner(const AstNode* other) const override {
    return this->callContentsMatchInner(other->toCall());
  }

  void markUniqueStringsInner(Context* context) const override {
    callMarkUniqueStringsInner(context);
  }

 public:
  ~Tuple() override = default;

  /**
    Create and return a tuple.
  */
  static owned<Tuple> build(Builder* builder,
                            Location loc,
                            AstList exprs);

};


} // end namespace uast
} // end namespace chpl

#endif
