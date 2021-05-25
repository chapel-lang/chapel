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

#ifndef CHPL_UAST_WITHCLAUSE_H
#define CHPL_UAST_WITHCLAUSE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents a with clause. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      forall myRange with (var x = 0) {
        writeln(x);
      }

  \endrst

  Creates a forall loop that has a with clause which declares a single
  task variable named 'x'.
*/
class WithClause final : public Expression {
 private:
  WithClause(ASTList exprs)
    : Expression(asttags::WithClause, std::move(exprs)) {
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:

  /**
    Create and return a with clause.
  */
  static owned<WithClause> build(Builder* builder, Location loc,
                                 ASTList exprs);

  /**
    Return a way to iterate over the expressions of this with clause.
  */
  ASTListIteratorPair<Expression> exprs() const {
    return ASTListIteratorPair<Expression>(children_.begin(),
                                           children_.end());
  }

  /**
    Return the number of expressions in this with clause.
  */
  int numExprs() const {
    return this->numChildren();
  }

  /**
    Return the i'th expression in this with clause.
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
