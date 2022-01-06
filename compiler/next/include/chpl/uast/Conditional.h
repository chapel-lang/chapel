/*
 * Copyright 2022 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/Block.h"
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
  Conditional(ASTList children,
              BlockStyle thenBlockStyle,
              BlockStyle elseBlockStyle,
              bool isExpressionLevel)
      : Expression(asttags::Conditional, std::move(children)),
        thenBlockStyle_(thenBlockStyle),
        elseBlockStyle_(elseBlockStyle),
        isExpressionLevel_(isExpressionLevel) {
    assert(isExpressionASTList(children_));

    assert(children_[thenBodyChildNum_]->isBlock());

    if (hasElseBlock()) {
      assert(0 <= elseBodyChildNum_);
      assert(elseBodyChildNum_ <= children_.size());
      assert(children_[elseBodyChildNum_]->isBlock());
    } else {
      assert(elseBlockStyle_ == BlockStyle::IMPLICIT);
      assert(numElseStmts() == 0);
    }

    assert(elseBlockStyle_ != BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK);


    if (isExpressionLevel_) {
      assert(thenBlockStyle_ == BlockStyle::IMPLICIT);
      assert(elseBlockStyle_ == BlockStyle::IMPLICIT);
      assert(numThenStmts() == 1);

      // May not have an else block if it's used in a filter expression.
      assert(numElseStmts() <= 1);
    }

    assert(conditionChildNum_ < (int) children_.size());
    assert(thenBodyChildNum_ < (int) children_.size());
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const Conditional* lhs = this;
    const Conditional* rhs = other->toConditional();

    if (lhs->thenBlockStyle_ != rhs->thenBlockStyle_)
      return false;

    if (lhs->elseBlockStyle_ != rhs->elseBlockStyle_)
      return false;

    if (lhs->isExpressionLevel_ != rhs->isExpressionLevel_)
      return false;

    if (!lhs->expressionContentsMatchInner(rhs))
      return false;

    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  // Condition always exists, and its position is always the same.
  static const int8_t conditionChildNum_ = 0;
  // Ditto then
  static const int8_t thenBodyChildNum_ = 1;
  // Ditto else (if this > children_.size(), there is no else clause)
  static const int8_t elseBodyChildNum_ = 2;

  BlockStyle thenBlockStyle_;
  BlockStyle elseBlockStyle_;
  bool isExpressionLevel_;

 public:
  ~Conditional() override = default;

  /**
    Create and return a conditional. 
  */
  static owned<Conditional> build(Builder* builder, Location loc,
                                  owned<Expression> condition,
                                  BlockStyle thenBlockStyle,
                                  owned<Block> thenBlock,
                                  BlockStyle elseBlockStyle,
                                  owned<Block> elseBlock,
                                  bool isExpressionLevel);

  /**
    Create and return a conditional without an else block.
  */
  static owned<Conditional> build(Builder* builder, Location loc,
                                  owned<Expression> condition,
                                  BlockStyle thenBlockStyle,
                                  owned<Block> thenBlock,
                                  bool isExpressionLevel);

  /**
    Get the condition of this conditional.
  */
  const Expression* condition() const {
    auto ret = child(conditionChildNum_);
    return (const Expression*)ret;
  }

  /**
    Return the Block containing the then statements.
   */
  const Block* thenBlock() const {
    auto ret = child(thenBodyChildNum_);
    return (const Block*) ret;
  }

  /**
    Iterate over the statements in the then block of this conditional.
  */
  ASTListIteratorPair<Expression> thenStmts() const {
    return thenBlock()->stmts();
  }

  /**
    Get the number of statements in the then block of this conditional.
  */
  int numThenStmts() const {
    return thenBlock()->numStmts();
  }

  /**
    Get the i'th statement in the then block of this conditional.
  */
  const Expression* thenStmt(int i) const {
    return thenBlock()->stmt(i);
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
    return 0 <= elseBodyChildNum_ && elseBodyChildNum_ < children_.size();
  }

  /**
    Return the Block containing the else statements, or
    nullptr if this Conditional has no else.
   */
  const Block* elseBlock() const {
    if (!hasElseBlock())
      return nullptr;

    auto ret = child(elseBodyChildNum_);
    assert(ret->isBlock());

    return (const Block*) ret;
  }

  /**
    Iterate over the statements in the else block of this conditional.
  */
  ASTListIteratorPair<Expression> elseStmts() const {
    const Block* elseB = elseBlock();

    if (elseB == nullptr) {
      return ASTListIteratorPair<Expression>(children_.end(),
                                             children_.end());
    }

    return elseB->stmts();
  }

  /**
    Get the number of statements in the else block of this conditional.
    If there is no else block, returns 0.
  */
  int numElseStmts() const {
    const Block* elseB = elseBlock();

    if (elseB == nullptr) {
      return 0;
    }

    return elseB->numStmts();
  }

  /**
    Get the i'th statement in the else block of this conditional.
    It is an error to call this function if there is no else block.
  */
  const Expression* elseStmt(int i) const {
    const Block* elseB = elseBlock();
    assert(elseB != nullptr);

    return elseB->stmt(i);
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
