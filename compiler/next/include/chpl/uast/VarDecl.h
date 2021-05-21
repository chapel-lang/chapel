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

#ifndef CHPL_UAST_VARDECL_H
#define CHPL_UAST_VARDECL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/NamedDecl.h"

namespace chpl {
namespace uast {


/**
  This abstract class represents any sort of variable-like declaration.
  This includes things like fields, formals, or variables.
 */
class VarDecl : public NamedDecl {
 protected:
  int8_t typeExpressionChildNum_;
  int8_t initExpressionChildNum_;

  VarDecl(ASTTag tag, ASTList children, Decl::Visibility vis,
          UniqueString name,
          int8_t typeExpressionChildNum,
          int8_t initExpressionChildNum)
    : NamedDecl(tag, std::move(children), vis, name),
      typeExpressionChildNum_(typeExpressionChildNum),
      initExpressionChildNum_(initExpressionChildNum) {

    assert(numChildren() <= 2);
    if (typeExpressionChildNum >= 0) {
      assert(typeExpressionChildNum <= 2);
      assert(child(typeExpressionChildNum)->isExpression());
    }
    if (initExpressionChildNum >= 0) {
      assert(initExpressionChildNum <= 2);
      assert(child(initExpressionChildNum)->isExpression());
    }
  }

  bool varDeclContentsMatchInner(const ASTNode* other) const {
    const VarDecl* lhs = this;
    const VarDecl* rhs = (const VarDecl*) other;
    return lhs->namedDeclContentsMatchInner(rhs) &&
           lhs->typeExpressionChildNum_ == rhs->typeExpressionChildNum_ &&
           lhs->initExpressionChildNum_ == rhs->initExpressionChildNum_;
  }

  void varDeclMarkUniqueStringsInner(Context* context) const {
    namedDeclMarkUniqueStringsInner(context);
  }

 public:
  ~VarDecl() = 0; // this is an abstract base class

  /**
    Returns the type expression used in this VarDecl's declaration, or
    nullptr if there wasn't one.
  */
  const Expression* typeExpression() const {
    if (typeExpressionChildNum_ >= 0) {
      const ASTNode* ast = this->child(typeExpressionChildNum_);
      assert(ast->isExpression());
      return (const Expression*)ast;
    } else {
      return nullptr;
    }
  }

  /**
    Returns the init expression used in this VarDecl's declaration, or
    nullptr if there wasn't one.
  */
  const Expression* initExpression() const {
    if (initExpressionChildNum_ >= 0) {
      const ASTNode* ast = this->child(initExpressionChildNum_);
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
