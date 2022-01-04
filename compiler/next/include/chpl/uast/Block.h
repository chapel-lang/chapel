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

#ifndef CHPL_UAST_BLOCK_H
#define CHPL_UAST_BLOCK_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/SimpleBlockLike.h"

namespace chpl {
namespace uast {


/**
  This class represents a { } block.
 */
class Block final : public SimpleBlockLike {
 private:
  Block(ASTList stmts, int bodyChildNum, int numBodyStmts)
    : SimpleBlockLike(asttags::Block, std::move(stmts),
                      BlockStyle::EXPLICIT,
                      bodyChildNum,
                      numBodyStmts) {
    assert(isExpressionASTList(children_));
    assert(blockStyle_ == BlockStyle::EXPLICIT);
    assert(bodyChildNum_ >= 0);
  }

  bool contentsMatchInner(const ASTNode* other) const override {
    return simpleBlockLikeContentsMatchInner(other);
  }

  void markUniqueStringsInner(Context* context) const override {
    simpleBlockLikeMarkUniqueStringsInner(context);
  }

 public:
  ~Block() override = default;

  /**
   Create and return a Block containing the passed stmts.
   */
  static owned<Block> build(Builder* builder, Location loc, ASTList stmts);

};


} // end namespace uast
} // end namespace chpl

#endif
