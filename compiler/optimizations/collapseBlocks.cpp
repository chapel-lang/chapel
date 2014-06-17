/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"

void collapseBlocks(BlockStmt* block) {
  FnSymbol* fn = toFnSymbol(block->parentSymbol);
  if (fn) {
    INT_ASSERT(fn->body == block); // for correctness of assignment at the end
    block->remove();
  }
  Vec<Expr*> exprs;
  collect_stmts(block, exprs);
  forv_Vec(Expr, expr, exprs) {
    if (BlockStmt* block = toBlockStmt(expr)) {
      if (block->list &&
          !block->blockInfo) {
        for_alist(expr, block->body)
          block->insertBefore(expr->remove());
        block->remove();
      }
    }
  }
  if (fn) {
    fn->body = block;
    insert_help(block, NULL, fn);
  }
  removeDeadIterResumeGotos();
}
