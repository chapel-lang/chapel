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

#include "chpl/uast/VariableDecl.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


VariableDecl::VariableDecl(owned<Variable> variable)
  : Decl(asttags::VariableDecl, std::move(variable)) {
}

bool VariableDecl::contentsMatchInner(const ASTNode* other) const {
  const VariableDecl* lhs = this;
  const VariableDecl* rhs = (const VariableDecl*) other;
  return lhs->declContentsMatchInner(rhs);
}
void VariableDecl::markUniqueStringsInner(Context* context) const {
  return declMarkUniqueStringsInner(context);
}

owned<VariableDecl>
VariableDecl::build(Builder* builder, Location loc,
                    UniqueString name, Sym::Visibility vis,
                    Variable::Tag tag,
                    owned<Expression> typeExpression,
                    owned<Expression> initExpression) {
  ASTList lst;
  int8_t typeExpressionChildNum = -1;
  int8_t initExpressionChildNum = -1;
  if (typeExpression.get() != nullptr) {
    lst.push_back(std::move(typeExpression));
    typeExpressionChildNum = lst.size() - 1;
  }
  if (initExpression.get() != nullptr) {
    lst.push_back(std::move(initExpression));
    initExpressionChildNum = lst.size() - 1;
  }
  Variable* sym = new Variable(std::move(lst), name, vis, tag,
                               typeExpressionChildNum, initExpressionChildNum);
  builder->noteLocation(sym, loc);
  VariableDecl* ret = new VariableDecl(toOwned(sym));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
