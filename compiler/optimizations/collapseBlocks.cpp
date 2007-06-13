#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"

//
// Removes unnecessary nested and/or empty block statements.
//
void collapseBlocksHelp(Expr* outer, BlockStmt* inner) {
  for_alist(Expr, expr, inner->body) {
    if (BlockStmt* block = dynamic_cast<BlockStmt*>(expr)) {
      if (block->body->length() == 0) {
        block->remove();
      } else if (block->list &&
                 !block->loopInfo &&
                 block->blockTag != BLOCK_COBEGIN &&
                 block->blockTag != BLOCK_BEGIN) {
        collapseBlocksHelp(outer, block);
        block->remove();
      } else {
        collapseBlocks(block);
        outer->insertBefore(block->remove());
      }
    } else {
      if (CondStmt* cond = dynamic_cast<CondStmt*>(expr)) {
        if (cond->thenStmt) collapseBlocks(cond->thenStmt);
        if (cond->elseStmt) collapseBlocks(cond->elseStmt);
      }
      outer->insertBefore(expr->remove());
    }
  }
}

void collapseBlocks(BlockStmt* block) {
  for_alist(Expr, expr, block->body) {
    if (BlockStmt* block = dynamic_cast<BlockStmt*>(expr)) {
      if (block->body->length() == 0) {
        block->remove();
      } else if (block->list &&
                 !block->loopInfo &&
                 block->blockTag != BLOCK_COBEGIN &&
                 block->blockTag != BLOCK_BEGIN) {
        collapseBlocksHelp(block, block);
        block->remove();
      } else {
        collapseBlocks(block);
      }
    } else if (CondStmt* cond = dynamic_cast<CondStmt*>(expr)) {
      if (cond->thenStmt) collapseBlocks(cond->thenStmt);
      if (cond->elseStmt) collapseBlocks(cond->elseStmt);
    }
  }
}
