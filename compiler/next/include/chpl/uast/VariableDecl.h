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

#ifndef CHPL_UAST_VARIABLEDECL_H
#define CHPL_UAST_VARIABLEDECL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/SymDecl.h"
#include "chpl/uast/Variable.h"

namespace chpl {
namespace uast {


/**
  This class represents a variable declaration
  E.g. here are some examples

  \rst
  .. code-block:: chapel

      var a = 1;
      ref b = a;
      const c = 2;
      const ref d = c;
      param e = "hi";
  \endrst

  Each of these is a VariableDecl that refers to a Variable Sym.
 */
class VariableDecl final : public SymDecl {
 private:
  int8_t typeExpressionChildNum_;
  int8_t initExpressionChildNum_;

  VariableDecl(ASTList children,
               int8_t symChildNum,
               int8_t typeExpressionChildNum,
               int8_t initExpressionChildNum)
    : SymDecl(asttags::VariableDecl, std::move(children), symChildNum),
      typeExpressionChildNum_(typeExpressionChildNum),
      initExpressionChildNum_(initExpressionChildNum) {

    assert(0 <= symChildNum && symChildNum <= 2);
    assert(numChildren() <= 3);
    assert(child(symChildNum)->isVariable());
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
  ~VariableDecl() override = default;
  static owned<VariableDecl> build(Builder* builder, Location loc,
                                   UniqueString name, Sym::Visibility vis,
                                   Variable::Kind kind,
                                   owned<Expression> typeExpression,
                                   owned<Expression> initExpression);

  /**
   Returns the Variable declared by this VariableDecl
   */
  const Variable* variable() const {
    const Sym* sym = this->sym();
    assert(sym->isVariable());
    return (Variable*)sym;
  }

  /**
    Returns the kind of the variable (`var` / `const` / `param` etc).
   */
  const Variable::Kind kind() const {
    return this->variable()->kind();
  }

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
