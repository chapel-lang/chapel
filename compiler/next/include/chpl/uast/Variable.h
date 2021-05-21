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

#ifndef CHPL_UAST_VARIABLE_H
#define CHPL_UAST_VARIABLE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/NamedDecl.h"

namespace chpl {
namespace uast {


/**
  This class represents a variable. For example:

  \rst
  .. code-block:: chapel

      var a = 1;
      ref b = a;
      const c = 2;
      const ref d = c;
      param e = "hi";
  \endrst

  each of these is a VariableDecl that refers to a Variable Sym.
 */
class Variable final : public NamedDecl {
 public:
  enum Kind {
    VAR,
    CONST,
    CONST_REF,
    REF,
    PARAM,
    TYPE,
    INDEX
  };

 private:
  Kind kind_;
  int8_t typeExpressionChildNum_;
  int8_t initExpressionChildNum_;

  Variable(ASTList children, Decl::Visibility vis, UniqueString name,
           Variable::Kind kind,
           int8_t typeExpressionChildNum, int8_t initExpressionChildNum)
    : NamedDecl(asttags::Variable, std::move(children), vis, name),
      kind_(kind),
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

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~Variable() override = default;

  static owned<Variable> build(Builder* builder, Location loc,
                               UniqueString name, Decl::Visibility vis,
                               Variable::Kind kind,
                               owned<Expression> typeExpression,
                               owned<Expression> initExpression);

  /**
    Returns the kind of the variable (`var` / `const` / `param` etc).
   */
  const Kind kind() const { return this->kind_; }

  /**
    Returns the type expression used in the variable's declaration, or nullptr
    if there wasn't one.
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
    Returns the init expression used in the variable's declaration, or nullptr
    if there wasn't one.
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
