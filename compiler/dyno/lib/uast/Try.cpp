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

#include "chpl/uast/Try.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<Try> Try::build(Builder* builder, Location loc, AstList stmts,
                      AstList catches,
                      bool isTryBang) {
  AstList lst;
  int numBodyStmts = stmts.size();
  int numHandlers = catches.size();
  bool isExpressionLevel = false;

  for (auto& ast : stmts) {
    lst.push_back(std::move(ast));
  }

  for (auto& ast : catches) {
    assert(ast->isCatch());
    lst.push_back(std::move(ast));
  }

  Try* ret = new Try(std::move(lst), numBodyStmts, numHandlers,
                     isExpressionLevel,
                     isTryBang);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<Try> Try::build(Builder* builder, Location loc,
                      owned<Expression> expr,
                      bool isTryBang,
                      bool isExpressionLevel) {
  AstList lst;
  int numBodyStmts = 1;
  int numHandlers = 0;

  lst.push_back(std::move(expr));

  Try* ret = new Try(std::move(lst), numBodyStmts, numHandlers,
                     isExpressionLevel,
                     isTryBang);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
