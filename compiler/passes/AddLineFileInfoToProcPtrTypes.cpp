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

#include "pass-manager-passes.h"

#include "CallExpr.h"
#include "FnSymbol.h"
#include "astutil.h"
#include "baseAST.h"
#include "driver.h"
#include "expr.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "virtualDispatch.h"

#include <algorithm>
#include <queue>
#include <unordered_set>
#include <vector>

namespace {
  // As a means of abbreviation.
  using Pass = AddLineFileInfoToProcPtrTypes;
}

bool Pass::shouldProcess(Symbol* sym) {
  return !isFnSymbol(sym) && isFunctionType(sym->type);
}

// This is used by "adjustSymbolType". Note that all the type constructor
// does is append the line/file formals to the end of the type - there is
// no check to see if they have already been appended.
static Type* adjustProcedureTypeToHaveLineNumbers(Type* t) {
  if (auto ft = toFunctionType(t)) {
    if (ft->hasForeignLinkage()) INT_FATAL(ft->symbol, "Not handled yet!");
    return ft->getWithLineFileInfo();
  }
  return t;
}

void Pass::process(Symbol* sym) {
  adjustSymbolType(sym, adjustProcedureTypeToHaveLineNumbers);
}
