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

#ifndef CHPL_UAST_RETURN_H
#define CHPL_UAST_RETURN_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents a return statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      proc foo(): int {
        return 0;
      }

  \endrst

 */
class Return final : public Expression {
 private:
  Return(ASTList children,  int8_t valueChildNum)
    : Expression(asttags::Return, std::move(children)),
      valueChildNum_(valueChildNum) {
    assert(isExpressionASTList(children_));
    assert(valueChildNum_ <= 0);
  }

  bool contentsMatchInner(const ASTNode* other) const override;

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  int8_t valueChildNum_;

 public:
  ~Return() override = default;

  /**
    Create and return a return statement. If value is nullptr, then there
    is no return value.
  */
  static owned<Return> build(Builder* builder, Location loc,
                             owned<Expression> value);

  /**
    Returns the value of this return statement, or nullptr if it does
    not exist.
  */
  const Expression* value() const {
    if (valueChildNum_ < 0) return nullptr;
    auto ret = child(valueChildNum_);
    assert(ret->isExpression());
    return (const Expression*) ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
