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

#ifndef CHPL_UAST_FORMAL_H
#define CHPL_UAST_FORMAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Sym.h"

namespace chpl {
namespace uast {


/**
  This class represents a formal. For example, `x` is a formal
  in the below:

  \rst
  .. code-block:: chapel

      proc f( x ) { }
  \endrst

  each Formal has a FormalDecl that refers to the FormalSym. The
  FormalDecls are stored inside of a Function.
 */
class Formal final : public Sym {
 friend class FormalDecl;

 public:
  enum Intent {
    DEFAULT,
    CONST,
    CONST_REF,
    REF,
    IN,
    CONST_IN,
    OUT,
    INOUT,
    PARAM,
    TYPE,
  };

 private:
  Intent intent_;
  int8_t typeExpressionChildNum;
  int8_t initExpressionChildNum;

  Formal(ASTList children,
         UniqueString name,
         Formal::IntentTag intent,
         int8_t typeExpressionChildNum,
         int8_t initExpressionChildNum)
    : Sym(asttags::Variable, std::move(children), name, vis),
      tag_(tag),
      typeExpressionChildNum(typeExpressionChildNum),
      initExpressionChildNum(initExpressionChildNum) {

    assert(-1 <= typeExpressionChildNum && typeExpressionChildNum <= 1);
    assert(-1 <= initExpressionChildNum && initExpressionChildNum <= 1);
    assert(numChildren() <= 2);
    assert(isExpressionASTList(children_));
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~Variable() override = default;
  const Tag tag() const { return this->tag_; }
  /**
    Returns the type expression used in the variable's declaration, or nullptr
    if there wasn't one.
    */
  const Expression* typeExpression() const {
    if (typeExpressionChildNum >= 0) {
      const ASTNode* ast = this->child(typeExpressionChildNum);
      assert(ast->isExpression());
      return (const Expression*)ast;
    } else {
      return nullptr;
    }
  }
  /**
    Returns the init expression used in the variable's declaration, or nullptr
    if there wasn't one.
    */
  const Expression* initExpression() const {
    if (initExpressionChildNum >= 0) {
      const ASTNode* ast = this->child(initExpressionChildNum);
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
