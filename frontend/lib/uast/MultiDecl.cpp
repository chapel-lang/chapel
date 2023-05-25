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

#include "chpl/uast/MultiDecl.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {

bool MultiDecl::isAcceptableMultiDecl() {
  for (auto elt : decls()) {
    if (elt->isVariable() || elt->isComment() || elt->isTupleDecl()) {
      // OK
    } else {
      return false;
    }
  }
  return true;
}

owned<MultiDecl> MultiDecl::build(Builder* builder, Location loc,
                                  owned<AttributeGroup> attributeGroup,
                                  Decl::Visibility vis,
                                  Decl::Linkage linkage,
                                  AstList varDecls) {
  AstList lst;
  int attributeGroupChildNum = NO_CHILD;

  if (attributeGroup.get() != nullptr) {
    attributeGroupChildNum = lst.size();
    lst.push_back(std::move(attributeGroup));
  }

  for (auto& ast : varDecls) {
    lst.push_back(std::move(ast));
  }


  MultiDecl* ret = new MultiDecl(std::move(lst), attributeGroupChildNum,
                                 vis,
                                 linkage);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
