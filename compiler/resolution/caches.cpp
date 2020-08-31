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


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

SymbolMapCache genericsCache;
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
