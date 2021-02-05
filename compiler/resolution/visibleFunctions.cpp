/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static std::set<const char*> typeHelperNames;

static bool useMethodVisibilityRules(CallExpr* call, const char* name) {
  return (call->numActuals() >=2 &&
          call->get(1)->typeInfo() == dtMethodToken)
         || typeHelperNames.count(name);
}


// This constructor creates a fresh VisibilityInfo
// when we start processing the call described by 'info'.
VisibilityInfo::VisibilityInfo(const CallInfo& info) :
  currStart(NULL), nextPOI(NULL), poiDepth(-1), call(info.call),
  useMethodVisibility(useMethodVisibilityRules(info.call, info.name))
{ }

// This constructor is for use in visitMorePOIs(), where it clones
// the incoming visInfo so it remains unaffected by local modifications.
// We skip 'visitedScopes' to avoid some copying, because we can.
VisibilityInfo::VisibilityInfo(const VisibilityInfo& src) :
  currStart(src.currStart), nextPOI(src.nextPOI),
  visitedScopes(), instnPoints(src.instnPoints), poiDepth(src.poiDepth),
  call(src.call), useMethodVisibility(src.useMethodVisibility)
{ }


// Might the 'scope' define a fn that might change resolution outcome?
// Such fn has the same name as this CFI's function and
// is not automatically applicable.
bool scopeMayDefineHazard(BlockStmt* scope, const char* fnName) {
  if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(scope))
    if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(fnName))
      forv_Vec(FnSymbol, fn, *fns)
        return true;
  return false;
}

bool scopeDefinesVisibleFunctions(BlockStmt* scope) {
  return visibleFunctionMap.get(scope) != NULL;
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void buildReexportVec(BlockStmt* scope, const char* name, CallExpr* call,
                             std::vector<FnSymbol*>* vec);
static void updateReexportEntry(VisibleFunctionBlock* vfb, const char* name,
                                BlockStmt* block, CallExpr* call);

static void getVisibleFunctionsVI(const char*         name,
                                CallExpr*             call,
                                VisibilityInfo*       visInfo,
                                std::set<BlockStmt*>* visited,
                                Vec<FnSymbol*>&       visibleFns);

static void getVisibleMethods(const char* name, CallExpr* call,
                              Vec<FnSymbol*>& visibleFns);

static void getVisibleMethodsVI(const char*           name,
                                CallExpr*             call,
                                VisibilityInfo*       visInfo,
                                std::set<BlockStmt*>* visited,
                                Vec<FnSymbol*>&       visibleFns);

static void  buildVisibleFunctionMap();

static BlockStmt* getVisibilityScopeNoParentModule(Expr* expr);

void getMoreVisibleFunctionsOrMethods(const char*     name,
                                CallExpr*             call,
                                VisibilityInfo*       visInfo,
                                std::set<BlockStmt*>* visited,
                                Vec<FnSymbol*>&       visibleFns) {
  if (visInfo->useMethodVisibility)
    getVisibleMethodsVI(name, call, visInfo, visited, visibleFns);
  else
    getVisibleFunctionsVI(name, call, visInfo, visited, visibleFns);
}

void findVisibleFunctionsAllPOIs(CallInfo&       info,
                                 Vec<FnSymbol*>& visibleFns) {
  findVisibleFunctions(info, NULL, NULL, NULL, visibleFns);
}

void findVisibleFunctions(CallInfo&             info,
                          VisibilityInfo*       visInfo,
                          std::set<BlockStmt*>* visited,
                          int*                  numVisitedP,
                          Vec<FnSymbol*>&       visibleFns) {
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
    // Methods, fields, and type helper functions should first look at the scope
    // where the type was defined.  All other functions don't need to do this.
    if (visInfo != NULL) {
      // cf. getMoreVisibleFunctionsOrMethods()
      if (visInfo->useMethodVisibility)
        getVisibleMethodsVI(info.name, call, visInfo, visited, visibleFns);
      else
        getVisibleFunctionsVI(info.name, call, visInfo, visited, visibleFns);

    } else {
      if (useMethodVisibilityRules(call, info.name))
        getVisibleMethods(info.name, call, visibleFns);
      else
        getVisibleFunctions(info.name, call, visibleFns);
    }
  }

  if ((explainCallLine && explainCallMatch(call)) ||
      call->id == explainCallID) {
    int startVisited = numVisitedP ? *numVisitedP : 0;
    if (startVisited == 0)
      USR_PRINT(call, "call: %s", info.toString());

    if (visibleFns.n == 0) {
     if (numVisitedP == NULL)
      USR_PRINT(call, "no visible functions found");

    } else {
      int i = 0;

      forv_Vec(FnSymbol, visibleFn, visibleFns) {
       if (i++ >= startVisited)
        USR_PRINT(visibleFn,
                  "%s %s",
                  i == 1 ? "visible functions are:" : "                      ",
                  toString(visibleFn));
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
        // As of PR #16695, getVisibleFunctions() et al do not check rootModule
        // for visible functions. (Although they could, if desired.)
        // So, ensure we do not add anything there.
        INT_ASSERT(block != rootBlock);
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

static void getVisibleMethodsFiltered(const char* name, CallExpr* call,
                                      BlockStmt* block, VisibilityInfo* visInfo,
                                      std::set<BlockStmt*>& visited,
                                      Vec<FnSymbol*>& visibleFns,
                                      bool inUseChain,
                                      std::set<const char*> typeNames);

static void getVisibleMethodsImpl(const char* name, CallExpr* call,
                                  BlockStmt*            block,
                                  VisibilityInfo*       visInfo,
                                  std::set<BlockStmt*>& visited,
                                  Vec<FnSymbol*>&       visibleFns,
                                  bool inUseChain);

static void lookAtTypeFirst(const char* name, CallExpr* call, BlockStmt* block,
                            VisibilityInfo*       visInfo,
                            std::set<BlockStmt*>& visited,
                            Vec<FnSymbol*>&       visibleFns);

static BlockStmt* getVisibleFnsInstantiationPt(BlockStmt* block);

static void getVisibleFnsShowBlock(const char* context, BlockStmt* block,
                                   BlockStmt* instantiationPt);

static void getVisibleMethodsVI(const char* name, CallExpr* call,
                                VisibilityInfo*       visInfo,
                                std::set<BlockStmt*>* visited,
                                Vec<FnSymbol*>&       visibleFns)
{
  INT_ASSERT(visited->empty() == !visInfo->inPOI()); //fyi

  if (visited->empty()) {
    // it is the first time we are here for this 'call'
    BlockStmt* block = getVisibilityScope(call);
    lookAtTypeFirst(name, call, block, visInfo, *visited, visibleFns);
  }

  getVisibleMethodsImpl(name, call, visInfo->currStart, visInfo,
                        *visited, visibleFns, false);
}

static void getVisibleMethods(const char* name, CallExpr* call,
                              Vec<FnSymbol*>& visibleFns) {
  BlockStmt*           block    = getVisibilityScope(call);
  std::set<BlockStmt*> visited;

  lookAtTypeFirst(name, call, block, NULL, visited, visibleFns);

  getVisibleMethodsImpl(name, call, block, NULL, visited, visibleFns, false);
}

static void lookAtTypeFirstHelper(const char* name, CallExpr* call,
                                  BlockStmt* block, AggregateType* curType,
                                  VisibilityInfo* visInfo,
                                  std::set<BlockStmt*>& visited,
                                  Vec<FnSymbol*>& visibleFns) {
  BlockStmt* typeScope = getVisibilityScope(curType->symbol->defPoint);
  // When searching the type scope, don't follow private uses and imports if we
  // aren't already in the scope where the type is defined.
  bool inScopeJump = typeScope != block;

  // Look at own methods
  getVisibleMethodsImpl(name, call, typeScope, visInfo, visited, visibleFns,
                        inScopeJump);

  // Follow inheritance, if necessary
  forv_Vec(AggregateType, pt, curType->dispatchParents) {
    lookAtTypeFirstHelper(name, call, block, pt, visInfo, visited, visibleFns);
  }
}

static void lookAtTypeFirst(const char* name, CallExpr* call, BlockStmt* block,
                            VisibilityInfo*       visInfo,
                            std::set<BlockStmt*>& visited,
                            Vec<FnSymbol*>&       visibleFns)
{
  INT_ASSERT(call->numActuals() >= 1);
  Expr* typeActual = NULL;

  if (call->numActuals() >= 2 && call->get(1)->typeInfo() == dtMethodToken) {
    typeActual = call->get(2);
  } else {
    typeActual = call->get(1);
  }
  Type* t = typeActual->getValType();

  BlockStmt* typeScope = getVisibilityScope(t->symbol->defPoint);
  // When searching the type scope, don't follow private uses and imports if we
  // aren't already in the scope where the type is defined.
  bool inScopeJump = typeScope != block;

  // Look at own methods
  getVisibleMethodsImpl(name, call, typeScope, visInfo, visited, visibleFns,
                        inScopeJump);

  if (AggregateType* at = toAggregateType(t)) {
    // Follow inheritance, if necessary
    forv_Vec(AggregateType, pt, at->dispatchParents) {
      lookAtTypeFirstHelper(name, call, block, pt, visInfo, visited,
                            visibleFns);
    }
  }
}

static void getVisibleMethodsFirstVisit(const char* name, CallExpr* call,
                                        BlockStmt* block,
                                        VisibilityInfo* visInfo,
                                        std::set<BlockStmt*>& visited,
                                        Vec<FnSymbol*>& visibleFns) {
  // The following statement causes this to apply to all blocks,
  // and not just module or function blocks.
  //
  // This is because e.g. in associative.chpl primer, instantiation occurs in
  // a block that isn't a fn or module block.
  visited.insert(block);
  if (visInfo != NULL)
    visInfo->visitedScopes.push_back(block);

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
}

static void getVisibleMethodsFirstVisitFiltered(const char* name,
                                                CallExpr* call,
                                                BlockStmt* block,
                                                VisibilityInfo* visInfo,
                                                std::set<BlockStmt*>& visited,
                                                Vec<FnSymbol*>& visibleFns,
                                                std::set<const char*> typeNames)
{
  // Why does the following statement apply to all blocks,
  // and not just module or function blocks?
  //
  // e.g. in associative.chpl primer, instantiation occurs in a
  // block that isn't a fn or module block.
  visited.insert(block);
  if (visInfo != NULL)
    visInfo->visitedScopes.push_back(block);

  if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(block)) {
    // the block defines functions

    if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name)) {
      forv_Vec(FnSymbol, fn, *fns) {
        // When private methods and fields are supported, we'll need to extend
        // this
        if (fn->isMethod()) {
          INT_ASSERT(typeNames.size() > 0);
          const char* nameToCheck = fn->_this->type->symbol->name;
          if (fn->_this->getValType() == dtUnknown) {
            ArgSymbol* _this = toArgSymbol(fn->_this);
            INT_ASSERT(_this);
            BlockStmt* typeExpr = _this->typeExpr;
            // The `this` arg doesn't know its type yet.  It could be a call
            // that needs to be resolved (in which case we throw up our hands
            // for now) or it could be a generic instantiation.
            if (CallExpr* thisTypeCall = toCallExpr(typeExpr->body.tail)) {
              if (SymExpr* callBase = toSymExpr(thisTypeCall->baseExpr)) {
                nameToCheck = callBase->symbol()->name;
              }
            }
          }
          std::set<const char*>::iterator it =
            typeNames.find(nameToCheck);
          if (it != typeNames.end()) {
            // This method is defined on a type that is in the filter list.
            // That means we should consider it a candidate.
            visibleFns.add(fn);
          }
        } else {
          visibleFns.add(fn);
        }
      }
    }
  }
}

static bool needToTraverseUse(bool firstVisit, bool inUseChain, bool isPrivate);
static void getVisibleMethodsFromUseListFiltered(const char* name,
                                                 CallExpr* call,
                                                 BlockStmt* block,
                                                 VisibilityInfo* visInfo,
                                                 std::set<BlockStmt*>& visited,
                                                 Vec<FnSymbol*>& visibleFns,
                                                 bool inUseChain,
                                                 bool firstVisit,
                                                 std::set<const char*> filter);
static void mergeFilters(std::set<const char*> outer,
                         std::set<const char*> inner,
                         std::set<const char*>* merged);

static void getVisibleMethodsFromUseList(const char* name, CallExpr* call,
                                         BlockStmt* block,
                                         VisibilityInfo* visInfo,
                                         std::set<BlockStmt*>& visited,
                                         Vec<FnSymbol*>& visibleFns,
                                         bool inUseChain, bool firstVisit) {
  std::set<const char*> filter;
  getVisibleMethodsFromUseListFiltered(name, call, block, visInfo, visited,
                                       visibleFns, inUseChain, firstVisit,
                                       filter);
}

static void getVisibleMethodsFromUseListFiltered(const char* name,
                                                 CallExpr* call,
                                                 BlockStmt* block,
                                                 VisibilityInfo* visInfo,
                                                 std::set<BlockStmt*>& visited,
                                                 Vec<FnSymbol*>& visibleFns,
                                                 bool inUseChain,
                                                 bool firstVisit,
                                                 std::set<const char*> filter) {
  // the block uses other modules
  for_actuals(expr, block->useList) {
    SymExpr* se = NULL;
    std::set<const char*> namedTypes;

    if (UseStmt* use = toUseStmt(expr)) {
      if (!needToTraverseUse(firstVisit, inUseChain, use->isPrivate))
        continue;
      if (call->numActuals() >= 2) {
        Expr* thisArg = call->get(2);
        Type* thisType = thisArg->getValType();
        namedTypes = use->typeWasNamed(thisType);
        if (use->hasExceptList() && namedTypes.size() == 0) {
          // The intersection of symbols allowed by the except list and this
          // type's name or inherited names was empty.  We should not traverse
          // this use statement.
          continue;
        }
      }
      if (use->skipSymbolSearch(name) && namedTypes.size() == 0)
        continue;
      se = toSymExpr(use->src);
    } else if (ImportStmt* import = toImportStmt(expr)) {
      if (!needToTraverseUse(firstVisit, inUseChain, import->isPrivate))
        continue;
      if (call->numActuals() >= 2) {
        Expr* thisArg = call->get(2);
        Type* thisType = thisArg->getValType();
        namedTypes = import->typeWasNamed(thisType);
      }
      if (import->skipSymbolSearch(name) && namedTypes.size() == 0)
        continue;

      se = toSymExpr(import->src);

    } else {
      INT_FATAL("bad expr in useList, expected ImportStmt or UseStmt");
    }
    // A use statement could be of an enum instead of a module, but only
    // modules can define functions.
    if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
      if (mod->isVisible(call)) {
        if (filter.size() > 0 && namedTypes.size() > 0) {
          std::set<const char*> intersection;
          mergeFilters(filter, namedTypes, &intersection);
          // should we call namedTypes.clear() for space concerns?

          if (intersection.size() > 0) {
            // We should only traverse this use or import if there is some
            // overlap between the filter we were already provided and our new
            // limitations.
            getVisibleMethodsFiltered(name, call, mod->block, visInfo, visited,
                                      visibleFns, true, intersection);
          }

        } else if (filter.size() > 0) {
          getVisibleMethodsFiltered(name, call, mod->block, visInfo, visited,
                                    visibleFns, true, filter);

        } else if (namedTypes.size() > 0) {
          getVisibleMethodsFiltered(name, call, mod->block, visInfo, visited,
                                    visibleFns, true, namedTypes);
        } else {
          getVisibleMethodsImpl(name, call, mod->block, visInfo, visited,
                                visibleFns, true);
        }
      }
    }
  }
}

// This function returns a set containing the intersection of outer and inner.
// Any symbol that is solely in one list and not the other is not valid and
// should not be returned.
static void mergeFilters(std::set<const char*> outer,
                         std::set<const char*> inner,
                         std::set<const char*>* merged) {
  INT_ASSERT(outer.size() > 0 && inner.size() > 0);
  std::set<const char*>::iterator it;

  for (it = outer.begin(); it != outer.end(); ++it) {
    if (inner.find(*it) != inner.end()) {
      merged->insert(*it);
    }
  }
}

static void getVisibleMethodsFiltered(const char* name, CallExpr* call,
                                      BlockStmt* block, VisibilityInfo* visInfo,
                                      std::set<BlockStmt*>& visited,
                                      Vec<FnSymbol*>& visibleFns,
                                      bool inUseChain,
                                      std::set<const char*> typeNames) {
  if (block == rootBlock) return; // nothing there
  //
  // avoid infinite recursion due to modules with mutual uses
  //
  const bool firstVisit = (visited.find(block) == visited.end());
  if (!firstVisit && inUseChain) {
    // inUseChain tracks how we are currently finding the scope.  There's
    // three scenarios:
    // - found the scope the first time by traversing a use/import
    //   - found the scope again by also traversing a use/import (which causes
    //      us to enter this if branch)
    //   - found the scope again by going up in scope from the call site (this
    //     case will *not* cause us to enter this if branch)
    // - found the scope the first time by going up in scope from the call site
    //   and again by traversing a use/import (this case will also cause us to
    //   enter this if branch)

    // We don't want to traverse private use/import statements unless we found
    // the scope with the use/import statement by going up in scope from the
    // call site.
    return;
  }

  BlockStmt* instantiationPt = getVisibleFnsInstantiationPt(block);

  // print tracking information
  if (call->id == breakOnResolveID) {
    getVisibleFnsShowBlock("methods", block, instantiationPt);
  }

  if (firstVisit)
    getVisibleMethodsFirstVisitFiltered(name, call, block, visInfo, visited,
                                        visibleFns, typeNames);

  if (block->useList != NULL) {
    getVisibleMethodsFromUseListFiltered(name, call, block, visInfo, visited,
                                         visibleFns, inUseChain, firstVisit,
                                         typeNames);
  }

  if (firstVisit && block->modRefs != NULL) {
    for_actuals(expr, block->modRefs) {
      SymExpr* se = toSymExpr(expr);
      INT_ASSERT(se);
      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        if (mod->isVisible(call)) {
          // Treat following a module reference as though it was a use statement
          // for the purpose of determining whether we can follow private uses
          // and imports
          getVisibleMethodsFiltered(name, call, mod->block, visInfo, visited,
                                    visibleFns, true, typeNames);
        }
      }
    }
  }

  // Recurse in the enclosing block
  BlockStmt* next  = getVisibilityScopeNoParentModule(block);
  getVisibleMethodsFiltered(name, call, next, visInfo, visited, visibleFns,
                            inUseChain, typeNames);

  if (instantiationPt != NULL) {
    // Also look at the instantiation point
    if (visInfo == NULL)
      getVisibleMethodsFiltered(name, call,   // visit all POIs right away
                                instantiationPt, NULL, visited, visibleFns,
                                inUseChain, typeNames);
    else {
      // I'm concerned that coming back to it later won't allow the filtering
      // to be maintained.
      visInfo->nextPOI = instantiationPt; // come back to it later
    }
  }
}


static void getVisibleMethodsImpl(const char* name, CallExpr* call,
                                  BlockStmt* block, VisibilityInfo* visInfo,
                                  std::set<BlockStmt*>& visited,
                                  Vec<FnSymbol*>& visibleFns, bool inUseChain) {
  if (block == rootBlock) return; // nothing there
  //
  // avoid infinite recursion due to modules with mutual uses
  //
  const bool firstVisit = (visited.find(block) == visited.end());
  if (!firstVisit && inUseChain) {
    // inUseChain tracks how we are currently finding the scope.  There's
    // three scenarios:
    // - found the scope the first time by traversing a use/import
    //   - found the scope again by also traversing a use/import (which causes
    //      us to enter this if branch)
    //   - found the scope again by going up in scope from the call site (this
    //     case will *not* cause us to enter this if branch)
    // - found the scope the first time by going up in scope from the call site
    //   and again by traversing a use/import (this case will also cause us to
    //   enter this if branch)

    // We don't want to traverse private use/import statements unless we found
    // the scope with the use/import statement by going up in scope from the
    // call site.
    return;
  }

  BlockStmt* instantiationPt = getVisibleFnsInstantiationPt(block);

  // print tracking information
  if (call->id == breakOnResolveID) {
    getVisibleFnsShowBlock("methods", block, instantiationPt);
  }

  if (firstVisit)
    getVisibleMethodsFirstVisit(name, call, block, visInfo, visited,
                                visibleFns);

  if (block->useList != NULL) {
    getVisibleMethodsFromUseList(name, call, block, visInfo, visited,
                                 visibleFns, inUseChain, firstVisit);
  }

  if (firstVisit && block->modRefs != NULL) {
    for_actuals(expr, block->modRefs) {
      SymExpr* se = toSymExpr(expr);
      INT_ASSERT(se);
      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        if (mod->isVisible(call)) {
          // Treat following a module reference as though it was a use statement
          // for the purpose of determining whether we can follow private uses
          // and imports
          getVisibleMethodsImpl(name, call, mod->block, visInfo, visited,
                                visibleFns, true);
        }
      }
    }
  }

  // Recurse in the enclosing block
  BlockStmt* next  = getVisibilityScopeNoParentModule(block);
  getVisibleMethodsImpl(name, call, next, visInfo, visited, visibleFns,
                        inUseChain);

  if (instantiationPt != NULL) {
    // Also look at the instantiation point
    if (visInfo == NULL)
      getVisibleMethodsImpl(name, call,   // visit all POIs right away
                            instantiationPt, NULL, visited, visibleFns,
                            inUseChain);
    else
      visInfo->nextPOI = instantiationPt; // come back to it later
  }
}

/************************************* | **************************************
*                                                                             *
* Collects functions called 'name' visible in 'block' and up the visibility   *
* chain.                                                                      *
* The functions defined/visible in a block are given by 'visibleFunctionMap'. *
*                                                                             *
************************************** | *************************************/

static void getVisibleFunctionsImpl(const char*       name,
                                CallExpr*             call,
                                BlockStmt*            block,
                                VisibilityInfo*       visInfo,
                                std::set<BlockStmt*>& visited,
                                Vec<FnSymbol*>&       visibleFns,
                                bool                  inUseChain);

static void getVisibleFunctionsVI(const char*            name,
                                CallExpr*                call,
                                VisibilityInfo*          visInfo,
                                std::set<BlockStmt*>*    visited,
                                Vec<FnSymbol*>&          visibleFns)
{
  getVisibleFunctionsImpl(name, call, visInfo->currStart, visInfo,
                          *visited, visibleFns, false);
}

void getVisibleFunctions(const char*      name,
                         CallExpr*        call,
                         Vec<FnSymbol*>&  visibleFns) {
  BlockStmt*           block    = getVisibilityScope(call);
  std::set<BlockStmt*> visited;

  getVisibleFunctionsImpl(name, call, block, NULL,
                          visited, visibleFns, false);
}

static BlockStmt* getVisibleFnsInstantiationPt(BlockStmt* block) {
  BlockStmt* instantiationPt = NULL;

  // We check for an instantiation point only at FnSymbols'
  // top-level blocks, i.e. body, where, etc.
  if (block->parentExpr != NULL)
    return NULL;

  if (FnSymbol* inFn = toFnSymbol(block->parentSymbol)) {
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

// prints "visible fns: block ...." or "visible methods: block ....
static void getVisibleFnsShowBlock(const char* context, BlockStmt* block,
                                   BlockStmt* instantiationPt)
{
  ModuleSymbol* inMod = toModuleSymbol(block->parentSymbol);
  FnSymbol*     inFn  = toFnSymbol(block->parentSymbol);

  if (inMod && block == inMod->block)
    printf("visible %s: block %i  module %s  %s\n", context,
           block->id, inMod->name, debugLoc(block));
  else if (inFn && block == inFn->body)
    printf("visible %s: block %i  fn %s  %s\n", context,
           block->id, inFn->name, debugLoc(block));
  else
    printf("visible %s: block %i  %s\n", context,
           block->id, debugLoc(block));

  if (instantiationPt)
    printf("  instantiated from block %i  %s\n",
           instantiationPt->id, debugLoc(instantiationPt));
}

static void getVisibleFnsFirstVisit(const char*       name,
                                CallExpr*             call,
                                BlockStmt*            block,
                                VisibilityInfo*       visInfo,
                                std::set<BlockStmt*>& visited,
                                Vec<FnSymbol*>&       visibleFns)
{
  // Why does the following statement apply to all blocks,
  // and not just module or function blocks?
  //
  // e.g. in associative.chpl primer, instantiation occurs in a
  // block that isn't a fn or module block.
  visited.insert(block);
  if (visInfo != NULL)
    visInfo->visitedScopes.push_back(block);

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
                                VisibilityInfo*       visInfo,
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
                getVisibleFunctionsImpl(use->getRenamedSym(name),
                  call, mod->block, visInfo, visited, visibleFns, true);
              } else {
                getVisibleFunctionsImpl(name, call, mod->block, visInfo, 
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
              getVisibleFunctionsImpl(import->getRenamedSym(name),
                call, mod->block, visInfo, visited, visibleFns, true);
            } else {
              getVisibleFunctionsImpl(name, call, mod->block, visInfo,
                                  visited, visibleFns, true);
            }
          }
        }
      }
    } else {
      INT_FATAL("Expected ImportStmt or UseStmt");
    }
  }
}

static void getVisibleFunctionsImpl(const char*       name,
                                CallExpr*             call,
                                BlockStmt*            block,
                                VisibilityInfo*       visInfo,
                                std::set<BlockStmt*>& visited,
                                Vec<FnSymbol*>&       visibleFns,
                                bool                  inUseChain)
{
  if (block == rootBlock) return; // nothing there

  const bool firstVisit = (visited.find(block) == visited.end());

  if (!firstVisit && inUseChain) {
    // We've seen this block already, but we just found it again from going up
    // in scope from the call site.  That means that we may have skipped private
    // uses, so we should go through only the private uses - not in a use chain.
    return;
  }

  BlockStmt* instantiationPt = getVisibleFnsInstantiationPt(block);

  if (firstVisit && call->id == breakOnResolveID)
    getVisibleFnsShowBlock("fns", block, instantiationPt);

  // avoid infinite recursion due to modules with mutual uses
  if (firstVisit)
    getVisibleFnsFirstVisit(name, call, block, visInfo, visited, visibleFns);

  if (block->useList != NULL)
    getVisibleFnsFromUseList(name, call, block, visInfo, visited, visibleFns,
                             inUseChain, firstVisit);

  // Recurse in the enclosing block
  BlockStmt* next  = getVisibilityScopeNoParentModule(block);
  getVisibleFunctionsImpl(name, call, next, visInfo, visited,
                          visibleFns, inUseChain);

  // Also look at the instantiation point
  if (instantiationPt != NULL)
  {
    INT_ASSERT(!inUseChain);
    if (visInfo == NULL)
      getVisibleFunctionsImpl(name, call, // visit all POIs right away
                  instantiationPt, NULL, visited, visibleFns, inUseChain);
    else
      // Executes after recursing to outer/enclosing scopes above.
      // Overwrites instantiationPt from an outer scope, if already in nextPOI.
      visInfo->nextPOI = instantiationPt; // come back to it later
  }
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
    //  within an ArgSymbol). This way we can consult the function's POIs
    // when resolving its ArgSymbol's code ex. in defaultExpr.
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

  if (expr->parentExpr != NULL || ! isModuleSymbol(expr->parentSymbol))
    // If so, we know we are not crossing a module boundary.
    return next;

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
