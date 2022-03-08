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

#ifndef CHPL_UAST_REQUIRE_H
#define CHPL_UAST_REQUIRE_H

#include "chpl/uast/Expression.h"
#include "chpl/queries/Location.h"

namespace chpl {
namespace uast {


/**
  This class represents a require statement.

  \rst
  .. code-block:: chapel

    // 'Require' tells the compiler where to look for C functions:
    require "foo.h", "foo.c";

  /endrst
*/
class Require final : public Expression {
 private:
  Require(ASTList children)
    : Expression(asttags::Require, std::move(children)) {
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    return expressionContentsMatchInner(other->toExpression());
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

 public:
  ~Require() override = default;

  /**
    Create and return a require statement.
  */
  static owned<Require> build(Builder* builder, Location loc,
                              ASTList exprs);

  /**
    Return a way to iterate over the expressions of this require statement.
  */
  ASTListIteratorPair<Expression> exprs() const {
    return ASTListIteratorPair<Expression>(children_.begin(),
                                           children_.end());
  }

  /**
    Return the number of expressions in this require statement.
  */
  int numExprs() const {
    return this->numChildren();
  }

  /**
    Return the i'th expression in this require statement.
  */
  const Expression* expr(int i) const {
    const ASTNode* ast = this->child(i);
    assert(ast->isExpression());
    return (const Expression*)ast;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
