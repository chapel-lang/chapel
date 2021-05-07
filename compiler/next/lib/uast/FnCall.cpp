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

#include "chpl/uast/FnCall.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {

bool FnCall::contentsMatchInner(const ASTNode* other) const {
  const FnCall* lhs = this;
  const FnCall* rhs = (const FnCall*) other;

  if (!lhs->callContentsMatchInner(rhs))
    return false;

  if (lhs->callUsedSquareBrackets_ != rhs->callUsedSquareBrackets_ ||
      lhs->actualNames_.size() != rhs->actualNames_.size())
    return false;

  size_t nActualNames = lhs->actualNames_.size();
  for (int i = 0; i < nActualNames; i++) {
    if (lhs->actualNames_[i] != rhs->actualNames_[i])
      return false;
  }

  return true;
}
void FnCall::markUniqueStringsInner(Context* context) const {

  callMarkUniqueStringsInner(context);

  for (const auto& str : actualNames_) {
    str.mark(context);
  }
}

owned<FnCall> FnCall::build(Builder* builder,
                            Location loc,
                            owned<Expression> calledExpression,
                            ASTList actuals,
                            std::vector<UniqueString> actualNames,
                            bool callUsedSquareBrackets) {
  ASTList lst;

  lst.push_back(std::move(calledExpression));
  for (auto& actual : actuals) {
    lst.push_back(std::move(actual));
  }
  actuals.clear();

  FnCall* ret = new FnCall(std::move(lst), std::move(actualNames),
                           callUsedSquareBrackets);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}
owned<FnCall> FnCall::build(Builder* builder,
                            Location loc,
                            owned<Expression> calledExpression,
                            ASTList actuals,
                            bool callUsedSquareBrackets) {
  std::vector<UniqueString> emptyActualNames;

  return FnCall::build(builder, loc,
                       std::move(calledExpression),
                       std::move(actuals),
                       std::move(emptyActualNames),
                       callUsedSquareBrackets);
}


} // namespace uast
} // namespace chpl
