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

#ifndef CHPL_UAST_BRACKETLOOP_H
#define CHPL_UAST_BRACKETLOOP_H

#include "chpl/queries/Location.h"
#include "chpl/uast/IndexableLoop.h"
#include "chpl/uast/WithClause.h"

namespace chpl {
namespace uast {


/**
  This class represents a bracket loop. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      [i in 0..15] writeln(i);

  \endrst

 */
class BracketLoop final : public IndexableLoop {
 private:
  BracketLoop(ASTList children, int8_t indexChildNum,
              int8_t iterandChildNum,
              int8_t withClauseChildNum,
              int loopBodyChildNum,
              int numLoopBodyStmts,
              bool usesImplicitBlock,
              bool isExpressionLevel)
    : IndexableLoop(asttags::BracketLoop, std::move(children),
                    indexChildNum,
                    iterandChildNum,
                    withClauseChildNum,
                    loopBodyChildNum,
                    numLoopBodyStmts,
                    usesImplicitBlock,
                    isExpressionLevel) {
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    return indexableLoopContentsMatchInner(other->toIndexableLoop());
  }

  void markUniqueStringsInner(Context* context) const override {
    indexableLoopMarkUniqueStringsInner(context);
  }

 public:
  ~BracketLoop() override = default;

  /**
    Create and return a bracket loop. 
  */
  static owned<BracketLoop> build(Builder* builder, Location loc,
                                  owned<Decl> index,
                                  owned<Expression> iterand,
                                  owned<WithClause> withClause,
                                  ASTList stmts,
                                  bool usesImplicitBlock,
                                  bool isExpressionLevel);


};


} // end namespace uast
} // end namespace chpl

#endif
