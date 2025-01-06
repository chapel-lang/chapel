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

#include "chpl/uast/Catch.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


std::string Catch::dumpChildLabelInner(int i) const {
  if (i == errorChildNum_) {
    return "error";
  } else if (i == bodyChildNum_) {
    return "body";
  }

  return "";
}

owned<Catch> Catch::build(Builder* builder, Location loc,
                          owned<Variable> error,
                          owned<Block> body,
                          bool hasParensAroundError) {
  CHPL_ASSERT(body.get() != nullptr);

  AstList lst;
  int8_t errorChildNum = NO_CHILD;
  int8_t bodyChildNum = NO_CHILD;

  if (error.get() != nullptr) {
    errorChildNum = lst.size();
    lst.push_back(std::move(error));
  }

  CHPL_ASSERT(body.get() != nullptr);
  bodyChildNum = lst.size();
  lst.push_back(std::move(body));

  Catch* ret = new Catch(std::move(lst), errorChildNum, bodyChildNum,
                         hasParensAroundError);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
