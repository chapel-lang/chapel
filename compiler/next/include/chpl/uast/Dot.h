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

#ifndef CHPL_UAST_DOT_H
#define CHPL_UAST_DOT_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Call.h"

namespace chpl {
namespace uast {

/**
  This class represents a dot expression. A dot expression might be:

   * a method call
   * field access
   * qualified access within a module or enum

  For example, `a.b`, `this.type`, `Module.myFunc` are dot expressions.

  Consider `myObject.myMethod()`, or `x.f(a=3)`. These are method
  calls that also involve Dot expressions. These are represented as
  an FnCall containing a Dot expression. For example, for `x.f(a=3)`,
  it is represented as

    FnCall(calledExpression=`x.f`, actuals=[3], names=[a]);

  where the `x.f` is a Dot expression.

 */
class Dot final : public Expression {
 private:
  // which field
 UniqueString fieldName_;

  Dot(ASTList children, UniqueString fieldName)
    : Expression(asttags::Dot, std::move(children)),
      fieldName_(fieldName) {
    assert(children_.size() == 1);
    assert(children_[0]->isExpression());
  }
  bool contentsMatchInner(const ASTNode* other) const override {
    const Dot* lhs = this;
    const Dot* rhs = (const Dot*) other;

    if (lhs->fieldName_ != rhs->fieldName_)
      return false;

    if (!lhs->expressionContentsMatchInner(rhs))
      return false;

    return true;
  }
  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
    fieldName_.mark(context);
  }

 public:
  ~Dot() override = default;
  static owned<Dot> build(Builder* builder,
                          Location loc,
                          owned<Expression> receiver,
                          UniqueString fieldName);

  /** Returns the left-hand-side of the Dot expression */
  const Expression* receiver() const {
    const ASTNode* ast = child(0);
    assert(ast->isExpression());
    return (const Expression*) ast;
  }
  /** Returns the name of the field or method accessed by the Dot expression */
  UniqueString field() const {
    return fieldName_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
