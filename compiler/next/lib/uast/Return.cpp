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

#include "chpl/uast/Return.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool Return::contentsMatchInner(const ASTNode* other) const {
  const Return* lhs = this;
  const Return* rhs = (const Return*) other;

  if (lhs->valueChildNum_ != rhs->valueChildNum_)
    return false;

  if (!lhs->expressionContentsMatchInner(rhs))
    return false;

  return true;
}

owned<Return> Return::build(Builder* builder, Location loc,
                            owned<Expression> value) {
  ASTList lst;
  int8_t valueChildNum = -1;

  if (value.get() != nullptr) {
    valueChildNum = lst.size();
    lst.push_back(std::move(value));
  }

  Return* ret = new Return(std::move(lst), valueChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
