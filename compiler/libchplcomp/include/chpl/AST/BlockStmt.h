#ifndef CHPL_AST_BLOCKSTMT_H
#define CHPL_AST_BLOCKSTMT_H

#include "chpl/AST/Stmt.h"

#include <vector>

namespace chpl {
namespace ast {

/**
  This class represents a { } block.
 */
class BlockStmt final : public Expr {
 private:
  BlockStmt(ExprList stmts);

 public:
  /**
   Create and return a BlockStmt containing the passed stmts.
   */
  static owned<BlockStmt> build(Builder* builder, ExprList stmts);

  int numStmts() const {
    return this->numChildren();
  }
  const Expr* stmt(int i) const {
    return this->getChild(i);
  }
};

} // end namespace ast
} // end namespace chpl

#endif
