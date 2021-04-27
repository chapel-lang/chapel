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

#include "chpl/AST/ModuleDecl.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace uast {


ModuleDecl::ModuleDecl(owned<Module> module)
  : Decl(asttags::ModuleDecl, std::move(module)) {
}

bool ModuleDecl::contentsMatchInner(const ASTBase* other) const {
  const ModuleDecl* lhs = this;
  const ModuleDecl* rhs = (const ModuleDecl*) other;
  return lhs->declContentsMatchInner(rhs);
}

owned<ModuleDecl>
ModuleDecl::build(Builder* builder, Location loc,
                  UniqueString name, Sym::Visibility vis,
                  Module::Tag tag, ASTList stmts) {

  Module* sym = new Module(std::move(stmts), name, vis, tag);
  ModuleDecl* ret = new ModuleDecl(toOwned(sym));
  builder->noteLocation(sym, loc);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
