#include "astutil.h"
#include "caches.h"
#include "stmt.h"
#include "stringutil.h"


SymbolMapCacheEntry::SymbolMapCacheEntry(FnSymbol* ifn, SymbolMap* imap) :
  fn(ifn), map(*imap) { }


void
addCache(SymbolMapCache& cache, FnSymbol* oldFn, FnSymbol* fn, SymbolMap* map) {
  Vec<SymbolMapCacheEntry*>* entries = cache.get(oldFn);
  if (entries) {
    entries->add(new SymbolMapCacheEntry(fn, map));
  } else {
    entries = new Vec<SymbolMapCacheEntry*>();
    entries->add(new SymbolMapCacheEntry(fn, map));
    cache.put(oldFn, entries);
  }
}


static bool
isCacheEntryMatch(SymbolMap* s1, SymbolMap* s2) {
  form_Map(SymbolMapElem, e, *s1)
    if (s2->get(e->key) != e->value)
      return false;
  form_Map(SymbolMapElem, e, *s2)
    if (s1->get(e->key) != e->value)
      return false;
  return true;
}


FnSymbol*
checkCache(SymbolMapCache& cache, FnSymbol* oldFn, SymbolMap* map) {
  if (Vec<SymbolMapCacheEntry*>* entries = cache.get(oldFn)) {
    forv_Vec(SymbolMapCacheEntry, entry, *entries) {
      if (isCacheEntryMatch(map, &entry->map)) {
        return entry->fn;
      }
    }
  }
  return NULL;
}


void
freeCache(SymbolMapCache& cache) {
  Vec<FnSymbol*> keys;
  cache.get_keys(keys);
  forv_Vec(FnSymbol, key, keys) {
    Vec<SymbolMapCacheEntry*>* entries = cache.get(key);
    forv_Vec(SymbolMapCacheEntry, entry, *entries) {
      delete entry;
    }
    delete entries;
  }
  cache.set_clear();
}


SymbolVecCacheEntry::SymbolVecCacheEntry(FnSymbol* ifn, Vec<Symbol*>* ivec) :
  fn(ifn), vec(*ivec) { }


void
addCache(SymbolVecCache& cache, FnSymbol* oldFn, FnSymbol* fn, Vec<Symbol*>* vec) {
  Vec<SymbolVecCacheEntry*>* entries = cache.get(oldFn);
  if (entries) {
    entries->add(new SymbolVecCacheEntry(fn, vec));
  } else {
    entries = new Vec<SymbolVecCacheEntry*>();
    entries->add(new SymbolVecCacheEntry(fn, vec));
    cache.put(oldFn, entries);
  }
}


static bool 
isCacheEntryMatch(Vec<Symbol*>* vec1, Vec<Symbol*>* vec2) {
  forv_Vec(Symbol, d, *vec1)
    if (!vec2->in(d))
      return false;
  forv_Vec(Symbol, d, *vec2)
    if (!vec1->in(d))
      return false;
  return true;
}


FnSymbol*
checkCache(SymbolVecCache& cache, FnSymbol* fn, Vec<Symbol*>* vec) {
  if (Vec<SymbolVecCacheEntry*>* entries = cache.get(fn)) {
    forv_Vec(SymbolVecCacheEntry, entry, *entries) {
      if (isCacheEntryMatch(vec, &entry->vec)) {
        return entry->fn;
      }
    }
  }
  return NULL;
}


void
freeCache(SymbolVecCache& cache) {
  Vec<FnSymbol*> keys;
  cache.get_keys(keys);
  forv_Vec(FnSymbol, key, keys) {
    Vec<SymbolVecCacheEntry*>* entries = cache.get(key);
    forv_Vec(SymbolVecCacheEntry, entry, *entries) {
      delete entry;
    }
    delete entries;
  }
  cache.set_clear();
}


SymbolMapCache genericsCache;
SymbolMapCache coercionsCache;
SymbolMapCache promotionsCache;
SymbolVecCache defaultsCache;
