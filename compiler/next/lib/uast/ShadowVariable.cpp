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

#include "chpl/uast/ShadowVariable.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool ShadowVariable::contentsMatchInner(const ASTNode* other) const {
  const ShadowVariable* lhs = this;
  const ShadowVariable* rhs = (const ShadowVariable*) other;
  return lhs->varLikeDeclContentsMatchInner(rhs) &&
         lhs->intent_ == rhs->intent_;
}

void ShadowVariable::markUniqueStringsInner(Context* context) const {
  varLikeDeclMarkUniqueStringsInner(context);
}

owned<ShadowVariable>
ShadowVariable::build(Builder* builder, Location loc,
                      UniqueString name, 
                      ShadowVariable::Intent intent,
                      owned<Expression> typeExpression,
                      owned<Expression> initExpression) {
  ASTList lst;
  int8_t typeExpressionChildNum = -1;
  int8_t initExpressionChildNum = -1;

  if (typeExpression.get() != nullptr) {
    typeExpressionChildNum = lst.size();
    lst.push_back(std::move(typeExpression));
  }

  if (initExpression.get() != nullptr) {
    initExpressionChildNum = lst.size();
    lst.push_back(std::move(initExpression));
  }

  ShadowVariable* ret = new ShadowVariable(std::move(lst), name, intent,
                                           typeExpressionChildNum,
                                           initExpressionChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
