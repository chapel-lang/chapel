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
