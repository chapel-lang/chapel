/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _VIRTUAL_DISPATCH_H_
#define _VIRTUAL_DISPATCH_H_

#include "map.h"
#include "vec.h"

class FnSymbol;
class Type;

// Map a method to the set of methods being overridden
extern Map<FnSymbol*, Vec<FnSymbol*>*> virtualRootsMap;

// Map a method to the set of methods that it may override
extern Map<FnSymbol*, Vec<FnSymbol*>*> virtualParentsMap;

// Map a method to the set of methods that might be invoked
extern Map<FnSymbol*, Vec<FnSymbol*>*> virtualChildrenMap;

// Map types to arrays of virtual methods
extern Map<Type*,     Vec<FnSymbol*>*> virtualMethodTable;

// Map a method to its index within the array of methods
extern Map<FnSymbol*, int>             virtualMethodMap;

extern bool                            inDynamicDispatchResolution;

void resolveDynamicDispatches();

void insertDynamicDispatchCalls();

#endif
