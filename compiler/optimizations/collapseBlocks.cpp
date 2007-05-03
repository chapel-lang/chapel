#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"

//
// Removes unnecessary nested and/or empty block statements.
//
void collapseBlocks(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (BlockStmt* block = dynamic_cast<BlockStmt*>(ast)) {
      if (block->body->length() == 0) {
        block->remove();
        continue;
      }
      if (block->loopInfo ||
          block->blockTag == BLOCK_COBEGIN ||
          block->blockTag == BLOCK_BEGIN)
        continue;
      if (block->list) {
        for_alist(Expr, stmt, block->body)
          block->insertBefore(stmt->remove());
        block->remove();
        continue;
      }
    }
  }
}
