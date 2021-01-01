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

#include "caches.h"

#include "callInfo.h"
#include "ResolutionCandidate.h"
#include "stmt.h"
#include "stringutil.h"
#include "visibleFunctions.h"
#include "view.h"


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

SymbolMapCache promotionsCache;

static bool isCacheEntryMatch(SymbolMap* s1, SymbolMap* s2);

SymbolMapCacheEntry::SymbolMapCacheEntry(FnSymbol* ifn, SymbolMap* imap) :
  fn(ifn), map(*imap) { }


void
addCache(SymbolMapCache& cache,
         FnSymbol*       oldFn,
         FnSymbol*       fn,
         SymbolMap*      map) {
  Vec<SymbolMapCacheEntry*>* entries = cache.get(oldFn);
  SymbolMapCacheEntry*       entry   = new SymbolMapCacheEntry(fn, map);

  if (entries) {
    entries->add(entry);

  } else {
    entries = new Vec<SymbolMapCacheEntry*>();
    entries->add(entry);
    cache.put(oldFn, entries);
  }
}


FnSymbol*
checkCache(SymbolMapCache& cache, FnSymbol* oldFn, SymbolMap* map) {
  if (Vec<SymbolMapCacheEntry*>* entries = cache.get(oldFn)) {
    forv_Vec(SymbolMapCacheEntry, entry, *entries) {
      if (isCacheEntryMatch(map, &entry->map))
        return entry->fn;
    }
  }
  return NULL;
}


void
freeCache(SymbolMapCache& cache) {
  form_Map(SymbolMapCacheElem, elem, cache) {
    forv_Vec(SymbolMapCacheEntry, entry, *elem->value) {
      delete entry;
    }
    delete elem->value;
  }
  cache.clear();
}

static bool isCacheEntryMatch(SymbolMap* s1, SymbolMap* s2) {
  form_Map(SymbolMapElem, e, *s1) {
    if (s2->get(e->key) != e->value) {
      return false;
    }
  }

  form_Map(SymbolMapElem, e, *s2) {
    if (s1->get(e->key) != e->value) {
      return false;
    }
  }

  return true;
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

//
// CalledFunInfo: information about one best candidate during resolution.
// CalledFunInfo entries are stored in FnSymbol->cacheInfo.
//
// Cf. a "VisibilityInfo" participates in gathering visible functions
// when resolving a call within that FnSymbol, then feeds into a CalledFunInfo.
//
class CalledFunInfo {
public:
  const char* fnName;       // the name being called
  BlockStmt* declScope;          // the scope where the candidate is declared
  std::set<BlockStmt*> visitedScopes; // scopes visited in getVisibleFunctions

  // Is this CFI's function defined in the 'scope'?
  inline bool isDeclarationScope(BlockStmt* scope) {
    return scope == declScope;
  }

  // Was the 'scope' visited while resolving the call to this CFI's function?
  inline bool inVisitedScopes(BlockStmt* scope) {
    return visitedScopes.find(scope) != visitedScopes.end();
  }

  // Does the 'scope' define a fn that might change resolution outcome?
  bool mayDefineHazard(BlockStmt* scope) {
    return scopeMayDefineHazard(scope, fnName);
  }

  CalledFunInfo(const char* funName, BlockStmt* dScope,
                std::vector<BlockStmt*> vScopes):
    fnName(funName),
    declScope(dScope),
    visitedScopes(vScopes.begin(), vScopes.end())
  { }
};


//
// Information about all best candidates while resolving this fn.
//
class GenericsCacheInfo {
public:
  std::vector<CalledFunInfo> infos;
  int size() { return (int)(infos.size()); }
};


//
// genericsCacheSummary(): summarize various GC-related things for debugging
//

void genericsCacheSummary(CalledFunInfo* fi) {
  if (fi == NULL) printf("<NULL>\n");
  else genericsCacheSummary(*fi);
}

void genericsCacheSummary(CalledFunInfo& fi) {
  printf("CFI \"%s\"  declScope %d %s  visitedScopes %d:", fi.fnName,
         fi.declScope->id, debugLoc(fi.declScope),
         (int)fi.visitedScopes.size());
  for_set(BlockStmt, block, fi.visitedScopes) printf(" %d", block->id);
  printf("\n");
}

void genericsCacheSummary(GenericsCacheInfo* ci) {
  if (ci == NULL) printf("<NULL>\n");
  else genericsCacheSummary(*ci);
}

void genericsCacheSummary(GenericsCacheInfo& ci) {
  printf("GenericsCacheInfo {\n");
  for (std::vector<CalledFunInfo>::iterator it = ci.infos.begin();
       it != ci.infos.end(); it++) {
    printf("  ");
    genericsCacheSummary(*it);
  }
  printf("GenericsCacheInfo }  %d infos\n\n", ci.size());
}

void genericsCacheSummary(VisibilityInfo* visInfo) {
  if (visInfo == NULL) printf("<NULL>\n");
  else genericsCacheSummary(*visInfo);
}

// todo also show visInfo.currStart, visInfo.nextPOI?
void genericsCacheSummary(VisibilityInfo& visInfo) {
  printf("{visitedScopes %d:", (int)visInfo.visitedScopes.size());
  for_vector(BlockStmt, block, visInfo.visitedScopes)
    printf(" %d", block->id);
  printf("\n");

  printf("instnPoints %d:\n", (int)visInfo.instnPoints.size());
  for_vector(BlockStmt, block, visInfo.instnPoints)
    printf("  %d %s\n", block->id, debugLoc(block));

  printf("}  poiDepth %d   call %d %s\n", visInfo.poiDepth,
         visInfo.call->id, debugLoc(visInfo.call));
}

void genericsCacheSummary(int id) {
  if (BaseAST* ast = aid09(id)) genericsCacheSummary(ast);
  else printf("genericsCacheSummary: id %d"
              "does not correspond to an AST node\n", id);
}

void genericsCacheSummary(BaseAST* ast) {
  if (FnSymbol* fn = toFnSymbol(ast)) {
    if (GenericsCacheInfo* ci = fn->cacheInfo) genericsCacheSummary(ci);
    else printf("fn %s[%d] has null cacheInfo\n", fn->name, fn->id);
  } else if (DefExpr* def = toDefExpr(ast)) {
    if (FnSymbol* fn = toFnSymbol(def->sym)) genericsCacheSummary(fn);
    else printf("def %d  defines %s[%d] that is a %s, not a fn\n",
            def->id, def->sym->name, def->sym->id, def->sym->astTagAsString());
  } else {
    printf("genericsCacheSummary: %d is a %s, not a fn or def\n",
           ast->id, ast->astTagAsString());
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

SymbolMapScopeCache genericsCache;

SymbolMapScopeCacheEntry::SymbolMapScopeCacheEntry(FnSymbol* ifn, SymbolMap* imap) :
  fn(ifn), map(*imap) { }

void
addCache(SymbolMapScopeCache& cache,
         FnSymbol*       oldFn,
         FnSymbol*       fn,
         SymbolMap*      map) {
  Vec<SymbolMapScopeCacheEntry*>* entries = cache.get(oldFn);
  SymbolMapScopeCacheEntry*       entry = new SymbolMapScopeCacheEntry(fn, map);

  if (entries) {
    entries->add(entry);

  } else {
    entries = new Vec<SymbolMapScopeCacheEntry*>();
    entries->add(entry);
    cache.put(oldFn, entries);
  }
}


static bool isApplicableInstantiation(VisibilityInfo& visInfo,
                                      FnSymbol* cgi);

FnSymbol*
checkCache(SymbolMapScopeCache& cache, FnSymbol* oldFn,
           VisibilityInfo* visInfo, SymbolMap* map)
{
  if (Vec<SymbolMapScopeCacheEntry*>* entries = cache.get(oldFn)) {
    forv_Vec(SymbolMapScopeCacheEntry, entry, *entries) {
      if (isCacheEntryMatch(map, &entry->map) &&
          (visInfo == NULL || isApplicableInstantiation(*visInfo, entry->fn)) )
        return entry->fn;
    }
  }

  return NULL;
}


void
freeCache(SymbolMapScopeCache& cache) {
  form_Map(SymbolMapScopeCacheElem, elem, cache) {
    forv_Vec(SymbolMapScopeCacheEntry, entry, *elem->value) {
      delete entry;
    }
    delete elem->value;
  }
  cache.clear();
}

//
// GenericsCacheInfo interface
//

void createCacheInfoIfNeeded(FnSymbol* fn) {
  INT_ASSERT(fn->cacheInfo == NULL);
  if (fn->instantiatedFrom == NULL)
    // We will not be checking applicability for a concrete function
    // because there will be no cached instantiations.
    // So, do not compute cacheInfo.
    return;

  fn->cacheInfo = new GenericsCacheInfo();
}

void clearCacheInfoIfEmpty(FnSymbol* fn) {
  if (GenericsCacheInfo* cacheInfo = fn->cacheInfo) {
    if (cacheInfo->infos.empty()) {
      delete cacheInfo;
      fn->cacheInfo = NULL; // Make future checks cheaper.
    }
  }
}

void cleanupCacheInfo(FnSymbol* fn) {
  delete fn->cacheInfo;
  fn->cacheInfo = NULL;
}

// Add to parentInfos the cacheInfo of the FnSymbol containing 'expr'.
// If the FnSymbol does not have a cacheInfo, return false.
static bool addOneParentInfo(std::vector<GenericsCacheInfo*>& parentInfos,
                             Expr* expr) {
  FnSymbol* parentFn = expr->getFunction();
  if (parentFn->cacheInfo == NULL)
    return false;

  parentInfos.push_back(parentFn->cacheInfo);
  return true;
}

// Generate a CalledFunInfo for 'candidFn' based on 'visInfo'
// and add it to all cacheInfos in 'parentInfos'.
static void updateOneCandidate(std::vector<GenericsCacheInfo*>& parentInfos,
                               FnSymbol* candidFn, VisibilityInfo& visInfo) {
  if (candidFn->hasFlag(FLAG_COBEGIN_OR_COFORALL) ||
      candidFn->hasFlag(FLAG_BEGIN) ||
      candidFn->hasFlag(FLAG_ON))
    return; // do not record

  CalledFunInfo cfi(candidFn->name, getVisibilityScope(candidFn->defPoint),
                    visInfo.visitedScopes);

  // Performance opt: skip this loop if the previous "best" candidate
  // had the same name and scope. ("scope" means its defPoint->parentExpr.)

  for_vector(GenericsCacheInfo, pInfo, parentInfos)
    pInfo->infos.push_back(cfi);
}

//
// The call 'visInfo.call' resolves to the candidate(s) visible through
// (one of) its POIs. So, 'visInfo.call' must be in an instantiation
// of a generic function; we have cached this instantiation in genericsCache.
// For this cache entry to be applicable to another call in the future, those
// candidates will need to be visible from that call or its POI(s).
//
// Record this requirement in the cacheInfos of the FnSymbols containing
// visInfo.call and the POI(s). The requirement represented as a CalledFunInfo
// within FnSymbol->cacheInfo. The CalledFunInfo is generated based on
// 'visInfo' in updateOneCandidate().
//
void updateCacheInfosForACall(VisibilityInfo& visInfo,
                              ResolutionCandidate* best1,
                              ResolutionCandidate* best2,
                              ResolutionCandidate* best3) {
  INT_ASSERT(visInfo.poiDepth <= (int)visInfo.instnPoints.size());

  //
  // 'parentInfos' contains the cacheInfos to be updated.
  // We precompute this set as an optimization, so that updateOneCandidate
  // can compute the CalledFunInfo once and reuse it for all cacheInfos.
  //
  std::vector<GenericsCacheInfo*> parentInfos;
  parentInfos.reserve(visInfo.poiDepth);

  if (!addOneParentInfo(parentInfos, visInfo.call))
    // We hit a special method or a method-like function.
    // Currently these are always applicable.
    // Do not impose CFI constraint on it or its callers.
    return;

  for (int depth = 0; depth < visInfo.poiDepth - 1; depth++)
    if (!addOneParentInfo(parentInfos, visInfo.instnPoints[depth]))
      break;

  if (best1) updateOneCandidate(parentInfos, best1->fn, visInfo);
  if (best2) updateOneCandidate(parentInfos, best2->fn, visInfo);
  if (best3) updateOneCandidate(parentInfos, best3->fn, visInfo);
}

//
// isApplicableInstantiation() and helpers
//

//
// Check how visInfo.visitedScopes, starting from 'startVS', cover the CFIs
// in 'toProcess'. Nullify the CFIs that have been satisfied and decrement
// 'remainingCFIs' correspondingly. If we encounter a scope that might make
// this cached instantiation not applicable, return false.
//
static bool analyzeVisitedScopes(std::vector<CalledFunInfo*>& toProcess,
                                              VisibilityInfo& visInfo,
                                                         int& remainingCFIs,
                                                         int  startVS)
{
  int sizeCI = (int)toProcess.size(); // usu. at most a couple of elements
  int sizeVS = (int)visInfo.visitedScopes.size();

  // Visit the scopes in the order they would be visited from fn's POI #1.
  // For each CFI, check whether the result of getVisibleFunctions()
  // that was done to resolve that call remains unchanged for this callsite.
  for (int iVS = startVS; iVS < sizeVS; iVS++) {
    BlockStmt* scope = visInfo.visitedScopes[iVS];

    // See how each CFI does w.r.t. this scope.
    for (int iCFI = 0; iCFI < sizeCI; iCFI++) {
      // Skip the CFIs that we have already checked off.
      if (CalledFunInfo* cfi = toProcess[iCFI]) {

        if (cfi->isDeclarationScope(scope)) {
          // We reached the scope where 'cfi' is defined.
          // No more checking for this 'cfi' is needed. Check it off.
          toProcess[iCFI] = NULL;
          remainingCFIs--;
          continue;
        }

        if (cfi->inVisitedScopes(scope)) {
          // 'fn' visited this scope during resolution. OK so far.

        } else if (!cfi->mayDefineHazard(scope)) {
          // This scope does not define a hazard. OK so far.

        } else {
          // This scope defines something that might change the outcome of
          // resolving the call represented by 'cfi'. In which case reusing
          // this cached instantiation would not be correct.
          // Conservatively, report this cached instantiation not applicable.
          return true;
        }
      }
    } // done traversing 'toProcess'

    if (remainingCFIs == 0) break; // approved all CFIs

  } // done traversing visInfo.visitedScopes

  return false; // no hazard scopes have been found
}

//
// The generic instantiation, here represented by 'toProcess', looks promising
// so far. Visit additional POI(s), looking for the scopes of remaining CFIs.
// If we skip this step, the compiler gets into infinite recursion in
//   test/functions/generic/poi/expBySquaring-repro.chpl
//
static void visitMorePOIs(std::vector<CalledFunInfo*>& toProcess,
                          VisibilityInfo&              visInfoOrig,
                          int&                         remainingCFIs)
{
// Implementation: we call getVisibleFunctions() to get the official order of
// scope traversal, following the steps in findVisibleFunctionsAndCandidates().
// All we need from getVisibleFunctions is a vector of scopes,
// so use a dummy name to avoid useless picking up any visible functions.
//
// Also we do not want to modify the original 'visInfo' and 'visited', as they
// may be needed for further *real* calls to findVisibleFunctions(), ex. if
// the current candidate ends up not applicable. So, use temporary variables.

  int numVisitedVis = 0;
  const char* dummyName = astr("");
  VisibilityInfo visInfo(visInfoOrig);
  advanceCurrStart(visInfo);

  Vec<FnSymbol*> visibleFns;
  std::set<BlockStmt*> visited(visInfoOrig.visitedScopes.begin(),
                               visInfoOrig.visitedScopes.end());
  do {
    visInfo.poiDepth++;

    getMoreVisibleFunctionsOrMethods(dummyName, visInfoOrig.call,
                        &visInfo, &visited, visibleFns);

    if (analyzeVisitedScopes(toProcess, visInfo, remainingCFIs, numVisitedVis))
      return;

    numVisitedVis = (int)visInfo.visitedScopes.size();
    advanceCurrStart(visInfo);
  }
  while
    (remainingCFIs > 0 && visInfo.currStart != NULL);
}

//
// Is the cached generic instantiation 'fn' applicable to the call
// described by 'visInfo' ?
//
static bool isApplicableInstantiation(VisibilityInfo& visInfo, FnSymbol* fn)
{
  GenericsCacheInfo* cacheInfo = fn->cacheInfo;
  if (cacheInfo == NULL) return true;
  int sizeCI = cacheInfo->size();

  int remainingCFIs = sizeCI;
  std::vector<CalledFunInfo*> toProcess(sizeCI); // working copy
  for (int i = 0; i < sizeCI; i++) toProcess[i] = &(cacheInfo->infos[i]);

  if (analyzeVisitedScopes(toProcess, visInfo, remainingCFIs, 0))
      return false;

  if (remainingCFIs > 0 && visInfo.nextPOI != NULL)
    visitMorePOIs(toProcess, visInfo, remainingCFIs);

  // Return true if we have found the declaring scopes of all CFIs.
  return remainingCFIs == 0;
}
