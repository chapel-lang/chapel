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

#include "chpl/uast/EnumDecl.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool EnumDecl::contentsMatchInner(const ASTNode* other) const {
  const EnumDecl* lhs = this;
  const EnumDecl* rhs = (const EnumDecl*) other;
  return lhs->declContentsMatchInner(rhs);
}
void EnumDecl::markUniqueStringsInner(Context* context) const {
  return declMarkUniqueStringsInner(context);
}

owned<EnumDecl>
EnumDecl::build(Builder* builder, Location loc,
                UniqueString name, Sym::Visibility vis,
                ASTList stmts) {
  Enum* sym = new Enum(std::move(stmts), name, vis);
  builder->noteLocation(sym, loc);
  EnumDecl* ret = new EnumDecl(toOwned(sym));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}
 

} // namespace uast
} // namespace chpl
