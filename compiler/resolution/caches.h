#ifndef _CACHES_H_
#define _CACHES_H_

#include "baseAST.h"

//
// SymbolMapCache: FnSymbol -> FnSymbol cache based on a SymbolMap
//
//   addCache(cache, old_fn, new_fn, map): adds an entry to cache from
//                                         old_fn to new_fn via map
//
//   checkCache(cache, fn, map): returns a function previously added
//                               via addMapCache if fn matches old_fn
//                               and the maps contain the same
//                               key-value pairs (in any order)
//
//   freeCache(cache): frees memory associated with cache
//
class SymbolMapCacheEntry {
 public:
  SymbolMapCacheEntry(FnSymbol* ifn, SymbolMap* imap);
  FnSymbol* fn;
  SymbolMap map;
};
typedef Map<FnSymbol*,Vec<SymbolMapCacheEntry*>*> SymbolMapCache;
typedef MapElem<FnSymbol*,Vec<SymbolMapCacheEntry*>*> SymbolMapCacheElem;


void addCache(SymbolMapCache& cache, FnSymbol* old, FnSymbol* fn, SymbolMap* map);
FnSymbol* checkCache(SymbolMapCache& cache, FnSymbol* oldFn, SymbolMap* map);
void replaceCache(SymbolMapCache& cache, FnSymbol* old, FnSymbol* fn, SymbolMap* map);
void freeCache(SymbolMapCache& cache);

//
// SymbolVecCache: FnSymbol -> FnSymbol cache based on a Vec<Symbol*>
//
//   This cache works similarly to the SymbolMapCache above except
//   instead of a map, the cache is based on a vector.  The cache
//   entries match if the functions are the same and the vectors
//   contain the same elements (in any order).
//
class SymbolVecCacheEntry {
 public:
  SymbolVecCacheEntry(FnSymbol* fn, Vec<Symbol*>* ivec);
  FnSymbol* fn;
  Vec<Symbol*> vec;
};
typedef Map<FnSymbol*,Vec<SymbolVecCacheEntry*>*> SymbolVecCache;
typedef MapElem<FnSymbol*,Vec<SymbolVecCacheEntry*>*> SymbolVecCacheElem;

void addCache(SymbolVecCache& cache, FnSymbol* newFn, FnSymbol* oldFn, Vec<Symbol*>* vec);
FnSymbol* checkCache(SymbolVecCache& cache, FnSymbol* fn, Vec<Symbol*>* vec);
void freeCache(SymbolVecCache& cache);

//
// Caches to avoid creating multiple identical wrappers and
// instantiating the same functions in the same ways
//
// Note that these caches are necessary for correctness, not just
// performance, because they can impact the types that are created
// when instantiating constructors and building up the default
// wrappers for constructors.
//
extern SymbolMapCache ordersCache;
extern SymbolMapCache genericsCache;
extern SymbolMapCache coercionsCache;
extern SymbolMapCache promotionsCache;
extern SymbolVecCache defaultsCache;

#endif
