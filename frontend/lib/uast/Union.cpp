/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/Union.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<Union> Union::build(Builder* builder, Location loc,
                          owned<AttributeGroup> attributeGroup,
                          Decl::Visibility vis,
                          Decl::Linkage linkage,
                          owned<AstNode> linkageName,
                          UniqueString name,
                          AstList interfaceExprs,
                          AstList contents) {
  AstList lst;
  int attributeGroupChildNum = NO_CHILD;
  int elementsChildNum = NO_CHILD;
  int interfaceExprChildNum = NO_CHILD;
  int numInterfaceExprs = 0;
  int numElements = contents.size();
  int linkageNameChildNum = NO_CHILD;

  if (attributeGroup.get()) {
    attributeGroupChildNum = lst.size();
    lst.push_back(std::move(attributeGroup));
  }

  if (linkageName.get()) {
    linkageNameChildNum = lst.size();
    lst.push_back(std::move(linkageName));
  }

  numInterfaceExprs = interfaceExprs.size();
  if (numInterfaceExprs > 0) {
    interfaceExprChildNum = lst.size();
    for (auto& interfaceExpr : interfaceExprs) {
      lst.push_back(std::move(interfaceExpr));
    }
  }

  elementsChildNum = lst.size();
  for (auto& ast : contents) {
    lst.push_back(std::move(ast));
  }

  Union* ret = new Union(std::move(lst), attributeGroupChildNum, vis,
                         linkage,
                         linkageNameChildNum,
                         name,
                         interfaceExprChildNum,
                         numInterfaceExprs,
                         elementsChildNum,
                         numElements);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
