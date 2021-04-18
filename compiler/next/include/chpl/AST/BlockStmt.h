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

  int numStmts() const {
    return this->numChildren();
  }
  const Expr* stmt(int i) const {
    const BaseAST* ast = this->child(i);
    assert(ast->isExpr());
    return (const Expr*)ast;
  }
};

} // end namespace ast
} // end namespace chpl

#endif
