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

#ifndef CHPL_UAST_LOOP_H
#define CHPL_UAST_LOOP_H

#include "chpl/uast/ASTNode.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This abstract class represents some sort of loop.
 */
class Loop: public Expression {
 protected:
  Loop(asttags::ASTTag tag, ASTList children, BlockStyle blockStyle,
       int loopBodyChildNum,
       int numLoopBodyStmts)
    : Expression(tag, std::move(children)),
      blockStyle_(blockStyle),
      loopBodyChildNum_(loopBodyChildNum),
      numLoopBodyStmts_(numLoopBodyStmts) {

    assert(loopBodyChildNum_ >= 0 && numLoopBodyStmts_ >= 0);
    assert((loopBodyChildNum_ + numLoopBodyStmts_) <= this->numChildren());
  }

  bool loopContentsMatchInner(const Loop* other) const;

  void loopMarkUniqueStringsInner(Context* context) const {
    expressionMarkUniqueStringsInner(context);
  }

  BlockStyle blockStyle_;
  int loopBodyChildNum_;
  int numLoopBodyStmts_;

 public:
  virtual ~Loop() override = 0; // this is an abstract base class

  /**
    Return a way to iterate over the statements of this loop.
   */
  ASTListIteratorPair<Expression> stmts() const {
    auto begin = children_.begin() + loopBodyChildNum_;
    auto end = children_.end();
    return ASTListIteratorPair<Expression>(begin, end);
  }

  /**
   Return the number of statements in the loop.
   */
  int numStmts() const {
    return numLoopBodyStmts_;
  }

  /**
   Return the i'th statement in the loop.
   */
  const Expression* stmt(int i) const {
    assert(i >= 0 && i < numLoopBodyStmts_);
    const ASTNode* ast = this->child(i+loopBodyChildNum_);
    assert(ast->isExpression());
    return (const Expression*)ast;
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
