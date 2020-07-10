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

#include "caches.h"
#include "stmt.h"
#include <algorithm>

SymbolMapCache genericsCache;
SymbolMapCache promotionsCache;

typedef std::pair<int,int> IntPair;
typedef std::vector<IntPair> PairVec;

static bool PairCompare(IntPair p1, IntPair p2) {
  return p1.first < p2.first;
}

FnSymbol*& lookupCache(SymbolMapCache& cache, FnSymbol* fn, SymbolMap* map) {
/*
First, build the key, which is the pair (fn, map). It is represented
as a sequence of node IDs for the following nodes

key[0] fn
key[1] the 1st key in the map
key[2] the 1st value in the map
...
key[2*n-1] the last key in the map
key[2*n]   the last value in the map

The (key,value) pairs are ordered in the order of key's ID.
*/
  int mapN = map->n; // this count includes null slots in 'map'
  int mapC = 0;      // this will count the filled slots

  // Convert 'map to a vector of (key id, value id) pairs, for sorting.
  PairVec toSort(mapN);
  // iterate over all (key, value) pairs
  for (int i = 0; i < mapN; i++) {
    SymbolMapElem& elem = map->v[i];
    if (elem.key) {
      toSort[mapC].first = elem.key->id;
      toSort[mapC].second = elem.value->id;
      mapC++;
    }
  }
  if (mapC > 1)
    std::sort(toSort.begin(), toSort.begin() + mapC, PairCompare);

  // Copy the sorted vector into the key.
  SymbolMapCacheKey cacheKey(1 + 2*mapC);
  cacheKey[0] = fn->id;
  for (int i = 0; i < mapC; i++)
    cacheKey[2*i + 1] = toSort[i].first,
    cacheKey[2*i + 2] = toSort[i].second;

  // Look up the key.
  return cache[cacheKey];
}

void
freeCache(SymbolMapCache& cache) {
  // todo: do we want to keep our caches after resolution?
  cache.clear();
}
