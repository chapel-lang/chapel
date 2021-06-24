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

#ifndef CHPL_UAST_ENUMELEMENT_H
#define CHPL_UAST_ENUMELEMENT_H

#include "chpl/queries/Location.h"
#include "chpl/uast/NamedDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents an element in an enum. For example,
  a, b, c in the below are EnumElements.

  \rst
  .. code-block:: chapel

      enum myEnum { a, b = 2, c }

  \endrst

 */
class EnumElement final : public NamedDecl {
 private:
  EnumElement(ASTList children, UniqueString name)
    : NamedDecl(asttags::EnumElement, std::move(children),
                Decl::DEFAULT_VISIBILITY, name) {

    assert(children_.size() == 0 || children_.size() == 1);
    assert(isExpressionASTList(children_));
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~EnumElement() override = default;
  static owned<EnumElement> build(Builder* builder, Location loc,
                                  UniqueString name,
                                  owned<Expression> initExpression);
  static owned<EnumElement> build(Builder* builder, Location loc,
                                  UniqueString name);

  /**
    Returns the init expression for this EnumElement or nullptr if there was
    none.
   */
  const Expression* initExpression() const {
    if (children_.size() > 0) {
      const ASTNode* ast = this->child(0);
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
