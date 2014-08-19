#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"

static void collapseBlock(BlockStmt* block) {
  if (block->list && !block->blockInfo) {
    for_alist(expr, block->body)
      block->insertBefore(expr->remove());
    block->remove();
  }
}

// The c for loop primitive is of the form:
//   __primitive("C for loop", {inits}, {test}, {incrs})
//
// For zippered iterators the inits and incrs can be composed of multiple
// (essentially scopeless) block statements next to each other. We want to
// collapse those into a single block (otherwise codegen would be a nightmare.)
static void collapseCForLoopBlocks(CallExpr* call) {
  if (call->isPrimitive(PRIM_BLOCK_C_FOR_LOOP)) {
    for_alist(cForExprs, call->argList) {
      if (BlockStmt* block = toBlockStmt(cForExprs)) {
        for_alist(cForExpr, block->body) {
          if (BlockStmt* innerBlock = toBlockStmt(cForExpr)) {
            collapseBlocks(innerBlock);
          }
        }
      }
    }
  }
}

void collapseBlocks(BlockStmt* block) {
  FnSymbol* fn = toFnSymbol(block->parentSymbol);
  if (fn) {
    INT_ASSERT(fn->body == block); // for correctness of assignment at the end
    block->remove();
  }
  Vec<Expr*> exprs;
  collect_stmts(block, exprs);
  forv_Vec(Expr, expr, exprs) {
    if (CallExpr* call = toCallExpr(expr)) {
      collapseCForLoopBlocks(call);
    } else if (BlockStmt* block = toBlockStmt(expr)) {
      collapseBlock(block);
    }
  }
  if (fn) {
    fn->body = block;
    insert_help(block, NULL, fn);
  }
  removeDeadIterResumeGotos();
}


