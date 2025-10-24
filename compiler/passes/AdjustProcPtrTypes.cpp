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
  using Pass = AdjustProcPtrTypes;
}

bool Pass::shouldProcess(Symbol* sym) {
  if (isFnSymbol(sym)) return true;
  if (auto ft = toFunctionType(sym->type)) {
    return !ft->hasForeignLinkage();
  }
  return false;
}

void Pass::process(Symbol* sym) {
  bool shouldAdjustSymbolType = true;

  // TODO: The 'adjustSymbolType' should be made to take a template arg.
  //       That way, we can avoid the cost of constructing std::function.
  AdjustTypeFn adjustTypeFn = [this](Type* t) {
    Type* ret = t;
    if (auto ft = toFunctionType(t)) {
      ret = this->computeAdjustedType(ft);
    }
    return ret;
  };

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

    auto oldType = sym->type;
    auto newType = adjustTypeFn(oldType);

    if (newType != oldType) {
      // But we do need to adjust uses to point to new types if appropriate.
      // TODO: Is there a step to compute uses or are these just omnipresent?
      for_SymbolSymExprs(se, sym) {
        // Retarget a use of the old type to point to the new type.
        enqueue(newType->symbol, se);
      }
    }
  }

  if (shouldAdjustSymbolType) {
    adjustSymbolType(sym, adjustTypeFn);
  }
}

void Pass::process(Symbol* newSymbol, SymExpr* oldUse) {
  oldUse->setSymbol(newSymbol);
}
