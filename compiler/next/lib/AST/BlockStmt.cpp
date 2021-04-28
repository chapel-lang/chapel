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

#include "chpl/AST/BlockStmt.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {


BlockStmt::BlockStmt(ASTList stmts) :
  Stmt(asttags::BlockStmt, std::move(stmts)) {

#ifndef NDEBUG
  // check that all children are exprs (and not, say, Symbols)
  for (int i = 0; i < this->numChildren(); i++) {
    assert(child(i)->isExpr());
  }
#endif
}

bool BlockStmt::contentsMatchInner(const BaseAST* other) const {
  return true;
}

owned<BlockStmt> BlockStmt::build(Builder* builder,
                                  Location loc,
                                  ASTList stmts) {
  BlockStmt* ret = new BlockStmt(std::move(stmts));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace ast
} // namespace chpl
