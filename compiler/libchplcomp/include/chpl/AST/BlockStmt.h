#ifndef CHPL_AST_BLOCKSTMT_H
#define CHPL_AST_BLOCKSTMT_H

#include "chpl/AST/Stmt.h"

#include <vector>

namespace chpl {
namespace ast {

/**
  This class represents a { } block.
 */
class BlockStmt : public Expr {
 private:
  std::vector<Expr*> body;

 public:
};

} // end namespace ast
} // end namespace chpl

#endif
