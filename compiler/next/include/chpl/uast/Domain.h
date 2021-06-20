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

#ifndef CHPL_UAST_DOMAIN_H
#define CHPL_UAST_DOMAIN_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents a domain expression. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var d = { 'foo', 'bar', 'baz' };

  \endrst

 */
class Domain final : public Expression {
 private:
  // TODO: Record if initializer list has trailing comma?
  Domain(ASTList children)
    : Expression(asttags::Domain, std::move(children)) {
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    return expressionContentsMatchInner(other->toExpression());
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

 public:
  ~Domain() override = default;

  /**
   Create and return a Domain expression.
   */
  static owned<Domain> build(Builder* builder, Location loc,
                             ASTList exprs);

  /**
    Return a way to iterate over the expressions of this domain.
  */
  ASTListIteratorPair<Expression> exprs() const {
    return ASTListIteratorPair<Expression>(children_.begin(),
                                           children_.end());
  }

  /**
    Return the number of expressions in this domain.
  */
  int numExprs() const {
    return this->numChildren();
  }

  /**
    Return the i'th expression in this domain.
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
