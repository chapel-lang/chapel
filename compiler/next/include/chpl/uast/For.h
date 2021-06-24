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

#ifndef CHPL_UAST_FOR_H
#define CHPL_UAST_FOR_H

#include "chpl/queries/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/IndexableLoop.h"

namespace chpl {
namespace uast {


/**
  This class represents a for loop. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      for i in myRange {
        var x;
      }

  \endrst

 */
class For final : public IndexableLoop {
 private:
  For(ASTList children,  int8_t indexChildNum,
      int8_t iterandChildNum,
      BlockStyle blockStyle,
      int loopBodyChildNum,
      int numLoopBodyStmts,
      bool isExpressionLevel,
      bool isParam)
    : IndexableLoop(asttags::For, std::move(children),
                    indexChildNum,
                    iterandChildNum,
                    /*withClauseChildNum*/ -1,
                    blockStyle,
                    loopBodyChildNum,
                    numLoopBodyStmts,
                    isExpressionLevel),
      isParam_(isParam) {

    assert(isExpressionASTList(children_));
    assert(withClause() == nullptr);
  }

  bool contentsMatchInner(const ASTNode* other) const override;

  void markUniqueStringsInner(Context* context) const override {
    indexableLoopMarkUniqueStringsInner(context);
  }

  bool isParam_;

 public:
  ~For() override = default;

  /**
    Create and return a for loop. 
  */
  static owned<For> build(Builder* builder, Location loc,
                          owned<Decl> index,
                          owned<Expression> iterand,
                          BlockStyle blockStyle,
                          ASTList stmts,
                          bool isExpressionLevel,
                          bool isParam);

  /**
    Returns true if this for loop is param.
  */
  bool isParam() const {
    return isParam_;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
