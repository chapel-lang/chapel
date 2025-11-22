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

#include "astutil.h"
#include "baseAST.h"
#include "CallExpr.h"
#include "FnSymbol.h"
#include "passes.h"
#include "resolution.h"
#include "symbol.h"
#include "type.h"

namespace {
  // As a means of abbreviation.
  using Pass = AdjustSymbolTypes;
}

bool Pass::shouldProcessDefault(Symbol* sym) {
  if (auto fn = toFnSymbol(sym)) {
    // Process because the functions's type has semantic importance.
    if (fn->isUsedAsValue()) return true;

    // Otherwise, process the function only if you should process its RVV.
    auto rvv = fn->getReturnSymbol();
    return rvv && shouldProcessDefault(rvv);
  }

  return true;
}

bool Pass::shouldProcessIfNonForeignLinkage(Symbol* sym) {
  if (auto fn = toFnSymbol(sym)) {
    if (fn->isUsedAsValue() && !fn->hasForeignLinkage()) return true;

    // Otherwise, process the function only if you should process its RVV.
    auto rvv = fn->getReturnSymbol();
    return rvv && shouldProcessIfNonForeignLinkage(rvv);
  }

  if (auto ft = toFunctionType(sym->type->getValType())) {
    return !ft->hasForeignLinkage();
  }

  bool ret = !sym->hasFlag(FLAG_EXTERN) &&
             !sym->hasFlag(FLAG_EXPORT);

  return ret;
}

bool Pass::shouldProcess(Symbol* sym) {
  return shouldProcessDefault(sym);
}

void Pass::process(Symbol* sym) {
  // TODO: The 'maybeAdjustSymbolType' should be made to take a template arg.
  //       That way, we can avoid the cost of constructing std::function. Or,
  //       we can migrate the entirety of the code to live in this file.
  AdjustTypeFn adjustTypeFn = [this](Type* t) {
    return this->computeAdjustedType(t);
  };

  // Store the type before and after adjusting the symbol's type.
  Type* oldType = sym->type;
  Type* newType = maybeAdjustSymbolType(sym, adjustTypeFn);
  Type* oldVt = oldType->getValType();
  Type* newVt = newType->getValType();
  bool changed = oldType != newType;
  bool wasSet = sym->type != oldType;

  INT_ASSERT(!wasSet || sym->type == newType);

  if (fVerify && changed) {
    // TODO: This does not necessarily always need to be the case, but for
    //       now it seems to be, so crash if it's not so.
    INT_ASSERT(!oldType->symbol->hasFlag(FLAG_REF) ||
                newType->symbol->hasFlag(FLAG_REF));
    INT_ASSERT(!oldType->symbol->hasFlag(FLAG_WIDE_REF) ||
                newType->symbol->hasFlag(FLAG_WIDE_REF));
  }

  if (isTypeSymbol(sym) && changed) {
    // Type symbols require some special handling. They're the one class of
    // symbol where it doesn't actually make any sense to set 'sym->type'.
    // Instead, if they "changed", what we have to do is walk all the uses
    // of the type symbol and point them to the new type.
    for_SymbolSymExprs(se, sym) {
      enqueue(newType->symbol, se);
    }
  }

  if (newVt != dtUnknown) {
    if (changed && oldVt->refType && !newVt->refType) {
      // Make sure that the type has a ref type, if needed.
      makeRefType(newType->getValType());
    }

    if (changed && oldVt->getWideRefType() && !newVt->getWideRefType()) {
      // Make sure that the type has a wide-ref type, if needed.
      auto refT = newVt->getRefType();
      INT_ASSERT(refT);

      SET_LINENO(refT->symbol);
      std::ignore = getOrMakeWideTypeDuringCodegen(refT);
      INT_ASSERT(newVt->getWideRefType());
    }
  }
}

void Pass::process(Symbol* newSymbol, SymExpr* oldUse) {
  oldUse->setSymbol(newSymbol);
}
