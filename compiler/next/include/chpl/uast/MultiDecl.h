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

#ifndef CHPL_UAST_MULTIDECL_H
#define CHPL_UAST_MULTIDECL_H

#include "chpl/uast/Decl.h"
#include "chpl/uast/Comment.h"
#include "chpl/queries/Location.h"

#include <cassert>

namespace chpl {
namespace uast {


/**
  This class represents a declaration for multiple variables.
 
  E.g.

  \rst
  .. code-block:: chapel

      var a, b:int, c, d = 1;
      var x: int, y = 3, z: real;

  \endrst

  Each of the lines above is represented by a MultiDecl containing a
  list of VariableDecls.  Note that the initial value and/or type is inferred
  from later declarations.

  Since the MultiDecl does not itself have a name, it is not
  a NamedDecl. Rather, it can contain NamedDecls.

 */
class MultiDecl final : public Decl {
 private:
  MultiDecl(ASTList children, Decl::Visibility vis)
    : Decl(asttags::MultiDecl, std::move(children), vis) {

    assert(isAcceptableMultiDecl());
  }
  bool isAcceptableMultiDecl();
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~MultiDecl() override = default;
  static owned<MultiDecl> build(Builder* builder, Location loc,
                                Decl::Visibility vis, 
                                ASTList varDecls);

  /**
    Return a way to iterate over the contained VariableDecls and Comments.
   */
  ASTListIteratorPair<Expression> declOrComments() const {
    return ASTListIteratorPair<Expression>(children_.begin(), children_.end());
  }

  /**
   Return the number of VariableDecls and Comments contained.
   */
  int numDeclOrComments() const {
    return this->numChildren();
  }
  /**
   Return the i'th contained VariableDecl or Comment.
   */
  const Expression* declOrComment(int i) const {
    const ASTNode* ast = this->child(i);
    assert(ast->isVariable() || ast->isTupleDecl() || ast->isComment());
    return (const Expression*)ast;
  }

  /**
   Return a way to iterate over the contained Decls (ignoring Comments)
   */
  ASTListNoCommentsIteratorPair<Decl> decls() const {
    return ASTListNoCommentsIteratorPair<Decl>(children_.begin(),
                                               children_.end());
  }

};


} // end namespace uast
} // end namespace chpl

#endif
