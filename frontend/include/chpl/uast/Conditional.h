/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/Location.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/AstNode.h"

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
class Conditional final : public AstNode {
 private:
  Conditional(AstList children,
              BlockStyle thenBlockStyle,
              BlockStyle elseBlockStyle,
              bool isExpressionLevel)
      : AstNode(asttags::Conditional, std::move(children)),
        thenBlockStyle_(thenBlockStyle),
        elseBlockStyle_(elseBlockStyle),
        isExpressionLevel_(isExpressionLevel) {

    CHPL_ASSERT(children_[thenBodyChildNum_]->isBlock());

    if (hasElseBlock()) {
      CHPL_ASSERT(0 <= elseBodyChildNum_);
      CHPL_ASSERT(elseBodyChildNum_ <= children_.size());
      CHPL_ASSERT(children_[elseBodyChildNum_]->isBlock());
    } else {
      CHPL_ASSERT(elseBlockStyle_ == BlockStyle::IMPLICIT);
      CHPL_ASSERT(numElseStmts() == 0);
    }

    CHPL_ASSERT(elseBlockStyle_ != BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK);


    if (isExpressionLevel_) {
      CHPL_ASSERT(thenBlockStyle_ == BlockStyle::IMPLICIT);
      CHPL_ASSERT(elseBlockStyle_ == BlockStyle::IMPLICIT);
      CHPL_ASSERT(numThenStmts() == 1);

      // May not have an else block if it's used in a filter expression.
      CHPL_ASSERT(numElseStmts() <= 1);
    }

    CHPL_ASSERT(conditionChildNum_ < (int) children_.size());
    CHPL_ASSERT(thenBodyChildNum_ < (int) children_.size());
  }

  Conditional(Deserializer& des)
    : AstNode(asttags::Conditional, des) {
    thenBlockStyle_ = des.read<BlockStyle>();
    elseBlockStyle_ = des.read<BlockStyle>();
    isExpressionLevel_ = des.read<bool>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Conditional* lhs = this;
    const Conditional* rhs = other->toConditional();

    if (lhs->thenBlockStyle_ != rhs->thenBlockStyle_)
      return false;

    if (lhs->elseBlockStyle_ != rhs->elseBlockStyle_)
      return false;

    if (lhs->isExpressionLevel_ != rhs->isExpressionLevel_)
      return false;

    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  void dumpFieldsInner(const DumpSettings& s) const override;
  std::string dumpChildLabelInner(int i) const override;

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
                                  owned<AstNode> condition,
                                  BlockStyle thenBlockStyle,
                                  owned<Block> thenBlock,
                                  BlockStyle elseBlockStyle,
                                  owned<Block> elseBlock,
                                  bool isExpressionLevel);

  /**
    Create and return a conditional without an else block.
  */
  static owned<Conditional> build(Builder* builder, Location loc,
                                  owned<AstNode> condition,
                                  BlockStyle thenBlockStyle,
                                  owned<Block> thenBlock,
                                  bool isExpressionLevel);

  /**
    Get the condition of this conditional.
  */
  const AstNode* condition() const {
    auto ret = child(conditionChildNum_);
    return ret;
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
  AstListIteratorPair<AstNode> thenStmts() const {
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
  const AstNode* thenStmt(int i) const {
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
    CHPL_ASSERT(ret->isBlock());

    return (const Block*) ret;
  }

  /**
    Iterate over the statements in the else block of this conditional.
  */
  AstListIteratorPair<AstNode> elseStmts() const {
    const Block* elseB = elseBlock();

    if (elseB == nullptr) {
      return AstListIteratorPair<AstNode>(children_.end(),
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
  const AstNode* elseStmt(int i) const {
    const Block* elseB = elseBlock();
    CHPL_ASSERT(elseB != nullptr);

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

  void serialize(Serializer& ser) const override {
    AstNode::serialize(ser);
    ser.write(thenBlockStyle_);
    ser.write(elseBlockStyle_);
    ser.write(isExpressionLevel_);
  }

  DECLARE_STATIC_DESERIALIZE(Conditional);

};


} // end namespace uast
} // end namespace chpl

#endif
