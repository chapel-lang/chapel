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
  BlockStmt(ExprList stmts);

 public:
  /**
   Create and return a BlockStmt containing the passed stmts.
   */
  static owned<BlockStmt> build(Builder* builder, Location loc, ExprList stmts);

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
