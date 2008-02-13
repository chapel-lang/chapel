#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"

void collapseBlocks(BlockStmt* block) {
  FnSymbol* fn = toFnSymbol(block->parentSymbol);
  if (fn)
    block->remove();
  Vec<Expr*> exprs;
  collect_stmts(&exprs, block);
  forv_Vec(Expr, expr, exprs) {
    if (BlockStmt* block = toBlockStmt(expr)) {
      if (block->list &&
          !block->loopInfo &&
          block->blockTag != BLOCK_ON &&
          block->blockTag != BLOCK_BEGIN) {
        for_alist(expr, block->body)
          block->insertBefore(expr->remove());
        block->remove();
      }
    }
  }
  if (fn) {
    fn->body = block;
    insert_help(block, NULL, fn, fn->argScope);
  }
}
