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

#include "chpl/uast/Dot.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {

bool Dot::contentsMatchInner(const ASTNode* other) const {
  const Dot* lhs = this;
  const Dot* rhs = (const Dot*) other;

  if (lhs->fieldName_ != rhs->fieldName_)
    return false;

  if (!lhs->callContentsMatchInner(rhs))
    return false;

  return true;
}
void Dot::markUniqueStringsInner(Context* context) const {

  callMarkUniqueStringsInner(context);

  fieldName_.mark(context);
}

owned<Dot> Dot::build(Builder* builder,
                      Location loc,
                      owned<Expression> receiver,
                      UniqueString fieldName) {
  ASTList list;

  list.push_back(std::move(receiver));

  Dot* ret = new Dot(std::move(list), fieldName);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
