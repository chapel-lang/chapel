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
  enum LimitationClauseKind {
    EXCEPT,
    ONLY,
    NONE
  };

 private:
  UseClause(ASTList children,  LimitationClauseKind limitationClauseKind,
            int8_t limitationClauseChildNum,
            int numLimitations)
    : Expression(asttags::UseClause, std::move(children)),
      limitationClauseKind_(limitationClauseKind),
      limitationClauseChildNum_(limitationClauseChildNum),
      numLimitations_(numLimitations) {

    switch (limitationClauseKind_) {
      case EXCEPT: assert(numLimitations >= 1); break;
      case NONE: assert(numLimitations_ == 0); break;
      case ONLY: assert(numLimitations_ >= 0); break;
    }
  }

  // No need to check 'nameChildNum_'.
  bool contentsMatchInner(const ASTNode* other) const override {
    const UseClause* rhs = other->toUseClause();
    return this->limitationClauseKind_ == rhs->limitationClauseKind_ &&
      this->limitationClauseChildNum_ == rhs->limitationClauseChildNum_ &&
      this->numLimitations_ == rhs->numLimitations_ &&
      this->expressionContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    expressionMarkUniqueStringsInner(context);
  }

  // This always exists and its position never changes.
  static const int8_t nameChildNum_ = 0;

  LimitationClauseKind limitationClauseKind_;
  int8_t limitationClauseChildNum_;
  int numLimitations_;

 public:
  ~UseClause() override = default;

  /**
    Create and return a use clause.
  */
  static owned<UseClause> build(Builder* builder, Location loc,
                                owned<Expression> name);

  /**
    Create and return a use clause.
  */
  static owned<UseClause> build(Builder* builder, Location loc,
                                owned<Expression> name,
                                LimitationClauseKind limitationClauseKind,
                                ASTList limitationClause);

  /**
    Get the name expression of this use clause. It may be a dot expression,
    an as expression, or an identifier.
  */
  const Expression* name() const {
    auto ret = child(nameChildNum_);
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
    auto begin = numLimitations()
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
    if (limitationClauseChildNum_ < 0) return nullptr;
    assert(i >= 0 && i < numLimitations_);
    const ASTNode* ast = this->child(limitationClauseChildNum_+i);
    assert(ast->isExpression());
    return (const Expression*)ast;
  }  
};


} // end namespace uast
} // end namespace chpl

#endif
