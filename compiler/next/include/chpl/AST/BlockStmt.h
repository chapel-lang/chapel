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

#ifndef CHPL_AST_BLOCKSTMT_H
#define CHPL_AST_BLOCKSTMT_H

#include "chpl/AST/Location.h"
#include "chpl/AST/Stmt.h"

namespace chpl {
namespace ast {

/**
  This class represents a { } block.
 */
class BlockStmt final : public Stmt {
 private:
  BlockStmt(ASTList stmts);
  bool contentsMatchInner(const BaseAST* other) const override;

 public:
  /**
   Create and return a BlockStmt containing the passed stmts.
   */
  static owned<BlockStmt> build(Builder* builder, Location loc, ASTList stmts);

  /**
   Return the number of statements in the block.
   */
  int numStmts() const {
    return this->numChildren();
  }
  /**
   Return the i'th statement in the block. Note that since simple expressions
   are also statements, this function returns an Expr.
   */
  const Expr* stmt(int i) const {
    const BaseAST* ast = this->child(i);
    assert(ast->isExpr());
    return (const Expr*)ast;
  }
};

} // end namespace ast
} // end namespace chpl

#endif
