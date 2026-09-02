/*
 * Copyright 2021-2026 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/MatchCase.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


void MatchCase::dumpFieldsInner(const DumpSettings& s) const {
  if (isOtherwise()) {
    s.out << " otherwise";
  }
  return AstNode::dumpFieldsInner(s);
}

owned<MatchCase> MatchCase::build(Builder* builder, Location loc,
                                  owned<AstNode> caseExpr,
                                  BlockStyle blockStyle,
                                  AstList stmts) {
  AstList lst;

  int caseExprIdx = -1;
  if (caseExpr != nullptr) {
    caseExprIdx = 0;
    lst.push_back(std::move(caseExpr));
  }

  auto bodyBlock = Block::build(builder,loc,std::move(stmts));
  lst.push_back(std::move(bodyBlock));

  MatchCase* ret = new MatchCase(std::move(lst), caseExprIdx, blockStyle);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
