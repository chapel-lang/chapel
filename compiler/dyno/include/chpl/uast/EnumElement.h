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
  EnumElement(AstList children, int attributesChildNum,
              UniqueString name)
    : NamedDecl(asttags::EnumElement, std::move(children),
                attributesChildNum,
                Decl::DEFAULT_VISIBILITY,
                Decl::DEFAULT_LINKAGE,
                /*linkageNameChildNum*/ -1,
                name) {

    assert(children_.size() <= 2);
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const EnumElement* lhs = (const EnumElement*) this;
    const EnumElement* rhs = (const EnumElement*) other;
    return lhs->namedDeclContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    namedDeclMarkUniqueStringsInner(context);
  }

  int initExpressionChildNum() const {
    return this->attributesChildNum() + 1;
  }

 public:
  ~EnumElement() override = default;

  static owned<EnumElement> build(Builder* builder, Location loc,
                                  owned<Attributes> attributes,
                                  UniqueString name,
                                  owned<AstNode> initExpression);

  static owned<EnumElement> build(Builder* builder, Location loc,
                                  owned<Attributes> attributes,
                                  UniqueString name);

  /**
    Returns the init expression for this EnumElement or nullptr if there was
    none.
   */
  const AstNode* initExpression() const {
    if (children_.size() > 0) {
      const AstNode* ast = this->child(initExpressionChildNum());
      return ast;
    } else {
      return nullptr;
    }
  }
};


} // end namespace uast
} // end namespace chpl

#endif
