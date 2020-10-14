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

#ifndef _VISIBLE_FUNCTIONS_H_
#define _VISIBLE_FUNCTIONS_H_

#include "vec.h"

#include <set>

class BlockStmt;
class CallExpr;
class CallInfo;
class Expr;
class FnSymbol;

class VisibilityInfo {
public:
  // for proper scope traversal
  BlockStmt* currStart;
  BlockStmt* nextPOI;

  // for CalledFunInfo
  std::vector<BlockStmt*> visitedScopes; // in visited order  
  std::vector<BlockStmt*> instnPoints;   // one per POI depth
  int poiDepth;
  CallExpr* call;

  VisibilityInfo(CallExpr* call_) :
    currStart(NULL), nextPOI(NULL), poiDepth(-1), call(call_) { }

  bool inPOI() { return poiDepth > 0; }
};

bool isTypeHelperName(const char* fnName);
bool cachedInstantiationIsAlwaysApplicable(FnSymbol* fn);
bool cachedInstantiationIsAlwaysApplicable(CallExpr* call);
bool scopeMayDefineHazard(BlockStmt* scope, const char* fnName);

void       findVisibleFunctionsAllPOIs(CallInfo&       info,
                                       Vec<FnSymbol*>& visibleFns);

void       findVisibleFunctions(CallInfo&             info,
                                VisibilityInfo*       visInfo,
                                std::set<BlockStmt*>* visited,
                                int*                  numVisitedP,
                                Vec<FnSymbol*>&       visibleFns);

void       getVisibleFunctions(const char*                name,
                                CallExpr*                call,
                                VisibilityInfo*          visInfo,
                                std::set<BlockStmt*>*    visited,
                                Vec<FnSymbol*>&          visibleFns);

void       getVisibleFunctions(const char*      name,
                               CallExpr*        call,
                               Vec<FnSymbol*>&  visibleFns);

BlockStmt* getVisibilityScope(Expr* expr);
BlockStmt* getInstantiationPoint(Expr* expr);

void       initTypeHelperNames();
void       visibleFunctionsClear();

#endif
