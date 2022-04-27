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

#ifndef CHPL_UAST_CALL_H
#define CHPL_UAST_CALL_H

#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This abstract class represents something call-like.
  It represents a called expression as well as a number of actuals.

  For example `f(1,2)`, `f` is the called expression and `1`, `2`
  are the actuals.
 */
class Call : public AstNode {
 protected:
  bool hasCalledExpression_;
  Call(AstTag tag)
    : AstNode(tag), hasCalledExpression_(false) {
  }
  Call(AstTag tag, AstList children, bool hasCalledExpression)
    : AstNode(tag, std::move(children)),
      hasCalledExpression_(hasCalledExpression) {
  }

  bool callContentsMatchInner(const Call* other) const {
    return true;
  }
  void callMarkUniqueStringsInner(Context* context) const {
  }

 public:
  ~Call() override = 0;

  /**
   Returns an iterable expression over the actuals of a call.
   */
  AstListIteratorPair<AstNode> actuals() const {
    return
      AstListIteratorPair<AstNode>(children_.begin()+hasCalledExpression_,
                                      children_.end());
  }

  // note: the reason for the +/- 1 below is that the
  // 0'th child is the called expression, which does
  // not count as an "actual".

  int numActuals() const {
    return this->numChildren() - hasCalledExpression_;
  }
  const AstNode* actual(int i) const {
    const AstNode* ast = this->child(i+hasCalledExpression_);
    return ast;
  }

  /**
    Returns the called expression, or nullptr if there is not for this call.

    Note that some subclasses of Call will never have a called expression
    and will always return nullptr from this function.
   */
  const AstNode* calledExpression() const {
    if (hasCalledExpression_ == 0) {
      return nullptr;
    } else {
      const AstNode* ast = this->child(0);
      return ast;
    }
  }
};


} // end namespace uast
} // end namespace chpl

#endif
