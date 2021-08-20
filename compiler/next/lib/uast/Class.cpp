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

#include "chpl/uast/Class.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<Class> Class::build(Builder* builder, Location loc,
                          Decl::Visibility vis, UniqueString name,
                          owned<Expression> parentClass,
                          ASTList contents) {
  ASTList lst;
  int parentClassChildNum = -1;
  int elementsChildNum = -1;
  int numElements = 0;
  if (parentClass.get() != nullptr) {
    parentClassChildNum = lst.size();
    lst.push_back(std::move(parentClass));
  }
  numElements = contents.size();
  if (numElements > 0) {
    elementsChildNum = lst.size();
    for (auto & elt : contents) {
      lst.push_back(std::move(elt));
    }
  }

  Class* ret = new Class(std::move(lst), vis, name,
                         elementsChildNum, numElements, parentClassChildNum);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}
 

} // namespace uast
} // namespace chpl
