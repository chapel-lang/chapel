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

#ifndef CHPL_UAST_TRY_H
#define CHPL_UAST_TRY_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/Catch.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This class represents a try statement or try expression. For example:

  \rst
  .. code-block:: chapel

      // Example of a try expression:
      var x = try! foo();

      // Example of a try statement:
      try! {
        foo();
      } catch e: FooError {
        halt('A FooError occurred');
      }

  \endrst

  A try statement may contain a number of catch blocks (represented by the
  Catch uAST node), while a try expression will never contain a catch
  block.
 */
class Try final : public AstNode {
 private:
  Try(AstList children, int numBodyStmts, int numHandlers,
      bool isExpressionLevel,
      bool isTryBang)
        : AstNode(asttags::Try, std::move(children)),
          numBodyStmts_(numBodyStmts),
          numHandlers_(numHandlers),
          isExpressionLevel_(isExpressionLevel),
          isTryBang_(isTryBang) {
    if (isExpressionLevel_) {
      assert(numBodyStmts == 1);
      assert(numHandlers == 0);
    }
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Try* rhs = other->toTry();
    return this->numBodyStmts_ == rhs->numBodyStmts_ &&
      this->numHandlers_ == rhs->numHandlers_ &&
      this->isExpressionLevel_ == rhs->isExpressionLevel_ &&
      this->isTryBang_ == rhs->isTryBang_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  // If this exists, its position is always the same.
  static const int8_t bodyChildNum_ = 0;

  int numBodyStmts_;
  int numHandlers_;
  bool isExpressionLevel_;
  bool isTryBang_;

 public:
  ~Try() override = default;

  /**
    Create and return a try statement.
  */
  static owned<Try> build(Builder* builder, Location loc, AstList stmts,
                          AstList catches,
                          bool isTryBang);

  /**
    Create and return a try expression.
  */
  static owned<Try> build(Builder* builder, Location loc,
                          owned<AstNode> expr,
                          bool isTryBang,
                          bool isExpressionLevel);

  /**
    Iterate over the statements contained in this try.
  */
  AstListIteratorPair<AstNode> stmts() const {
    auto begin = numBodyStmts_ ? children_.begin() + bodyChildNum_
                               : children_.end();
    auto end = begin + numBodyStmts_;
    return AstListIteratorPair<AstNode>(begin, end);
  }

  /**
    Return the number of statements contained in this try.
  */
  int numStmts() const {
    return numBodyStmts_;
  }

  /**
    Get the i'th statement in the body of this try.
  */
  const AstNode* stmt(int i) const {
    assert(i >= bodyChildNum_ && i < numBodyStmts_);
    auto ret = child(bodyChildNum_ + i);
    assert(ret);
    return ret;
  }

  /**
    Iterate over the catch blocks contained in this try.
  */
  AstListIteratorPair<Catch> handlers() const {
    auto begin = numHandlers_ ? children_.begin() + numBodyStmts_
                              : children_.end();
    auto end = begin + numHandlers_;
    return AstListIteratorPair<Catch>(begin, end);
  }

  /**
    Return the number of catch blocks contained in this try.
  */
  int numHandlers() const {
    return numHandlers_;
  }

  /**
    Return the i'th catch block contained in this try.
  */
  const Catch* handler(int i) const {
    assert(i >= 0 && i < numHandlers_);
    auto ret = child(numBodyStmts_ + i);
    assert(ret && ret->isCatch());
    return (const Catch*)ret;
  }

  /**
    Return true if this try is at an expression level.
  */
  bool isExpressionLevel() const {
    return isExpressionLevel_;
  }

  /**
    Return true if this try should halt when an error is not handled.
  */
  bool isTryBang() const {
    return isTryBang_;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
