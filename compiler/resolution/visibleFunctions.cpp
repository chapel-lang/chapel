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
    if (!fn->hasFlag(FLAG_INVISIBLE_FN) && fn->inTree() && !isArgSymbol(fn->defPoint->parentSymbol)) {
      BlockStmt* block = NULL;
      if (fn->hasFlag(FLAG_AUTO_II)) {
        block = theProgram->block;
      } else {
        block = getVisibilityBlock(fn->defPoint, false);
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
************************************** | *************************************/

static void getVisibleFunctions(const char*           name,
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

static void getVisibleFunctions(const char*           name,
                                CallExpr*             call,
                                BlockStmt*            block,
                                std::set<BlockStmt*>& visited,
                                Vec<FnSymbol*>&       visibleFns) {
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

    bool moduleBlock = false;
    ModuleSymbol* inMod = toModuleSymbol(block->parentSymbol);
    if (inMod && block == inMod->block)
      moduleBlock = true;
    bool fnBlock = false;
    FnSymbol* inFn = toFnSymbol(block->parentSymbol);
    if (inFn && (block == inFn->body || block == inFn->where))
      fnBlock = true;


    if (call->id == breakOnResolveID) {
      if (moduleBlock)
        printf("visible fns: block %i visiting module %s %s:%i\n", block->id,
            inMod->name, block->fname(), block->linenum());
      else if (fnBlock) {
        if (inFn->instantiatedFrom)
          printf("visible fns: block %i visiting fn %s %s:%i insn from block id %i\n", block->id,
               inFn->name, block->fname(), block->linenum(),
               inFn->instantiatedFrom->id);
        else
          printf("visible fns: block %i visiting fn %s %s:%i\n", block->id,
               inFn->name, block->fname(), block->linenum());
      }
      else
        printf("visible fns: block %i visiting block %s:%i\n", block->id,
              block->fname(), block->linenum());
    }

    /*if (moduleBlock || fnBlock)  -- see comment below*/
    {

      // e.g. in associative.chpl primer, instatiation occurs in a
      // block that isn't a fn or module block.
      // But, we could probably optimize by knowing:
      //   - which blocks are irrelevant
      //   - which blocks are traversed as outer blocks
      visited.insert(block);
    }

    if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(block)) {
      // note, defines functions

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
      // note, uses other modules
      for_actuals(expr, block->useList) {
        UseStmt* use = toUseStmt(expr);

        INT_ASSERT(use);

        if (use->skipSymbolSearch(name) == false) {
          SymExpr* se = toSymExpr(use->src);

          INT_ASSERT(se);

          if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
            // The use statement could be of an enum instead of a module,
            // but only modules can define functions.

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

    if (block != rootModule->block) {
      BlockStmt* next  = getVisibilityBlock(block, false);

      // Recurse in the enclosing block
      getVisibleFunctions(name, call, next, visited, visibleFns);

      if (fnBlock) {
        FnSymbol* fn = toFnSymbol(block->parentSymbol);
        if (fn->instantiationPoint && fn->instantiationPoint->parentSymbol) {
          // Also look at the instantiation point
          BlockStmt* next = fn->instantiationPoint;
          getVisibleFunctions(name, call, next, visited, visibleFns);
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* return the innermost block for searching for visible functions              *
*                                                                             *
************************************** | *************************************/

static bool isTryTokenCond(Expr* expr);

BlockStmt* getVisibilityBlock(Expr* expr, bool usePtOfInsn) {
  if (BlockStmt* block = toBlockStmt(expr->parentExpr)) {
    if (block->blockTag == BLOCK_SCOPELESS ||
        block->blockTag == BLOCK_TYPE)
      return getVisibilityBlock(block, usePtOfInsn);
    else if (block->parentExpr && isTryTokenCond(block->parentExpr)) {
      // Make the visibility block of the then and else blocks of a
      // conditional using chpl__tryToken be the block containing the
      // conditional statement.  Without this, there were some cases where
      // a function gets instantiated into one side of the conditional but
      // used in both sides, then the side with the instantiation gets
      // folded out leaving expressions with no visibility block.
      // test/functions/iterators/angeles/dynamic.chpl is an example that
      // currently fails without this.
      return getVisibilityBlock(block->parentExpr, usePtOfInsn);
    } else
      return block;
  } else if (expr->parentExpr) {
    return getVisibilityBlock(expr->parentExpr, usePtOfInsn);
  } else if (Symbol* s = expr->parentSymbol) {
    FnSymbol* fn = toFnSymbol(s);
    if (fn && fn->instantiationPoint && usePtOfInsn)
      return fn->instantiationPoint;
    else
      return getVisibilityBlock(s->defPoint, usePtOfInsn);
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
