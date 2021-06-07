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

#include "chpl/uast/Coforall.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<Coforall> Coforall::build(Builder* builder, Location loc,
                                owned<Decl> index,
                                owned<Expression> iterand,
                                owned<WithClause> withClause,
                                BlockStyle blockStyle,
                                ASTList stmts) {

  assert(iterand.get() != nullptr);

  ASTList lst;
  int8_t indexChildNum = -1;
  int8_t iterandChildNum = -1;
  int8_t withClauseChildNum = -1;

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
  const int numLoopBodyStmts = stmts.size();

  for (auto& stmt : stmts) {
    lst.push_back(std::move(stmt));
  }

  Coforall* ret = new Coforall(std::move(lst), indexChildNum,
                               iterandChildNum,
                               withClauseChildNum,
                               blockStyle,
                               loopBodyChildNum,
                               numLoopBodyStmts);

  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
