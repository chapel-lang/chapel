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
  return lhs->expressionContentsMatchInner(rhs);
}
void Serial::markUniqueStringsInner(Context* context) const {
  expressionMarkUniqueStringsInner(context);
}

owned<Serial> Serial::build(Builder* builder,
                          Location loc,
                          ASTList stmts,
                          bool usesDo) {
  ASTList lst;
  int8_t condChildNum = -1;

  for (auto& stmt : stmts) {
    lst.push_back(std::move(stmt));
  }

  Serial* ret = new Serial(std::move(lst), condChildNum, usesDo);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<Serial> Serial::build(Builder* builder,
                          Location loc,
                          owned<Expression> condition,
                          ASTList stmts,
                          bool usesDo) {
#ifndef NDEBUG
  assert(condition.get() != nullptr);
#endif

  ASTList lst;
  int8_t condChildNum = -1;

  if (condition.get() != nullptr) {
    condChildNum = lst.size();
    lst.push_back(std::move(condition));
  }

  for (auto& stmt : stmts) {
    lst.push_back(std::move(stmt));
  }

  Serial* ret = new Serial(std::move(lst), condChildNum, usesDo);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
