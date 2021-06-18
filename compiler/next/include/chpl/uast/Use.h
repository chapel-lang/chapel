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
#include "chpl/uast/Decl.h"
#include "chpl/uast/Expression.h"
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

  This creates a use statement that has two use clauses, 'Foo' and
  'Bar as A'.
*/
class Use final : public Expression {
 private:
  Use(ASTList children, Decl::Visibility visibility)
    : Expression(asttags::Use, std::move(children)),
      visibility_(visibility) {
    assert(numChildren() >= 1);
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const Use* rhs = other->toUse();
    return this->visibility_ == rhs->visibility_ &&
      this->expressionContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  Decl::Visibility visibility_;

 public:

  /**
    Create and return a use statement.
  */
  static owned<Use> build(Builder* builder, Location loc,
                          Decl::Visibility visibility,
                          ASTList useClauses);

  /**
    Return the visibility of the use clauses in this use statement.
  */
  Decl::Visibility visibility() const {
    return visibility_;
  }

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
    assert(ret->isUseClause());
    return (const UseClause*)ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
