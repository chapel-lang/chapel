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

#include "chpl/uast/Record.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<Record> Record::build(Builder* builder, Location loc,
                            Decl::Visibility vis,
                            Decl::Linkage linkage,
                            owned<Expression> linkageName,
                            UniqueString name,
                            ASTList contents) {
  ASTList lst;
  int elementsChildNum = -1;
  int numElements = contents.size();
  int linkageNameChildNum = -1;

  if (linkageName.get()) {
    linkageNameChildNum = lst.size();
    lst.push_back(std::move(linkageName));
  }

  elementsChildNum = lst.size();
  for (auto& ast : contents) {
    lst.push_back(std::move(ast));
  }

  Record* ret = new Record(std::move(lst), vis, linkage,
                           linkageNameChildNum,
                           name,
                           elementsChildNum,
                           numElements);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}
 

} // namespace uast
} // namespace chpl
