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

#include "chpl/uast/While.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<While> While::build(Builder* builder, Location loc,
                      owned<Expression> condition,
                      BlockStyle blockStyle,
                      owned<Block> body) {

  assert(condition.get() != nullptr);
  assert(body.get() != nullptr);

  ASTList lst;
  int8_t conditionChildNum = lst.size();

  lst.push_back(std::move(condition));

  const int loopBodyChildNum = lst.size();

  lst.push_back(std::move(body));

  While* ret = new While(std::move(lst), conditionChildNum,
                         blockStyle,
                         loopBodyChildNum);

  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
