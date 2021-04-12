#include "chpl/AST/BlockStmt.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {

BlockStmt::BlockStmt(ExprList stmts) :
  Expr(asttags::BlockStmt, std::move(stmts)) {
}

owned<BlockStmt> BlockStmt::build(Builder* builder, ExprList stmts) {
  return toOwned(new BlockStmt(std::move(stmts)));
}

} // namespace ast
} // namespace chpl
