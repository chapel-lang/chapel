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
#include "resolution.h"
#include "symbol.h"
#include "type.h"

namespace {
  // As a means of abbreviation.
  using Pass = AdjustProcPtrTypes;
}

bool Pass::shouldProcessDefault(Symbol* sym) {
  if (auto fn = toFnSymbol(sym)) {
    if (fn->isUsedAsValue()) return true;

    // Otherwise, process the function only if you should process its RVV.
    auto rvv = fn->getReturnSymbol();
    return rvv && shouldProcessDefault(rvv);
  }

  return isFunctionType(sym->type);
}

bool Pass::shouldProcessIfNonForeignLinkage(Symbol* sym) {
  if (auto fn = toFnSymbol(sym)) {
    if (fn->isUsedAsValue() && !fn->hasForeignLinkage()) return true;

    // Otherwise, process the function only if you should process its RVV.
    auto rvv = fn->getReturnSymbol();
    return rvv && shouldProcessIfNonForeignLinkage(rvv);
  }

  if (auto ft = toFunctionType(sym->type)) {
    return !ft->hasForeignLinkage();
  }

  return false;
}

bool Pass::shouldProcess(Symbol* sym) {
  return shouldProcessDefault(sym);
}

void Pass::process(Symbol* sym) {
  // TODO: The 'adjustSymbolType' should be made to take a template arg.
  //       That way, we can avoid the cost of constructing std::function.
  AdjustTypeFn adjustTypeFn = [this](Type* t) {
    Type* ret = t;
    if (auto ft = toFunctionType(t)) {
      ret = this->computeAdjustedType(ft);
    }
    return ret;
  };

  // Store the type before and after adjusting the symbol's type.
  Type* oldType = sym->type;
  Type* newType = maybeAdjustSymbolType(sym, adjustTypeFn);

  if (isTypeSymbol(sym) && oldType != newType) {
    // Type symbols require some special handling. They're the one class of
    // symbol where it doesn't actually make any sense to set 'sym->type'.
    // Instead, if they "changed", what we have to do is walk all the uses
    // of the type symbol and point them to the new type.
    for_SymbolSymExprs(se, sym) {
      enqueue(newType->symbol, se);
    }

  } else {
    INT_ASSERT(sym->type == newType);
  }

  if (sym->hasFlag(FLAG_RVV)) {
    // The adjust function only makes references types as needed, but the
    // '--verify' pass will expect every return type to have a 'ref' type.
    if (!sym->type->refType) makeRefType(sym->type);
  }
}

void Pass::process(Symbol* newSymbol, SymExpr* oldUse) {
  oldUse->setSymbol(newSymbol);
}
