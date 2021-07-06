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

#ifndef CHPL_UAST_RANGE_H
#define CHPL_UAST_RANGE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents a range expression. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var r = 0..15;

  \endrst

 */
class Range final : public Expression {
 public:
  enum OpKind {
    DEFAULT,
    OPEN_UPPER
  };

 private:
  Range(ASTList children, OpKind opKind,
        int8_t lowerBoundChildNum,
        int8_t upperBoundChildNum)
    : Expression(asttags::Range, std::move(children)),
      opKind_(opKind),
      lowerBoundChildNum_(lowerBoundChildNum),
      upperBoundChildNum_(upperBoundChildNum) {
    // TODO: Add some asserts.
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const Range* rhs = other->toRange();
    return this->opKind_ == rhs->opKind_ &&
      this->lowerBoundChildNum_ == rhs->lowerBoundChildNum_ &&
      this->upperBoundChildNum_ == rhs->upperBoundChildNum_ &&
      this->expressionContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  OpKind opKind_;
  int8_t lowerBoundChildNum_;
  int8_t upperBoundChildNum_;

 public:
  ~Range() override = default;

  /**
    Create and return a range expression.
  */
  static owned<Range> build(Builder* builder, Location loc,
                            OpKind opKind,
                            owned<Expression> lowerBound,
                            owned<Expression> upperBound);

  /**
    Returns the operator kind used to constrct this range.
  */
  OpKind opKind() const {
    return this->opKind_;
  }

  /**
    Returns the lower bound of this range, or nullptr if there is none.
  */
  const Expression* lowerBound() const {
    if (lowerBoundChildNum_ < 0) return nullptr;
    auto ret = child(lowerBoundChildNum_);
    assert(ret->isExpression());
    return (const Expression*)ret;
  }

  /**
    Returns the upper bound of this range, or nullptr if there is none.
  */
  const Expression* upperBound() const {
    if (upperBoundChildNum_ < 0) return nullptr;
    auto ret = child(upperBoundChildNum_);
    assert(ret->isExpression());
    return (const Expression*)ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
