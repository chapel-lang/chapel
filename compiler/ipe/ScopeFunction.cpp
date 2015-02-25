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

#include "ScopeFunction.h"

#include "stmt.h"
#include "symbol.h"

ScopeFunction::ScopeFunction(FnSymbol* fn, const ScopeBase* parent) : ScopeBase(parent)
{
  mFunction = fn;

  for (int i = 1; i <= fn->formals.length; i++)
  {
    DefExpr*   defExpr = 0;
    ArgSymbol* formal  = 0;

    defExpr = toDefExpr(fn->formals.get(i));
    INT_ASSERT(defExpr);

    formal = toArgSymbol(defExpr->sym);

    INT_ASSERT(formal);

    addSym(defExpr->sym);
  }
}

ScopeFunction::~ScopeFunction()
{

}

void ScopeFunction::describeHeader(FILE* fp) const
{
  fprintf(fp, "Function %s", mFunction->name);
}
