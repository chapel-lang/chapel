/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef _OPTIMIZATIONS_H_
#define _OPTIMIZATIONS_H_

// If DEBUG_CP > 0, then the debug code is enabled.  Inside of
// copyPropagation.cpp, one still has to set debug=1 to obtain verbose debug
// output.
// IF DEBUG_CP > 1, then calls to copy propagation from other passes are also
// disabled.
// localCopyPropagation is called from several optimization passes, so DEBUG_CP
// was hoisted into this common include file for convenience.
#define DEBUG_CP 0

#include "map.h"
#include "vec.h"

#include <map>
#include <vector>

class BaseAST;
class BitVec;
class BlockStmt;
class FnSymbol;
class Symbol;
class SymExpr;

void removeUnnecessaryGotos(FnSymbol* fn);
void removeUnusedLabels(FnSymbol* fn);
size_t localCopyPropagation(FnSymbol* fn);
size_t globalCopyPropagation(FnSymbol* fn);
void eliminateSingleAssignmentReference(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                        Map<Symbol*,Vec<SymExpr*>*>& useMap,
                                        Symbol* var);
size_t singleAssignmentRefPropagation(FnSymbol* fn);
void deadVariableElimination(FnSymbol* fn);
void deadExpressionElimination(FnSymbol* fn);
void deadCodeElimination(FnSymbol* fn);

void liveVariableAnalysis(FnSymbol* fn,
                          Vec<Symbol*>& locals,
                          Map<Symbol*,int>& localID,
                          Vec<SymExpr*>& useSet,
                          Vec<SymExpr*>& defSet,
                          std::vector<BitVec*>& OUT);

void
buildDefUseChains(FnSymbol* fn,
                  std::map<SymExpr*,Vec<SymExpr*>*>& DU,
                  std::map<SymExpr*,Vec<SymExpr*>*>& UD);

void
freeDefUseChains(std::map<SymExpr*,Vec<SymExpr*>*>& DU,
                 std::map<SymExpr*,Vec<SymExpr*>*>& UD);

void
remoteValueForwarding(Vec<FnSymbol*>& fns);


#endif
