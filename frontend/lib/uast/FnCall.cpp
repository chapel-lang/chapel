/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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


void FnCall::dumpFieldsInner(const DumpSettings& s) const {
  Call::dumpFieldsInner(s);

  if (callUsedSquareBrackets_) {
    s.out << "[]";
  }

  int i = 0;
  for (auto name : actualNames_) {
    if (!name.isEmpty()) {
      s.out << " actual " << i << " name= " << name.str();
    }
    i++;
  }
}

owned<FnCall> FnCall::build(Builder* builder,
                            Location loc,
                            owned<AstNode> calledExpression,
                            AstList actuals,
                            std::vector<UniqueString> actualNames,
                            bool callUsedSquareBrackets) {
  AstList lst;

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
                            owned<AstNode> calledExpression,
                            AstList actuals,
                            bool callUsedSquareBrackets) {
  std::vector<UniqueString> emptyActualNames;

  return FnCall::build(builder, loc,
                       std::move(calledExpression),
                       std::move(actuals),
                       std::move(emptyActualNames),
                       callUsedSquareBrackets);
}

owned<FnCall> FnCall::build(Builder* builder,
                            Location loc,
                            owned<AstNode> calledExpression,
                            bool callUsedSquareBrackets) {
  AstList emptyActuals;
  std::vector<UniqueString> emptyActualNames;

  return FnCall::build(builder, loc,
                       std::move(calledExpression),
                       std::move(emptyActuals),
                       std::move(emptyActualNames),
                       callUsedSquareBrackets);
}


} // namespace uast
} // namespace chpl
