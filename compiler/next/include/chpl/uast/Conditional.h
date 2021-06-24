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

#ifndef CHPL_UAST_CONDITIONAL_H
#define CHPL_UAST_CONDITIONAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents a conditional. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      const flag = false;
      if flag then writeln('a'); else writeln('b');

  \endrst

 */
class Conditional final : public Expression {
 private:
  Conditional(ASTList children, BlockStyle thenBlockStyle,
              int8_t thenBodyChildNum,
              int numThenBodyStmts,
              BlockStyle elseBlockStyle,
              int elseBodyChildNum,
              int numElseBodyStmts,
              bool isExpressionLevel)
      : Expression(asttags::Conditional, std::move(children)),
        thenBlockStyle_(thenBlockStyle),
        thenBodyChildNum_(thenBodyChildNum),
        numThenBodyStmts_(numThenBodyStmts),
        elseBlockStyle_(elseBlockStyle),
        elseBodyChildNum_(elseBodyChildNum),
        numElseBodyStmts_(numElseBodyStmts),
        isExpressionLevel_(isExpressionLevel) {
    assert(isExpressionASTList(children_));

    if (!hasElseBlock()) {
      assert(numElseBodyStmts_ == 0);
      assert(elseBodyChildNum_ < 0);
      assert(elseBlockStyle_ == BlockStyle::IMPLICIT);
    }

    assert(elseBlockStyle_ != BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK);

    if (isExpressionLevel_) {
      assert(hasElseBlock());
      assert(thenBlockStyle_ == BlockStyle::IMPLICIT);
      assert(elseBlockStyle_ == BlockStyle::IMPLICIT);
      assert(numThenBodyStmts == 1);
      assert(numElseBodyStmts == 1);
    } else {
      assert(numThenBodyStmts_ >= 1);
      assert(numElseBodyStmts_ >= 0);
    }

    auto numKnownChildren = 1 + numThenBodyStmts + numElseBodyStmts;
    assert(((size_t)numKnownChildren) <= children_.size());
  }

  bool contentsMatchInner(const ASTNode* other) const override;

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  // Condition always exists, and its position is always the same.
  static const int8_t conditionChildNum_ = 0;

  BlockStyle thenBlockStyle_;
  int8_t thenBodyChildNum_;
  int numThenBodyStmts_;
  BlockStyle elseBlockStyle_;
  int elseBodyChildNum_;
  int numElseBodyStmts_;
  bool isExpressionLevel_;

 public:
  ~Conditional() override = default;

  /**
    Create and return a conditional. 
  */
  static owned<Conditional> build(Builder* builder, Location loc,
                                  owned<Expression> condition,
                                  BlockStyle thenBlockStyle,
                                  ASTList thenStmts,
                                  BlockStyle elseBlockStyle,
                                  ASTList elseStmts,
                                  bool isExpressionLevel);

  /**
    Create and return a conditional without an else block.
  */
  static owned<Conditional> build(Builder* builder, Location loc,
                                  owned<Expression> condition,
                                  BlockStyle thenBlockStyle,
                                  ASTList thenStmts);

  /**
    Get the condition of this conditional.
  */
  const Expression* condition() const {
    auto ret = child(conditionChildNum_);
    return (const Expression*)ret;
  }

  /**
    Iterate over the statements in the then block of this conditional.
  */
  ASTListIteratorPair<Expression> thenStmts() const {
    auto begin = children_.begin() + thenBodyChildNum_;
    auto end = begin + numThenBodyStmts_;
    return ASTListIteratorPair<Expression>(begin, end);
  }

  /**
    Get the number of statements in the then block of this conditional.
  */
  int numThenStmts() const {
    return numThenBodyStmts_;
  }

  /**
    Get the i'th statement in the then block of this conditional.
  */
  const Expression* thenStmt(int i) const {
    assert(i >= 0 && i < numThenBodyStmts_);
    auto ret = this->child(i+thenBodyChildNum_);
    assert(ret->isExpression());
    return (const Expression*)ret;
  }

  /**
    Returns the block style of the then block of this conditional.
  */
  BlockStyle thenBlockStyle() const {
    return thenBlockStyle_;
  }

  /**
    Returns true if this conditional has an else block.
  */
  bool hasElseBlock() const {
    return elseBodyChildNum_ >= 0;
  }

  /**
    Iterate over the statements in the else block of this conditional.
  */
  ASTListIteratorPair<Expression> elseStmts() const {
    auto begin = hasElseBlock() ? children_.begin() + elseBodyChildNum_
                                : children_.end();
    auto end = begin + numElseBodyStmts_;
    return ASTListIteratorPair<Expression>(begin, end);
  }

  /**
    Get the number of statements in the else block of this conditional.
  */
  int numElseStmts() const {
    return numElseBodyStmts_;
  }

  /**
    Get the i'th statement in the else block of this conditional.
  */
  const Expression* elseStmt(int i) const {
    assert(i >= 0 && i < numElseBodyStmts_);
    auto ret = this->child(i+elseBodyChildNum_);
    assert(ret->isExpression());
    return (const Expression*)ret;
  }

  /**
    Returns the block style of the else block of this conditional.
  */
  BlockStyle elseBlockStyle() const {
    return elseBlockStyle_;
  }

  /**
    Return true if this conditional is at an expression level.
  */
  bool isExpressionLevel() const {
    return isExpressionLevel_;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
