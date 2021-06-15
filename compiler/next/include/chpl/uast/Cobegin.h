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

#ifndef CHPL_UAST_COBEGIN_H
#define CHPL_UAST_COBEGIN_H

#include "chpl/queries/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/SimpleBlockLike.h"
#include "chpl/uast/WithClause.h"

namespace chpl {
namespace uast {


/**
  This class represents a cobegin statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var x = 0;
      cobegin {
        writeln(x);
      }

  \endrst

 */
class Cobegin final : public Expression {
 private:
  Cobegin(ASTList children, int8_t withClauseChildNum, int bodyChildNum,
          int numTaskBodies)
    : Expression(asttags::Cobegin, std::move(children)),
      withClauseChildNum_(withClauseChildNum),
      bodyChildNum_(bodyChildNum),
      numTaskBodies_(numTaskBodies) {
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override;

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  int8_t withClauseChildNum_;
  int bodyChildNum_;
  int numTaskBodies_;

 public:

  /**
    Create and return a cobegin statement. 
  */
  static owned<Cobegin> build(Builder* builder, Location loc,
                              owned<WithClause> withClause,
                              ASTList taskBodies);

  /**
    Returns the with clause of this cobegin statement, or nullptr if there
    is none.
  */
  const WithClause* withClause() const {
    if (withClauseChildNum_ < 0) return nullptr;
    auto ret = child(withClauseChildNum_);
    assert(ret->isWithClause());
    return (const WithClause*)ret;
  }

  /**
    Return a way to iterate over the task bodies.
   */
  ASTListIteratorPair<Expression> taskBodies() const {
    auto begin = children_.begin() + bodyChildNum_;
    auto end = begin + numTaskBodies_ - 1;
    return ASTListIteratorPair<Expression>(begin, end);
  }

  /**
    Return the number of task bodies in this.
  */
  int numTaskBodies() const {
    return this->numTaskBodies_;
  }

  /**
    Return the i'th task body in this.
  */
  const Expression* taskBody(int i) const {
    assert(i >= 0 && i < numTaskBodies_);
    const ASTNode* ast = this->child(i + bodyChildNum_);
    assert(ast->isExpression());
    return (const Expression*)ast;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
