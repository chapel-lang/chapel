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

#include "chpl/uast/Implements.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


void Implements::dumpFieldsInner(const DumpSettings& s) const {
  if (isExpressionLevel_) {
    s.out << " expr";
  }
}

std::string Implements::dumpChildLabelInner(int i) const {
  if (i == typeIdentChildNum_) {
    return "type-ident";
  }
  return "";
}

UniqueString Implements::interfaceName() const {
  auto expr = interfaceExpr();
  CHPL_ASSERT(expr);

  if (auto ident = expr->toIdentifier()) {
    return ident->name();
  } else if (auto call = expr->toFnCall()) {
    auto called = call->calledExpression();
    CHPL_ASSERT(called && called->isIdentifier());
    auto ret = called->toIdentifier()->name();
    return ret;
  } else {
    CHPL_ASSERT(false && "Not possible!");
    UniqueString empty;
    return empty;
  }
}

owned<Implements>
Implements::build(Builder* builder, Location loc,
                  owned<Identifier> typeExpr,
                  owned<AstNode> interfaceExpr,
                  bool isExpressionLevel) {
  AstList children;
  int8_t typeExprChildNum = AstNode::NO_CHILD;

  if (typeExpr.get() != nullptr) {
    typeExprChildNum = children.size();
    children.push_back(std::move(typeExpr));
  }

  CHPL_ASSERT(interfaceExpr.get() != nullptr);
  CHPL_ASSERT(interfaceExpr->isIdentifier() || interfaceExpr->isFnCall());
  children.push_back(std::move(interfaceExpr));

  Implements* ret = new Implements(std::move(children), typeExprChildNum,
                                   isExpressionLevel);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
