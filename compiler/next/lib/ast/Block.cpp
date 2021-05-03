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

#include "chpl/ast/Block.h"

#include "chpl/ast/Builder.h"

namespace chpl {
namespace uast {


Block::Block(ASTList stmts) :
  Exp(asttags::Block, std::move(stmts)) {

#ifndef NDEBUG
  // check that all children are exprs (and not, say, Symbols)
  for (const ASTBase* child : this->children()) {
    assert(child->isExp());
  }
#endif
}

bool Block::contentsMatchInner(const ASTBase* other) const {
  const Block* lhs = this;
  const Block* rhs = (const Block*) other;
  return lhs->expContentsMatchInner(rhs);
}
void Block::markUniqueStringsInner(Context* context) const {
  expMarkUniqueStringsInner(context);
}

owned<Block> Block::build(Builder* builder,
                          Location loc,
                          ASTList stmts) {
  Block* ret = new Block(std::move(stmts));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
