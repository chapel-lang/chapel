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

#include "chpl/uast/OpCall.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {

bool OpCall::contentsMatchInner(const ASTNode* other) const {
  const OpCall* lhs = this;
  const OpCall* rhs = (const OpCall*) other;

  if (!lhs->callContentsMatchInner(rhs))
    return false;

  if (lhs->op_ != rhs->op_)
    return false;

  return true;
}
void OpCall::markUniqueStringsInner(Context* context) const {

  callMarkUniqueStringsInner(context);

  op_.mark(context);
}

owned<OpCall> OpCall::build(Builder* builder,
                            Location loc,
                            UniqueString op,
                            owned<Expression> lhs,
                            owned<Expression> rhs) {
  ASTList lst;

  lst.push_back(std::move(lhs));
  lst.push_back(std::move(rhs));

  OpCall* ret = new OpCall(std::move(lst), op);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}
owned<OpCall> OpCall::build(Builder* builder,
                            Location loc,
                            UniqueString op,
                            owned<Expression> expr) {
  ASTList lst;

  lst.push_back(std::move(expr));

  OpCall* ret = new OpCall(std::move(lst), op);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
