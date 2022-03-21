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

#ifndef CHPL_UAST_WHEN_H
#define CHPL_UAST_WHEN_H

#include "chpl/queries/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/SimpleBlockLike.h"

namespace chpl {
namespace uast {


/**
  This class represents a when statement. When statements make up the body
  of the select statement.
 */
class When final : public SimpleBlockLike {
 private:
  When(AstList children, int numCaseExprs, BlockStyle blockStyle,
       int bodyChildNum,
       int numBodyStmts)
    : SimpleBlockLike(asttags::When, std::move(children), blockStyle,
                      bodyChildNum,
                      numBodyStmts),
      numCaseExprs_(numCaseExprs) {
    assert(isExpressionAstList(children_));
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const When* rhs = other->toWhen();
    return this->numCaseExprs_ == rhs->numCaseExprs_ &&
      this->simpleBlockLikeContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    simpleBlockLikeMarkUniqueStringsInner(context);
  }

  // The position of this never changes.
  static const int8_t caseExprChildNum_ = 0;

  int numCaseExprs_;

 public:

  /**
    Create and return a when statement. If 'caseExprs' is empty then
    'isOtherwise()' will evaluate to true.
  */
  static owned<When> build(Builder* builder, Location loc,
                           AstList caseExprs,
                           BlockStyle blockStyle,
                           AstList stmts);

  /**
    Returns the number of case expressions for this when statement.
  */
  int numCaseExprs() const {
    return numCaseExprs_;
  }

  /**
    Returns the i'th case of this when statement.
  */
  const Expression* caseExpr(int i) const {
    if (numCaseExprs_ <= 0) return nullptr;
    assert(i >= 0 && i < numCaseExprs_);
    auto ret = child(i);
    assert(ret->isExpression());
    return (const Expression*)ret;
  }

  /**
    Return a way to iterate over the cases of this when statement.
  */
  AstListIteratorPair<Expression> caseExprs() const {
    auto begin = (numCaseExprs_ >= 0) ? children_.begin() : children_.end();
    auto end = begin + numCaseExprs_;
    return AstListIteratorPair<Expression>(begin, end);
  }

  /**
    Returns true if this when statement uses the otherwise keyword.
  */
  bool isOtherwise() const {
    return numCaseExprs_ == 0;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
