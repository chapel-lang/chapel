/*
 * Copyright 2004-2017 Cray Inc.
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

//
// The virtualMethodTable maps types to their arrays of methods.  The
// virtualMethodMap maps methods to their indexes into these arrays.
// The virtualChildrenMap maps methods to all of the methods that
// could be called when they are called.  The virtualRootsMap maps
// methods to the root methods that it overrides.  Note that multiple
// inheritance will require more virtual method tables, one for each
// path up the class hierarchy to each class root.
//

extern Map<FnSymbol*, int>             virtualMethodMap;
extern Map<Type*,     Vec<FnSymbol*>*> virtualMethodTable;
extern Map<FnSymbol*, Vec<FnSymbol*>*> virtualChildrenMap;
extern Map<FnSymbol*, Vec<FnSymbol*>*> virtualRootsMap;

extern bool                            inDynamicDispatchResolution;

void resolveDynamicDispatches();

void insertDynamicDispatchCalls();

#endif
