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

#ifndef _VISIBILE_CANDIDATES_H_
#define _VISIBILE_CANDIDATES_H_

#include "vec.h"

#include <vector>

class ArgSymbol;
class BlockStmt;
class CallInfo;
class FnSymbol;
class ResolutionCandidate;

void      findVisibleCandidates(CallInfo&                  info,
                                Vec<FnSymbol*>&            visibleFns,
                                Vec<ResolutionCandidate*>& candidates);

void      resolveTypedefedArgTypes(FnSymbol* fn);

FnSymbol* expandIfVarArgs(FnSymbol* origFn,
                          int       numActuals);

void      substituteVarargTupleRefs(BlockStmt*               ast,
                                    int                      numArgs,
                                    ArgSymbol*               formal,
                                    std::vector<ArgSymbol*>& varargFormals);

bool      checkResolveFormalsWhereClauses(ResolutionCandidate* currCandidate);

bool      checkGenericFormals(ResolutionCandidate* currCandidate);

#endif
