/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_IMPLEMENTS_H
#define CHPL_UAST_IMPLEMENTS_H

#include "chpl/framework/Location.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/NamedDecl.h"
#include "chpl/uast/VarLikeDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents an 'implements' statement or expression.

  For example:

  \rst
  .. code-block:: chapel

      T implements Foo(A, B);

  \endrst

  Is an implements statement which states that type 'T' implements the
  interface 'Foo(A, B)'.
*/
class Implements final : public AstNode {
 private:
  int8_t typeIdentChildNum_;
  bool isExpressionLevel_;

  /* Position of the interface expression when no type name exists. */
  static const int8_t interfaceExprNoTypeChildNum_ = 0;

  /* Compute the position of the interface expression. */
  int8_t interfaceExprChildNum() const {
    auto ret = typeIdentChildNum_ == AstNode::NO_CHILD
      ? interfaceExprNoTypeChildNum_
      : typeIdentChildNum_ + 1;
    return ret;
  }

  Implements(AstList children, int8_t typeIdentChildNum,
             bool isExpressionLevel)
      : AstNode(asttags::Implements, std::move(children)),
        typeIdentChildNum_(typeIdentChildNum),
        isExpressionLevel_(isExpressionLevel) {
    if (typeIdentChildNum_ != AstNode::NO_CHILD) {
      CHPL_ASSERT(children_.size() == 2);
      CHPL_ASSERT(child(typeIdentChildNum_)->isIdentifier());
    } else {
      CHPL_ASSERT(children_.size() == 1);
    }

    CHPL_ASSERT(interfaceExpr());
    CHPL_ASSERT(interfaceExpr()->isIdentifier() ||
           interfaceExpr()->isFnCall());
  }

  Implements(Deserializer& des)
    : AstNode(asttags::Implements, des) {
      typeIdentChildNum_ = des.read<int8_t>();
      isExpressionLevel_ = des.read<bool>();
    }
  bool contentsMatchInner(const AstNode* other) const override {
    const Implements* lhs = this;
    const Implements* rhs = (const Implements*) other;
    return lhs->typeIdentChildNum_ == rhs->typeIdentChildNum_ &&
           lhs->isExpressionLevel_ == rhs->isExpressionLevel_;
  }

  void markUniqueStringsInner(Context* context) const override {}

  void dumpFieldsInner(const DumpSettings& s) const override;
  std::string dumpChildLabelInner(int i) const override;

 public:
  ~Implements() override = default;

  /**
    Returns an Identifier naming the type this implements is for.
    May return nullptr.

    For the following:

    \rst
    .. code-block:: chapel

        T implements Foo(A, B);

    \endrst

    This method returns the identifier storing 'T'.
  */
  const Identifier* typeIdent() const {
    if (typeIdentChildNum_ == AstNode::NO_CHILD) return nullptr;
    auto ret = child(typeIdentChildNum_)->toIdentifier();
    CHPL_ASSERT(ret);
    return ret;
  }

  /**
    Returns true if this implements statement is at an expression level.
  */
  bool isExpressionLevel() const {
    return isExpressionLevel_;
  }

  /**
    Returns the name of the interface this is implementing.

    For the following:

    \rst
    .. code-block:: chapel

        T implements Foo(A, B);
        T implements Bar;

    \endrst

    This method returns 'Foo' and 'Bar', respectively.
  */
  UniqueString interfaceName() const;

  /**
    Returns the interface expression. This method may return either an
    Identifier or a FnCall.

    For the following:

    \rst
    .. code-block:: chapel

        T implements Foo(A, B);
        T implements Bar;

    \endrst

    This method returns the FnCall 'Foo(A, B)' or the Identifier 'Bar',
    respectively.
  */
  const AstNode* interfaceExpr() const {
    CHPL_ASSERT(interfaceExprChildNum() != AstNode::NO_CHILD);
    const AstNode* ret = child(interfaceExprChildNum());
    CHPL_ASSERT(ret);
    return ret;
  }

  static owned<Implements> build(Builder* builder, Location loc,
                                 owned<Identifier> typeIdent,
                                 owned<AstNode> interfaceExpr,
                                 bool isExpressionLevel);

  void serialize(Serializer& ser) const override {
    AstNode::serialize(ser);
    ser.write(typeIdentChildNum_);
    ser.write(isExpressionLevel_);
  }

  DECLARE_STATIC_DESERIALIZE(Implements);

};


} // end namespace uast
} // end namespace chpl

#endif
