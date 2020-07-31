/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
#include "ImportStmt.h"
#include "map.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "view.h"

#include <map>
#include <set>
#include <utility>
#include <vector>


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

typedef std::pair<bool, std::vector<FnSymbol*> > ReexportEntry;

class VisibleFunctionBlock {
public:
                                        VisibleFunctionBlock();

  Map<const char*, Vec<FnSymbol*>*>     visibleFunctions;
  std::map<const char*, ReexportEntry>  reexports;
};

static Map<BlockStmt*, VisibleFunctionBlock*> visibleFunctionMap;

static int                                    nVisibleFunctions       = 0;

static std::map<std::pair<BlockStmt*, BlockStmt*>, bool> scopeIsVisForMethods;
static std::set<const char*> typeHelperNames;


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void buildReexportVec(BlockStmt* scope, const char* name, CallExpr* call,
                             std::vector<FnSymbol*>* vec);
static void updateReexportEntry(VisibleFunctionBlock* vfb, const char* name,
                                BlockStmt* block, CallExpr* call);

static void getVisibleMethods(const char* name, CallExpr* call,
                              Vec<FnSymbol*>& visibleFns);

static void  buildVisibleFunctionMap();

static BlockStmt* getVisibilityScopeNoParentModule(Expr* expr);

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
    if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(block)) {
      if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(info.name)) {
        visibleFns.append(*fns);
      }
      updateReexportEntry(vfb, info.name, block, call);
      visibleFns.append(vfb->reexports[info.name].second);
    }
  } else {
    // Methods, fields, and type helper functions should ignore the privacy and
    // limitations on use statements.  All other symbols should respect them.
    if (call->numActuals() >=2 && call->get(1)->typeInfo() == dtMethodToken) {

      getVisibleMethods(info.name, call, visibleFns);

    } else if (typeHelperNames.find(info.name) != typeHelperNames.end()) {
      getVisibleMethods(info.name, call, visibleFns);

    } else {
      getVisibleFunctions(info.name, call, visibleFns);

    }
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

// Build the cache of names we care about even though they aren't methods
void initTypeHelperNames() {
  typeHelperNames.insert(astrSassign);
  typeHelperNames.insert(astrSeq);
  typeHelperNames.insert(astrSne);
  typeHelperNames.insert(astrSgt);
  typeHelperNames.insert(astrSgte);
  typeHelperNames.insert(astrSlt);
  typeHelperNames.insert(astrSlte);
  typeHelperNames.insert(astrSswap); // ?
  typeHelperNames.insert(astr_cast);
  typeHelperNames.insert(astr_defaultOf);
  typeHelperNames.insert(astrNew);
  typeHelperNames.insert(astr_initCopy);
  typeHelperNames.insert(astr_autoCopy);
  typeHelperNames.insert(astr("chpl__autoDestroy"));
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/
static void buildReexportVec(VisibilityStmt* visStmt, const char* name,
                             CallExpr* call, std::vector<FnSymbol*>* vec);

static void buildReexportVec(BlockStmt* scope, const char* name, CallExpr* call,
                             std::vector<FnSymbol*>* vec) {
  if (scope->useList != NULL) {
    for_actuals(expr, scope->useList) {
      if (ImportStmt* import = toImportStmt(expr)) {
        buildReexportVec(import, name, call, vec);
      } else if (UseStmt* use = toUseStmt(expr)) {
        buildReexportVec(use, name, call, vec);
      } else {
        INT_FATAL("unhandled case");
      }
    }
  }
}

static void buildReexportVec(VisibilityStmt* visStmt, const char* name,
                             CallExpr* call, std::vector<FnSymbol*>* vec) {
  if (!visStmt->isPrivate) {
    // Only public visStmt statements re-export, no need to search
    // otherwise
    if (!visStmt->skipSymbolSearch(name)) {
      SymExpr* se = toSymExpr(visStmt->src);

      INT_ASSERT(se);

      ModuleSymbol* mod = toModuleSymbol(se->symbol());
      INT_ASSERT(mod);

      if (mod->isVisible(call) == true) {
        const char *nameToUse = name;
        const bool isSymRenamed = visStmt->isARenamedSym(name);
        if (isSymRenamed) {
          nameToUse = visStmt->getRenamedSym(name);
        }

        if (VisibleFunctionBlock* vfb =
            visibleFunctionMap.get(mod->block)) {
          // Does the visStmted module define functions with this name?
          if (Vec<FnSymbol*>* fns =
              vfb->visibleFunctions.get(nameToUse)) {
            // Optimization: only check visibility of one private function
            // per scope searched.  The same answer should hold for all
            // private symbols in the same scope.
            bool privacyChecked = false;
            bool privateOkay = false;

            forv_Vec(FnSymbol, fn, *fns) {
              if (fn->hasFlag(FLAG_PRIVATE)) {
                // Ensure that private functions are not used outside of
                // their proper scope
                if (!privacyChecked) {
                  // We haven't checked the privacy of a function in this
                  // scope yet.  Do so now, and remember the result
                  privacyChecked = true;
                  if (fn->isVisible(call) == true) {
                    // We've determined that this function, even though it
                    // is private, can be used
                    vec->push_back(fn);
                    privateOkay = true;
                  }
                } else if (privateOkay) {
                  // We've already checked that private symbols are
                  // accessible in this pass and they are, so it's okay to
                  // add this function to the visible functions list
                  vec->push_back(fn);
                }
              } else {
                // This was a public function, so always include it.
                vec->push_back(fn);
              }
            }
          }

          updateReexportEntry(vfb, nameToUse, mod->block, call);
          for_vector(FnSymbol, fn, vfb->reexports[nameToUse].second) {
            vec->push_back(fn);
          }
        } else {
          // No visible function map, so don't worry about it and just
          // follow the public visStmt statements
          buildReexportVec(mod->block, nameToUse, call, vec);
        }
      }
    }
  }
}

static void updateReexportEntry(VisibleFunctionBlock* vfb, const char* name,
                                BlockStmt* block, CallExpr* call) {
  // Check to see if this scope also had already checked for
  // re-exports with that particular name.
  ReexportEntry& reexportEntry = vfb->reexports[name];
  if (reexportEntry.first == false) {
    // We haven't checked before, so recurse, save, and include what we found
    reexportEntry.first = true;
    buildReexportVec(block, name, call, &reexportEntry.second);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void getVisibleMethods(const char* name, CallExpr* call,
                              BlockStmt* block, std::set<BlockStmt*>& visited,
                              Vec<FnSymbol*>& visibleFns);

static bool isScopeVisibleForMethods(ModuleSymbol* mod, CallExpr* call);


static void getVisibleMethods(const char* name, CallExpr* call,
                              Vec<FnSymbol*>& visibleFns) {
  BlockStmt*           block    = getVisibilityScope(call);
  std::set<BlockStmt*> visited;

  getVisibleMethods(name, call, block, visited, visibleFns);

}

static void getVisibleMethods(const char* name, CallExpr* call,
                              BlockStmt* block, std::set<BlockStmt*>& visited,
                              Vec<FnSymbol*>& visibleFns) {
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
        printf("visible methods: block %i  module %s  %s\n",
               block->id, inMod->name, debugLoc(block));
      else if (fnBlock)
        printf("visible methods: block %i  fn %s  %s\n",
               block->id, inFn->name, debugLoc(block));
      else
        printf("visible methods: block %i  %s\n",
               block->id, debugLoc(block));

      if (instantiationPt) {
        printf("  instantiated from block %i  %s\n",
               instantiationPt->id, debugLoc(instantiationPt));
      }
    }

    // The following statement causes this to apply to all blocks,
    // and not just module or function blocks.
    //
    // This is because e.g. in associative.chpl primer, instantiation occurs in
    // a block that isn't a fn or module block.
    visited.insert(block);

    if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(block)) {
      // the block defines functions

      if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name)) {
        forv_Vec(FnSymbol, fn, *fns) {
          // When private methods and fields are supported, we'll need to extend
          // this
          visibleFns.add(fn);
        }
      }
    }

    if (block->useList != NULL) {
      // the block uses other modules
      for_actuals(expr, block->useList) {
        SymExpr* se = NULL;
        if (UseStmt* use = toUseStmt(expr)) {
          se = toSymExpr(use->src);
        } else if (ImportStmt* import = toImportStmt(expr)) {
          se = toSymExpr(import->src);
        } else {
          INT_FATAL("bad expr in useList, expected ImportStmt or UseStmt");
        }
        // Intentionally ignore use/import privacy and limitations
        // Methods, fields, and special type support functions cannot be
        // re-enabled via only lists (or doing so is not easy for the user).

        // A use statement could be of an enum instead of a module, but only
        // modules can define functions.
        if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
          if (isScopeVisibleForMethods(mod, call)) {
            getVisibleMethods(name, call, mod->block, visited, visibleFns);
          }
        }
      }
    }

    if (block->modRefs != NULL) {
      for_actuals(expr, block->modRefs) {
        SymExpr* se = toSymExpr(expr);
        INT_ASSERT(se);
        if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
          if (isScopeVisibleForMethods(mod, call)) {
            getVisibleMethods(name, call, mod->block, visited, visibleFns);
          }
        }
      }
    }

    if (block != rootModule->block) {
      BlockStmt* next  = getVisibilityScopeNoParentModule(block);

      // Recurse in the enclosing block
      getVisibleMethods(name, call, next, visited, visibleFns);

      if (instantiationPt != NULL) {
        // Also look at the instantiation point
        getVisibleMethods(name, call, instantiationPt, visited, visibleFns);
      }
    }

  }
}

static bool isScopeVisibleForMethods(ModuleSymbol* mod, CallExpr* call) {
  bool isVisible;

  if (mod->hasFlag(FLAG_PRIVATE)) {
    // Get potential scope pair.
    std::pair<BlockStmt*, BlockStmt*> curPair =
      std::make_pair(getVisibilityScope(call), mod->block);
    // See if it's already in the map
    std::map<std::pair<BlockStmt*, BlockStmt*>, bool>::iterator it =
      scopeIsVisForMethods.find(curPair);
    // If not, determine and record the result, otherwise use the cached
    // version.
    if (it == scopeIsVisForMethods.end()) {
      isVisible = mod->isVisible(call);
      scopeIsVisForMethods[curPair] = isVisible;
    } else {
      isVisible = it->second;
    }
  } else {
    isVisible = true;
  }
  return isVisible;
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
                                Vec<FnSymbol*>&       visibleFns,
                                bool inUseChain);

void getVisibleFunctions(const char*      name,
                         CallExpr*        call,
                         Vec<FnSymbol*>&  visibleFns) {
  BlockStmt*           block    = getVisibilityScope(call);
  std::set<BlockStmt*> visited;

  getVisibleFunctions(name, call, block, visited, visibleFns, false);
}

static BlockStmt* getVisibleFnsInstantiationPt(BlockStmt*    block,
                                               ModuleSymbol* inMod,
                                               FnSymbol*     inFn) {
  BlockStmt* instantiationPt = NULL;

  if (block->parentExpr != NULL) {
    // not a module or function level block
  } else if (inMod && block == inMod->block) {
    // module-level block
  } else if (inFn != NULL) {
    // TODO - probably remove this assert
    INT_ASSERT(block->parentSymbol == inFn ||
               isArgSymbol(block->parentSymbol) ||
               isShadowVarSymbol(block->parentSymbol));

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

  return instantiationPt;
}

static void getVisibleFnsShowBlock(BlockStmt* block, ModuleSymbol* inMod,
                                   FnSymbol* inFn, BlockStmt* instantiationPt)
{
  if (inMod && block == inMod->block)
    printf("visible fns: block %i  module %s  %s\n",
           block->id, inMod->name, debugLoc(block));
  else if (inFn && block == inFn->body)
    printf("visible fns: block %i  fn %s  %s\n",
           block->id, inFn->name, debugLoc(block));
  else
    printf("visible fns: block %i  %s\n",
           block->id, debugLoc(block));

  if (instantiationPt) {
    printf("  instantiated from block %i  %s\n",
           instantiationPt->id, debugLoc(instantiationPt));
  }
}

static void getVisibleFnsFirstVisit(const char*       name,
                                CallExpr*             call,
                                BlockStmt*            block,
                                std::set<BlockStmt*>& visited,
                                Vec<FnSymbol*>&       visibleFns)
{
  // Why does the following statement apply to all blocks,
  // and not just module or function blocks?
  //
  // e.g. in associative.chpl primer, instantiation occurs in a
  // block that isn't a fn or module block.
  visited.insert(block);

  if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(block)) {
    // the block defines functions

    if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name)) {
      // Optimization: only check visibility of one private function per scope
      // searched.  The same answer should hold for all private symbols in the
      // same scope.
      bool privacyChecked = false;
      bool privateOkay = false;

      forv_Vec(FnSymbol, fn, *fns) {
        if (fn->hasFlag(FLAG_PRIVATE)) {
          // Ensure that private functions are not used outside of their
          // proper scope
          if (!privacyChecked) {
            // We haven't checked the privacy of a function in this scope yet.
            // Do so now, and remember the result
            privacyChecked = true;
            if (fn->isVisible(call)) {
              // We've determined that this function, even though it is
              // private, can be used
              visibleFns.add(fn);
              privateOkay = true;
            }
          } else if (privateOkay) {
            // We've already checked that private symbols are accessible in
            // this pass and they are, so it's okay to add this function to
            // the visible functions list
            visibleFns.add(fn);
          }
        } else {
          // This was a public function, so always include it.
          visibleFns.add(fn);
        }
      }
    }
  }
}

// The same formula applies to 'use' and 'import' statements.
static bool needToTraverseUse(bool firstVisit,
                              bool inUseChain,
                              bool isPrivate) {
  if (firstVisit) {
    // Only traverse private use statements if we are in the scope
    // that defines them.
    if (!inUseChain)
      // If we're not already in a use chain, by definition we can see
      // private uses.
      return true;
    else
      // If we're in a use chain, assume that private uses
      // are not available to us.
      return !isPrivate;
  } else {
    // We are here only if !inUseChain.
    // Only traverse private use statements at this point.  Public use
    // statements will have already been handled the first time
    // this scope was seen.
    return isPrivate;
  }
}

static void getVisibleFnsFromUseList(const char*      name,
                                CallExpr*             call,
                                BlockStmt*            block,
                                std::set<BlockStmt*>& visited,
                                Vec<FnSymbol*>&       visibleFns,
                                bool                  inUseChain,
                                bool                  firstVisit)
{
  // the block uses other modules
  for_actuals(expr, block->useList) {
    if (UseStmt* use = toUseStmt(expr)) {
      if (needToTraverseUse(firstVisit, inUseChain, use->isPrivate)) {
        if (use->skipSymbolSearch(name) == false) {
          SymExpr* se = toSymExpr(use->src);

          INT_ASSERT(se);

          if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
            // The use statement could be of an enum instead of a module,
            // but only modules can define functions.

            if (mod->isVisible(call)) {
              if (use->isARenamedSym(name)) {
                getVisibleFunctions(use->getRenamedSym(name),
                                    call, mod->block,
                                    visited, visibleFns, true);
              } else {
                getVisibleFunctions(name, call, mod->block,
                                    visited, visibleFns, true);
              }
            }
          }
        }
      }
    } else if (ImportStmt* import = toImportStmt(expr)) {
      if (needToTraverseUse(firstVisit, inUseChain, import->isPrivate)) {
        // Not all import statements define symbols for unqualified access,
        // traverse into those that do when the name we're seeking is
        // specified
        if (import->skipSymbolSearch(name) == false) {
          SymExpr* se = toSymExpr(import->src);

          INT_ASSERT(se);
          ModuleSymbol* mod = toModuleSymbol(se->symbol());
          INT_ASSERT(mod);
          if (mod->isVisible(call)) {
            if (import->isARenamedSym(name)) {
              getVisibleFunctions(import->getRenamedSym(name), call,
                                  mod->block, visited, visibleFns, true);
            } else {
              getVisibleFunctions(name, call, mod->block, visited,
                                  visibleFns, true);
            }
          }
        }
      }
    } else {
      INT_FATAL("Expected ImportStmt or UseStmt");
    }
  }
}

static void getVisibleFunctions(const char*           name,
                                CallExpr*             call,
                                BlockStmt*            block,
                                std::set<BlockStmt*>& visited,
                                Vec<FnSymbol*>&       visibleFns,
                                bool                  inUseChain)
{
  const bool firstVisit = (visited.find(block) == visited.end());

  if (!firstVisit && inUseChain) {
    // We've seen this block already, but we just found it again from going up
    // in scope from the call site.  That means that we may have skipped private
    // uses, so we should go through only the private uses - not in a use chain.
    return;
  }

  ModuleSymbol* inMod = block->getModule();
  FnSymbol*     inFn  = block->getFunction();
  BlockStmt*    instantiationPt = getVisibleFnsInstantiationPt(block,
                                                               inMod, inFn);
  if (firstVisit && call->id == breakOnResolveID)
    getVisibleFnsShowBlock(block, inMod, inFn, instantiationPt);

  // avoid infinite recursion due to modules with mutual uses
  if (firstVisit)
    getVisibleFnsFirstVisit(name, call, block, visited, visibleFns);

  if (block->useList != NULL)
    getVisibleFnsFromUseList(name, call, block, visited, visibleFns,
                             inUseChain, firstVisit);

  // Need to continue going up in case our parent scopes also had private
  // uses that were skipped.
  if (block != rootModule->block) {
    BlockStmt* next  = getVisibilityScopeNoParentModule(block);

    // Recurse in the enclosing block
    getVisibleFunctions(name, call, next, visited, visibleFns, inUseChain);
  }

    // Also look at the instantiation point
  if (instantiationPt != NULL)
    getVisibleFunctions(name, call, instantiationPt, visited, visibleFns,
                        inUseChain);
}

/*
   This function returns a BlockStmt to use as the instantiationPoint
   for expr (to be used when instantiating a type or a function).
 */
BlockStmt* getInstantiationPoint(Expr* expr) {

  if (TypeSymbol* ts = toTypeSymbol(expr->parentSymbol)) {
    if (BlockStmt* block = ts->instantiationPoint) {
      return block;
    }
  }

  Expr* cur = expr;
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
      if (FnSymbol* fn = toFnSymbol(s)) {
        if (BlockStmt* instantiationPt = fn->instantiationPoint())
          return instantiationPt;
      } else if (TypeSymbol* ts = toTypeSymbol(s)) {
        if (BlockStmt* block = ts->instantiationPoint) {
          return block;
        }
      }
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


/* This function returns the next BlockStmt enclosing `expr` that
   should be searched for function definitions when getting visible
   functions.  Unlike getVisibilityScope() above, it will skip over
   inner module's parent (ancestor) modules since we don't consider
   those symbols to be lexically visible as of PR #15312.
 */
static BlockStmt* getVisibilityScopeNoParentModule(Expr* expr) {
  BlockStmt* next = getVisibilityScope(expr);

  ModuleSymbol* blockMod = expr->getModule();
  ModuleSymbol* nextMod = next->getModule();
  if (blockMod != nextMod && nextMod != theProgram && nextMod != rootModule) {
    next = standardModule->block;
  }

  return next;
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

    std::map<const char*, ReexportEntry>::iterator it;
    for (it= vfb->reexports.begin(); it != vfb->reexports.end(); ++it) {
      it->second.second.clear();
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
