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

typedef Map<FnSymbol*,     Vec<SymbolMapCacheEntry*>*> SymbolMapCache;
typedef MapElem<FnSymbol*, Vec<SymbolMapCacheEntry*>*> SymbolMapCacheElem;


void      addCache(SymbolMapCache& cache,
                   FnSymbol*       oldFn,
                   FnSymbol*       newFn,
                   SymbolMap*      map);

FnSymbol* checkCache(SymbolMapCache& cache,
                     FnSymbol*       oldFn,
                     SymbolMap*      map);

void      replaceCache(SymbolMapCache& cache,
                       FnSymbol*       oldFn,
                       FnSymbol*       newFn,
                       SymbolMap*      map);

void      freeCache(SymbolMapCache& cache);

//
// Caches to avoid creating multiple identical wrappers and
// instantiating the same functions in the same ways
//
// Note that these caches are necessary for correctness, not just
// performance, because they can impact the types that are created
// when instantiating constructors and building up the default
// wrappers for constructors.
//

extern SymbolMapCache genericsCache;
extern SymbolMapCache promotionsCache;







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

  FnSymbol*    fn;
  Vec<Symbol*> vec;
};

typedef Map<FnSymbol*,     Vec<SymbolVecCacheEntry*>*> SymbolVecCache;
typedef MapElem<FnSymbol*, Vec<SymbolVecCacheEntry*>*> SymbolVecCacheElem;


void      addCache(SymbolVecCache& cache,
                   FnSymbol*       newFn,
                   FnSymbol*       oldFn,
                   Vec<Symbol*>* vec);

FnSymbol* checkCache(SymbolVecCache& cache,
                     FnSymbol*       fn,
                     Vec<Symbol*>*   vec);

void      freeCache(SymbolVecCache& cache);

//
// Caches to avoid creating multiple identical wrappers and
// instantiating the same functions in the same ways
//
// Note that these caches are necessary for correctness, not just
// performance, because they can impact the types that are created
// when instantiating constructors and building up the default
// wrappers for constructors.
//
extern SymbolVecCache defaultsCache;

#endif
