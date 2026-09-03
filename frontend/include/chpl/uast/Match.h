/*
 * Copyright 2021-2026 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_MATCH_H
#define CHPL_UAST_MATCH_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/MatchCase.h"

namespace chpl {
namespace uast {


/**
  This class represents a match statement
  (currently only supported for unions). For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      union U {var x: int, var y: real};
      var u = new U(y=3.14);
      union select u {
        when x do writeln('x');
        when y do writeln('y');
        otherwise do writeln('otherwise');
      }

  \endrst

 */
class Match final : public AstNode {
 friend class AstNode;

 private:
  // The position of these never change.
  static const int8_t exprChildNum_ = 0;
  static const int8_t caseStmtStartChildNum_ = 1;
  int numCaseStmts_;
  int otherwiseStmtChildNum_;

  Match(AstList children, int numCaseStmts, int otherwiseStmtChildNum)
    : AstNode(asttags::Match, std::move(children)),
      numCaseStmts_(numCaseStmts), otherwiseStmtChildNum_(otherwiseStmtChildNum) {
  }

  void serializeInner(Serializer& ser) const override {
    ser.writeVInt(numCaseStmts_);
    ser.writeVInt(otherwiseStmtChildNum_);
  }

  explicit Match(Deserializer& des) : AstNode(asttags::Match, des) {
    numCaseStmts_ = des.readVInt();
    otherwiseStmtChildNum_ = des.readVInt();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Match* rhs = other->toMatch();
    return this->numCaseStmts_ == rhs->numCaseStmts_ &&
           this->otherwiseStmtChildNum_ == rhs->otherwiseStmtChildNum_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  std::string dumpChildLabelInner(int i) const override;

 public:

  /**
    Create and return a match statement.
  */
  static owned<Match> build(Builder* builder, Location loc,
                             owned<AstNode> expr,
                             AstList caseStmts,
                             owned<AstNode> otherwiseStmt);

  /**
    Returns the expression of this match statement.
  */
  const AstNode* expr() const {
    auto ret = child(exprChildNum_);
    return ret;
  }

  /**
    Returns the number of case statements in this match statement.
  */
  int numCaseStmts() const {
    return numCaseStmts_;
  }

  /**
    Return the i'th case statement in this match statement.
  */
  const MatchCase* caseStmt(int i) const {
    CHPL_ASSERT(i >= 0 && i < numCaseStmts_);
    auto ret = child(caseStmtStartChildNum_ + i);
    CHPL_ASSERT(ret->isMatchCase());
    return (const MatchCase*)ret;
  }

  /**
    Iterate over the case statements in this match statement.
  */
  AstListIteratorPair<MatchCase> caseStmts() const {
    auto begin = numCaseStmts_ > 0
        ? children_.begin() + caseStmtStartChildNum_
        : children_.end();
    auto end = begin + numCaseStmts_;
    return AstListIteratorPair<MatchCase>(begin, end);
  }

  /**
   Iterate over the case statements (including the otherwise) in this match statement.
  */
 AstListIteratorPair<MatchCase> allCaseStmts() const {
  auto begin = children_.begin() + caseStmtStartChildNum_;
    auto end = otherwiseStmtChildNum_ >= 0
        ? children_.begin() + otherwiseStmtChildNum_ + 1
        : begin + numCaseStmts_;
    return AstListIteratorPair<MatchCase>(begin, end);
  }

  /**
    Return the otherwise statement in this match statement, if it exists.
  */
  const MatchCase* otherwiseStmt() const {
    if (otherwiseStmtChildNum_ < 0) return nullptr;
    auto ret = child(otherwiseStmtChildNum_);
    CHPL_ASSERT(ret->isMatchCase());
    return (const MatchCase*)ret;
  }

  bool hasOtherwise() const {
    return otherwiseStmtChildNum_ >= 0;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
