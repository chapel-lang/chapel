/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#include "TemporaryConversionThunk.h"

#include "AstVisitor.h"
#include "global-ast-vecs.h"

TemporaryConversionThunk::TemporaryConversionThunk() : Expr(E_TemporaryConversionThunk) {
  children.parent = this;
  gTemporaryConversionThunks.push_back(this);
}

void TemporaryConversionThunk::verify() {
}

void TemporaryConversionThunk::accept(AstVisitor* visitor) {
  if (visitor->enterThunk(this)) {
    for_alist(child, children) {
      child->accept(visitor);
    }
    visitor->exitThunk(this);
  }
}

TemporaryConversionThunk* TemporaryConversionThunk::copyInner(SymbolMap* map) {
  INT_FATAL("thunks should not be copied");
  return nullptr;
}

Expr* TemporaryConversionThunk::getFirstExpr() {
  return children.first();
}
GenRet TemporaryConversionThunk::codegen() {
  INT_FATAL("should not be reached");
  return GenRet();
}
