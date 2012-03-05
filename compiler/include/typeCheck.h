#ifndef TYPECHECK_H
#define TYPECHECK_H

#include "symbol.h"
#include "astutil.h"
#include "map.h"
#include "vec.h"

class VisibleFunctionBlock {
 public:
  Map<const char*,Vec<FnSymbol*>*> visibleFunctions;
  VisibleFunctionBlock() { }
};

struct VisibleFunctionManager {
  Map<BlockStmt*,VisibleFunctionBlock*> visibleFunctionMap;
  int nVisibleFunctions; // for incremental build
  Map<BlockStmt*,BlockStmt*> visibilityBlockCache;
  Vec<BlockStmt*> standardModuleSet;

  VisibleFunctionManager() : nVisibleFunctions(0) {}
  //
  // return the innermost block for searching for visible functions
  //
  static BlockStmt*
  getVisibilityBlock(Expr* expr) {
    if (BlockStmt* block = toBlockStmt(expr->parentExpr)) {
      if (block->blockTag == BLOCK_SCOPELESS)
        return getVisibilityBlock(block);
      else
        return block;
    } else if (expr->parentExpr) {
      return getVisibilityBlock(expr->parentExpr);
    } else {
      FnSymbol* fn = toFnSymbol(expr->parentSymbol);
      if (fn && fn->instantiationPoint)
        return fn->instantiationPoint;
      else
        return getVisibilityBlock(expr->parentSymbol->defPoint);
    }
  }

  void buildVisibleFunctionMap() {
    for (int i = nVisibleFunctions; i < gFnSymbols.n; i++) {
      FnSymbol* fn = gFnSymbols.v[i];
      if (!fn->hasFlag(FLAG_INVISIBLE_FN) && fn->defPoint->parentSymbol &&
          !isArgSymbol(fn->defPoint->parentSymbol) &&
          !isInterfaceSymbol(fn->defPoint->parentSymbol)) {
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

  BlockStmt*
  getVisibleFunctions(BlockStmt* block,
                      const char* name,
                      Vec<FnSymbol*>& visibleFns,
                      Vec<BlockStmt*>& visited) {
    //
    // all functions in standard modules are stored in a single block
    //
    if (standardModuleSet.set_in(block))
      block = theProgram->block;

    //
    // avoid infinite recursion due to modules with mutual uses
    //
    if (visited.set_in(block))
      return NULL;
    else if (isModuleSymbol(block->parentSymbol))
      visited.set_add(block);

    bool canSkipThisBlock = true;

    VisibleFunctionBlock* vfb = visibleFunctionMap.get(block);
    if (vfb) {
      canSkipThisBlock = false; // cannot skip if this block defines functions
      Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name);
      if (fns) {
        visibleFns.append(*fns);
      }
    }

    if (block->modUses) {
      for_actuals(expr, block->modUses) {
        SymExpr* se = toSymExpr(expr);
        INT_ASSERT(se);
        ModuleSymbol* mod = toModuleSymbol(se->var);
        INT_ASSERT(mod);
        canSkipThisBlock = false; // cannot skip if this block uses modules
        getVisibleFunctions(mod->block, name, visibleFns, visited);
      }
    }

    //
    // visibilityBlockCache contains blocks that can be skipped
    //
    if (BlockStmt* next = visibilityBlockCache.get(block)) {
      getVisibleFunctions(next, name, visibleFns, visited);
      return (canSkipThisBlock) ? next : block;
    }

    if (block != rootModule->block) {
      BlockStmt* next = getVisibilityBlock(block);
      BlockStmt* cache = getVisibleFunctions(next, name, visibleFns, visited);
      if (cache)
        visibilityBlockCache.put(block, cache);
      return (canSkipThisBlock) ? cache : block;
    }

    return NULL;
  }
};

#endif
