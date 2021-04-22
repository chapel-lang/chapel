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

#ifndef CHPL_AST_DECL_H
#define CHPL_AST_DECL_H

#include "chpl/AST/Expr.h"
#include "chpl/AST/Symbol.h"
#include "chpl/AST/UniqueString.h"

namespace chpl {
namespace ast {

/**
  This is an abstract base class for declarations
 */
class Decl : public Expr {
 protected:
  Decl(asttags::ASTTag tag, owned<Symbol> symbol);
  bool declContentsMatchInner(const Decl* other) const {
    return true;
  }

 public:
  virtual ~Decl() = 0; // this is an abstract base class

  const Symbol* symbol() const {
    const BaseAST* ast = child(0);
    assert(ast->isSymbol());
    return (const Symbol*) ast;
  }
  UniqueString name() const {
    return symbol()->name();
  }
  Symbol::Visibility visibility() const {
    return symbol()->visibility();
  }
};

} // end namespace ast
} // end namespace chpl

#endif
