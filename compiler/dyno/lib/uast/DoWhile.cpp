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

#include "chpl/uast/DoWhile.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<DoWhile> DoWhile::build(Builder* builder, Location loc,
                              BlockStyle blockStyle,
                              owned<Block> body,
                              owned<Expression> condition) {

  assert(condition.get() != nullptr);
  assert(body.get() != nullptr);

  ASTList lst;
  const int loopBodyChildNum = lst.size();

  lst.push_back(std::move(body));

  int conditionChildNum = lst.size();
  lst.push_back(std::move(condition));

  DoWhile* ret = new DoWhile(std::move(lst), blockStyle,
                             loopBodyChildNum,
                             conditionChildNum);

  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
