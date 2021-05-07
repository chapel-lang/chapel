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

#ifndef CHPL_UAST_FUNCTIONDECL_H
#define CHPL_UAST_FUNCTIONDECL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Function.h"

#include <cassert>

namespace chpl {
namespace uast {


/**
  This class represents a function declaration
  E.g. here are some examples

  \rst
  .. code-block:: chapel

      proc f(arg) { }

      proc g(x: int = 32) where something() { }
  \endrst

  Each of these is a FunctionDecl that refers to a Function Sym.
 */
class FunctionDecl final : public Decl {
 private:
  FunctionDecl(owned<Function> function)
    : Decl(asttags::FunctionDecl, std::move(function)) {
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  enum FunctionKind {
    PROC,
    ITER,
    OPERATOR,
  };

  ~FunctionDecl() override = default;
  static owned<FunctionDecl> build(Builder* builder, Location loc,
                                   UniqueString name, Sym::Visibility vis,
                                   ASTList formals,
                                   owned<Expression> where,
                                   owned<Expression> lifetime,
                                   Function::FunctionKind kind,
                                   bool throws);
  const Function* function() const {
    const Sym* sym = this->sym();
    assert(sym->isFunction());
    return (Function*)sym;
  }
  // TODO: convenience forwards to Function
};


} // end namespace uast
} // end namespace chpl

#endif
