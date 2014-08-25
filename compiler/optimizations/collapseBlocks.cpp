/*
 * Copyright 2004-2014 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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


