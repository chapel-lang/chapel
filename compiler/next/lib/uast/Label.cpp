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

#include "chpl/uast/Label.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {

bool Label::contentsMatchInner(const ASTNode* other) const {
  const Label* lhs = this;
  const Label* rhs = other->toLabel();

  if (lhs->name_ != rhs->name_)
    return false;

  if (!lhs->simpleBlockLikeContentsMatchInner(rhs))
    return false;

  return true;
}

void Label::markUniqueStringsInner(Context* context) const {
  name_.mark(context);
  simpleBlockLikeMarkUniqueStringsInner(context);
}

owned<Label> Label::build(Builder* builder, Location loc, UniqueString name,
                          BlockStyle blockStyle,
                          ASTList stmts) {
  const int bodyChildNum = 0;
  const int numBodyStmts = stmts.size();

  Label* ret = new Label(std::move(stmts), name, blockStyle,
                         bodyChildNum,
                         numBodyStmts);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
