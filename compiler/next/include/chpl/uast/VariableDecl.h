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
  VariableDecl(owned<Variable> variable)
    : SymDecl(asttags::VariableDecl, std::move(variable)) {
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
  const Variable* variable() const {
    const Sym* sym = this->sym();
    assert(sym->isVariable());
    return (Variable*)sym;
  }
  const Variable::Kind kind() const {
    return this->variable()->kind();
  }
  const Expression* typeExpression() const {
    return this->variable()->typeExpression();
  }
  const Expression* initExpression() const {
    return this->variable()->initExpression();
  }
};


} // end namespace uast
} // end namespace chpl

#endif
