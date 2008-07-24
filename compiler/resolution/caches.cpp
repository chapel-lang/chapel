#include "astutil.h"
#include "caches.h"
#include "stmt.h"
#include "stringutil.h"


Map<FnSymbol*,Vec<Inst*>*> icache; // instantiation cache
Map<FnSymbol*,Vec<Inst*>*> cw_cache; // coercion wrappers cache
Map<FnSymbol*,Vec<Inst*>*> pw_cache; // promotion wrappers cache
Vec<DWCacheItem*> dwcache; // default wrappers cache


static bool 
subs_match(SymbolMap* s1, SymbolMap* s2) {
  form_Map(SymbolMapElem, e, *s1)
    if (s2->get(e->key) != e->value)
      return false;
  form_Map(SymbolMapElem, e, *s2)
    if (s1->get(e->key) != e->value)
      return false;
  return true;
}


void
addMapCache(Map<FnSymbol*,Vec<Inst*>*>& cache,
            FnSymbol* oldFn, FnSymbol* newFn, SymbolMap* subs) {
  Vec<Inst*>* lines = cache.get(oldFn);
  if (lines) {
    lines->add(new Inst(newFn, subs));
  } else {
    lines = new Vec<Inst*>();
    lines->add(new Inst(newFn, subs));
    cache.put(oldFn, lines);
  }
}


FnSymbol*
checkMapCache(Map<FnSymbol*,Vec<Inst*>*>& cache, FnSymbol* oldFn, SymbolMap* subs) {
  if (Vec<Inst*>* lines = cache.get(oldFn)) {
    forv_Vec(Inst, inst, *lines) {
      if (subs_match(subs, &inst->subs)) {
        return inst->newFn;
      }
    }
  }
  return NULL;
}


void
freeSymbolMapCache(Map<FnSymbol*,Vec<Inst*>*>& cache) {
  Vec<FnSymbol*> keys;
  cache.get_keys(keys);
  forv_Vec(FnSymbol, key, keys) {
    Vec<Inst*>* insts = cache.get(key);
    forv_Vec(Inst, inst, *insts) {
      delete inst;
    }
    delete insts;
  }
  cache.set_clear();
}


static bool 
dw_match(Vec<Symbol*>* d1, Vec<Symbol*>* d2) {
  forv_Vec(Symbol, d, *d1)
    if (!d2->in(d))
      return false;
  forv_Vec(Symbol, d, *d2)
    if (!d1->in(d))
      return false;
  return true;
}

FnSymbol*
check_dwcache(FnSymbol* fn, Vec<Symbol*>* defaults) {
  forv_Vec(DWCacheItem, item, dwcache)
    if (item->oldFn == fn && dw_match(defaults, item->defaults))
      return item->newFn;
  return NULL;
}

void
add_dwcache(FnSymbol* newFn, FnSymbol* oldFn, Vec<Symbol*>* defaults) {
  dwcache.add(new DWCacheItem(oldFn, newFn, defaults));
}
