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

namespace {
  // As a means of abbreviation.
  using Pass = WidenComponentsOfProcPtrTypes;
}

bool Pass::shouldProcess(Symbol* sym) {
  if (isTypeSymbol(sym)) return false;
  return isFnSymbol(sym) || isFunctionType(sym->type);
}

// This is used by "adjustSymbolType". It will replace any procedure type
// with "widenable" components (e.g., 'ref' or 'class' formals) with a new
// type where those components are wide.
static Type* adjustProcedureTypeToHaveWidenedComponents(Type* t) {
  if (auto ft = toFunctionType(t)) {
    if (!ft->hasForeignLinkage()) {
      return ft->getWithWidenedComponents();
    }
  }
  return t;
}

void Pass::process(Symbol* sym) {
  auto adjustTypeFn = adjustProcedureTypeToHaveWidenedComponents;
  bool shouldAdjustSymbolType = true;

  if (auto fn = toFnSymbol(sym)) {
    if (!fn->isUsedAsValue()) {
      // If the function is not used as a value then do not adjust it.
      shouldAdjustSymbolType = false;

      // And clear the type, it can be recomputed later if needed.
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
    maybeAdjustSymbolType(sym, adjustTypeFn);
  }
}
