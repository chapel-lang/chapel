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

#ifndef CHPL_UAST_TUPLEDECL_H
#define CHPL_UAST_TUPLEDECL_H

#include "chpl/uast/Decl.h"
#include "chpl/uast/Variable.h"
#include "chpl/queries/Location.h"

#include <cassert>

namespace chpl {
namespace uast {


/**
  This class represents a tuple variable declaration
 
  E.g.

  \rst
  .. code-block:: chapel

      var (a, b) = (1,2);
      var (c, _) = (3.0,"hi");
      var (d, e) = returnTuple();

  \endrst

  Each of the lines above is represented by a MultiDecl containing a
  list of VariableDecls.  Note that the initial value and/or type is inferred
  from later declarations.

  Since the MultiDecl does not declare a Sym itself, it is not a
  subclass of Decl. Rather, it contains several VariableDecls.

 */
class TupleDecl final : public Decl {
 private:
  Variable::Kind kind_;
  int numElements_; 
  int typeExpressionChildNum_;
  int initExpressionChildNum_;

  TupleDecl(ASTList children, Decl::Visibility vis, Variable::Kind kind,
            int numElements,       
            int typeExpressionChildNum,
            int initExpressionChildNum)
    : Decl(asttags::TupleDecl, std::move(children), vis),
      kind_(kind),
      numElements_(numElements),
      typeExpressionChildNum_(typeExpressionChildNum),
      initExpressionChildNum_(initExpressionChildNum) {

    assert(assertAcceptableTupleDecl());
  }
  bool assertAcceptableTupleDecl();
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~TupleDecl() override = default;
  static owned<TupleDecl> build(Builder* builder, Location loc,
                                Decl::Visibility vis,
                                Variable::Kind kind,
                                ASTList elements,
                                owned<Expression> typeExpression,
                                owned<Expression> initExpression);

  /**
    Returns the kind of the tuple (`var` / `const` / `param` etc).
   */
  const Variable::Kind kind() const { return this->kind_; }

  /**
    Return a way to iterate over the contained Decls
    (which are each Variables or TupleDecls).
   */
  ASTListIteratorPair<Decl> decls() const {
    return ASTListIteratorPair<Decl>(children_.begin(),
                                     children_.begin() + numElements_);
  }

  /**
   Return the number of Decls contained within this TupleDecl.
   */
  int numDecls() const {
    return numElements_;
  }
  /**
   Return the i'th contained Decl.
   */
  const Decl* decl(int i) const {
    const ASTNode* ast = this->child(i);
    assert(ast->isVariable() || ast->isTupleDecl());
    assert(ast->isDecl());
    return (const Decl*)ast;
  }

  /**
    Returns the type expression used in this TupleDecl's declaration, or
    nullptr if there wasn't one.
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
    Returns the init expression used in this TupleDecl's declaration, or
    nullptr if there wasn't one.
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
