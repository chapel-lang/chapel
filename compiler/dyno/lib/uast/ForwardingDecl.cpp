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

#include "chpl/uast/ForwardingDecl.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<ForwardingDecl> ForwardingDecl::build(Builder* builder, Location loc,
                                            owned<Attributes> attributes,
                                            owned<Expression> expr) {
  assert(expr.get() != nullptr);

  return ForwardingDecl::build(builder, loc, std::move(attributes),
                               std::move(expr),
                               Decl::DEFAULT_VISIBILITY);
}

owned<ForwardingDecl> ForwardingDecl::build(Builder* builder, Location loc,
                                            owned<Attributes> attributes,
                                            owned<Expression> expr,
                                            Decl::Visibility visibility) {
  assert(expr.get() != nullptr);
  int attributesChildNum = -1;
  ASTList lst;

  // store the attributes and the location of the attributes node if one exists
  if (attributes.get() != nullptr) {
    attributesChildNum = lst.size();
    lst.push_back(std::move(attributes));
  }

  // store the child node
  lst.push_back(std::move(expr));

  ForwardingDecl* ret = new ForwardingDecl(std::move(lst),
                                           visibility,
                                           attributesChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
