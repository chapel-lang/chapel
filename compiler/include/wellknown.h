/*
 * Copyright 2004-2020 Cray Inc.
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

#ifndef _WELL_KNOWN_H_
#define _WELL_KNOWN_H_

#include <vector>

class AggregateType;
class FnSymbol;

void gatherIteratorTags();
void gatherWellKnownTypes();
void gatherWellKnownFns();

std::vector<FnSymbol*> getWellKnownFunctions();
void clearGenericWellKnownFunctions();

// The well-known types
extern AggregateType* dtArray;
extern AggregateType* dtBaseArr;
extern AggregateType* dtBaseDom;
extern AggregateType* dtCFI_cdesc_t;
extern AggregateType* dtDist;
extern AggregateType* dtError;
extern AggregateType* dtExternalArray;
extern AggregateType* dtLocaleID;
extern AggregateType* dtMainArgument;
extern AggregateType* dtOnBundleRecord;
extern AggregateType* dtOpaqueArray;
extern AggregateType* dtTaskBundleRecord;
extern AggregateType* dtTuple;
extern AggregateType* dtRef;

// The well-known functions
extern FnSymbol *gChplHereAlloc;
extern FnSymbol *gChplHereFree;
extern FnSymbol *gChplDecRunningTask;
extern FnSymbol *gChplIncRunningTask;
extern FnSymbol *gChplDoDirectExecuteOn;
extern FnSymbol *gBuildTupleType;
extern FnSymbol *gBuildTupleTypeNoRef;
extern FnSymbol *gBuildStarTupleType;
extern FnSymbol *gBuildStarTupleTypeNoRef;
extern FnSymbol *gPrintModuleInitFn;
extern FnSymbol *gGetDynamicEndCount;
extern FnSymbol *gSetDynamicEndCount;
extern FnSymbol *gChplDeleteError;
extern FnSymbol *gChplUncaughtError;
extern FnSymbol *gChplPropagateError;
extern FnSymbol *gChplSaveTaskError;
extern FnSymbol *gChplForallError;
extern FnSymbol *gAtomicFenceFn;
extern FnSymbol *gChplAfterForallFence;
extern FnSymbol *gChplCreateStringWithLiteral;

#endif
