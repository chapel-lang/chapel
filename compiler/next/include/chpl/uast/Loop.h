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
#include "chpl/uast/ControlFlow.h"

namespace chpl {
namespace uast {


/**
  This abstract class represents some sort of loop.
 */
class Loop: public ControlFlow {
 protected:
  Loop(ASTTag tag, ASTList children, int32_t statementChildNum, bool usesDo);
  bool expressionContentsMatchInner(const Expression* other) const {
    return true;
  }
  void expressionMarkUniqueStringsInner(Context* context) const {}
  int32_t statementChildNum_;
  bool usesDo_;

 public:
  virtual ~Loop() override = 0; // this is an abstract base class

  /**
    Return a way to iterate over the statements of this loop.
   */
  ASTListIteratorPair<Expression> stmts() const {
    auto begin = children_.begin() + statementChildNum_;
    auto end = children_.end();
    return ASTListIteratorPair<Expression>(begin, end);
  }

  /**
   Return the number of statements in the loop.
   */
  int numStmts() const {
    return this->numChildren() - statementChildNum_;
  }

  /**
   Return the i'th statement in the loop.
   */
  const Expression* stmt(int i) const {
    const ASTNode* ast = this->child(i+statementChildNum_);
    assert(ast->isExpression());
    return (const Expression*)ast;
  }

  /**
    Returns true if the first child statement of this loop is preceded
    by a 'do'.
  */
  bool usesDo() const {
    return usesDo_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
