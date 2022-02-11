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

#include "chpl/uast/Select.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<Select> Select::build(Builder* builder, Location loc,
                            owned<Expression> expr,
                            ASTList whenStmts) {
  assert(expr.get() != nullptr);

  ASTList lst;
  const int numWhenStmts = whenStmts.size();

  lst.push_back(std::move(expr));

  for (auto& ast : whenStmts) {
    assert(ast->isWhen());
    lst.push_back(std::move(ast));
  }

  Select* ret = new Select(std::move(lst), numWhenStmts);

  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
