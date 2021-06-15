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

#include "chpl/uast/Continue.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool Continue::contentsMatchInner(const ASTNode* other) const {
  const Continue* lhs = this;
  const Continue* rhs = other->toContinue();

  if (lhs->targetChildNum_ != rhs->targetChildNum_)
    return false;

  if (!lhs->expressionContentsMatchInner(rhs))
    return false;

  return true;
}

owned<Continue> Continue::build(Builder* builder, Location loc,
                                owned<Identifier> target) {
  ASTList lst;
  int8_t targetChildNum = -1;

  if (target.get() != nullptr) {
    targetChildNum = lst.size();
    lst.push_back(std::move(target));
  }

  Continue* ret = new Continue(std::move(lst), targetChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
