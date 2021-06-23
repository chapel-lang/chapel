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

#include "chpl/uast/Module.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


bool Module::contentsMatchInner(const ASTNode* other) const {
  const Module* lhs = this;
  const Module* rhs = (const Module*) other;
  return lhs->namedDeclContentsMatchInner(rhs) &&
         lhs->kind_ == rhs->kind_;
}
void Module::markUniqueStringsInner(Context* context) const {
  namedDeclMarkUniqueStringsInner(context);
}

owned<Module>
Module::build(Builder* builder, Location loc,
              UniqueString name, Decl::Visibility vis,
              Module::Kind kind, ASTList stmts) {

  Module* ret = new Module(std::move(stmts), vis, name, kind);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
