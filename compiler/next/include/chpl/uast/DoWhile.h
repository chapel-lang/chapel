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

#ifndef CHPL_UAST_DOWHILE_H
#define CHPL_UAST_DOWHILE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Loop.h"

namespace chpl {
namespace uast {


/**
  This class represents a do-while loop. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var i = 0;
      do {
        writeln(i);
        i += 1;
      } while i < 5;

  \endrst

 */
class DoWhile final : public Loop {
 private:
  DoWhile(ASTList children, int loopBodyChildNum, int numLoopBodyStmts,
          int conditionChildNum,
          bool isBodyBlock)
    : Loop(asttags::DoWhile, std::move(children),
           loopBodyChildNum,
           numLoopBodyStmts,
           /*usesDo*/ true),
      conditionChildNum_(conditionChildNum),
      isBodyBlock_(isBodyBlock) {
    assert(isExpressionASTList(children_));
    assert(condition());
  }

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

  int8_t conditionChildNum_;
  bool isBodyBlock_;

 public:
  ~DoWhile() override = default;

  /**
    Create and return a do-while loop. 
  */
  static owned<DoWhile> build(Builder* builder, Location loc,
                              ASTList stmts,
                              owned<Expression> condition,
                              bool isBodyBlock);

  /**
    Return the condition of this do-while loop.
  */
  const Expression* condition() const {
    auto ret = child(conditionChildNum_);
    assert(ret->isExpression());
    return (const Expression*)ret;
  }

  /**
    Returns true if this do-while loop body is a block.
  */
  bool isBodyBlock() const {
    return isBodyBlock_;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
