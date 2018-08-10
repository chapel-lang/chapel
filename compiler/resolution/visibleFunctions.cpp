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
        block = getVisibilityScope(fn->defPoint);
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
* chain.                                                                      *
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
  BlockStmt*           block    = getVisibilityScope(call);
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
    bool fnBlock = false;
    ModuleSymbol* inMod = block->getModule();
    FnSymbol* inFn = block->getFunction();
    BlockStmt* instantiationPt = NULL;
    if (block->parentExpr != NULL) {
      // not a module or function level block
    } else if (inMod && block == inMod->block) {
      moduleBlock = true;
    } else if (inFn != NULL) {
      // TODO - probably remove this assert
      INT_ASSERT(block->parentSymbol == inFn ||
                 isArgSymbol(block->parentSymbol) ||
                 isShadowVarSymbol(block->parentSymbol));
      fnBlock = true;
      BlockStmt* inFnInstantiationPoint = inFn->instantiationPoint();
      if (inFnInstantiationPoint && !inFnInstantiationPoint->parentSymbol) {
        INT_FATAL(inFn, "instantiation point not in tree\n"
                        "try --break-on-remove-id %i and consider making\n"
                        "that block scopeless",
                        inFnInstantiationPoint->id);
      }
      if (inFnInstantiationPoint && inFnInstantiationPoint->parentSymbol)
        instantiationPt = inFnInstantiationPoint;
    }

    if (call->id == breakOnResolveID) {
      if (moduleBlock)
        printf("visible fns: block %i module %s %s:%i\n",
               block->id, inMod->name,
               block->fname(), block->linenum());
      else if (fnBlock)
        printf("visible fns: block %i fn %s %s:%i\n",
               block->id, inFn->name,
               block->fname(), block->linenum());
      else
        printf("visible fns: block %i %s:%i\n",
               block->id,
               block->fname(), block->linenum());

      if (instantiationPt) {
        printf("  instantiated from block %i %s:%i\n",
               instantiationPt->id,
               instantiationPt->fname(), instantiationPt->linenum());
      }
    }

    // Why does the following statement apply to all blocks,
    // and not just module or function blocks?
    //
    // e.g. in associative.chpl primer, instantiation occurs in a
    // block that isn't a fn or module block.
    visited.insert(block);

    if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(block)) {
      // the block defines functions

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
      // the block uses other modules
      for_actuals(expr, block->useList) {
        UseStmt* use = toUseStmt(expr);

        INT_ASSERT(use);

        bool isMethodCall = false;
        if (call->numActuals() >= 2 &&
            call->get(1)->typeInfo() == dtMethodToken)
          isMethodCall = true;

        if (use->skipSymbolSearch(name, isMethodCall) == false) {
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
      BlockStmt* next  = getVisibilityScope(block);

      // Recurse in the enclosing block
      getVisibleFunctions(name, call, next, visited, visibleFns);

      if (instantiationPt != NULL) {
        // Also look at the instantiation point
        getVisibleFunctions(name, call, instantiationPt, visited, visibleFns);
      }
    }
  }
}

static bool isTryTokenCond(Expr* expr);

static Expr* getTryTokenParent(Expr* expr);

/*
   This function returns a BlockStmt to use as the instantiationPoint
   for expr (to be used when instantiating a type or a function).
 */
BlockStmt* getInstantiationPoint(Expr* expr) {

  Expr* cur = getTryTokenParent(expr);
  while (cur != NULL) {
    if (BlockStmt* block = toBlockStmt(cur->parentExpr)) {
      if (block->blockTag == BLOCK_SCOPELESS) {
        // continue
      } else {
        return block;
      }
    } else if (cur->parentExpr) {
      // continue
    } else if (Symbol* s = cur->parentSymbol) {
      if (FnSymbol* fn = toFnSymbol(s))
        if (BlockStmt* instantiationPt = fn->instantiationPoint())
          return instantiationPt;
      // otherwise continue
    }

    // Where to look next?
    if (cur->parentExpr)
      cur = cur->parentExpr;
    else if (cur->parentSymbol)
      cur = cur->parentSymbol->defPoint;
    else
      cur = NULL;
  }

  if (cur == NULL)
    INT_FATAL(expr, "Expression has no visibility block.");

  return NULL;
}

/* This function returns the next BlockStmt enclosing `expr` that
   should be searched for function definitions when getting visible
   functions.

   This can be considered the scope of `expr`.
   Note that `expr` might be able to resolve calls from an instantiation
   point as well.
 */
BlockStmt* getVisibilityScope(Expr* expr) {

  Expr* cur = expr;
  while (cur != NULL) {
    // Pretend that ArgSymbols are in the function's body
    // (which is reasonable since functions cannot be defined
    //  within an ArgSymbol).
    // See e.g. test default-argument-generic.chpl
    if (isArgSymbol(cur->parentSymbol)) {
      return cur->getFunction()->body;
    }

    // Stop when we find a non-scopeless block
    if (BlockStmt* block = toBlockStmt(cur->parentExpr))
      if (block->blockTag != BLOCK_SCOPELESS)
        return block;

    // Where to look next?
    if (cur->parentExpr)
      cur = cur->parentExpr;
    else if (cur->parentSymbol)
      cur = cur->parentSymbol->defPoint;
    else
      cur = NULL;
  }

  if (cur == NULL)
    INT_FATAL(expr, "Expression has no visibility block.");

  return NULL;
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

//
// If the expr is in a CondStmt with chpl__tryToken (including in
// nested blocks), then return the CondStmt. Otherwise, just return expr.
//
// Why is this relevant for visibility?  If a function has an
// instantiationPoint that is a block that is removed, then bad things
// happen (in particular functions that should be visible are no longer
// visible). And the chpl__tryToken handling can remove all nested blocks
// inside the clause not selected.
//
// test/functions/iterators/angeles/dynamic.chpl might be a relevant example.
//
static Expr* getTryTokenParent(Expr* expr) {
  for (Expr* cur = expr; cur != NULL; cur = cur->parentExpr) {
    if (isTryTokenCond(cur))
      return cur;
  }
  return expr;
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
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

VisibleFunctionBlock::VisibleFunctionBlock() {

}
