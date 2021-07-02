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

#ifndef CHPL_UAST_VISIBILITYCLAUSE_H
#define CHPL_UAST_VISIBILITYCLAUSE_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This class represents a visibility clause. Visibility clauses make up the
  contents of Use and Import statements. For example:

  \rst
  .. code-block:: chapel

    // This contains two visibility clauses, 'Foo as X' and 'Baz as Y'.
    use Foo as X, Baz as Y;

    // This contains one visibility clause, 'Baz as Z'.
    import Baz as Z;

  \endrst
 */
class VisibilityClause final : public Expression {
 public:

  /**
    These values represent the kind of limitations possessed by a visibility
    clause. Visibility clauses within use statements may have limitations of
    kind 'EXCEPT', 'ONLY', or 'NONE'. Those within import statements may have
    have limitations of kind 'BRACES' or 'NONE'.

    If the limitation kind is 'NONE', then the visibility clause should have
    no limitations. If the kind is 'BRACES' or 'EXCEPT', then the visibility
    clause should have one or more limitations. If the kind is 'ONLY', then
    it may have zero or more limitations.
  */
  enum LimitationKind {
    BRACES,
    EXCEPT,
    ONLY,
    NONE
  };

 private:
  VisibilityClause(ASTList children,  LimitationKind limitationKind,
                   int numLimitations)
    : Expression(asttags::VisibilityClause, std::move(children)),
      limitationKind_(limitationKind),
      numLimitations_(numLimitations) {

    switch (limitationKind_) {
      case BRACES: 
      case EXCEPT: assert(numLimitations >= 1); break;
      case ONLY: assert(numLimitations_ >= 0); break;
      case NONE: assert(numLimitations_ == 0); break;
    }
  }

  // No need to check 'symbolChildNum_' or 'limitationChildNum_'.
  bool contentsMatchInner(const ASTNode* other) const override {
    const VisibilityClause* rhs = other->toVisibilityClause();
    return this->limitationKind_ == rhs->limitationKind_ &&
      this->numLimitations_ == rhs->numLimitations_ &&
      this->expressionContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  // These always exist and their position never changes.
  static const int8_t symbolChildNum_ = 0;
  static const int8_t limitationChildNum_ = 1;

  LimitationKind limitationKind_;
  int numLimitations_;

 public:
  ~VisibilityClause() override = default;

  /**
    Create and return a visibility clause.
  */
  static owned<VisibilityClause> build(Builder* builder, Location loc,
                                       owned<Expression> symbol);

  /**
    Create and return a visibility clause.
  */
  static owned<VisibilityClause> build(Builder* builder, Location loc,
                                       owned<Expression> symbol,
                                       LimitationKind limitationKind,
                                       ASTList limitations);

  /**
    Get the symbol of this visibility clause. It may be a dot expression,
    an as expression, or an identifier.
  */
  const Expression* symbol() const {
    auto ret = child(symbolChildNum_);
    assert(ret->isExpression());
    return (const Expression*)ret;
  }

  /**
    Return the kind of limitations contained by this visibility clause.
  */
  LimitationKind limitationKind() const {
    return limitationKind_;
  }

  /**
    Return a way to iterate over the limitations of this visibility clause.
  */
  ASTListIteratorPair<Expression> limitations() const {
    auto begin = (numLimitations() > 0)
        ? children_.begin() + limitationChildNum_
        : children_.end();
    auto end = begin + numLimitations_;
    return ASTListIteratorPair<Expression>(begin, end);
  }

  /**
    Return the number of limitations contained in this visibility clause.
  */
  int numLimitations() const {
    return this->numLimitations_;
  }

  /**
    Return the i'th limitation of this visibility clause. If the limitation
    kind is 'EXCEPT', then the limitations will all be identifiers. If the
    limitation kind is 'ONLY' or 'BRACES', then the limitations may be
    identifers or as expressions containing identifiers.
  */
  const Expression* limitation(int i) const {
    assert(i >= 0 && i < numLimitations_);
    const ASTNode* ast = this->child(limitationChildNum_+i);
    assert(ast->isExpression());
    return (const Expression*)ast;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
