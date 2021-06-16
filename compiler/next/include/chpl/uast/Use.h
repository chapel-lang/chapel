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

#ifndef CHPL_UAST_USE_H
#define CHPL_UAST_USE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/UseClause.h"

namespace chpl {
namespace uast {


/**
  This class represents a use statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      use Foo, Bar as A;

  \endrst

  This creates a use statement that has two use clauses, 'use Foo' and
  'use Bar as A'.
*/
class Use final : public Expression {
 private:
  Use(ASTList children)
    : Expression(asttags::Use, std::move(children)) {
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    return expressionContentsMatchInner(other->toExpression());
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

 public:

  /**
    Create and return a use statement.
  */
  static owned<Use> build(Builder* builder, Location loc,
                          ASTList useClauses);

  /**
    Return a way to iterate over the uses clauses.
  */
  ASTListIteratorPair<UseClause> useClauses() const {
    return ASTListIteratorPair<UseClause>(children_.begin(),
                                          children_.end());
  }

  /**
    Return the number of use clauses in this use statement.
  */
  int numUseClauses() const {
    return this->numChildren();
  }

  /**
    Return the i'th use clause in this use statement.
  */
  const UseClause* useClause(int i) const {
    auto ret = this->child(i);
    assert(ast->isUseClause());
    return (const UseClause*)ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
