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

#include "chpl/uast/Local.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


std::string Local::dumpChildLabelInner(int i) const {
  if (i == condChildNum_) {
    return "cond";
  }

  return SimpleBlockLike::dumpChildLabelInner(i);
}

owned<Local> Local::build(Builder* builder,
                          Location loc,
                          BlockStyle blockStyle,
                          AstList stmts) {

  AstList lst;
  int8_t condChildNum = NO_CHILD;

  const int bodyChildNum = lst.size();
  const int numBodyStmts = stmts.size();

  for (auto& stmt : stmts) {
    lst.push_back(std::move(stmt));
  }

  Local* ret = new Local(std::move(lst), condChildNum, blockStyle,
                         bodyChildNum,
                         numBodyStmts);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<Local> Local::build(Builder* builder,
                          Location loc,
                          owned<AstNode> condition,
                          BlockStyle blockStyle,
                          AstList stmts) {
  CHPL_ASSERT(condition.get() != nullptr);

  AstList lst;
  int8_t condChildNum = NO_CHILD;

  if (condition.get() != nullptr) {
    condChildNum = lst.size();
    lst.push_back(std::move(condition));
  }

  const int bodyChildNum = lst.size();
  const int numBodyStmts = stmts.size();

  for (auto& stmt : stmts) {
    lst.push_back(std::move(stmt));
  }

  Local* ret = new Local(std::move(lst), condChildNum, blockStyle,
                         bodyChildNum,
                         numBodyStmts);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
