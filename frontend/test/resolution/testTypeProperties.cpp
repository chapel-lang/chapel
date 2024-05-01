/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

// Primitive/builtin types that do default initialize.
static void test1() {
  Context context;
  auto ctx = &context;
  {
    auto t = BoolType::get(ctx);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = IntType::get(ctx, 0);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = IntType::get(ctx, 8);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = IntType::get(ctx, 16);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = IntType::get(ctx, 32);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = IntType::get(ctx, 64);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = RealType::get(ctx, 0);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = ComplexType::get(ctx, 64);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = ImagType::get(ctx, 64);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = CompositeType::getStringType(ctx);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = CompositeType::getBytesType(ctx);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = CompositeType::getRangeType(ctx);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = CompositeType::getLocaleType(ctx);
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    auto t = AnyIntegralType::get(ctx);
    assert(!isTypeWithDefaultValue(ctx, t));
  }
  {
    ctx->advanceToNextRevision(false);
    auto t = resolveTypeOfX(ctx,
                           R""""(
                           class C {}
                           record r { var f: owned C?; }
                           var x: r; 
                           )"""");
    assert(t && t->isRecordType());
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    ctx->advanceToNextRevision(false);
    auto t = resolveTypeOfX(ctx,
                           R""""(
                           class C {}
                           record r { var f: owned C; }
                           var x: r; 
                           )"""");
    assert(t && t->isRecordType());
    assert(!isTypeWithDefaultValue(ctx, t));
  }
  {
    ctx->advanceToNextRevision(false);
    auto t = resolveTypeOfX(ctx,
                           R""""(
                           class C {}
                           record r { var f: owned C = new owned C(); }
                           var x: r; 
                           )"""");
    assert(t && t->isRecordType());
    assert(isTypeWithDefaultValue(ctx, t));
  }

  {
    ctx->advanceToNextRevision(false);
    auto t = resolveTypeOfX(ctx,
                           R""""(
                           class C {}
                           // Simulate having a internal module '='.
                           operator =(ref lhs: owned, ref rhs: owned) {}
                           record r {
                             var f: owned C;
                             proc init() { this.f = new owned C(); }
                           }
                           var x: r; 
                           )"""");
    assert(t && t->isRecordType());
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    ctx->advanceToNextRevision(false);
    auto t = resolveTypeOfX(ctx,
                           R""""(
                           class C {}
                           var x: owned C?; 
                           )"""");
    assert(t && t->isClassType());
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    ctx->advanceToNextRevision(false);
    auto t = resolveTypeOfX(ctx,
                           R""""(
                           class C {}
                           var x: shared C?; 
                           )"""");
    assert(t && t->isClassType());
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    ctx->advanceToNextRevision(false);
    auto t = resolveTypeOfX(ctx,
                           R""""(
                           class C {}
                           var x: borrowed C?; 
                           )"""");
    assert(t && t->isClassType());
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    ctx->advanceToNextRevision(false);
    auto t = resolveTypeOfX(ctx,
                           R""""(
                           class C {}
                           var x: unmanaged C?; 
                           )"""");
    assert(t && t->isClassType());
    assert(isTypeWithDefaultValue(ctx, t));
  }
  {
    ctx->advanceToNextRevision(false);
    auto t = resolveTypeOfX(ctx,
                           R""""(
                           class C {}
                           var x: owned C = new owned C(); 
                           )"""");
    assert(t && t->isClassType());
    assert(!isTypeWithDefaultValue(ctx, t));
  }
  {
    ctx->advanceToNextRevision(false);
    auto t = resolveTypeOfX(ctx,
                           R""""(
                           class C {}
                           var x: shared C = new shared C(); 
                           )"""");
    assert(t && t->isClassType());
    assert(!isTypeWithDefaultValue(ctx, t));
  }
  {
    ctx->advanceToNextRevision(false);
    auto t = resolveTypeOfX(ctx,
                           R""""(
                           class C {}
                           var x: unmanaged C = new unmanaged C(); 
                           )"""");
    assert(t && t->isClassType());
    assert(!isTypeWithDefaultValue(ctx, t));
  }
}

int main() {
    test1();
    return 0;
}
