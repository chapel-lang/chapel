/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/IntentList.h"
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

  Each of the lines above is represented by a TupleDecl containing a
  list of VariableDecls.  Note that the initial value and/or type is inferred
  from later declarations.

  Since the Tuple does not itself have a name, it is not
  a NamedDecl. Rather, it can contain NamedDecls.

 */
class TupleDecl final : public Decl {
 public:
  enum IntentOrKind {
    DEFAULT_INTENT = (int) IntentList::DEFAULT_INTENT,
    CONST_INTENT   = (int) IntentList::CONST_INTENT,
    VAR            = (int) IntentList::VAR,
    CONST_VAR      = (int) IntentList::CONST_VAR,
    CONST_REF      = (int) IntentList::CONST_REF,
    REF            = (int) IntentList::REF,
    IN             = (int) IntentList::IN,
    CONST_IN       = (int) IntentList::CONST_IN,
    OUT            = (int) IntentList::OUT,
    INOUT          = (int) IntentList::INOUT,
    INDEX          = (int) IntentList::INDEX,
    PARAM          = (int) IntentList::PARAM,
    TYPE           = (int) IntentList::TYPE
  };

 private:
  TupleDecl::IntentOrKind intentOrKind_;
  int numElements_; 
  int typeExpressionChildNum_;
  int initExpressionChildNum_;

  TupleDecl(ASTList children, int attributesChildNum, Decl::Visibility vis,
            Decl::Linkage linkage,
            IntentOrKind intentOrKind,
            int numElements,       
            int typeExpressionChildNum,
            int initExpressionChildNum)
    : Decl(asttags::TupleDecl, std::move(children), attributesChildNum,
           vis,
           linkage,
           /*linkageNameChildNum*/ -1),
      intentOrKind_(intentOrKind),
      numElements_(numElements),
      typeExpressionChildNum_(typeExpressionChildNum),
      initExpressionChildNum_(initExpressionChildNum) {

    assert(assertAcceptableTupleDecl());
  }

  bool assertAcceptableTupleDecl();

  bool contentsMatchInner(const ASTNode* other) const override {
    const TupleDecl* lhs = this;
    const TupleDecl* rhs = (const TupleDecl*) other;
    return lhs->declContentsMatchInner(rhs) &&
           lhs->intentOrKind_ == rhs->intentOrKind_ &&
           lhs->numElements_ == rhs->numElements_ &&
           lhs->typeExpressionChildNum_ == rhs->typeExpressionChildNum_ &&
           lhs->initExpressionChildNum_ == rhs->initExpressionChildNum_;
  }

  void markUniqueStringsInner(Context* context) const override {
    declMarkUniqueStringsInner(context);
  }

  int declChildNum() const {
    return attributes() ? 1 : 0;
  }

 public:
  ~TupleDecl() override = default;

  static owned<TupleDecl> build(Builder* builder, Location loc,
                                owned<Attributes> attributes,
                                Decl::Visibility vis,
                                Decl::Linkage linkage,
                                IntentOrKind intentOrKind,
                                ASTList elements,
                                owned<Expression> typeExpression,
                                owned<Expression> initExpression);

  /**
    Returns the intent or kind of the tuple (`var` / `in` / `param` etc).
   */
  IntentOrKind intentOrKind() const { return this->intentOrKind_; }

  /**
    Return a way to iterate over the contained Decls
    (which are each Variables or TupleDecls).
   */
  ASTListIteratorPair<Decl> decls() const {
    auto begin = numDecls()
        ? children_.begin() + declChildNum()
        : children_.end();
    auto end = begin + numDecls();
    return ASTListIteratorPair<Decl>(begin, end);
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
    assert(i >= 0 && i < numDecls());
    const ASTNode* ast = this->child(i + declChildNum());
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
