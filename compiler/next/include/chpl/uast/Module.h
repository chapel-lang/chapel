/*
 * Copyright 2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/NamedDecl.h"
#include "chpl/queries/Location.h"

namespace chpl {
namespace uast {


/**
  This class represents a module declaration. For example:

  \rst
  .. code-block:: chapel

      module M { }
  \endrst

  is a declaration for a module named M.
 */
class Module final : public NamedDecl {
 public:
  enum Kind {
    DEFAULT_MODULE_KIND,
    PROTOTYPE,
    IMPLICIT,
  };

 private:
  Kind kind_;

  Module(ASTList children, int attributesChildNum, Decl::Visibility vis,
         UniqueString name,
         Kind kind)
    : NamedDecl(asttags::Module, std::move(children), attributesChildNum,
                vis,
                Decl::DEFAULT_LINKAGE,
                /*linkageNameChildNum*/ -1,
                name),
                kind_(kind) {

    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    const Module* lhs = this;
    const Module* rhs = (const Module*) other;
    return lhs->namedDeclContentsMatchInner(rhs) &&
           lhs->kind_ == rhs->kind_;

  }
  void markUniqueStringsInner(Context* context) const override {
    namedDeclMarkUniqueStringsInner(context);
  }

  int stmtChildNum() const {
    return attributes() ? 1 : 0;
  }

 public:
  ~Module() override = default;

  static owned<Module> build(Builder* builder, Location loc,
                             owned<Attributes> attributes,
                             Decl::Visibility vis,
                             UniqueString name,
                             Module::Kind kind,
                             ASTList stmts);

  /**
    Return the kind of this module (e.g. 'PROTOTYPE' or 'IMPLICIT');
  */
  Kind kind() const { return this->kind_; }

  /**
    Iterate over the statements in this module.
  */
  ASTListIteratorPair<Expression> stmts() const {
    auto begin = numStmts()
        ? children_.begin() + stmtChildNum()
        : children_.end();
    auto end = begin + numStmts();
    return ASTListIteratorPair<Expression>(begin, end);
  }

  /**
    Return the number of statements in this module.
  */
  int numStmts() const {
    return attributes() ? numChildren()-1 : numChildren();
  }

  /**
    Get the i'th statement in this module.
  */
  const Expression* stmt(int i) const {
    assert(0 <= i && i < numStmts());
    const ASTNode* ast = this->child(i + stmtChildNum());
    assert(ast->isExpression());
    return (Expression*) ast;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
