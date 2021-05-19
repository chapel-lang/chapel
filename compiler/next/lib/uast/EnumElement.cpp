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

#include "chpl/uast/EnumElement.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool EnumElement::contentsMatchInner(const ASTNode* other) const {
  const EnumElement* lhs = this;
  const EnumElement* rhs = (const EnumElement*) other;
  return lhs->namedDeclContentsMatchInner(rhs);
}
void EnumElement::markUniqueStringsInner(Context* context) const {
  namedDeclMarkUniqueStringsInner(context);
}

owned<EnumElement> EnumElement::build(Builder* builder, Location loc,
                                      UniqueString name,
                                      owned<Expression> initExpression) {
  ASTList lst;
  if (initExpression.get() != nullptr) {
    lst.push_back(std::move(initExpression));
  }
  EnumElement* ret = new EnumElement(std::move(lst), name);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}
owned<EnumElement>
EnumElement::build(Builder* builder, Location loc, UniqueString name) {
  owned<Expression> empty;
  return EnumElement::build(builder, loc, name, std::move(empty));
}
 

} // namespace uast
} // namespace chpl
