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

#include "chpl/uast/MultiDecl.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {

bool MultiDecl::isVariableDeclAndCommentList(const ASTList& list) {
  for (const auto& elt: list) {
    if (elt->isVariableDecl() || elt->isComment()) {
      // OK
    } else {
      return false;
    }
  }
  return true;
}

bool MultiDecl::contentsMatchInner(const ASTNode* other) const {
  const MultiDecl* lhs = this;
  const MultiDecl* rhs = (const MultiDecl*) other;
  return lhs->declContentsMatchInner(rhs);
}
void MultiDecl::markUniqueStringsInner(Context* context) const {
  declMarkUniqueStringsInner(context);
}

owned<MultiDecl> MultiDecl::build(Builder* builder,
                                  Location loc,
                                  ASTList varDecls) {
  MultiDecl* ret = new MultiDecl(std::move(varDecls));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
