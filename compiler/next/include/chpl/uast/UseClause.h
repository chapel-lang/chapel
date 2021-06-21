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
    // This contains two use clauses, 'Foo as X' and 'Baz as Y'.
    use Foo as X, Baz as Y;

  \endrst
 */
class UseClause final : public Expression {
 public:

  /**
    These values represent the kind of limitation clause possessed by a
    use clause. If the limitation clause is opened by the keyword 'except',
    then the value of 'LimitationClauseKind' will be 'EXCEPT'. The same
    goes for 'ONLY'. If there is no limitation clause then the kind will
    be 'NONE'.

    If the limitation clause kind is 'NONE', then the use clause should have
    no limitations. If the kind is 'EXCEPT', then the use clause should have
    at least one limitation. If the kind is 'ONLY', then the use clause may
    have zero or more limitations.
  */
  enum LimitationClauseKind {
    EXCEPT,
    ONLY,
    NONE
  };

 private:
  UseClause(ASTList children,  LimitationClauseKind limitationClauseKind,
            int numLimitations)
    : Expression(asttags::UseClause, std::move(children)),
      limitationClauseKind_(limitationClauseKind),
      numLimitations_(numLimitations) {

    switch (limitationClauseKind_) {
      case EXCEPT: assert(numLimitations >= 1); break;
      case NONE: assert(numLimitations_ == 0); break;
      case ONLY: assert(numLimitations_ >= 0); break;
    }
  }

  // No need to check 'symbolChildNum_' or 'limitationClauseChildNum_'.
  bool contentsMatchInner(const ASTNode* other) const override {
    const UseClause* rhs = other->toUseClause();
    return this->limitationClauseKind_ == rhs->limitationClauseKind_ &&
      this->numLimitations_ == rhs->numLimitations_ &&
      this->expressionContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  // These always exist and their position never changes.
  static const int8_t symbolChildNum_ = 0;
  static const int8_t limitationClauseChildNum_ = 1;

  LimitationClauseKind limitationClauseKind_;
  int numLimitations_;

 public:
  ~UseClause() override = default;

  /**
    Create and return a use clause.
  */
  static owned<UseClause> build(Builder* builder, Location loc,
                                owned<Expression> symbol);

  /**
    Create and return a use clause.
  */
  static owned<UseClause> build(Builder* builder, Location loc,
                                owned<Expression> symbol,
                                LimitationClauseKind limitationClauseKind,
                                ASTList limitationClause);

  /**
    Get the name expression of this use clause. It may be a dot expression,
    an as expression, or an identifier.
  */
  const Expression* symbol() const {
    auto ret = child(symbolChildNum_);
    assert(ret->isExpression());
    return (const Expression*)ret;
  }

  /**
    Return the kind of limitation clause contained by this use clause.
  */
  LimitationClauseKind limitationClauseKind() const {
    return limitationClauseKind_;
  }

  /**
    Return a way to iterate over the limitations of this use clause.
  */
  ASTListIteratorPair<Expression> limitations() const {
    auto begin = (numLimitations() > 0)
        ? children_.begin() + limitationClauseChildNum_
        : children_.end();
    auto end = begin + numLimitations_;
    return ASTListIteratorPair<Expression>(begin, end);
  }

  /**
    Return the number of limitations contained in this use clause.
  */
  int numLimitations() const {
    return this->numLimitations_;
  }

  /**
    Return the i'th limitation of this use clause. If the limitation clause
    kind is 'EXCEPT', then the limitations will all be identifiers. If the
    limitation clause kind is 'ONLY', then the limitations may be
    identifers or as expressions containing identifiers.
  */
  const Expression* limitation(int i) const {
    assert(i >= 0 && i < numLimitations_);
    const ASTNode* ast = this->child(limitationClauseChildNum_+i);
    assert(ast->isExpression());
    return (const Expression*)ast;
  }  
};


} // end namespace uast
} // end namespace chpl

#endif
