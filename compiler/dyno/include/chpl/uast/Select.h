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

#ifndef CHPL_UAST_SELECT_H
#define CHPL_UAST_SELECT_H

#include "chpl/queries/Location.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/When.h"

namespace chpl {
namespace uast {


/**
  This class represents a select statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      const i = 2;
      select i {
        when 0 do writeln('zero');
        otherwise do writeln('number: ', i);
      }

  \endrst

 */
class Select final : public AstNode {
 private:
  Select(AstList children, int numWhenStmts)
    : AstNode(asttags::Select, std::move(children)),
      numWhenStmts_(numWhenStmts) {
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Select* rhs = other->toSelect();
    return this->numWhenStmts_ == rhs->numWhenStmts_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  // The position of these never change.
  static const int8_t exprChildNum_ = 0;
  static const int8_t whenStmtStartChildNum_ = 1;

  int numWhenStmts_;

 public:

  /**
    Create and return a select statement.
  */
  static owned<Select> build(Builder* builder, Location loc,
                             owned<AstNode> expr,
                             AstList whenStmts);

  /**
    Returns the expression of this select statement.
  */
  const AstNode* expr() const {
    auto ret = child(exprChildNum_);
    return ret;
  }

  /**
    Returns the number of when statements in this select statement.
  */
  int numWhenStmts() const {
    return numWhenStmts_;
  }

  /**
    Return the i'th when statement in this select statement.
  */
  const When* whenStmt(int i) const {
    assert(i >= 0 && i < numWhenStmts_);
    auto ret = child(whenStmtStartChildNum_ + i);
    assert(ret->isWhen());
    return (const When*)ret;
  }

  /**
    Iterate over the when statements in this select statement.
  */
  AstListIteratorPair<When> whenStmts() const {
    auto begin = numWhenStmts_ > 0
        ? children_.begin() + whenStmtStartChildNum_
        : children_.end();
    auto end = begin + numWhenStmts_;
    return AstListIteratorPair<When>(begin, end);
  }

};


} // end namespace uast
} // end namespace chpl

#endif
