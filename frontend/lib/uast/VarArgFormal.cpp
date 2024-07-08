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

#include "chpl/uast/VarArgFormal.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


std::string VarArgFormal::dumpChildLabelInner(int i) const {
  if (i == countChildNum_) {
    return "count";
  }

  return VarLikeDecl::dumpChildLabelInner(i);
}

owned<VarArgFormal> VarArgFormal::build(Builder* builder, Location loc,
                                        owned<AttributeGroup> attributeGroup,
                                        UniqueString name,
                                        Formal::Intent intent,
                                        owned<AstNode> typeExpression,
                                        owned<AstNode> count) {
  AstList lst;
  int attributeGroupChildNum = NO_CHILD;
  int8_t typeExpressionChildNum = NO_CHILD;
  int8_t countChildNum = NO_CHILD;

  if (attributeGroup.get() != nullptr) {
    attributeGroupChildNum = lst.size();
    lst.push_back(std::move(attributeGroup));
  }

  if (typeExpression.get() != nullptr) {
    typeExpressionChildNum = lst.size();
    lst.push_back(std::move(typeExpression));
  }

  if (count.get() != nullptr) {
    countChildNum = lst.size();
    lst.push_back(std::move(count));
  }

  VarArgFormal* ret = new VarArgFormal(std::move(lst), attributeGroupChildNum,
                                       name,
                                       intent,
                                       typeExpressionChildNum,
                                       countChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
