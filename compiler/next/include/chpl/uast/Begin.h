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

#ifndef CHPL_UAST_BEGIN_H
#define CHPL_UAST_BEGIN_H

#include "chpl/queries/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/SimpleBlockLike.h"
#include "chpl/uast/WithClause.h"

namespace chpl {
namespace uast {


/**
  This class represents a begin statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var x = 0;
      begin {
        writeln(x);
      }

  \endrst

 */
class Begin final : public SimpleBlockLike {
 private:
  Begin(ASTList children, int8_t withClauseChildNum, BlockStyle blockStyle,
        int bodyChildNum,
        int numBodyStmts)
    : SimpleBlockLike(asttags::Begin, std::move(children), blockStyle,
                      bodyChildNum,
                      numBodyStmts),
      withClauseChildNum_(withClauseChildNum) {
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override;

  void markUniqueStringsInner(Context* context) const override {
    simpleBlockLikeMarkUniqueStringsInner(context);
  }

  int8_t withClauseChildNum_;

 public:

  /**
    Create and return a begin statement. 
  */
  static owned<Begin> build(Builder* builder, Location loc,
                            owned<WithClause> withClause,
                            BlockStyle blockStyle,
                            ASTList stmts);

  /**
    Returns the with clause of this begin statement, or nullptr if it does
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
