/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

static void test1() {
  // make sure that function return type computation does not throw
  // away the param.
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         proc p(param x: int(64), param y: int(64)) param do return __primitive("+", x, y);

                         param x = p(1,2);
                         )"""");
  assert(qt.kind() == QualifiedType::PARAM);
  auto typePtr = qt.type();
  auto paramPtr = qt.param();
  assert(typePtr && typePtr->isIntType());
  assert(paramPtr && paramPtr->isIntParam() && paramPtr->toIntParam()->value() == 3);
}

static void test2() {
  // make sure unknown types don't slip into candidate search
  auto context = buildStdContext();
  QualifiedType qt =  resolveTypeOfXInit(context,
                         R""""(
                         proc test(z) {}
                         var x = test(y);
                         )"""", false);
  assert(qt.isUnknown());
}

static void helpTest3(const std::string& theFunction) {
  // Make sure that types depending on earlier actual types are properly
  // enforced
  {
    auto context = buildStdContext();
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(true, "hello", "world");
                                 )"""");
    assert(qt.type() != nullptr);
    assert(qt.type()->isStringType());
  }
  {
    auto context = buildStdContext();
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(false, 0, 1);
                                 )"""");
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());
  }
  {
    auto context = buildStdContext();
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(true, 0, 1);
                                 )"""");
    assert(qt.isUnknown());
  }
  {
    auto context = buildStdContext();
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(false, "hello", "world");
                                 )"""");
    assert(qt.isUnknown());
  }
  {
    auto context = buildStdContext();
    auto qt = resolveTypeOfXInit(context, theFunction +
                                 R""""(
                                 var x = f(true, "hello", 0);
                                 )"""");
    assert(qt.isUnknown());
  }
}

static void test3a() {
  const std::string theFunction =
    R""""(
    proc f(param a: bool,
           const ref b: if a then string else int,
           const ref c: b.type) const ref {
      return c;
    })"""";
  helpTest3(theFunction);
}

static void test3b() {
  const std::string theFunction =
    R""""(
    proc f(param a: bool,
           b: if a then string else int,
           c: b.type) {
      return c;
    })"""";
  helpTest3(theFunction);
}

// Test calling dependently typed type constructor, for type with param field
static void test4() {
  // With param field type declared
  {
    printf("part 1\n");
    auto context = buildStdContext();
    ErrorGuard guard(context);

    const std::string program =
      R""""(
      record Foo {
        param fooStrides : int;
      }
      proc bar(param strides = 1, other: Foo(strides)) {
        return other.fooStrides;
      }
      var x = bar(1, new Foo(1));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    for (auto& e : guard.errors()) std::cout << e->message() << std::endl;
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());

    context->collectGarbage();
  }

  // Param field of generic type
  {
    printf("part 2\n");
    auto context = buildStdContext();

    const std::string program =
      R""""(
      record Foo {
        param fooStrides;
      }
      proc bar(param strides = 1, other: Foo(strides)) {
        return other.fooStrides;
      }
      var x = bar(1, new Foo(1));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());

    context->collectGarbage();
  }

  // With param formal type declared
  {
    printf("part 3\n");
    auto context = buildStdContext();

    const std::string program =
      R""""(
      record Foo {
        param fooStrides : int;
      }
      proc bar(param strides : int, other: Foo(strides)) {
        return other.fooStrides;
      }
      var x = bar(1, new Foo(1));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());

    context->collectGarbage();
  }
}

// Test calling dependently typed type constructor, in a type formal
static void test5() {
  // With param field type declared
  {
    printf("part 1\n");
    auto context = buildStdContext();
    ErrorGuard guard(context);

    const std::string program =
      R""""(
      record Foo {
        param fooField : int;
      }
      proc bar(param strides = 1, type other: Foo(strides)) param {
        return other.fooField;
      }
      var x = bar(1, Foo(1));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    for (auto& e : guard.errors()) std::cout << e->message() << std::endl;
    assert(qt.type() != nullptr);
    ensureParamInt(qt, 1);

    context->collectGarbage();
  }

  // Param field of generic type
  {
    printf("part 2\n");
    auto context = buildStdContext();
    ErrorGuard guard(context);

    const std::string program =
      R""""(
      record Foo {
        param fooField;
      }
      proc bar(param strides = 1, type other: Foo(strides)) param {
        return other.fooField;
      }
      var x = bar(1, Foo(1));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    for (auto& e : guard.errors()) std::cout << e->message() << std::endl;
    assert(qt.type() != nullptr);
    ensureParamInt(qt, 1);

    context->collectGarbage();
  }

  // type field
  {
    printf("part 3\n");
    auto context = buildStdContext();
    ErrorGuard guard(context);

    const std::string program =
      R""""(
      record Foo {
        type fooField;
      }
      proc bar(x, type other: Foo(x.type)) type {
        return other.fooField;
      }
      type x = bar(1, Foo(int));
      )"""";

    auto qt = resolveTypeOfXInit(context, program);
    for (auto& e : guard.errors()) std::cout << e->message() << std::endl;
    assert(qt.type() != nullptr);
    assert(qt.type()->isIntType());
    assert(qt.kind() == QualifiedType::TYPE);

    context->collectGarbage();
  }
}

int main() {
  test1();
  test2();
  test3a();
  test3b();
  test4();
  test5();

  return 0;
}
