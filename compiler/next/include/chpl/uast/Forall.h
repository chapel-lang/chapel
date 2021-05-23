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

#ifndef CHPL_UAST_FORALL_H
#define CHPL_UAST_FORALL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/IndexableLoop.h"
#include "chpl/uast/WithClause.h"

namespace chpl {
namespace uast {


/**
  This class represents a forall loop. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var x: atomic int;
      forall i in myRange {
        x.fetchAdd(i);
      }

  \endrst

 */
class Forall final : public IndexableLoop {
 private:
  // Compute the start of the loop body.
  int8_t computeLoopBodyChildNum(int8_t indexVariableChildNum,
                                 int8_t iterandChildNum,
                                 int8_t withClauseChildNum) {
    return (indexVariableChildNum >= 0) + (iterandChildNum >= 0) +
           (withClauseChildNum >= 0);
  }

  Forall(ASTList children, int8_t indexVariableChildNum,
         int8_t iterandChildNum,
         int8_t withClauseChildNum,
         bool usesDo,
         bool isExpressionLevel)
    : IndexableLoop(asttags::Forall, std::move(children),
                    indexVariableChildNum,
                    iterandChildNum,
                    computeLoopBodyChildNum(indexVariableChildNum,
                                            iterandChildNum,
                                            withClauseChildNum),
                    usesDo),
      withClauseChildNum_(withClauseChildNum),
      isExpressionLevel_(isExpressionLevel) {
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

  int8_t withClauseChildNum_;
  bool isExpressionLevel_;

 public:
  ~Forall() override = default;

  /**
    Create and return a forall loop. 
  */
  static owned<Forall> build(Builder* builder, Location loc,
                             owned<Decl> indexVariable,
                             owned<Expression> iterand,
                             owned<WithClause> withClause,
                             ASTList stmts,
                             bool usesDo,
                             bool isExpressionLevel);

  /**
    Create and return a forall loop. 
  */
  static owned<Forall> build(Builder* builder, Location loc,
                             owned<Decl> indexVariable,
                             owned<Expression> iterand,
                             ASTList stmts,
                             bool usesDo,
                             bool isExpressionLevel);

  /**
    Create and return a forall loop. 
  */
  static owned<Forall> build(Builder* builder, Location loc,
                             owned<Expression> iterand,
                             owned<WithClause> withClause,
                             ASTList stmts,
                             bool usesDo);

  /**
    Create and return a forall loop.
  */
  static owned<Forall> build(Builder* builder, Location loc,
                             owned<Expression> iterand,
                             ASTList stmts,
                             bool usesDo);

  /**
    Returns true if this forall loop appears at the expression level. 
  */
  bool isExpressionLevel() const {
    return isExpressionLevel_;
  }

  /**
    Returns the with clause of this forall loop, or nullptr if it does
    not exist.
  */
  const WithClause* withClause() const {
    if (withClauseChildNum_ < 0) return nullptr;
    auto ret = child(withClauseChildNum_);
    assert(ret->isWithClause());
    return (const WithClause*)ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
