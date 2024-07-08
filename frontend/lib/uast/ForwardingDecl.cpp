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

#include "chpl/uast/ForwardingDecl.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<ForwardingDecl> ForwardingDecl::build(Builder* builder, Location loc,
                                            owned<AttributeGroup> attributeGroup,
                                            owned<AstNode> expr) {
  CHPL_ASSERT(expr.get() != nullptr);

  return ForwardingDecl::build(builder, loc, std::move(attributeGroup),
                               std::move(expr),
                               Decl::DEFAULT_VISIBILITY);
}

owned<ForwardingDecl> ForwardingDecl::build(Builder* builder, Location loc,
                                            owned<AttributeGroup> attributeGroup,
                                            owned<AstNode> expr,
                                            Decl::Visibility visibility) {
  CHPL_ASSERT(expr.get() != nullptr);
  int attributeGroupChildNum = NO_CHILD;
  AstList lst;

  // store the attributeGroup and the location of the attributeGroup node
  if (attributeGroup.get() != nullptr) {
    attributeGroupChildNum = lst.size();
    lst.push_back(std::move(attributeGroup));
  }

  // store the child node
  lst.push_back(std::move(expr));

  ForwardingDecl* ret = new ForwardingDecl(std::move(lst),
                                           visibility,
                                           attributeGroupChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
