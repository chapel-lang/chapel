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
  return shouldProcessIfNonForeignLinkage(sym);
}

Type* Pass::computeAdjustedType(Type* t) const {
  if (auto ft = toFunctionType(t->getValType())) {
    auto [line, file] = FunctionType::constructLineFileInfoFormals();

    if (ft->numFormals() >= 2) {
      if (*(ft->formals().end() - 1) == file &&
          *(ft->formals().end() - 2) == line) {
        // TODO: This is just a heuristic, but it's the best we can do at
        //       the moment. To truly make this operation idempotent, we
        //       need to address the TODO in 'runPassOverAllSymbols' or
        //       add extra information to 'FunctionType' to let us detect
        //       if this has already been done.
        //
        // Do not change if the formals match the line/file formals.
        return t;
      }
    }

    // Otherwise, compute the new type and match the ref level.
    auto ret = ft->getWithLineFileInfo();

    return ret;
  }

  return t;
}
