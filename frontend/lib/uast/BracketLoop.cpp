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

#include "chpl/uast/BracketLoop.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<BracketLoop> BracketLoop::build(Builder* builder, Location loc,
                                      owned<Decl> index,
                                      owned<AstNode> iterand,
                                      owned<WithClause> withClause,
                                      BlockStyle blockStyle,
                                      owned<Block> body,
                                      bool isExpressionLevel,
                                      owned<AttributeGroup> attributeGroup) {

  CHPL_ASSERT(iterand.get() != nullptr);
  CHPL_ASSERT(body.get() != nullptr);

  AstList lst;
  int8_t indexChildNum = NO_CHILD;
  int8_t iterandChildNum = NO_CHILD;
  int8_t withClauseChildNum = NO_CHILD;
  int attributeGroupChildNum = NO_CHILD;

  if (attributeGroup.get() != nullptr) {
    attributeGroupChildNum = lst.size();
    lst.push_back(std::move(attributeGroup));
  }

  if (index.get() != nullptr) {
    indexChildNum = lst.size();
    lst.push_back(std::move(index));
  }

  if (iterand.get() != nullptr) {
    iterandChildNum = lst.size();
    lst.push_back(std::move(iterand));
  }

  if (withClause.get() != nullptr) {
    withClauseChildNum = lst.size();
    lst.push_back(std::move(withClause));
  }

  const int loopBodyChildNum = lst.size();
  lst.push_back(std::move(body));

  BracketLoop* ret = new BracketLoop(std::move(lst), indexChildNum,
                                     iterandChildNum,
                                     withClauseChildNum,
                                     blockStyle,
                                     loopBodyChildNum,
                                     isExpressionLevel,
                                     attributeGroupChildNum);

  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

bool BracketLoop::isMaybeArrayType() const {
  if (!isExpressionLevel()) return false;
  if (iterand()->isZip()) return false;
  if (numStmts() != 1) return false;
  if (index() && stmt(0)->isConditional()) return false;
  return true;
}


} // namespace uast
} // namespace chpl
