/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


/*** repositionDefExpressions.cpp
 ***
 *** This pass and function moves around the defPoints for variables to
 *** where the first parallel (PRIM_BLOCK_XMT_PRAGMA_FORALL_I_IN_N) use is.
 ***
 *** The algorithm: look at a function and collect all the variables. Push
 *** all the variables down into a parallel block if the variable isn't used
 *** at the top level nor is it used in a sibling block. Repeat for each block
 *** and sub-block.
 ***
 *** vars = collectVarsInFun(f)
 *** for each var in Vars
 ***   current_block = f->body
 ***   top_par_block = current_block
 ***   while (true)
 ***     if isTopDecl(current_block, var)
 ***       declare var in top_par_block
 ***       break
 ***     else
 ***       childBlocks = getChildBlocks(current_block)
 ***       if countBlocks(childBlocks, var) > 1
 ***         declare var in top_par_block
 ***         break
 ***       else
 ***         current_block = getUniqueBlock(childBlocks, var)
 ***         if current_block->blockInfo PRIM_BLOCK_XMT_PRAGMA_FORALL_I_IN_N
 ***           top_par_block = current_block
 ***
 *** Problem: without live variable analysis, could define variable inside
 ***          block where variable is live going in, thus writing over it
 ***          on each iteration of loop
 ***
 *** Note: algorithm changed to only consider parallel construct:
 ***       PRIM_BLOCK_XMT_PRAGMA_FORALL_I_IN_N. Therefore, this pass is only
 ***       relevant on an XMT. I've left the old code mostly extant such that
 ***       if we ever want to go back to declaring variables at the top of
 ***       every block, if, then, or else clause, it'll be easy.
 ***
 ***       The CondStmt code is in getChildBlocks() and isTopDecl().
 ***/

#include "astutil.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"

// Gets a total on the number of blocks within some vector of blocks (likely
// captured via getChildBlocks) which contain var. Useful in determining if
// a variable is used in multiple locations.
static int countBlocks(Vec<BlockStmt*>& check_vec_block, VarSymbol* var) {
  int answer = 0;
  forv_Vec(BlockStmt*, block, check_vec_block) {

    Vec<SymExpr*> symbols;
    collectSymExprs(block, symbols);
    forv_Vec(SymExpr, symbol, symbols) {
      if (toVarSymbol(symbol->var) == var) {
        answer++;
        break;
      }
    }

  }

  return answer;
}

// Gets a vector of the block statements in a given block of code. For
// example, if some code contains 2 declarations, an if statement, and
// a while loop, the resulting vector would have two elements: the if
// and the while.
static Vec<BlockStmt*> getChildBlocks(BlockStmt* check_block) {
  Vec<BlockStmt*> answer;

  for_alist(expr, check_block->body) {
    if (BlockStmt* block = toBlockStmt(expr)) {
      answer.add(block);
    } else {
      if (CondStmt* cond = toCondStmt(expr)) {
        if (BlockStmt* thenclause = toBlockStmt(cond->thenStmt)) {
          answer.add(thenclause);
        }
        if (BlockStmt* elseclause = toBlockStmt(cond->elseStmt)) {
          answer.add(elseclause);
        }
      }
    }
  }

  return answer;
}

// Returns the first block statement within a vector of blocks that
// contains a variable.
static BlockStmt* getUniqueBlock(Vec<BlockStmt*>& check_vec_block, 
                                 VarSymbol* var) {
  forv_Vec(BlockStmt*, block, check_vec_block) {

    Vec<SymExpr*> symbols;
    collectSymExprs(block, symbols);
    forv_Vec(SymExpr, symbol, symbols) {
      if (symbol->var == var) {
        return block;
      }
    }

  }

  return NULL;
}

// Returns true if a variable appears at the top level of a block. For
// example, if some code contains 2 declarations, one for x and another
// for y, an if statement, and a while loop (which contains z), this
// function would return true if called with x, but false if called with
// z.
static bool isTopDecl(BlockStmt* check_block, VarSymbol* var) {
  for_alist(expr, check_block->body) {
    Vec<SymExpr*> syms;
    if ( (!(toBlockStmt(expr))) &&
         (!(toCondStmt(expr))) ) {
      collectSymExprs(expr, syms);
    } else if (BlockStmt* new_block = toBlockStmt(expr)) {
      if (new_block->blockInfo) {
        collectSymExprs(new_block->blockInfo, syms);
      }
    } else if (CondStmt* new_cond = toCondStmt(expr)) {
      collectSymExprs(new_cond->condExpr, syms);
    }
    forv_Vec(SymExpr, sym, syms) {
      if (sym->var == var)
        return true;
    }
  }
  return false;
}

void repositionDefExpressions(void) {
  if (fNoRepositionDefExpr) return;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    Vec<DefExpr*> vars;
    Vec<DefExpr*> defs;

    // Collect all the variables defined in function
    collectDefExprs(fn, defs);
    forv_Vec(DefExpr*, def, defs) {
      if (isVarSymbol(def->sym)) {
        vars.add(def);
      }
    }

    forv_Vec(DefExpr*, def, vars) {
      BlockStmt* current_block = fn->body;
      BlockStmt* top_par_block = current_block;
      VarSymbol* var = toVarSymbol(def->sym);

      while (true) {
        if (isTopDecl(current_block, var)) {
          // declare var here
          if (!toTypeSymbol(def->sym)) {
            top_par_block->insertAtHead(def->remove());
          }

          break;
        } else {
          Vec<BlockStmt*> childBlocks = getChildBlocks(current_block);
          if (countBlocks(childBlocks, var) > 1) {
            // declare var here
            if (!toTypeSymbol(def->sym)) {
              top_par_block->insertAtHead(def->remove());
            }

            break;
          } else {
            current_block = getUniqueBlock(childBlocks, var);
            if (!current_block) break;
            if (current_block->blockInfo)
              if (current_block->blockInfo->isPrimitive(PRIM_BLOCK_XMT_PRAGMA_FORALL_I_IN_N))
                top_par_block = current_block;
          }
        }
      }

    }

    // Several copies of the noalias pragma are stuffed into a given block.
    // This removes all but one, and pulls it ahead of the block.
    Vec<CallExpr*> calls;
    BlockStmt* block = NULL;
    collectCallExprs(fn, calls);
    forv_Vec(CallExpr*, call, calls) {
      if (call->isPrimitive(PRIM_BLOCK_XMT_PRAGMA_NOALIAS)) {
        BlockStmt* current_block = toBlockStmt(call->parentExpr);
        if (block == current_block) { 
          call->remove();
        } else {
          block = current_block;
          current_block->insertBefore(call->remove());
        }
      }
    }

  }
}
