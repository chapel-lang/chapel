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

#include "chpl/uast/Serial.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool Serial::contentsMatchInner(const ASTNode* other) const {
  const Serial* lhs = this;
  const Serial* rhs = (const Serial*) other;

  if (lhs->condChildNum_ != rhs->condChildNum_)
    return false;

  if (lhs->blockStyle_ != rhs->blockStyle_)
    return false;

  if (!lhs->expressionContentsMatchInner(rhs))
    return false;

  return true;
}

void Serial::markUniqueStringsInner(Context* context) const {
  expressionMarkUniqueStringsInner(context);
}

owned<Serial> Serial::build(Builder* builder,
                          Location loc,
                          BlockStyle blockStyle,
                          ASTList stmts) {

  ASTList lst;
  int8_t condChildNum = -1;

  for (auto& stmt : stmts) {
    lst.push_back(std::move(stmt));
  }

  Serial* ret = new Serial(std::move(lst), condChildNum, blockStyle);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<Serial> Serial::build(Builder* builder,
                          Location loc,
                          owned<Expression> condition,
                          BlockStyle blockStyle,
                          ASTList stmts) {
  assert(condition.get() != nullptr);

  ASTList lst;
  int8_t condChildNum = -1;

  if (condition.get() != nullptr) {
    condChildNum = lst.size();
    lst.push_back(std::move(condition));
  }

  for (auto& stmt : stmts) {
    lst.push_back(std::move(stmt));
  }

  Serial* ret = new Serial(std::move(lst), condChildNum, blockStyle);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
