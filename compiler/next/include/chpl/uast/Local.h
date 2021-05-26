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

#ifndef CHPL_UAST_LOCAL_H
#define CHPL_UAST_LOCAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents a local statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      const flag = true;
      local flag {
        var x = 0;
        writeln(x);
      }

      // Example 2:
      var x = 0;
      local do writeln(x);
  \endrst

 */
class Local final : public Expression {
 private:
  Local(ASTList stmts, int8_t condChildNum, bool usesImplicitBlock)
    : Expression(asttags::Local, std::move(stmts)),
      condChildNum_(condChildNum),
      usesImplicitBlock_(usesImplicitBlock) {
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;
  int8_t condChildNum_;
  bool usesImplicitBlock_;

 public:

  /**
    Create and return a local statement containing the passed statements.
  */
  static owned<Local> build(Builder* builder, Location loc,
                            ASTList stmts,
                            bool usesImplicitBlock);

  /**
    Create and return a local statement with the given condition and
    containing the passed statements.
  */
  static owned<Local> build(Builder* builder, Location loc,
                            owned<Expression> condition,
                            ASTList stmts,
                            bool usesImplicitBlock);

  /**
    Return a way to iterate over the statements.
  */
  ASTListIteratorPair<Expression> stmts() const {
    auto begin = condition() ? children_.begin()++ : children_.begin();
    auto end = children_.end();
    return ASTListIteratorPair<Expression>(begin, end);
  }

  /**
    Returns the condition of this local statement, or nullptr if it does
    not exist.
  */
  const Expression* condition() const {
    return condChildNum_ < 0 ? nullptr
      : (const Expression*)child(condChildNum_);
  }

  /**
    Return the number of statements in the local statement.
  */
  int numStmts() const {
    return condition() ? (numChildren()-1) : numChildren();
  }

  /**
    Return the i'th statement in the local statement.
  */
  const Expression* stmt(int i) const {
    int idx = condition() ? (i+1) : i;
    const ASTNode* ast = child(idx);
    assert(ast->isExpression());
    return (const Expression*)ast;
  }

  /**
    Returns true if the first child statement of this local statement is
    preceded by a 'do'.
  */
  bool usesImplicitBlock() const {
    return usesImplicitBlock_;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
