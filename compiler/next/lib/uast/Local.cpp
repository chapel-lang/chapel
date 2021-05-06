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

#include "chpl/uast/Local.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


Local::Local(ASTList stmts, bool exprExists, bool usesDo) :
    Expression(asttags::Local, std::move(stmts)),
    exprExists_(exprExists),
    usesDo_(usesDo) {

#ifndef NDEBUG
  // check that all children are exprs (and not, say, Symbols)
  for (const ASTNode* child : this->children()) {
    assert(child->isExpression());
  }
#endif
}

bool Local::contentsMatchInner(const ASTNode* other) const {
  const Local* lhs = this;
  const Local* rhs = (const Local*) other;
  return lhs->expressionContentsMatchInner(rhs);
}
void Local::markUniqueStringsInner(Context* context) const {
  expressionMarkUniqueStringsInner(context);
}

owned<Local> Local::build(Builder* builder,
                          Location loc,
                          owned<Expression> expr,
                          ASTList stmts,
                          bool usesDo) {
  ASTList lst;
  bool exprExists = expr.get() != nullptr;

  if (exprExists) {
    ASTNode* ptr = expr.release();
    lst.push_back(std::move(toOwned(ptr)));
  }

  for (auto& stmt : stmts) {
    lst.push_back(std::move(toOwned(stmt.release())));
  }

  Local* ret = new Local(std::move(lst), exprExists, usesDo);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
