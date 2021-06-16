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

#ifndef CHPL_UAST_USECLAUSE_H
#define CHPL_UAST_USECLAUSE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents an individual use clause. For example:

  \rst
  .. code-block:: chapel
    // This contains two use clauses, 'Foo as X' and 'Baz as Y only A'.
    use Foo as X, Baz as Y only A;

  \endrst
 */
class UseClause final : public Expression {
 public:
  enum QualifierListKind {
    EXCEPT,
    ONLY,
    NONE
  };

 private:
  // TODO: Have this inherit from NamedDecl? Need to figure out what happens
  // when the used name is e.g. 'Foo.Bar'. Need to think about how this
  // will affect e.g. the resolver.
  UseClause(ASTList children, Decl::Visibility visibility,
            QualifierListKind qualifierListKind,
            int8_t qualifierListBodyChildNum,
            int numQualifiers)
    : Expression(asttags::UseClause, std::move(children)),
      visibility_(visibility),
      qualifierListKind_(qualifierListKind),
      qualifierListChildNum_(qualifierListChildNum),
      numQualifiers_(numQualifiers) {
    if (qualifierListKind_ == ONLY) {
      assert(numQualifiers >= 1);
    }
  }

  // No need to check 'nameExprChildNum_'.
  bool contentsMatchInner(const ASTNode* other) const override {
    const UseClause* rhs = other->toUseClause();
    return this->visibility_ == rhs->visibility_ &&
      this->qualifierListKind_ == rhs->qualifierListKind_ &&
      this->qualifierListChildNum_ == rhs->qualifierListChildNum_ &&
      this->numQualifiers == rhs->numQualifiers_ &&
      this->expressionContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  // This always exists and its position never changes.
  static const int8_t nameExprChildNum_ = 0;

  Decl::Visibility visibility_;
  QualifierListKind qualifierListKind_;
  int8_t qualiferListChildNum_;
  int numQualifiers_;

 public:
  ~UseClause() override = default;

  /**
    Create and return a use clause.
  */
  static owned<UseClause> build(Builder* builder, Location loc,
                                Decl::Visibility visibility,
                                owned<Expr> nameExpr);

  /**
    Create and return a use clause.
  */
  static owned<UseClause> build(Builder* builder, Location loc,
                                Decl::Visibility visibility,
                                owned<Expr> nameExpr,
                                QualifierListKind qualifierListKind,
                                ASTList qualifierList);

  /**
    Return the visibility of this use clause.
  */
  Decl::Visibility visibility() const {
    return visibility_;
  }

  /**
    Return the qualifier list kind of this use clause.
  */
  QualifierListKind qualifierListKind() const {
    return qualifierListKind_;
  }

  /**
    Return a way to iterate over the qualifier list of this use clause.
  */
  ASTListIteratorPair<Expression> qualifiers() const {
    auto begin = numQualifiers() ? children_.begin() + qualifierListChildNum_
                                 : children_.end();
    auto end = begin + numQualifiers_ - 1;
    return ASTListIteratorPair<Expression>(begin, end);
  }

  /**
    Return the length of the qualifier list contained in this use clause.
  */
  int numQualifiers() const {
    return this->numQualifiers_;
  }

  /**
    Return the i'th qualifier in the qualifier list of this use clause.
  */
  const Expression* qualifier(int i) const {
    if (qualifierListChildNum_ < 0) return nullptr;
    assert(i >= 0 && i < numQualifiers_);
    const ASTNode* ast = this->child(qualifierListChildNum_+i);
    assert(ast->isExpression());
    return (const Expression*)ast;
  }  
};


} // end namespace uast
} // end namespace chpl

#endif
