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

#ifndef CHPL_UAST_AS_H
#define CHPL_UAST_AS_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"

namespace chpl {
namespace uast {


/**
  This class represents an 'as' expression. As expressions are used within
  use clauses to rename a symbol in the current scope. For example:

  \rst
  .. code-block:: chapel
    // Here an as clause is used to rename 'Foo' to 'X'.
    use Foo as X;

  \endrst
 */
class As final : public Expression {
 private:
  As(ASTList children)
    : Expression(asttags::As, std::move(children)) {
    const Expression* symbol = this->symbol();
    assert(symbol->isIdentifier() || symbol->isDot());
  }

  // No need to match 'symbolChildNum_' or 'renameChildNum_', they are const.
  bool contentsMatchInner(const ASTNode* other) const override {
    return expressionContentsMatchInner(other->toExpression());
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  // These always exist and their position will never change.
  static const int8_t symbolChildNum_ = 0;
  static const int8_t renameChildNum_ = 1;

 public:
  ~As() override = default;

  /**
    Create and return an as expression.
  */
  static owned<As> build(Builder* builder, Location loc,
                         owned<Expression> symbol,
                         owned<Identifier> rename);

  /**
    Return the original name specified by this as expression.
  */
  const Expression* symbol() const {
    auto ret = child(symbolChildNum_);
    assert(ret->isExpression());
    return (const Expression*)ret;
  }

  /**
    Return the rename specified by this as expression.
  */
  const Identifier* rename() const {
    auto ret = child(renameChildNum_);
    assert(ret->isIdentifier());
    return (const Identifier*)ret;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
