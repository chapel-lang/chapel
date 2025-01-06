/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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


void Try::dumpFieldsInner(const DumpSettings& s) const {
  if (isTryBang_) {
    s.out << " try!";
  }
  if (isExpressionLevel_) {
    s.out << " expr";
  }
}

std::string Try::dumpChildLabelInner(int i) const {
  if (i == bodyChildNum_) {
    return "body";
  } else if (numBodyStmts_ <= i && i < numBodyStmts_ + numHandlers_) {
    return "handler " + std::to_string(i - numBodyStmts_);
  }
  return "";
}

owned<Try> Try::build(Builder* builder, Location loc,
                      owned<Block> body,
                      AstList catches,
                      bool isTryBang) {
  AstList lst;
  int numHandlers = catches.size();
  bool containsBlock = true;
  bool isExpressionLevel = false;

  lst.push_back(std::move(body));

  for (auto& ast : catches) {
    CHPL_ASSERT(ast->isCatch());
    lst.push_back(std::move(ast));
  }

  Try* ret = new Try(std::move(lst),
                     numHandlers,
                     containsBlock,
                     isExpressionLevel,
                     isTryBang);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<Try> Try::build(Builder* builder, Location loc,
                      owned<AstNode> expr,
                      bool isTryBang,
                      bool isExpressionLevel) {
  AstList lst;
  int numHandlers = 0;
  bool containsBlock = false;

  lst.push_back(std::move(expr));

  Try* ret = new Try(std::move(lst),
                     numHandlers,
                     containsBlock,
                     isExpressionLevel,
                     isTryBang);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
