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

#include "chpl/uast/Conditional.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool Conditional::contentsMatchInner(const ASTNode* other) const {
  const Conditional* lhs = this;
  const Conditional* rhs = other->toConditional();

  if (lhs->conditionChildNum_ != rhs->conditionChildNum_)
    return false;

  if (lhs->thenBlockStyle_ != rhs->thenBlockStyle_)
    return false;

  if (lhs->thenBodyChildNum_ != rhs->thenBodyChildNum_)
    return false;

  if (lhs->numThenBodyStmts_ != rhs->numThenBodyStmts_)
    return false;

  if (lhs->elseBlockStyle_ != rhs->elseBlockStyle_)
    return false;

  if (lhs->elseBodyChildNum_ != rhs->elseBodyChildNum_)
    return false;

  if (lhs->numElseBodyStmts_ != rhs->numElseBodyStmts_)
    return false;

  if (lhs->isExpressionLevel_ != rhs->isExpressionLevel_)
    return false;

  if (!lhs->expressionContentsMatchInner(rhs))
    return false;

  return true;
}

owned<Conditional> Conditional::build(Builder* builder, Location loc,
                                      owned<Expression> condition,
                                      BlockStyle thenBlockStyle,
                                      ASTList thenStmts,
                                      BlockStyle elseBlockStyle,
                                      ASTList elseStmts,
                                      bool isExpressionLevel) {
  ASTList lst;
  int8_t conditionChildNum = -1;

  if (condition.get() != nullptr) {
    conditionChildNum = lst.size();
    lst.push_back(std::move(condition));
  }

  const int8_t thenBodyChildNum = lst.size();
  const int numThenBodyStmts = thenStmts.size();

  for (auto& stmt : thenStmts) {
    lst.push_back(std::move(stmt));
  }

  const int elseBodyChildNum = lst.size();
  const int numElseBodyStmts = elseStmts.size();

  for (auto& stmt : elseStmts) {
    lst.push_back(std::move(stmt));
  }

  Conditional* ret = new Conditional(std::move(lst), conditionChildNum,
                                     thenBlockStyle,
                                     thenBodyChildNum,
                                     numThenBodyStmts,
                                     elseBlockStyle,
                                     elseBodyChildNum,
                                     numElseBodyStmts,
                                     isExpressionLevel);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<Conditional> Conditional::build(Builder* builder, Location loc,
                                      owned<Expression> condition,
                                      BlockStyle thenBlockStyle,
                                      ASTList thenStmts) {
  ASTList lst;
  int8_t conditionChildNum = -1;

  if (condition.get() != nullptr) {
    conditionChildNum = lst.size();
    lst.push_back(std::move(condition));
  }

  const int8_t thenBodyChildNum = lst.size();
  const int numThenBodyStmts = thenStmts.size();

  for (auto& stmt : thenStmts) {
    lst.push_back(std::move(stmt));
  }

  const BlockStyle elseBlockStyle = BlockStyle::IMPLICIT;
  const int elseBodyChildNum = -1;
  const int numElseBodyStmts = 0; 

  Conditional* ret = new Conditional(std::move(lst), conditionChildNum,
                                     thenBlockStyle,
                                     thenBodyChildNum,
                                     numThenBodyStmts,
                                     elseBlockStyle,
                                     elseBodyChildNum,
                                     numElseBodyStmts,
                                     /*isExpressionLevel*/ false);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

} // namespace uast
} // namespace chpl
