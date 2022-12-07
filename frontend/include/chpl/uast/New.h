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

#ifndef CHPL_UAST_NEW_H
#define CHPL_UAST_NEW_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {

/**
  This class represents a new expression. For example:

  \rst
  .. code-block:: chapel

    var foo = new bar(a = 1, 2);
  \endrst

  The initialization expression of foo is an FnCall where the base expression
  is a New node (representing 'new bar').
*/
class New : public AstNode {
 public:

  /**
    Possible management flavors for a new expression.
  */
  enum Management {
    DEFAULT_MANAGEMENT,
    BORROWED,
    OWNED,
    SHARED,
    UNMANAGED
  };

  /**
    Given a management style, return the Chapel keyword representing it.
  */
  static const char* managementToString(Management management);

  /**
    Given a string, return a management style, or 'DEFAULT_MANAGEMENT' if
    there was not a match.
  */
  static Management stringToManagement(UniqueString ustr);

 private:
  New(AstList children, New::Management management)
    : AstNode(asttags::New, std::move(children)),
      management_(management) {}

  bool contentsMatchInner(const AstNode* other) const override {
    const New* lhs = this;
    const New* rhs = (const New*) other;

    if (lhs->management_ != rhs->management_)
      return false;

    return true;
  }
  void markUniqueStringsInner(Context* context) const override {
  }

  Management management_;

 public:

  /**
    Create and return a new expression with the given type expression and
    management style.
  */
  static owned<New> build(Builder* builder, Location loc,
                          owned<AstNode> typeExpression,
                          Management management);

  /**
    Returns the type expression of this new expression.
  */
  const AstNode* typeExpression() const {
    CHPL_ASSERT(children_.size() > 0);
    return children_[0].get();
  }

  /**
    Returns the management style of this new expression.
  */
  Management management() const {
    return management_;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
