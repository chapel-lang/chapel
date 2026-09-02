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

#ifndef CHPL_UAST_MATCHCASE_H
#define CHPL_UAST_MATCHCASE_H

#include "chpl/framework/Location.h"
#include "chpl/uast/BlockStyle.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/SimpleBlockLike.h"
#include "chpl/uast/Block.h"

namespace chpl {
namespace uast {


/**
  This class represents a match case statement. These statements make up the body
  of the match statement.
 */
class MatchCase final : public AstNode {
 friend class AstNode;

 private:
  const int caseExprChildNum_;
  const BlockStyle blockStyle_;

  MatchCase(AstList children, int caseExprChildNum, BlockStyle blockStyle)
    : AstNode(asttags::MatchCase, std::move(children)),
      caseExprChildNum_(caseExprChildNum), blockStyle_(blockStyle) {
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(blockStyle_);
  }

  explicit MatchCase(Deserializer& des)
    : AstNode(asttags::MatchCase, des),
      caseExprChildNum_(des.readVInt()),
      blockStyle_(des.read<BlockStyle>()) {}

  bool contentsMatchInner(const AstNode* other) const override {
    const MatchCase* rhs = other->toMatchCase();
    return this->caseExprChildNum_ == rhs->caseExprChildNum_ &&
           this->blockStyle_ == rhs->blockStyle_;
  }

  void markUniqueStringsInner(Context* context) const override {

  }

  void dumpFieldsInner(const DumpSettings& s) const override;

 public:

  /**
    Create and return a match case statement.
  */
  static owned<MatchCase> build(Builder* builder, Location loc,
                           owned<AstNode> caseExpr,
                           BlockStyle blockStyle,
                           AstList stmts);

  /**
   Returns the block style of this match case statement.
  */
  BlockStyle blockStyle() const {
    return blockStyle_;
  }

  /**
    Returns the expression of this match case statement.
  */
  const AstNode* expr() const {
    if (caseExprChildNum_ < 0) return nullptr;
    return child(caseExprChildNum_);
  }

  /**
    Returns the body node
  */
  const Block* body() const {
    auto idx = isOtherwise() ? 0 : 1;
    auto ret = child(idx);
    CHPL_ASSERT(ret->isBlock());
    return (const Block*)ret;
  }

  /**
    Returns true if this match case statement uses the otherwise keyword.
  */
  bool isOtherwise() const {
    return caseExprChildNum_ < 0;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
