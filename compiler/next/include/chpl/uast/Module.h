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

#ifndef CHPL_UAST_MODULE_H
#define CHPL_UAST_MODULE_H

#include "chpl/uast/Sym.h"
#include "chpl/queries/Location.h"

namespace chpl {
namespace uast {


/**
  This class represents a module. For example:

  \rst
  .. code-block:: chapel

      module M { }
  \endrst

  contains a ModuleDecl that refers to a Module Sym.
 */
class Module final : public Sym {
 friend class ModuleDecl;

 public:
  enum Tag {
    DEFAULT,
    PROTOTYPE,
    IMPLICIT,
  };

 private:
  Tag tag_;

  Module(ASTList children, UniqueString name,
         Sym::Visibility vis, Module::Tag tag)
    : Sym(asttags::Module, std::move(children), name, vis), tag_(tag) {

    assert(isExpressionASTList(children_));
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~Module() override = default;
  const Tag tag() const { return this->tag_; }

  ASTListIteratorPair<Expression> stmts() const {
    return ASTListIteratorPair<Expression>(children_.begin(), children_.end());
  }

  int numStmts() const {
    return this->numChildren();
  }
  const Expression* stmt(int i) const {
    const ASTNode* ast = this->child(i);
    assert(ast->isExpression());
    return (Expression*) ast;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
