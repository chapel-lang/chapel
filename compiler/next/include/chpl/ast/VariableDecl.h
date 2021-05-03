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

#ifndef CHPL_AST_VARIABLEDECL_H
#define CHPL_AST_VARIABLEDECL_H

#include "chpl/ast/Decl.h"
#include "chpl/ast/Location.h"
#include "chpl/ast/Variable.h"

#include <cassert>

namespace chpl {
namespace uast {


class Builder;

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
class VariableDecl final : public Decl {
 friend class Builder;

 private:
  VariableDecl(owned<Variable> variable);
  bool contentsMatchInner(const ASTBase* other) const override;

 public:
  ~VariableDecl() override = default;
  static owned<VariableDecl> build(Builder* builder, Location loc,
                                   UniqueString name, Sym::Visibility vis,
                                   Variable::Tag tag,
                                   owned<Exp> typeExp, owned<Exp> initExp);
  const Variable* variable() const {
    const Sym* sym = this->sym();
    assert(sym->isVariable());
    return (Variable*)sym;
  }
  const Variable::Tag tag() const {
    return this->variable()->tag();
  }
  const Exp* typeExp() const {
    return this->variable()->typeExp();
  }
  const Exp* initExp() const {
    return this->variable()->initExp();
  }
};


} // end namespace uast
} // end namespace chpl

#endif
