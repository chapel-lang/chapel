#ifndef _CACHES_H_
#define _CACHES_H_

#include "baseAST.h"

class Inst {
 public:
  Inst(FnSymbol* iNewFn, SymbolMap* iSubs) : newFn(iNewFn), subs(*iSubs) { }
  FnSymbol* newFn;
  SymbolMap subs;
};

extern Map<FnSymbol*,Vec<Inst*>*> icache; // instantiation cache
extern Map<FnSymbol*,Vec<Inst*>*> cw_cache; // coercion wrappers cache
extern Map<FnSymbol*,Vec<Inst*>*> pw_cache; // promotion wrappers cache

void addMapCache(Map<FnSymbol*,Vec<Inst*>*>& cache, FnSymbol* oldFn, FnSymbol* newFn, SymbolMap* subs);
FnSymbol* checkMapCache(Map<FnSymbol*,Vec<Inst*>*>& cache, FnSymbol* oldFn, SymbolMap* subs);
void freeSymbolMapCache(Map<FnSymbol*,Vec<Inst*>*>& cache);

class DWCacheItem {
 public:
  DWCacheItem(FnSymbol* iOldFn, FnSymbol* iNewFn, Vec<Symbol*>* iDefaults) :
    oldFn(iOldFn), newFn(iNewFn), defaults(new Vec<Symbol*>(*iDefaults)) { }
  FnSymbol* oldFn;
  FnSymbol* newFn;
  Vec<Symbol*>* defaults;
};

extern Vec<DWCacheItem*> dwcache; // default wrappers cache

FnSymbol* check_dwcache(FnSymbol* fn, Vec<Symbol*>* defaults);
void add_dwcache(FnSymbol* newFn, FnSymbol* oldFn, Vec<Symbol*>* defaults);

#endif
