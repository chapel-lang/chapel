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

#ifndef CHPL_UAST_IMPLEMENTS_H
#define CHPL_UAST_IMPLEMENTS_H

#include "chpl/queries/Location.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/NamedDecl.h"
#include "chpl/uast/VarLikeDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents an implements statement.

  \rst
  .. code-block:: chapel

      interface example(T) {
        // TODO: What goes in the body?
      }

  \endrst

  Creates an interface named example with a constraint 'T'.
 */
class Implements final : public AstNode {
 private:
  int8_t typeExprChildNum_;
  bool isConstraint_;

  /* Position of the interface expression when no type name exists. */
  static const int8_t interfaceExprNoTypeChildNum_ = 0;

  /* Compute the position of the interface expression. */
  int8_t interfaceExprChildNum() const {
    auto ret = typeExprChildNum_ == AstNode::NO_CHILD
      ? interfaceExprNoTypeChildNum_
      : typeExprChildNum_ + 1;
    return ret;
  }

  Implements(AstList children, int8_t typeExprChildNum, bool isConstraint)
      : AstNode(asttags::Implements, std::move(children)),
        typeExprChildNum_(typeExprChildNum),
        isConstraint_(isConstraint) {
    if (typeExprChildNum_ != AstNode::NO_CHILD) {
      assert(children_.size() == 2);
      assert(child(typeExprChildNum_)->isIdentifier());
    } else {
      assert(children_.size() == 1);
    }

    assert(interfaceExpr());
    assert(interfaceExpr()->isIdentifier() ||
           interfaceExpr()->isFnCall());
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Implements* lhs = this;
    const Implements* rhs = (const Implements*) other;
    return lhs->typeExprChildNum_ == rhs->typeExprChildNum_ &&
           lhs->isConstraint_ == rhs->isConstraint_;
  }

  void markUniqueStringsInner(Context* context) const override {}

 public:
  ~Implements() override = default;

  /**
    Returns an Identifier naming the type this implements is for.
    May return nullptr.
  */
  const Identifier* typeExpr() const {
    if (typeExprChildNum_ == AstNode::NO_CHILD) return nullptr;
    auto ret = child(typeExprChildNum_)->toIdentifier();
    assert(ret);
    return ret; 
  }

  /**
    Returns true if this implements statement is a constraint.
  */
  bool isConstraint() const {
    return isConstraint_;
  }

  /**
    Returns the name of the interface this implements is implementing.

    This method is a convenience. If 'interfaceExpr()' returns a FnCall,
    this method returns the name of the call. If 'interfaceExpr()' is an
    Identifier, this method returns its name.
  */
  UniqueString interfaceName() const;

  /**
    Returns the interface expression. This method may return either an
    Identifier or a FnCall.
  */
  const AstNode* interfaceExpr() const {
    assert(interfaceExprChildNum() != AstNode::NO_CHILD);
    const AstNode* ret = child(interfaceExprChildNum());
    assert(ret);
    return ret;
  }

  static owned<Implements> build(Builder* builder, Location loc,
                                 owned<Identifier> typeExpr,
                                 owned<AstNode> interfaceExpr,
                                 bool isConstraint);
};


} // end namespace uast
} // end namespace chpl

#endif
