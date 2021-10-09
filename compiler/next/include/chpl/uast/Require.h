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

#ifndef CHPL_UAST_REQUIRE_H
#define CHPL_UAST_REQUIRE_H

#include "chpl/uast/Call.h"
#include "chpl/queries/Location.h"

namespace chpl {
namespace uast {


/**
  This class represents a require statement.
*/
class Require final : public Call {
 private:
  Require(ASTList children)
    : Call(asttags::Require, std::move(children),
           /*hasCalledExpression*/ false) {
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    return callContentsMatchInner(other->toCall());
  }

  void markUniqueStringsInner(Context* context) const override {
    callMarkUniqueStringsInner(context);
  }

 public:
  ~Require() override = default;

  /**
    Create and return a require statement.
  */
  static owned<Require> build(Builder* builder, Location loc,
                              ASTList exprs);

};


} // end namespace uast
} // end namespace chpl

#endif
