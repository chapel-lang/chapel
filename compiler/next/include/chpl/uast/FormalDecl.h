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

#ifndef CHPL_UAST_FORMALDECL_H
#define CHPL_UAST_FORMALDECL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/SymDecl.h"
#include "chpl/uast/Formal.h"

#include <cassert>

namespace chpl {
namespace uast {


/**
  This class represents a formal declaration..
  
  For example, the Function `f` below has
  a FormalDecl for `x` than refers to a Formal sym.

  \rst
  .. code-block:: chapel

      proc f( x ) { }
  \endrst

 */
class FormalDecl final : public SymDecl {
 private:
  int8_t typeExpressionChildNum_;
  int8_t initExpressionChildNum_;
  FormalDecl(ASTList children,
             int8_t symChildNum,
             int8_t typeExpressionChildNum,
             int8_t initExpressionChildNum)
    : SymDecl(asttags::FormalDecl, std::move(children), symChildNum),
      typeExpressionChildNum_(typeExpressionChildNum),
      initExpressionChildNum_(initExpressionChildNum) {

    assert(0 <= symChildNum && symChildNum <= 2);
    assert(numChildren() <= 3);
    assert(child(symChildNum)->isFormal());
    if (typeExpressionChildNum >= 0) {
      assert(typeExpressionChildNum <= 2);
      assert(child(typeExpressionChildNum)->isExpression());
    }
    if (initExpressionChildNum >= 0) {
      assert(initExpressionChildNum <= 2);
      assert(child(initExpressionChildNum)->isExpression());
    }
  }

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~FormalDecl() override = default;
  static owned<FormalDecl> build(Builder* builder, Location loc,
                                 UniqueString name,
                                 Formal::Intent intent,
                                 owned<Expression> typeExpression,
                                 owned<Expression> initExpression);

  /**
   Returns the formal declared by this FormalDecl.
   */
  const Formal* formal() const {
    const Sym* sym = this->sym();
    assert(sym->isFormal());
    return (Formal*)sym;
  }

  /**
   Returns the intent of the formal, e.g. in `proc f(const ref y: int)`,
   the formal `y` has intent `const ref`.
   */
  const Formal::Intent intent() const {
    return this->formal()->intent();
  }

  /**
   Returns the type expression used in the formal's declaration, or nullptr
   if there wasn't one.

   For example, in `proc f(y: int)`, the formal `y` has type expression `int`.
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
   Returns the init expression used in the formal's declaration, or nullptr
   if there wasn't one.

   For example, in `proc f(z = 3)`, the formal `z` has init expression `3`.
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
