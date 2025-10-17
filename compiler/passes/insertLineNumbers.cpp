/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#include "insertLineNumbers.h"

#include "CallExpr.h"
#include "FnSymbol.h"
#include "astutil.h"
#include "baseAST.h"
#include "driver.h"
#include "expr.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "virtualDispatch.h"

#include <algorithm>
#include <queue>
#include <unordered_set>
#include <vector>

#include "global-ast-vecs.h"

// Contains the old 'insertLineNumbers' pass.
void insertLineNumbers() {
  PassManager pm;

  // TODO: "Run pass on multiple lists...".
  pm.runPass<Symbol*>(AddLineFileInfoToProcPtrTypes(), gArgSymbols);
  pm.runPass<Symbol*>(AddLineFileInfoToProcPtrTypes(), gFnSymbols);
  pm.runPass<Symbol*>(AddLineFileInfoToProcPtrTypes(), gVarSymbols);
  // No need to adjust type symbols at present...
  // pm.runPass<Symbol*>(pass, gTypeSymbols);

  // TODO It's pretty apparent in this refactoring that we don't actually need
  // to recompute all call sites, but only the subset we'll be working on, which
  // is probably better done on demand
  // NOTE this is required for looking up fn->calledBy in
  // InsertLineNumbers::getLineAndFileForFn
  pm.runPass<FnSymbol*>(ComputeCallSitesPass(), gFnSymbols);

  if (!fNoNilChecks) {
    // TODO: this is a temporary workaround to avoid iterating over a vector
    // that changes size during iteration. gCallExprs grows during
    // insertNilChecks because it creates new CallExpr's, but we also know that
    // those new CallExpr's don't need to get processed
    std::vector<CallExpr*> toProcess(gCallExprs.begin(), gCallExprs.end());
    pm.runPass<CallExpr*>(InsertNilChecks(), toProcess);
  }

  // TODO: this is a WIP state on integrating with the PassManager
  // right now, we pass the same instance of InsertLineNumbers so it can
  // maintain state and it handles the queuing
  //
  pm.runPass<FnSymbol*>(InsertLineNumbers(), gFnSymbols);
}
