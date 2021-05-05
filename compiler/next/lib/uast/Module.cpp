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


Module::Module(ASTList children, UniqueString name, Sym::Visibility vis,
               Module::Tag tag)
  : Sym(asttags::Module, std::move(children), name, vis), tag_(tag) {

#ifndef NDEBUG
  // check that all children are exprs (and not, say, Syms)
  for (const ASTNode* child : this->children()) {
    assert(child->isExpression());
  }
#endif
}

bool Module::contentsMatchInner(const ASTNode* other) const {
  const Module* lhs = this;
  const Module* rhs = (const Module*) other;
  return lhs->symContentsMatchInner(rhs) &&
         lhs->tag_ == rhs->tag_;
}
void Module::markUniqueStringsInner(Context* context) const {
  symMarkUniqueStringsInner(context);
}


} // namespace uast
} // namespace chpl
