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

#ifndef CHPL_UAST_MULTIVARDECLARATION_H
#define CHPL_UAST_MULTIVARDECLARATION_H

#include "chpl/queries/Location.h"
#include "chpl/uast/VariableDecl.h"

#include <cassert>

namespace chpl {
namespace uast {


/**
  This class represents a declaration for
  multiple variables.
 
  E.g.

  \rst
  .. code-block:: chapel

      var a, b:int, c, d = 1;
      var x: int, y = 3, z: real;

  \endrst

  Each of the lines above is represented by a MultiVarDeclaration containing a
  list of VariableDecls.  Note that the initial value and/or type is inferred
  from later declarations.

  Since the MultiVarDeclaration does not declare a Sym itself, it is not a
  subclass of Decl. Rather, it contains several VariableDecls.

 */
class MultiVarDeclaration final : public Expression {
 private:
  MultiVarDeclaration(ASTList children)
    : Expression(asttags::MultiVarDeclaration, std::move(children)) {

    assert(isVariableDeclAndCommentList(children_));
  }
  static bool isVariableDeclAndCommentList(const ASTList& list);
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~MultiVarDeclaration() override = default;
  static owned<MultiVarDeclaration> build(Builder* builder, Location loc,
                                          ASTList varDecls);

  /**
    Return a way to iterate over the contained VariableDecls and Comments.
   */
  ASTListIteratorPair<Expression> stmts() const {
    return ASTListIteratorPair<Expression>(children_.begin(), children_.end());
  }

  /**
   Return the number of VariableDecls and Comments contained.
   */
  int numStmts() const {
    return this->numChildren();
  }
  /**
   Return the i'th contained VariableDecl or Comment.
   */
  const Expression* stmt(int i) const {
    const ASTNode* ast = this->child(i);
    assert(ast->isVariableDecl() || ast->isComment());
    return (const Expression*)ast;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
