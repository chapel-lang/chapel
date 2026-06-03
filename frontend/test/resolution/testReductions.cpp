/*
 * Copyright 2021-2026 Hewlett Packard Enterprise Development LP
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
#include "chpl/types/IntType.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

// Test basic sum-reduce
static void test1() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         iter f() {
                           yield 1;
                           yield 2;
                           yield 3;
                         }
                         var x = + reduce f();
                         )"""");
  assert(qt.type());
  assert(qt.type()->isIntType());
  assert(qt.type()->toIntType()->isDefaultWidth());
}

// Test basic or-reduce
static void test2() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         iter f() {
                           yield true;
                           yield false;
                         }
                         var x = || reduce f();
                         )"""");
  assert(qt.type());
  assert(qt.type()->isBoolType());
}

// Tests calling `these` on a record when it's used for a reduction
static void test3() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         record R {
                           iter these() {
                             yield 1;
                             yield 2;
                             yield 3;
                           }
                         }
                         var r: R;
                         var x = + reduce r;
                         )"""");
  assert(qt.type());
  assert(qt.type()->isIntType());
  assert(qt.type()->toIntType()->isDefaultWidth());
}

static void test4() {
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         iter f() {
                           yield 1;
                           yield 2;
                           yield 3;
                         }
                         class AnyEvenReduceScanOp: ReduceScanOp {
                           type eltType;
                           var value: bool;

                           // Rely on the default value of the desired type.
                           // Todo: is this efficient when that is an array?
                           inline proc identity {
                             return false;
                           }
                           inline proc accumulate(x) {
                             value = value || (x % 2 == 0);
                           }
                           inline proc accumulateOntoState(ref state, x) {
                             state = state || (x % 2 == 0);
                           }
                           inline proc combine(x) {
                             value = value || x.value;
                           }
                           inline proc generate() do return value;
                           inline proc clone() do return new unmanaged AnyEvenReduceScanOp(eltType=eltType);
                         }
                         var x = AnyEvenReduceScanOp reduce f();
                         )"""");
  assert(qt.type());
  assert(qt.type()->isBoolType());
}

// test 'minloc' and 'maxloc' reductions
static void test5() {
  auto context = buildStdContext();
  auto qts = resolveTypesOfVariablesInit(context,
                        R""""(
                        iter f() {
                          yield (1, 0);
                          yield (2, 1);
                          yield (3, 2);
                        }
                        var x = maxloc reduce f();
                        var y = minloc reduce f();
                        )"""", {"x", "y"});
  for (auto& pair : qts) {
    auto& qt = pair.second;
    assert(qt.type());
    assert(qt.type()->isTupleType());
    assert(qt.type()->toTupleType()->numElements() == 2);
    assert(qt.type()->toTupleType()->elementType(0).type()->isIntType());
    assert(qt.type()->toTupleType()->elementType(1).type()->isIntType());
  }
}

// test 'zip' forms for reductions (serial and parallel)
static void test6() {
  auto context = buildStdContext();
  auto qts = resolveTypesOfVariablesInit(context,
                        R""""(
                        iter f() { yield 1; yield 2; yield 3; }

                        var x = maxloc reduce zip(1..10, 1..10);
                        var y = maxloc reduce zip(f(), f());
                        )"""", {"x", "y"});
  for (auto& pair : qts) {
    auto& qt = pair.second;
    assert(qt.type());
    assert(qt.type()->isTupleType());
    assert(qt.type()->toTupleType()->numElements() == 2);
    assert(qt.type()->toTupleType()->elementType(0).type()->isIntType());
    assert(qt.type()->toTupleType()->elementType(1).type()->isIntType());
  }
}

// test 'reduce' with parallel-only iterators
static void test7() {
  auto context = buildStdContext();
  auto qts = resolveTypesOfVariablesInit(context,
                        R""""(
                        iter f(param tag) where tag == iterKind.standalone { yield 1; yield 2; yield 3; }

                        var x = + reduce f();
                        )"""", {"x"});
  for (auto& pair : qts) {
    auto& qt = pair.second;
    assert(qt.type());
    assert(qt.type()->isIntType());
  }
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
}
