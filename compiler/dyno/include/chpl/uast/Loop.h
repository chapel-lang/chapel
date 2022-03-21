/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_LOOP_H
#define CHPL_UAST_LOOP_H

#include "chpl/uast/AstNode.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This abstract class represents some sort of loop.
 */
class Loop: public Expression {
 protected:
  Loop(asttags::AstTag tag, AstList children, BlockStyle blockStyle,
       int loopBodyChildNum)
    : Expression(tag, std::move(children)),
      blockStyle_(blockStyle),
      loopBodyChildNum_(loopBodyChildNum) {

    assert(0 <= loopBodyChildNum_ &&
           loopBodyChildNum_ < (int) children_.size());
    assert(children_[loopBodyChildNum_]->isBlock());
  }

  bool loopContentsMatchInner(const Loop* other) const {
    const Loop* lhs = this;
    const Loop* rhs = other;

    if (lhs->loopBodyChildNum_ != rhs->loopBodyChildNum_)
      return false;

    if (lhs->blockStyle_ != rhs->blockStyle_)
      return false;

    if (!lhs->expressionContentsMatchInner(rhs))
      return false;

    return true;
  }

  void loopMarkUniqueStringsInner(Context* context) const {
    expressionMarkUniqueStringsInner(context);
  }

  BlockStyle blockStyle_;
  int loopBodyChildNum_;

 public:
  virtual ~Loop() override = 0; // this is an abstract base class

  /**
    Returns the block containing the body of this loop.
  */
  const Block* body() const {
    auto ret = child(loopBodyChildNum_);
    return (const Block*) ret;
  }

  /**
    Return a way to iterate over the statements of this loop.
   */
  AstListIteratorPair<Expression> stmts() const {
    return body()->stmts();
  }

  /**
   Return the number of statements in the loop.
   */
  int numStmts() const {
    return body()->numStmts();
  }

  /**
   Return the i'th statement in the loop.
   */
  const Expression* stmt(int i) const {
    return body()->stmt(i);
  }

  /**
    Returns the block style of the current loop.
  */
  BlockStyle blockStyle() const {
    return blockStyle_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
