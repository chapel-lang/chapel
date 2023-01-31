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

#include "chpl/uast/When.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


void When::dumpFieldsInner(const DumpSettings& s) const {
  if (isOtherwise()) {
    s.out << " otherwise";
  }
  return SimpleBlockLike::dumpFieldsInner(s);
}

owned<When> When::build(Builder* builder, Location loc,
                        AstList caseExprs,
                        BlockStyle blockStyle,
                        AstList stmts) {
  AstList lst;
  const int numCaseExprs = caseExprs.size();
  const int numBodyStmts = stmts.size();
  int bodyChildNum = NO_CHILD;

  for (auto& ast : caseExprs) {
    lst.push_back(std::move(ast));
  }

  bodyChildNum = lst.size();

  for (auto& ast : stmts) {
    lst.push_back(std::move(ast));
  }

  When* ret = new When(std::move(lst), numCaseExprs, blockStyle,
                       bodyChildNum,
                       numBodyStmts);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
