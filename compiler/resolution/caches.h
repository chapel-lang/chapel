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
#include <vector>
#include <map>

typedef std::vector<int> SymbolMapCacheKey;
typedef std::map<SymbolMapCacheKey, FnSymbol*> SymbolMapCache;

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

FnSymbol*& lookupCache(SymbolMapCache& cache, FnSymbol* fn, SymbolMap* map);
void      freeCache(SymbolMapCache& cache);

#endif
