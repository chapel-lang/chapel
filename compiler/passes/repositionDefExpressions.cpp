/*** repositionDefExpressions.cpp
 ***
 *** This pass and function moves around the defPoints for variables to
 *** where the first use is.
 ***
 *** The algorithm: look at a function and collect all the variables. Push
 *** all the variables down into a block if the variable isn't used at the
 *** top level nor is it used in a sibling block. Repeat for each block and
 *** sub-block.
 ***
 *** vars = collectVarsInFun(f);
 *** for each var in Vars
 ***   current_block = f->body;
 ***   while (true)
 ***     if isTopDecl(current_block, var)
 ***       declare var in current_block
 ***       break
 ***     else
 ***       childBlocks = getChildBlocks(current_block)
 ***       if countBlocks(childBlocks, var) > 1
 ***         declare var in current_block
 ***         break
 ***       else
 ***         current_block = getUniqueBlock(childBlocks, var)
 ***
 *** Problem: without live variable analysis, could define variable inside
 ***          block where variable is live going in, thus writing over it
 ***          on each iteration of loop
 ***/

#include "astutil.h"
#include "stmt.h"
#include "symbol.h"

// Gets a total on the number of blocks within some vector of blocks (likely
// captured via getChildBlocks) which contain var. Useful in determining if
// a variable is used in multiple locations.
int countBlocks(Vec<BlockStmt*>& check_vec_block, VarSymbol* var) {
  int answer = 0;
  forv_Vec(BlockStmt*, block, check_vec_block) {

    Vec<SymExpr*> symbols;
    collectSymExprs(block, symbols);
    forv_Vec(SymExpr, symbol, symbols) {
      if (toVarSymbol(symbol->var) == var) {
        answer++;
      }
    }

  }

  return answer;
}

// Gets a vector of the block statements in a given block of code. For
// example, if some code contains 2 declarations, an if statement, and
// a while loop, the resulting vector would have two elements: the if
// and the while.
Vec<BlockStmt*> getChildBlocks(BlockStmt* check_block) {
  Vec<BlockStmt*> answer;

  for_alist(expr, check_block->body) {
    if (BlockStmt* block = toBlockStmt(expr)) {
      answer.add(block);
    }
  }

  return answer;
}

// Returns the first block statement within a vector of blocks that
// contains a variable.
BlockStmt* getUniqueBlock(Vec<BlockStmt*>& check_vec_block, VarSymbol* var) {
  forv_Vec(BlockStmt*, block, check_vec_block) {

    Vec<Symbol*> symbols;
    collectSymbols(block, symbols);
    forv_Vec(Symbol, symbol, symbols) {
      if (toVarSymbol(symbol) == var) {
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
bool isTopDecl(BlockStmt* check_block, VarSymbol* var) {
  for_alist(expr, check_block->body) {
    if (!(toBlockStmt(expr))) {
      Vec<Symbol*> syms;
      collectSymbols(expr, syms);
      forv_Vec(Symbol, sym, syms) {
        if (toVarSymbol(sym) == var)
          return true;
      }
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
      VarSymbol* var = toVarSymbol(def->sym);

      while (true) {
        if (isTopDecl(current_block, var)) {
          // declare var here
          if (!toTypeSymbol(def->sym)) {
            current_block->insertAtHead(def->remove());
          }

          break;
        } else {
          Vec<BlockStmt*> childBlocks = getChildBlocks(current_block);
          if (countBlocks(childBlocks, var) > 1) {
            // declare var here
            if (!toTypeSymbol(def->sym)) {
              current_block->insertAtHead(def->remove());
            }

            break;
          } else {
            current_block = getUniqueBlock(childBlocks, var);
          }
        }
      }

    }

  }

}
