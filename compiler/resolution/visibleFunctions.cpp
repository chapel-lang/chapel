/*
 * Copyright 2004-2018 Cray Inc.
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

#include "visibleFunctions.h"

#include "callInfo.h"
#include "driver.h"
#include "expr.h"
#include "map.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include <map>
#include <set>


/*
   The process of finding visible functions works with some global
   tables. The global tables map

     block id -> function name -> FnSymbol*s

   In this way, the symbols declared in each block are stored.
   These tables do not save the transitive symbols available.
   To do that, code using the table needs to go up blocks/scopes
   and explicitly consider module 'use's.

   This file also includes an optimization, where the
   symbols available to all modules (i.e. what is in ChapelStandard)
   is considered to be in a single block. This optimization
   provides a significant performance improvement for compiling 'hello'.
 */

class VisibleFunctionBlock {
public:
                                        VisibleFunctionBlock();

  Map<const char*, Vec<FnSymbol*>*>     visibleFunctions;
};

static Map<BlockStmt*, VisibleFunctionBlock*> visibleFunctionMap;
static Map<BlockStmt*, BlockStmt*>            visibilityBlockCache;

static int                                    nVisibleFunctions       = 0;

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void  buildVisibleFunctionMap();

void findVisibleFunctions(CallInfo&       info,
                          Vec<FnSymbol*>& visibleFns) {
  CallExpr* call = info.call;

  //
  // update visible function map as necessary
  //
  if (gFnSymbols.n != nVisibleFunctions) {
    buildVisibleFunctionMap();
  }

  INT_ASSERT(call->isResolved() == false);

  if (BlockStmt* block = info.scope) {
    // all functions in standard modules are stored in a single block
    if (standardModuleSet.set_in(block) != NULL) {
      block = theProgram->block;
    }

    if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(block)) {
      if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(info.name)) {
        visibleFns.append(*fns);
      }
    }

  } else {
    getVisibleFunctions(info.name, call, visibleFns);

  }

  if ((explainCallLine && explainCallMatch(call)) ||
      call->id == explainCallID) {
    USR_PRINT(call, "call: %s", info.toString());

    if (visibleFns.n == 0) {
      USR_PRINT(call, "no visible functions found");

    } else {
      bool first = true;

      forv_Vec(FnSymbol, visibleFn, visibleFns) {
        USR_PRINT(visibleFn,
                  "%s %s",
                  first ? "visible functions are:" : "                      ",
                  toString(visibleFn));
        first = false;
      }
    }
  }
}



static void buildVisibleFunctionMap() {
  for (int i = nVisibleFunctions; i < gFnSymbols.n; i++) {
    FnSymbol* fn = gFnSymbols.v[i];
    if (!fn->hasFlag(FLAG_INVISIBLE_FN) && fn->defPoint->parentSymbol && !isArgSymbol(fn->defPoint->parentSymbol)) {
      BlockStmt* block = NULL;
      if (fn->hasFlag(FLAG_AUTO_II)) {
        block = theProgram->block;
      } else {
        block = getVisibilityBlock(fn->defPoint);
        //
        // add all functions in standard modules to theProgram
        //
        if (standardModuleSet.set_in(block))
          block = theProgram->block;
      }
      VisibleFunctionBlock* vfb = visibleFunctionMap.get(block);
      if (!vfb) {
        vfb = new VisibleFunctionBlock();
        visibleFunctionMap.put(block, vfb);
      }
      Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(fn->name);
      if (!fns) {
        fns = new Vec<FnSymbol*>();
        vfb->visibleFunctions.put(fn->name, fns);
      }
      fns->add(fn);
    }
  }
  nVisibleFunctions = gFnSymbols.n;
}

/************************************* | **************************************
*                                                                             *
* Collects functions called 'name' visible in 'block' and up the visibility   *
* chain, which is defined by getVisibilityBlock().                            *
* The functions defined/visible in a block are given by 'visibleFunctionMap'. *
*                                                                             *
* 'visibilityBlockCache' maps a block to the nearest block up the visibility  *
* chain that contains any functions, according to visibleFunctionMap.         *
* In other words, it skips those blocks that do not define any visible        *
* functions (of any name).                                                    *
* Todo: some blocks only define if-functions (i.e. _if_fnNNN); such blocks    *
* probably should not be present in visibleFunctionMap.                       *
*                                                                             *
* getVisibleFunctions returns the block appropriate for visibilityBlockCache  *
* or NULL if there is none, e.g. when the next block up is the rootModule.    *
*                                                                             *
************************************** | *************************************/

static BlockStmt* getVisibleFunctions(const char*           name,
                                      CallExpr*             call,
                                      BlockStmt*            block,
                                      std::set<BlockStmt*>& visited,
                                      Vec<FnSymbol*>&       visibleFns);

void getVisibleFunctions(const char*      name,
                         CallExpr*        call,
                         Vec<FnSymbol*>&  visibleFns) {
  BlockStmt*           block    = getVisibilityBlock(call);
  std::set<BlockStmt*> visited;

  getVisibleFunctions(name, call, block, visited, visibleFns);
}

static BlockStmt* getVisibleFunctions(const char*           name,
                                      CallExpr*             call,
                                      BlockStmt*            block,
                                      std::set<BlockStmt*>& visited,
                                      Vec<FnSymbol*>&       visibleFns) {
  BlockStmt* retval = NULL;

  //
  // all functions in standard modules are stored in a single block
  //
  if (standardModuleSet.set_in(block)) {
    block = theProgram->block;
  }

  //
  // avoid infinite recursion due to modules with mutual uses
  //
  if (visited.find(block) == visited.end()) {
    bool canSkipThisBlock = true;

    if (isModuleSymbol(block->parentSymbol)) {
      visited.insert(block);
    }

    if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(block)) {
      canSkipThisBlock = false; // cannot skip if this block defines functions

      if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name)) {
        forv_Vec(FnSymbol, fn, *fns) {
          if (fn->isVisible(call) == true) {
            // isVisible checks if the function is private to its defining
            // module (and in that case, if we are under its defining module)
            // This ensures that private functions will not be used outside
            // of their proper scope.
            visibleFns.add(fn);
          }
        }
      }
    }

    if (block->useList != NULL) {
      for_actuals(expr, block->useList) {
        UseStmt* use = toUseStmt(expr);

        INT_ASSERT(use);

        if (use->skipSymbolSearch(name) == false) {
          SymExpr* se = toSymExpr(use->src);

          INT_ASSERT(se);

          if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
            // The use statement could be of an enum instead of a module,
            // but only modules can define functions.
            // cannot skip if this block uses modules
            canSkipThisBlock = false;

            if (mod->isVisible(call) == true) {
              if (use->isARename(name) == true) {
                getVisibleFunctions(use->getRename(name),
                                    call,
                                    mod->block,
                                    visited,
                                    visibleFns);
              } else {
                getVisibleFunctions(name,
                                    call,
                                    mod->block,
                                    visited,
                                    visibleFns);
              }
            }
          }
        }
      }
    }

    //
    // visibilityBlockCache contains blocks that can be skipped
    //
    if (BlockStmt* next = visibilityBlockCache.get(block)) {
      getVisibleFunctions(name, call, next, visited, visibleFns);

      retval = (canSkipThisBlock) ? next : block;

    } else if (block != rootModule->block) {
      BlockStmt* next  = getVisibilityBlock(block);
      BlockStmt* cache = getVisibleFunctions(name,
                                             call,
                                             next,
                                             visited,
                                             visibleFns);

      if (cache) {
        visibilityBlockCache.put(block, cache);
      }

      retval = (canSkipThisBlock) ? cache : block;
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* return the innermost block for searching for visible functions              *
*                                                                             *
************************************** | *************************************/

static bool isTryTokenCond(Expr* expr);

BlockStmt* getVisibilityBlock(Expr* expr) {
  if (BlockStmt* block = toBlockStmt(expr->parentExpr)) {
    if (block->blockTag == BLOCK_SCOPELESS)
      return getVisibilityBlock(block);
    else if (block->parentExpr && isTryTokenCond(block->parentExpr)) {
      // Make the visibility block of the then and else blocks of a
      // conditional using chpl__tryToken be the block containing the
      // conditional statement.  Without this, there were some cases where
      // a function gets instantiated into one side of the conditional but
      // used in both sides, then the side with the instantiation gets
      // folded out leaving expressions with no visibility block.
      // test/functions/iterators/angeles/dynamic.chpl is an example that
      // currently fails without this.
      return getVisibilityBlock(block->parentExpr);
    } else
      return block;
  } else if (expr->parentExpr) {
    return getVisibilityBlock(expr->parentExpr);
  } else if (Symbol* s = expr->parentSymbol) {
      FnSymbol* fn = toFnSymbol(s);
      if (fn && fn->instantiationPoint)
        return fn->instantiationPoint;
      else
        return getVisibilityBlock(s->defPoint);
  } else {
    INT_FATAL(expr, "Expression has no visibility block.");
    return NULL;
  }
}

//
// return true if expr is a CondStmt with chpl__tryToken as its condition
//
static bool isTryTokenCond(Expr* expr) {
  CondStmt* cond = toCondStmt(expr);

  if (!cond) return false;

  SymExpr* sym = toSymExpr(cond->condExpr);

  if (!sym) return false;

  return sym->symbol() == gTryToken;
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void visibleFunctionsClear() {
  Vec<VisibleFunctionBlock*> vfbs;

  visibleFunctionMap.get_values(vfbs);

  forv_Vec(VisibleFunctionBlock, vfb, vfbs) {
    Vec<Vec<FnSymbol*>*> vfns;

    vfb->visibleFunctions.get_values(vfns);

    forv_Vec(Vec<FnSymbol*>, vfn, vfns) {
      delete vfn;
    }

    delete vfb;
  }

  visibleFunctionMap.clear();

  visibilityBlockCache.clear();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

VisibleFunctionBlock::VisibleFunctionBlock() {

}
