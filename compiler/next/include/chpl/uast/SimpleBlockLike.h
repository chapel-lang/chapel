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

#ifndef CHPL_UAST_SIMPLEBLOCKLIKE_H
#define CHPL_UAST_SIMPLEBLOCKLIKE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents any sort of block-like construct.
 */
class SimpleBlockLike : public Expression {
 protected:
  SimpleBlockLike(ASTTag tag, ASTList children, BlockStyle blockStyle,
                  int bodyChildNum,
                  int numBodyStmts)
    : Expression(tag, std::move(children)),
      blockStyle_(blockStyle),
      bodyChildNum_(bodyChildNum),
      numBodyStmts_(numBodyStmts) {

    assert(isExpressionASTList(children_));
  }

  bool simpleBlockLikeContentsMatchInner(const ASTNode* other) const;

  void simpleBlockLikeMarkUniqueStringsInner(Context* context) const {
    expressionMarkUniqueStringsInner(context);
  }

  BlockStyle blockStyle_;
  int bodyChildNum_;
  int numBodyStmts_;

 public:
  virtual ~SimpleBlockLike() override = 0; // this is an abstract base class

  /**
    Return a way to iterate over the statements.
   */
  ASTListIteratorPair<Expression> stmts() const {
    auto begin = children_.begin() + bodyChildNum_;
    auto end = begin + numBodyStmts_ - 1;
    return ASTListIteratorPair<Expression>(begin, end);
  }

  /**
    Return the number of statements in this.
  */
  int numStmts() const {
    return this->numBodyStmts_;
  }

  /**
    Return the i'th statement in this.
  */
  const Expression* stmt(int i) const {
    assert(i >= 0 && i < numBodyStmts_);
    const ASTNode* ast = this->child(i + bodyChildNum_);
    assert(ast->isExpression());
    return (const Expression*)ast;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
