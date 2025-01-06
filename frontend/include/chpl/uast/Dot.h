/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/Location.h"
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
class Dot final : public AstNode {
 friend class AstNode;

 private:
  // which field
  UniqueString fieldName_;

  Dot(AstList children, UniqueString fieldName)
    : AstNode(asttags::Dot, std::move(children)),
      fieldName_(fieldName) {
    CHPL_ASSERT(children_.size() == 1);
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(fieldName_);
  }

  explicit Dot(Deserializer& des)
    : AstNode(asttags::Dot, des) {
    fieldName_ = des.read<UniqueString>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Dot* lhs = this;
    const Dot* rhs = (const Dot*) other;

    if (lhs->fieldName_ != rhs->fieldName_)
      return false;

    return true;
  }
  void markUniqueStringsInner(Context* context) const override {
    fieldName_.mark(context);
  }

  void dumpFieldsInner(const DumpSettings& s) const override;

 public:
  ~Dot() override = default;
  static owned<Dot> build(Builder* builder,
                          Location loc,
                          owned<AstNode> receiver,
                          UniqueString fieldName);

  /** Returns the left-hand-side of the Dot expression */
  const AstNode* receiver() const {
    const AstNode* ast = child(0);
    return ast;
  }
  /** Returns the name of the field or method accessed by the Dot expression */
  UniqueString field() const {
    return fieldName_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
