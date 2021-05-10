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
  ~FunctionDecl() override = default;

  static owned<FunctionDecl> build(Builder* builder, Location loc,
                                   UniqueString name, Sym::Visibility vis,
                                   Function::Linkage linkage,
                                   owned<Expression> linkageNameExpr,
                                   bool inline_,
                                   bool override_,
                                   Function::Kind kind,
                                   owned<FormalDecl> receiver,
                                   Function::ReturnIntent returnIntent,
                                   bool throws,
                                   ASTList formals,
                                   owned<Expression> returnType,
                                   owned<Expression> where,
                                   ASTList lifetime,
                                   ASTList body);

  /**
   Return the Function declared by this FunctionDecl
   */
  const Function* function() const {
    const Sym* sym = this->sym();
    assert(sym->isFunction());
    return (Function*)sym;
  }

  // convenience methods
  Function::Linkage linkage() const {
    return function()->linkage();
  }
  Function::Kind kind() const {
    return function()->kind();
  }
  Function::ReturnIntent returnIntent() const {
    return function()->returnIntent();
  }
  bool isInline() const {
    return function()->isInline();
  }
  bool isOverride() const {
    return function()->isOverride();
  }
  bool throws() const {
    return function()->throws();
  }

  const Expression* linkageNameExpression() const {
    return function()->linkageNameExpression();
  }

  ASTListIteratorPair<FormalDecl> formalDecls() const {
    return function()->formalDecls();
  }
  DeclListSymIteratorPair<Formal> formals() const {
    return function()->formals();
  }
  int numFormals() const {
    return function()->numFormals();
  }
  const FormalDecl* formalDecl(int i) const {
    return function()->formalDecl(i);
  }
  const Formal* formal(int i) const {
    return function()->formal(i);
  }
  const FormalDecl* thisFormalDecl() const {
    return function()->thisFormalDecl();
  }
  const Formal* thisFormal() const {
    return function()->thisFormal();
  }

  const Expression* returnType() const {
    return function()->returnType();
  }
  const Expression* whereClause() const {
    return function()->whereClause();
  }
  ASTListIteratorPair<Expression> lifetimeClauses() const {
    return function()->lifetimeClauses();
  }
  int numLifetimeClauses() const {
    return function()->numLifetimeClauses();
  }
  const Expression* lifetimeClause(int i) const {
    return function()->lifetimeClause(i);
  }
  ASTListIteratorPair<Expression> stmts() const {
    return function()->stmts();
  }
  int numStmts() const {
    return function()->numStmts();
  }
  const Expression* stmt(int i) const {
    return function()->stmt(i);
  }
};


} // end namespace uast
} // end namespace chpl

#endif
