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

#include "chpl/uast/Reduce.h"
#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<Reduce> Reduce::build(Builder* builder,
                            Location loc,
                            owned<AstNode> lhs,
                            owned<AstNode> expr) {
  assert(expr.get() != nullptr);
  assert(lhs->isIdentifier() || lhs->isFnCall());

  AstList lst;
  lst.push_back(std::move(expr));
  lst.push_back(std::move(lhs));

  Reduce* ret = new Reduce(std::move(lst));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
