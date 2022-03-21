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

#ifndef CHPL_UAST_VARLIKEDECL_H
#define CHPL_UAST_VARLIKEDECL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/NamedDecl.h"
#include "chpl/uast/IntentList.h"

namespace chpl {
namespace uast {


/**
  This abstract class represents any sort of variable-like declaration.
  This includes things like fields, formals, or variables.
 */
class VarLikeDecl : public NamedDecl {
 protected:
  IntentList storageKind_;
  int8_t typeExpressionChildNum_;
  int8_t initExpressionChildNum_;

  VarLikeDecl(ASTTag tag, ASTList children, int attributesChildNum,
              Decl::Visibility vis,
              Decl::Linkage linkage,
              int linkageNameChildNum,
              UniqueString name,
              IntentList storageKind,
              int8_t typeExpressionChildNum,
              int8_t initExpressionChildNum)
    : NamedDecl(tag, std::move(children), attributesChildNum, vis,
                linkage,
                linkageNameChildNum,
                name),
      storageKind_(storageKind),
      typeExpressionChildNum_(typeExpressionChildNum),
      initExpressionChildNum_(initExpressionChildNum) {

    // Linkage name can be a child num.
    assert(numChildren() <= 3);
    if (typeExpressionChildNum >= 0) {
      assert(typeExpressionChildNum <= 3);
      assert(child(typeExpressionChildNum)->isExpression());
    }
    if (initExpressionChildNum >= 0) {
      assert(initExpressionChildNum <= 3);
      assert(child(initExpressionChildNum)->isExpression());
    }
  }

  bool varLikeDeclContentsMatchInner(const AstNode* other) const {
    const VarLikeDecl* lhs = this;
    const VarLikeDecl* rhs = (const VarLikeDecl*) other;
    return lhs->namedDeclContentsMatchInner(rhs) &&
           lhs->storageKind_ == rhs->storageKind_ &&
           lhs->typeExpressionChildNum_ == rhs->typeExpressionChildNum_ &&
           lhs->initExpressionChildNum_ == rhs->initExpressionChildNum_;
  }

  void varLikeDeclMarkUniqueStringsInner(Context* context) const {
    namedDeclMarkUniqueStringsInner(context);
  }

 public:
  ~VarLikeDecl() = 0; // this is an abstract base class

  /**
    Returns the storage kind (e.g. type, param, in, etc).
    Subclasses have more specific functions that return the
    subset appropriate for that subclass.
   */
  IntentList storageKind() const {
    return storageKind_;
  }

  /**
    Returns the type expression used in this VarLikeDecl's declaration, or
    nullptr if there wasn't one.
  */
  const Expression* typeExpression() const {
    if (typeExpressionChildNum_ >= 0) {
      const AstNode* ast = this->child(typeExpressionChildNum_);
      assert(ast->isExpression());
      return (const Expression*)ast;
    } else {
      return nullptr;
    }
  }

  /**
    Returns the init expression used in this VarLikeDecl's declaration, or
    nullptr if there wasn't one.
  */
  const Expression* initExpression() const {
    if (initExpressionChildNum_ >= 0) {
      const AstNode* ast = this->child(initExpressionChildNum_);
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
