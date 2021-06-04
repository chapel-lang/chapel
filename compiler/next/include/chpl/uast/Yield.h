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

#ifndef CHPL_UAST_YIELD_H
#define CHPL_UAST_YIELD_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents a yield statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      iter foo(): int {
        for i in range 0..15 do yield i;
      }

  \endrst

 */
class Yield final : public Expression {
 private:
  Yield(ASTList children)
    : Expression(asttags::Yield, std::move(children)) {
    assert(isExpressionASTList(children_));
    assert(children_.size() == 1);
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    return this->expressionContentsMatchInner(other->toExpression());
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  // Value always exists, and its position is always the same.
  static const int8_t valueChildNum_ = 0;

 public:
  ~Yield() override = default;

  /**
    Create and return a yield statement. The value formal cannot be nullptr. 
  */
  static owned<Yield> build(Builder* builder, Location loc,
                            owned<Expression> value);

  /**
    Returns the value of this yield statement.
  */
  const Expression* value() const {
    auto ret = child(valueChildNum_);
    assert(ret && ret->isExpression());
    return (const Expression*) ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
