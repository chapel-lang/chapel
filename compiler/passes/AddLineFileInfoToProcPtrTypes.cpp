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
#include "symbol.h"
#include "type.h"

namespace {
  // As a means of abbreviation.
  using Pass = AddLineFileInfoToProcPtrTypes;
}

bool Pass::shouldProcess(Symbol* sym) {
  if (isTypeSymbol(sym)) return false;
  return isFnSymbol(sym) || isFunctionType(sym->type);
}

// This is used by "adjustSymbolType". Note that all the type constructor
// does is append the line/file formals to the end of the type - there is
// no check to see if they have already been appended.
static Type* adjustProcedureTypeToHaveLineNumbers(Type* t) {
  if (auto ft = toFunctionType(t)) {
    if (!ft->hasForeignLinkage()) {
      return ft->getWithLineFileInfo();
    }
  }

  return t;
}

void Pass::process(Symbol* sym) {
  auto adjustTypeFn = adjustProcedureTypeToHaveLineNumbers;
  bool shouldAdjustSymbolType = true;

  if (auto fn = toFnSymbol(sym)) {
    if (!fn->isUsedAsValue()) {
      // If the function is not used as a value then its type should not be
      // adjusted. Clear its type - it can be recomputed later if needed.
      shouldAdjustSymbolType = false;
      fn->type = dtUnknown;
    }

    // As a special case, we have to handle the 'fn->retType' separately.
    fn->retType = adjustTypeFn(fn->retType);
  }

  if (isTypeSymbol(sym)) {
    // Don't adjust type symbols!
    shouldAdjustSymbolType = false;
  }

  if (shouldAdjustSymbolType) {
    adjustSymbolType(sym, adjustTypeFn);
  }
}
