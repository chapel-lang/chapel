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
#include "chpl/uast/all-uast.h"

// assumes the last statement is a variable declaration for x
// with an initialization expression.
// Returns the type of the initializer expression.

static void test1() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  QualifiedType qt = resolveTypeOfXInit(context, "var x = true || f();");
  assert(qt.isParamTrue());
}

static void test2() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  QualifiedType qt = resolveTypeOfXInit(context, "var x = false && f();");
  assert(qt.isParamFalse());
}

static void test3() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // 2nd argument param should not be folded
  QualifiedType qt = resolveTypeOfXInit(context,
                                      "var a: bool; var x = a || true;");
  assert(!qt.isParam() && !qt.hasParamPtr());
  assert(qt.type() == BoolType::get(context));
}

static void test4() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // 2nd argument param should not be folded
  QualifiedType qt = resolveTypeOfXInit(context,
                                      "var a: bool; var x = a && false;");
  assert(!qt.isParam() && !qt.hasParamPtr());
  assert(qt.type() == BoolType::get(context));
}

static void test5() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // both args are params, should make a param init expr
  QualifiedType qt = resolveTypeOfXInit(context,
                                      "var x = true && false;");
  assert(qt.isParamFalse());
}

static void test6() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // both args are params, should make a param init expr
  QualifiedType qt = resolveTypeOfXInit(context,
                                      "var x = false || true;");
  assert(qt.isParamTrue());
}

static void test7() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // both args are the (or) identity params, should make param false.
  QualifiedType qt = resolveTypeOfXInit(context,
                                      "var x = false || false;");
  assert(qt.isParamFalse());
}

static void test8() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // both args are the (and) identity params, should make param true.
  QualifiedType qt = resolveTypeOfXInit(context,
                                      "var x = true && true;");
  assert(qt.isParamTrue());
}

static void test9() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // the type of y is unknown, so the whole type is unknown.
  QualifiedType qt = resolveTypeOfXInit(context,
                                      "var x = true && y;");
  assert(qt.isUnknown());

  assert(guard.numErrors() > 0);
  guard.realizeErrors();
}

static void test10() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // the type of y is unknown, so the whole type is unknown.
  QualifiedType qt = resolveTypeOfXInit(context,
                                      "var x = false || y;");
  assert(qt.isUnknown());

  assert(guard.numErrors() > 0);
  guard.realizeErrors();
}

static void testUnary() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string program = R"""(
    param val = false;

    record R {
      proc type foo(x:int) {
        return x;
      }
    }

    if __primitive("u!", val) {
      var x = R.foo(5);
    } else {
      var x = R.foo("hello");
    })""";

  auto m = parseModule(context, program);
  std::ignore = resolveModule(context, m->id());
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();

  testUnary();

  return 0;
}
