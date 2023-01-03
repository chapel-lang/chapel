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

#include "chpl/uast/OpCall.h"

#include "chpl/uast/Builder.h"
#include "chpl/framework/UniqueString.h"

namespace chpl {
namespace uast {


void OpCall::dumpFieldsInner(const DumpSettings& s) const {
  s.out << " " << op_.str();
  Call::dumpFieldsInner(s);
}

owned<OpCall> OpCall::build(Builder* builder,
                            Location loc,
                            UniqueString op,
                            owned<AstNode> lhs,
                            owned<AstNode> rhs) {
  AstList list;

  list.push_back(std::move(lhs));
  list.push_back(std::move(rhs));

  OpCall* ret = new OpCall(std::move(list), op);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}
owned<OpCall> OpCall::build(Builder* builder,
                            Location loc,
                            UniqueString op,
                            owned<AstNode> expr) {
  AstList list;

  list.push_back(std::move(expr));

  OpCall* ret = new OpCall(std::move(list), op);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

bool isOpName(UniqueString name) {
  return name == USTR("=") || name == USTR("==") || name == USTR("!=") ||
         name == USTR(">") || name == USTR(">=") || name == USTR("<") ||
         name == USTR("<=") || name == USTR("<=>") || name == USTR("&") ||
         name == USTR("|") || name == USTR("^") || name == USTR("~") ||
         name == USTR("+") || name == USTR("-") || name == USTR("*") ||
         name == USTR("/") || name == USTR("<<") || name == USTR(">>") ||
         name == USTR("%") || name == USTR("**") || name == USTR("!") ||
         name == USTR("+=") || name == USTR("-=") ||
         name == USTR("*=") || name == USTR("/=") || name == USTR("%=") ||
         name == USTR("**=") || name == USTR("&=") || name == USTR("|=") ||
         name == USTR("^=") || name == USTR(">>=") || name == USTR("<<=") ||
         name == USTR("#") || name == USTR("chpl_by") || name == USTR("by") ||
         name == USTR("align") || name == USTR("chpl_align") ||
         name == USTR(":");
}


} // namespace uast
} // namespace chpl
