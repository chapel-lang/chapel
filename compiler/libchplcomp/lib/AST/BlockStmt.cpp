#include "chpl/AST/BlockStmt.h"

#include "chpl/AST/Builder.h"

namespace chpl {
namespace ast {


BlockStmt::BlockStmt(ExprList stmts) :
  Stmt(asttags::BlockStmt, std::move(stmts)) {
}

owned<BlockStmt> BlockStmt::build(Builder* builder,
                                  Location loc,
                                  ExprList stmts) {
  BlockStmt* ret = new BlockStmt(std::move(stmts));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace ast
} // namespace chpl
