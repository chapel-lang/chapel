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

#ifndef CHPL_UAST_FORWARDING_H
#define CHPL_UAST_FORWARDING_H

#include "chpl/queries/Location.h"
#include "chpl/uast/VisibilityClause.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Variable.h"

namespace chpl {
namespace uast {


/**
  This class represents a forwarding statement. Forwarding allows a record
  or class to specify that certain method calls will be forwarded to a
  particular expression.

  \rst
  .. code-block:: chapel

    record MyCircle {
      forwarding var impl: MyCircleImpl;
    }

    record MyCircle {
      var impl: MyCircleImpl;
      forwarding impl except area;
    }

  \endrst

  The forwarding statement stores an expression that is either
  a VisibilityClause, a FnCall, or a Variable.

*/

class ForwardingDecl final : public Decl {


private:
  ForwardingDecl(ASTList children, Decl::Visibility visibility,
                 int attributesChildNum)
    : Decl(asttags::ForwardingDecl, std::move(children), attributesChildNum,
                visibility,
                Decl::DEFAULT_LINKAGE,
                /*linkageNameChildNum*/ -1
                ) {

    assert(children_.size() >= 0 && children_.size() <= 2);
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const ForwardingDecl* lhs = (const ForwardingDecl*) this;
    const ForwardingDecl* rhs = (const ForwardingDecl*) other;
    return lhs->declContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    declMarkUniqueStringsInner(context);
  }

  int exprChildNum() const {
    return this->attributesChildNum() + 1;
  }

 public:
  ~ForwardingDecl() override = default;


  static owned<ForwardingDecl> build(Builder* builder, Location loc,
                                     owned<Attributes> attributes,
                                     owned<Expression> expr);

  static owned<ForwardingDecl> build(Builder* builder, Location loc,
                                     owned<Attributes> attributes,
                                     owned<Expression> expr,
                                     Decl::Visibility visibility);

  /**
    Returns the child for this ForwardingDecl or nullptr if there was none.
  */
  const Expression* expr() const {
    if (children_.size() > 0) {
      const ASTNode* ast = this->child(exprChildNum());
      assert(ast->isExpression());
      return (const Expression*)ast;
    } else {
      return nullptr;
    }
  }
};


} // end namespace uast
} // end namespace chpl

#endif
