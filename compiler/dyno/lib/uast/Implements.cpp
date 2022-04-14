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

#include "chpl/uast/Implements.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


UniqueString Implements::interfaceName() const {
  auto expr = interfaceExpr();
  assert(expr);

  if (auto ident = expr->toIdentifier()) {
    return ident->name();
  } else if (auto call = expr->toFnCall()) {
    auto called = call->calledExpression();
    assert(called && called->isIdentifier());
    auto ret = called->toIdentifier()->name();
    return ret;
  } else {
    assert(false && "Not possible!");
    UniqueString empty;
    return empty;
  }
}

owned<Implements>
Implements::build(Builder* builder, Location loc, 
                  owned<Identifier> typeExpr,
                  owned<AstNode> interfaceExpr,
                  bool isConstraint) {
  AstList children;
  int8_t typeExprChildNum = AstNode::NO_CHILD;

  if (typeExpr.get() != nullptr) {
    typeExprChildNum = children.size();
    children.push_back(std::move(typeExpr));
  }

  assert(interfaceExpr.get() != nullptr);
  assert(interfaceExpr->isIdentifier() || interfaceExpr->isFnCall());
  children.push_back(std::move(interfaceExpr));

  Implements* ret = new Implements(std::move(children), typeExprChildNum,
                                   isConstraint);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
