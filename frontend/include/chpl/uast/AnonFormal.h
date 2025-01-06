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

#ifndef CHPL_UAST_ANON_FORMAL_H
#define CHPL_UAST_ANON_FORMAL_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/Qualifier.h"


namespace chpl {
namespace uast {


/**
  This class represents an anonymous formal. Anonymous formals can
  appear in function types. Their presence indicates that the
  name of a particular formal does not matter for the purposes of
  resolution.

  \rst
  .. code-block:: chapel

    // Both formals of this procedure type are anonymous (no name).
    proc T = proc(int, int): int;

    proc foo(x: int, y: int) { return x + y; }
    proc bar(a: int, b: int) { return a + b; }

    var x: T = nil;

    x = foo; // OK, the names of T's formals are not specified.
    x = bar; // OK, the names of T's formals are not specified.

  \endrst

  Because the formals of 'T' are anonymous, the corresponding formal
  names for concrete functions assigned to 'x' are not significant.
  If the formals were not anonymous, then a type error would be
  issued when one (or either) of the procedures was assigned to 'x'.

  As anonymous formals  do not have a name, they do not inherit from
  NamedDecl (and their API is appropriately more simple).

  They also do not carry an initialization expression.
*/
class AnonFormal final : public AstNode {
 friend class AstNode;

 public:
  using Intent = Formal::Intent;

 private:
  Intent intent_;
  int8_t typeExpressionChildNum_ = NO_CHILD;

  AnonFormal(AstList children, Intent intent,
             int8_t typeExpressionChildNum)
    : AstNode(asttags::AnonFormal, std::move(children)),
      intent_(intent),
      typeExpressionChildNum_(typeExpressionChildNum) {
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(intent_);
    ser.write(typeExpressionChildNum_);
  }

  explicit AnonFormal(Deserializer& des)
    : AstNode(asttags::AnonFormal, des) {
    intent_ = des.read<Formal::Intent>();
    typeExpressionChildNum_ = des.read<int8_t>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const AnonFormal* lhs = this;
    const AnonFormal* rhs = (const AnonFormal*) other;
    return lhs->intent_ == rhs->intent_ &&
           lhs->typeExpressionChildNum_ == rhs->typeExpressionChildNum_;
  }

  void markUniqueStringsInner(Context* context) const override { }

  void dumpInner(const DumpSettings& s) const;

 public:
  ~AnonFormal() override = default;

  static owned<AnonFormal> build(Builder* builder, Location loc,
                                 Intent intent,
                                 owned<AstNode> typeExpression);

  /**
    Returns the intent of the formal, e.g. in `proc(const ref: int)`,
    the first formal has intent `const ref`.
  */
  Intent intent() const { return intent_; }

  Qualifier storageKind() const { return ((Qualifier) intent_); }

  /**
    Returns the type expression of the formal.
  */
  const AstNode* typeExpression() const {
    if (typeExpressionChildNum_ == NO_CHILD) return nullptr;
    auto ret = child(typeExpressionChildNum_);
    return ret;
  }

  static std::string intentToString(Intent intent) {
    return Formal::intentToString(intent);
  }
};


} // end namespace uast


DECLARE_SERDE_ENUM(uast::Formal::Intent, uint8_t);


} // end namespace chpl

#endif
