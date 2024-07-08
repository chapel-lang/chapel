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

#include "chpl/uast/Let.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


std::string Let::dumpChildLabelInner(int i) const {
  if (0 <= i && i < numDecls_) {
    return "decl " + std::to_string(i);
  } else if (i == numDecls_) {
    return "expr";
  }

  return "";
}

owned<Let> Let::build(Builder* builder, Location loc, AstList decls,
                      owned<AstNode> expression) {
  CHPL_ASSERT(decls.size() >= 1);
  CHPL_ASSERT(expression.get() != nullptr);

  const int numDecls = decls.size();
  AstList children = std::move(decls);

  children.push_back(std::move(expression));

  Let* ret = new Let(std::move(children), numDecls);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
