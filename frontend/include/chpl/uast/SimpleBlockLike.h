/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This class represents any sort of block-like construct. Candidates for
  use of this abstract class have:

    - Simple-to-no (e.g. Local, or Block) control flow
    - A body that may be enclosed by curly braces
    - A body containing statements that execute serially

  Thus Begin would be a candidate for this class (because while it spawns
  a new task, the contained statements execute serially), while Cobegin
  would not because the contained statements each execute in a different
  task.

  Conditional would not be a candidate for this class because its control
  flow is slightly more complex than "conditionally execute entire block".
  This is because the conditional may have an else block.
 */
class SimpleBlockLike : public AstNode {
 friend class AstNode;

 protected:
  BlockStyle blockStyle_;
  int bodyChildNum_;
  int numBodyStmts_;

  SimpleBlockLike(AstTag tag, AstList children, BlockStyle blockStyle,
                  int bodyChildNum,
                  int numBodyStmts)
    : AstNode(tag, std::move(children)),
      blockStyle_(blockStyle),
      bodyChildNum_(bodyChildNum),
      numBodyStmts_(numBodyStmts) {

  }

  void simpleBlockLikeSerializeInner(Serializer& ser) const {
    ser.write(blockStyle_);
    ser.writeVInt(bodyChildNum_);
    ser.writeVInt(numBodyStmts_);
  }

  SimpleBlockLike(AstTag tag, Deserializer& des)
    : AstNode(tag, des) {
    blockStyle_ = des.read<BlockStyle>();
    bodyChildNum_ = des.readVInt();
    numBodyStmts_ = des.readVInt();
  }

  bool simpleBlockLikeContentsMatchInner(const AstNode* other) const {
    const SimpleBlockLike* lhs = this;
    const SimpleBlockLike* rhs = other->toSimpleBlockLike();

    if (lhs->blockStyle_ != rhs->blockStyle_)
      return false;

    if (lhs->bodyChildNum_ != rhs->bodyChildNum_)
      return false;

    if (lhs->numBodyStmts_ != rhs->numBodyStmts_)
      return false;

    return true;
  }

  void simpleBlockLikeMarkUniqueStringsInner(Context* context) const {
  }

 public:
  virtual ~SimpleBlockLike() override = 0; // this is an abstract base class

  /**
    Return a way to iterate over the statements.
   */
  AstListIteratorPair<AstNode> stmts() const {
    auto begin = children_.begin() + bodyChildNum_;
    auto end = begin + numBodyStmts_;
    return AstListIteratorPair<AstNode>(begin, end);
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
  const AstNode* stmt(int i) const {
    CHPL_ASSERT(i >= 0 && i < numBodyStmts_);
    const AstNode* ast = this->child(i + bodyChildNum_);
    return ast;
  }

  /**
    Get the block style of this.
  */
  BlockStyle blockStyle() const {
    return blockStyle_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
