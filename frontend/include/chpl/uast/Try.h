/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/Location.h"
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
 friend class AstNode;

 private:
  // body position is always the same
  static const int8_t bodyChildNum_ = 0;
  // try expressions always contain a body expression
  // try statements always contain a body block
  static const int8_t numBodyStmts_ = 1;

  int numHandlers_;
  bool containsBlock_;
  bool isExpressionLevel_;
  bool isTryBang_;

  Try(AstList children,
      int numHandlers,
      bool containsBlock,
      bool isExpressionLevel,
      bool isTryBang)
        : AstNode(asttags::Try, std::move(children)),
          numHandlers_(numHandlers),
          containsBlock_(containsBlock),
          isExpressionLevel_(isExpressionLevel),
          isTryBang_(isTryBang) {
    if (isExpressionLevel_) {
      CHPL_ASSERT(numHandlers == 0);
    }
    if (containsBlock) {
      CHPL_ASSERT(child(bodyChildNum_)->isBlock());
    } else {
      CHPL_ASSERT(numHandlers == 0);
    }
  }

  void serializeInner(Serializer& ser) const override {
    ser.writeVInt(numHandlers_);
    ser.write(containsBlock_);
    ser.write(isExpressionLevel_);
    ser.write(isTryBang_);
  }

  explicit Try(Deserializer& des)
    : AstNode(asttags::Try, des) {
      numHandlers_ = des.readVInt();
      containsBlock_ = des.read<bool>();
      isExpressionLevel_ = des.read<bool>();
      isTryBang_ = des.read<bool>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Try* rhs = other->toTry();
    return this->numHandlers_ == rhs->numHandlers_ &&
      this->containsBlock_ == rhs->containsBlock_ &&
      this->isExpressionLevel_ == rhs->isExpressionLevel_ &&
      this->isTryBang_ == rhs->isTryBang_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  void dumpFieldsInner(const DumpSettings& s) const override;
  std::string dumpChildLabelInner(int i) const override;

 public:
  ~Try() override = default;

  /**
    Create and return a try statement.
  */
  static owned<Try> build(Builder* builder, Location loc, owned<Block> body,
                          AstList catches,
                          bool isTryBang);

  /**
    Create and return a try expression or decorated statement.
  */
  static owned<Try> build(Builder* builder, Location loc,
                          owned<AstNode> expr,
                          bool isTryBang,
                          bool isExpressionLevel);

  /**
    Return the contained block for a try statement.
    For an expression-level try, returns nullptr.
   */
  const Block* body() const {
    if (containsBlock_) {
      CHPL_ASSERT(numBodyStmts_ == 1);
      return (Block*) child(bodyChildNum_);
    } else {
      return nullptr;
    }
  }

  /**
    Iterate over the statements contained in this try.
  */
  AstListIteratorPair<AstNode> stmts() const {
    if (const Block* stmtBody = body()) {
      return stmtBody->stmts();
    } else {
      auto begin = numBodyStmts_ ? children_.begin() + bodyChildNum_
                                 : children_.end();
      auto end = begin + numBodyStmts_;
      return AstListIteratorPair<AstNode>(begin, end);
    }
  }

  /**
    Return the number of statements contained in this try.
  */
  int numStmts() const {
    if (const Block* stmtBody = body()) {
      return stmtBody->numStmts();
    } else {
      return numBodyStmts_;
    }
  }

  /**
    Get the i'th statement in the body of this try.
  */
  const AstNode* stmt(int i) const {
    if (const Block* stmtBody = body()) {
      return stmtBody->stmt(i);
    } else {
      CHPL_ASSERT(i >= bodyChildNum_ && i < numBodyStmts_);
      auto ret = child(bodyChildNum_ + i);
      CHPL_ASSERT(ret);
      return ret;
    }
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
    CHPL_ASSERT(i >= 0 && i < numHandlers_);
    auto ret = child(numBodyStmts_ + i);
    CHPL_ASSERT(ret && ret->isCatch());
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
