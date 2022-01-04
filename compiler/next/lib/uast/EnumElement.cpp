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


owned<EnumElement> EnumElement::build(Builder* builder, Location loc,
                                      owned<Attributes> attributes,
                                      UniqueString name,
                                      owned<Expression> initExpression) {
  ASTList lst;
  int attributesChildNum = -1;

  if (attributes.get() != nullptr) {
    attributesChildNum = lst.size();
    lst.push_back(std::move(attributes));
  }

  if (initExpression.get() != nullptr) {
    lst.push_back(std::move(initExpression));
  }

  EnumElement* ret = new EnumElement(std::move(lst), attributesChildNum,
                                     name);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<EnumElement> EnumElement::build(Builder* builder, Location loc,
                                      owned<Attributes> attributes,
                                      UniqueString name) {
  return EnumElement::build(builder, loc, std::move(attributes),
                            name,
                            /*initExpression*/ nullptr);
}
 

} // namespace uast
} // namespace chpl
